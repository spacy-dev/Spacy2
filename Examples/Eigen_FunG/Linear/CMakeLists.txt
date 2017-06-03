project(Spacy-Examples-EigenAndFung-Example1)

cmake_minimum_required(VERSION 3.1)

find_package(FunG REQUIRED)
include_directories(${FUNG_INCLUDE_DIRS})

find_package(Eigen3 REQUIRED)
include_directories(${EIGEN3_INCLUDE_DIR})

# Include path, link path, library and compiler flags for Spacy
find_package(Spacy REQUIRED)
include_directories(${Spacy_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD ${Spacy_CXX_STANDARD})

# Executable
add_executable(run main.cpp)

# Target libraries
target_link_libraries(run ${Spacy_LIBRARIES})
