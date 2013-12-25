CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(BSONDEMO)

# Include example settings.
INCLUDE(../../MongoClientExamples.cmake)

# bson demo
MongoClient_BUILD_EXAMPLES(bsondemo bsondemo.cpp)
