CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

IF (MSVC)
    # Add unicode support (needed by "text.h").
    ADD_DEFINITIONS(-DUNICDOE -D_UNICODE)
ELSE ()
    IF (MongoClient_BOOST_TIME_UTC_HACK)
        ADD_DEFINITIONS(-DMONGO_BOOST_TIME_UTC_HACK)
    ENDIF ()
ENDIF ()

SET(MONGOCLIENT_SYSTEM_LIBS "")

IF (MSVC)
    SET(MONGOCLIENT_SYSTEM_LIBS
        ${MONGOCLIENT_SYSTEM_LIBS}
        ws2_32
        dbghelp
        psapi
    )
ENDIF ()

# Generate error_codes & action_types.
ADD_SUBDIRECTORY(base)
ADD_SUBDIRECTORY(db/auth)

# Set mongoclient source files.
SET(MURMURHASH3_SRC 
    ../third_party/murmurhash3/MurmurHash3.cpp
    ../third_party/murmurhash3/MurmurHash3.h
)

SET(SASL_SRC
    #client/sasl_client_authenticate_impl.cpp
    client/sasl_client_authenticate.cpp
    client/sasl_client_authenticate.h
    #client/sasl_client_session.cpp
    #client/sasl_client_session.h
)

IF (WIN32)
    SET(PROCESSINFO_SRC
        util/processinfo_win32.cpp
    )
ELSEIF (APPLE)
    SET(PROCESSINFO_SRC
        util/processinfo_darwin.cpp
    )
ELSE (UNIX)
    IF (CMAKE_SYSTEM_NAME MATCHES "Linux")
        SET(PROCESSINFO_SRC
            util/processinfo_linux2.cpp
        )
    ELSEIF (CMAKE_SYSTEM_NAME MATCHES "FreeBSD")
        SET(PROCESSINFO_SRC
            util/processinfo_freebsd.cpp
        )
    ELSEIF (CMAKE_SYSTEM_NAME MATCHES "SunOS")
        SET(PROCESSINFO_SRC
            util/processinfo_sunos5.cpp
        )
    ELSE ()
        SET(PROCESSINFO_SRC
            util/processinfo_none.cpp
        )
    ENDIF ()
ENDIF ()

SET(ERROR_CODES_SRC 
    ${PROJECT_BINARY_DIR}/src/mongo/base/error_codes.h
    ${PROJECT_BINARY_DIR}/src/mongo/base/error_codes.cpp
)
SET(ACTION_TYPES_SRC
    ${PROJECT_BINARY_DIR}/src/mongo/db/auth/action_type.h
    ${PROJECT_BINARY_DIR}/src/mongo/db/auth/action_type.cpp
)

SET(PCH_SRC pch.h pch.cpp)

