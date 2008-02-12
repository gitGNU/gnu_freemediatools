/*******************************************************************************
**
** The source file for the Jahshaka core model file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "coreobject.h"


//extern unsigned char* loadshaderfile(const char *name);
//extern void find_shader_program_error(unsigned char * source, char* program_name);
//extern void create_pbuffer(int width, int height);

//#warning  FIXME    The mesh definition needs to be cleaned up
//#define MESH_X_DIMENSION                            72               
//#define MESH_Y_DIMENSION                            48
//
//#define IMAGE_SCALE                                 1.25


//////////////////////////////////////////////////////////
// Mesh Control utilities

void JahLayer::setMeshCoord( int i, int j, GLfloat x, GLfloat y) {

    MeshXCoord[i][j] = x;
    MeshYCoord[i][j] = y;

}

void JahLayer::setMeshCoord( int i, int j, GLfloat x, GLfloat y, GLfloat z) {

    MeshXCoord[i][j] = x;
    MeshYCoord[i][j] = y;
    MeshZCoord[i][j] = z;
}

void JahLayer::getMeshCoord( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z)
{
    x = MeshXCoord[i][j];
    y = MeshYCoord[i][j];
    z = MeshZCoord[i][j];
}

void 
JahLayer::getMeshCoord( int i, int j, float4& vector)
{
    vector.x = MeshXCoord[i][j];
    vector.y = MeshYCoord[i][j];
    vector.z = MeshZCoord[i][j];
    vector.w = 1.0f;
}

void 
JahLayer::saveMeshCoords()
{
    for (int i = 0; i <= JAHMESH_X_DIMENSION; i++)
    {
        for (int j = 0; j <= JAHMESH_Y_DIMENSION; j++)
        {
            m_mesh_save[i][j].x = MeshXCoord[i][j];
            m_mesh_save[i][j].y = MeshYCoord[i][j];
            m_mesh_save[i][j].z = MeshZCoord[i][j];
        }
    }
}

void 
JahLayer::restoreMeshCoords()
{
    for (int i = 0; i <= JAHMESH_X_DIMENSION; i++)
    {
        for (int j = 0; j <= JAHMESH_Y_DIMENSION; j++)
        {
            MeshXCoord[i][j] = m_mesh_save[i][j].x;
            MeshYCoord[i][j] = m_mesh_save[i][j].y;
            MeshZCoord[i][j] = m_mesh_save[i][j].z;
        }
    }
}
void JahLayer::setMeshNormal( int i, int j, GLfloat x, GLfloat y, GLfloat z) 
{
    MeshXNormal[i][j] = x;
    MeshYNormal[i][j] = y;
    MeshZNormal[i][j] = z;
}

void JahLayer::setMeshNormal( int i, int j, float4& vector) 
{
    MeshXNormal[i][j] = vector.x;
    MeshYNormal[i][j] = vector.y;
    MeshZNormal[i][j] = vector.z;
}

void JahLayer::getMeshNormal( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z)
{
    x = MeshXNormal[i][j];
    y = MeshYNormal[i][j];
    z = MeshZNormal[i][j];
}

void JahLayer::getMeshNormal( int i, int j, float4& vector)
{
    vector.x = MeshXNormal[i][j];
    vector.y = MeshYNormal[i][j];
    vector.z = MeshZNormal[i][j];
}

void 
JahLayer::saveMeshNormals()
{
    for (int i = 0; i <= JAHMESH_X_DIMENSION; i++)
    {
        for (int j = 0; j <= JAHMESH_Y_DIMENSION; j++)
        {
            m_mesh_normal_save[i][j].x = MeshXNormal[i][j];
            m_mesh_normal_save[i][j].y = MeshYNormal[i][j];
            m_mesh_normal_save[i][j].z = MeshZNormal[i][j];
        }
    }
}

void 
JahLayer::restoreMeshNormals()
{
    for (int i = 0; i <= JAHMESH_X_DIMENSION; i++)
    {
        for (int j = 0; j <= JAHMESH_Y_DIMENSION; j++)
        {
            MeshXNormal[i][j] = m_mesh_normal_save[i][j].x;
            MeshYNormal[i][j] = m_mesh_normal_save[i][j].y;
            MeshZNormal[i][j] = m_mesh_normal_save[i][j].z;
        }
    }
}

void
JahLayer::generateMeshNormals()
{
    int i;
    int j;

    for (i = 0; i < JAHMESH_X_DIMENSION; i++)
    {
        for (j = 0; j < JAHMESH_Y_DIMENSION; j++) 
        {
            float4 vertex_coords;
            getMeshCoord(i, j, vertex_coords);
            float4 end1_coords;
            getMeshCoord(i + 1, j, end1_coords);
            float4 end2_coords;
            getMeshCoord(i, j + 1, end2_coords);

            float4 vector1 = end1_coords - vertex_coords;
            float4 vector2 = end2_coords - vertex_coords;

            float4 cross_vector = float4::crossProduct(vector1, vector2);
            setMeshNormal(i, j, cross_vector);
        }
    }

    float4 normal;
    j = JAHMESH_Y_DIMENSION;

    for (i = 0; i < JAHMESH_X_DIMENSION; i++)
    {
        getMeshNormal(i, j - 1, normal);
        setMeshNormal(i, j, normal);
    }

    i = JAHMESH_X_DIMENSION;

    for (j = 0; j < JAHMESH_Y_DIMENSION; j++)
    {
        getMeshNormal(i - 1, j, normal);
        setMeshNormal(i, j, normal);
    }

    getMeshNormal(JAHMESH_X_DIMENSION - 1, JAHMESH_Y_DIMENSION - 1, normal);
    setMeshNormal(JAHMESH_X_DIMENSION, JAHMESH_Y_DIMENSION, normal);
}

/////////////////////////////////////////////////
// vertex shader control utilities
/*
void JahLayer::initVertexShader(int shader)
{

    static int 					count = 2;

    //debug(">><<initializing vertex shader");

    if (GLEW_NV_fragment_program && GLEW_NV_vertex_program)
    {
        //debug(">><<initialized vertex shader");

        unsigned char* data = NULL;
        float stage = 1.0;

        if (shader==1)
            data = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/program1.vp");
        if (shader==2)
            data = loadshaderfile(JahBasePath+"source/Jahshaka/JahLibraries/shaders/program2.vp");

        glGenProgramsNV(1,&program);
        glBindProgramNV(GL_VERTEX_PROGRAM_NV,program);
        glLoadProgramNV(GL_VERTEX_PROGRAM_NV,program, (GLsizei)strlen((char*)data),data);

        glTrackMatrixNV(GL_VERTEX_PROGRAM_NV,0, GL_MODELVIEW_PROJECTION_NV,GL_IDENTITY_NV);

        glProgramParameter4fNV(GL_VERTEX_PROGRAM_NV,5,stage * 2, ((float)count / 3.0) - 0.50, count / 2, 16);

        delete data;

        count = (count == 16) ? 2 : count + 1;
    }
}

unsigned char * JahLayer::loadshaderfile(const char *name) {

    FILE*                       file; 
    int                         size;
    unsigned char*              data;

    file = fopen(name, "rb");

    if(!file) 
    { 
        fprintf(stderr, "JahLayer::loadshaderfile:: error loading %s file",name); 
        return NULL; 
    }

    fseek(file,0,SEEK_END);
    size = ftell(file);
    data = new unsigned char[size + 1];
    fseek(file,0,SEEK_SET);
    fread(data,1,size,file);
    data[size] = '\0';
    fclose(file);
    return data;
}
*/ 


