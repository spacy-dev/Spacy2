project(Spacy-Examples-Fenics-NonlinearHeat)

cmake_minimum_required(VERSION 3.1)

# Set path to cmake scripts
#set(CMAKE_MODULE_PATH
#    ${CMAKE_MODULE_PATH}
#    ../../../../cmake
#    )

# Do not allow in-source builds
#include(ConfigSafeGuards)

# Get DOLFIN configuration data (DOLFINConfig.cmake must be in
# DOLFIN_CMAKE_CONFIG_PATH)
#set(DOLFIN_DIR ../../../../cmake)
find_package(DOLFIN CONFIG REQUIRED)

# Include directories
include_directories(${DOLFIN_INCLUDE_DIRS})
include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
# Compiler definitions
add_definitions(${DOLFIN_CXX_DEFINITIONS})

# Need to get VTK config because VTK uses advanced VTK features which
# mean it's not enough to just link to the DOLFIN target. See
# http://www.vtk.org/pipermail/vtk-developers/2013-October/014402.html
find_package(VTK HINTS ${VTK_DIR} $ENV{VTK_DIR} NO_MODULE QUIET)

# Default build type (can be overridden by user)
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug MinSizeRel Release RelWithDebInfo." FORCE)
endif()

# Include path, link path, library and compiler flags for Spacy
find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIR})
add_definitions(${Spacy_CXX_FLAGS})

# Executable
add_executable(run main.cpp)

# Target libraries
target_link_libraries(run ${DOLFIN_LIBRARIES} ${Spacy_LIBRARIES})
