# - Try to find Spacy
# Once done this will define
#  SPACY_FOUND - System has Spacy
#  SPACY_INCLUDE_DIRS - The Spacy include directories
#  SPACY_LIBRARIES - The libraries needed to use Spacy
#  SPACY_DEFINITIONS - Compiler switches required for using Spacy

find_path(SPACY_INCLUDE_DIR Spacy/vector.hh )
find_library(SPACY_LIBRARY NAMES spacy )

set(SPACY_LIBRARIES ${SPACY_LIBRARY} )
set(SPACY_LIBRARY_DIR ${SPACY_LIBRARY}/.. )
set(SPACY_INCLUDE_DIRS ${SPACY_INCLUDE_DIR} )

set(SPACY_CXX_OPTIONS "-std=c++1y")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set SPACY_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(Spacy  DEFAULT_MSG
                                  SPACY_LIBRARY SPACY_INCLUDE_DIR)

mark_as_advanced(SPACY_INCLUDE_DIR SPACY_LIBRARY )