SET(BASIC_SRC
    # Base files.
    base/counter.h
    base/disallow_copying.h
    base/global_initializer.cpp
    base/global_initializer.h
    base/global_initializer_registerer.cpp
    base/global_initializer_registerer.h
    #base/init.cpp # Moved to base/initializer.cpp
    base/init.h
    base/initializer.cpp
    base/initializer.h
    base/initializer_context.cpp
    base/initializer_context.h
    base/initializer_dependency_graph.cpp
    base/initializer_dependency_graph.h
    base/initializer_function.h
    base/make_string_vector.cpp
    base/make_string_vector.h
    base/owned_pointer_map.h
    base/owned_pointer_vector.h
    base/parse_number.cpp
    base/parse_number.h
    base/status.cpp
    base/status-inl.h
    base/status.h
    base/status_with.h
    base/string_data.cpp
    base/string_data-inl.h
    base/string_data.h
    # BSON files.
    bson/bson-inl.h
    bson/bson.h
    bson/bson_builder_base.h
    bson/bson_db.h
    bson/bson_field.h
    bson/bson_validate.cpp
    bson/bson_validate.h
    bson/bsonelement.h
    bson/bsonmisc.h
    bson/bsonobj.h
    bson/bsonobjbuilder.h
    bson/bsonobjiterator.h
    bson/bsontypes.h
    bson/inline_decls.h
    bson/oid.cpp
    bson/oid.h
    bson/ordering.h
    bson/util/atomic_int.h
    bson/util/bson_extract.cpp
    bson/util/bson_extract.h
    bson/util/builder.h
    bson/util/misc.h
    # Client files.
    client/auth_helpers.cpp
    client/auth_helpers.h
    client/authlevel.h
    client/clientAndShell.cpp
    client/clientOnly.cpp
    client/clientOnly-private.h
    client/connpool.cpp
    client/connpool.h
    client/constants.h
    client/dbclient.cpp
    client/dbclient.h
    client/dbclient_rs.cpp
    client/dbclient_rs.h
    client/dbclientcursor.cpp
    client/dbclientcursor.h
    client/dbclientinterface.h
    client/dbclientmockcursor.h
    client/export_macros.h
    client/gridfs.cpp
    client/gridfs.h
    client/init.cpp
    client/init.h
    client/redef_macros.h
    #client/sasl_client_authenticate.cpp
    #client/sasl_client_authenticate.h
    client/syncclusterconnection.cpp
    client/syncclusterconnection.h
    client/undef_macros.h
    # DB files.
    db/jsobj.cpp
    db/jsobj.h
    db/jsobjmanipulator.h
    db/json.cpp
    db/json.h
    db/lasterror.cpp
    db/lasterror.h
    db/dbmessage.cpp
    db/dbmessage.h
    db/server_options.cpp
    db/server_options.h
    # Logger files.
    logger/appender.h
    logger/console.cpp
    logger/console.h
    logger/console_appender.h
    logger/encoder.h
    logger/labeled_level.h
    logger/log_domain.h
    logger/log_domain-impl.h
    logger/log_manager.cpp
    logger/log_manager.h
    logger/log_severity.cpp
    logger/log_severity-inl.h
    logger/log_severity.h
    logger/logger.cpp
    logger/logger.h
    logger/logstream_builder.cpp
    logger/logstream_builder.h
    logger/message_event.h
    logger/message_event_utf8_encoder.cpp
    logger/message_event_utf8_encoder.h
    logger/message_log_domain.cpp
    logger/message_log_domain.h
    logger/ramlog.cpp
    logger/ramlog.h
    logger/rotatable_file_appender.h
    logger/rotatable_file_manager.cpp
    logger/rotatable_file_manager.h
    logger/rotatable_file_writer.cpp
    logger/rotatable_file_writer.h
    logger/syslog_appender.h
    logger/tee.h
    # Platform files.
    platform/atomic_intrinsics.h
    platform/atomic_intrinsics_gcc.h
    platform/atomic_intrinsics_win32.h
    platform/atomic_word.h
    platform/backtrace.cpp
    platform/backtrace.h
    platform/basic.h
    platform/bits.h
    platform/compiler.h
    platform/compiler_gcc.h
    platform/compiler_msvc.h
    platform/cstdint.h
    platform/float_utils.h
    platform/hash_namespace.h
    platform/posix_fadvise.cpp
    platform/posix_fadvise.h
    platform/process_id.cpp
    platform/process_id.h
    platform/random.cpp
    platform/random.h
    #platform/strcasestr.cpp
    #platform/strcasestr.h
    #platform/stroll.h
    #platform/unordered_map.h
    #platform/unordered_set.h
    #platform/windows_basic.h
    # Util files.
    util/assert_util.cpp
    util/assert_util.h
    util/background.cpp
    util/background.h
    util/base64.cpp
    util/base64.h
    util/bson_util.h
    util/bufreader.h
    util/checksum.h
    util/debug_util.cpp
    util/debug_util.h
    util/descriptive_stats-inl.h
    util/descriptive_stats.h
    util/exit_code.h
    util/fail_point.cpp
    util/fail_point.h
    util/fail_point_registry.cpp
    util/fail_point_registry.h
    util/fail_point_service.cpp
    util/fail_point_service.h
    util/file.cpp
    util/file.h
    util/file_allocator.cpp
    util/file_allocator.h
    util/hex.cpp
    util/hex.h
    util/histogram.cpp
    util/histogram.h
    util/intrusive_counter.cpp
    util/intrusive_counter.h
    util/log.cpp
    util/log.h
    util/md5.cpp
    util/md5.h
    util/md5.hpp
    util/md5main.cpp
    util/password.cpp
    util/password.h
    util/paths.cpp
    util/paths.h
    util/processinfo.cpp
    util/processinfo.h
    util/signal_handlers.cpp
    util/signal_handlers.h
    util/stacktrace.cpp
    util/stacktrace.h
    util/stringutils.cpp
    util/stringutils.h
    util/text.cpp
    util/text.h
    util/time_support.cpp
    util/time_support.h
    util/timer.cpp
    util/timer-inl.h
    util/timer.h
    util/timer-generic-inl.h
    util/timer-posixclock-inl.h
    util/timer-win32-inl.h
    util/trace.cpp
    util/trace.h
    util/util.cpp
    util/version.cpp
    util/version.h
    util/winutil.h
    util/net/hostandport.h
    util/net/httpclient.cpp
    util/net/httpclient.h
    util/net/listen.cpp
    util/net/listen.h
    util/net/message.cpp
    util/net/message.h
    util/net/message_port.cpp
    util/net/message_port.h
    util/net/sock.cpp
    util/net/sock.h
    util/net/socket_poll.cpp
    util/net/socket_poll.h
    util/net/ssl_manager.cpp
    util/net/ssl_manager.h
    util/concurrency/list.h
    util/concurrency/mapsf.h
    util/concurrency/msg.h
    util/concurrency/mutex.h
    util/concurrency/mutexdebugger.cpp
    util/concurrency/mutexdebugger.h
    util/concurrency/mvar.h
    util/concurrency/qlock.h
    util/concurrency/race.h
    util/concurrency/rwlock.h
    util/concurrency/rwlockimpl.cpp
    util/concurrency/rwlockimpl.h
    util/concurrency/shared_mutex_win.hpp
    util/concurrency/simplerwlock.h
    util/concurrency/spin_lock.cpp
    util/concurrency/spin_lock.h
    util/concurrency/synchronization.cpp
    util/concurrency/synchronization.h
    util/concurrency/task.cpp
    util/concurrency/task.h
    util/concurrency/thread_name.cpp
    util/concurrency/thread_name.h
    util/concurrency/thread_pool.cpp
    util/concurrency/thread_pool.h
    util/concurrency/threadlocal.h
    util/concurrency/ticketholder.h
    util/concurrency/value.h
)

