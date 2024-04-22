# Initialize
AC_INIT([qmckldgemm], [0.1])
AM_PROG_AR
LT_INIT
AC_CONFIG_MACRO_DIR([m4])

# Generate the Makefile
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

# Include macros
sinclude(acx_mkl.m4)
sinclude(ac_pthread.m4)
sinclude(libs.m4)
#sinclude(acx_openblas.m4)


# Provide special options for INTEL MKL
# We force the use of icc
AC_MSG_CHECKING([whether INTEL's MKL is enabled])
AC_ARG_ENABLE(mkl,
	[AS_HELP_STRING([--enable-mkl],
	[Use INTELs MKL for solvers and FFTs (default = no)])],
        enable_icc="yes"
        CC="icc"
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))

AC_LANG(C)
 
AC_PROG_FC
AC_PROG_CC
AC_PROG_CC_C_O
AM_PROG_CC_C_O

PKG_PROG_PKG_CONFIG([])
PKG_LIBS=""


# Generate config
AC_CONFIG_HEADERS([config.h])

# Provide special options for the MKL library
AC_ARG_WITH(mkl-dir,
	[AS_HELP_STRING([--with-mkl-dir=<MKL path>],
	[Provide an alternative path to the MKL library])])

AC_ARG_ENABLE(fortran,
	[AS_HELP_STRING([--enable-fortran],
	[Use Fortran in test suite])],
        [enable_fortran=yes],
        [enable_fortran=no])

AM_CONDITIONAL([HAVE_FORTRAN], [test "$enable_fortran" = "yes"])

AS_IF([test "$enable_fortran" = "yes"],
      [AC_PROG_FC_C_O
       AC_DEFINE(HAVE_FORTRAN, [1], [use Fortran in test suite])
      ])

# Provide special options for BLAS
AC_MSG_CHECKING([whether BLAS is enabled])
AC_ARG_ENABLE(blas,
	[AS_HELP_STRING([--enable-blas],
	[Use the BLAS library instead of ATLAS (default = no)])],
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))
AC_ARG_WITH(blas-libdir,
	[AS_HELP_STRING([--with-blas-libdir=<BLAS library path>],
	[Provide an alternative path to the BLAS library])])
AC_ARG_WITH(blas-incdir,
	[AS_HELP_STRING([--with-blas-incdir=<BLAS header dir>],
	[Provide an alternative path to the BLAS header directory])])


# Set flags for multithreading
#n_pthreads=4
#AC_ARG_ENABLE(threads,
#	[AS_HELP_STRING([--enable-threads@<:@=<max_number_of_threads>@:>@],
#	[Enable multhreading (on with up to 4 threads by default)])],
#    if test "$enableval" = "no"; then
#      use_pthreads="no"
#    else
#      use_pthreads="yes"
#      if test "$enableval" != "yes"; then
#        n_pthreads=$enableval
#      fi
#    fi,
#    use_pthreads=yes
#    )

# Enable linking options for making the executable as portable as possible.
AC_MSG_CHECKING([best linking option])
AC_ARG_ENABLE(best-link,
	[AS_HELP_STRING([--enable-best-link],
	[Choose the right combination of static and dynamic linking to make \
the executable as portable as possible (default = no)])],
	AC_MSG_RESULT([yes]),
	AC_MSG_RESULT([no]))

################ Actions to complete in case of multhreading ################
#AC_DEFINE_UNQUOTED(THREADS_NMAX, $n_pthreads,[Maximum number of POSIX threads])
#if test "$use_pthreads" = "yes"; then
#  AC_MSG_CHECKING([for multithreading])
#  AC_MSG_RESULT([maximum of $n_pthreads thread(s)])
#  AC_DEFINE(USE_THREADS, 1, [Triggers multhreading])
## CC, CFLAGS and LIBS are system and compiler-dependent
#  ACX_PTHREAD
#  CC="$PTHREAD_CC"
#  [AM_CFLAGS="$AM_CFLAGS $PTHREAD_CFLAGS -D_REENTRANT"]
#  LIBS="$PTHREAD_LIBS $LIBS"
#fi
#AM_CONDITIONAL(USE_THREADS, test $use_pthreads = "no")

