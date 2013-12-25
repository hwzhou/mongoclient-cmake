CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

# Set common definitions.
IF (MONGOCLIENT_PLATFORM_DEFINITIONS)
    SET(EXAPLES_DEFINITIONS "${MONGOCLIENT_PLATFORM_DEFINITIONS}")
ENDIF ()

IF (MongoClient_BOOST_TIME_UTC_HACK)
    IF (EXAPLES_DEFINITIONS)
        SET(EXAPLES_DEFINITIONS "${EXAPLES_DEFINITIONS};")
    ENDIF ()
    SET(EXAPLES_DEFINITIONS 
        "${EXAPLES_DEFINITIONS}MONGO_BOOST_TIME_UTC_HACK"
    )
ENDIF ()

# Set definitions for OpenSSL.
IF (OPENSSL_FOUND)
    IF (EXAPLES_DEFINITIONS)
        SET(EXAPLES_DEFINITIONS "${EXAPLES_DEFINITIONS};")
    ENDIF ()
    SET(EXAPLES_DEFINITIONS 
        "${EXAPLES_DEFINITIONS}MONGO_SSL"
    )
ENDIF ()

# Set definitions for boost.
IF (NOT Boost_USE_STATIC_LIBS)
    IF (EXAPLES_DEFINITIONS)
        SET(EXAPLES_DEFINITIONS "${EXAPLES_DEFINITIONS};")
    ENDIF ()
    SET(EXAPLES_DEFINITIONS 
        "${EXAPLES_DEFINITIONS}BOOST_ALL_DYN_LINK"
    )
ENDIF ()

# Set other definitions.
IF (MSVC)
    IF (EXAPLES_DEFINITIONS)
        SET(EXAPLES_DEFINITIONS "${EXAPLES_DEFINITIONS};")
    ENDIF ()
    SET(EXAPLES_DEFINITIONS "${EXAPLES_DEFINITIONS}_CRT_SECURE_NO_WARNINGS")
ENDIF ()

SET(EXAMPLES_STATIC_DEFINITIONS "${EXAPLES_DEFINITIONS}")
SET(EXAMPLES_SHARED_DEFINITIONS "${EXAPLES_DEFINITIONS}")

IF (MSVC)
    IF (EXAMPLES_SHARED_DEFINITIONS)
        SET(EXAMPLES_SHARED_DEFINITIONS "${EXAMPLES_SHARED_DEFINITIONS};")
    ENDIF ()
    SET(EXAMPLES_SHARED_DEFINITIONS "${EXAMPLES_SHARED_DEFINITIONS}LIBMONGOCLIENT_CONSUMER")
ENDIF ()

SET(MONGOCLIENT_LIBS "")

IF (MSVC)
    SET(MONGOCLIENT_LIBS
        ${MONGOCLIENT_LIBS}
        ws2_32
        dbghelp
        psapi
    )
ELSE ()
    SET(MONGOCLIENT_LIBS
        ${Boost_LIBRARIES}
        pthread
    )
ENDIF ()

SET(MONGOCLIENT_STATIC_LIBS
    ${MONGOCLIENT_LIBS}
    "$<TARGET_LINKER_FILE_DIR:mongoclient_static>/$<TARGET_LINKER_FILE_NAME:mongoclient_static>"
)

SET(MONGOCLIENT_SHARED_LIBS
    ${MONGOCLIENT_LIBS}
    "$<TARGET_LINKER_FILE_DIR:mongoclient_shared>/$<TARGET_LINKER_FILE_NAME:mongoclient_shared>"
)

IF (OPENSSL_FOUND)
    SET(MONGOCLIENT_STATIC_LIBS
        ${MONGOCLIENT_STATIC_LIBS}
        ${OPENSSL_LIBRARIES}
    )

    SET(MONGOCLIENT_SHARED_LIBS
        ${MONGOCLIENT_SHARED_LIBS}
        ${OPENSSL_LIBRARIES}
    )
ENDIF ()

# Add static and shared examples.
FUNCTION (MongoClient_BUILD_EXAMPLES EXAMPLE_NAME)
    SET(EXAMPLE_STATIC "${EXAMPLE_NAME}_static")

    ADD_EXECUTABLE(${EXAMPLE_STATIC} ${ARGN})

    SET_TARGET_PROPERTIES(${EXAMPLE_STATIC} PROPERTIES
                          OUTPUT_NAME "${EXAMPLE_NAME}_s"
                          DEBUG_OUTPUT_NAME "${EXAMPLE_NAME}_sd"
                          COMPILE_DEFINITIONS "${EXAMPLES_STATIC_DEFINITIONS}"
    )

    IF (MSVC)
        SET_TARGET_PROPERTIES(${EXAMPLE_STATIC} PROPERTIES
                              COMPILE_OPTIONS "/wd4996;/wd4244;/wd4800;/wd4251;/wd4267"
        )
    ENDIF ()

    ADD_DEPENDENCIES(${EXAMPLE_STATIC} mongoclient_static)

    TARGET_LINK_LIBRARIES(${EXAMPLE_STATIC}
                          ${MONGOCLIENT_STATIC_LIBS}
    )

    SET(EXAMPLE_SHARED "${EXAMPLE_NAME}_shared")

    ADD_EXECUTABLE(${EXAMPLE_SHARED} ${ARGN})

    SET_TARGET_PROPERTIES(${EXAMPLE_SHARED} PROPERTIES
                          OUTPUT_NAME ${EXAMPLE_NAME}
                          DEBUG_OUTPUT_NAME "${EXAMPLE_NAME}d"
                          COMPILE_DEFINITIONS "${EXAMPLES_SHARED_DEFINITIONS}"
    )

    IF (MSVC)
        SET_TARGET_PROPERTIES(${EXAMPLE_SHARED} PROPERTIES
                              COMPILE_OPTIONS "/wd4996;/wd4244;/wd4800;/wd4251;/wd4267"
        )
    ENDIF ()

    ADD_DEPENDENCIES(${EXAMPLE_SHARED} mongoclient_shared)

    TARGET_LINK_LIBRARIES(${EXAMPLE_SHARED}
                          ${MONGOCLIENT_SHARED_LIBS}
    )
ENDFUNCTION ()
