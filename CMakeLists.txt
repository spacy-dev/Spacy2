project(Algorithms)

cmake_minimum_required(VERSION 2.8)
# The version number
set (Algorithms_VERSION_MAJOR 1)
set (Algorithms_VERSION_MINOR 0)

#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-3.5")
set(CMAKE_BUILD_TYPE Debug)

option(BuildTest "BuildTest" ON)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_CURRENT_SOURCE_DIR}/CMake
    )

include(ConfigSafeGuards)

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wwrite-strings -Wpedantic")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y -Weffc++")
endif()

include_directories(
#    ${PROJECT_SOURCE_DIR}/RFFGen
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


find_package(DOLFIN)


#link_directories(/home/ignaz/lib/armadillo-4.650.4/)
include_directories(${PROJECT_SOURCE_DIR})
#include_directories(/home/ignaz/C++/RFFGen/RFFGen)
include_directories(${DOLFIN_INCLUDE_DIRS})
include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})

aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/. SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Adapter/FEniCS SRC_LIST)

aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Algorithm SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Algorithm/Functions_1D SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Algorithm/CG SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Algorithm/CompositeStep SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Algorithm/Newton SRC_LIST)

aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/VectorSpaces/RealSpace SRC_LIST)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/VectorSpaces/VectorSpace SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/VectorSpaces/ProductSpace SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Util SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Util/Base SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Util/Mixins SRC_LIST)
aux_source_directory(${PROJECT_SOURCE_DIR}/Algorithms/Util/Exceptions SRC_LIST)
add_library(algorithms ${SRC_LIST} ${OPERATOR_SRC_LIST})
#add_library(operators ${OPERATOR_SRC_LIST})
#target_link_libraries(operators algorithms)
#add_executable(Test ${PROJECT_SOURCE_DIR}/Algorithms/main.cpp)
#target_link_libraries(Test operators armadillo)

#if(BuildTest)
    aux_source_directory(Test TEST_SRC_LIST)
    aux_source_directory(Test/FunctionSpaces TEST_SRC_LIST)
    aux_source_directory(Test/Operators TEST_SRC_LIST)
    add_executable(unit_tests ${TEST_SRC_LIST})
    add_dependencies(unit_tests gtest)
    target_link_libraries(unit_tests algorithms gtest pthread)
#endif()
