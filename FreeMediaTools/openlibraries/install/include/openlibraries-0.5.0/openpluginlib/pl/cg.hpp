
// openpluginlib - A plugin interface to openlibraries.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef HAVE_CG_RUNTIME
#include <Cg/cg.h>
#include <Cg/cgGL.h>
#endif

#include <openpluginlib/pl/config.hpp>

namespace olib { namespace openpluginlib {

// init and fini
OPENPLUGINLIB_DECLSPEC bool init_Cg( );
OPENPLUGINLIB_DECLSPEC void uninit_Cg( );

#ifdef HAVE_CG_RUNTIME

// cg
typedef CGcontext			( *cgCreateContext_ptr )( void );
typedef void				( *cgDestroyContext_ptr )( CGcontext ctx );
typedef CGbool				( *cgIsContext_ptr )( CGcontext ctx );
typedef CGprogram			( *cgCreateProgram_ptr )( CGcontext ctx, CGenum program_type, const char* program, CGprofile profile, const char* entry, const char** args );
typedef void				( *cgDestroyProgram_ptr )( CGprogram prog );
typedef CGparameter			( *cgGetNamedParameter_ptr )( CGprogram prog, const char* name );
typedef void				( *cgSetParameter1f_ptr )( CGparameter param, float x );
typedef void				( *cgSetParameter2fv_ptr )( CGparameter param, const float* v );
typedef void				( *cgSetParameter4fv_ptr )( CGparameter param, const float* v );
typedef void				( *cgSetMatrixParameterfc_ptr )( CGparameter param, const float* v );
typedef CGeffect			( *cgCreateEffectFromFile_ptr )( CGcontext context, const char* filename, const char** args );
typedef CGtechnique			( *cgGetFirstTechnique_ptr )( CGeffect effect );
typedef CGtechnique			( *cgGetNextTechnique_ptr )( CGtechnique technique );
typedef CGtechnique			( *cgGetNamedTechnique_ptr )( CGeffect effect, const char* name );
typedef CGbool				( *cgValidateTechnique_ptr )( CGtechnique technique );
typedef CGpass				( *cgGetFirstPass_ptr )( CGtechnique technique );
typedef CGpass				( *cgGetNamedPass_ptr )( CGtechnique technique, const char* name );
typedef CGparameter			( *cgGetNamedEffectParameter_ptr )( CGeffect effect, const char* name );
typedef CGerror				( *cgGetError_ptr )( void );
typedef CGerror				( *cgGetFirstError_ptr )( void );
typedef const char*			( *cgGetErrorString_ptr )( CGerror error );
typedef const char*			( *cgGetLastErrorString_ptr )( CGerror error );
typedef void				( *cgSetErrorCallback_ptr )( CGerrorCallbackFunc func );
typedef CGerrorCallbackFunc	( *cgGetErrorCallback_ptr )( void );
typedef void				( *cgSetErrorHandler_ptr )( CGerrorHandlerFunc func, void* data );
typedef CGerrorHandlerFunc	( *cgGetErrorHandler_ptr )( void** data );

// cgGL
typedef CGprofile	( *cgGLGetLatestProfile_ptr )( CGGLenum profile_type );
typedef void		( *cgGLSetOptimalOptions_ptr )( CGprofile profile );
typedef void		( *cgGLLoadProgram_ptr )( CGprogram program );
typedef void		( *cgGLSetManageTextureParameters_ptr )( CGcontext ctx, CGbool flag );
typedef void		( *cgGLSetStateMatrixParameter_ptr )( CGparameter param, CGGLenum matrix, CGGLenum transform );
typedef void		( *cgGLSetTextureParameter_ptr )( CGparameter param, GLuint texobj );
typedef void		( *cgGLEnableTextureParameter_ptr )( CGparameter param );
typedef void		( *cgGLEnableProfile_ptr )( CGprofile profile );
typedef void		( *cgGLDisableProfile_ptr )( CGprofile profile );
typedef void		( *cgGLBindProgram_ptr )( CGprogram program );
typedef void		( *cgGLUnbindProgram_ptr )( CGprofile profile );
typedef void		( *cgGLRegisterStates_ptr )( CGcontext context );

// Goncalo - the declspec magic is needed until we consolidate oel into ool.
// cg
OPENPLUGINLIB_DECLSPEC extern cgCreateContext_ptr					cgCreateContext;
OPENPLUGINLIB_DECLSPEC extern cgDestroyContext_ptr					cgDestroyContext;
OPENPLUGINLIB_DECLSPEC extern cgIsContext_ptr						cgIsContext;
OPENPLUGINLIB_DECLSPEC extern cgCreateProgram_ptr					cgCreateProgram;
OPENPLUGINLIB_DECLSPEC extern cgDestroyProgram_ptr					cgDestroyProgram;
OPENPLUGINLIB_DECLSPEC extern cgGetNamedParameter_ptr				cgGetNamedParameter;
OPENPLUGINLIB_DECLSPEC extern cgSetParameter1f_ptr					cgSetParameter1f;
OPENPLUGINLIB_DECLSPEC extern cgSetParameter2fv_ptr					cgSetParameter2fv;
OPENPLUGINLIB_DECLSPEC extern cgSetParameter4fv_ptr					cgSetParameter4fv;
OPENPLUGINLIB_DECLSPEC extern cgSetMatrixParameterfc_ptr			cgSetMatrixParameterfc;
OPENPLUGINLIB_DECLSPEC extern cgCreateEffectFromFile_ptr			cgCreateEffectFromFile;
OPENPLUGINLIB_DECLSPEC extern cgGetFirstTechnique_ptr				cgGetFirstTechnique;
OPENPLUGINLIB_DECLSPEC extern cgGetNextTechnique_ptr				cgGetNextTechnique;
OPENPLUGINLIB_DECLSPEC extern cgGetNamedTechnique_ptr				cgGetNamedTechnique;
OPENPLUGINLIB_DECLSPEC extern cgValidateTechnique_ptr				cgValidateTechnique;
OPENPLUGINLIB_DECLSPEC extern cgGetFirstPass_ptr					cgGetFirstPass;
OPENPLUGINLIB_DECLSPEC extern cgGetNamedPass_ptr					cgGetNamedPass;
OPENPLUGINLIB_DECLSPEC extern cgGetNamedEffectParameter_ptr			cgGetNamedEffectParameter;
OPENPLUGINLIB_DECLSPEC extern cgGetError_ptr						cgGetError;
OPENPLUGINLIB_DECLSPEC extern cgGetFirstError_ptr					cgGetFirstError;
OPENPLUGINLIB_DECLSPEC extern cgGetErrorString_ptr					cgGetErrorString;
OPENPLUGINLIB_DECLSPEC extern cgGetLastErrorString_ptr				cgGetLastErrorString;
OPENPLUGINLIB_DECLSPEC extern cgSetErrorCallback_ptr				cgSetErrorCallback;
OPENPLUGINLIB_DECLSPEC extern cgGetErrorCallback_ptr				cgGetErrorCallback;
OPENPLUGINLIB_DECLSPEC extern cgSetErrorHandler_ptr					cgSetErrorHandler;
OPENPLUGINLIB_DECLSPEC extern cgGetErrorHandler_ptr					cgGetErrorHandler;

// cgGL
OPENPLUGINLIB_DECLSPEC extern cgGLGetLatestProfile_ptr				cgGLGetLatestProfile;
OPENPLUGINLIB_DECLSPEC extern cgGLSetOptimalOptions_ptr				cgGLSetOptimalOptions;
OPENPLUGINLIB_DECLSPEC extern cgGLLoadProgram_ptr					cgGLLoadProgram;
OPENPLUGINLIB_DECLSPEC extern cgGLSetManageTextureParameters_ptr	cgGLSetManageTextureParameters;
OPENPLUGINLIB_DECLSPEC extern cgGLSetStateMatrixParameter_ptr		cgGLSetStateMatrixParameter;
OPENPLUGINLIB_DECLSPEC extern cgGLSetTextureParameter_ptr			cgGLSetTextureParameter;
OPENPLUGINLIB_DECLSPEC extern cgGLEnableTextureParameter_ptr		cgGLEnableTextureParameter;
OPENPLUGINLIB_DECLSPEC extern cgGLEnableProfile_ptr					cgGLEnableProfile;
OPENPLUGINLIB_DECLSPEC extern cgGLDisableProfile_ptr				cgGLDisableProfile;
OPENPLUGINLIB_DECLSPEC extern cgGLBindProgram_ptr					cgGLBindProgram;
OPENPLUGINLIB_DECLSPEC extern cgGLUnbindProgram_ptr					cgGLUnbindProgram;
OPENPLUGINLIB_DECLSPEC extern cgGLRegisterStates_ptr				cgGLRegisterStates;

#endif

} }
