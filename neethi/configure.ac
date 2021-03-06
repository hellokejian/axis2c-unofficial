dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(neethi-src, 1.6.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
dnl AM_INIT_AUTOMAKE([tar-ustar])
AM_INIT_AUTOMAKE
m4_ifdef([_A][M_PROG_TAR],[_A][M_SET_OPTION([tar-ustar])])
AC_PREFIX_DEFAULT(/usr/local/neethi)

dnl Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LIBTOOL
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET

dnl check for flavours of varargs macros (test from GLib)
AC_MSG_CHECKING(for ISO C99 varargs macros in C)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(...) a(1,__VA_ARGS__)
call_a(2,3);
],axis2c_have_iso_c_varargs=yes,axis2c_have_iso_c_varargs=no)
AC_MSG_RESULT($axis2c_have_iso_c_varargs)

dnl Checks for libraries.
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(pthread, pthread_join)

#CFLAGS="$CFLAGS -ansi -Wall -D_LARGEFILE64_SOURCE -Wno-implicit-function-declaration"
CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE"
if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -Wall -Wno-implicit-function-declaration"
fi


dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])
AC_CHECK_HEADERS([sys/socket.h])
dnl This is a check to see if we are running MacOS X
dnl It may be better to do a Darwin check
AC_CHECK_HEADER([sys/appleapiopts.h],
               [AC_DEFINE([IS_MACOSX],[1],[Define to 1 if compiling on MacOS X])],
               [])

dnl Checks for typedefs, structures, and compiler characteristics.
dnl AC_C_CONST

dnl Checks for library functions.
dnl AC_FUNC_MALLOC
dnl AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])

VERSION_NO="6:0:6"
VERSION_INFO="-version-info ${VERSION_NO}"

case $host in
  *mingw*)
    VERSION_INFO="-avoid-version"
    CFLAGS="$CFLAGS -DWIN32 -D_WINDOWS -D_MBCS -DAXIS2_DECLARE_EXPORT -D_CRT_SECURE_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS"
    LDFLAGS="$LDFLAGS -no-undefined"
  ;;
  *darwin*)
  ;;
  *)
	SAVED_CFLAGS="$CFLAGS"
	CFLAGS=-Wno-unused-but-set-variable
	AC_MSG_CHECKING([whether CC supports -Wno-unused-but-set-variable])
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([])],
		[AC_MSG_RESULT([yes])]
		[AM_CFLAGS=-Wno-unused-but-set-variable],
		[AC_MSG_RESULT([no])]
	)
	CFLAGS="$SAVED_CFLAGS"
	AC_SUBST([AM_CFLAGS])
  ;;
esac

case $CC in
  *android*)
    VERSION_INFO="-avoid-version"
    CFLAGS="${CFLAGS//-ansi/}"
  ;;
esac

AC_SUBST(VERSION_NO)
AC_SUBST(VERSION_INFO)


AC_CONFIG_FILES([Makefile \
    src/Makefile \
    src/secpolicy/Makefile \
    src/secpolicy/model/Makefile \
    src/secpolicy/builder/Makefile \
    src/rmpolicy/Makefile \
    test/Makefile
    ])
    
AC_OUTPUT
