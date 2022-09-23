# Installation

The simplest way to obtain the source files of QMCkl\_dgemm is to download a source
distribution. This particular repository is for maintainers, who write custom kernels.

## For maintainers

### Use Intel MKL for testing

```
./autogen.sh

./configure --enable-mkl CC=icc FC=ifort

make
make check
```
### Use OpebBlas or system Blas

```
./autogen.sh

./configure --enable-blas CC=gcc FC=gfortran

make
make check
make install
```

### Enable Fortran tests

```
./autogen.sh

./configure --enable-blas CC=gcc FC=gfortran --enable-fortran

make
make check
make install
```

