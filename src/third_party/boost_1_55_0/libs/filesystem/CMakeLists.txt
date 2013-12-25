CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_FILESYSTEM_SRC
    src/codecvt_error_category.cpp
    src/operations.cpp
    src/path.cpp
    src/path_traits.cpp
    src/portability.cpp
    src/unique_path.cpp
    src/utf8_codecvt_facet.cpp
    src/windows_file_codecvt.cpp
)

ADD_LIBRARY(boost_filesystem ${BOOST_TARGET_TYPE} ${BOOST_FILESYSTEM_SRC})

ADD_DEPENDENCIES(boost_filesystem boost_system)

SET_TARGET_PROPERTIES(boost_filesystem PROPERTIES
                      OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_filesystem${BOOST_TARGET_LIBRARY_SUFFIX_RELEASE}"
                      DEBUG_OUTPUT_NAME "${BOOST_TARGET_LIBRARY_PREFIX}boost_filesystem${BOOST_TARGET_LIBRARY_SUFFIX_DEBUG}"
)

SET(Boost_BUILTIN_LIBS 
    ${Boost_BUILTIN_LIBS} 
    boost_filesystem 
    PARENT_SCOPE
)

# Install.
INSTALL(TARGETS boost_filesystem
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        COMPONENT boost_libraries
)
