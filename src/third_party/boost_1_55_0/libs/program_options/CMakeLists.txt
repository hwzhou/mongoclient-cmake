CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_PROGRAM_OPTIONS_SRC
    src/cmdline.cpp
    src/config_file.cpp
    src/convert.cpp
    src/options_description.cpp
    src/parsers.cpp
    src/positional_options.cpp
    src/split.cpp
    src/utf8_codecvt_facet.cpp
    src/value_semantic.cpp
    src/variables_map.cpp
    src/winmain.cpp
)

ADD_LIBRARY(boost_program_options ${BOOST_TARGET_TYPE} ${BOOST_PROGRAM_OPTIONS_SRC})

SET_TARGET_PROPERTIES(boost_program_options PROPERTIES
                      OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_program_options${BOOST_TARGET_LIBRARY_SUFFIX_RELEASE}"
                      DEBUG_OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_program_options${BOOST_TARGET_LIBRARY_SUFFIX_DEBUG}"
)

SET(Boost_BUILTIN_LIBS 
    ${Boost_BUILTIN_LIBS} 
    boost_program_options 
    PARENT_SCOPE
)

# Install.
INSTALL(TARGETS boost_program_options
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT boost_libraries
)
