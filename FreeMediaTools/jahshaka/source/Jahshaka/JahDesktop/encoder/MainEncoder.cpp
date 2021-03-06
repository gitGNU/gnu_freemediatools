/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "MainEncoder.h"
#include "jahtranslate.h"

#include <qtplayer.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qtabwidget.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtimer.h>
#include <qcombobox.h>
#include <qstylefactory.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qpopupmenu.h>
#include <qlineedit.h>
#include <qprogressdialog.h>
#include <qspinbox.h>
#include <qapplication.h>
#include <qfiledialog.h>

#include <jahformatter.h>
#include <valueFormatters.h>
#include <widget.h>
#include <supergrangecontrol.h>
#include <timelineSlider.h>
#include <jahpreferences.h>
#include <jahtracer.h>

#include <assetexchange.h>
#include <render.h>

double MainEncoder::fpsVal[NumFPS];


//==============================================================================
MainEncoder::MainEncoder( const QGLWidget *core, QWidget *parent, const char* name, QHBox* control, QHBox* controller,
	QHBox* leftcontroller, QHBox* rightcontroller) 
	: QWidget (parent, name)
	, m_core( core )
	, m_parent( parent )
	, m_control( control )
	, m_controller( controller )
	, m_leftcontroller( leftcontroller )
	, m_rightcontroller( rightcontroller ),
	  m_started( false ),
	  m_render_formats_label(0),
	  m_render_normalisation_label(0),
	  m_render_profiles_label(0),
	  m_render_formats_combobox(0),
	  m_render_normalisation_combobox(0),
	  m_render_profiles_combobox(0),
	  EffectsFrame(0),
	  MiddleFrame(0),
	  Slider2(0),
	  animframe(0),
	  EncodeButton(0),
	  MainController(0),
	  frameNumString(0),
	  TimecodeFrame(0),
	  controllerffworardbutton(0),
	  controllerframe(0),
	  controllernextbutton(0),
	  controllerplaybutton(0),
	  controllerpreviousbutton(0),
	  controllerrewindbutton(0),
	  controllerstopbutton(0),
	  forceplaycheckbox(0),
	  forceplayframe(0),
	  imagingphotobutton(0),
	  imagingrenderbutton(0),
	  keyframebutton(0),
	  m_pControllerSlider(0),
	  m_render_button(0),
	  maincontroller(0),
	  page(0),
	  pingButton(0),
	  rewButton(0),
	  scrublast(0),
	  stopButton(0),
	  Form1Layout(0),
	  controlers(0),
	  controlpanel(0),
	  controls(0),
	  //	  forcedplay(0),
	  imagingframe(0),
	  jahshakaLayout(0),
	  maincontrollerLayout(0),
	  mainplayerLayout(0),
	  mainplayerframe(0),
	  mainworld(0),
	  mainworldLayout(0),
	  object_controls(0),
	  pageLayout(0),
	  player(0),
	  playerClip(0),
	  playerframe(0),
	  playerh(0),
	  playerv(0),
	  playmodecombo(0),
	  scene_controls(0),
	  scrubfirst(0),
	  scrubkey(0),
	  scrubnext(0),
	  scrubplay(0),
	  scrubprevious(0),
	  scrubrender(0),
	  scrubrenderAll(0),
	  scrubstop(0),
	  timecodedisplay(0),
	  timecodeframe(0),
	  //	  button(0),
	  filePopup(0),
	  frameNum(0),
	  frameScroll(0),
	  glworldLeft(0),
	  glworldRight(0),
	  m_render_formats_button(0),
	  m_render_profiles_button(0),
	  menuBar(0),
	  midL(0),
	  midM(0),
	  midR(0),
	  middle(0),
	  status(0),
	  timer(0),
	  toolbinview(0),
	  toolrotate(0),
	  toolscale(0),
	  toolthumbview(0),
	  tooltranslate(0),
	  toolzoomin(0),
	  toolzoomout(0),
	  topLayout(0),
	  utilPopup(0),
	  zoom(0),
	  minFrame(0),
	  maxFrame(0),
	  ModuleName(name)
{
  
}

MainEncoder::~MainEncoder()
{
  //	delete m_jahnle_render;
}

void MainEncoder::activate( )
{
	if ( m_started ) return;

	m_started = true;

    m_jah_translate = JahTranslate::getInstance();

	//initalize the global vars from the prefs
    jtrace = JahTrace::getInstance();	//set up tracer
    JahPrefs& jprefs = JahPrefs::getInstance();

    JahBasePath  = jprefs.getBasePath().data();

    buildController(m_controller);

    //restore the preferences note that its not a dialog
    loadPreferences();

    /////////////////////////////////////////////////////////
    //create the ui for the player

    mainworldLayout = new QHBoxLayout( m_parent);

    encplayer = new QtPlayer( m_core, "test:", 0, m_parent);
    mainworldLayout->addWidget( encplayer );

    //////////////////////////////////////////////////////////
    //this creates the interface of the module for us
    buildInterface( m_control );

    //////////////////////////////////////////////////////////////////////////
    // connect the slots

    connect(encplayer, SIGNAL( showImage( int, QImage * ) ), this, SLOT( showImage( int, QImage * ) ) );    //connect player drag n drop to player load signal
    connect(encplayer, SIGNAL(draggedAImage()), this, SLOT(draggedAImage()) );
    connect(encplayer, SIGNAL(resizePlayer(mediaSize)), this, SLOT(ResizeDisplay(mediaSize)) );

    //////////////////////////////////////////////////////////
    //create our timer for the player
    timer = new QTimer(this);
	//playonce = false;
    //connect(timer, SIGNAL(timeout()), this, SLOT(Action()) );

	framerateCounter = 0;
	accumulated = 0;

    //create config object to be passed to the encoder
    encodingStatus = false;
}

