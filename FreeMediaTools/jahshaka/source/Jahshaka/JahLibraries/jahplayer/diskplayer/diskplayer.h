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

#ifndef JahPlayer_h
#define JahPlayer_h

//necessary Qt libraries
#include <qstring.h>
#include <qdialog.h>
#include <qwmatrix.h>
#include <qmessagebox.h>
#include <qimage.h>
#include <qdropsite.h>
#include <qdragobject.h>

//the visual media openlibraries
#include "openmedialib.h"
#include "gpumathlib.h"

//the jahshaka gl core object
#include "glcore.h"

//the jahtracer
#include "jahtracer.h"

class JahPlayer	: public GLCore
{
	Q_OBJECT

public:
	JahPlayer(QWidget *parent = 0, const char* name = 0);
	~JahPlayer();

    //the asset to be loaded in the player
    assetData       playerAssetData;
    assetExchange* exchange;
    
    //video head preferences
    bool    videohead;

	void	Reset();
	bool	InitializePlayer(assetData desktopclip);
	int		getCurrentFrame(void);	

    void    toggleZoomMode (void);

	//display color modes here
	colorMode theColormode;
	bool useColorModes;

	void    toggleColorMode  (void) { theColormode = D_ALL;   drawFrame();   };
	void    toggleColorRMode (void) { theColormode = D_RED;   drawFrame();  };
	void    toggleColorGMode (void) { theColormode = D_GREEN; drawFrame();   };
	void    toggleColorBMode (void) { theColormode = D_BLUE;  drawFrame();  };
	void    toggleColorAMode (void) { theColormode = D_ALPHA; drawFrame();   };


	void	SetDisplayZoom(double);

    void    CalcZoomValues(void);

    bool    zoommode;
	
	bool playerisactive;

    //this is a pointer to the image to be drawn
    QImage		*currentframe;

	//for moder day texture mapping
	GLuint		the_texture;	

	void	drawFrame(QImage * theimage, int frameNo);

public slots:
    void	LoadTheFrame(int frame);
	
signals:
	void	LoadingFrame(int);
	void    resizePlayer(mediaSize);
	void	updateplayer2(QPixmap);

    void    draggedAImage();

protected:
	void	buildTexture();
	void	updateTexture();
	void	drawTexture(); 
	void	calculateTextureSizes(); 
	void	calculateColorMode(); 

	void	drawFrame();
    void	paintEvent(QPaintEvent * );

    ///////////////////////////////////////////////////////////////
    void dragEnterEvent(QDragEnterEvent* event);
    void dropEvent(QDropEvent* event);

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

	int			loadZoom;
	float		displayZoom;

	bool    flipColorData;
	GLenum      texture_format;
    int         texture_mode;

	int			loadFrame;
	GLint		frameWidth;
	GLint		frameHeight;

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

};


#endif // JahPlayer_h
