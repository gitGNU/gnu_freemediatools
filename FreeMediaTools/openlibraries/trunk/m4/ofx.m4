
#
# Check for OFX
#

AC_DEFUN([AC_CHECK_OFX],[
	AC_ARG_WITH(ofxinclude, AC_HELP_STRING([--with-ofxinclude], [OFX include directory]),,with_ofxinclude="/usr/local/include/OFX")

	OFX_INCLUDE_PATH="-I${with_ofxinclude}"
	OFX_LIBS=""

	save_CPPFLAGS="$CPPFLAGS"
	CPPFLAGS="$CPPFLAGS ${OFX_INCLUDE_PATH}"

	save_CXXFLAGS="$CXXFLAGS"
	CXXFLAGS="$CXXFLAGS ${OFX_INCLUDE_PATH}"

	AC_LANG(C++)
	AC_CHECK_HEADER(OfxCore.h,
		[ac_have_ofx="yes" AC_DEFINE(HAVE_OFX,1,[Define this if you have OFX support])],
		[AC_MSG_NOTICE([*** OFX development headers not available. Please check your OFX installation ***])])

	if test x$ac_have_ofx = "xyes"; then
		case $host in
			*-apple-darwin*)
				OFX_LIBS="-Xlinker -framework -Xlinker CoreFoundation"
			;;
		esac
		ac_use_ofx=yes
	fi

	CPPFLAGS="$save_CPPFLAGS"
	CXXFLAGS="$save_CXXFLAGS"

	AC_SUBST(OFX_INCLUDE_PATH)
	AC_SUBST(OFX_LIBS)
	AM_CONDITIONAL(HAVE_OFX, [test x$ac_use_ofx = "xyes"])

	if test x$ac_use_ofx = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
