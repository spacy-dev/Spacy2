cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME nonlinear_heat)
project(${PROJECT_NAME})

# Set verbose output while testing CMake
#set(CMAKE_VERBOSE_MAKEFILE 1)

# Set CMake behavior
cmake_policy(SET CMP0004 OLD)

# Get DOLFIN configuration data (DOLFINConfig.cmake must be in
# DOLFIN_CMAKE_CONFIG_PATH)
set(DOLFIN_DIR ../../../../cmake)
find_package(DOLFIN REQUIRED)

# Need to get VTK config because VTK uses advanced VTK features which
# mean it's not enough to just link to the DOLFIN target. See
# http://www.vtk.org/pipermail/vtk-developers/2013-October/014402.html
find_package(VTK HINTS ${VTK_DIR} $ENV{VTK_DIR} NO_MODULE QUIET)

# Default build type (can be overridden by user)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug MinSizeRel Release RelWithDebInfo." FORCE)
endif()

# Include directories
include_directories(${DOLFIN_INCLUDE_DIRS})
include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
set(LIBS ${LIBS} ${DOLFIN_LIBRARIES})
# Compiler definitions
add_definitions(${DOLFIN_CXX_DEFINITIONS})

# Include path, link path, library and compiler flags for Spacy
find_package(Spacy REQUIRED)
include_directories(${SPACY_INCLUDE_DIRS})
link_directories(${SPACY_LIBRARY_DIR})
#set(LIBS ${LIBS} ${SPACY_LIBRARIES})
# Compiler flags from Spacy, i.e. -std=c++1y
add_compile_options(${SPACY_CXX_OPTIONS})

# Executable
add_executable(${PROJECT_NAME} main.cpp)

# Target libraries
target_link_libraries(${PROJECT_NAME} ${LIBS} ${SPACY_LIBRARY_DIR}/libspacy.a)
