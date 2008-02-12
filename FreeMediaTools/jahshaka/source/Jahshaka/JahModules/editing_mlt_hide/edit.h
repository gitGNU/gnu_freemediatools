/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef EDIT_H
#define EDIT_H

#include <glworld.h>

#include "base.h"

//for the mediatable
#include "mediatable.h"

// Jahshaka Preview and Project declarations
namespace jahnle
{
	class Preview;
	class Project;
	class Page;
}

// MLT forward declarations
namespace Mlt
{
	class Deque;
	class Consumer;
	class Playlist;
	class ClipInfo;
	class Properties;
	class Filter;
	class Geometry;
	class Frame;
}

// Core qt widgets used
class QTabWidget;

// Multitrack widget components
class MultiTrack;
class MultiTrackTrack;
class MultiTrackItem;

// Active tool mode
typedef enum
{
	tool_null = 0,
	tool_translate = 1,
	tool_scale = 2
}
nle_tool_mode;

// Editor class
class GLEdit : public GLWorld, public EditBase
{
	Q_OBJECT

	friend class jahnle::Project;
	friend class jahnle::Preview;
	friend class jahnle::Page;

	private:
		GLCore *m_core;
		QHBox *m_controls;
		QHBox *m_rightcontroller;
		QHBox *m_parent1;

	public:
		GLEdit( GLCore *, QHBox*, QHBox* parent1, const char *, QHBox *, QHBox *, QHBox *, QHBox *, int *, const QGLWidget *shareWidget = 0 );
		~GLEdit();
		virtual void start( );

	public:
	
		///////////////////////////////////////////////////////////////////
		//used to set up custom mediatable
    	void initializeMediaTable(QHBox* parentbox);
		JahDesktopSideTable *mediatable;
		void initializeEffects(void);
		void addAssetToEffectsTable(QString thedir, QString fxfilename);

		// We may use OpenGL, but this method isn't needed
		void updateGL( void ) { }

		// Overrides of GLWorld virtual methods (no ops)
		void layerClicked( QListViewItem * ) { }
    	void mouseMoveCamera( int, int ) { }
    	void mouseMoveLayer ( int, int, int ) { }
    	void addLayer( void ) { }
    	void clicked( QListViewItem * ) { }
    	void changeParticle() { }
    	void changeFontDraw() { }
    	void changeObjectDraw() { }
    	void keyCliplayer() { }
    	void keyClipkey() { }
    	void addClipKey( assetData ) { }
    	void updateEditanimframe( int ) { }
    	void updateSetinFrames( int, int ) { }
		void updateAnimation( int, bool ) { }
		virtual void updatePosition( ){ }
		virtual void schedulePosition( ) { }

		// Key frame methods
    	void addKeyframe( ) { }
    	void removeKeyframe( ) { }
    	void prevKeyframe( ) { }
    	void nextKeyframe( ) { }
    	void updateKeyframeDisplay( ) { }

		// Mouse handling
    	void mousePressEvent( QMouseEvent * );
    	void mouseReleaseEvent( QMouseEvent * );
    	void mouseMoveEvent( QMouseEvent  * );

		// Smooths updates while scrubbing (slots associated to main jahshaka 
		// slider but also reused for slide related widgets here)
		void sliderselected( ) { m_controlled = true; }
		void updateAnimation( ) { m_controlled = false; }
	
		// KB: Why, what ??
    	void toggleForcePlay( void ) { }
    	void setForcePlay( bool ) { }
	
		// Create an empty project
		void createProject( );
		void switchProject( jahnle::Project *, int );

		// Not sure why there's a distinction here (doesn't apply anyway)
    	void addClip( assetData asset ) { addClipEdit( asset ); }
    	void addClipEdit( assetData asset );
	
		// Grab and add?
    	void grabDesktop( void ) { emit GrabClip(); };
    	void addDesktop( void ) { emit AddClip(); };
	
		// Refresh UI
    	void updateUiSettings( );
    	void updateUI( ) { updateUiSettings( ); }
	
		// Slider related methods
    	void updateSlider( int value );
		void updatePosition( int value ) { emit updateSlider( value ); }
	
