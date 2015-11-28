project(spacy)

cmake_minimum_required(VERSION 2.8)

# The version number
set (SPACY_MAJOR_VERSION 0)
set (SPACY_MINOR_VERSION 1)
set (SPACY_PATCH_VERSION 0)
set (SPACY_VERSION ${SPACY_MAJOR_VERSION}.${SPACY_MINOR_VERSION}.${SPACY_PATCH_VERSION})

# Set path to cmake scripts
set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake
    )
  
# Do not allow in-source builds  
include(ConfigSafeGuards)  
  
# Set compiler flags
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wwrite-strings -Wpedantic -std=c++1y")# -Weffc++")
endif()
  
# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()
 
# set up include-directories
include_directories("${PROJECT_SOURCE_DIR}")

#include_directories(/home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost_1_59_0/include)
#link_directories(/home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost_1_59_0/lib)

# set up subdirectory containing code
add_subdirectory(Spacy)
  
# Does not debug auto, but is less sloppy than gcc
# set(CMAKE_CXX_COMPILER "/usr/bin/clang++-3.5")
#set(CMAKE_CXX_COMPILER "/home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/gcc/bin/g++")
#set(CMAKE_CXX_COMPILER "/usr/bin/g++-5")

# enable/disable tests
option(BuildTest "BuildTest" ON)
option(Kaskade "Kaskade" OFF)

#if(BuildTest)
  if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wwrite-strings -Wpedantic -std=c++1y -Weffc++")
  endif()
  include_directories(${PROJECT_SOURCE_DIR}/External/googletest ${PROJECT_SOURCE_DIR}/External/googletest/include )
  add_custom_target(git_update
                    COMMAND git submodule init
                    COMMAND git submodule update
                    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  add_library(gtest
              ${PROJECT_SOURCE_DIR}/External/googletest/src/gtest-all.cc
              ${PROJECT_SOURCE_DIR}/External/googletest/src/gtest_main.cc)

  add_dependencies(gtest git_update)

  set_source_files_properties(${PROJECT_SOURCE_DIR}/External/googletest/src/gtest-all.cc  PROPERTIES GENERATED 1)
  set_source_files_properties(${PROJECT_SOURCE_DIR}/External/googletest/src/gtest_main.cc PROPERTIES GENERATED 1)
  aux_source_directory(Test TEST_SRC_LIST)
  aux_source_directory(Test/Mock TEST_SRC_LIST)
  aux_source_directory(Test/Spacy TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Adapter/Scalar TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Algorithm TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Algorithm/Newton/Scalar TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Spaces TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Spaces/ProductSpace TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Spaces/PrimalDualProductSpace TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Util TEST_SRC_LIST)
  aux_source_directory(Test/Spacy/Util/Base TEST_SRC_LIST)

#  find_package(Eigen3)
#  if(EIGEN3_FOUND)
#    include_directories(${EIGEN3_INCLUDE_DIR})
#    aux_source_directory(Test/Spacy/Adapter/Eigen TEST_SRC_LIST)
#  endif()

  if(Kaskade)
    include_directories(/home/lars/Projects/OpenSource/Kaskade/Kaskade7.3
#                       /home/lars/libs/acml/acml5.3.1/gfortran64/include
                        /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost-1.59.0/include
                        /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/include
                        /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/include/ug)
    link_directories(/home/lars/Projects/OpenSource/Kaskade/Kaskade7.3/libs
#                     /home/lars/libs/acml/acml5.3.1/gfortran64/lib
                     /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost-1.59.0/lib
                     /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/lib)
    aux_source_directory(Test/Spacy/Adapter/Kaskade TEST_SRC_LIST)
  endif()

  add_executable(unit_tests ${TEST_SRC_LIST})
  target_link_libraries(unit_tests spacy gtest pthread)
  if(Kaskade)
    target_link_libraries(unit_tests kaskade boost_system dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX)
#    target_link_libraries(unit_tests kaskade dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX boost_signals boost_program_options boost_system boost_timer boost_thread boost_chrono acml)
  endif()
#endif()


# export add_library
# ==================
export(TARGETS spacy FILE ${PROJECT_BINARY_DIR}/SpacyTargets.cmake)
export(PACKAGE Spacy)
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}" "${PROJECT_SOURCE_DIR}")
# ... for the build tree
include(${PROJECT_SOURCE_DIR}/cmake/SpacyConfig.cmake.in)
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(CONF_LIBRARY_DIR "${PROJECT_BINARY_DIR}/Spacy")
configure_file(${PROJECT_SOURCE_DIR}/cmake/SpacyConfig.cmake.in
  "${PROJECT_BINARY_DIR}/SpacyConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${VSPACE_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(${PROJECT_SOURCE_DIR}/cmake/SpacyConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SpacyConfig.cmake" @ONLY)
# ... for both
configure_file(${PROJECT_SOURCE_DIR}/cmake/SpacyConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/SpacyConfigVersion.cmake" @ONLY)
 
# Install the SpacyConfig.cmake and SpacyConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/SpacyConfig.cmake"
  "${PROJECT_BINARY_DIR}/SpacyConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT SpacyTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
