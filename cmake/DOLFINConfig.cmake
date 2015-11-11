# - Build details for DOLFIN: Dynamic Object-oriented Library for
# - FINite element computation
#
# This file has been automatically generated.

# FIXME: Check that naming conforms to CMake standards

# Library dependencies (contains definitions for IMPORTED targets)
# NOTE: DOLFIN demo CMakeLists.txt are written to be stand-alone, as
# well as the build system building the demo. Therefore, we need the
# below guard to avoid exporting the targets twice.
if (NOT TARGET dolfin)
  include("/usr/share/dolfin/cmake/DOLFINTargets.cmake")
endif()

# Compilers
set(DOLFIN_CXX_COMPILER "/usr/bin/x86_64-linux-gnu-g++")

# Compiler defintions
set(DOLFIN_CXX_DEFINITIONS "-DDOLFIN_VERSION=\"1.6.0\";-DNDEBUG;-DDOLFIN_SIZE_T=8;-DDOLFIN_LA_INDEX_SIZE=4;-DHAS_HDF5;-D_LARGEFILE_SOURCE;-D_LARGEFILE64_SOURCE;-D_BSD_SOURCE;-D_FORTIFY_SOURCE=2;-DHAS_SLEPC;-DHAS_PETSC;-DHAS_UMFPACK;-DHAS_CHOLMOD;-DHAS_SCOTCH;-DHAS_ZLIB;-DHAS_MPI;-DHAS_OPENMP;-DHAS_QT4;-DHAS_VTK;-DHAS_QVTK")

# Compiler flags
set(DOLFIN_CXX_FLAGS "-g -O2 -fstack-protector-strong -Wformat -Werror=format-security -D_FORTIFY_SOURCE=2  -std=c++11  -Wno-deprecated   -fopenmp")

# Linker flags
set(DOLFIN_LINK_FLAGS " -Wl,-Bsymbolic-functions -Wl,-z,relro  ")

# Include directories
set(DOLFIN_INCLUDE_DIRS "/usr/include")

# Third party include directories
set(DOLFIN_3RD_PARTY_INCLUDE_DIRS "/usr/include;/usr/lib/slepcdir/3.4.2;/usr/lib/slepcdir/3.4.2/linux-gnu-c-opt/include;/usr/lib/slepcdir/3.4.2/include;/usr/include/suitesparse;/usr/include/suitesparse;/usr/include/scotch;/usr/include;/usr/lib/openmpi/include;/usr/lib/openmpi/include/openmpi;/usr/include/eigen3;/usr/include;/usr/include/hdf5/openmpi;/usr/lib/petscdir/3.4.2/include;/usr/lib/petscdir/3.4.2/linux-gnu-c-opt/include;/usr/include;/usr/include/suitesparse;/usr/include/scotch;/usr/lib/x86_64-linux-gnu/hdf5/openmpi/include;/usr/lib/openmpi/include;/usr/lib/openmpi/include/openmpi;/usr/lib/openmpi/include;/usr/lib/openmpi/include/openmpi;/usr/include/qt4;/usr/include/vtk-5.8")

# Python include directories
set(DOLFIN_PYTHON_INCLUDE_DIRS "/usr/lib/python2.7/dist-packages/numpy/core/include;/usr/include/python2.7;/usr/lib/pymodules/python2.7/petsc4py/include")

# Python definitions
set(DOLFIN_PYTHON_DEFINITIONS "-DHAS_PETSC4PY;-DNUMPY_VERSION_MAJOR=1;-DNUMPY_VERSION_MINOR=8;-DNUMPY_VERSION_MICRO=2;-DNPY_NO_DEPRECATED_API=NPY_1_8_API_VERSION")

# Python libraries
set(DOLFIN_PYTHON_LIBRARIES "/usr/lib/x86_64-linux-gnu/libpython2.7.so")

set(DOLFIN_SWIG_EXECUTABLE "/usr/bin/swig3.0")

# DOLFIN library
set(DOLFIN_LIBRARIES dolfin)

# Version
set(DOLFIN_VERSION_MAJOR "1")
set(DOLFIN_VERSION_MINOR "6")
set(DOLFIN_VERSION_MICRO "0")
set(DOLFIN_VERSION_STR   "1.6.0")

# The location of the UseDOLFIN.cmake file
set(DOLFIN_USE_FILE "/usr/share/dolfin/cmake/UseDOLFIN.cmake")
