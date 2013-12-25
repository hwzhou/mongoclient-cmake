CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(MONGOCLIENT)

# Uses static BOOST library or not?
OPTION(Boost_USE_STATIC_LIBS "Use static boost library?" OFF)

# Define "MONGO_BOOST_TIME_UTC_HACK" or not?
IF (NOT MSVC)
    OPTION(MongoClient_BOOST_TIME_UTC_HACK "Need to define MONGO_BOOST_TIME_UTC_HACK?" OFF)
ENDIF ()

# Build examples or not?
OPTION(MongoClient_BUILD_EXAMPLES "Build examples?" OFF)

# Only support Win7 and later?
IF (MSVC)
    OPTION(MongoClient_ONLY_WIN7_AND_LATER "Only support Win7 and later?" OFF)
ENDIF ()

# Find boost.
SET(NEEDED_BOOST_COMPONENTS
    date_time
    filesystem
    program_options
    system
    thread
)

IF (Boost_INCLUDE_DETAIL)
    SET(NEEDED_BOOST_COMPONENTS ${NEEDED_BOOST_COMPONENTS} detail)
ENDIF ()

FIND_PACKAGE(Boost 1.49.0
             COMPONENTS ${NEEDED_BOOST_COMPONENTS}
)

# Find OpenSSL.
FIND_PACKAGE(OpenSSL)

# Check if the compiler supports newest boost distributions.
IF (MSVC)
    IF (MSVC_VERSION LESS 1400)
        # VC6, VC7, VC71 don't support newest boost distributions.
        SET(Boost_LAGACY_COMPILER ON)
    ENDIF ()
ELSE ()
    # Need to check other compilers.
ENDIF ()

# Check boost configurations.
IF (Boost_INCLUDE_DIR)
    SET(MongoClient_BOOST_VERSION "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}")
    IF (Boost_SUBMINOR_VERSION)
        SET(MongoClient_BOOST_VERSION "${MongoClient_BOOST_VERSION}.${Boost_SUBMINOR_VERSION}")
    ENDIF ()

    MESSAGE(STATUS "Found boost: " ${MongoClient_BOOST_VERSION})

    IF (MSVC)
        IF (Boost_LAGACY_COMPILER AND Boost_MINOR_VERSION GREATER 49)
            # VC6, VC7, VC71 need boost version 1.49 or older.
            MESSAGE(STATUS "VC6, VC7, VC71 can't use boost newer than 1.49. "
                            "Use builtin boost library instead. See http://www.boost.org."
            )
            
            SET(Boost_MAJOR_VERSION 1)
            SET(Boost_MINOR_VERSION 49)
            SET(Boost_SUBMINOR_VERSION 0)

            SET(Boost_USE_BUILTINS ON)
        ELSEIF (MSVC12 AND Boost_MINOR_VERSION LESS 55)
            # VC12 needs boost version 1.55 or newer.
            MESSAGE(STATUS "VC12 is not supported by boost older than 1.55. "
                            "Use builtin boost library instead. See http://www.boost.org."
            )

            SET(Boost_MAJOR_VERSION 1)
            SET(Boost_MINOR_VERSION 55)
            SET(Boost_SUBMINOR_VERSION 0)

            SET(Boost_USE_BUILTINS ON)
        ENDIF ()
    ELSE (MSVC)
        # Need to check other compilers.
    ENDIF (MSVC)
ELSE (Boost_INCLUDE_DIR)
    # boost not found.
    SET(Boost_USE_BUILTINS ON)
ENDIF (Boost_INCLUDE_DIR)

# Check to see if we need to use builtin boost.
IF (Boost_USE_BUILTINS)
    # Normally we use builtin boost version 1.55.
    SET(Boost_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third_party/boost_1_55_0)

    IF (Boost_LAGACY_COMPILER)
        # For legacy compilers, we need to use builtin boost version 1.49.
        SET(Boost_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third_party/boost_1_49_0)
    #ELSEIF (MSVC10 OR MSVC11)
    #    # For debug purpose only.
    #    SET(Boost_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src/third_party/boost_1_49_0)
    ENDIF ()
ENDIF ()

# Set boost include directory.
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})

# Set OpenSSL include directory.
IF (OPENSSL_FOUND)
    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIR})
ENDIF ()

# Set mongoclient include directories.
INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR}/src . src src/third_party/murmurhash3)

IF (Boost_USE_BUILTINS)
    ADD_SUBDIRECTORY(${Boost_INCLUDE_DIR})
ENDIF ()

# Add boost library directory to link directories.
LINK_DIRECTORIES(${Boost_LIBRARY_DIR})

