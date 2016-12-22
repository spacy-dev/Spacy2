cmake_minimum_required(VERSION 3.1)

FIND_PACKAGE(deal.II 8.4 REQUIRED
  HINTS ${deal.II_DIR} ${DEAL_II_DIR} ../ ../../ $ENV{DEAL_II_DIR}
  )

DEAL_II_INITIALIZE_CACHED_VARIABLES()

project(Spacy-Examples-dealII-Optimization-Linear-Heat)

include_directories(${deal.II_INCLUDE_DIRS})

find_package(FunG REQUIRED)
include_directories(${FUNG_INCLUDE_DIRS})

# Include path, link path, library and compiler flags for Spacy
find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIR})
add_definitions(${Spacy_CXX_FLAGS})

# Executable
add_executable(run linear_heat.cpp)

DEAL_II_SETUP_TARGET(run)

# Target libraries
target_link_libraries(run ${Spacy_LIBRARIES})
