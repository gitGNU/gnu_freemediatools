/*******************************************************************************
**
** The header file for the Jahshaka render module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef RENDER_H
#define RENDER_H

#include "coreobject.h"
#include "jahpreferences.h"

#ifdef NEWOPENOBJECTLIB
#include <GL/glew.h>
#else
#include "glew.h"
#endif

class JahLayer;
class GLWorld;

class JahRender {

public:
    JahRender(GLWorld* glworld_ptr);
    ~JahRender(void);

	////////////////////////////////////////////////
    //object drawing routines
    void positionObject( motionNode * node );
    void drawLayer(JahLayer* layer, bool set_position);
    int drawLights(JahLayer* layer);
    void drawNodes(void);

	////////////////////////////////////////////////
	//routines to make objects and widgets
	//change all of these to render<>

    void makeObject( void );
    void makeKey(float offset);
    void makeTheLayerOutline( void );

	////////////////////////////////////////////////
	//objects 
	void makeLayer( JahLayer* layer );
	void makeLargeLayer( JahLayer* layer );
	void makeKeyLayer( float z_offset = 0.0f ); 
	void makeKeyLayerGLSL( JahLayer* jah_layer, bool set_position ); 
	void makeBlackBackground( void );
	void makeBackground( void );
    void makeTitleGrid( void );
	void makeCube( void );
	void makeCylinder( void );
	void makeSphere( void );
	void makeBezier( void );

	//for font objects
	void drawText( void );
    void addTextObj( void );
    void draw3dTextOutline( void );
    void makeText( void );
    bool loadFontText( void );
	bool loadQtFontText( void ); 
	
	//for 3d objects
	bool loadObj( void );
	void makeObj( void );

    //this routine draws the partciles
    void            makeParticle();
    void            addParticleObj();

    void            Atom(bool first_time);
    void            JetSpray(bool first_time);
    void            Waterfall(bool first_time);
    void            Balloons(bool first_time);
    void            Rocket(bool first_time);
    void            Shape(bool first_time);
    void            FireFlies(bool first_time) ;
    void            Shower(bool first_time);
    void            Swirl(bool first_time);
    void            Explosion(bool first_time);
    void            Fountain(bool first_time);

    void            DrawGroupQuadSplat(const pVector &, const pVector &,
                                       float size_scale, bool draw_tex,
                                       bool const_size, bool const_color);

    void            DrawGroupTriSplat(const pVector &view, const pVector &up,
                                      float size_scale, bool draw_tex,
                                      bool const_size, bool const_color);

    //////////////////////////////////////////////////
    //used for the mesh layer

    void            initializeMeshLayer( void);
    void            makeMeshLayer( void );
    void            setMeshCoord( int i, int j, GLfloat x, GLfloat y);
    void            setMeshCoord( int i, int j, GLfloat x, GLfloat y, GLfloat z);
	void            getMeshCoord( int i, int j, GLfloat& x, GLfloat& y, GLfloat& z);

	////////////////////////////////////////////////
	//for the lights
    void            drawLight( void);

	////////////////////////////////////////////////
	//widgets
	void makeLayerOutline( void ); 
	void makeBackdropOutline( void );
	void makeKeyFrames( void );
	void makeGrid(void );
	void makeLayerTranslate( void ); 
	void makeLayerScale( void );
	void makeLayerRotate( void );

	////////////////////////////////////////////////
    //the current tool?
    void setToolMode(MouseMode);

	//variable to track the tool
    MouseMode theToolMode;

    GLWorld*        getGlworldPtr() { return m_glworld_ptr; }
    void            setGlworldPtr(GLWorld* glworld_ptr) { m_glworld_ptr = glworld_ptr; }
    void            drawQuadToPbufferAndUpdateCompositeTexture(JahLayer* jah_layer);

protected:
    GLWorld*        m_glworld_ptr;
private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

	QString JahBasePath;
	JahLayer * thelayer;
};


#endif
