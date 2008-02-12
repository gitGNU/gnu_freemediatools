
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <openpluginlib/pl/string.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/cg.hpp>

namespace olib { namespace openpluginlib {

namespace
{
#if defined WIN32
	typedef HMODULE dll_t;
	
	const string cg_dllname		= "cg.dll";
	const string cgGL_dllname	= "cgGL.dll";
#elif defined __linux__
	typedef void* dll_t;
	
	const string cg_dllname		= "libCg.so";
	const string cgGL_dllname	= "libCgGL.so";
#else	// OS/X presumably
	typedef void* dll_t;
	
	const string cg_dllname		= "/System/Library/Frameworks/Cg.framework/Cg";
	const string cgGL_dllname	= "/System/Library/Frameworks/Cg.framework/Cg";
#endif

	dll_t dlopen_( const string& path )
	{
#ifdef WIN32
		return LoadLibrary( to_wstring( path ).c_str( ) );
#else
		return dlopen( path.c_str( ), RTLD_GLOBAL | RTLD_NOW );
#endif
	}
	
	void* dlsym_( dll_t shared, const char* entry_point )
	{
#ifdef WIN32
		return GetProcAddress( shared, entry_point );
#else
		return dlsym( shared, entry_point );
#endif
	}
	
	bool dlclose_( dll_t shared )
	{
#ifdef WIN32
		return FreeLibrary( shared ) == TRUE;
#else
		return dlclose( shared ) == 0;
#endif
	}
	
