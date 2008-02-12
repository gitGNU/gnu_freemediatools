/*******************************************************************************
**
** The source file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "MainPlayer.h"
#include "qtplayer.h"
#include <qlayout.h>
#include <qpopupmenu.h>
#include <qlineedit.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qmenubar.h>
#include <qframe.h>
#include <qlabel.h>
#include <qmultilineedit.h>
#include <qslider.h>

#include <desktop.h>

//used for videodata support
#include "openmedialib.h"

//custom ui objects
#include "widget.h"
#include "InputLCD.h"

//for the tracer
#include "jahtracer.h"

//for preferences global
#include "jahpreferences.h"
#include "jahformatter.h"
#include "valueFormatters.h"
#include "jahtranslate.h"

#include "supergrangecontrol.h"
#include "timelineSlider.h"


#include "collapsable.h"

//for the plotter to be waveform / vectorscope
#include "levelswidget.h"

class JahHeadable;

//==============================================================================
MainPlayer::MainPlayer( const QGLWidget *core, QWidget *parent, const char* name, 
                        QHBox * parent1,
                        QHBox* control, QHBox* controller,
                        QHBox* leftcontroller, QHBox* rightcontroller) 
	: QWidget (parent, name)
	, m_core( core )
	, m_parent( parent )
	, m_parent1( parent1 )
	, m_control( control )
	, m_controller( controller )
	, m_leftcontroller( leftcontroller )
	, m_rightcontroller( rightcontroller )
	, m_started( false )
	, m_mode( SHOW_ALL )
	, ModuleName( name )
	, jahplayer( 0 )
{
    minFrame = 0;
    maxFrame = 0;
}

//--------------------------------------------------------------------
MainPlayer::~MainPlayer()
{
}

void MainPlayer::activate( )
{
	// Ensure this method is only called once
	if ( m_started ) return;
	m_started = true;

	// Get the preferences and restore them
    loadPreferences( );

	// Set up the translator
    jt = JahTranslate::getInstance();

    // Initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath  = jprefs.getBasePath().data();

    // Set up main interface
    buildInterfaceMenus(m_leftcontroller,m_rightcontroller);
    buildController(m_controller);

    // Set up the color scope widget
    colorscope  = new LevelsWidget(m_parent1);
  
    // This is where we initialize the player
    QHBoxLayout *mainworldLayout = new QHBoxLayout( m_parent);

    jahplayer = new QtPlayer( m_core, "test:", 0, m_parent );
    mainworldLayout->addWidget( jahplayer );
	switchModeFps( scrubrender->currentItem( ) );

#ifndef JAHPLAYER
    //ok we need to set this one up now and we are cool mon!
	buildInterface( m_control );
#endif

    // connect the slots
    connect( colorscope, SIGNAL( realTimeChanged( bool ) ), this, SLOT( realTimeChanged( bool ) ) );
    connect( jahplayer, SIGNAL( showImage( int, QImage * ) ), this, SLOT( showImage( int, QImage * ) ) );

    //connect player drag n drop to player load signal
    connect(jahplayer, SIGNAL( draggedAImage( ) ), this, SLOT( draggedAImage( ) ) );
    connect(jahplayer, SIGNAL( requestNextItem( ) ), this, SLOT( requestNextItem( ) ) );
    connect(jahplayer, SIGNAL( showInHead( JahHeadable * ) ), this, SLOT( showHead( JahHeadable * ) ) );

#ifdef JAHPLAYER
    //used for the standalone JahPlayer
    connect( toolthumbview,   SIGNAL(clicked()), this, SLOT(toggleDesktopImages() )  );
    connect( toolbinview,     SIGNAL(clicked()), this, SLOT(toggleDesktopTable() )  );
#endif
}

void MainPlayer::showHead( JahHeadable *thing )
{
	emit showInHead( thing );
	setFocus( );
}

void MainPlayer::realTimeChanged( bool )
{
	jahplayer->refresh( );
}

inline void showAlpha( QImage &image )
{
	int w = image.width( );
	int h = image.height( );
	int i = 0;
	unsigned char *data = image.bits( );

	glColorMask( true, true, true, true );

	if ( image.depth( ) == 32 )
	{
		while( h -- )
		{
			i = w;
			while( i -- )
			{
				*( data ) = *( data + 3 );
				*( data + 1 ) = *( data + 3 );
				*( data + 2 ) = *( data + 3 );
				*( data + 3 ) = 255;
				data += 4;
			}
		}
	}
}

void MainPlayer::showImage( int position, QImage *image )
{
	SetFrameNumber( position );
	if ( image != 0 )
		updateScopes( *image, jahplayer->speed( ) == 1 );
	if ( m_mode == SHOW_ALPHA && image != 0 )
		showAlpha( *image );
	else if ( m_mode != SHOW_ALL && image != 0 )
		glColorMask( m_mode == SHOW_RED, m_mode == SHOW_GREEN, m_mode == SHOW_BLUE, true );
	else
		glColorMask( true, true, true, true );
}

//--------------------------------------------------------------------
void	MainPlayer::SetFrameNumber(int i)
{
	int frame = i;

	if (frame > (int)maxFrame ) 
    {
        frame = minFrame;
        jahplayer->seek(frame);
    }

	if (frame < (int)minFrame) 
    {
        frame = minFrame;
        jahplayer->seek(frame);
    }

	animframe->blockSignals( true );
	animframe->setValue( frame );
	animframe->blockSignals( false );
	controllerslider->blockSignals( true );
	controllerslider->setValue( frame );
	controllerslider->blockSignals( false );
}

void MainPlayer::updateScopes( QImage &image, bool check)
{
    //if check = true  then update if Rt is on
    //if check = false then update if RT is off
    bool rtstatus = colorscope->realTimeCheckBox->isOn( );

    if (check && rtstatus)
        colorscope->setTheImage( &image );
	else if (!check && !rtstatus)
		colorscope->setTheImage( &image );
}

void MainPlayer::sliderValueChanged(int fr)
{
	if ( jahplayer->speed( ) == 0 )
		jahplayer->seek( fr );
}

void MainPlayer::toggleRamPlayer( )
{
	LoadMySequence( theassetData );
}

void MainPlayer::LoadMySequence(assetData desktopclip)
{
    //the local instance of the asset
	QString filename = desktopclip.location+desktopclip.filename+desktopclip.extension;
	int speed = 1;

	emit syncMediaTable( desktopclip );

    switch (desktopclip.theCategory)
    {
	    case VideoCategory::IMAGE:
			filename += QString( "/sequence:" );
			if ( jahplayer->get_eof( ) == QtPlayer::next )
				filename += QString( "fps=1,%1" ).arg( frameStep );
			else
				speed = 0;
			break;

		case VideoCategory::MEDIA:
		case VideoCategory::CLIP:
            if ( playmodecombo->currentItem( ) == 1 && desktopclip.theType != VideoType::MOVIE )
				filename = desktopclip.location + desktopclip.filename + QString( "*" ) + desktopclip.extension + QString( "/sequence:cache=500" );
			else if ( desktopclip.theType != VideoType::MOVIE )
				filename = desktopclip.location + desktopclip.filename + QString( "*" ) + desktopclip.extension + QString( "/sequence:" );
			break;

		case VideoCategory::AUDIO:
		default:
			break;
    }

	if ( filename != current_file_ )
	{
    	theassetData = desktopclip;

		filename.replace( "//", "/" );
		jahplayer->play( filename, speed );
		current_file_ = filename;

		minFrame = 0;
		maxFrame = jahplayer->length( ) - 1;

    	controllerslider->setRange( 0, jahplayer->length( ) - 1 );
        animframe->setRange( minFrame, maxFrame );

    	//this sets up the interface values
    	LoadSequenceCompleted( );

    	//updating asset display data
    	UpdateAssetData(desktopclip);
	}
}

//--------------------------------------------------------------------
void	MainPlayer::LoadSequenceCompleted( )
{
    startFrame = theassetData.startframe;
    minFrame = 0;
    maxFrame = jahplayer->length( ) - 1;

#ifndef JAHPLAYER
//     minFrameInput->setValue (minFrame);
//     minFrameInput->setMinMaxInt(minFrame, maxFrame);
//     maxFrameInput->setMinMaxInt(minFrame, maxFrame);
//     maxFrameInput->setValue (maxFrame);
#endif
    
    controllerslider->setValue( 0 );
}

//--------------------------------------------------------------------
void	MainPlayer::SetMinFrame(int value)
{
    if ( value <= int( maxFrame ) )
    {
    	minFrame = value;
    	controllerslider->setRange(minFrame, maxFrame);
    	animframe->setMin( minFrame );
    }
    else
    {
//     	minFrameInput->setValue( minFrame );
    }
}

void	MainPlayer::SetMaxFrame(int value)
{
    if ( value >= int( minFrame ) )
    {
    	maxFrame = value;
    	controllerslider->setRange(minFrame, maxFrame);
    	animframe->setMax( maxFrame );
    }
    else
    {
    	// maxFrameInput->setValue( maxFrame );
    }
}

//--------------------------------------------------------------------
void	MainPlayer::LoopForward()
{
	jahplayer->set_speed( 1 );
}

//--------------------------------------------------------------------
void	MainPlayer::Stop()
{
	if ( jahplayer ) 
		jahplayer->set_speed( 0 );
}

void	MainPlayer::FastForward()
{
	if ( jahplayer ) 
		jahplayer->set_speed( frameStep * 5 );
}

void	MainPlayer::FrameForward()
{
	if ( jahplayer )
	{
		jahplayer->set_speed( 0 );
		jahplayer->seek( jahplayer->position( ) + frameStep );
	}
}

void	MainPlayer::FrameBackward()
{
	if ( jahplayer )
	{
		jahplayer->set_speed( 0 );
		jahplayer->seek( jahplayer->position( ) - frameStep );
	}
}

void	MainPlayer::Rewind()
{
	if ( jahplayer ) 
		jahplayer->set_speed( - frameStep * 5 );
}

void MainPlayer::sliderPressed ()
{
	jahplayer->set_speed( 0 );
}

void MainPlayer::sliderReleased ()
{
	jahplayer->refresh( );
	jahplayer->restore_speed( );
}

void MainPlayer::keyPressEvent( QKeyEvent *e )
{
	if ( jahplayer )
	{
		e->accept( );
		switch( e->key() )
		{
			case ' ': 
				jahplayer->set_speed( !jahplayer->speed( ) ); 
				break;
			case Qt::Key_Up: 
				jahplayer->set_volume( jahplayer->volume( ) + ( jahplayer->volume( ) < 1.0f ? 0.1f : 0.0f ) ); 
				break;
			case Qt::Key_Down: 
				jahplayer->set_volume( jahplayer->volume( ) - ( jahplayer->volume( ) > 0.0f ? 0.1f : 0.0f ) ); 
				break;
			case Qt::Key_Left: 
				if ( jahplayer->speed( ) != 0 ) jahplayer->set_speed( 0 ); 
				jahplayer->seek( jahplayer->position( ) - frameStep ); 
				break;
			case Qt::Key_Right: 
				if ( jahplayer->speed( ) != 0 ) jahplayer->set_speed( 0 ); 
				jahplayer->seek( jahplayer->position( ) + frameStep ); 
				break;
			case Qt::Key_PageUp: 
				if ( jahplayer->speed( ) != 0 ) jahplayer->set_speed( 0 ); 
				jahplayer->seek( jahplayer->position( ) - 1000 ); 
				jahplayer->restore_speed( ); 
				break;
			case Qt::Key_PageDown: 
				if ( jahplayer->speed( ) != 0 ) jahplayer->set_speed( 0 ); 
				jahplayer->seek( jahplayer->position( ) + 1000 ); 
				jahplayer->restore_speed( ); 
				break;
			case 'I': 
				jahplayer->switch_deinterlace_mode( ); 
				break;
			case 'M':
				jahplayer->toggle_mute( );
				break;
			case 'Z':
				toggleZoomLock( );
				break;
			case Qt::Key_End:
				requestNextItem( );
				break;
			default: 
				e->ignore( );
				break;
		}
	}
}

void	MainPlayer::loadPreferences()
{
    //initalize the global vars from the prefs
    JahPrefs& jprefs = JahPrefs::getInstance();

    /////////////////////////////////////////////////////////
    //from the prefs
    JahBasePath  = jprefs.getBasePath().data();

	projectData thedata;

//     minFrameInput = maxFrameInput = 0;
    frameStep = 1;
}

//////////////////////////////////////////////////////////////////
// for drag n drop

void MainPlayer::draggedAImage()
{
    emit playerDraggedAImage( );
}

void MainPlayer::requestNextItem( )
{
	if ( AssetTables::getInstance( )->getSelectedAsset().clipnumber == theassetData.clipnumber )
    	emit playerRequestsNextItem( );
	else
    	emit playerDraggedAImage( );
}

/////////////////////////////////////////////////////////////////
//used in the jahplayer to flip desktop display modes
void MainPlayer::toggleDesktopImages() 
{
    emit toggleTheDesktopImages();
}

void MainPlayer::toggleDesktopTable() 
{
    emit toggleTheDesktopTable();
}

//actually we should use this routine for single frames as well
//for less confusion...
//--------------------------------------------------------------------
void	MainPlayer::UpdateAssetData(assetData &desktopclip)
{
#ifndef JAHPLAYER
    //used for conversions
    QString data;

    l_assetname_edit->setText(desktopclip.clipname);
    l_filename_edit->setText(desktopclip.filename);

    //figure out the category
    switch (desktopclip.theCategory)
    {
    case VideoCategory::IMAGE    : { data="IMAGE";      break; }
    case VideoCategory::CLIP     : { data="SEQUENCE";   break; }
    case VideoCategory::AUDIO    : { data="AUDIO";      break; }
    case VideoCategory::MEDIA    : { data="MEDIA";      break; }
    default                      : { data="UNKNOWN";    break; }
    }

    l_assettype_edit->setText(data);

    l_extension_edit->setText(desktopclip.extension);
    l_location_edit->setText(desktopclip.location);

    //get the width and height from the clip
    //i guess for audio this would be something different?
    assetExchange exchange;
    QImage theimage; 
    theimage = exchange.getImage(desktopclip);

    data.setNum( theimage.width());
    l_resolution_edit->setText( QString( "%1x%2" ).arg( theimage.width() ).arg( theimage.height() ) );

#endif 
}

///////////////////////////////////////////////
// function to control application level zoom
void MainPlayer::SetZoom(int index)
{
    jahplayer->zoom( double( 1000 - index ) / 100.0 );
}

void MainPlayer::wheelEvent( QWheelEvent *e )
{
	if ( !zoom->isVisible( ) )
		toggleZoomLock( );

	if ( e->delta( ) < 0 )
		zoom->setValue( zoom->value( ) + 10 );
	else if ( e->delta( ) > 0 )
		zoom->setValue( zoom->value( ) - 10 );
}

void MainPlayer::toggleZoomLock( void )
{
    bool enable = !zoom->isEnabled();

    zoom->setEnabled( enable );
    toolzoomin->setEnabled( enable );
    toolzoomout->setEnabled( enable );
    toolzoomreset->setEnabled( enable );

	// Switch between zoom modes
	if ( enable )
    	SetZoom( zoom->value( ) );
	else
    	jahplayer->zoom( 0.0 );
}

void MainPlayer::toggleZoomReset(void)
{
    zoom->setValue( 900 );
}

void MainPlayer::updateZoomIn(void)
{
    zoom->setValue( zoom->value( ) + 1 );
}

void MainPlayer::updateZoomOut(void)
{
    zoom->setValue( zoom->value( ) - 1 );
}

//for color view modes
void MainPlayer::toggleViewAll(void)
{
	m_mode = SHOW_ALL;
	jahplayer->ensureWritableImages( false );
	jahplayer->refresh( );
}

void MainPlayer::toggleViewRed(void)
{
	m_mode = SHOW_RED;
	jahplayer->ensureWritableImages( false );
	jahplayer->refresh( );
}

void MainPlayer::toggleViewGreen(void)
{
	m_mode = SHOW_GREEN;
	jahplayer->ensureWritableImages( false );
	jahplayer->refresh( );
}

void MainPlayer::toggleViewBlue(void)
{
	m_mode = SHOW_BLUE;
	jahplayer->ensureWritableImages( false );
	jahplayer->refresh( );
}

void MainPlayer::toggleViewAlpha(void)
{
	m_mode = SHOW_ALPHA;
	jahplayer->ensureWritableImages( true );
	jahplayer->refresh( );
}

#ifdef JAHPLAYER
void MainPlayer::buildInterfaceMenus( QHBox* leftcontroller, QHBox* rightcontroller) 
#else
void MainPlayer::buildInterfaceMenus( QHBox* , QHBox* rightcontroller) 
#endif
{
	QString image;

#ifdef JAHPLAYER
    //left menu here
    QFrame *glworldLeft = new QFrame (leftcontroller, "gl left");

    image = JahBasePath+"Pixmaps/desktop/thumbview.png";

    //we are only using these 2 for now but we may use the others later on
    toolthumbview = new JahToolButton( glworldLeft, "thumbview" );
    JahFormatter::addJahPlayerButton( toolthumbview, 0, 205, 30, 27, 24, 28, image, image );

    toolbinview = new JahToolButton( glworldLeft, "binview" );
	image = JahBasePath+"Pixmaps/desktop/binview.png";
    JahFormatter::addJahPlayerButton( toolbinview, 0, 235, 30, 27, 24, 28, image, image );

    connect( toolbinview, SIGNAL(clicked()), this, SLOT(toggleDesktopTable() )  );
    connect( toolthumbview, SIGNAL(clicked()), this, SLOT(toggleDesktopImages() ) ); 
#endif
	// Container widget
	QWidget *container = new QWidget( rightcontroller, "page" );
	QVBoxLayout *container_layout = new QVBoxLayout( container, 0, 0, "container_layout");

	container_layout->addStretch( );

    //rightmenu here
	image = JahBasePath+"Pixmaps/desktop/zoomin.png";
    toolzoomout = new JahToolButton( container, "zoomout" );
    JahFormatter::addJahPlayerButton( toolzoomout, image, image );
    connect( toolzoomout, SIGNAL(clicked()), this, SLOT(updateZoomOut() ) );
	toolzoomout->setAutoRepeat( true );
	container_layout->addWidget( toolzoomout );

	container_layout->addSpacing( 3 );

    zoom = new QSlider( container, "zoomslider" );
    zoom->setOrientation( QSlider::Vertical );
    zoom->setMinValue( 0 );    
    zoom->setMaxValue( 980 );    
    zoom->setValue( 900 );
    zoom->setPageStep( 1 );
	container_layout->addWidget( zoom, 0, Qt::AlignHCenter );

    connect( zoom, SIGNAL(valueChanged(int)), this, SLOT(SetZoom(int) ) );

	container_layout->addSpacing( 3 );

    toolzoomin = new JahToolButton( container, "zoomin" );
	image = JahBasePath+"Pixmaps/desktop/zoomout.png";
    JahFormatter::addJahPlayerButton( toolzoomin, image, image );
    connect( toolzoomin, SIGNAL( pressed( ) ), this, SLOT( updateZoomIn( ) ) );
	toolzoomin->setAutoRepeat( true );
	container_layout->addWidget( toolzoomin );

	container_layout->addSpacing( 3 );

    toolzoomreset = new JahToolButton( container, "zoomreset" );
	image = JahBasePath+"Pixmaps/desktop/zoomin.png";
    JahFormatter::addJahPlayerButton( toolzoomreset, image, image );
    connect( toolzoomreset, SIGNAL(clicked()), this, SLOT( toggleZoomReset( ) ) );
	container_layout->addWidget( toolzoomreset );

    toolzoomlock = new JahToolButton( container, "zoomlock" );
	image = JahBasePath+"Pixmaps/player/reflect.png";
    JahFormatter::addJahPlayerButton( toolzoomlock, image, image );
    connect( toolzoomlock, SIGNAL(clicked()), this, SLOT(toggleZoomLock() ) );
	container_layout->addWidget( toolzoomlock );

	container_layout->addSpacing( 10 );

	////////////////////////////////////////
	//now for the color options
    toolShowAll = new JahToolButton( container, "showall" );
	toolShowAll->setText( "ALL" );
	toolShowAll->setFixedSize( 30, 30 );
    connect( toolShowAll, SIGNAL(clicked()), this, SLOT(toggleViewAll() )  );
	container_layout->addWidget( toolShowAll );

    toolShowRed = new JahToolButton( container, "showred" );
	toolShowRed->setText( "R" );
	toolShowRed->setFixedSize( 30, 30 );
    connect( toolShowRed, SIGNAL(clicked()), this, SLOT(toggleViewRed() ) );
	container_layout->addWidget( toolShowRed );

    toolShowGreen = new JahToolButton( container, "showgreen" );
	toolShowGreen->setText( "G" );
	toolShowGreen->setFixedSize( 30, 30 );
    connect( toolShowGreen, SIGNAL(clicked()), this, SLOT(toggleViewGreen() ) );
	container_layout->addWidget( toolShowGreen );

    toolShowBlue = new JahToolButton( container, "showblue" );
	toolShowBlue->setText( "B" );
	toolShowBlue->setFixedSize( 30, 30 );
    connect( toolShowBlue, SIGNAL(clicked()), this, SLOT(toggleViewBlue() ) );
	container_layout->addWidget( toolShowBlue );

    toolShowAlpha = new JahToolButton( container, "showalpha" );
	toolShowAlpha->setText( "A" );
	toolShowAlpha->setFixedSize( 30, 30 );
    connect( toolShowAlpha, SIGNAL(clicked()), this, SLOT(toggleViewAlpha() ) );
	container_layout->addWidget( toolShowAlpha );

	container_layout->addStretch( );
}

//////////////////////////////////////////////////////////////
// This is the player control bar
// please clean me up and make me look cool

void MainPlayer::buildController( QHBox* controller) 
{
    QVBox* top = new QVBox( controller );

    // Container widget
    QWidget *transportContainer = new QWidget( top, "page" );
    QHBoxLayout *transportLayout = new QHBoxLayout( transportContainer );
    
    transportLayout->addStretch( 10 );

    // Play mode 
    playmodecombo = new QComboBox( transportContainer, "playermode" );
    playmodecombo->setFixedSize( QSize( 90, 20 ) );
    playmodecombo->insertItem( tr( "Loop" ) );
    playmodecombo->insertItem( tr( "RAM Player" ) );
    playmodecombo->insertItem( tr( "Play" ) );
    playmodecombo->insertItem( tr( "Playlist" ) );
    connect( playmodecombo,  SIGNAL( activated( int ) ), SLOT( setPlayMode( int ) ) ); 
    transportLayout->addWidget( playmodecombo );
    
    transportLayout->addSpacing( 10 );

    // Transport buttons
    scrubfirst = new JahToolButton( transportContainer, "controllerrewindbutton" );
    scrubfirst->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubfirst, JahBasePath+"Pixmaps/player/rewind_off.png", JahBasePath+"Pixmaps/player/rewind_on.png" );
    connect( scrubfirst,  SIGNAL(pressed()), SLOT(Rewind())  );
    connect( scrubfirst,  SIGNAL(released()), this, SLOT(sliderReleased()) );
    transportLayout->addWidget( scrubfirst );
    
    scrubprevious = new JahToolButton( transportContainer, "controllerpreviousbutton" );
    scrubprevious->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubprevious, JahBasePath+"Pixmaps/player/frewind_off.png", JahBasePath+"Pixmaps/player/frewind_on.png" );
    connect( scrubprevious,  SIGNAL(pressed()), SLOT(FrameBackward())  );
    connect( scrubprevious,  SIGNAL(released()), this, SLOT(sliderReleased()));
    transportLayout->addWidget( scrubprevious );
    
    scrubstop = new JahToolButton( transportContainer, "controllerstopbutton" );
    scrubstop->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubstop, JahBasePath+"Pixmaps/player/stop_off.png", JahBasePath+"Pixmaps/player/stop_on.png" );
    connect( scrubstop,  SIGNAL(clicked()), SLOT(Stop())  );
    transportLayout->addWidget( scrubstop );
    
    scrubplay = new JahToolButton( transportContainer, "play" );
    scrubplay->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubplay, JahBasePath+"Pixmaps/player/play_off.png", JahBasePath+"Pixmaps/player/play_on.png" );
    connect( scrubplay,  SIGNAL(clicked()), SLOT(LoopForward())  );
    transportLayout->addWidget( scrubplay );
    
    scrubnext = new JahToolButton( transportContainer, "controllernextbutton" );
    scrubnext->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrubnext, JahBasePath+"Pixmaps/player/ffoward_off.png", JahBasePath+"Pixmaps/player/ffoward_on.png" );
    connect( scrubnext,  SIGNAL(pressed()), SLOT(FrameForward())  );
    connect( scrubnext,  SIGNAL(released()), this, SLOT(sliderReleased())  );
    transportLayout->addWidget( scrubnext );
    
    scrublast = new JahToolButton( transportContainer, "controllerffworardbutton" );
    scrublast->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( scrublast, JahBasePath+"Pixmaps/player/foward_off.png",  JahBasePath+"Pixmaps/player/foward_on.png" );
    connect( scrublast,  SIGNAL(pressed()), SLOT(FastForward())  );
    connect( scrublast,  SIGNAL(released()), this, SLOT(sliderReleased())  );
    transportLayout->addWidget( scrublast );

    transportLayout->addSpacing( 10 );

    // Time code display
    animframe = new SupergRangeControl( transportContainer, "timecodedisplay" );
    animframe->setFormatter( TimecodeValueFormatter() );
    JahFormatter::configure( animframe, 0, 0 );
    connect( animframe, SIGNAL(valueChanged(int)), this, SLOT( sliderValueChanged( int ) ) );
    transportLayout->addWidget( animframe );

    transportLayout->addSpacing( 10 );

    // FPS for image sequences
    scrubrender = new QComboBox( transportContainer, "fps" );
    scrubrender->setFixedSize( QSize( 80, 20 ) );
    scrubrender->insertItem( tr( "15 fps" ) );
    scrubrender->insertItem( tr( "24 fps" ) );
    scrubrender->insertItem( tr( "25 fps" ) );
    scrubrender->insertItem( tr( "30 fps" ) );
    scrubrender->insertItem( tr( "60 fps" ) );
    scrubrender->insertItem( tr( "90 fps" ) );
    scrubrender->insertItem( tr( "120 fps" ) );
    scrubrender->insertItem( tr( "150 fps" ) );
    scrubrender->insertItem( tr( "180 fps" ) );
    scrubrender->setCurrentItem( 3 );
    connect( scrubrender,  SIGNAL( activated( int ) ), SLOT( switchModeFps( int ) ) ); 

    transportLayout->addWidget( scrubrender );

    transportLayout->addSpacing( 10 );

    // Scrubber step offset selection
    scrubrenderAll = new QComboBox( transportContainer, "imagingrenderbutton" );
    scrubrenderAll->setFixedSize( QSize( 50, 20 ) );
    scrubrenderAll->insertItem( "+1" );
    scrubrenderAll->insertItem( "+2" );
    scrubrenderAll->insertItem( "+3" );
    scrubrenderAll->insertItem( "+4" );
    scrubrenderAll->insertItem( "+5" );
    connect( scrubrenderAll,  SIGNAL( activated( int ) ), SLOT( switchModeStep( int ))  );
    
    transportLayout->addWidget( scrubrenderAll );

    transportLayout->addStretch( 5 );

    // Container widget
    QWidget* sliderContainer = new QWidget( top );
    QHBoxLayout* sliderLayout = new QHBoxLayout( sliderContainer );

    QPalette p = palette();
    JahFormatter::swap( p, QColorGroup::Shadow, QColorGroup::Background );
    
    SupergRangeControl* controllerStartFrameControl = new SupergRangeControl( sliderContainer );
    controllerStartFrameControl->setFormatter( TimecodeValueFormatter() );
    JahFormatter::configure( controllerStartFrameControl, 0, 0 );
    controllerStartFrameControl->setPalette( p );
    controllerStartFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    sliderLayout->addWidget( controllerStartFrameControl );

    // Slider
    controllerslider = new TimelineSlider( sliderContainer, "controllerslider" );
    sliderLayout->addWidget( controllerslider );

    SupergRangeControl* controllerEndFrameControl = new SupergRangeControl( sliderContainer );
    controllerEndFrameControl->setFormatter( TimecodeValueFormatter() );
    JahFormatter::configure( controllerEndFrameControl, 0, 0 );
    controllerEndFrameControl->setPalette( p );
    controllerEndFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    sliderLayout->addWidget( controllerEndFrameControl );

    connect( controllerslider, SIGNAL( minChanged( int ) ), controllerStartFrameControl, SLOT( setValue( int ) ) );
    connect( controllerslider, SIGNAL( maxChanged( int ) ), controllerEndFrameControl, SLOT( setValue( int ) ) );
    connect( controllerStartFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( SetMinFrame( int ) ) );
    connect( controllerEndFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( SetMaxFrame( int ) ) );

    connect(controllerslider, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) );
    connect(controllerslider, SIGNAL(sliderPressed()), this, SLOT(sliderPressed()) );
    connect(controllerslider, SIGNAL(sliderReleased()), this, SLOT(sliderReleased()) );
}

void MainPlayer::buildInterface( QHBox* dummy ) 
{
	///////////////////////////////////////////////////////////////////////
    //set up asset data menus

    QWidget* f = new QWidget( dummy );
    QBoxLayout* mainLayout = new QHBoxLayout( f );
    JahFormatter::setMarginAndSpacing( mainLayout );

    Collapsable* container = new Collapsable( f );
    container->setTitle( tr( "asset" ) );
    mainLayout->addWidget( container );

    QWidget* assetGroupBox = new QWidget( container );
    container->add( assetGroupBox );

    QGridLayout* assetLayout = new QGridLayout( assetGroupBox, 4, 4 );
    JahFormatter::setMarginAndSpacing( assetLayout );

    l_assetname_edit = new QLabel( assetGroupBox, "l_assetname_edit" );
    l_filename_edit = new QLabel( assetGroupBox, "l_filename_edit" );
    l_assettype_edit = new QLabel( assetGroupBox, "l_assettype_edit" );
    l_extension_edit = new QLabel( assetGroupBox, "l_extension_edit" );
    l_location_edit = new QLabel( assetGroupBox, "l_location_edit" );
    l_resolution_edit = new QLabel( assetGroupBox, "l_resolution_edit" );

    assetLayout->addWidget( new QLabel( tr( "name:" ), assetGroupBox, "l_assetname" ), 0, 0, Qt::AlignRight );
    assetLayout->addWidget( l_assetname_edit, 0, 1 );
    assetLayout->addWidget( new QLabel( tr( "filename:" ), assetGroupBox, "l_filename" ), 1, 0, Qt::AlignRight );
    assetLayout->addWidget( l_filename_edit, 1, 1 );
    assetLayout->addWidget( new QLabel( tr( "type:" ), assetGroupBox, "l_assettype" ), 0, 2, Qt::AlignRight );
    assetLayout->addWidget( l_assettype_edit, 0, 3 );
    assetLayout->addWidget( new QLabel( tr( "extn.:" ), assetGroupBox, "l_extension" ), 1, 2, Qt::AlignRight );
    assetLayout->addWidget( l_extension_edit, 1, 3 );
    assetLayout->addWidget( new QLabel( tr( "resolution:" ), assetGroupBox, "l_resolution" ), 2, 0, Qt::AlignRight );
    assetLayout->addMultiCellWidget( l_resolution_edit, 2, 2, 1, 3 );
    assetLayout->addWidget( new QLabel( tr( "location:" ), assetGroupBox, "l_location" ), 3, 0, Qt::AlignRight );
    assetLayout->addMultiCellWidget( l_location_edit, 3, 3, 1, 3 );

    QBoxLayout* v = new QVBoxLayout();
    mainLayout->addLayout( v );

//     QGridLayout* frameLayout = new QGridLayout( 2, 2 );
//     v->addLayout( frameLayout );
//     frameLayout->addWidget( new QLabel( tr( "start:" ), f, "l_startframe" ), 0, 0 );
//     frameLayout->addWidget( new QLabel( tr( "end:" ), f, "l_startframe" ), 1, 0 );

//     minFrameInput = new InputLCD( f, "lcd_startframe" );
//     minFrameInput->setFormatter( TimecodeValueFormatter() );
//     frameLayout->addWidget( minFrameInput, 0, 1 );
//     JahFormatter::addSliderLcd( minFrameInput );
//     connect(minFrameInput, SIGNAL(valueChanged(int)), SLOT(SetMinFrame(int)) );

//     maxFrameInput = new InputLCD( f, "lcd_endframe" );
//     maxFrameInput->setFormatter( TimecodeValueFormatter() );
//     frameLayout->addWidget( maxFrameInput, 1, 1 );
//     JahFormatter::addSliderLcd( maxFrameInput );
//     connect(maxFrameInput, SIGNAL(valueChanged(int)), SLOT(SetMaxFrame(int)) );

    v->addStretch();

    mainLayout->addStretch();
}

void MainPlayer::setPlayMode( int index )
{
	switch( index )
	{
		case 0: jahplayer->set_eof( QtPlayer::loop ); break;
		case 1: jahplayer->set_eof( QtPlayer::ramplayer ); break;
		case 2: jahplayer->set_eof( QtPlayer::pause ); break;
		case 3: jahplayer->set_eof( QtPlayer::next ); break;
	}

	LoadMySequence( theassetData );
}

void MainPlayer::switchModeFps( int index )
{
    float fpsval = 25.0f;

	switch( index )
	{
		case 0: fpsval = 15.0f; break;
		case 1: fpsval = 24.0f; break;
		case 2: fpsval = 25.0f; break;
		case 3: fpsval = 30.0f; break;
		case 4: fpsval = 60.0f; break;
		case 5: fpsval = 90.0f; break;
		case 6: fpsval = 120.0f; break;
		case 7: fpsval = 150.0f; break;
		case 8: fpsval = 180.0f; break;
	}

    jahplayer->set_fps( fpsval );
}

void MainPlayer::switchModeStep( int value )
{
	frameStep = value + 1;
}

void MainPlayer::createMenuItem( QPopupMenu * )
{
}

QString MainPlayer::getModuleName()
{
  //  return m_qsModuleName ;
  return ModuleName;
}
