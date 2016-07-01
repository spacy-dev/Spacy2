cmake_minimum_required(VERSION 3.0)
project(type_erasure_test)

add_definitions(-std=c++11)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)
aux_source_directory(basic SRC_LIST)
aux_source_directory(cow SRC_LIST)
aux_source_directory(sbo SRC_LIST)
aux_source_directory(sbo_cow SRC_LIST)

add_executable(unit_tests ${SRC_LIST})
target_link_libraries(unit_tests ${GTEST_LIBRARIES} pthread)

include(CTest)
enable_testing()
add_test(test ${PROJECT_BINARY_DIR}/Test/unit_tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
DEPENDS ${PROJECT_BINARY_DIR}/Test/unit_tests)
