project(Spacy-Examples-Scalar-Trust-Region)

cmake_minimum_required(VERSION 3.1)

find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD ${Spacy_CXX_STANDARD})

add_executable(run trust_region.cpp)
target_link_libraries(run ${Spacy_LIBRARIES})

