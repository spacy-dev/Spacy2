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
  include("/home/lars/.hashdist/bld/dolfin/6qf3p654zczj/share/dolfin/cmake/DOLFINTargets.cmake")
endif()

# Compilers
set(DOLFIN_CXX_COMPILER "/usr/bin/c++")

# Compiler defintions
set(DOLFIN_CXX_DEFINITIONS "-DDOLFIN_VERSION=\"1.6.0\";-DNDEBUG;-DDOLFIN_SIZE_T=8;-DDOLFIN_LA_INDEX_SIZE=4;-DHAS_HDF5;-D_LARGEFILE_SOURCE;-D_LARGEFILE64_SOURCE;-D_BSD_SOURCE;-DHAS_SLEPC;-DHAS_PETSC;-DENABLE_PETSC_TAO;-DHAS_UMFPACK;-DHAS_CHOLMOD;-DHAS_SCOTCH;-DHAS_PARMETIS;-DHAS_ZLIB;-DHAS_MPI;-DHAS_OPENMP;-DHAS_VTK")

# Compiler flags
set(DOLFIN_CXX_FLAGS " -std=c++11  -Wno-deprecated   -fopenmp")

# Linker flags
set(DOLFIN_LINK_FLAGS " -L/home/lars/.hashdist/bld/blas/ga3yotglkobk/lib -Wl,-rpath=/home/lars/.hashdist/bld/blas/ga3yotglkobk/lib -L/home/lars/.hashdist/bld/boost/gcxncmarpzpi/lib -Wl,-rpath=/home/lars/.hashdist/bld/boost/gcxncmarpzpi/lib -L/home/lars/.hashdist/bld/bzip2/x77hkrpcbch3/lib -Wl,-rpath=/home/lars/.hashdist/bld/bzip2/x77hkrpcbch3/lib -L/home/lars/.hashdist/bld/cmake/oxbcdkcislsb/lib -Wl,-rpath=/home/lars/.hashdist/bld/cmake/oxbcdkcislsb/lib -L/home/lars/.hashdist/bld/eigen/jcfod5s5rny7/lib -Wl,-rpath=/home/lars/.hashdist/bld/eigen/jcfod5s5rny7/lib -L/home/lars/.hashdist/bld/ffc/4fdupcbuk3lr/lib -Wl,-rpath=/home/lars/.hashdist/bld/ffc/4fdupcbuk3lr/lib -L/home/lars/.hashdist/bld/fiat/unmnxain4vxi/lib -Wl,-rpath=/home/lars/.hashdist/bld/fiat/unmnxain4vxi/lib -L/home/lars/.hashdist/bld/hdf5/227wp7vys2oi/lib -Wl,-rpath=/home/lars/.hashdist/bld/hdf5/227wp7vys2oi/lib -L/home/lars/.hashdist/bld/instant/ak5zz4bk4xew/lib -Wl,-rpath=/home/lars/.hashdist/bld/instant/ak5zz4bk4xew/lib -L/home/lars/.hashdist/bld/libxml2/mlk7x2dhjiud/lib -Wl,-rpath=/home/lars/.hashdist/bld/libxml2/mlk7x2dhjiud/lib -L/home/lars/.hashdist/bld/mpi/gmkx3ftk3tz2/lib -Wl,-rpath=/home/lars/.hashdist/bld/mpi/gmkx3ftk3tz2/lib -L/home/lars/.hashdist/bld/numpy/4265m45hn6du/lib -Wl,-rpath=/home/lars/.hashdist/bld/numpy/4265m45hn6du/lib -L/home/lars/.hashdist/bld/blas/ga3yotglkobk/lib -Wl,-rpath=/home/lars/.hashdist/bld/blas/ga3yotglkobk/lib -L/home/lars/.hashdist/bld/parmetis/wqtkpsp6qboj/lib -Wl,-rpath=/home/lars/.hashdist/bld/parmetis/wqtkpsp6qboj/lib -L/home/lars/.hashdist/bld/patchelf/k3rloj265ogt/lib -Wl,-rpath=/home/lars/.hashdist/bld/patchelf/k3rloj265ogt/lib -L/home/lars/.hashdist/bld/petsc/tu34imkveoli/lib -Wl,-rpath=/home/lars/.hashdist/bld/petsc/tu34imkveoli/lib -L/home/lars/.hashdist/bld/petsc4py/gzd3neejyhhs/lib -Wl,-rpath=/home/lars/.hashdist/bld/petsc4py/gzd3neejyhhs/lib -L/home/lars/.hashdist/bld/pkg-config/pjksilnbb4iy/lib -Wl,-rpath=/home/lars/.hashdist/bld/pkg-config/pjksilnbb4iy/lib -L/home/lars/.hashdist/bld/ply/glvoo3mf3arx/lib -Wl,-rpath=/home/lars/.hashdist/bld/ply/glvoo3mf3arx/lib -L/home/lars/.hashdist/bld/python/iseantsfuvdz/lib -Wl,-rpath=/home/lars/.hashdist/bld/python/iseantsfuvdz/lib -L/home/lars/.hashdist/bld/scotch/vm3qczcxeszc/lib -Wl,-rpath=/home/lars/.hashdist/bld/scotch/vm3qczcxeszc/lib -L/home/lars/.hashdist/bld/six/xs56jvn42hmk/lib -Wl,-rpath=/home/lars/.hashdist/bld/six/xs56jvn42hmk/lib -L/home/lars/.hashdist/bld/slepc/ueoqvrxbddpc/lib -Wl,-rpath=/home/lars/.hashdist/bld/slepc/ueoqvrxbddpc/lib -L/home/lars/.hashdist/bld/slepc4py/7mskop2nbjop/lib -Wl,-rpath=/home/lars/.hashdist/bld/slepc4py/7mskop2nbjop/lib -L/home/lars/.hashdist/bld/suitesparse/psbs2mcuk4dc/lib -Wl,-rpath=/home/lars/.hashdist/bld/suitesparse/psbs2mcuk4dc/lib -L/home/lars/.hashdist/bld/swig/go7z4z6mxyka/lib -Wl,-rpath=/home/lars/.hashdist/bld/swig/go7z4z6mxyka/lib -L/home/lars/.hashdist/bld/sympy/64ind65ynn4e/lib -Wl,-rpath=/home/lars/.hashdist/bld/sympy/64ind65ynn4e/lib -L/home/lars/.hashdist/bld/ufl/5kxhbpl43jz5/lib -Wl,-rpath=/home/lars/.hashdist/bld/ufl/5kxhbpl43jz5/lib -L/home/lars/.hashdist/bld/vtk/3meqpbk2xrzw/lib -Wl,-rpath=/home/lars/.hashdist/bld/vtk/3meqpbk2xrzw/lib -L/home/lars/.hashdist/bld/zlib/v6anirfvk2op/lib -Wl,-rpath=/home/lars/.hashdist/bld/zlib/v6anirfvk2op/lib  ")

