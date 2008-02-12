dnl
dnl	Configure.ac for openlibraries
dnl

AC_INIT
AC_CONFIG_SRCDIR([src/openobjectlib/sg/core.hpp])
AM_CONFIG_HEADER([openlibraries_global_config.hpp])

dnl
dnl	Require autoconf version 2.56
dnl
AC_PREREQ(2.56)

dnl
dnl Versioning
dnl
OL_MAJOR=0
OL_MINOR=5
OL_SUB=0

OL_LT_CURRENT=0
OL_LT_REVISION=5
OL_LT_AGE=0

AC_SUBST(OL_MAJOR)
AC_SUBST(OL_MINOR)
AC_SUBST(OL_SUB)

AC_SUBST(OL_LT_CURRENT)
AC_SUBST(OL_LT_REVISION)
AC_SUBST(OL_LT_AGE)

dnl
AM_INIT_AUTOMAKE("openlibraries", 0.5.0)

dnl Check for programs
AC_PROG_CXX
AC_HEADER_STDC
AC_PROG_MAKE_SET
AC_PROG_INSTALL
AC_PROG_LN_S

dnl Check if specific olibs should be disabled
AC_ARG_ENABLE(glew, AC_HELP_STRING([--disable-glew], [disable GLEW dependent parts]), [enableglew=$enableval], [enableglew=yes])
if test "$enableglew" = "no" ; then
	AC_MSG_WARN([*** GLEW is disabled - continuing build with non-opengl dependent components ***])
fi

AC_ARG_ENABLE(openassetlib, AC_HELP_STRING([--with-openassetlib], [disable openassetlib]), [enableassetlib=$enableval], [enableassetlib=yes])
if test "x$enableassetlib" = "xyes" && test "x$enableglew" = "xyes" ; then
	AC_DEFINE(HAVE_OPENASSETLIB,1,[Defined if you have openassetlib])
fi
AM_CONDITIONAL(HAVE_OPENASSETLIB, [test "x$enableassetlib" = "xyes" && test "x$enableglew" = "xyes"])

AC_ARG_ENABLE(openeffectlib, AC_HELP_STRING([--with-openeffectlib], [disable openeffectlib]), [enableeffectlib=$enableval], [enableeffectlib=yes])
if test "x$enableeffectlib" = "xyes" && test "x$enableglew" = "xyes" ; then
	AC_DEFINE(HAVE_OPENEFFECTLIB,1,[Defined if you have openeffectlib])
fi
AM_CONDITIONAL(HAVE_OPENEFFECTLIB, [test "x$enableeffectlib" = "xyes" && test "x$enableglew" = "xyes"])

AC_ARG_ENABLE(openobjectlib, AC_HELP_STRING([--with-openobjectlib], [disable openobjectlib]), [enableobjectlib=$enableval], [enableobjectlib=yes])
if test "x$enableobjectlib" = "xyes" && test "x$enableglew" = "xyes" ; then
	AC_DEFINE(HAVE_OPENOBJECTLIB, 1,[Defined if you have openobjectlib])
fi
AM_CONDITIONAL(HAVE_OPENOBJECTLIB, [test "x$enableobjectlib" = "xyes" && test "x$enableglew" = "xyes"])

AC_ARG_ENABLE(pool, AC_HELP_STRING([--disable-pool], [disable memory pooling]), [enablepool=$enableval], [enablepool=yes])
if test "$enablepool" = "yes" ; then
	AC_DEFINE(HAVE_POOLING, 1,[Define this if you want memory pooling])
fi

dnl libtool
AC_LIBTOOL_DLOPEN
AC_DISABLE_STATIC
AC_PROG_LIBTOOL
AC_SUBST(LIBTOOL_DEPS)

dnl dl support
AC_CHECK_LIB(dl, dlopen,
			 have_dl=yes DL_LIBS="-ldl",
			 AC_MSG_ERROR([*** dynamic loading support is not available. Compilation will fail. ***]))
AC_SUBST(DL_LIBS)

dnl determine libdir name. tests for 64bit. (from xine-lib).
AC_MSG_CHECKING([if compiler is 64bit])
case $host in
	*-*-linux*)
		# Test if the compiler is 64bit
		echo 'int i;' > conftest.$ac_ext
		ol_cv_cc_64bit_output=no
		if AC_TRY_EVAL(ac_compile); then
			case `/usr/bin/file conftest.$ac_objext` in
				*"ELF 64"*)
				ol_cv_cc_64bit_output=yes
				;;
			esac
 		fi
 	rm -rf conftest*
 	;;
