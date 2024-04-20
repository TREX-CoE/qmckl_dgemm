# QMCkl DGEMM: Native DGEMM library for Quantum Monte Carlo Kernel Library (QMCkl)

To clone the repository, use:
```
git clone https://github.com/trex-coe/qmckl_dgemm.git
```

# Installation

The simplest way to obtain the source files of ~QMCkl\_dgemm~ is to download a source
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

## Linking to your program

The `make install` command takes care of installing the ~QMCkl\_dgemm~ shared library on the user machine.
Once installed, add `-lqmckldgemm` to the list of compiler options.

In some cases (e.g. when using custom `prefix` during configuration), the QMCkl library might end up installed in a directory, which is absent in the default `$LIBRARY_PATH`.
In order to link the program against ~QMCkl\_dgemm~, the search paths can be modified as follows:

`export LIBRARY_PATH=$LIBRARY_PATH:<path_to_qmckl_dgemm>/lib`

(same holds for `$LD_LIBRARY_PATH`). The `<path_to_qmckl_dgemm>` has to be replaced with the prefix used during the installation.


------------------------------

![European flag](https://trex-coe.eu/sites/default/files/inline-images/euflag.jpg)
[TREX: Targeting Real Chemical Accuracy at the Exascale](https://trex-coe.eu) project has received funding from the European Union’s Horizon 2020 - Research and Innovation program - under grant agreement no. 952165. The content of this document does not represent the opinion of the European Union, and the European Union is not responsible for any use that might be made of such content.