SET(MONGOCLIENT_SRC 
    #${BUILDINFO_SRC}
    ${ERROR_CODES_SRC}
    ${ACTION_TYPES_SRC}
    ${PCH_SRC}
    ${BASIC_SRC}
    ${PROCESSINFO_SRC}
    ${SASL_SRC}
    ${MURMURHASH3_SRC}
)

# These files are generated by tools.
SET_SOURCE_FILES_PROPERTIES(${BUILDINFO_SRC}
                            ${ERROR_CODES_SRC}
                            ${ACTION_TYPES_SRC}
                            PROPERTIES
                            GENERATED ON
)

# Set output directory.
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# Set dependencies.
SET(MONGOCLIENT_DEPENDENCIES
    #generate_buildinfo
    generate_error_codes
    generate_action_types
)

# Set common definitions.
IF (MONGOCLIENT_PLATFORM_DEFINITIONS)
    SET(MONGOCLIENT_DEFINITIONS "${MONGOCLIENT_PLATFORM_DEFINITIONS};")
ENDIF ()
SET(MONGOCLIENT_DEFINITIONS "${MONGOCLIENT_DEFINITIONS}MONGO_EXPOSE_MACROS")

# Set definitions for OpenSSL.
IF (OPENSSL_FOUND)
    SET(MONGOCLIENT_DEFINITIONS 
        "${MONGOCLIENT_DEFINITIONS};MONGO_SSL"
    )
ENDIF ()

# Set definitions for boost.
IF (NOT Boost_USE_STATIC_LIBS)
    SET(MONGOCLIENT_DEFINITIONS
        "${MONGOCLIENT_DEFINITIONS};BOOST_ALL_DYN_LINK"
    )
ENDIF ()

# Add mongoclient static library.
ADD_LIBRARY(mongoclient_static STATIC ${MONGOCLIENT_SRC})

ADD_DEPENDENCIES(mongoclient_static ${MONGOCLIENT_DEPENDENCIES})

IF (MSVC)
    SET(MONGOCLIENT_STATIC_OUTPUT_NAME libmongoclient)
ELSE ()
    SET(MONGOCLIENT_STATIC_OUTPUT_NAME mongoclient)
ENDIF ()

SET(MONGOCLIENT_STATIC_DEFINITIONS ${MONGOCLIENT_DEFINITIONS})
    
SET_TARGET_PROPERTIES(mongoclient_static PROPERTIES
                      OUTPUT_NAME ${MONGOCLIENT_STATIC_OUTPUT_NAME}
                      DEBUG_OUTPUT_NAME ${MONGOCLIENT_STATIC_OUTPUT_NAME}d
)

