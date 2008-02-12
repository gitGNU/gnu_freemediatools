/*******************************************************************************
**
** The header file for the Jahshaka opengpulib module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glsl_objects.h"

#include <fstream>
using namespace std;

GlslProgram::GlslProgram(vector<GlslShader*>& shader_vector)
{
    m_gl_handle = glCreateProgramObjectARB();

    for (int i = 0; i < int(shader_vector.size()); i++)
    {
        glAttachObjectARB(m_gl_handle, *shader_vector[i]->getGlHandle() );
    }

 	glLinkProgramARB(m_gl_handle);
}

GlslShader::GlslShader(GLenum type, const char* shader_text)
{
    m_gl_handle = glCreateShaderObjectARB(type);

	// done to maintain the original structure of the code but using
	// basic_string<GLcharARB> is best. Please see the OpenObjectLib
	// for an example.    
    int shader_len = int( strlen(shader_text) ) + 1;
    
    m_shader_text = new char[ shader_len ];
    
    memset( m_shader_text, 0, shader_len );
    strncpy(m_shader_text, shader_text, strlen( shader_text) );

    GLsizei message_length;
    char message[1000];

    glShaderSourceARB(m_gl_handle, 1, (const char**)&m_shader_text, NULL);
    glGetInfoLogARB(m_gl_handle, 1000, &message_length, message);

    glCompileShaderARB(m_gl_handle);
    glGetInfoLogARB(m_gl_handle, 1000, &message_length, message);
}

GlslShader::GlslShader(char* shader_filename, GLenum type)
{
    m_gl_handle = glCreateShaderObjectARB(type);

    if ( readFileIntoBuffer(m_shader_text, shader_filename) )
    {
	    glShaderSourceARB(m_gl_handle, 1, (const char**)&m_shader_text, NULL);
  	    glCompileShaderARB(m_gl_handle);
    }
}

GlslShader::~GlslShader()
{
    delete[ ] m_shader_text;
}

bool         
GlslShader::readFileIntoBuffer(char*& buffer, char* file_name)
{
    ifstream input_file;
    input_file.open(file_name);

    if (!input_file)
    {
        return false;
    }

    input_file.seekg(0, ios::end);
    long file_size = input_file.tellg();
    input_file.seekg(0, ios::beg);

    buffer = new char[file_size];
    input_file.read(buffer, file_size);
    buffer[file_size] = '\0';

    return true;
}

bool                
GlslProgram::setTextureHandle(const char* name, int texture_unit)
{
    GLint handle = glGetUniformLocationARB(*getGlHandle(), name);

    if (handle == -1)
    {
        return false;
    }

    glUniform1iARB( handle, texture_unit );
    return true;
}