esac

case $host_cpu:$ol_cv_cc_64bit_output in
	powerpc64:yes | x86_64:yes)
		OL_LIBNAME="lib64"
		if test "$libdir" = '${exec_prefix}/lib'; then
			libdir='${exec_prefix}/lib64'
		fi
 		;;
	*:*)
 		OL_LIBNAME="lib"
 		;;
esac
AC_MSG_RESULT([$OL_LIBNAME])
AC_SUBST(OL_LIBNAME)

dnl C math library.
AC_CHECK_LIB(m, sin)

dnl zlib support
AC_CHECK_ZLIB( )

dnl bzip2 support
AC_CHECK_BZIP2( )

dnl SQLite support
AC_CHECK_SQLITE3( )

dnl check for X11
AC_PATH_XTRA
if test x"$no_x" != "xyes"; then
    AC_DEFINE(HAVE_X11,1,[Define this if you have X11R6 installed])
fi
AM_CONDITIONAL(HAVE_X11, [test x"$no_x" != "xyes"])

dnl OpenGL, GLU, GLUT support
OPL_EXTRA_LIBS=""
AC_MSG_CHECKING([the OpenGL environment])
case $host in
	*-*-linux*)
		AC_CHECK_OPENGL( )
	;;
	*-apple-darwin*)
		if test "x$enableglew" = "xyes" ; then
			AC_MSG_RESULT([Apple Darwin build. OpenGL defaults on OSX...])
			AM_CONDITIONAL(HAVE_GL_GL_H, [test x"yes" = x"yes"] )
			AM_CONDITIONAL(HAVE_GL_GLUT_H, [test x"yes" = x"yes"] )
			OPENGL_LIBS="-Xlinker -framework -Xlinker OpenGL"
			GLUT_LIBS="-Xlinker -framework -Xlinker GLUT"
		else
			OPL_EXTRA_LIBS="-Xlinker -framework -Xlinker Carbon"
			AM_CONDITIONAL(HAVE_GL_GL_H, [false])
			AM_CONDITIONAL(HAVE_GL_GLUT_H, [false])
		fi
		AC_SUBST(OPENGL_LIBS)
		AC_SUBST(GLUT_LIBS)
	;;
	*)
		AC_MSG_RESULT([OpenGL configuration not tested.])
		AM_CONDITIONAL(HAVE_GL_GL_H, [false])
		AM_CONDITIONAL(HAVE_GL_GLUT_H, [false])
	;;
esac
AC_SUBST(OPL_EXTRA_LIBS)

dnl GLEW support
AC_CHECK_GLEW( )

dnl NVIDIA Cg runtime support
AC_CHECK_CG_RUNTIME( )

dnl OFX support
AC_CHECK_OFX( )

dnl Umbrella Framework
AC_CHECK_UMBRELLA_FRAMEWORK()

dnl Boost support
AC_LANG([C++])
AC_CHECK_BOOST( )

dnl libxml2 support
AC_PATH_PROG(LIBXML2_CONFIG, xml2-config, no)
if test "x$LIBXML2_CONFIG" = "xno" ; then
	AC_MSG_RESULT([*** libxml2 support is not available ***])
else
	XML2_CXXFLAGS=`$LIBXML2_CONFIG --cflags`
	XML2_LIBS=`$LIBXML2_CONFIG --libs`
	have_libxml2="yes"
	AC_DEFINE(HAVE_LIBXML2,1,[Define this for libxml2 support])
fi

AM_CONDITIONAL(HAVE_LIBXML2, test x"$have_libxml2" = "xyes")
AC_SUBST(XML2_CXXFLAGS)
AC_SUBST(XML2_LIBS)

dnl OpenImageLib support
AC_MSG_NOTICE([*** Configuring OpenImageLib ***])
AC_CHECK_OPENIMAGELIB( )
AC_CHECK_TIFF( )

AC_CHECK_QUICKTIME( )

SWAB_AUDIO=""
AC_ARG_ENABLE(swab-audio, AC_HELP_STRING([--enable-swab-audio], [enable byte swapping of audio samples]),
		[enableswabaudio=$enableval], [enableswabaudio=no])
if test x$enableswabaudio = "xyes" ; then
	SWAB_AUDIO="-DSWAB_AUDIO"
fi
AC_SUBST(SWAB_AUDIO)

