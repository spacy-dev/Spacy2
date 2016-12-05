cmake_minimum_required(VERSION 3.1)

# Set path to cmake scripts
#set(CMAKE_MODULE_PATH
#    ${CMAKE_MODULE_PATH}
#    ../../../../cmake
#    )

# Do not allow in-source builds
#include(ConfigSafeGuards)

FIND_PACKAGE(deal.II 8.4 REQUIRED
  HINTS ${deal.II_DIR} ${DEAL_II_DIR} ../ ../../ $ENV{DEAL_II_DIR}
  )
include_directories(${deal.II_INCLUDE_DIRS})

# Default build type (can be overridden by user)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug MinSizeRel Release RelWithDebInfo." FORCE)
endif()

# Include path, link path, library and compiler flags for Spacy
find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIR})
add_definitions(${Spacy_CXX_FLAGS})

DEAL_II_INITIALIZE_CACHED_VARIABLES()

project(Spacy-Examples-dealII-Laplace)

# Executable
add_executable(run laplace.cpp)

DEAL_II_SETUP_TARGET(run)

# Target libraries
target_link_libraries(run ${Spacy_LIBRARIES})
