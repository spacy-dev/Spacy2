set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}/cmake/Modules
    )
    
find_package(DOLFIN)
if(DOLFIN_FOUND)
  include_directories(${DOLFIN_INCLUDE_DIRS})
  include_directories(SYSTEM ${DOLFIN_3RD_PARTY_INCLUDE_DIRS})
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Woverloaded-virtual -Wwrite-strings -Wpedantic -std=c++1y -Weffc++")
endif()
    
    
# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib CACHE PATH "Installation directory for libraries")
set(INSTALL_INCLUDE_DIR include CACHE PATH "Installation directory for header files") 
if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/CMake/VSpace)
endif()
set(INSTALL_CMAKE_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

aux_source_directory(. SRC_LIST)    
aux_source_directory(Adapter/FEniCS SRC_LIST)
aux_source_directory(Algorithm SRC_LIST)
aux_source_directory(Algorithm/Functions_1D SRC_LIST)
aux_source_directory(Algorithm/CG SRC_LIST)
aux_source_directory(Algorithm/CompositeStep SRC_LIST)
aux_source_directory(Algorithm/Newton SRC_LIST)
aux_source_directory(Spaces/RealSpace SRC_LIST)
aux_source_directory(Spaces/ProductSpace SRC_LIST)
aux_source_directory(Util SRC_LIST)
aux_source_directory(Util/Base SRC_LIST)
aux_source_directory(Util/Mixins SRC_LIST)
aux_source_directory(Util/Exceptions SRC_LIST)


add_library(vspace ${SRC_LIST})

install(TARGETS vspace
	EXPORT VSpaceTargets
	LIBRARY DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib 
	ARCHIVE DESTINATION "${INSTALL_LIB_DIR}" COMPONENT lib
	PUBLIC_HEADER DESTINATION "${INSTALL_INCLUDE_DIR}"
	COMPONENT dev)