	dll_t cg_dll	= 0;
	dll_t cgGL_dll	= 0;
}

#ifdef HAVE_CG_RUNTIME

// cg
cgCreateContext_ptr					cgCreateContext					= 0;
cgDestroyContext_ptr				cgDestroyContext				= 0;
cgIsContext_ptr						cgIsContext						= 0;
cgCreateProgram_ptr					cgCreateProgram					= 0;
cgDestroyProgram_ptr				cgDestroyProgram				= 0;
cgGetNamedParameter_ptr				cgGetNamedParameter				= 0;
cgSetParameter1f_ptr				cgSetParameter1f				= 0;
cgSetParameter2fv_ptr				cgSetParameter2fv				= 0;
cgSetParameter4fv_ptr				cgSetParameter4fv				= 0;
cgSetMatrixParameterfc_ptr			cgSetMatrixParameterfc			= 0;
cgCreateEffectFromFile_ptr			cgCreateEffectFromFile			= 0;
cgGetFirstTechnique_ptr				cgGetFirstTechnique				= 0;
cgGetNextTechnique_ptr				cgGetNextTechnique				= 0;
cgGetNamedTechnique_ptr				cgGetNamedTechnique				= 0;
cgValidateTechnique_ptr				cgValidateTechnique				= 0;
cgGetFirstPass_ptr					cgGetFirstPass					= 0;
cgGetNamedPass_ptr					cgGetNamedPass					= 0;
cgGetNamedEffectParameter_ptr		cgGetNamedEffectParameter		= 0;
cgGetError_ptr						cgGetError						= 0;
cgGetFirstError_ptr					cgGetFirstError					= 0;
cgGetErrorString_ptr				cgGetErrorString				= 0;
cgGetLastErrorString_ptr			cgGetLastErrorString			= 0;
cgSetErrorCallback_ptr				cgSetErrorCallback				= 0;
cgGetErrorCallback_ptr				cgGetErrorCallback				= 0;
cgSetErrorHandler_ptr				cgSetErrorHandler				= 0;
cgGetErrorHandler_ptr				cgGetErrorHandler				= 0;

// cgGL
cgGLGetLatestProfile_ptr			cgGLGetLatestProfile			= 0;
cgGLSetOptimalOptions_ptr			cgGLSetOptimalOptions			= 0;
cgGLLoadProgram_ptr					cgGLLoadProgram					= 0;
cgGLSetManageTextureParameters_ptr	cgGLSetManageTextureParameters	= 0;
cgGLSetStateMatrixParameter_ptr		cgGLSetStateMatrixParameter		= 0;
cgGLSetTextureParameter_ptr			cgGLSetTextureParameter			= 0;
cgGLEnableTextureParameter_ptr		cgGLEnableTextureParameter		= 0;
cgGLEnableProfile_ptr				cgGLEnableProfile				= 0;
cgGLDisableProfile_ptr				cgGLDisableProfile				= 0;
cgGLBindProgram_ptr					cgGLBindProgram					= 0;
cgGLUnbindProgram_ptr				cgGLUnbindProgram				= 0;
cgGLRegisterStates_ptr				cgGLRegisterStates				= 0;

#endif

bool init_Cg( )
{
#ifdef HAVE_CG_RUNTIME

#	define DEFINE_CG_SYMBOL( dll, name )	\
		name = reinterpret_cast<name##_ptr>( dlsym_( dll, #name ) );

	cg_dll	 = dlopen_( cg_dllname );
	cgGL_dll = dlopen_( cgGL_dllname );

	if( !( cg_dll && cgGL_dll ) )
		return false;

	// cg
	DEFINE_CG_SYMBOL( cg_dll, cgCreateContext )
	DEFINE_CG_SYMBOL( cg_dll, cgDestroyContext )
	DEFINE_CG_SYMBOL( cg_dll, cgIsContext )
	DEFINE_CG_SYMBOL( cg_dll, cgCreateProgram )
	DEFINE_CG_SYMBOL( cg_dll, cgDestroyProgram )
	DEFINE_CG_SYMBOL( cg_dll, cgGetNamedParameter )
	DEFINE_CG_SYMBOL( cg_dll, cgSetParameter1f )
	DEFINE_CG_SYMBOL( cg_dll, cgSetParameter2fv )
	DEFINE_CG_SYMBOL( cg_dll, cgSetParameter4fv )
	DEFINE_CG_SYMBOL( cg_dll, cgSetMatrixParameterfc )
	DEFINE_CG_SYMBOL( cg_dll, cgCreateEffectFromFile )
	DEFINE_CG_SYMBOL( cg_dll, cgGetFirstTechnique )
	DEFINE_CG_SYMBOL( cg_dll, cgGetNextTechnique )
	DEFINE_CG_SYMBOL( cg_dll, cgGetNamedTechnique )
	DEFINE_CG_SYMBOL( cg_dll, cgValidateTechnique )
	DEFINE_CG_SYMBOL( cg_dll, cgGetFirstPass )
	DEFINE_CG_SYMBOL( cg_dll, cgGetNamedPass )
	DEFINE_CG_SYMBOL( cg_dll, cgGetNamedEffectParameter )
	DEFINE_CG_SYMBOL( cg_dll, cgGetError )
	DEFINE_CG_SYMBOL( cg_dll, cgGetFirstError )
	DEFINE_CG_SYMBOL( cg_dll, cgGetErrorString )
	DEFINE_CG_SYMBOL( cg_dll, cgGetLastErrorString )
	DEFINE_CG_SYMBOL( cg_dll, cgSetErrorCallback )
	DEFINE_CG_SYMBOL( cg_dll, cgGetErrorCallback )
	DEFINE_CG_SYMBOL( cg_dll, cgSetErrorHandler )
	DEFINE_CG_SYMBOL( cg_dll, cgGetErrorHandler )

	// cgGL
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLGetLatestProfile )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLSetOptimalOptions )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLLoadProgram )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLSetManageTextureParameters )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLSetStateMatrixParameter )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLSetTextureParameter )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLEnableTextureParameter )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLEnableProfile )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLDisableProfile )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLBindProgram )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLUnbindProgram )
	DEFINE_CG_SYMBOL( cgGL_dll, cgGLRegisterStates )

#	undef DEFINE_CG_SYMBOL

#else
	return false;
#endif

	return true;
}

void uninit_Cg( )
{
	if( cg_dll )
		dlclose_( cg_dll );
		
	if( cgGL_dll )
		dlclose_( cgGL_dll );
}

} }
