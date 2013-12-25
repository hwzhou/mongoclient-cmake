CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(BOOST_FILESYSTEM_SRC
    v2/src/v2_operations.cpp
    v2/src/v2_path.cpp
    v2/src/v2_portability.cpp
    v3/src/codecvt_error_category.cpp
    v3/src/operations.cpp
    v3/src/path.cpp
    v3/src/path_traits.cpp
    v3/src/portability.cpp
    v3/src/unique_path.cpp
    v3/src/utf8_codecvt_facet.cpp
    v3/src/windows_file_codecvt.cpp
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
