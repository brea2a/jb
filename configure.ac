AC_PREREQ([2.63])
AC_INIT([jo], [1.2], [jpmens@gmail.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SRCDIR([jo.c])

# Checks for programs.
AC_PROG_CC

# Checks for header files.
AC_USE_SYSTEM_EXTENSIONS
AC_CHECK_HEADERS([stddef.h stdint.h stdlib.h string.h unistd.h stdbool.h])

# Checks for library functions.
# AC_FUNC_MALLOC
# AC_FUNC_REALLOC
AC_FUNC_STRTOD
AC_CHECK_FUNCS([strchr strrchr strlcpy strlcat snprintf pledge err errx])

AM_INIT_AUTOMAKE([foreign -Wall])
AM_SILENT_RULES([yes])
AC_REQUIRE_AUX_FILE([tap-driver.sh])

AC_ARG_VAR(PANDOC, [pandoc path])
AC_PATH_PROG(PANDOC, [pandoc], [])
if test -z "$PANDOC"
then
  AC_MSG_WARN([pandoc not found, man pages rebuild will not be possible])
fi
AM_CONDITIONAL([USE_PANDOC], [test -n "$PANDOC"])

AC_CONFIG_FILES([Makefile tests/jo.07.sh])
AC_OUTPUT

echo "
  Jo.............: version $PACKAGE_VERSION
  Prefix.........: $prefix
  C compiler.....: $CC $CFLAGS $CPPFLAGS
  Pandoc.........: ${PANDOC:-NONE}

  Now type 'make @<:@<target>@:>@'
    where the optional <target> is:
      all                - build all binaries
      check              - run the tests
      install            - install everything
"
