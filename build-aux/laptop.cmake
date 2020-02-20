# Initial cache list for laptop
#
# Usage: cmake -C /path/to/this/file /path/to/DCA/source -D<option>=<value> ...

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Debug Type")

# Use jsrun for executing the tests.
set(TEST_RUNNER "mpiexec" CACHE STRING "Command for executing (MPI) programs.")
set(MPIEXEC_NUMPROC_FLAG "-np" CACHE STRING
  "Flag used by TEST_RUNNER to specify the number of processes.")

# Enable the GPU support.
option(DCA_WITH_CUDA "Enable GPU support." OFF)

set(LAPACK_LIBRARIES /home/weile/project/dev/install/lapck/liblapack.so;/home/weile/project/dev/install/lapck/libblas.so CACHE FILEPATH "Libraries to link against to use LAPACK.")


# FFTW paths.
set(FFTW_INCLUDE_DIRS /home/weile/project/dev/install/fftw3/include CACHE PATH "Path to fftw3.h.")
set(FFTW_LIBRARIES /home/weile/project/dev/install/fftw3/lib/libfftw3.so CACHE FILEPATH "The FFTW3(-compatible) library.")

option(DCA_WITH_TESTS_FAST "enable fast tests" ON)
option(DCA_WITH_TESTS_EXTENSIVE "Enable extensive tests" ON)
option(DCA_WITH_THREADED_SOLVER "Enable threaded solver" ON)
set(FFTW_ROOT /home/weile/project/dev/install/fftw3/ CACHE PATH "Path to fftw3.h.")
set(DART_TESTING_TIMEOUT 120)

set(DCA_WITH_HPX ON)
set(HPX_DIR /home/weile/project/dev/install/hpx_Debug/lib/cmake/HPX CACHE PATH "PATH to HPX")