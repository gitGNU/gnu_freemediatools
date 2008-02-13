/*******************************************************************************
**
** The source file for the Jahshaka core font file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahrender.h"

#include <qfont.h>

#include <FTGLOutlineFont.h>
#include <FTGLPolygonFont.h>
#include <FTGLBitmapFont.h>
#include <FTGLTextureFont.h>
#include <FTGLPixmapFont.h>
#include <FTGLExtrdFont.h>

//////////////////////////////////////////////////////////
// Font Control routines

void JahRender::drawText(void) 
{
    if (!thelayer->text)
    {
        return;
    }

    int current_font;

    if (thelayer->text->fontshere) 
    {

        glEnable( GL_LINE_SMOOTH);

        //if textureing is on... need a flag for this also FTGL doesnt support it yet
        //glEnable(GL_TEXTURE_2D);

        switch ( thelayer->text->fontstyle ) {

            case FontDrawstyle::FOUTLINE     :     
            {    
                current_font = 0;  
                glLineWidth(2.0f);
                break; 
            }
            case FontDrawstyle::FPOLYGON     :     {    current_font = 1;                 break; }
            case FontDrawstyle::FTEXTURE     :     {    current_font = 2;                 break; }
            case FontDrawstyle::FBITMAP      :     {    current_font = 3;                 break; }
            
            case FontDrawstyle::FEXTRUDE     :     
            {    
                current_font = 4;
                float extrude_depth = thelayer->layernodes->m_node->extrude;
                thelayer->text->fonts[current_font]->Depth(extrude_depth);
                break; 
            }

            case FontDrawstyle::FPIXMAP      :     {    current_font = 5;                 break; }
            default           :     {    current_font = 1;                 break; }

        }

    
        //a embarasing positioning rig!
        glPushMatrix(); //first we correct the orientation
        glTranslatef( -40.0, 0.0, 0.0 );
        glScalef ( 5, 5, 5 );

        glNormal3f( 0.0, 0.0, 1.0);
        glColor3f(thelayer->Red * 4, thelayer->Green * 4, thelayer->Blue * 4);  // HACK FIXME
		
		//////////////////////////////////////////////////////////////////
		// does this affect when we add them as well and is this necessary?
		//may be due to lighting ro something else
		// Colors are artificially brightened here to compensate for some
		// problem with load saved scenes having colors too dark

        //this command actually draws the object
        thelayer->text->fonts[current_font]->Render( thelayer->text->text);

        glDisable( GL_LINE_SMOOTH);

        glPopMatrix();

    }
}


void JahRender::draw3dTextOutline(void) 
{

    if (thelayer->text->fontshere) 
    {

        int current_font = 4;

        glLineWidth( 0.5 );
        glEnable( GL_LINE_SMOOTH);

        float x1, y1, z1, x2, y2, z2;
        thelayer->text->fonts[current_font]->BBox( thelayer->text->text, x1, y1, z1, x2, y2, z2);

        // Draw the bounding box
        glDisable( GL_LIGHTING);
        
        //glDisable( GL_TEXTURE_2D);

        glEnable(GL_BLEND);
        glBlendFunc( GL_SRC_ALPHA, GL_ONE); // GL_ONE_MINUS_SRC_ALPHA
    
        //a embarasing positioning rig!
        glPushMatrix(); //first we correct the orientation
        glTranslatef( -40.0, 0.0, 0.0 );
        glScalef ( 5, 5, 5 );

        glColor3f( 0.0, 1.0, 0.0);

        // Draw the front face
        glBegin( GL_LINE_LOOP);
            glVertex3f( x1, y1, z1);
            glVertex3f( x1, y2, z1);
            glVertex3f( x2, y2, z1);
            glVertex3f( x2, y1, z1);
        glEnd();
        
        // Draw the back face
        if( current_font == 4 && z1 != z2)
        {
            glBegin( GL_LINE_LOOP);
                glVertex3f( x1, y1, z2);
                glVertex3f( x1, y2, z2);
                glVertex3f( x2, y2, z2);
                glVertex3f( x2, y1, z2);
            glEnd();
            // Join the faces
            glBegin( GL_LINES);
                glVertex3f( x1, y1, z1);
                glVertex3f( x1, y1, z2);

                glVertex3f( x1, y2, z1);
                glVertex3f( x1, y2, z2);

                glVertex3f( x2, y2, z1);
                glVertex3f( x2, y2, z2);

                glVertex3f( x2, y1, z1);
                glVertex3f( x2, y1, z2);
            glEnd();
        }

            // Draw the baseline, Ascender and Descender
        glBegin( GL_LINES);
            glColor3f( 0.0, 0.0, 1.0);
            glVertex3f( 0.0, 0.0, 0.0);
            glVertex3f( thelayer->text->fonts[current_font]->Advance( thelayer->text->text), 0.0, 0.0);
            glVertex3f( 0.0, thelayer->text->fonts[current_font]->Ascender(), 0.0);
            glVertex3f( 0.0, thelayer->text->fonts[current_font]->Descender(), 0.0);

        glEnd();

        // Draw the origin
        glColor3f( 1.0, 0.0, 0.0);
        glPointSize( 5.0);
        glBegin( GL_POINTS);
        glVertex3f( 0.0, 0.0, 0.0);
        glEnd();
        glPointSize( 0.5);

        glPopMatrix();    

        glDisable(GL_BLEND);
        glDisable( GL_LINE_SMOOTH);
    }
}


bool JahRender::loadFontText(void) 
{
    if (!thelayer->text)
    {
        return false;
    }

    //need to delete and recreate object here if it exists
    //array delete crashes linux ie delete [] text->fonts;
    if (thelayer->text->fontshere) 
	{	
		delete thelayer->text->fonts[0]; 
		delete thelayer->text->fonts[1];
        delete thelayer->text->fonts[2]; 
		delete thelayer->text->fonts[3];
        delete thelayer->text->fonts[4]; 
		delete thelayer->text->fonts[5];  
	}

    //check if the file exists first ie look for text->font
    FILE	*check; 
	bool fileok;

	//debug("font:"+text->font);

    check = fopen((const char*)thelayer->text->font, "r");

	if (!check)
	{
		if (thelayer->text->font.startsWith("./"))
		{
			thelayer->text->font.remove(0,2);
			thelayer->text->font = JahBasePath + thelayer->text->font;

		    check = fopen((const char*)thelayer->text->font, "r");
		}
	}


    if (!check) 
	{ 
		//if its not ok lets check for relative paths before giving up
		//we can clean tis up later on...

		fileok = false; 
		thelayer->text->fontshere = false; 
	} 
	else 
	{ 
		fileok = true; 
		fclose(check); 
	}

    //if the files there then load it in
    if (fileok)
    {
        thelayer->text->fontshere = true;

        thelayer->text->fonts[0] = new FTGLOutlineFont( thelayer->text->font );
        thelayer->text->fonts[1] = new FTGLPolygonFont( thelayer->text->font );
        thelayer->text->fonts[2] = new FTGLTextureFont( thelayer->text->font );
        thelayer->text->fonts[3] = new FTGLBitmapFont( thelayer->text->font );
        thelayer->text->fonts[4] = new FTGLExtrdFont( thelayer->text->font );
        thelayer->text->fonts[5] = new FTGLPixmapFont( thelayer->text->font );

        for (int i=0; i<= 5; i++) 
        {

	  jtrace->debug ("Reading font" );

            int point_size = 24;

            thelayer->text->fonts[i]->Depth(thelayer->layernodes->m_node->extrude);
            thelayer->text->fonts[i]->CharMap(ft_encoding_unicode);

            if (!thelayer->text->fonts[i]->FaceSize(point_size)) 
            {
	      jtrace->error( "ERROR: Unable to set font face size " );
                fileok = false; 
		thelayer->text->fontshere = false;
            }
        }

    }

  return fileok;

}


void JahRender::makeText(void) 
{
	//all this jumbo only if font data is new or changed
	//and font is not already loaded
	if (loadFontText()) 
	{	
	  drawText(); 
	}
}

///////////////////////////////////////////////////////////
// for Qt font support

bool JahRender::loadQtFontText(void) 
{
	//load the font    
	//should only do this if the font has changed...
	//use a static to test this one
	thelayer->text->theqfont->setFamily("Helvetica");

	//set up the font
	thelayer->text->theqfont->setItalic(thelayer->text->italic);
	thelayer->text->theqfont->setUnderline(thelayer->text->underline);
	thelayer->text->theqfont->setBold(thelayer->text->bold);
	thelayer->text->theqfont->setPointSize(thelayer->text->pointsize);
    
	//You can specify the foundry you want in the family name. Both fonts, f1 and f2, in the above example will be set to "Helvetica [Cronyx]". 
	//To determine the attributes of the font actually used in the window system, use a QFontInfo object, e.g. 

    QFontInfo info( *thelayer->text->theqfont );
    QString family = info.family();
	debug(family);
    
	return true;
}