dnl include paths
INCLUDES='-I$(top_srcdir)/src'
AC_SUBST(INCLUDES)

dnl OpenLibraries common flags
dnl NOTE: we could use $(var) instead of @var@ if we want late-expansion in the generated makefiles.
OLIB_CXXFLAGS='-fvisibility=hidden -fvisibility-inlines-hidden'
AC_SUBST(OLIB_CXXFLAGS)

OLIB_LDFLAGS=''

case $host in
	*-*-linux*)
		OLIB_LDFLAGS='-Wl,-export-dynamic'
	;;
	*-apple-darwin*)
		OLIB_LDFLAGS='-Wl,-headerpad_max_install_names'
	;;
esac

AC_SUBST(OLIB_LDFLAGS)

dnl Check for ffmpeg's libavformat
AC_CHECK_PROG(HAVE_PKG_CONFIG, pkg-config, yes)
if test "x$HAVE_PKG_CONFIG" = "xyes" ; then
  PKG_CHECK_MODULES(LIBAVFORMAT, libavformat, libavformat="yes", libavformat="no")
  if test x"$libavformat" = x"no"; then
	AC_MSG_RESULT([*** ffmpeg development environment is not available ***])
  else
	AC_DEFINE(HAVE_LIBAVFORMAT,1,[Define this if you have libavformat])
	AC_SUBST(LIBAVFORMAT_CFLAGS)
	AC_SUBST(LIBAVFORMAT_LIBS)
  fi
fi
AM_CONDITIONAL(HAVE_LIBAVFORMAT, [test x"$libavformat" = "xyes"])

dnl mlt support
AC_ARG_ENABLE(mlt, AC_HELP_STRING([--disable-mlt], [disable mlt dependent parts]), [enablemlt=$enableval], [enablemlt=yes])
if test "$enablemlt" = "yes" ; then
	AC_PATH_PROG(MLT_CONFIG, mlt-config, no)
	if test "x$MLT_CONFIG" = "xno" ; then
		AC_MSG_RESULT([*** mlt support is not available ***])
	else
		AC_DEFINE(HAVE_LIBMLT,1,[Define this for mlt support])
		MLT_CXXFLAGS=`$MLT_CONFIG --cflags`
		MLT_LIBS="-lmlt++ `$MLT_CONFIG --libs`"
		AC_SUBST(MLT_CXXFLAGS)
		AC_SUBST(MLT_LIBS)
		have_libmlt="yes"
	fi
fi
AM_CONDITIONAL(HAVE_LIBMLT, [test x"$have_libmlt" = "xyes"])

dnl sdl support
AC_PATH_PROG(SDL_CONFIG, sdl-config, no)
if test "x$SDL_CONFIG" = "xno" ; then
	AC_MSG_RESULT([*** sdl support is not available ***])
else
	AC_DEFINE(HAVE_LIBSDL,1,[Define this for sdl support])
	SDL_CXXFLAGS=`$SDL_CONFIG --cflags`
	SDL_LIBS=`$SDL_CONFIG --libs`
	AC_SUBST(SDL_CXXFLAGS)
	AC_SUBST(SDL_LIBS)
	have_libsdl="yes"
fi
AM_CONDITIONAL(HAVE_LIBSDL, [test x"$have_libsdl" = "xyes"])

dnl qimage support
AC_CHECK_QT( )

dnl Check for ILM OpenEXR
if test "x$HAVE_PKG_CONFIG" = "xyes" ; then
  PKG_CHECK_MODULES(OPENEXR, OpenEXR, OpenEXR="yes", OpenEXR="no")
  if test x"$OpenEXR" = x"no"; then
	AC_MSG_RESULT([*** ILM OpenEXR development environment is not available ***])
  else
	AC_DEFINE(HAVE_OPENEXR,1,[Define this if you have ILM OpenEXR])
	AC_SUBST(OPENEXR_CFLAGS)
	AC_SUBST(OPENEXR_LIBS)
  fi
fi
AM_CONDITIONAL(HAVE_OPENEXR, [test x"$OpenEXR" = "xyes"])

dnl Check for openal
AC_CHECK_OPENAL_RUNTIME( )

dnl Check for gpl components
AC_CHECK_GPL( )

dnl Check for Python
AC_CHECK_PYTHON( )

dnl Universal Binary Support (OS X only)
AC_CHECK_UNIVERSAL_BINARY_SUPPORT( )

dnl Fast Math support
AC_CHECK_FAST_MATH( )

