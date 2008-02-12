
#
# Enable/Disable Umbrella framework on OS/X
#

AC_DEFUN([AC_CHECK_UMBRELLA_FRAMEWORK],[
	AC_ARG_ENABLE(umbrellaframework, AC_HELP_STRING([--disable-umbrellaframework], [disable Umbrella Framework]),
		[enableumbrellaframework=$enableval],
		[enableumbrellaframework=yes])

	UMBRELLA_FLAGS=""

	AC_MSG_CHECKING(if building an umbrella framework)
	if test x$enableumbrellaframework = "xyes" ; then
		case $host in
			*-apple-darwin*)
				ac_use_umbrellaframework=yes
				AC_DEFINE(HAVE_UMBRELLAFRAMEWORK,1,[Define this if you want a umbrella framework])
			;;
		esac
	fi
	
	AC_SUBST(UMBRELLA_FLAGS)
	AM_CONDITIONAL(HAVE_UMBRELLAFRAMEWORK, [test x$ac_use_umbrellaframework = "xyes"])
	
	if test x$ac_use_umbrellaframework = "xyes"; then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
	fi
	
	if test x$ac_use_umbrellaframework = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])
