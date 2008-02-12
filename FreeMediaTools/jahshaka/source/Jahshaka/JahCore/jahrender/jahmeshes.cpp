/*******************************************************************************
**
** The source file for the Jahshaka core model file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"

//#warning  FIXME    The mesh definition needs to be cleaned up
#define MESH_X_DIMENSION                            72               
#define MESH_Y_DIMENSION                            48

//////////////////////////////////////////////////////////
// Mesh Control objects

void JahRender::initializeMeshLayer( void) 
{ 
    GLfloat                 scene_width;
    GLfloat                 scene_height;
    GLfloat                 aspect_ratio;
    GLfloat                 width_scale;
    GLfloat                 height_scale;

    scene_width  = thelayer->getImageWidth();
    scene_height = thelayer->getImageHeight();


    // I'm not sure where the 0.5 factor comes from, but it seems to work
    aspect_ratio = ((float)scene_width / (float)scene_height) * 0.5;

    height_scale = scene_height / (float)MESH_Y_DIMENSION;
    width_scale = scene_width / (float)MESH_X_DIMENSION;

    for (int i = 0; i <= MESH_X_DIMENSION; i++) 
    {
        for (int j = 0; j <= MESH_Y_DIMENSION; j++) 
        {
            //set x and y coords in mesh
            thelayer->MeshXCoord[i][j] = (float)(i - (MESH_X_DIMENSION / 2)) * width_scale; 
            thelayer->MeshYCoord[i][j] = (float)(j - (MESH_Y_DIMENSION / 2)) * height_scale;
            thelayer->MeshZCoord[i][j] = 0.0;

            thelayer->MeshXNormal[i][j] = 0.0f;
            thelayer->MeshYNormal[i][j] = 0.0f;
            thelayer->MeshZNormal[i][j] = 1.0f;
        }
    }

}

////////////////////////////////////////////////////////////
//these may need to stay in the layer routines
void JahRender::setMeshCoord( int i, int j, GLfloat x, GLfloat y) 
{
    thelayer->MeshXCoord[i][j] = x;
    thelayer->MeshYCoord[i][j] = y;
}

void JahRender::setMeshCoord( int i, int j, GLfloat x, GLfloat y, GLfloat z) 
{
    thelayer->MeshXCoord[i][j] = x;
    thelayer->MeshYCoord[i][j] = y;
    thelayer->MeshZCoord[i][j] = z;
}

void JahRender::getMeshCoord( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z)
{
    x = thelayer->MeshXCoord[i][j];
    y = thelayer->MeshYCoord[i][j];
    z = thelayer->MeshZCoord[i][j];
}


void JahRender::makeMeshLayer( void) 
{
    //do we need this here? we use the smooth variable in jahrender.cpp to set smooth shading
    //we set a override at the end of the routine to set it back to what it supposed to be
    glShadeModel( GL_SMOOTH ); 
    
    //bool usedagpu = false;

    if (thelayer->ObjectDisplayMode) 
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    
    int                         ii, jj;

    GLfloat                     i, j;


    float4 texture_ratio;
    GLuint texture_id = thelayer->getBaseOrCompositeTextureId();
    //printf("JahRender::makeMeshLayer : Drawing texture id %d\n", texture_id);
    glBindTexture(GL_TEXTURE_2D, texture_id);
    texture_ratio.x = thelayer->textureData.texwidthratio;
    texture_ratio.y = thelayer->textureData.texheightratio;

    GLint current_light_model;

    glGetIntegerv(GL_LIGHT_MODEL_TWO_SIDE, &current_light_model);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, 1);

    glBegin(GL_QUADS);

    for (jj = 0; jj < MESH_Y_DIMENSION; jj++) 
    {
        j = (float)jj;

        for (ii = 0; ii < MESH_X_DIMENSION; ii++) 
        {   
            i = (float)ii;

            glTexCoord2f(	
                        (i / MESH_X_DIMENSION) * texture_ratio.x, 
                        (j / MESH_Y_DIMENSION) * texture_ratio.y);
            glNormal3f(
                        thelayer->MeshXNormal[ii][jj],
                        thelayer->MeshYNormal[ii][jj],
                        thelayer->MeshZNormal[ii][jj]);
            glVertex3f( 
                        thelayer->MeshXCoord[ii][jj], 
                        thelayer->MeshYCoord[ii][jj], 
                        thelayer->MeshZCoord[ii][jj]);

            glTexCoord2f(
                        ((i + 1) / MESH_X_DIMENSION) * texture_ratio.x, 
                        ((j + 0) / MESH_Y_DIMENSION) * texture_ratio.y);
            glNormal3f( 
                        thelayer->MeshXNormal[ii + 1][jj], 
                        thelayer->MeshYNormal[ii + 1][jj], 
                        thelayer->MeshZNormal[ii + 1][jj]);
            glVertex3f( 
                        thelayer->MeshXCoord[ii + 1][jj], 
                        thelayer->MeshYCoord[ii + 1][jj], 
                        thelayer->MeshZCoord[ii + 1][jj]);

            glTexCoord2f(
                        ((i + 1) / MESH_X_DIMENSION) * texture_ratio.x, 
                        ((j + 1) / MESH_Y_DIMENSION) * texture_ratio.y); 
            glNormal3f( 
                        thelayer->MeshXNormal[ii + 1][jj + 1],   
                        thelayer->MeshYNormal[ii + 1][jj + 1],   
                        thelayer->MeshZNormal[ii + 1][jj + 1]);
            glVertex3f( 
                        thelayer->MeshXCoord[ii + 1][jj + 1],   
                        thelayer->MeshYCoord[ii + 1][jj + 1],   
                        thelayer->MeshZCoord[ii + 1][jj + 1]);

            glTexCoord2f(
                        ((i + 0) / MESH_X_DIMENSION) * texture_ratio.x, 
                        ((j + 1) / MESH_Y_DIMENSION) * texture_ratio.y);
            glNormal3f( 
                        thelayer->MeshXNormal[ii][jj + 1],   
                        thelayer->MeshYNormal[ii][jj + 1],   
                        thelayer->MeshZNormal[ii][jj + 1]);
            glVertex3f( 
                        thelayer->MeshXCoord[ii][jj + 1],   
                        thelayer->MeshYCoord[ii][jj + 1],   
                        thelayer->MeshZCoord[ii][jj + 1]);
        }
    }

    glEnd(); //GL_QUADS

    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, current_light_model);

    if (thelayer->ObjectDisplayMode) 
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
    
    //this is used to set the GL_SMOOTH at the top of the routine back to what it supposed to be
    if (thelayer->smooth)
    {
        //glShadeModel( GL_SMOOTH ); 
    }
    else
    {
        glShadeModel( GL_FLAT ); 
    }

}


