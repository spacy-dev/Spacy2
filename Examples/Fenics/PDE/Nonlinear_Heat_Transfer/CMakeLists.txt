cmake_minimum_required(VERSION 2.8)

set(PROJECT_NAME nonlinear_heat)
project(${PROJECT_NAME})

# Set verbose output while testing CMake
#set(CMAKE_VERBOSE_MAKEFILE 1)

# Set CMake behavior
cmake_policy(SET CMP0004 OLD)

# Get DOLFIN configuration data (DOLFINConfig.cmake must be in
# DOLFIN_CMAKE_CONFIG_PATH)
find_package(DOLFIN)

# Need to get VTK config because VTK uses advanced VTK features which
# mean it's not enough to just link to the DOLFIN target. See
# http://www.vtk.org/pipermail/vtk-developers/2013-October/014402.html
find_package(VTK HINTS ${VTK_DIR} $ENV{VTK_DIR} NO_MODULE QUIET)

# Default build type (can be overridden by user)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug MinSizeRel Release RelWithDebInfo." FORCE)
endif()

# Compiler definitions
add_definitions(${DOLFIN_CXX_DEFINITIONS})

# Include directories
include_directories(${DOLFIN_INCLUDE_DIRS})
include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
set(LIBS ${LIBS} ${DOLFIN_LIBRARIES})

find_package(VSpace REQUIRED)
include_directories(${VSPACE_INCLUDE_DIRS})
link_directories(${VSPACE_LIBRARY_DIRS})
set(LIBS ${LIBS} ${VSPACE_LIBRARIES})

# Compiler flags inherited from Spacy
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${VSPACE_CXX_FLAGS}" )

# Executable
add_executable(${PROJECT_NAME} main.cpp)

# Target libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
