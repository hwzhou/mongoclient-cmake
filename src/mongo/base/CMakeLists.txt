CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_EXECUTABLE(generate_error_codes ../../../utils/generate_error_codes.cpp)

GET_TARGET_PROPERTY(GENERATE_ERROR_CODES_EXE_LOC generate_error_codes LOCATION)

ADD_CUSTOM_COMMAND(TARGET generate_error_codes
                   POST_BUILD
                   COMMAND ${GENERATE_ERROR_CODES_EXE_LOC} 
                           "${PROJECT_SOURCE_DIR}/src/mongo/base/error_codes.err" 
                           "${PROJECT_BINARY_DIR}/src/mongo/base/error_codes.h" 
                           "${PROJECT_BINARY_DIR}/src/mongo/base/error_codes.cpp"
)
