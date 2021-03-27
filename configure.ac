AC_PREREQ([2.71])
AC_INIT([pvm], [0.1], [https://github.com/apachejuice/pvm/issues])
AM_INIT_AUTOMAKE([subdir-objects foreign])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([src/config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_UINT8_T
AC_TYPE_UINT32_T

# Checks for library functions.
AC_CHECK_FUNC([vasprintf], [
    AC_DEFINE([HAVE_VASPRINTF], [], [vasprintf was found on the system])
], [
    AC_MSG_WARN([vasprintf not found, using unofficial implementation])
])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
