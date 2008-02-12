/*******************************************************************************
**
** The header file for the Jahshaka player JahPlayer file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
** Based on * flip - pageflipper for TIFF animations
** Copyright (C) 1999 Mark B. Allan (mba@reptilelabour.com)
**
** in the process of being rewritten
**
*******************************************************************************/

#ifndef VIDEOHEAD_H
#define VIDEOHEAD_H

//necessary Qt libraries
#include <qstring.h>
#include <qdialog.h>
#include <qwmatrix.h>
#include <qmessagebox.h>
#include <qimage.h>

//the visual media openlibraries
#include "gpumathlib.h"

//the jahshaka gl core object
#include "glcore.h"

//the jahtracer
#include "jahtracer.h"
#include "jahpreferences.h"

class VideoHead	: public GLCore
{
	Q_OBJECT

public:
	VideoHead(QWidget *parent = 0, const char* name = 0);
	~VideoHead();
   
	bool	InitializePlayer(QImage *theimage = 0, bool status = 0);

    void    updateDisplayZoom(void);
    void    CalcZoomValues(void);

    bool    playerisactive;
	void	Reset();
	
	void	drawFrame(QImage * theimage);

	//for moder day texture mapping
	GLuint		the_texture;	
    bool    zoommode;
	bool	staticPlayer;

public:
    void	drawFrame();

    void    toggleZoomMode (void);

	void	SetDisplayZoom(double);

	//overloaded and public
	void	drawTexture(	GLuint		thenew_texture	);

protected:
	void	buildTexture();
	void	updateTexture();
	void	drawTexture(); 

	void	calculateTextureSizes(); 
	void	calculateColorMode(); 

    void	paintEvent(QPaintEvent * );

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

    //this is a pointer to the image to be drawn
    QImage		*currentframe;
    QImage		*cacheImage;

	int			frameWidth;
	int			frameHeight;

	bool    flipColorData;
	GLenum      texture_format;
    int         texture_mode;

	int			loadZoom;
	float		displayZoom;

    int zoomPos_x;
    int zoomPos_y;

    double zoomScale_x;
    double zoomScale_y;

    GLfloat zoomWidth;
    GLfloat zoomHeight;

    GLfloat    texture_x;
    GLfloat    texture_y;
    GLfloat    vertex_x;
    GLfloat    vertex_y;
    GLfloat wresolution;//   = mapvalw;    // was layerres
    GLfloat hresolution;//   = mapvalh;    // was layerres

	QString JahBasePath;
};


#endif // JahPlayer_h
