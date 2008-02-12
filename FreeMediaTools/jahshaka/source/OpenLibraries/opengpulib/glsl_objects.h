/*******************************************************************************
**
** The header file for the Jahshaka opengpulib module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLSL_OBJECTS_H
#define GLSL_OBJECTS_H

#include "config.h"

#include <GL/glew.h>

#include <vector>

class OPENGPULIB_API GlslShader
{
protected:
    GLhandleARB         m_gl_handle;
    char*               m_shader_text;

public:
    GlslShader(char* shader_filename, GLenum type);
    GlslShader(GLenum type, const char* shader_text);
    ~GlslShader();

    GLhandleARB*        getGlHandle()	{ return &m_gl_handle; }
    char**              getShaderText() { return &m_shader_text; }
    static bool         readFileIntoBuffer(char*& buffer, char* file_name);
};

class OPENGPULIB_API GlslProgram
{
protected:
    GLhandleARB         m_gl_handle;

public:
    GlslProgram(std::vector<GlslShader*>& shader_vector);
    GLhandleARB*        getGlHandle() { return &m_gl_handle; }
    bool                setTextureHandle( const char* name, int texture_unit);
};

#endif // GLSL_OBJECTS_H
