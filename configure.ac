AC_PREREQ([2.69])
AC_INIT([jo], [0.6], [jpmens@gmail.com])
AC_CONFIG_SRCDIR([jo.c])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_USE_SYSTEM_EXTENSIONS
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h])

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strchr strrchr])

AM_INIT_AUTOMAKE
AM_SILENT_RULES([yes])

AC_ARG_VAR(PANDOC, [pandoc path])
AC_PATH_PROG(PANDOC, [pandoc], [])
if test -z "$PANDOC"
then
  AC_MSG_WARN([pandoc not found, man pages rebuild will not be possible])
fi

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
