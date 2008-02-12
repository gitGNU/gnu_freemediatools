/*******************************************************************************
**
** The source file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <iostream>
using namespace std;

#include "edit.h"
#include "project.h"
#include "gpupreview.h"

#include <glcore.h>

#include "render.h"
using namespace Mlt;
using namespace jahnle;

// Minor classes used here
#include "undo.h"
#include "track.h"
#include "item.h"

// Page widgets
#include "pageproject.h"
#include "pagecut.h"
#include "pagetrack.h"
#include "pagesettings.h"

// Additional widgets used
#include <qlayout.h>
#include <qprogressdialog.h>
#include <qfiledialog.h>
#include <jahformatter.h>
#include <valueFormatters.h>
#include <supergrangecontrol.h>
#include <qslider.h>
#include <timelineSlider.h>
#include <qtabwidget.h>
#include <qvbox.h>
#include <qhbox.h>
#include <qmessagebox.h>

// Jah Custom widgets
#include <widget.h>
#include <jahtranslate.h>

// For now, the sdl previewer is only available on linux
#ifdef JAHLINUX
#include "previewsdl.h"
#endif

// Constructor for the MLT edit module:
//
// This is responsible for initialising the MLT factory, preview component, project
// container and events, as well as the GUI components. Note that the project related
// components are transitory and may be reconstructed in their entirety during use.

GLEdit::GLEdit( GLCore *core, QHBox*, QHBox* parent1, const char *name, 
                QHBox *controls, QHBox *controller, QHBox *, QHBox *rightcontroller, int *gcn, const QGLWidget * ) :  
	GLWorld( core, name, controller, gcn ),
	m_core( core ),
	m_controls( controls ),
	m_rightcontroller( rightcontroller ),
	m_parent1( parent1 ),
	m_preview( NULL ),
	m_project( NULL ),
	m_tab_frame( NULL ),
	m_ignore_changes( false ),
	m_undo( NULL ),
	m_redo( NULL ),
	m_serialiser( NULL ),
	m_auto_save( NULL ),
	m_controlled( false ),
	m_tool_mode( tool_null )
{
	for ( int i = 0; i < 4; i ++ )
		m_page[ i ] = NULL;
}

// Destructor for the MLT module:
// Destroys everything MLT and UI related
GLEdit::~GLEdit() 
{
	// Ensure that the pages are removed before the project
	for ( int i = 0; i < 4; i ++ )
		delete m_page[ i ];

	// Delete all MLT objects
	delete m_preview;
	delete m_project;

	// Wipe undo/redo
	while ( m_undo && m_undo->count( ) )
		delete ( UndoState * )m_undo->pop_front( );
	while ( m_redo && m_redo->count( ) )
		delete ( UndoState * )m_redo->pop_front( );
	delete m_undo;
	delete m_redo;
	delete m_serialiser;
	delete m_auto_save;
}

static QString startUpDiagnostics( QString test_card )
{
	QString result = "";

	Filter colourspace( "avcolour_space" );
	if ( !colourspace.is_valid( ) )
		result += QString( "Colour space conversion missing - requires ffmpeg\n" );

	Producer image( ( char * )test_card.ascii( ) );
	if ( !image.is_valid( ) )
		result += QString( "Image support missing - requires GTK2 or QImage\n" );

	Consumer xml( "westley", "memory" );
	if ( !xml.is_valid( ) )
		result += "XML support missing - requires libxml2\n";

	return result;
}

void GLEdit::start( )
{
	// Construct the factory
	Factory::init( );

	// Will use an image during diagnostic checks
	QString test_card = JahBasePath + "Pixmaps/jahlayer.png";

	// Check diagnostics
	m_error_msg = startUpDiagnostics( test_card );

	if ( m_error_msg == "" )
	{
		// Initialise the video editing components
		initialiseSubsystem( m_core );

		// Build the GUI
    	buildInterface( m_controls );
		buildRightController( m_rightcontroller );

		// Create mediatable
		initializeMediaTable( m_parent1 );
	
		// Now load in all the effects
		initializeEffects( );
	}
}

// Initialise the core components of the MLT system 
void GLEdit::initialiseSubsystem( GLCore * )
{
	// For the hell of it, we'll use the jahshaka logo as the test card
	QString test_card = JahBasePath + "Pixmaps/jahlayer.png";
	
	// Deep copy of westley files
#ifdef WIN32
	Factory::setenv( "MLT_WESTLEY_DEEP", "true" );
#else
	setenv( "MLT_WESTLEY_DEEP", "true", 1 );
#endif

	// Construct our preview
#ifdef JAHLINUX
	if ( getenv( "JAH_USE_SDL" ) )
		m_preview = new PreviewSdl( this, test_card );
	else
		m_preview = new PreviewGpu( this, test_card );
#else
	m_preview = new PreviewGpu( this, test_card );
#endif

	// Create undo/redo
	m_undo = new Deque( );
	m_redo = new Deque( );

	// Our serialiser simply writes the doc to a propery called memory
	m_serialiser = new Consumer( "westley", "memory" );
	m_serialiser->set( "store", "jah_" );

	// Our auto save serialiser will write to the a temp file
	QString name = JahMediaPath + QString( "editor/autosave.jef" );
	m_auto_save = new Consumer( "westley", ( char * )( name.ascii( ) ) );
	m_auto_save->set( "store", "jah_" );

	// Create the auto load file reference
	QFile file( name );

	// We will use a file to indicate that we're attempting to load the file
	QString guard_name = JahMediaPath + QString( "editor/loading" );
	QFile guard( guard_name );

	// If both the autosave and guard file exist, something went wrong on the last attempt
	// to load the project - this might mean we repeatedly fail, so allow the user to 
	// override the autoload here
	if ( file.exists( ) && guard.exists( ) )
	{
		QString message = "The current autosave appears to have failed to\nreload on a previous attempt.\n\nDo you want to try again?";
		if ( QMessageBox::question( this, "Jahshaka Editor", message, tr( "&Yes" ), tr( "&No" ), QString::null, 0, 1 ) )
			file.remove( );
		guard.remove( );
	}

	// Should be ok to attempt to reload now
	if ( file.exists( ) )
	{
		// Create the guard
		if ( guard.open( IO_WriteOnly ) )
			guard.close( );

		// Need to convert to an asset (hope the hack holds...)
		assetData asset;
		asset.location = name;

		// Load as a project
		Project *project = new Project( this, asset, m_project );

		// Use it if valid
		if ( project->is_valid( ) )
			switchProject( project, 0 );
		else
			delete project;
	}

	// If we have no project at this point, we need to create one
	if ( m_project == NULL )
	{
		createProject( );
		m_project->set_speed( 0 );
		m_preview->show( m_project );
	}

	// Ensure the project matches the current profile
	m_preview->storeProfile( m_project );

	// Remove the guard
	// NB: This may not catch all autoload crashes, but it should catch most (hopefully)
	if ( guard.exists( ) )
		guard.remove( );
}

void GLEdit::createProject( )
{
	// Remove the existing project
	delete m_project;

	// Construct our project container
	m_project = new Project( this );

	// Define the first track
	m_project->addTrack( );

	// Connnect the two objects and set up events
	if ( m_preview && m_preview->is_valid( ) )
		m_preview->show( m_project );
	
	// Wipe the previous undo/redo
	while ( m_undo->count( ) )
		delete ( UndoState * )m_undo->pop_front( );
	while ( m_redo->count( ) )
		delete ( UndoState * )m_redo->pop_front( );

	// Update the UI if it's been created (won't be on an initial call here)
	if ( m_tab_frame != NULL )
	{
		m_animframe_lcd->setFormatter( TimecodeValueFormatter( m_preview->isPAL() ? 25 : 30 ) );
		for ( int i = 0; i < 4; i ++ )
			m_page[ i ]->restart( );
		updateUiSettings( );
	}

	// Connect the project to the undo/redo mechanism and seed it
	m_serialiser->connect( *m_project );
	m_auto_save->connect( *m_project );
	checkPoint( );
}

void GLEdit::switchProject( Project *pro, int position )
{
	// Stop if and restart if we need to
	bool running = m_preview != NULL && !m_preview->is_stopped( );
	if ( running )
		m_preview->stop( );

	// Connect to the undo/redo and auto save serialisers
	m_serialiser->connect( *pro );
	m_auto_save->connect( *pro );

	// Delete the previous project
	delete m_project;

	// Use this one instead
	m_project = pro;

	// Force it to the current settings
	ignoreChanges( true );
	m_project->refresh( );
	ignoreChanges( false );

	// Show this one
	if ( m_preview )
		m_preview->show( m_project );

	// Update the UI now
	updateUiSettings( );

	// Seek to the stored position
	seek( position );

	// Now auto save
	m_auto_save->start( );

	// Restart if we were running before
	if ( running )
		m_preview->start( );
}

// Save to desktop
// Currently this will create a new file on each save
void GLEdit::saveDesktop( )
{
    assetData thedata;
    QString temp;

	// Set up the asset 
    thedata.theCategory = VideoCategory::MEDIA;
    thedata.theType     = VideoType::MOVIE;
    thedata.clipname    = ModuleName + "Scene";
    thedata.clipnumber  = *clipnumber + 1;
    thedata.location    = scenepath;
    thedata.filename    = thedata.clipname + temp.setNum( thedata.clipnumber );
    thedata.extension   = ".jef";
    thedata.startframe  = 1;
    thedata.endframe    = length( );
	thedata.xsize       = 180;
	thedata.ysize       = m_preview->isPAL( ) ? 144 : 120;

	// Make sure the location actually exists
	QDir d( thedata.location );
	if ( !d.exists( ) )
		d.mkdir( thedata.location, true );

	// Store the preview specific stuff
	m_preview->storeProfile( m_project );

	// Save it via westley
	Consumer westley( "westley", ( char * )( thedata.getFullFileName( ).ascii( ) ) );
	westley.set( "store", "jah_" );
	westley.connect( *project( ) );
	westley.start( );

	// Send it to the desktop
    emit updateDesktop( thedata );

	// Increment the global clip count
	*clipnumber = thedata.clipnumber;
}

// New project
void GLEdit::newProject( )
{
	m_preview->stop( );
	createProject( );
	updateUiSettings( );
	m_preview->start( );
}

// Open project callback
void GLEdit::openProject( )
{
    QString file_name = QFileDialog::getOpenFileName( scenepath, "*.jef", this );

    if ( !file_name.isEmpty() )
	{
		// Need to convert to an asset (hope the hack holds...)
		assetData asset;
		asset.location = file_name;

		// Load as a project
		Project *project = new Project( this, asset, m_project );

		// Use it if valid
		if ( project->is_valid( ) )
			switchProject( project, 0 );
		else
			delete project;
	}
}

// Save project callback
void GLEdit::saveProject( )
{
    QString file_name = QFileDialog::getSaveFileName( scenepath, "*.jef", this );

    if ( !file_name.isEmpty() )
	{
        if ( !file_name.endsWith( ".jef" ) )
			file_name += ".jef";

		// Save it via westley
		Consumer westley( "westley", ( char * )( file_name.ascii( ) ) );
		westley.set( "store", "jah_" );
		westley.connect( *project( ) );
		westley.start( );
	}
}

// Build the GUI for this module
void GLEdit::buildInterface( QHBox *controls ) 
{
	// Hack for world supplied widgets
	m_animframe_lcd->blockSignals( true );
        JahFormatter::configure( m_animframe_lcd, 0, 0 );
	m_animframe_lcd->setFormatter( TimecodeValueFormatter( m_preview->isPAL() ? 25 : 30 ) );
	m_animframe_lcd->blockSignals( false );

	// Fixed sizes (for now)
	const int width = controls->width( );
	const int height = controls->height( );

	// Create the tab frame
	m_tab_frame = new QTabWidget( controls, "tab_frame" );
	m_tab_frame->setGeometry( QRect( 0, 0, width, height ) );
	m_tab_frame->setTabShape( QTabWidget::Triangular );
	m_tab_frame->setAutoMask( FALSE );
	connect( m_tab_frame, SIGNAL( currentChanged( QWidget * ) ), this, SLOT( pageChanged( QWidget * ) ) );

	// Add the tabs
	m_page[ 0 ] = new PageProject( m_tab_frame, this );
	m_tab_frame->insertTab( m_page[ 0 ], "PROJECT" );
	m_page[ 1 ] = new PageCut( m_tab_frame, this );
	m_tab_frame->insertTab( m_page[ 1 ], "CUT" );
	m_page[ 2 ] = new PageTrack( m_tab_frame, this );
	m_tab_frame->insertTab( m_page[ 2 ], "TRACK" );
	m_page[ 3 ] = new PageSettings( m_tab_frame, this );
	m_tab_frame->insertTab( m_page[ 3 ], "SETTINGS" );

	// Update the UI
	updateUiSettings( );
}

void GLEdit::setZoom( int value )
{
	m_preview->setZoom( value );
}

void GLEdit::updateZoomOut( )
{
}

void GLEdit::updateZoomIn( )
{
}

void GLEdit::buildController( QHBox* controller) 
{
    Astartframe=1;  Aendframe=100;  Aanimframe=1;

    QVBox* top = new QVBox( controller );

    // Container widget
    QWidget *transportContainer = new QWidget( top, "page" );
    QHBoxLayout *transportLayout = new QHBoxLayout( transportContainer );
    
    transportLayout->addStretch();
    
    // Transport buttons
    scrubfirst = new JahToolButton( transportContainer, "controllerrewindbutton" );
    scrubfirst->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubfirst, JahBasePath+"Pixmaps/player/rewind_off.png", JahBasePath+"Pixmaps/player/rewind_on.png" );
    connect( scrubfirst,  SIGNAL(clicked()), SLOT(firstframeanimation())  );
    transportLayout->addWidget( scrubfirst );
    
    scrubprevious = new JahToolButton( transportContainer, "controllerpreviousbutton" );
    scrubprevious->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubprevious, JahBasePath+"Pixmaps/player/frewind_off.png", JahBasePath+"Pixmaps/player/frewind_on.png" );
    connect( scrubprevious,  SIGNAL(clicked()), SLOT(previousframeanimation())  );
    transportLayout->addWidget( scrubprevious );
    
    scrubstop = new JahToolButton( transportContainer, "controllerstopbutton" );
    scrubstop->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubstop, JahBasePath+"Pixmaps/player/stop_off.png", JahBasePath+"Pixmaps/player/stop_on.png" );
    connect( scrubstop,  SIGNAL(clicked()), SLOT(stopanimation())  );
    transportLayout->addWidget( scrubstop );
    
    scrubplay = new JahToolButton( transportContainer, "play" );
    scrubplay->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubplay, JahBasePath+"Pixmaps/player/play_off.png", JahBasePath+"Pixmaps/player/play_on.png" );
    connect( scrubplay,  SIGNAL(clicked()), SLOT(startanimation())  );
    transportLayout->addWidget( scrubplay );
    
    scrubnext = new JahToolButton( transportContainer, "controllernextbutton" );
    scrubnext->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubnext, JahBasePath+"Pixmaps/player/ffoward_off.png", JahBasePath+"Pixmaps/player/ffoward_on.png" );
    connect( scrubnext,  SIGNAL(clicked()), SLOT(nextframeanimation())  );
    transportLayout->addWidget( scrubnext );
    
    scrublast = new JahToolButton( transportContainer, "controllerffworardbutton" );
    scrublast->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrublast, JahBasePath+"Pixmaps/player/foward_off.png",  JahBasePath+"Pixmaps/player/foward_on.png" );
    connect( scrublast,  SIGNAL(clicked()), SLOT(lastframeanimation())  );
    transportLayout->addWidget( scrublast );

    transportLayout->addSpacing( 10 );

    // Time code display
    m_animframe_lcd = new SupergRangeControl( transportContainer, "timecodedisplay" );
    m_animframe_lcd->setFormatter( TimecodeValueFormatter() );
    JahFormatter::configure( m_animframe_lcd, 1, 1 );
    connect( m_animframe_lcd,   SIGNAL(valueChanged(int)),	 SLOT(sliderselected()) );
    connect(m_animframe_lcd, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged( int )) );
    transportLayout->addWidget( m_animframe_lcd );
    transportLayout->addStretch();

    // Container widget
    QWidget* sliderContainer = new QWidget( top );
    QHBoxLayout* sliderLayout = new QHBoxLayout( sliderContainer );

    // Slider
    controllerslider = new TimelineSlider( sliderContainer, "controllerslider" );
    controllerslider->setMinValue( Astartframe );
    controllerslider->setMaxValue( Aendframe );
    controllerslider->setValue   ( Aanimframe );
    sliderLayout->addWidget( controllerslider );

    connect( controllerslider, SIGNAL( sliderMoved(int) ),  SLOT(updateAnimation(int)) );
    connect( controllerslider, SIGNAL( valueChanged(int) ), SLOT(sliderValueChanged(int)) );
    connect( controllerslider, SIGNAL( valueChanged(int) ), m_animframe_lcd, SLOT(setValue(int)) );
    connect( m_animframe_lcd,  SIGNAL( valueChanged(int) ), controllerslider, SLOT(setValue(int)) );

    GetButton = new QPushButton( transportContainer, "GetButton" );
    GetButton->setFixedSize( 120, 20 );
    GetButton->setText( jt->tr( "&Get Desktop"  ) );
    connect( GetButton,  SIGNAL(clicked()), SIGNAL( GrabDesktop() ) );
    transportLayout->addWidget( GetButton );
}


void GLEdit::buildRightController( QHBox *controls ) 
{
    // Container widget
    QWidget *container = new QWidget( controls, "container" );
    QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");
    
    container_layout->addStretch( 0 );
    
    QString image = JahBasePath+"Pixmaps/desktop/zoomin.png";
    JahToolButton *toolzoomout = new JahToolButton( container, "zoomout" );
    JahFormatter::addJahPlayerButton( toolzoomout, image, image );
    connect( toolzoomout, SIGNAL(clicked()), this, SLOT(updateZoomOut() ) );
    toolzoomout->setAutoRepeat( true );
    container_layout->addWidget( toolzoomout );
    
    container_layout->addSpacing( 3 );
    
    QSlider *zoom = new QSlider( container, "zoomslider" );
    zoom->setOrientation( QSlider::Vertical );
    zoom->setMinValue( 0 );    
    zoom->setMaxValue( 3900 );    
    zoom->setValue( 3000 );
    zoom->setPageStep( 1 );
    container_layout->addWidget( zoom, 0, Qt::AlignHCenter );
    
    connect( zoom, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int) ) );
    
    container_layout->addSpacing( 3 );
    
    JahToolButton *toolzoomin = new JahToolButton( container, "zoomin" );
    image = JahBasePath+"Pixmaps/desktop/zoomout.png";
    JahFormatter::addJahPlayerButton( toolzoomin, image, image );
    connect( toolzoomin, SIGNAL( pressed( ) ), this, SLOT( updateZoomIn( ) ) );
    toolzoomin->setAutoRepeat( true );
    container_layout->addWidget( toolzoomin );
    
    container_layout->addSpacing( 10 );
    
    JahToolButton *null = new JahToolButton( container, "null" );
    container_layout->addWidget( null );
    JahFormatter::addJahPlayerButton( null, JahBasePath + "Pixmaps/desktop/simplezoomout.png", JahBasePath + "Pixmaps/desktop/simplezoomout.png" );
    
    JahToolButton *translate = new JahToolButton( container, "translate" );
    container_layout->addWidget( translate );
    JahFormatter::addJahPlayerButton( translate, JahBasePath + "Pixmaps/desktop/transtool.png", JahBasePath + "Pixmaps/desktop/transtool.png" );

    JahToolButton *scale = new JahToolButton( container, "scale" );
    JahFormatter::addJahPlayerButton( scale, JahBasePath + "Pixmaps/desktop/scaletool.png", JahBasePath + "Pixmaps/desktop/scaletool.png" );
    container_layout->addWidget( scale );
    
    JahToolButton* toolzoomlock = new JahToolButton( container, "zoomlock" );
    JahFormatter::addJahPlayerButton( toolzoomlock, JahBasePath+"Pixmaps/player/reflect.png", JahBasePath+"Pixmaps/player/reflect.png" );
    container_layout->addWidget( toolzoomlock );
    
    container_layout->addSpacing( 10 );
    
    // Render functionality
    JahToolButton *photo = new JahToolButton( container, "photo" );
    JahFormatter::addJahPlayerButton( photo, JahBasePath + "Pixmaps/desktop/phototool.png", JahBasePath + "Pixmaps/desktop/phototool.png" );
    container_layout->addWidget( photo );
    
    JahToolButton *render = new JahToolButton( container, "render" );
    JahFormatter::addJahPlayerButton( render, JahBasePath + "Pixmaps/desktop/rendertool.png", JahBasePath + "Pixmaps/desktop/rendertool.png" );
    container_layout->addWidget( render );
    
    container_layout->addStretch( );
    container_layout->addSpacing( 10 );
    
    connect( null, SIGNAL( clicked( ) ), this, SLOT( toolNull( ) ) );
    connect( translate, SIGNAL( clicked( ) ), this, SLOT( toolTranslate( ) ) );
    connect( scale, SIGNAL( clicked( ) ), this, SLOT( toolScale( ) ) );
    connect( photo, SIGNAL( clicked( ) ), this, SLOT( Render( ) ) );
    connect( render, SIGNAL( clicked( ) ), this, SLOT( RenderScene( ) ) );
    connect( toolzoomlock, SIGNAL( clicked() ), this, SLOT( toggleZoomLock() ) );
}

void GLEdit::toolNull( )
{
	setToolMode( tool_null );
}

void GLEdit::toolTranslate( )
{
	setToolMode( tool_translate );
}

void GLEdit::toolScale( )
{
	setToolMode( tool_scale );
}

void GLEdit::toggleZoomLock()
{
    m_preview->setAutoZoom( !m_preview->getAutoZoom() );
}

nle_tool_mode GLEdit::getToolMode( )
{
	return m_tool_mode;
}

void GLEdit::setToolMode( nle_tool_mode mode )
{
	m_tool_mode = mode;
}

void GLEdit::pageChanged( QWidget *page )
{
	( ( Page * )page )->refresh( );
	( ( Page * )page )->setPosition( position( ) );
}

// General invitation to update widgets
void GLEdit::updateUiSettings( )
{
	// Sanity check
	if ( m_tab_frame == NULL ) return;

	// Update page widgets (note 0th page hack)
	int i = m_tab_frame->currentPageIndex( );
	if ( i != 0 ) m_page[ 0 ]->refresh( );
	m_page[ i ]->refresh( );
}

// Position has changed
void GLEdit::updateSlider( int value )
{
	// Main jahshaka slider
	controllerslider->blockSignals( true );
	controllerslider->setMinValue( 0 );
	controllerslider->setMaxValue( length( ) - 1 );
	controllerslider->setValue( value );
	// This is a flaw - the active item is tracked on the 0th page - should be independent
	if ( m_project != NULL && m_project->getMultiTrack( ) != NULL )
	{
		NleMultiTrackTrack *track = m_project->getMultiTrack( );
		NleMultiTrackItem *item = ( NleMultiTrackItem * )track->firstItem( );
		MultiTrack *multitrack = track->multiTrack( );
		while ( item != NULL && ( value < item->clipStart( ) || value >= item->clipEnd( ) ) )
			item = ( NleMultiTrackItem * )item->nextItem( );
		if ( item != NULL && !multitrack->isSelected( item ) )
			updateUiSettings( );
	}
	else if ( m_project != NULL )
	{
		updateUiSettings( );
	}
	controllerslider->blockSignals( false );

	// The lcd display associated to main jahshaka slider
	m_animframe_lcd->blockSignals( true );
	m_animframe_lcd->setValue( value );
	m_animframe_lcd->blockSignals( false );

	// Update current page widget (note 0th page hack)
	if ( m_tab_frame )
	{
		int i = m_tab_frame->currentPageIndex( );
		if ( i != 0 ) m_page[ 0 ]->setPosition( value );	
		m_page[ i ]->setPosition( value );
	}
}

// Main transport slider slot
void GLEdit::sliderValueChanged( int value ) 
{
	if ( m_project )
	{
		value = rangeCheck( value, 0, length( ) - 1 ); 
		seek( value ); 
	}
}

// Length of the project
int GLEdit::length( )
{
	return m_project->get_length( );
}

// Set speed
void GLEdit::setSpeed( int speed )
{
	if ( m_project && m_project->get_speed( ) != speed )
	{
		int pos = controllerslider->value( ) + ( int )m_project->get_speed( );
		m_project->set_speed( speed );
		seek( pos );
		m_preview->start( );
	}
}

// Seek to a position
void GLEdit::seek( int position )
{
	if ( m_tab_frame != NULL ) 
	{
		updateSlider( position );
		m_project->seek( position );
		m_preview->refresh( );
	}
}

// Get the position in the current m_project
int GLEdit::position( )
{
	return controllerslider->value( );
}

// Keyboard bindings
void GLEdit::keyPressEvent( QKeyEvent *k )
{
	sliderselected( );

	// We will ignore all key presses by default
	k->ignore( );

	// Offer it to the current page first - all pages should ignore
	// the event if they aren't relevant
	int i = m_tab_frame->currentPageIndex( );
	m_page[ i ]->processKeyEvent( k );

	// Odd OS/X behaviour - control key state isn't correctly received?
#ifndef JAHDARWIN
	int control_button = Qt::ControlButton;
#else
	int control_button = 2048;
#endif

	if ( k->isAccepted( ) )
	{
		// Do nothing
	}
	else if ( k->state( ) == 0 )
	{
		k->accept( );
		switch ( toupper( k->key( ) ) )
		{
			case 'J': setSpeed( -1 ); break;
			case 'K': setSpeed( 0 ); break;
			case 'L': setSpeed( 1 ); break;
			case Key_Home: seek( 0 ); break;
			case Key_End: seek( length( ) - 1 ); break;
			case Key_PageDown: m_project->moveNext( ); break;
			case Key_PageUp: m_project->movePrev( ); break;
			case Key_Left: previousframeanimation( ); break;
			case Key_Right: nextframeanimation( ); break;
			case Key_Escape: setPage( 0 ); break;
			case ' ': setSpeed( !m_project->get_speed( ) ); break; 
			default: k->ignore( ); break;
		}
	}
	else if ( k->state( ) == ( control_button | AltButton ) )
	{
		k->accept( );
		switch ( toupper( k->key( ) ) )
		{
			case 'P': m_project->toggleDebug( ); break;
			default: k->ignore( ); break;
		}
	}
	else if ( k->state( ) == control_button )
	{
		k->accept( );
		switch ( toupper( k->key( ) ) )
		{
			case 'J': setSpeed( m_preview->isPAL( ) ? -25 : -30 ); break;
			case 'K': setSpeed( 1 ); break;
			case 'L': setSpeed( m_preview->isPAL( ) ? 25 : 30 ); break;
			case 'R': if ( !k->isAutoRepeat( ) ) redo( ); break;
			case 'Z': if ( !k->isAutoRepeat( ) ) undo( ); break;
			case 'S': m_preview->save( ); break;
			case Key_Left: seek( position( ) - ( m_preview->isPAL( ) ? 25 : 30 ) ); break;
			case Key_Right: seek( position( ) + ( m_preview->isPAL( ) ? 25 : 30 ) ); break;
			default: k->ignore( ); break;
		}
	}
	else if ( k->state( ) == AltButton )
	{
		k->accept( );
		switch ( toupper( k->key( ) ) )
		{
			case 'J': setSpeed( ( m_preview->isPAL( ) ? 250 : 300 ) ); break;
			case 'K': setSpeed( 1 ); break;
			case 'L': setSpeed( ( m_preview->isPAL( ) ? 250 : 300 ) ); break;
			case 'Z': m_preview->setAutoZoom( !m_preview->getAutoZoom( ) );
			case Key_Left: seek( position( ) - ( m_preview->isPAL( ) ? 250 : 300 ) ); break;
			case Key_Right: seek( position( ) + ( m_preview->isPAL( ) ? 250 : 300 ) ); break;
			case Key_Up: m_project->setActiveTrack( m_project->getActiveTrack( ) - 1 ); updateUiSettings( ); break;
			case Key_Down: m_project->setActiveTrack( m_project->getActiveTrack( ) + 1 ); updateUiSettings( ); break;
			case Key_PageDown: m_tab_frame->setCurrentPage( m_tab_frame->currentPageIndex( ) + 1 ); break;
			case Key_PageUp: m_tab_frame->setCurrentPage( m_tab_frame->currentPageIndex( ) - 1 ); break;
			default: k->ignore( ); break;
		}
	}
}

void GLEdit::setPage( int i )
{
	if ( i >= 0 && i < 4 )
		m_tab_frame->setCurrentPage( i );
}

void GLEdit::keyReleaseEvent( QKeyEvent * )
{
	updateAnimation( );
}

// This method is invoked when a refresh is needed
// Note that we have two rendering mechanisms here - we can use opengl or the
// SDL accelerated YUV422 displays

void GLEdit::paintGL( )
{
	static bool error_shown = false;

	if ( m_preview != NULL && m_preview->is_valid( ) )
	{
		// Make sure that we are running
		if ( m_preview->is_stopped( ) )
			m_preview->start( );

		m_preview->paint( );
	}
	else if ( !error_shown )
	{
		// Make sure we only show this once
		error_shown = true;

		// These errors should never occur - but if they do, they should be trapped on the first use
		QMessageBox::critical( 0, "jahshaka", QString( "Failed to initialise the editor module:\n") + m_error_msg );
	}
}

// Interact with the desktop
void GLEdit::addClipEdit( assetData asset ) 
{
	if ( m_project == 0 ) return;

	bool is_project = asset.extension == ".jef";
	int first_track = m_project->hasBackground( ) ? 1 : 0;

	// Special case: check if the user is reloading a project when empty
	if ( is_project && length( ) == 0 && m_project->count( ) == first_track + 1 )
	{
		// Load as a project
		Project *project = new Project( this, asset, m_project );

		// Use it if valid
		if ( project->is_valid( ) )
			switchProject( project, 0 );
		else
			delete project;
	}
	else
	{
		// Drop on page if it wants it, or pass to project
		if ( !m_page[ m_tab_frame->currentPageIndex( ) ]->dropAsset( asset ) )
			m_project->addAsset( asset );
	}

	// Set the focus back to the current page
	m_page[ m_tab_frame->currentPageIndex( ) ]->setFocus( );
}

QImage GLEdit::Photo( assetData asset )
{
	// Stop & block
	m_preview->stop( );
	m_project->block( );

	// Attach and configure a colourspace conversion filter
	Filter filter( "avcolour_space" );
	filter.set( "forced", mlt_image_yuv422 );
	m_project->attach( filter );

	// Get the frame
	Frame *f = m_project->get_frame( );

	// Specify the requested image, width and height
	mlt_image_format format = mlt_image_rgb24a;
	int w = ( int )( m_preview->getHeight( ) * m_preview->getDisplayRatio( ) + 0.5 );
	int h = m_preview->getHeight( );

	// Grab the relevant settings from the preview
	f->set( "consumer_deinterlace", m_preview->isProgressive( ) );
	f->set( "rescale", m_preview->getRescale( ) );
	f->set( "deinterlace_method", m_preview->getDeinterlaceMethod( ) );
	f->set( "consumer_aspect_ratio", 1 );

	// Get the image
	uint8_t *image = f->get_image( format, w, h );

	// Convert to a QImage
	QImage temp( image, w, h, 32, NULL, 0, QImage::IgnoreEndian );
	temp.setAlphaBuffer( true );
	QImage copy = temp.copy( );
	delete f;

	// Detach
	m_project->detach( filter );

	// Unblock and start
	m_project->unblock( );
	m_preview->start( );

	// Save the image as the requested asset
	QString writefile = asset.location + asset.filename + asset.extension;
	copy.save( writefile, renderformat, 100 );

	return copy;
}

void GLEdit::RenderScene( void )
{
	// Pause and check point
	setSpeed( 0 );
	checkPoint( );

	// Construct the render object
	jahnle::Render render;

	// Use the interactive profile selection
	bool pal = m_preview->isPAL( );
	double ar = m_project->get_double( "jah_consumer_aspect_ratio" );

	// Get the user to select the profile
	if ( render.fetchProfile( JahBasePath, pal, ar ) )
	{
		// Copy
		Producer *copy = new Project( this, m_serialiser->get( "memory" ), m_project );
		copy->seek( 0 );
		copy->set_speed( 1 );

		// Create the progress dialog
		QProgressDialog progress( QString( "Rendering " ) + render.getFileName( ), "Abort", copy->get_length( ) );

		// Iterate through each frame and push it to the consumer
		for ( int i = 0; i < copy->get_length( ); i ++ )
		{
			// Set the progress and process any pending events
    		progress.setProgress( i );
    		qApp->processEvents( );
    		if ( progress.wasCanceled() )
        		break;
	
			// Fetch, push and delete
			Frame *f = copy->get_frame( );
			render.push( f );
			delete f;
	
			// This isn't nice - try to force the preview to update
			qApp->unlock( false );
			qApp->lock( );
		}
	
		// Clean up
		delete copy;
	}
}

// Undo/Redo handling - The editor uses a stack for undos and a queue for redos.
// Each entry contains the serialised form of the project in XML.

void GLEdit::checkPoint( )
{
	// Allow modifications here
	m_project->block( );

	// Allow multiple changes in a single operation
	if ( !m_ignore_changes )
	{
		// Retrieve the undo
		UndoState *last = ( UndoState * )m_undo->peek_back( );

		// Serialise
		m_serialiser->start( );

		// Fetch the generated XML doc
		char *doc = m_serialiser->get( "memory" );

		// If this is the first item, or the docs differ, store it
		if ( last == NULL || ( doc != NULL && strcmp( last->getDoc( ).utf8( ), doc ) ) )
		{
			// Create a state object
			UndoState *state = new UndoState( doc, position( ) );

			// Dupe and push
			m_undo->push_back( state );

			// Wipe redo
			while ( m_redo->count( ) )
				delete ( UndoState * )m_redo->pop_front( );

			// Autosave and Schedule a refresh
			if ( m_preview )
				m_preview->refresh( );
			m_auto_save->start( );
		}
	}

	// Unblock the project now (but don't throw an event)
	m_project->unblock( false );
}

void GLEdit::undo( )
{
	// Make sure we have something to undo to
	if ( m_undo->count( ) == 0 )
	{
		m_ignore_changes = false;
		checkPoint( );
	}

	// We should always have something to undo to
	if ( m_undo->count( ) > 1 )
	{
		// Pop the last document (which matches the current state)
		UndoState *doc = ( UndoState * )m_undo->pop_back( );

		// Ensure that we go back to the current position on a redo
		doc->setPosition( position( ) );

		// Push this on to the redo stack
		m_redo->push_front( doc );

		// Take a look at the doc before that
		doc = ( UndoState * )m_undo->peek_back( );

		// Recreate the project
		Project *project = new Project( this, doc->getDoc( ).utf8( ), m_project );

		// Switch projects if successful
		if ( project->is_valid( ) )
			switchProject( project, doc->getPosition( ) );
		else
			delete project;
	}
}

void GLEdit::redo( )
{
	// Only redo when we have a redo to do :-)
	if ( m_redo->count( ) > 0 )
	{
		// Fetch the first redo item
		UndoState *doc = ( UndoState * )m_redo->pop_front( );

		// Push it back to the undo stack
		m_undo->push_back( doc );

		// Convert it to a project
		Project *project = new Project( this, doc->getDoc( ).utf8( ), m_project );

		// Switch projects if successful
		if ( project->is_valid( ) )
			switchProject( project, doc->getPosition( ) );
		else
			delete project;
	}
}

// Mouse handling - mostly this comes down to determing the position of the cursor
// and informing the active page (if it's interested) of the state and the position

void GLEdit::getMouseCoords( QMouseEvent *e, double &x, double &y )
{
	double rx, ry, rw, rh;
	m_preview->getRegion( rx, ry, rw, rh );
	x = e->x( ) - rx;
	x = x < 0 ? 0 : x;
	x = x > rw ? rw : x;
	y = e->y( ) - ry;
	y = y < 0 ? 0 : y;
	y = y > rh ? rh : y;
	x = ( x * 100.0 ) / rw;
	y = ( y * 100.0 ) / rh;
}

void GLEdit::mousePressEvent( QMouseEvent *e )
{
	int i = m_tab_frame->currentPageIndex( );
	m_page[ i ]->setFocus( );
	if ( m_page[ i ]->acceptsSelection( ) )
	{
		getMouseCoords( e, m_start_x, m_start_y );
		m_page[ i ]->startSelection( m_start_x, m_start_y );
	}
}

void GLEdit::mouseMoveEvent( QMouseEvent  *e )
{
	int i = m_tab_frame->currentPageIndex( );
	if ( m_page[ i ]->acceptsSelection( ) )
	{
		double x, y;
		getMouseCoords( e, x, y );
		m_page[ i ]->moveSelection( x - m_start_x, y - m_start_y );
	}
}

void GLEdit::mouseReleaseEvent( QMouseEvent *e )
{
	int i = m_tab_frame->currentPageIndex( );
	if ( m_page[ i ]->acceptsSelection( ) )
	{
		double x, y;
		getMouseCoords( e, x, y );
		m_page[ i ]->setSelection( x - m_start_x, y - m_start_y );
	}
}

// Draw a border on the image to show the extents of the compositing positions
static void show_border( uint8_t *image, mlt_image_format format, int w, int h )
{
	if ( format == mlt_image_rgb24 )
	{
		uint8_t *p = image;
		int i;

		i = w;
		while( i -- )
		{
			*p ++ = 0xff; *p ++ = 0x00; *p ++ = 0x00;
		}

		i = h - 2;
		while( i -- )
		{
			*p ++ = 0xff; *p ++ = 0x00; *p ++ = 0x00;
			p += w * 3 - 6;
			*p ++ = 0xff; *p ++ = 0x00; *p ++ = 0x00;
		}

		i = w;
		while( i -- )
		{
			*p ++ = 0xff; *p ++ = 0x00; *p ++ = 0x00;
		}
	}
	else if ( format == mlt_image_opengl )
	{
		uint32_t *p = ( uint32_t * )image;
		int i;

		i = w;
		while( i -- )
			*p ++ = 0xff0000ff;

		i = h - 2;
		while( i -- )
		{
			*p ++ = 0xff0000ff;
			p += w - 2;
			*p ++ = 0xff0000ff;
		}

		i = w;
		while( i -- )
			*p ++ = 0xff0000ff;
	}
	else if ( format == mlt_image_yuv422 )
	{
		uint8_t *p = image;
		int i;
		int y, u, v;
		RGB2YUV( 0xff, 0x00, 0x00, y, u, v );

		i = w / 2;
		while( i -- )
		{
			*p ++ = y; *p ++ = u; *p ++ = y; *p ++ = v;
		}

		i = h - 2;
		while( i -- )
		{
			*p ++ = y; *p ++ = u; *p ++ = y; *p ++ = v;
			p += w * 2 - 8;
			*p ++ = y; *p ++ = u; *p ++ = y; *p ++ = v;
		}

		i = w / 2;
		while( i -- )
		{
			*p ++ = y; *p ++ = u; *p ++ = y; *p ++ = v;
		}
	}
}

// This method is invoked when a frame should be shown - this can be done
// by the MLT preview module, or it can be handled by the application depending
// on the state of the consumer's 'preview_off' flag
// NB: It's invoked from the consumers thread and the GUI is locked during its
// execution

void GLEdit::consumerFrameShow( Frame &f )
{
	// Make sure that the frame has been rendered
	int rendered = f.get_int( "rendered" );

	// Fetch the display aspect ratio and place it on the frame
	double ar = m_preview->getDisplayRatio( );
	f.set( "display_ratio", ar );

	if ( rendered )
	{
		// Send it to the current page in case it's interested
		int i = m_tab_frame->currentPageIndex( );
		m_page[ i ]->consumerFrameShow( f );

		// Get the width and height and format of the image.
		int w = f.get_int( "width" );
		int h = f.get_int( "height" );
		mlt_image_format format = ( mlt_image_format )f.get_int( "format" );
		uint8_t *image = f.get_image( format, w, h );

		// Show the border when paused
		if ( f.get_int( "_speed" ) == 0 )
			show_border( image, format, w, h );
	}
}

void GLEdit::headRender( int width, int height ) 
{ 
    glViewport( 0, 0, width, height );
 
	m_preview->setHeadRender( true );
	m_preview->setDimensions( width, height );
    m_preview->paint( ); 
	m_preview->setHeadRender( false );
}

void GLEdit::glworldUpdate( )
{
	m_core->updateGL( );
	emit showInHead( this );
	if ( m_page[ 0 ] != 0 )
	{
		int i = m_tab_frame->currentPageIndex( );
		m_page[ i ]->setFocus( );
	}
}
