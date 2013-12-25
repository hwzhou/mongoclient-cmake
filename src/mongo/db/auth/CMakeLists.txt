CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

ADD_EXECUTABLE(generate_action_types ../../../../utils/generate_action_types.cpp)

GET_TARGET_PROPERTY(GENERATE_ACTION_TYPES_EXE_LOC generate_action_types LOCATION)

ADD_CUSTOM_COMMAND(TARGET generate_action_types
                   POST_BUILD
                   COMMAND ${GENERATE_ACTION_TYPES_EXE_LOC} 
                           "${PROJECT_SOURCE_DIR}/src/mongo/db/auth/action_types.txt"
                           "${PROJECT_BINARY_DIR}/src/mongo/db/auth/action_type.h" 
                           "${PROJECT_BINARY_DIR}/src/mongo/db/auth/action_type.cpp"
)
