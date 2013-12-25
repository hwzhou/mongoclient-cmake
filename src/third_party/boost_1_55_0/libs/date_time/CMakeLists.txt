CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_DATE_TIME_SRC
    src/gregorian/date_generators.cpp
    src/gregorian/greg_month.cpp
    src/gregorian/greg_weekday.cpp
    src/gregorian/gregorian_types.cpp
    src/posix_time/posix_time_types.cpp
)

ADD_LIBRARY(boost_date_time ${BOOST_TARGET_TYPE} ${BOOST_DATE_TIME_SRC})

SET_TARGET_PROPERTIES(boost_date_time PROPERTIES
                      OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_date_time${BOOST_TARGET_LIBRARY_SUFFIX_RELEASE}"
                      DEBUG_OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_date_time${BOOST_TARGET_LIBRARY_SUFFIX_DEBUG}"
)

SET(Boost_BUILTIN_LIBS 
    ${Boost_BUILTIN_LIBS} 
    boost_date_time 
    PARENT_SCOPE
)

# Install.
INSTALL(TARGETS boost_date_time
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT boost_libraries
)
