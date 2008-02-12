
#
#
#

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = dist-bzip2 dist-zip

am__tar=$(AMTAR) -ch --posix -f - "$$tardir"
am__untar=$(AMTAR) -x --posix -f -

SUBDIRS = 	\
	src	\
	m4	\
	media	\
	effects	\
	test

openlibraries_configdir = $(libdir)/openlibraries-$(OL_MAJOR).$(OL_MINOR).$(OL_SUB)/include
openlibraries_config_DATA = openlibraries_global_config.hpp

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = openlibraries.pc
