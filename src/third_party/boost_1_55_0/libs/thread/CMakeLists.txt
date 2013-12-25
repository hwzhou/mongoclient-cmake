CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_THREAD_SRC
    src/future.cpp
    src/tss_null.cpp
)

IF (WIN32)
  SET(BOOST_THREAD_SRC
      ${BOOST_THREAD_SRC}
      src/win32/thread.cpp
      src/win32/tss_dll.cpp
      src/win32/tss_pe.cpp
)
ELSE ()
  SET(BOOST_THREAD_SRC
      ${BOOST_THREAD_SRC}
      src/pthread/once.cpp
      src/pthread/once_atomic.cpp
      src/pthread/thread.cpp
)
ENDIF ()

ADD_LIBRARY(boost_thread ${BOOST_TARGET_TYPE} ${BOOST_THREAD_SRC})

ADD_DEPENDENCIES(boost_thread boost_date_time boost_chrono)

SET_TARGET_PROPERTIES(boost_thread PROPERTIES
                      OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_thread${BOOST_TARGET_LIBRARY_SUFFIX_RELEASE}"
                      DEBUG_OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_thread${BOOST_TARGET_LIBRARY_SUFFIX_DEBUG}"
)

IF (Boost_USE_STATIC_LIBS)
    SET_TARGET_PROPERTIES(boost_thread PROPERTIES 
                          COMPILE_DEFINITIONS BOOST_THREAD_BUILD_LIB)
ELSE ()
    SET_TARGET_PROPERTIES(boost_thread PROPERTIES 
                          COMPILE_DEFINITIONS BOOST_THREAD_BUILD_DLL)
ENDIF ()

SET(Boost_BUILTIN_LIBS 
    ${Boost_BUILTIN_LIBS} 
    boost_thread 
    PARENT_SCOPE
)

# Install.
INSTALL(TARGETS boost_thread
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT boost_libraries
)
