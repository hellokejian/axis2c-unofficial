dnl run autogen.sh to generate the configure script.

AC_PREREQ(2.59)

AC_INIT(guththilac-src, 1.6.0)
AC_CANONICAL_SYSTEM
AM_CONFIG_HEADER(config.h)
dnl AM_INIT_AUTOMAKE([tar-ustar])
AM_INIT_AUTOMAKE
AC_PREFIX_DEFAULT(/usr/local/guththila)

m4_ifdef([_A][M_PROG_TAR],[_A][M_SET_OPTION([tar-ustar])])

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

AC_MSG_CHECKING(for GNUC varargs macros)
AC_TRY_COMPILE([],[
int a(int p1, int p2, int p3);
#define call_a(params...) a(1,params)
call_a(2,3);
],axis2c_have_gnuc_varargs=yes,axis2c_have_gnuc_varargs=no)
AC_MSG_RESULT($axis2c_have_gnuc_varargs)

dnl Output varargs tests
if test x$axis2c_have_iso_c_varargs = xyes; then
    AC_DEFINE(HAVE_ISO_VARARGS,1,[Have ISO C99 varargs macros])
fi
if test x$axis2c_have_gnuc_varargs = xyes; then
    AC_DEFINE(HAVE_GNUC_VARARGS,1,[Have GNU-style varargs macros])
fi

dnl Checks for libraries.
AC_CHECK_LIB(dl, dlopen)
AC_CHECK_LIB(pthread, pthread_join)

CFLAGS="$CFLAGS -D_LARGEFILE64_SOURCE -DAXIS2_GUTHTHILA_ENABLED"

if test "$GCC" = "yes"; then
    CFLAGS="$CFLAGS -ansi -g -O0 -Wall -Wno-implicit-function-declaration"
fi


dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdio.h stdlib.h string.h])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

dnl Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
#AC_CHECK_FUNCS([memmove])


UTILINC=$axis2_utilinc
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

AC_SUBST(UTILINC)
AC_SUBST(VERSION_NO)
AC_SUBST(VERSION_INFO)

AC_CONFIG_FILES([Makefile \
    src/Makefile \
    ])
    
AC_OUTPUT