SHARED_EXT="so"
case $host in
	*-apple-darwin*)
		SHARED_EXT="dylib"
	;;
esac
AC_SUBST(SHARED_EXT)

dnl include, lib and plugin path
OPENLIBRARIES_INCLUDEPATH="$includedir/openlibraries-$OL_MAJOR.$OL_MINOR.$OL_SUB"
OPENLIBRARIES_LIBPATH="$libdir/openlibraries-$OL_MAJOR.$OL_MINOR.$OL_SUB"
OPENLIBRARIES_DATAPATH="$datadir/openlibraries-$OL_MAJOR.$OL_MINOR.$OL_SUB"
OPENIMAGELIB_LIBPATH="${OPENLIBRARIES_LIBPATH}/openimagelib/$OL_LIBNAME"
OPENIMAGELIB_PLUGINPATH="${OPENLIBRARIES_LIBPATH}/openimagelib/plugins"
OPENMEDIALIB_LIBPATH="${OPENLIBRARIES_LIBPATH}/openmedialib/$OL_LIBNAME"
OPENMEDIALIB_PLUGINPATH="${OPENLIBRARIES_LIBPATH}/openmedialib/plugins"
OPENEFFECTSLIB_PLUGINPATH="${OPENLIBRARIES_LIBPATH}/openeffectslib/plugins"
OPENOBJECTLIB_LIBPATH="${OPENLIBRARIES_LIBPATH}/openobjectlib/$OL_LIBNAME"
OPENOBJECTLIB_PLUGINPATH="${OPENLIBRARIES_LIBPATH}/openobjectlib/plugins"
OPENOBJECTLIB_MEDIAPATH="$datadir/openlibraries-$OL_MAJOR.$OL_MINOR/media"
OPENOBJECTLIB_SHADERSPATH="${OPENLIBRARIES_DATAPATH}/shaders"
OPENASSETLIB_LIBPATH="${OPENLIBRARIES_LIBPATH}/openassetlib/$OL_LIBNAME"
OPENASSETLIB_PLUGINPATH="${OPENLIBRARIES_LIBPATH}/openassetlib/plugins"
OPENPLUGINLIB_LIBPATH="${OPENLIBRARIES_LIBPATH}/openpluginlib/$OL_LIBNAME"
OPENEFFECTSLIB_SHADERSPATH="${OPENLIBRARIES_DATAPATH}/shaders"
AC_SUBST(OPENIMAGELIB_LIBPATH)
AC_SUBST(OPENIMAGELIB_PLUGINPATH)
AC_SUBST(OPENMEDIALIB_LIBPATH)
AC_SUBST(OPENMEDIALIB_PLUGINPATH)
AC_SUBST(OPENEFFECTSLIB_PLUGINPATH)
AC_SUBST(OPENLIBRARIES_INCLUDEPATH)
AC_SUBST(OPENLIBRARIES_LIBPATH)
AC_SUBST(OPENLIBRARIES_DATAPATH)
AC_SUBST(OPENOBJECTLIB_LIBPATH)
AC_SUBST(OPENOBJECTLIB_MEDIAPATH)
AC_SUBST(OPENOBJECTLIB_PLUGINPATH)
AC_SUBST(OPENOBJECTLIB_SHADERSPATH)
AC_SUBST(OPENASSETLIB_LIBPATH)
AC_SUBST(OPENASSETLIB_PLUGINPATH)
AC_SUBST(OPENPLUGINLIB_LIBPATH)

dnl pkg config link flags
OPENOBJECTLIB_LDFLAGS=""
if test "x$enableobjectlib" = "xyes" && test "x$enableglew" = "xyes" ; then
	OPENOBJECTLIB_LDFLAGS="-L$OPENLIBRARIES_LIBPATH/openobjectlib/$OL_LIBNAME -lopenobjectlib_sg"
fi
AC_SUBST(OPENOBJECTLIB_LDFLAGS)

OPENASSETLIB_LDFLAGS=""
if test "x$enableassetlib" = "xyes" && test "x$enableglew" = "xyes" ; then
	OPENASSETLIB_LDFLAGS="-L$OPENLIBRARIES_LIBPATH/openassetlib/$OL_LIBNAME -lopenassetlib_al"
fi
AC_SUBST(OPENASSETLIB_LDFLAGS)

OPENMEDIALIB_LDFLAGS="-L$OPENLIBRARIES_LIBPATH/openmedialib/$OL_LIBNAME -lopenmedialib_ml"
AC_SUBST(OPENMEDIALIB_LDFLAGS)

