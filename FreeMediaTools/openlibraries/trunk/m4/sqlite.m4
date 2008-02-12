# $Id: sqlite.m4,v 1.2 2006-02-01 12:06:10 ezequiel Exp $

#
# Check for SQLite runtime support.
#

AC_DEFUN([AC_CHECK_SQLITE3],[
	AC_ARG_ENABLE(sqlite3, AC_HELP_STRING([--disable-sqlite3], [disable SQlite dependent plugin(s)]),
		[enablesqlite3=$enableval],
		[enablesqlite3=yes])

	SQLITE3_LIBS=""

	if test x$enablesqlite3 = "xyes" ; then
		# save_LIBS="$LIBS"
		# LIBS="$save_LIBS"

		AC_CHECK_LIB(sqlite3, sqlite3_exec,
			[AC_CHECK_HEADER(sqlite3.h,
				[ac_have_sqlite3="yes" SQLITE3_LIBS="-lsqlite3" AC_DEFINE(HAVE_SQLITE3,1,[Define this if you have SQLite support])],
				[AC_MSG_ERROR([*** SQLite runtime is available but no development headers. Please check your SQLite installation ***])])],
			[AC_MSG_WARN([*** SQLite runtime is not available. All SQLite dependent parts will be disabled ***])])

		if test x$ac_have_sqlite3 = "xyes"; then
			ac_use_sqlite3=yes
			SQLITE3_LIBS="-lsqlite3"
		fi
	fi

	AC_SUBST(SQLITE3_LIBS)
	AM_CONDITIONAL(HAVE_SQLITE3, [test x$ac_use_sqlite3 = "xyes"])

	if test x$ac_use_sqlite3 = "xyes"; then
		ifelse([$1], , :, [$1])
	else
		ifelse([$2], , :, [$2])
	fi
])

