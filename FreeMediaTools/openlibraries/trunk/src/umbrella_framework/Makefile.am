
#
#
#

libdir = $(OPENMEDIALIB_LIBPATH)

if HAVE_UMBRELLAFRAMEWORK

lib_LTLIBRARIES = libumbrella_framework.la

libumbrella_framework_la_SOURCES = 	\
	openlibraries.cpp		\
	openlibraries.hpp

libumbrella_framework_la_CXXFLAGS = \
	$(OLIB_CXXFLAGS)				\
	$(BOOST_INCLUDE_PATH)			\
	$(GLEW_INCLUDE_PATH)			\
	$(PYTHON_INCLUDE_PATH)

libumbrella_framework_la_LIBADD =	\
	$(top_builddir)/src/openpluginlib/pl/libopenpluginlib_pl.la	\
	$(top_builddir)/src/openimagelib/il/libopenimagelib_il.la	\
	$(top_builddir)/src/openmedialib/ml/libopenmedialib_ml.la	\
	$(top_builddir)/src/openassetlib/al/libopenassetlib_al.la	\
	$(BOOST_PYTHON_LIBS)										\
	$(GLEW_LIBS)

libumbrella_framework_la_LDFLAGS = 	\
	$(OLIB_LDFLAGS)			\
	$(BOOST_LIB_PATH)		\
	-version-info $(OL_LT_CURRENT):$(OL_LT_REVISION):$(OL_LT_AGE)

library_includedir = $(OPENLIBRARIES_INCLUDEPATH)/openmedialib/ml

library_include_HEADERS = 	\
	openlibraries.hpp
	
endif