OPENIMAGELIB_LDFLAGS="-L$OPENLIBRARIES_LIBPATH/openimagelib/$OL_LIBNAME -lopenimagelib_il"
AC_SUBST(OPENIMAGELIB_LDFLAGS)

OPENPLUGINLIB_LDFLAGS="-L$OPENLIBRARIES_LIBPATH/openpluginlib/$OL_LIBNAME -lopenpluginlib_pl"
AC_SUBST(OPENPLUGINLIB_LDFLAGS)

dnl
dnl Output configuration files
dnl
AC_CONFIG_FILES([
Makefile
src/Makefile
src/openpluginlib/Makefile
src/openpluginlib/pl/Makefile
src/openpluginlib/py/Makefile
src/openimagelib/Makefile
src/openimagelib/il/Makefile
src/openimagelib/plugins/Makefile
src/openimagelib/plugins/3D_lightmap/Makefile
src/openimagelib/plugins/dds/Makefile
src/openimagelib/plugins/dpx/Makefile
src/openimagelib/plugins/exr/Makefile
src/openimagelib/plugins/hdr/Makefile
src/openimagelib/plugins/jpg/Makefile
src/openimagelib/plugins/png/Makefile
src/openimagelib/plugins/quicktime/Makefile
src/openimagelib/plugins/sgi/Makefile
src/openimagelib/plugins/tga/Makefile
src/openimagelib/plugins/tiff/Makefile
src/openimagelib/py/Makefile
src/openmedialib/Makefile
src/openmedialib/ml/Makefile
src/openmedialib/plugins/Makefile
src/openmedialib/plugins/avformat/Makefile
src/openmedialib/plugins/oil/Makefile
src/openmedialib/plugins/openal/Makefile
src/openmedialib/plugins/sdl/Makefile
src/openmedialib/plugins/gensys/Makefile
src/openmedialib/plugins/quicktime/Makefile
src/openmedialib/plugins/ofx/Makefile
src/openmedialib/py/Makefile
src/openeffectslib/Makefile
src/openeffectslib/plugins/Makefile
src/openeffectslib/plugins/tonemap/Makefile
src/openobjectlib/Makefile
src/openobjectlib/sg/Makefile
src/openobjectlib/plugins/Makefile
src/openobjectlib/plugins/X3D/Makefile
src/openobjectlib/plugins/wavefront/Makefile
src/openassetlib/Makefile
src/openassetlib/al/Makefile
src/openassetlib/py/Makefile
src/openassetlib/plugins/Makefile
src/openassetlib/plugins/sqlite/Makefile
src/umbrella_framework/Makefile
media/Makefile
effects/Makefile
m4/Makefile
test/Makefile
test/openeffectslib/Makefile
test/openeffectslib/tm_bench/Makefile
test/openpluginlib/Makefile
test/openpluginlib/discovery/Makefile
test/openpluginlib/enumerate/Makefile
test/openpluginlib/gettimeofday/Makefile
test/openpluginlib/gpu_timer_query/Makefile
test/openpluginlib/initialization/Makefile
test/openpluginlib/rdtsc/Makefile
test/openpluginlib/pcos/Makefile
test/openpluginlib/pcos/key/Makefile
test/openpluginlib/pcos/property/Makefile
test/openpluginlib/pcos/subject-observer/Makefile
test/openpluginlib/pcos/property_container/Makefile
test/openimagelib/Makefile
test/openimagelib/GL/Makefile
test/openimagelib/GL/_2D/Makefile
test/openimagelib/GL/_2D_compressed/Makefile
test/openimagelib/GL/_2D_compressed_cubemap/Makefile
test/openimagelib/GL/_2D_crop/Makefile
test/openimagelib/GL/_2D_crop2/Makefile
test/openimagelib/GL/_2D_exr/Makefile
test/openimagelib/GL/_2D_scale/Makefile
test/openimagelib/GL/_2D_sgi/Makefile
test/openmedialib/Makefile
test/openmedialib/player/Makefile
test/openmedialib/store/Makefile
test/openobjectlib/Makefile
test/openobjectlib/GL/Makefile
test/openobjectlib/GL/multipass_draw/Makefile
test/openobjectlib/GL/noise_volume/Makefile
test/openobjectlib/GL/obj_draw/Makefile
test/openobjectlib/GL/texture_draw/Makefile
test/openassetlib/Makefile
openlibraries.pc
])
AC_OUTPUT
