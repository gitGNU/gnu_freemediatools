
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#include <algorithm>
#include <functional>

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif // WIN32

#include <boost/filesystem/path.hpp>

#include <openpluginlib/pl/openplugin.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>

#include <discovery_traits.hpp>
#include <algorithm>
#include <functional>
#include <vector>

#include <openpluginlib/pl/config.hpp>
#include <openpluginlib/pl/discovery_traits.hpp>
#include <openpluginlib/pl/openplugin.hpp>
#include <openpluginlib/pl/string.hpp>

namespace fs = boost::filesystem;


