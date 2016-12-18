project(Spacy-Examples-Scalar-Trust-Region)

cmake_minimum_required(VERSION 3.1)

find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIR})
add_definitions(${Spacy_CXX_DEFINITIONS})

add_executable(run trust_region.cpp)
target_link_libraries(run ${Spacy_LIBRARIES})