# Set platform definitions.
IF (MSVC)
    IF (MongoClient_ONLY_WIN7_AND_LATER)
        SET(MONGOCLIENT_PLATFORM_DEFINITIONS
            "_WIN32_WINNT=0x0601;NTDDI_VERSION=0x06010000"
        )
    ELSE (MongoClient_ONLY_WIN7_AND_LATER)
        IF (CMAKE_CL_64)
            # 64-bit.
            SET(MONGOCLIENT_PLATFORM_DEFINITIONS
                "_WIN32_WINNT=0x0502;NTDDI_VERSION=0x05020200"
            )
        ELSE ()
            # 32-bit.
            SET(MONGOCLIENT_PLATFORM_DEFINITIONS
                "_WIN32_WINNT=0x0501;NTDDI_VERSION=0x05010300"
            )
        ENDIF ()
    ENDIF (MongoClient_ONLY_WIN7_AND_LATER)
ELSE()
    SET(MONGOCLIENT_PLATFORM_DEFINITIONS
        "MONGO_HAVE_EXECINFO_BACKTRACE"
    )
ENDIF ()

ADD_SUBDIRECTORY(src/mongo)

# Add examples.
IF (MongoClient_BUILD_EXAMPLES)
    ADD_SUBDIRECTORY(src/mongo/client/examples)
    ADD_SUBDIRECTORY(src/mongo/bson/bsondemo)
ENDIF ()

# Install license.
INSTALL(FILES "APACHE-2.0.txt"
        DESTINATION .
        COMPONENT mongoclient_libraries
)

# Set CPack options.
SET(CPACK_PACKAGE_NAME "MongoClient")
SET(CPACK_PACKAGE_VENDOR "Hongwei Zhou")
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MongoClient - MongoDB Client Libraries")
SET(CPACK_PACKAGE_VERSION "2.4.8")
SET(CPACK_PACKAGE_VERSION_MAJOR "2")
SET(CPACK_PACKAGE_VERSION_MINOR "4")
SET(CPACK_PACKAGE_VERSION_PATCH "8")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "MongoDB Client ${CPACK_PACKAGE_VERSION}")

SET(CPACK_RESOURCE_FILE_LICENSE "${PROJECT_SOURCE_DIR}/APACHE-2.0.txt")

# Set CPack NSIS options.

# Set CPack WIX options.
#SET(CPACK_WIX_PRODUCT_GUID
#    "AB96B2E6-0B65-4C06-85A9-549D3B305B53"
#)
#
#SET(CPACK_WIX_UPGRADE_GUID
#    "960AA2E2-EF99-466A-B974-23D748062491"
#)

# Add CPack support.
INCLUDE(CPack)

# Add component groups.
CPACK_ADD_COMPONENT_GROUP(mongoclient
                          DISPLAY_NAME "Library for MongoDB Client"
                          DESCRIPTION "Library for MongoDB Client"
                          EXPANDED
                          BOLD_TITLE
)

CPACK_ADD_COMPONENT_GROUP(headers
                          DISPLAY_NAME "C++ Headers"
                          DESCRIPTION "C++ Header Files"
                          PARENT_GROUP mongoclient
                          EXPANDED
                          BOLD_TITLE
)

CPACK_ADD_COMPONENT_GROUP(libraries
                          DISPLAY_NAME "Libraries"
                          DESCRIPTION "Libraries"
                          PARENT_GROUP mongoclient
                          EXPANDED
                          BOLD_TITLE
)

# Add components.
CPACK_ADD_COMPONENT(mongoclient_headers
                    DISPLAY_NAME "MongoDB Client C++ Headers"
                    DESCRIPTION "MongoDB Client C++ Header files - 2.4.8"
                    GROUP headers
)

CPACK_ADD_COMPONENT(mongoclient_libraries
                    DISPLAY_NAME "MongoDB Client C++ Libraries"
                    DESCRIPTION "MongoDB Client C++ Libraries - 2.4.8"
                    GROUP libraries
)

IF (Boost_USE_BUILTINS)
    SET(MongoClient_BOOST_VERSION "${Boost_MAJOR_VERSION}.${Boost_MINOR_VERSION}")
    IF (Boost_SUBMINOR_VERSION)
        SET(MongoClient_BOOST_VERSION "${MongoClient_BOOST_VERSION}.${Boost_SUBMINOR_VERSION}")
    ENDIF ()

    CPACK_ADD_COMPONENT(boost_headers
                        DISPLAY_NAME "BOOST C++ Headers"
                        DESCRIPTION "BOOST C++ Header files - ${MongoClient_BOOST_VERSION}"
                        GROUP headers
    )

    CPACK_ADD_COMPONENT(boost_libraries
                        DISPLAY_NAME "BOOST C++ Libraries"
                        DESCRIPTION "BOOST C++ Libraries - ${MongoClient_BOOST_VERSION}"
                        GROUP libraries
    )
ENDIF ()
