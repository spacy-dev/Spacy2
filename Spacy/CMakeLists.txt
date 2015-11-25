set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake
    )
    
find_package(DOLFIN)
if(DOLFIN_FOUND)
  include_directories(${DOLFIN_INCLUDE_DIRS})
  include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
  aux_source_directory(Adapter/FEniCS SRC_LIST)
endif()
find_package(Eigen3)
if(EIGEN3_FOUND)
  include_directories(${EIGEN3_INCLUDE_DIR})
  aux_source_directory(Adapter/Eigen SRC_LIST)
endif()

aux_source_directory(. SRC_LIST)
aux_source_directory(Adapter/Scalar SRC_LIST)
aux_source_directory(Adapter/Generic SRC_LIST)
aux_source_directory(Algorithm SRC_LIST)
aux_source_directory(Algorithm/Functions_1D SRC_LIST)
aux_source_directory(Algorithm/CG SRC_LIST)
aux_source_directory(Algorithm/CompositeStep SRC_LIST)
aux_source_directory(Algorithm/Newton SRC_LIST)
aux_source_directory(Algorithm/Rothe SRC_LIST)
aux_source_directory(Spaces/RealSpace SRC_LIST)
aux_source_directory(Spaces/ProductSpace SRC_LIST)
aux_source_directory(Util SRC_LIST)
aux_source_directory(Util/Base SRC_LIST)
aux_source_directory(Util/Mixins SRC_LIST)
aux_source_directory(Util/Exceptions SRC_LIST)


add_library(spacy ${SRC_LIST})

install(TARGETS spacy
	EXPORT SpacyTargets
	ARCHIVE DESTINATION "${PROJECT_BINARY_DIR}/Spacy" COMPONENT lib
	PUBLIC_HEADER DESTINATION "${PROJECT_SOURCE_DIR}"
	COMPONENT dev)
