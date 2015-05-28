project(Algorithms)

cmake_minimum_required(VERSION 2.8)
# The version number
set (Algorithms_VERSION_MAJOR 1)
set (Algorithms_VERSION_MINOR 0)

option(BuildTest "BuildTest" OFF)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
    )

include(ConfigSafeGuards)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "-Wall -Wno-unknown-pragmas -Wno-sign-compare -Woverloaded-virtual -Wwrite-strings -Wno-unused")
    set(CMAKE_CXX_FLAGS_DEBUG   "-O0 -g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O0 -g")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage -std=c++1y")
#    set(CMAKE_CXX_LINK_FLAGS "-fprofile-arcs -ftest-coverage")
endif()

include_directories(
    ${PROJECT_SOURCE_DIR}/RFFGen
    ${PROJECT_SOURCE_DIR}/External/googletest
    ${PROJECT_SOURCE_DIR}/External/googletest/include
    )

add_custom_target(
    git_update
    COMMAND git submodule init
    COMMAND git submodule update
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    )

add_library(
    gtest
    ${PROJECT_SOURCE_DIR}/External/googletest/src/gtest-all.cc
    ${PROJECT_SOURCE_DIR}/External/googletest/src/gtest_main.cc
    )

add_dependencies(gtest git_update)

set_source_files_properties(${PROJECT_SOURCE_DIR}/External/googletest/src/gtest-all.cc  PROPERTIES GENERATED 1)
set_source_files_properties(${PROJECT_SOURCE_DIR}/External/googletest/src/gtest_main.cc PROPERTIES GENERATED 1)

link_directories(/home/ignaz/lib/armadillo-4.650.4/)
include_directories(Algorithms/Interface)
include_directories(Algorithms/RealNumbers)
include_directories(Algorithms/Test)

aux_source_directory(Algorithms/. SRC_LIST)
aux_source_directory(Algorithms/Algorithm SRC_LIST)
aux_source_directory(Algorithms/Algorithm/ConjugateGradients SRC_LIST)
aux_source_directory(Algorithms/Interface SRC_LIST)
aux_source_directory(Algorithms/FunctionSpaces/RealNumbers SRC_LIST)
aux_source_directory(Algorithms/FunctionSpaces/VectorSpace SRC_LIST)
aux_source_directory(Algorithms/FunctionSpaces/ProductSpace SRC_LIST)
aux_source_directory(Algorithms/Test SRC_LIST)
aux_source_directory(Algorithms/Util SRC_LIST)
add_library(algorithms ${SRC_LIST})
add_executable(Test Algorithms/main.cpp)
target_link_libraries(Test algorithms )

if(BuildTest)
    aux_source_directory(Test TEST_SRC_LIST)
    aux_source_directory(Test/FunctionSpaces TEST_SRC_LIST)
    add_executable(unit_tests ${TEST_SRC_LIST})
    add_dependencies(unit_tests gtest)
    target_link_libraries(unit_tests algorithms gtest pthread)
endif()
