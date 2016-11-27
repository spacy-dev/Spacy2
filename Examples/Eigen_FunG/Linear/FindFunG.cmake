# - Try to find FunG
# Once done this will define
#  FUNG_FOUND - System has FunG
#  FUNG_INCLUDE_DIRS - The FunG include directories
#  FUNG_LIBRARIES - The libraries needed to use FunG
#  FUNG_DEFINITIONS - Compiler switches required for using FunG

find_path(FUNG_INCLUDE_DIR fung/fung.hh )

set(FUNG_INCLUDE_DIRS ${FUNG_INCLUDE_DIR} )

set(FUNG_CXX_OPTIONS "-std=c++1y")

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set FUNG_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(FunG  DEFAULT_MSG
                                  FUNG_INCLUDE_DIR)

mark_as_advanced(FUNG_INCLUDE_DIR)