		// Transport related methods
    	void firstframeanimation( void ) { seek( 0 ); }
    	void previousframeanimation( void ) { setSpeed( 0 ); seek( position( ) - 1 ); }
    	void nextframeanimation( void ) { setSpeed( 0 ); seek( position( ) + 1 ); }
    	void lastframeanimation( void ) { seek( length( ) - 1 ); }
    	void startanimation() { setSpeed( 1 ); }
    	void stopanimation() { setSpeed( 0 ); }
    	void toggleAnimation( bool value ) { setSpeed( value ? 1 : 0 ); }
	
		// Render methods
		QImage Photo( assetData );
    	void RenderScene( void );
	
		// Keyboard handling
		void keyPressEvent( QKeyEvent *k );
		void keyReleaseEvent( QKeyEvent *k );
	
		// Undo/redo
		void ignoreChanges( bool ignore = true ) { m_ignore_changes = ignore; if ( !ignore ) checkPoint( ); }
		void checkPoint( );
		void undo( );
		void redo( );

		// Get the tool mode
		nle_tool_mode getToolMode( );

		// Tab page switcher
		void setPage( int );

		// Hook back for video head
		void glworldUpdate( );

		// Provides access to the jah base path
		const QString &getMediaPath( ) { return JahMediaPath; }

		// Head render override
		virtual void headRender( int width = 0, int height = 0 );

	public slots:
		// New, open and save project
		void newProject( );
		void openProject( );
		void saveProject( );
		void saveDesktop( );

		// Override of GLWorld virtual
    	void sliderValueChanged( int value );

		// Tab page changed
		void pageChanged( QWidget * );

		// Set playback speed
		void setSpeed( int speed = 1 );
	
		// Seek to a position
		void seek( int position );

		// Length of the project
		int length( );
	
		// Get the position in the current project
		int position( );

		// Is the position under the users control?
		bool isControlled( ) { return true; }

		// Callback from the preview when a frame is about to be shown
		void consumerFrameShow( Mlt::Frame & );

		// Provides access to the gl core
		GLCore *getCore( ) { return core; }

		// Translate and scale tools
		void toolNull( );
		void toolTranslate( );
		void toolScale( );
    void toggleZoomLock( );

		// Effect selection changed
		void assetSelected( assetData );

		// Zoom events
		void setZoom( int value );
		void updateZoomOut( );
		void updateZoomIn( );

	signals:
    	void GrabClip( void );
    	void AddClip( void );
	
	protected:
		// Override of the keyframe mechanism on the glworld which isn't supported here
		virtual bool usesKeyFrames( ) { return false; }

		// Convenience methods for Page objects
		jahnle::Preview *preview( ) { return m_preview; }
		jahnle::Project *project( ) { return m_project; }

		// Initialise MLT Core and components
		void initialiseSubsystem( GLCore * );
	
		// Build the UI
    	void buildInterface( QHBox * );
    	void buildRightController( QHBox * );
        void buildController( QHBox* );
	
		// Redraw method
    	void paintGL( );
	
		// Tool mode manipulation and accessors
		void setToolMode( nle_tool_mode );

	private:
		// Preview and Project Objects
		jahnle::Preview *m_preview;
		jahnle::Project *m_project;

		// GUI Object for tab control
		QTabWidget *m_tab_frame;

		// GUI Objects for the project settings
		jahnle::Page *m_page[ 4 ];

		// Undo/Redo state
		bool m_ignore_changes;
		Mlt::Deque *m_undo;
		Mlt::Deque *m_redo;
		Mlt::Consumer *m_serialiser;
		Mlt::Consumer *m_auto_save;

		// State of controller(s)
		bool m_controlled;

		// Tool mode
		nle_tool_mode m_tool_mode;

		// State of mouse
		void getMouseCoords( QMouseEvent *, double &, double & );
		double m_start_x;
		double m_start_y;

		// For start up diagnostics
		QString m_error_msg;

	public:
		// Range check
		static int rangeCheck( int v, int min, int max ) { return v < min ? min : v > max ? max : v; }
};

#endif