void MainEncoder::loadPreferences()
{
    //different frame rates
    fpsVal[F90] = 90.0;
    fpsVal[F60] = 60.0;
    fpsVal[F48] = 48.0;
    fpsVal[F30] = 30.0;
    fpsVal[F24] = 24.0;
    fpsVal[F20] = 20.0;
    fpsVal[F15] = 15.0;
    fpsVal[F12] = 12.0;
    fpsVal[F10] = 10.0;

    //default to 30fps
    currentSpeed = fpsVal[F30];

    //default values
    curFrame	=  0;
    pingpong	=  1;
    frameStep   =  1;
}


///////////////////////////////////////////////////////////////
// this is the main calling routine

void MainEncoder::LoadMySequence(assetData desktopclip)
{
    //the local instance of the asset
    theassetData = desktopclip;
	QString filename = theassetData.location+theassetData.filename+theassetData.extension;

    switch (desktopclip.theCategory)
    {
	    case VideoCategory::IMAGE:
			filename += QString( "/sequence:" );
			break;

		case VideoCategory::MEDIA:
		case VideoCategory::CLIP:
            if (theassetData.theType != VideoType::MOVIE)
				filename = theassetData.location + theassetData.filename + QString( "*" ) + theassetData.extension + QString( "/sequence:" );
			break;

		case VideoCategory::AUDIO:
		default:
			break;
    }
    
    filename.replace( "//", "/" );

    encplayer->show( );
	encplayer->play( filename, 0 );

	if (m_pControllerSlider)
	    m_pControllerSlider->setRange( 0,  encplayer->length( ) );

    //this sets up the interface values
    LoadSequenceCompleted(1, true);

    SetFrameNumber( 0 );
}

void	MainEncoder::SetFrameNumber(int i)
{
    int frame = i;

    if (maxFrame == 0)
    {
        // No clip loaded yet
        return;
    }

    if(frame > (int)maxFrame) 
    {
        frame = minFrame;
        encplayer->seek(frame);
        encplayer->stop();
    }
    
    if(frame < (int)minFrame) 
    {
        frame = minFrame;
        encplayer->seek(frame);
    }

    frameNumString.setNum( frame );
    if (animframe) // CRASH
      animframe->setValue( frame );
    if (m_pControllerSlider) // CRASH
      m_pControllerSlider->setValue( frame );
}

void	MainEncoder::Clear()
{
    Stop();

    //encplayer->Reset();

    //zero the sliders and settings
    curFrame	=  0;
    pingpong	=  0;
    frameStep   =  1;
}

//used for external access to plyaer
void	MainEncoder::ClearOut()
{
    Clear();
}

void	MainEncoder::LoadSequenceCompleted(int, bool)
{
    startFrame = theassetData.startframe;
    minFrame = 0;
    maxFrame = encplayer->length( );

if (m_pControllerSlider)
  m_pControllerSlider->setValue( 0 );

}

void MainEncoder::sliderValueChanged(int frame_number)
{
	if ( encplayer->speed() == 0 )
    {
		encplayer->seek(frame_number);
    }
}

//--------------------------------------------------------------------
void	MainEncoder::SetMinFrame(int value)
{
	if ( value <= int( maxFrame ) )
	{
            minFrame = value;
	    if (m_pControllerSlider)
	      m_pControllerSlider->setRange(minFrame, maxFrame);
	}
}

void	MainEncoder::SetMaxFrame(int value)
{
	if ( value >= int( minFrame ) )
	{
            maxFrame = value;
	    if (m_pControllerSlider)
	      m_pControllerSlider->setRange(minFrame, maxFrame);
	}
}

//--------------------------------------------------------------------
void	MainEncoder::LoopForward()
{
	encplayer->set_speed( 1 );
}

void	MainEncoder::setLoopForward()
{
    bool isact = false;

    if (timer->isActive())
        { timer->stop(); isact=true; }


        playMode = false;
        pingpong = false;

		framerateCounter = 0;
		accumulated = 0;

        //timer speeds here
        if (isact) 
        { 
            timer->start((int)(1000.0/currentSpeed)); 
        }
}

