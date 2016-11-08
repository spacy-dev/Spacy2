find_package(GTest REQUIRED)

include_directories( ${GTEST_INCLUDE_DIRS} )

#aux_source_directory(. TEST_SRC_LIST)
aux_source_directory(Mock MOCK_SRC_LIST)
aux_source_directory(Spacy TEST_SRC_LIST)
aux_source_directory(Spacy/Adapter/Scalar TEST_SRC_LIST)
aux_source_directory(Spacy/Algorithm TEST_SRC_LIST)
aux_source_directory(Spacy/Algorithm/CompositeStep TEST_SRC_LIST)
aux_source_directory(Spacy/Algorithm/Scalar TEST_SRC_LIST)
aux_source_directory(Spacy/Algorithm/Generic TEST_SRC_LIST)
aux_source_directory(Spacy/Algorithm/Newton/Scalar TEST_SRC_LIST)
aux_source_directory(Spacy/Spaces TEST_SRC_LIST)
aux_source_directory(Spacy/Spaces/ProductSpace TEST_SRC_LIST)
aux_source_directory(Spacy/Spaces/PrimalDualProductSpace TEST_SRC_LIST)
aux_source_directory(Spacy/Util TEST_SRC_LIST)
aux_source_directory(Spacy/Util/Base TEST_SRC_LIST)
aux_source_directory(Spacy/Util/Mixins TEST_SRC_LIST)

find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
  aux_source_directory(Spacy/Adapter/Eigen TEST_SRC_LIST)
endif()

if(Kaskade)
  include_directories(/home/lars/Libraries/Kaskade7.3
                      /opt/acml5.3.1/gfortran64/include
                      /home/lars/Libraries/software/gcc-5.4.0/dune-2.4.0/include
                      /home/lars/Libraries/software/gcc-5.4.0/umfpack-5.4.0/include
                      /home/lars/Libraries/software/gcc-5.4.0/dune-2.4.0/include/ug)
  link_directories(/home/lars/Libraries/Kaskade7.3/libs
                   /opt/acml5.3.1/gfortran64/lib
                   /home/lars/Libraries/software/gcc-5.4.0/umfpack-5.4.0/lib
                   /home/lars/Libraries/software/gcc-5.4.0/dune-2.4.0/lib)
  aux_source_directory(Spacy/Adapter/Kaskade TEST_SRC_LIST)
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wwrite-strings -Wpedantic") #-Weffc++ ")
endif()

include(CTest)
enable_testing()
foreach(TEST ${TEST_SRC_LIST})
  get_filename_component(TEST_NAME ${TEST} NAME_WE)
  get_filename_component(TEST_DIR ${TEST} DIRECTORY)
  string(REGEX REPLACE "/" "_" TEST_DIR ${TEST_DIR})
  set(TEST_UNIQUE_NAME "${TEST_DIR}_${TEST_NAME}")
  add_executable(${TEST_UNIQUE_NAME} ${TEST} ${MOCK_SRC_LIST})
  target_link_libraries(${TEST_UNIQUE_NAME} spacy ${GTEST_LIBRARIES} pthread)
  if(Kaskade)
    target_link_libraries(${TEST_UNIQUE_NAME} kaskade boost_system boost_timer boost_thread dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX umfpack amd acml)
  endif()
  add_test(${TEST_UNIQUE_NAME} ${PROJECT_BINARY_DIR}/Test/${TEST_UNIQUE_NAME})
endforeach()

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
