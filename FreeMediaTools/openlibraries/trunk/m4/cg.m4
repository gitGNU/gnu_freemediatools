
#
# Check for NVIDIA's Cg runtime support.
#

AC_DEFUN([AC_CHECK_CG_RUNTIME],[
	AC_ARG_ENABLE(cg, AC_HELP_STRING([--disable-cg], [disable NVIDIA Cg dependent parts]),
		[enablecg=$enableval],
		[enablecg=yes])

	CG_LIBS=""

	if test x$enablecg = "xyes" && test x$enableglew = "xyes" ; then
		save_LIBS="$LIBS"
		save_CXXFLAGS="$CXXFLAGS"
		save_CPPFLAGS="$CPPFLAGS"
	
		case $host in
			*-*-linux*)
				LIBS="$LIBS -lpthread ${OPENGL_LIBS} -lsupc++"

				AC_CHECK_LIB(Cg, cgCreateProgramFromFile,
					[AC_CHECK_HEADER(Cg/cg.h,
						[ac_have_cg="yes" CG_LIBS="-lCg" AC_DEFINE(HAVE_CG_RUNTIME,1,[Define this if you have NVIDIA Cg runtime support])
							AC_CHECK_LIB(CgGL, cgGLGetLatestProfile,
								[AC_CHECK_HEADER(Cg/cgGL.h,
									[ac_have_cg="yes" CG_LIBS="$CG_LIBS -lCgGL" ],
									[AC_MSG_ERROR([*** NVIDIA Cg runtime is available but no CgGL headers. Please check your Cg installation ***])])	
								],
								[AC_MSG_ERROR([*** NVIDIA Cg runtime is available but no CgGL. Please check your Cg installation ***])])],
						[AC_MSG_ERROR([*** NVIDIA Cg runtime is available but no development headers. Please check your Cg installation ***])])],
					[AC_MSG_WARN([*** NVIDIA Cg runtime is not available. All Cg dependent parts will be disabled ***])])
				
			;;
			*-apple-darwin*)
				AC_MSG_CHECKING(the availability of NVIDIA's Cg)
				LIBS="-Xlinker -framework -Xlinker Cg -Xlinker -framework -Xlinker OpenGL"
				AC_TRY_LINK([#include <Cg/Cg.h>],[cgCreateContext(); return 0;], ac_have_cg="yes" AC_DEFINE(HAVE_CG_RUNTIME,1,[Define this if you have NVIDIA Cg runtime support]))
				if test x$ac_have_cg = "xyes"; then
					AC_MSG_RESULT(yes)
				else
					AC_MSG_RESULT(no)
				fi
			;;
		esac

		if test x$ac_have_cg = "xyes"; then
			ac_use_cg=yes
			CG_LIBS="$LIBS"
		fi
	
		CPPFLAGS="$save_CPPFLAGS"
		CXXFLAGS="$save_CXXFLAGS"
		LIBS="$save_LIBS"
	fi

	AC_SUBST(CG_LIBS)
	AM_CONDITIONAL(HAVE_CG_RUNTIME, [test x$ac_use_cg = "xyes"])

	if test x$ac_use_cg = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
