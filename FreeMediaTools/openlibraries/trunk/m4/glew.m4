
#
# Check for GLEW
#

AC_DEFUN([AC_CHECK_GLEW],[
	AC_ARG_WITH(glewprefix,	AC_HELP_STRING([--with-glewprefix], [GLEW installation prefix directory]),,
				with_glewprefix="")

	GLEW_PREFIX="$with_glewprefix"
	GLEW_INCLUDE_PATH=""
	GLEW_LIB_PATH=""
	GLEW_LIBS=""

	if test x$enableglew = "xyes" ; then
		GLEW_INCLUDE_PATH="-DHAVE_GL_GLEW_H"

		AS_IF([test "x$with_glewprefix" != "x" ],
			[ AC_MSG_WARN([*** GLEW prefix is user defined. Current value is $with_glewprefix. ***])
			  GLEW_INCLUDE="$with_glewprefix/include/"
			  GLEW_INCLUDE_PATH="-DHAVE_GL_GLEW_H -I$GLEW_INCLUDE"
			  GLEW_LIB_PATH="-L$with_glewprefix/$OL_LIBNAME"],
			  AC_MSG_RESULT([*** GLEW prefix is not defined. Will use system defaults. ***]))
						  	     
		save_LDFLAGS="$LDFLAGS"
		LDFLAGS="$GLEW_LIB_PATH $save_LDFLAGS"

		AC_CHECK_LIB(GLEW, glewInit,
			[AC_CHECK_HEADER(${GLEW_INCLUDE}GL/glew.h,
			[ac_have_glew="yes" GLEW_LIBS="-lGLEW" AC_DEFINE(HAVE_GL_GLEW_H, 1,[Define this if you have GLEW support])],
			[AC_MSG_ERROR([*** GLEW support is not available. Please install GLEW to proceed. ***])])]

		LDFLAGS="$save_LDFLAGS", [AC_MSG_ERROR([*** GLEW support is not available. Please install GLEW to proceed. ***])],[])				    
	fi
	
	AC_SUBST(GLEW_PREFIX)
	AC_SUBST(GLEW_INCLUDE_PATH)
	AC_SUBST(GLEW_LIB_PATH)
	AC_SUBST(GLEW_LIBS)

	if test x$ac_use_glew = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
