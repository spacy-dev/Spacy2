# - Config file for the Spacy package 
set(Spacy_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(Spacy_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libspacy.a)
set(Spacy_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(Spacy_LIBS spacy)
set(Spacy_CXX_FLAGS "-std=c++1y")
set(Spacy_CXX_DEFINITIONS "-std=c++1y")
set(Spacy_CXX_STANDARD 14)
