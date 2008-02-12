/*******************************************************************************
**
** The source file for the Jahshaka core model file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"

//////////////////////////////////////////////////////////
// Layer Control objects


void JahRender::drawLight( void) 
{
    if ( !Globals::getRendering() && Globals::getShowLights() )
    //if ( !Globals::getRendering() )
    {
        GLUquadricObj* q = gluNewQuadric();

        if (thelayer->selected) 
        {
            // Show selected layer as blue
            glColor3f(0.0f, 0.0f, 1.0f);
        }
        else
        {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        glNormal3f( 0.0, 0.0, 0.0 );

        // cylinder
        gluQuadricNormals( q, GLU_SMOOTH );
        gluQuadricTexture( q, GL_TRUE );

        gluSphere(q, 10.0f, 8, 8);
    }
}