############ handle the INTEL MKL library ###########
if test "$enable_mkl" = "yes"; then
  AC_MSG_CHECKING([Before ACX Call])
  convlibs=""
  ACX_MKL($with_mkl_dir,no,$enable_best_link,$convlibs)
  AC_MSG_CHECKING([for the INTEL MKL])
  if test "$MKL_WARN" == ""; then
    AC_MSG_RESULT([yes])
  else
    AC_MSG_RESULT([no])
    AC_MSG_WARN([$MKL_WARN])
  fi
  AM_CFLAGS="$AM_CFLAGS $MKL_CFLAGS "
  FCFLAGS=" $MKL_CFLAGS "
  AM_LDFLAGS="$AM_LDFLAGS $MKL_LDFLAGS "
  LIBS="$LIBS $MKL_LIBS"
else
######## Handle the BLAS library (linear algebra: BLAS + LAPACKe) ########
# In case there are BLAS kernels that are used by the example applications
# we may specify which library to use. Note that this is not used for StarPU
# itself.
    blas_lib=maybe
    #AC_ARG_ENABLE(blas-lib,
    # [  --enable-blas-lib[=blaslibname]:
    #                      none [default]: no BLAS lib is used
    #                      atlas: use ATLAS library
    #                      goto: use GotoBLAS library
    #		      mkl: use MKL library (you may need to set specific CFLAGS and LDFLAGS with --with-mkl-cflags and --with-mkl-ldflags)],
    # [
    #     if   test "x$enableval" = "xatlas" ; then
    #        blas_lib=atlas
    #     elif test "x$enableval" = "xgoto" ; then
    #        blas_lib=goto
    #     elif test "x$enableval" = "xopenblas" ; then
    #        blas_lib=openblas
    #     elif test "x$enableval" = "xnone" ; then
    #        blas_lib=none
    #     elif test "x$enableval" = "xmkl" ; then
    #        blas_lib=mkl
    #     elif test "x$enableval" = "xarmpl" ; then
    #        blas_lib=armpl
    #     elif test x$enableval = xno; then
    #	blas_lib=none
    #     else
    #        echo
    #        echo "Error!"
    #        echo "Unknown BLAS library"
    #        exit -1
    #     fi
    # ])

    #if test x$blas_lib = xmaybe -o x$blas_lib = xgoto; then
    #   AC_ARG_WITH(goto-dir, [AS_HELP_STRING([--with-goto-dir=<dir>], [specify GotoBLAS lib location])],
    #   	[
    #	    blas_lib=goto
    #	    gotodir=$withval
    #	    AC_SUBST(GOTODIR, $gotodir)
    #
    #	    CPPFLAGS="${CPPFLAGS} -I$gotodir/ "
    #	    LDFLAGS="${LDFLAGS} -L$gotodir/ "
    #	]
    #	)
    #
    #fi

    #if test x$blas_lib = xmaybe -o x$blas_lib = xatlas; then
    #    AC_ARG_WITH(atlas-dir, [AS_HELP_STRING([--with-atlas-dir=<dir>], [specify ATLAS lib location])],
    #	[
    #	    AC_MSG_CHECKING(STARPU_ATLAS location)
    #	    blas_lib=atlas
    #	    atlasdir=$withval
    #	    AC_MSG_RESULT($atlasdir)
    #	    AC_SUBST(ATLASDIR, $atlasdir)
    #
    #	    CPPFLAGS="${CPPFLAGS} -I$atlasdir/include/ "
    #	    LDFLAGS="${LDFLAGS} -L$atlasdir/lib/ "
    #	]
    #    )
    #
    #    if test x$blas_lib = xatlas; then
    #	# test whether STARPU_ATLAS is actually available
    #	AC_CHECK_HEADER([cblas.h],,AC_MSG_ERROR([cannot find atlas headers]))
    #    fi
    #fi

    if test x$blas_lib = xmaybe -o x$blas_lib = xopenblas; then
        PKG_CHECK_MODULES([OPENBLAS], [openblas],
        				  [PKG_CHECK_MODULES([BLAS_OPENBLAS], [openblas],
    				  				      [AC_DEFINE([HAVE_OPENBLAS], [1], [Define to 1 if you use the openblas library.])
    					          		       AC_SUBST([HAVE_OPENBLAS], [1])
    						          	       CFLAGS="${CFLAGS} ${OPENBLAS_CFLAGS} ${BLAS_OPENBLAS_CFLAGS} "
    							               LIBS="${LIBS} ${OPENBLAS_LIBS} ${BLAS_OPENBLAS_LIBS} "
    								       blas_lib=openblas
          								      ],
    								      [ if test x$blas_lib = xopenblas; then
    				  				      	   AC_DEFINE([HAVE_OPENBLAS], [1], [Define to 1 if you use the openblas library.])
    					          		       	   AC_SUBST([HAVE_OPENBLAS], [1])
    									fi
    				  				      ])
    				  ],
    				  [ if test x$blas_lib = xopenblas; then
    				       AC_DEFINE([HAVE_OPENBLAS], [1], [Define to 1 if you use the openblas library.])
    				       AC_SUBST([HAVE_OPENBLAS], [1])
          				    fi
                                      ] )
    fi

    #if test x$blas_lib = xmaybe -o x$blas_lib = xmkl; then
    #    # Should we use MKL ?
    #    if test -n "$MKLROOT" ; then
    #        CPPFLAGS="${CPPFLAGS} -I$MKLROOT/include"
    #	case $host_vendor in
    #	    *1om) mkl_plat=mic ;;
    #	    *)    mkl_plat=intel64 ;;
    #	esac
    #	SAVED_LIBS=$LIBS
    #	BLAS_LDFLAGS="-L$MKLROOT/lib/$mkl_plat -lmkl_intel_lp64 -lmkl_sequential -lmkl_core -lm -lpthread -ldl"
    #	LIBS="$LIBS $BLAS_LDFLAGS"
    #	AC_LINK_IFELSE(
    #		[AC_LANG_PROGRAM([[
    #			#include <mkl.h>
    #		]], [[ ]])],
    #		[ blas_lib=mkl ],
    #		[ BLAS_LDFLAGS="" ],
    #	)
    #	LIBS=$SAVED_LIBS
    #    fi
    #    AC_ARG_WITH(mkl-cflags, [AS_HELP_STRING([--with-mkl-cflags], [specify MKL compilation flags])],
    #	[
    #        CPPFLAGS="${CPPFLAGS} $withval"
    #	blas_lib=mkl
    #	])
    #
    #    AC_ARG_WITH(mkl-ldflags, [AS_HELP_STRING([--with-mkl-ldflags], [specify MKL linking flags])],
    #	[
    #	BLAS_LDFLAGS="$withval"
    #	blas_lib=mkl
    #	])
    #    if test x$blas_lib = xmkl; then
    #	AC_DEFINE(HAVE_MKL, [1], [use MKL library])
    #    fi
    #fi

    if test x$blas_lib = xmaybe; then
        #perhaps it is possible to use some BLAS lib from the system
        use_system_blas=no
        STARPU_SEARCH_LIBS(BLAS,[sgemm_],[blas],use_system_blas=yes,,)
        if test x$use_system_blas = xyes; then
            AC_DEFINE(HAVE_SYSTEM_BLAS, [1], [use refblas library])
    	blas_lib=system
        elif test x"$BLAS_LIBS" != x; then
            AC_DEFINE(HAVE_SYSTEM_BLAS, [1], [use user defined library])
            STARPU_BLAS_LDFLAGS="$BLAS_LIBS"
      	    LDFLAGS="${LDFLAGS} $BLAS_LIBS"
            blas_lib=system
            AC_ARG_VAR([BLAS_LIBS], [linker flags for blas])
        else
    	blas_lib=none
        fi
    fi

    if test x$blas_lib = xsystem; then
        AC_CHECK_HEADER([cblas.h], [have_cblas_h=yes], [have_cblas_h=no])
      	LDFLAGS="${LDFLAGS} $ac_cv_search_sgemm_"
    fi