# Include directories
set(DOLFIN_INCLUDE_DIRS "/home/lars/.hashdist/bld/dolfin/6qf3p654zczj/include")

# Third party include directories
set(DOLFIN_3RD_PARTY_INCLUDE_DIRS "/home/lars/.hashdist/bld/ffc/4fdupcbuk3lr/include;/home/lars/.hashdist/bld/slepc/ueoqvrxbddpc/include;/home/lars/.hashdist/bld/suitesparse/psbs2mcuk4dc/include/suitesparse;/home/lars/.hashdist/bld/suitesparse/psbs2mcuk4dc/include/suitesparse;/home/lars/.hashdist/bld/scotch/vm3qczcxeszc/include;/home/lars/.hashdist/bld/zlib/v6anirfvk2op/include;/home/lars/.hashdist/bld/mpi/gmkx3ftk3tz2/include;/home/lars/.hashdist/bld/eigen/jcfod5s5rny7/include/eigen3;/home/lars/.hashdist/bld/boost/gcxncmarpzpi/include;/home/lars/.hashdist/bld/hdf5/227wp7vys2oi/include;/home/lars/.hashdist/bld/petsc/tu34imkveoli/include;/home/lars/.hashdist/bld/hypre/3sialo4py6ds/include;/home/lars/.hashdist/bld/superlu_dist/yff3crdm7naz/include;/home/lars/.hashdist/bld/suitesparse/psbs2mcuk4dc/include/suitesparse;/home/lars/.hashdist/bld/parmetis/wqtkpsp6qboj/include;/home/lars/.hashdist/bld/scotch/vm3qczcxeszc/include;/home/lars/.hashdist/bld/libxml2/mlk7x2dhjiud/include;/home/lars/.hashdist/bld/mpi/gmkx3ftk3tz2/include;/home/lars/.hashdist/bld/parmetis/wqtkpsp6qboj/include;/home/lars/.hashdist/bld/mpi/gmkx3ftk3tz2/include;/home/lars/.hashdist/bld/vtk/3meqpbk2xrzw/include/vtk-5.10")

# Python include directories
set(DOLFIN_PYTHON_INCLUDE_DIRS "/home/lars/.hashdist/bld/numpy/4265m45hn6du/lib/python2.7/site-packages/numpy/core/include;/home/lars/.hashdist/bld/python/iseantsfuvdz/include/python2.7;/home/lars/.hashdist/bld/petsc4py/gzd3neejyhhs/lib/python2.7/site-packages/petsc4py/include;/home/lars/.hashdist/bld/slepc4py/7mskop2nbjop/lib/python2.7/site-packages/slepc4py/include")

# Python definitions
set(DOLFIN_PYTHON_DEFINITIONS "-DHAS_PETSC4PY;-DHAS_SLEPC4PY;-DNUMPY_VERSION_MAJOR=1;-DNUMPY_VERSION_MINOR=8;-DNUMPY_VERSION_MICRO=2;-DNPY_NO_DEPRECATED_API=NPY_1_8_API_VERSION")

# Python libraries
set(DOLFIN_PYTHON_LIBRARIES "/home/lars/.hashdist/bld/python/iseantsfuvdz/lib/libpython2.7.so")

set(DOLFIN_SWIG_EXECUTABLE "/home/lars/.hashdist/bld/swig/go7z4z6mxyka/bin/swig")

# DOLFIN library
set(DOLFIN_LIBRARIES dolfin)

# Version
set(DOLFIN_VERSION_MAJOR "1")
set(DOLFIN_VERSION_MINOR "6")
set(DOLFIN_VERSION_MICRO "0")
set(DOLFIN_VERSION_STR   "1.6.0")

# The location of the UseDOLFIN.cmake file
set(DOLFIN_USE_FILE "/home/lars/.hashdist/bld/dolfin/6qf3p654zczj/share/dolfin/cmake/UseDOLFIN.cmake")
