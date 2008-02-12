
#
#
#

AC_DEFUN([AC_CHECK_TIFF],[
	AC_ARG_WITH(tiffprefix,	AC_HELP_STRING([--with-tiffprefix], [TIFF installation prefix directory]),,
				with_tiffprefix="")

	TIFF_PREFIX="$with_tiffprefix"
	TIFF_INCLUDE=""
	TIFF_INCLUDE_PATH=""
	TIFF_LIB_PATH=""
	TIFF_LIBS=""

	AS_IF([test "x$with_tiffprefix" != "x" ],
		[ AC_MSG_WARN([*** TIFF prefix is user defined. Current value is $with_tiffprefix. ***])
		  TIFF_INCLUDE="$with_tiffprefix/include/"
		  TIFF_INCLUDE_PATH="-I$TIFF_INCLUDE"
		  TIFF_LIB_PATH="-L$with_tiffprefix/$OL_LIBNAME"],
		  AC_MSG_RESULT([*** TIFF prefix is not defined. Will use system defaults. ***]))
						  	     
	save_LDFLAGS="$LDFLAGS"
	LDFLAGS="$TIFF_LIB_PATH $save_LDFLAGS"

	AC_CHECK_LIB(tiff, TIFFOpen,
		[AC_CHECK_HEADER(${TIFF_INCLUDE}tiffio.h,
		[ac_have_tiff="yes" TIFF_LIBS="-ltiff" AC_DEFINE(HAVE_TIFF_H, 1,[Define this if you have TIFF support])],
		[AC_MSG_ERROR([*** TIFF support is not available. Please install libtiff to proceed. ***])])]

	LDFLAGS="$save_LDFLAGS", [AC_MSG_ERROR([*** TIFF support is not available. Please install libtiff to proceed. ***])],[])
				    
	if test x$ac_have_tiff = "xyes"; then
		ac_use_tiff=yes
	fi

	AC_SUBST(TIFF_PREFIX)
	AC_SUBST(TIFF_INCLUDE_PATH)
	AC_SUBST(TIFF_LIB_PATH)
	AC_SUBST(TIFF_LIBS)
	AM_CONDITIONAL(HAVE_LIBTIFF, test x"$ac_use_tiff" = "xyes")

	if test x$ac_use_tiff = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
