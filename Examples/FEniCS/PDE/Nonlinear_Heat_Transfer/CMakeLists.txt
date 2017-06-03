project(Spacy-Examples-Fenics-NonlinearHeat)

cmake_minimum_required(VERSION 3.1)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING
    "Choose the type of build, options are: Debug MinSizeRel Release RelWithDebInfo." FORCE)
endif()

# FEniCS - dolfin
find_package(DOLFIN CONFIG REQUIRED)
include(${DOLFIN_USE_FILE})
include_directories(${DOLFIN_INCLUDE_DIRS})
include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
add_definitions(${DOLFIN_CXX_DEFINITIONS})
find_package(VTK HINTS ${VTK_DIR} $ENV{VTK_DIR} NO_MODULE QUIET)

# Spacy
find_package(Spacy CONFIG REQUIRED)
include_directories(${Spacy_INCLUDE_DIRS})
set(CMAKE_CXX_STANDARD ${Spacy_CXX_STANDARD})
# Executable
add_executable(run main.cpp)

# Target libraries
target_link_libraries(run ${DOLFIN_LIBRARIES} ${Spacy_LIBRARIES})