//--------------------------------------------------------------------
void	MainEncoder::PingPong()
{
    bool isact = false;

    if (timer->isActive())
        { timer->stop(); isact=true; }

    playMode = true;
    pingpong = true;

	framerateCounter = 0;
	accumulated = 0;

    if (isact) 
    { 
        timer->start((int)(1000.0/currentSpeed)); 
    }

}
//stops the timer if its running
void	MainEncoder::Stop()
{
	if ( encplayer ) encplayer->stop();
}

void	MainEncoder::FastForward()
{
	if ( encplayer ) encplayer->set_speed( 25 );
}

void	MainEncoder::FrameForward()
{
	if ( encplayer ) encplayer->seek( encplayer->position( ) + 1 );
}

void	MainEncoder::FrameBackward()
{
	if ( encplayer ) encplayer->seek( encplayer->position( ) - 1 );
}

void	MainEncoder::Rewind()
{
	if ( encplayer ) encplayer->set_speed( -25 );
}

//--------------------------------------------------------------------
void MainEncoder::Action()
{
    //if frame==lastframe then stop timer for regular play
    switch(playMode)
    {
        case LOOP_FORWARD:
            ActionLoopForward();
            break;
        case PING_PONG:
            ActionPingPong();
            break;
        default:
            //warning ("MainEncoder::playMode is set incorrectly.\nplayMode = %d", playMode);
            timer->stop();
    }
}

//- - - - - - - - - - - - - - - - - - - - - - -
void MainEncoder::ActionLoopForward()
{
        curFrame += frameStep;

        if (curFrame > maxFrame)
        {
            if (playonce)
            {
                timer->stop();
                curFrame = maxFrame;
            }
            else
            {
                curFrame = minFrame + 1;
            }
        }

	if (m_pControllerSlider)
	  m_pControllerSlider->setValue(curFrame);
        SetFrameNumber(curFrame);
}

void MainEncoder::ActionPingPong()
{
    curFrame += pingpong;
    if (curFrame > maxFrame)
    {
        curFrame = maxFrame;
        pingpong = -frameStep;
    }
    else if (curFrame < minFrame)
    {
        curFrame = minFrame;
        pingpong = frameStep;
    }

    if (m_pControllerSlider)
      m_pControllerSlider->setValue(curFrame);
    SetFrameNumber(curFrame);
}

//--------------------------------------------------------------------
void MainEncoder::switchFps(int fpsval)
{

    switch(fpsval) {
        case 90  : { SetFPS(F90); break; }
        case 60  : { SetFPS(F60); break; }
        case 48  : { SetFPS(F48); break; }
        case 30  : { SetFPS(F30); break; }
        case 24  : { SetFPS(F24); break; }
        case 20  : { SetFPS(F20); break; }
        case 15  : { SetFPS(F15); break; }
        case 12  : { SetFPS(F12); break; }
        case 10  : { SetFPS(F10); break; }
        default  : { SetFPS(F30); break; }
    }


}
//--------------------------------------------------------------------
void MainEncoder::SetFPS(FPS index)
{
    //for(int i = 0; i < 7; i++)

    currentSpeed = fpsVal[index];

    if (timer->isActive())
    {
        timer->changeInterval((int)(1000.0/currentSpeed));
    }
}

//--------------------------------------------------------------------

void MainEncoder::SetStep(int index)
{

    switch(index)
    {
        case 1:	frameStep = 1;
            break;
        case 2:	frameStep = 2;
            break;
        case 3:	frameStep = 3;
            break;
        case 4:	frameStep = 4;
            break;
        case 5:	frameStep = 5;
            break;
        case 6:	frameStep = 6;
            break;
    }
}

//--------------------------------------------------------------------

void MainEncoder::SetZoom(int index)
{
    //map the zoom value
    if (index==0) 
        index=1;

    //double thezoom = (index/100.0);    

    //update the necessary player object now
    //encplayer->SetDisplayZoom(thezoom);

    //now update the ui based on the slider position
    if (m_pControllerSlider)
      SetFrameNumber(m_pControllerSlider->value());

}

void MainEncoder::ResizeDisplay(mediaSize thesize)
{
    player->setMinimumSize( QSize( thesize.width, thesize.height ) );
    player->setMaximumSize( QSize( thesize.width, thesize.height ) );
}

void MainEncoder::slotGenerateRenderFormatList()
{
}

void MainEncoder::slotGenerateRenderProfileList()
{

}

void MainEncoder::slotSelectRenderFormat(int offset)
{

}
    
void MainEncoder::slotSelectRenderProfile(int offset)
{

}

void MainEncoder::slotStartRender()
{

}

void MainEncoder::draggedAImage()
{

}

void MainEncoder::showImage( int position, QImage * )
{

}

void MainEncoder::sliderPressed( )
{
	encplayer->set_speed( 0 );
}

void MainEncoder::sliderReleased( )
{
	encplayer->refresh( );
	encplayer->restore_speed( );
}

void MainEncoder::buildController( QHBox* controller) 
{
}

void MainEncoder::buildInterface( QHBox* qtinterface )
{


}

void MainEncoder::createMenuItem( QPopupMenu * themenu )
{
	Q_CHECK_PTR(themenu);
}


QString MainEncoder::getModuleName()
{
  return ModuleName;
}
