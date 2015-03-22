#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([HPkit], [0.2.5], [giulio@rapgenic.com], [hpkit], [www.rapgenic.com/hpkit])
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_SRCDIR([src/])
AM_CONFIG_HEADER([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES(XML, libxml-2.0 >= 2.4)

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h termios.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([memset])

AC_CONFIG_FILES([Makefile])
AC_DEFINE([STR_MAXLEN], [100], ["Maximum number of characters per string"])
AC_DEFINE([BUF_MAXLEN], [400], ["Maximum number of characters per buffer"])
AC_OUTPUT
