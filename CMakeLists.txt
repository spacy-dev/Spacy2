project(Algorithms)

cmake_minimum_required(VERSION 2.8)
# The version number
set (Algorithms_VERSION_MAJOR 1)
set (Algorithms_VERSION_MINOR 0)

#link_directories(/home/ignaz/lib/armadillo-4.650.4/)
include_directories(Algorithms/Interface)
include_directories(Algorithms/RealNumbers)
include_directories(Algorithms/Test)

aux_source_directory(Algorithms/. SRC_LIST)
aux_source_directory(Algorithms/ConjugateGradients SRC_LIST)
aux_source_directory(Algorithms/Interface SRC_LIST)
aux_source_directory(Algorithms/Newton SRC_LIST)
aux_source_directory(Algorithms/FunctionSpaces/RealNumbers SRC_LIST)
aux_source_directory(Algorithms/FunctionSpaces/VectorSpace SRC_LIST)
aux_source_directory(Algorithms/Test SRC_LIST)
aux_source_directory(Algorithms/Util SRC_LIST)
add_executable(Test ${SRC_LIST})
#target_link_libraries(Test armadillo)

SET(CMAKE_CXX_FLAGS "-std=c++1y")