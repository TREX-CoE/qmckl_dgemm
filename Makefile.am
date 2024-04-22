AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4 --install

src_main_c = src/qmckl_dgemm.c src/utils.c src/kernel_avx2_16regs.c
src_main_f = src/qmckl_dgemm_f.f90
src_main_h = include/qmckl_dgemm.h include/qmckl_dgemm_private.h include/utils.h src/kernel_avx2_16regs.h src/bli_x86_asm_macros.h
test_main_c = tests/test_main.c

include_HEADERS = include/qmckl_dgemm.h
include_HEADERS += src/qmckl_dgemm_f.f90

AM_CPPFLAGS = -I$(top_builddir)/src -I$(top_builddir)/include
AM_CPPFLAGS += -I$(srcdir)/src -I$(srcdir)/include


lib_LTLIBRARIES = src/libqmckldgemm.la
src_libqmckldgemm_la_SOURCES = $(src_main_c) $(src_main_f) $(src_main_h)

TESTS = tests/test_main

tests_test_main_SOURCES = $(test_main_c) $(src_main_h)
tests_test_main_LDADD   = src/libqmckldgemm.la
tests_test_main_LDFLAGS  = -no-install

if HAVE_FORTRAN
test_main_f = tests/test.f90

TESTS += tests/test_f
tests_test_f_SOURCES = $(test_main_f) $(src_main_h)
tests_test_f_LDADD   = src/libqmckldgemm.la
tests_test_f_LDFLAGS = -no-install

endif

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pkgconfig/qmckldgemm.pc


check_PROGRAMS = $(TESTS)
CLEANFILES = qmckl_dgemm.mod
