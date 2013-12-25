CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_CHRONO_SRC
    src/chrono.cpp
    src/process_cpu_clocks.cpp
    src/thread_clock.cpp
)

ADD_LIBRARY(boost_chrono ${BOOST_TARGET_TYPE} ${BOOST_CHRONO_SRC})

SET_TARGET_PROPERTIES(boost_chrono PROPERTIES
                      OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_chrono${BOOST_TARGET_LIBRARY_SUFFIX_RELEASE}"
                      DEBUG_OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_chrono${BOOST_TARGET_LIBRARY_SUFFIX_DEBUG}"
)

SET(Boost_BUILTIN_LIBS 
    ${Boost_BUILTIN_LIBS} 
    boost_chrono 
    PARENT_SCOPE
)

# Install.
INSTALL(TARGETS boost_chrono
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT boost_libraries
)
