CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BOOST)

# Include "FindBoost.cmake" to use its configure variables.
INCLUDE(FindBoost)

IF (Boost_USE_STATIC_LIBS)
    # Set library type.
    SET(BOOST_TARGET_TYPE "STATIC")
    # Set library prefix.
    SET(BOOST_TARGET_LIBRARY_PREFIX lib)
    
    ADD_DEFINITIONS(-D_MT -D__MT__)
ELSE ()
    # Set library type.
    SET(BOOST_TARGET_TYPE "SHARED")

    ADD_DEFINITIONS(-D_DLL -D_RTLDLL -DBOOST_ALL_DYN_LINK -D_MT -D__MT__)
ENDIF ()

# Set library suffix.
SET(BOOST_TARGET_LIBRARY_SUFFIX_RELEASE "${_boost_COMPILER}${_boost_MULTITHREADED}${_boost_RELEASE_ABI_TAG}-${Boost_LIB_VERSION}")
SET(BOOST_TARGET_LIBRARY_SUFFIX_DEBUG "${_boost_COMPILER}${_boost_MULTITHREADED}${_boost_DEBUG_ABI_TAG}-${Boost_LIB_VERSION}")

# Set output directory.
SET(LIBRARY_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

# Set link directory.
LINK_DIRECTORIES(${LIBRARY_OUTPUT_PATH})

# Set library directory for other projects.
SET(Boost_LIBRARY_DIR ${LIBRARY_OUTPUT_PATH} PARENT_SCOPE)

ADD_SUBDIRECTORY(libs/date_time)
ADD_SUBDIRECTORY(libs/system)
ADD_SUBDIRECTORY(libs/filesystem)
ADD_SUBDIRECTORY(libs/thread)
ADD_SUBDIRECTORY(libs/program_options)

IF (Boost_INCLUDE_DETAIL)
    #SET(Boost_BUILTIN_LIBS "" PARENT_SCOPE)

    ADD_SUBDIRECTORY(libs/detail)
ENDIF ()

# Set builtin boost libraries.
SET(Boost_BUILTIN_LIBS ${Boost_BUILTIN_LIBS} PARENT_SCOPE)

# Install header files.
INSTALL(DIRECTORY boost
        DESTINATION include
        COMPONENT boost_headers
        FILES_MATCHING REGEX ".*\\.hp?p?$"
)

# Install license.
INSTALL(FILES "BOOST-1.0.txt"
        DESTINATION .
        COMPONENT boost_libraries
)

# Install libraries.
#SET(INSTALL_TARGETS boost_date_time boost_system boost_filesystem boost_thread boost_program_options)
#
#IF (Boost_INCLUDE_DETAIL)
#    SET(INSTALL_TARGETS ${INSTALL_TARGETS} boost_detail)
#ENDIF ()
#
#INSTALL(TARGETS ${INSTALL_TARGETS}
#        RUNTIME DESTINATION bin
#        LIBRARY DESTINATION lib
#        ARCHIVE DESTINATION lib
#        COMPONENT boost_targets
#)

## Set CPack options.
#SET(CPACK_PACKAGE_NAME "BOOST 1.49.0")
#SET(CPACK_PACKAGE_VENDOR "Hongwei Zhou")
#SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "BOOST C++ Libraries - 1.49.0")
#SET(CPACK_PACKAGE_VERSION "1.49.0")
#SET(CPACK_PACKAGE_VERSION_MAJOR "1")
#SET(CPACK_PACKAGE_VERSION_MINOR "49")
#SET(CPACK_PACKAGE_VERSION_PATCH "0")
#SET(CPACK_PACKAGE_INSTALL_DIRECTORY "boost_1_49_0")
#
## Add CPack support.
#INCLUDE(CPack)
#
## Add components.
#CPACK_ADD_COMPONENT(boost_headers
#                    DISPLAY_NAME "BOOST C++ Headers"
#                    DESCRIPTION "BOOST C++ Header files - 1.49.0"
#                    GROUP headers
#)
#
#CPACK_ADD_COMPONENT(boost_targets
#                    DISPLAY_NAME "BOOST C++ Libraries"
#                    DESCRIPTION "BOOST C++ Libraries - 1.49.0"
#                    GROUP libraries
#)
