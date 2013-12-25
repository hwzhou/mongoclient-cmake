CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(EXAMPLES)

# Include example settings.
INCLUDE(../../MongoClientExamples.cmake)

# authentication test
MongoClient_BUILD_EXAMPLES(authTest authTest.cpp)

# client test
MongoClient_BUILD_EXAMPLES(clientTest clientTest.cpp)

# first example
MongoClient_BUILD_EXAMPLES(firstExample first.cpp)

# http client test
MongoClient_BUILD_EXAMPLES(httpClientTest httpClientTest.cpp)

# rs example
MongoClient_BUILD_EXAMPLES(rsExample rs.cpp)

# second example
MongoClient_BUILD_EXAMPLES(secondExample second.cpp)

# tutorial
MongoClient_BUILD_EXAMPLES(tutorial tutorial.cpp)

# where example
MongoClient_BUILD_EXAMPLES(whereExample whereExample.cpp)
