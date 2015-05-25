project(Algorithms)

cmake_minimum_required(VERSION 2.8)
# The version number
set (Algorithms_VERSION_MAJOR 1)
set (Algorithms_VERSION_MINOR 0)

include_directories(Interface)
include_directories(RealNumbers)
include_directories(Test)

aux_source_directory(. SRC_LIST)
aux_source_directory(ConjugateGradients SRC_LIST)
aux_source_directory(Interface SRC_LIST)
aux_source_directory(Newton SRC_LIST)
aux_source_directory(FunctionSpaces/RealNumbers SRC_LIST)
aux_source_directory(FunctionSpaces/VectorSpace SRC_LIST)
aux_source_directory(Test SRC_LIST)
aux_source_directory(Util SRC_LIST)
add_executable(Test ${SRC_LIST})
#target_link_libraries(Test LINK_PUBLIC Algorithms)

SET(CMAKE_CXX_FLAGS "-std=c++1y")
