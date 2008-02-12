
#
# Check for OpenGL, GLU and GLUT
#

AC_DEFUN([AC_CHECK_OPENGL],[
	AC_ARG_ENABLE(opengl, AC_HELP_STRING([--disable-opengl], [disable OpenGL dependent parts]),
		[enableopengl=$enableval],
		[enableopengl=yes])
		
	AC_ARG_ENABLE(glu, AC_HELP_STRING([--disable-glu], [disable GLU dependent parts]),
		[enableglu=$enableval],
		[enableglu=yes])

	AC_ARG_ENABLE(glut, AC_HELP_STRING([--disable-glut], [disable GLUT dependent parts]),
		[enableglut=$enableval],
		[enableglut=yes])

	if test x$enableopengl = "xyes" && test x$enableglew = "xyes" ; then
		AC_CHECK_LIB(GL, glPushMatrix,
			[AC_CHECK_HEADER(GL/gl.h,
				[ac_have_opengl="yes" OPENGL_LIBS="-lGL" AC_DEFINE(HAVE_GL_GL_H,1,[Define this if you have OpenGL support])
					if test x$enableglu = "xyes" ; then
						save_LDFLAGS="$LDFLAGS"
					 	LDFLAGS="$LDFLAGS -L/usr/$OL_LIBNAME -L/usr/X11R6/$OL_LIBNAME -lGL"	
						AC_CHECK_LIB(GLU, gluPerspective,
							[AC_CHECK_HEADER(GL/glu.h,
								[ac_have_glu="yes"
								 OPENGL_GLU_LIBS="-lGLU"
								 AC_DEFINE(HAVE_GL_GLU_H,1,[Define this if you have GLU support])],
								[])
							],
							[AC_MSG_ERROR([*** OpenGL is not available. Please install OpenGL to proceed. ***])],
							[])
						LDFLAGS="$save_LDFLAGS"
					fi

					if test x$enableglut = "xyes" ; then
						AC_CHECK_LIB(glut, glutInit,
							[AC_CHECK_HEADER(GL/glut.h,
								[ac_have_glut="yes"
								 GLUT_LIBS="-lglut -lGL"
								 AC_DEFINE(HAVE_GL_GLUT_H, 1,[Define this if you have GLUT support])],
								[])
							],
							[AC_MSG_WARN([*** GLUT support is not available. Compilation of tests will fail. ***])],
							[])
					fi
				],
				[],
				[])
		])
		
		if test x$ac_have_opengl = "xyes"; then
			ac_use_opengl=yes
		fi
	else
		AC_MSG_RESULT([disabled])
         	ac_use_opengl=no
	fi
	
	AC_SUBST(OPENGL_LIBS)
	AC_SUBST(OPENGL_GLU_LIBS)
	AC_SUBST(GLUT_LIBS)
	AM_CONDITIONAL(HAVE_GL_GL_H, [test x$ac_use_opengl = "xyes"])
	AM_CONDITIONAL(HAVE_GL_GLUT_H, [test x$ac_have_glut = "xyes"])

	if test x$ac_use_opengl = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