fi
##############################################################################

PKG_LIBS="$PKG_LIBS $LIBS"


AC_SUBST(CFLAGS)
AC_SUBST(PKG_LIBS)
AC_SUBST(AM_CFLAGS)
AC_SUBST(AM_CPPFLAGS)
AC_SUBST(AM_LDFLAGS)

# Generate Makefile from Makefile.in
AC_CONFIG_FILES([Makefile
                 pkgconfig/qmckldgemm.pc])

# Done
AC_OUTPUT

echo \
"-------------------------------------------------

${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Prefix ........:  ${prefix}
CC ............:  ${CC}
FC ............:  ${FC}
CPPFLAGS ......:  ${CPPFLAGS}
CFLAGS ........:  ${CFLAGS}
FCFLAGS........:  ${FCFLAGS}
LDFLAGS .......:  ${LDFLAGS}
LIBS ..........:  ${LIBS}

Package features:
  Intel ICC    ...........:  ${enable_icc}
  Blas         ...........:  ${enable_blas}
  MKL enabled  ...........:  ${enable_mkl}
  Fortran enabled  .......:  ${enable_fortran}


Now type 'make @<:@<target>@:>@'
where the optional <target> is:
  all           - build C library and Fortran binding
  check	        - run unit tests
  install       - install the library
--------------------------------------------------"
