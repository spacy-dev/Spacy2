# - Config file for the Spacy package 
set(Spacy_INCLUDE_DIRS ${CMAKE_INSTALL_PREFIX}/include)
set(Spacy_LIBRARIES ${CMAKE_INSTALL_PREFIX}/lib/libspacy.a)
set(SPACY_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set(SPACY_LIBRARIES spacy)
set(SPACY_CXX_OPTIONS "-std=c++1y")
