dnl Checks for required headers and functions
dnl
dnl Version: 20200713

dnl Function to detect if libmsiecf dependencies are available
AC_DEFUN([AX_LIBMSIECF_CHECK_LOCAL],
  [dnl Check for internationalization functions in libmsiecf/libmsiecf_i18n.c
  AC_CHECK_FUNCS([bindtextdomain])
])

dnl Function to detect if msiecftools dependencies are available
AC_DEFUN([AX_MSIECFTOOLS_CHECK_LOCAL],
  [AC_CHECK_HEADERS([signal.h sys/signal.h unistd.h])

  AC_CHECK_FUNCS([close getopt setvbuf])

  AS_IF(
   [test "x$ac_cv_func_close" != xyes],
   [AC_MSG_FAILURE(
     [Missing function: close],
     [1])
  ])

  dnl Headers included in msiecftools/log_handle.c
  AC_CHECK_HEADERS([stdarg.h varargs.h])

  AS_IF(
    [test "x$ac_cv_header_stdarg_h" != xyes && test "x$ac_cv_header_varargs_h" != xyes],
    [AC_MSG_FAILURE(
      [Missing headers: stdarg.h and varargs.h],
      [1])
  ])
])

dnl Function to check if DLL support is needed
AC_DEFUN([AX_LIBMSIECF_CHECK_DLL_SUPPORT],
  [AS_IF(
    [test "x$enable_shared" = xyes && test "x$ac_cv_enable_static_executables" = xno],
    [AS_CASE(
      [$host],
      [*cygwin* | *mingw* | *msys*],
      [AC_DEFINE(
        [HAVE_DLLMAIN],
        [1],
        [Define to 1 to enable the DllMain function.])
      AC_SUBST(
        [HAVE_DLLMAIN],
        [1])

      AC_SUBST(
        [LIBMSIECF_DLL_EXPORT],
        ["-DLIBMSIECF_DLL_EXPORT"])

      AC_SUBST(
        [LIBMSIECF_DLL_IMPORT],
        ["-DLIBMSIECF_DLL_IMPORT"])
      ])
    ])
  ])

