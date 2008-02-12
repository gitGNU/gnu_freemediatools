
// HDRFlow Umbrella Framework main header file.

// Copyright (C) 2007 Goncalo N. M. de Carvalho
// Released under the LGPL.
// For more information, see http://www.cryogenicgraphics.com/hdrflow.

#ifndef OPENLIBRARIES_INC_
#define OPENLIBRARIES_INC_

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef HAVE_PYTHON

#ifndef __OBJC__
#include <boost/python.hpp>
#endif

#endif

#include <GL/glew.h>

#include <boost/algorithm/string.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/tokenizer.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/GL_utility.hpp>
#include <openimagelib/il/il.hpp>
#include <openimagelib/il/openimagelib_plugin.hpp>
#include <openmedialib/ml/ml.hpp>
#include <openmedialib/ml/openmedialib_plugin.hpp>

#endif
