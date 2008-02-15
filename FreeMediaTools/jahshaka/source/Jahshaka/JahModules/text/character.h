/*******************************************************************************
**
** The header file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef CHARACTER_H
#define CHARACTER_H

#include <qtabwidget.h>
#include "anime.h"
#include "jahpluginlib.h"

class GLChar :  public  GLAnime 
{

    Q_OBJECT

private:
	QHBox *m_controls; 
	QHBox *m_leftcontroller; 
	QHBox *m_rightcontroller;

 public:
	GLChar( GLCore* core, const char* name,
		QHBox* controls, QHBox* controller, QHBox* leftcontroller,
		QHBox* rightcontroller,
		int* globalclipnumber, const QGLWidget* =0   );
	~GLChar();

    void buildInterface( QHBox* qtinterface );
    void createMenuItem( QPopupMenu* themenu );

public slots:
    void addLayer(void);
    void addLayerFromTopMenu(int val);
    void addLightFromTopMenu();

    #endif // CHARACTER_H

};





