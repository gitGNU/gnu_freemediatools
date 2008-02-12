/*******************************************************************************
**
** The header file for the Jahshaka Intro module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLTEXOBJ_H
#define GLTEXOBJ_H

//#include "glew.h"

//#include "glew.h"
#include <qgl.h>
//#include <GL/gl.h>

#include <qimage.h>
#include <qtimer.h>

#include "jahpreferences.h"

class GLTexobj : public QGLWidget
{
    Q_OBJECT

public:

    GLTexobj( QWidget* parent, const char* name );
    ~GLTexobj();

    QString * vendor, * renderer, * version, * extensions;
    bool firsttime;

public slots:

    void		toggleAnimation();

signals:
    void    introFinished(void);

protected:

    void		initializeGL();
    void		paintGL();
    void		resizeGL( int w, int h );

	void		makeObjectLayer ( GLuint & theObj, const QImage& tex1 );

private:
    bool animation;
    GLuint object1, object2;
    GLfloat xRot, yRot, zRot, scale, zPos;
    QTimer* timer;
    
    QString JahBasePath;

    QImage tex1, tex2;

};


#endif // GLTEXOBJ_H
