/*******************************************************************************
**
** The source file for the Jahshaka RenderObject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"

//////////////////////////////////////////////////////////////
// Draw layer outlines

void JahRender::makeLayerOutline( void) 
{  

    glNormal3f( 0.0, 0.0, 0.0 );
    glLineWidth(2.0f);

    glColor3f(1.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glBegin(GL_POLYGON);

        glVertex3f(-thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Left Of The Texture and Quad
        glVertex3f( thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Right Of The Texture and Quad
        glVertex3f( thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
        glVertex3f(-thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    glColor3f(1.0f, 1.0f, 1.0f);

}

//for the titlesafef area
void JahRender::makeTitleGrid( void ) 
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    GLfloat titleWidth  = thelayer->widthOffset;// * (8/10);
    GLfloat titleHeight = thelayer->heightOffset;// * (8/10);

	//outer mesh
    glBegin(GL_POLYGON);
    
	glVertex3f(-titleWidth, -titleHeight,  0.0f);	// Bottom Left Of The Texture and Quad
    glVertex3f( titleWidth, -titleHeight,  0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f( titleWidth,  titleHeight,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
    glVertex3f(-titleWidth,  titleHeight,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

	//inner mesh
    titleWidth  = titleWidth * 0.80;
    titleHeight = titleHeight * 0.80; //was 8/10

    glBegin(GL_POLYGON);

	glVertex3f(-titleWidth, -titleHeight,  0.0f);	// Bottom Left Of The Texture and Quad
    glVertex3f( titleWidth, -titleHeight,  0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f( titleWidth,  titleHeight,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
    glVertex3f(-titleWidth,  titleHeight,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

}

void JahRender::makeBackdropOutline(void) 
{

    glNormal3f( 0.0, 0.0, 0.0 );

    glColor3f(0.0f, 0.0f, 0.70f);
    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glBegin(GL_POLYGON);

        glVertex3f(-thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Left Of The Texture and Quad
        glVertex3f( thelayer->widthOffset, -thelayer->heightOffset,  0.0f);	// Bottom Right Of The Texture and Quad
        glVertex3f( thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
        glVertex3f(-thelayer->widthOffset,  thelayer->heightOffset,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    
    glColor3f(1.0f, 1.0f, 1.0f);

}

//////////////////////////////////////////////////////////////////////
// Draw the layers keyframes

void JahRender::makeKeyFrames(void ) 
{
	//we want to loop through array of nodes[]for object
	//and draw each keyframe connected by dots

    glLineWidth( 0.5 );
    glEnable( GL_LINE_SMOOTH);

    // Draw the bounding box
    glEnable(GL_BLEND);
    glEnable(GL_DEPTH);
    glBlendFunc( GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA

    glColor3f( 1.0, 0.0, 0.0);

	if (thelayer->layernodes->m_nodelist->count()>0) 
	{

		for (unsigned int x=0; x<thelayer->layernodes->m_nodelist->count(); x++) 
		{

		//we only draw the keyframe for the first frame
		if (x==0) {

			glBegin(GL_LINES);

			//horizontal line
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx-1, thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx+1, thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz);

			// Vertical lines.
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty-1, thelayer->layernodes->m_nodelist->at(x)->tz);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty+1, thelayer->layernodes->m_nodelist->at(x)->tz);

			// depth lines.
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz-1);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz+1);

			glEnd();
		}
		else
		{
			//for all keyframes after the first we draw the connecting line
			glBegin(GL_LINES);
			//horizontal line
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx-1, thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx+1, thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz);

			// Vertical lines.
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty-1, thelayer->layernodes->m_nodelist->at(x)->tz);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty+1, thelayer->layernodes->m_nodelist->at(x)->tz);

			// depth lines.
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz-1);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz+1);

			glEnd();

		glBegin(GL_LINES);
			//horizontal line
			glVertex3f(thelayer->layernodes->m_nodelist->at(x-1)->tx, thelayer->layernodes->m_nodelist->at(x-1)->ty, thelayer->layernodes->m_nodelist->at(x-1)->tz);
			glVertex3f(thelayer->layernodes->m_nodelist->at(x)->tx,   thelayer->layernodes->m_nodelist->at(x)->ty,   thelayer->layernodes->m_nodelist->at(x)->tz);

		glEnd();
		}

		}//end keyframe loop
	}
	else
	{

		//this means its just the node for the layer so no keyfrane has been set
		glBegin(GL_LINES);
		//horizontal line
		glVertex3f(thelayer->layernodes->m_node->tx-1, thelayer->layernodes->m_node->ty,   thelayer->layernodes->m_node->tz);
		glVertex3f(thelayer->layernodes->m_node->tx+1, thelayer->layernodes->m_node->ty,   thelayer->layernodes->m_node->tz);

		// Vertical lines.
		glVertex3f(thelayer->layernodes->m_node->tx,   thelayer->layernodes->m_node->ty-1, thelayer->layernodes->m_node->tz);
		glVertex3f(thelayer->layernodes->m_node->tx,   thelayer->layernodes->m_node->ty+1, thelayer->layernodes->m_node->tz);

		// depth lines.
		glVertex3f(thelayer->layernodes->m_node->tx,   thelayer->layernodes->m_node->ty,   thelayer->layernodes->m_node->tz-1);
		glVertex3f(thelayer->layernodes->m_node->tx,   thelayer->layernodes->m_node->ty,   thelayer->layernodes->m_node->tz+1);
		glEnd();

	}

    glDisable(GL_BLEND);
    glDisable( GL_LINE_SMOOTH);

	glColor3f( 1.0f, 1.0f, 1.0f );
}


void JahRender::makeGrid(void) 
{

    // This is the old grid code, new code does it all in hardware
    // although we need to see if this covers older boards
    // Render grid over 0..rows, 0..columns.
    glColor3f( 0.7f, 0.7f, 0.7f);

    glLineWidth( 0.5 );
    glEnable( GL_LINE_SMOOTH);

    //glDisable( GL_TEXTURE_2D);

    glBlendFunc( GL_SRC_ALPHA, GL_ONE);

    const GLfloat grid2x2[2][2][3] = 
    {
        {{-800.0, -250.0, -800.0},
        {  800.0, -250.0, -800.0}},
        {{-800.0, -250.0,  800.0},
        {  800.0, -250.0,  800.0}}
    };

    glEnable(GL_MAP2_VERTEX_3);

    glMap2f( GL_MAP2_VERTEX_3,
        0.0, 1.0,  // U ranges 0..1 
        3,         // U stride, 3 floats per coord 
        2,         // U is 2nd order, ie. linear 
        0.0, 1.0,  // V ranges 0..1 
        2 * 3,     // V stride, row is 2 coords, 3 floats per coord 
        2,         // V is 2nd order, ie linear 
        **grid2x2);  // control points 


    glMapGrid2f(
        60, 0.0, 1.0,
        60, 0.0, 1.0);


    glEvalMesh2(GL_LINE,
        0, 60,   // Starting at 0 mesh 5 steps (rows). 
        0, 60);  // Starting at 0 mesh 6 steps (columns). 


    glDisable(GL_MAP2_VERTEX_3);
    glDisable( GL_LINE_SMOOTH);

}

//////////////////////////////////////////////////////////
// Layer Control objects

void JahRender::makeLayerTranslate( void) 
{

    glNormal3f( 0.0, 0.0, 0.0 );

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glColor3f(0.0f, 0.0f, 0.7f);

    glLineWidth( 2.0 );

    glBegin(GL_POLYGON); 

    glVertex3f(-thelayer->textureData.mapwidth, -thelayer->textureData.mapheight,  0.0f);	// Bottom Left Of The Texture and Quad
    glVertex3f( thelayer->textureData.mapwidth, -thelayer->textureData.mapheight,  0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f( thelayer->textureData.mapwidth,  thelayer->textureData.mapheight,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
    glVertex3f(-thelayer->textureData.mapwidth,  thelayer->textureData.mapheight,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

    //reset poly mode...
    glLineWidth( 0.5 );

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);



}

void JahRender::makeLayerScale( void) 
{

    glNormal3f( 0.0, 0.0, 0.0 );

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glLineWidth( 2.0 );

    glColor3f(0.0f, 0.0f, 0.7f);

    glBegin(GL_POLYGON); // begin drawing a cube

    //line setup data

    glVertex3f(-thelayer->textureData.mapwidth, -thelayer->textureData.mapheight,  0.0f);	// Bottom Left Of The Texture and Quad
    glVertex3f( thelayer->textureData.mapwidth, -thelayer->textureData.mapheight,  0.0f);	// Bottom Right Of The Texture and Quad
    glVertex3f( thelayer->textureData.mapwidth,  thelayer->textureData.mapheight,  0.0f);	// Top Right Of The Texture and Quad     1.0f 1.0f
    glVertex3f(-thelayer->textureData.mapwidth,  thelayer->textureData.mapheight,  0.0f);	// Top Left Of The Texture and Quad

    glEnd();

    //reset poly mode...
    glLineWidth( 0.5 );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


}

void JahRender::makeLayerRotate( void) 
{
    glNormal3f( 0.0, 0.0, 0.0 );

    glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);


    // the rotate object
    GLUquadricObj* q = gluNewQuadric();
    glLineWidth( 2.0 );

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glNormal3f( 0.0, 0.0, 0.0 );

    // cylinder
    gluQuadricNormals( q, GLU_SMOOTH );


    glColor3f(  0.7f, 0.0f, 0.0f);
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    gluDisk( q, 60.0, 60.0, 24, 1 );

    glColor3f(0.7f, 0.0f, 0.7f);
    glRotatef( 90.0, 0.0, 1.0, 0.0 );
    gluDisk( q, 60.0, 60.0, 24, 1 );

    glColor3f(0.0f, 0.0f, 0.7f);
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    gluDisk( q, 60.0, 60.0, 24, 1 );


    glPopMatrix();

    //reset poly mode...
    glLineWidth( 0.5 );
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);


}

