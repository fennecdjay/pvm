AC_PREREQ([2.71])
AC_INIT([PVM], [0.1], [https://github.com/apachejuice/pvm/issues])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_CONFIG_SRCDIR([src/config.h.in])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIRS([m4])

AC_SUBST([FORMATTED_VERSION], [pvm-$PACKAGE_VERSION])
TEST_CFLAGS="-Wall -Wextra -Werror -Wno-unused-function -Wno-unused-parameter -Wno-switch -Wno-pedantic -g"
PVM_CFLAGS=
for flag in TEST_CFLAGS; do
    AX_CHECK_COMPILE_FLAG([$flag], [PVM_CFLAGS="$PVM_CFLAGS $flag"])
done

# Checks for programs.
AC_PROG_CC
AC_PROG_CC_C_O
LT_INIT

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([inttypes.h stdint.h])
AC_CHECK_HEADERS([unicode/umachine.h], [], [
    AC_MSG_ERROR([libicu required but not found])
])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN([], [
    AC_DEFINE_UNQUOTED(
        [is_lil_endian_uwu],
        [1],
        [host system is little endian]
    )
], [])

AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([strerror])

AC_CONFIG_FILES([
    Makefile
    src/Makefile
])

AC_OUTPUT
