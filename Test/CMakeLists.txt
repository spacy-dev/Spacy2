find_package(GTest REQUIRED)

include_directories( ${GTEST_INCLUDE_DIRS} )

aux_source_directory(. TEST_SRC_LIST)
aux_source_directory(Mock TEST_SRC_LIST)
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
  include_directories(/home/lars/Projects/OpenSource/Kaskade/Kaskade7.3
                      /home/lars/libs/acml/acml5.3.1/gfortran64/include
                      /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost-1.59.0/include
                      /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/include
                      /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/umfpack-5.4.0/include
                      /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/include/ug)
  link_directories(/home/lars/Projects/OpenSource/Kaskade/Kaskade7.3/libs
                   /home/lars/libs/acml/acml5.3.1/gfortran64/lib
                   /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/boost-1.59.0/lib
                   /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/umfpack-5.4.0/lib
                   /home/lars/Projects/OpenSource/Kaskade/software/gcc-5.2.0/dune-2.4.0/lib)
  aux_source_directory(Spacy/Adapter/Kaskade TEST_SRC_LIST)
endif()

add_executable(unit_tests ${TEST_SRC_LIST})
target_link_libraries(unit_tests spacy ${GTEST_LIBRARIES} pthread)

if(Kaskade)
  target_link_libraries(unit_tests kaskade boost_system boost_timer boost_thread dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX umfpack amd acml)
#    target_link_libraries(unit_tests kaskade dunegrid dunecommon dunegeometry ugS3 ugS2 ugL3 ugS2 devS devX boost_signals boost_program_options boost_system boost_timer boost_thread boost_chrono acml)
endif()

include(CTest)
enable_testing()
add_test(test ${PROJECT_BINARY_DIR}/Test/unit_tests)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND}
DEPENDS ${PROJECT_BINARY_DIR}/Test/unit_tests)
