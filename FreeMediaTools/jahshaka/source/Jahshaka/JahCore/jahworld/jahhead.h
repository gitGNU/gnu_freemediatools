/*******************************************************************************
**
** The header file for the Jahshaka video head class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAH_HEAD_
#define JAH_HEAD_

#include <GL/glew.h>
#include <qgl.h>
#include "jahheadable.h"

class JahHead : public QGLWidget
{
 public:

  JahHead( const QGLWidget *core, QWidget * parent = 0, const char * name = 0,
	   WFlags f = 0 ) ;

  void showInHead( JahHeadable *other );
 protected:
  void initializeGL();
  void resizeGL( int w, int h );
  void paintGL( );
 protected:
  void toggle_full_screen( );
  void keyPressEvent( QKeyEvent *e );
 private:
  JahHeadable *other_;
  int width_, height_;
};


#endif