IF (MSVC)
    # VC will give the following warnings: 4996, 4244, 4800, 4251, 4267.
    SET_TARGET_PROPERTIES(mongoclient_static PROPERTIES
                          COMPILE_OPTIONS "/wd4996;/wd4244;/wd4800;/wd4251;/wd4267"
    )
ENDIF ()

SET_TARGET_PROPERTIES(mongoclient_static PROPERTIES
                      COMPILE_DEFINITIONS "${MONGOCLIENT_STATIC_DEFINITIONS}"
)

# Add mongoclient shared library.
ADD_LIBRARY(mongoclient_shared SHARED ${MONGOCLIENT_SRC})

SET_TARGET_PROPERTIES(mongoclient_shared PROPERTIES
                      OUTPUT_NAME mongoclient
                      DEBUG_OUTPUT_NAME mongoclientd
)

ADD_DEPENDENCIES(mongoclient_shared ${MONGOCLIENT_DEPENDENCIES})

IF (Boost_BUILTIN_LIBS)
    ADD_DEPENDENCIES(mongoclient_shared ${Boost_BUILTIN_LIBS})
ENDIF ()

SET(MONGOCLIENT_SHARED_DEFINITIONS ${MONGOCLIENT_DEFINITIONS})

IF (MSVC)
    # VC will give the following warnings: 4996, 4244, 4800, 4251, 4267.
    SET_TARGET_PROPERTIES(mongoclient_shared PROPERTIES
                          COMPILE_OPTIONS "/wd4996;/wd4244;/wd4800;/wd4251;/wd4267"
    )

    SET(MONGOCLIENT_SHARED_DEFINITIONS
        "${MONGOCLIENT_SHARED_DEFINITIONS};LIBMONGOCLIENT_BUILDING"
    )
ELSE ()
    SET_TARGET_PROPERTIES(mongoclient_shared PROPERTIES
                          COMPILE_OPTIONS "-fPIC"
    )
ENDIF ()

SET_TARGET_PROPERTIES(mongoclient_shared PROPERTIES
                      COMPILE_DEFINITIONS "${MONGOCLIENT_SHARED_DEFINITIONS}"
)

# Link to system libraries.
TARGET_LINK_LIBRARIES(mongoclient_shared ${MONGOCLIENT_SYSTEM_LIBS})

# Link to OpenSSL.
IF (OPENSSL_FOUND)
    TARGET_LINK_LIBRARIES(mongoclient_shared ${OPENSSL_LIBRARIES})
ENDIF ()

# Install header files.
INSTALL(DIRECTORY .
        DESTINATION include/mongo
        COMPONENT mongoclient_headers
        FILES_MATCHING REGEX ".*\\.hp?p?$"
)

INSTALL(FILES ${PROJECT_BINARY_DIR}/src/mongo/base/error_codes.h
        DESTINATION include/mongo/base/
        COMPONENT mongoclient_headers
)

INSTALL(FILES ${PROJECT_BINARY_DIR}/src/mongo/db/auth/action_type.h
        DESTINATION include/mongo/db/auth/
        COMPONENT mongoclient_headers
)

# Install libraries.
SET(INSTALL_TARGETS mongoclient_static mongoclient_shared)

INSTALL(TARGETS ${INSTALL_TARGETS}
        #DESTINATION bin
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT mongoclient_libraries
)

#INSTALL(TARGETS mongoclient_shared
#        DESTINATION bin
#        COMPONENT mongoclient_libraries
#)

# Set CPack options.
#SET(CPACK_PACKAGE_NAME "MONGOCLIENT")
#SET(CPACK_PACKAGE_VENDOR "Hongwei Zhou")
#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "MONGOCLIENT - MongoDB Client Libraries")
#SET(CPACK_PACKAGE_VERSION "2.4.8")
#SET(CPACK_PACKAGE_VERSION_MAJOR "2")
#SET(CPACK_PACKAGE_VERSION_MINOR "4")
#SET(CPACK_PACKAGE_VERSION_PATCH "8")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "mongoclient")

## Add CPack support.
#INCLUDE(CPack)

## Add components.
#CPACK_ADD_COMPONENT(mongoclient_headers
#                    DISPLAY_NAME "MongoDB Client C++ Headers"
#                    DESCRIPTION "MongoDB Client C++ Header files - 2.4.8"
#                    GROUP headers
#)
#
#CPACK_ADD_COMPONENT(mongoclient_targets
#                    DISPLAY_NAME "MongoDB Client C++ Libraries"
#                    DESCRIPTION "MongoDB Client C++ Libraries - 1.4.8"
#                    GROUP libraries
#)
