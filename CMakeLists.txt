project(spacy)

cmake_minimum_required(VERSION 2.8)
# The version number
set (VSPACe_MAJOR_VERSION 0)
set (VSPACE_MINOR_VERSION 1)
set (VSPACE_PATCH_VERSION 0)
set(VSPACE_VERSION ${FOOBAR_MAJOR_VERSION}.${FOOBAR_MINOR_VERSION}.${FOOBAR_PATCH_VERSION})

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/Modules
    )
  
include(CMakeConfigGuards)  
  
# Make relative paths absolute (needed later on)
foreach(p LIB BIN INCLUDE CMAKE)
  set(var INSTALL_${p}_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()
 
# set up include-directories
include_directories(
  "${PROJECT_SOURCE_DIR}"
  "${PROJECT_BINARY_DIR}")

add_subdirectory(Spacy)
  
#set(CMAKE_CXX_COMPILER "/usr/bin/clang++-3.5")
#set(CMAKE_BUILD_TYPE Debug)

# enable/disable unit tests
option(BuildTest "BuildTest" OFF)


#include(ConfigSafeGuards) 

if(BuildTest)
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
  aux_source_directory(Test/FunctionSpaces TEST_SRC_LIST)
  add_executable(unit_tests ${TEST_SRC_LIST})
  add_dependencies(unit_tests gtest)
  target_link_libraries(unit_tests vspace gtest pthread)
endif()


# export add_library
# ==================
export(TARGETS vspace FILE ${PROJECT_BINARY_DIR}/VSpaceTargets.cmake)
export(PACKAGE VSpace)
file(RELATIVE_PATH REL_INCLUDE_DIR "${INSTALL_CMAKE_DIR}"
   "${INSTALL_INCLUDE_DIR}")
# ... for the build tree
include(${PROJECT_SOURCE_DIR}/cmake/Modules/VSpaceConfig.cmake.in)
set(CONF_INCLUDE_DIRS "${PROJECT_SOURCE_DIR}" "${PROJECT_BINARY_DIR}")
set(CONF_LIBRARY_DIRS "${PROJECT_BINARY_DIR}/Spacy")
configure_file(${PROJECT_SOURCE_DIR}/cmake/Modules/VSpaceConfig.cmake.in
  "${PROJECT_BINARY_DIR}/VSpaceConfig.cmake" @ONLY)
# ... for the install tree
set(CONF_INCLUDE_DIRS "\${VSPACE_CMAKE_DIR}/${REL_INCLUDE_DIR}")
configure_file(${PROJECT_SOURCE_DIR}/cmake/Modules/VSpaceConfig.cmake.in
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/VSpaceConfig.cmake" @ONLY)
# ... for both
configure_file(${PROJECT_SOURCE_DIR}/cmake/Modules/VSpaceConfigVersion.cmake.in
  "${PROJECT_BINARY_DIR}/VSpaceConfigVersion.cmake" @ONLY)
 
# Install the VSpaceConfig.cmake and VSpaceConfigVersion.cmake
install(FILES
  "${PROJECT_BINARY_DIR}${CMAKE_FILES_DIRECTORY}/VSpaceConfig.cmake"
  "${PROJECT_BINARY_DIR}/VSpaceConfigVersion.cmake"
  DESTINATION "${INSTALL_CMAKE_DIR}" COMPONENT dev)
 
# Install the export set for use with the install-tree
install(EXPORT VSpaceTargets DESTINATION
  "${INSTALL_CMAKE_DIR}" COMPONENT dev)
