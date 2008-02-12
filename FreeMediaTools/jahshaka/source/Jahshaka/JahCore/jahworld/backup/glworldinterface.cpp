/*******************************************************************************
**
** The source file for the Jahshaka glworld module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"

#include <qhbox.h>
#include <qvbox.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qstylefactory.h>

#include <jahtranslate.h>
#include <jahformatter.h>
#include <valueFormatters.h>
#include <qslider.h>
#include <timelineSlider.h>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <widget.h>
#include <supergrangecontrol.h>

void GLWorld::buildController( QHBox* controller) 
{
    Astartframe=1;  Aendframe=100;  Aanimframe=1;

    QVBox* top = new QVBox( controller );

    // Container widget
    QWidget *transportContainer = new QWidget( top, "page" );
    QHBoxLayout *transportLayout = new QHBoxLayout( transportContainer );
    
    transportLayout->addStretch( 10 );
    
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
    
#ifndef JAHPLAYER
    if ( usesKeyFrames( ) )
    {
        transportLayout->addStretch( 5 );

        keyback = new JahToolButton( transportContainer, "controllerrewindbutton" );
        keyback->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( keyback, JahBasePath+"Pixmaps/player/keyback_off.png", JahBasePath+"Pixmaps/player/keyback_on.png" );
        connect( keyback,  SIGNAL(clicked()), SLOT(prevKeyframe())  );
        transportLayout->addWidget( keyback );
	
        keyadd = new JahToolButton( transportContainer, "controllerstopbutton" );
        keyadd->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( keyadd, JahBasePath+"Pixmaps/player/keyplus_off.png", JahBasePath+"Pixmaps/player/keyplus_on.png" );
        connect( keyadd,  SIGNAL(clicked()), SLOT(addKeyframe())  );
        transportLayout->addWidget( keyadd );
	
        keyremove = new JahToolButton( transportContainer, "play" );
        keyremove->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( keyremove, JahBasePath+"Pixmaps/player/keyminus_off.png", JahBasePath+"Pixmaps/player/keyminus_on.png" );
        connect( keyremove,  SIGNAL(clicked()), SLOT(removeKeyframe())  );
        transportLayout->addWidget( keyremove );
	
        keynext = new JahToolButton( transportContainer, "controllerffworardbutton" );
        keynext->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( keynext, JahBasePath+"Pixmaps/player/keyforward_off.png",  JahBasePath+"Pixmaps/player/keyforward_on.png" );
        connect( keynext,  SIGNAL(clicked()), SLOT(nextKeyframe())  );
        transportLayout->addWidget( keynext );
	
        transportLayout->addSpacing( 10 );
    }
    else
    {
        transportLayout->addStretch( 10 );
    }

    GetButton = new QPushButton( transportContainer, "GetButton" );
    GetButton->setFixedSize( 120, 20 );
    GetButton->setText( jt->tr( "&Get Desktop"  ) );
    connect( GetButton,  SIGNAL(clicked()), SLOT( grabDesktop())  );
    transportLayout->addWidget( GetButton );

#else
    transportLayout->addStretch();
#endif

    // Container widget
    QWidget* sliderContainer = new QWidget( top );
    QHBoxLayout* sliderLayout = new QHBoxLayout( sliderContainer );

    QPalette p = palette();
    JahFormatter::swap( p, QColorGroup::Shadow, QColorGroup::Background );
    
    controllerStartFrameControl = new SupergRangeControl( sliderContainer );
    controllerStartFrameControl->setFormatter( TimecodeValueFormatter() );
    controllerStartFrameControl->setPalette( p );
    controllerStartFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    JahFormatter::configure( controllerStartFrameControl, Astartframe, Astartframe );
    sliderLayout->addWidget( controllerStartFrameControl );

    // Slider
    controllerslider = new TimelineSlider( sliderContainer, "controllerslider" );
    controllerslider->setMinValue( Astartframe );
    controllerslider->setMaxValue( Aendframe );
    controllerslider->setValue   ( Aanimframe );
    sliderLayout->addWidget( controllerslider );

    controllerEndFrameControl = new SupergRangeControl( sliderContainer );
    controllerEndFrameControl->setFormatter( TimecodeValueFormatter() );
    controllerEndFrameControl->setPalette( p );
    controllerEndFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    JahFormatter::configure( controllerEndFrameControl, Aendframe, Astartframe );
    sliderLayout->addWidget( controllerEndFrameControl );

    connect( controllerslider, SIGNAL( minChanged( int ) ), controllerStartFrameControl, SLOT( setValue( int ) ) );
    connect( controllerslider, SIGNAL( maxChanged( int ) ), controllerEndFrameControl, SLOT( setValue( int ) ) );
    connect( controllerStartFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( updatesliderStartframe( int ) ) );
    connect( controllerEndFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( updatesliderEndframe( int ) ) );

    connect( controllerslider, SIGNAL( sliderMoved(int) ),  SLOT(updateAnimation(int)) );
    connect( controllerslider, SIGNAL( valueChanged(int) ), SLOT(sliderValueChanged(int)) );
    connect( controllerslider, SIGNAL( valueChanged(int) ), m_animframe_lcd, SLOT(setValue(int)) );
    connect( m_animframe_lcd,  SIGNAL( valueChanged(int) ), controllerslider, SLOT(setValue(int)) );
}

void GLWorld::updatesliderStartframe( int startframe ) 
{
    if (startframe > maxFrame)
    {
        startframe = maxFrame;
    }

    controllerslider->setMinValue( startframe );
    minFrame = startframe;
}

void GLWorld::updatesliderEndframe( int endframe )     
{	
	controllerslider->setMaxValue( endframe );
	maxFrame = endframe;
}

void GLWorld::createAxisPanel ( QFrame* theuiframe)
{
    //////////////////////////////////////////////////////////////////////////////////////////
    // set up rotate sliders
    //set some flags
    hasLabels = true;
    hasSliders = true;
    //JahTranslate& jt = JahTranslate::getInstance();

    //JahModuleLabel[0] = new QLabel( theuiframe, "RotateText_1" );
    //JahModuleLabelText[0] = jt->tr("ROTATE");

    //JahFormatter::addLabel( JahModuleLabel[0], 10, 10, 110, 21, JahModuleLabelText[0], 9);

    int y_displacement = 35;
    int start_y_position = 30;
    int y_position = start_y_position;
    int range = 2000;

    JahSliders[0] = new QSlider( theuiframe );
    JahSlidersLCD[0] = new InputLCD( theuiframe );
    JahSlidersLabel[0] = new QLabel ( theuiframe);
    JahSlidersLabelText[0]="X Rotation";

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[0], 0, y_position, 10, 21,  range, JahSlidersLCD[0],
                        JahSlidersLabel[0], JahSlidersLabelText[0], 9);

    connect( JahSliders[0],   SIGNAL(valueChanged(int)), JahSlidersLCD[0], SLOT(setValue(int)) );
    connect(JahSliders[0], SIGNAL(valueChanged(int)), this, SLOT(setXRotation(int)) );
    connect(JahSlidersLCD[0], SIGNAL(valueChanged(int)), this, SLOT(setXRotation(int)) );
    connect(JahSlidersLCD[0], SIGNAL(valueChanged(int)), JahSliders[0], SLOT(setValue(int)) );

    JahSliders[1] = new QSlider( theuiframe );
    JahSlidersLCD[1] = new InputLCD( theuiframe );
    JahSlidersLabel[1] = new QLabel ( theuiframe);
    JahSlidersLabelText[1]="Y Rotation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[1], 0, y_position, 10, 21,  range, JahSlidersLCD[1],
                        JahSlidersLabel[1], JahSlidersLabelText[1], 9);

    connect( JahSliders[1],   SIGNAL(valueChanged(int)), JahSlidersLCD[1], SLOT(setValue(int)) );
    connect(JahSliders[1], SIGNAL(valueChanged(int)), this, SLOT(setYRotation(int)) );
    connect(JahSlidersLCD[1], SIGNAL(valueChanged(int)), this, SLOT(setYRotation(int)) );
    connect(JahSlidersLCD[1], SIGNAL(valueChanged(int)), JahSliders[1], SLOT(setValue(int)) );

    JahSliders[2] = new QSlider( theuiframe );
    JahSlidersLCD[2] = new InputLCD( theuiframe );
    JahSlidersLabel[2] = new QLabel ( theuiframe);
    JahSlidersLabelText[2]="Z Rotation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[2], 0, y_position, 10, 21,  range, JahSlidersLCD[2],
                        JahSlidersLabel[2], JahSlidersLabelText[2], 9);
    connect( JahSliders[2],   SIGNAL(valueChanged(int)), JahSlidersLCD[2], SLOT(setValue(int)) );
    connect(JahSliders[2], SIGNAL(valueChanged(int)), this, SLOT(setZRotation(int)) );
    connect(JahSlidersLCD[2], SIGNAL(valueChanged(int)), this, SLOT(setZRotation(int)) );
    connect(JahSlidersLCD[2], SIGNAL(valueChanged(int)), JahSliders[2], SLOT(setValue(int)) );


    //////////////////////////////////////////////////////////////////////////////////////////
    // set up translate sliders

    //JahModuleLabel[1] = new QLabel( theuiframe, "TranslateText_1" );
    //JahModuleLabelText[1] = jt->tr("TRANSLATE");

    //JahFormatter::addLabel( JahModuleLabel[1], 180, 10, 110, 21, JahModuleLabelText[1], 9);

    JahSliders[3] = new QSlider( theuiframe );
    JahSlidersLCD[3] = new InputLCD( theuiframe );
    JahSlidersLabel[3] = new QLabel ( theuiframe);
    JahSlidersLabelText[3]="X Translation";

    y_position = start_y_position;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[3], 180, y_position, 10, 21,  range, JahSlidersLCD[3],
                        JahSlidersLabel[3], JahSlidersLabelText[3], 9);

    connect( JahSliders[3],   SIGNAL(valueChanged(int)), JahSlidersLCD[3], SLOT(setValue(int)) );
    connect( JahSliders[3],   SIGNAL(valueChanged(int)), this, SLOT(setXTrans(int)) );
    connect( JahSlidersLCD[3],   SIGNAL(valueChanged(int)), this, SLOT(setXTrans(int)) );
    connect(JahSlidersLCD[3], SIGNAL(valueChanged(int)), JahSliders[3], SLOT(setValue(int)) );

    JahSliders[4] = new QSlider( theuiframe );
    JahSlidersLCD[4] = new InputLCD( theuiframe );
    JahSlidersLabel[4] = new QLabel ( theuiframe);
    JahSlidersLabelText[4]="Y Translation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[4], 180, y_position, 10, 21,  range, JahSlidersLCD[4],
                        JahSlidersLabel[4], JahSlidersLabelText[4], 9);

    connect( JahSliders[4],   SIGNAL(valueChanged(int)), JahSlidersLCD[4], SLOT(setValue(int)) );
    connect(JahSliders[4], SIGNAL(valueChanged(int)), this, SLOT(setYTrans(int)) );
    connect(JahSlidersLCD[4], SIGNAL(valueChanged(int)), this, SLOT(setYTrans(int)) );
    connect(JahSlidersLCD[4], SIGNAL(valueChanged(int)), JahSliders[4], SLOT(setValue(int)) );

    JahSliders[5] = new QSlider( theuiframe );
    JahSlidersLCD[5] = new InputLCD( theuiframe );
    JahSlidersLabel[5] = new QLabel ( theuiframe);
    JahSlidersLabelText[5]="Z Translation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[5], 180, y_position, 10, 21,  range, JahSlidersLCD[5],
                        JahSlidersLabel[5], JahSlidersLabelText[4], 9);

    connect( JahSliders[5],   SIGNAL(valueChanged(int)), JahSlidersLCD[5], SLOT(setValue(int)) );
    connect(JahSliders[5], SIGNAL(valueChanged(int)), this, SLOT(setZTrans(int)) );
    connect(JahSlidersLCD[5], SIGNAL(valueChanged(int)), this, SLOT(setZTrans(int)) );
    connect(JahSlidersLCD[5], SIGNAL(valueChanged(int)), JahSliders[5], SLOT(setValue(int)) );


    //////////////////////////////////////////////////////////////////////////////////////////
    // set up scale sliders

    //JahModuleLabel[2] = new QLabel( theuiframe, "ScaleText_1" );
    //JahModuleLabelText[2] = jt->tr("SCALE");

    //JahFormatter::addLabel( JahModuleLabel[2], 350, 10, 110, 21, JahModuleLabelText[2], 9);

    JahSliders[6] = new QSlider( theuiframe );
    JahSlidersLCD[6] = new InputLCD( theuiframe );
    JahSlidersLabel[6] = new QLabel ( theuiframe);
    JahSlidersLabelText[6]="X Scale";

    y_position = start_y_position;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[6], 350, y_position, 10, 21,  range, JahSlidersLCD[6],
                        JahSlidersLabel[6],  JahSlidersLabelText[6], 9);

    connect( JahSliders[6],   SIGNAL(valueChanged(int)), JahSlidersLCD[6], SLOT(setValue(int)) );
    connect(JahSliders[6], SIGNAL(valueChanged(int)), this, SLOT(setXScale(int)) );
    connect(JahSlidersLCD[6], SIGNAL(valueChanged(int)), this, SLOT(setXScale(int)) );
    connect(JahSlidersLCD[6], SIGNAL(valueChanged(int)), JahSliders[6], SLOT(setValue(int)) );

    JahSliders[7] = new QSlider( theuiframe );
    JahSlidersLCD[7] = new InputLCD( theuiframe );
    JahSlidersLabel[7] = new QLabel ( theuiframe);
    JahSlidersLabelText[7]="Y Scale";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[7], 350, y_position, 10, 21,  range, JahSlidersLCD[7],
                        JahSlidersLabel[7], JahSlidersLabelText[7], 9);

    connect( JahSliders[7],   SIGNAL(valueChanged(int)), JahSlidersLCD[7], SLOT(setValue(int)) );
    connect(JahSliders[7], SIGNAL(valueChanged(int)), this, SLOT(setYScale(int)) );
    connect(JahSlidersLCD[7], SIGNAL(valueChanged(int)), this, SLOT(setYScale(int)) );
    connect(JahSlidersLCD[7], SIGNAL(valueChanged(int)), JahSliders[7], SLOT(setValue(int)) );

    JahSliders[8] = new QSlider( theuiframe );
    JahSlidersLCD[8] = new InputLCD( theuiframe );
    JahSlidersLabel[8] = new QLabel ( theuiframe);
    JahSlidersLabelText[8]="Z Scale";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[8], 350, y_position, 10, 21,  range, JahSlidersLCD[8],
                        JahSlidersLabel[8], JahSlidersLabelText[8], 9);

    connect( JahSliders[8],   SIGNAL(valueChanged(int)), JahSlidersLCD[8], SLOT(setValue(int)) );
    connect(JahSliders[8], SIGNAL(valueChanged(int)), this, SLOT(setZScale(int)) );
    connect(JahSlidersLCD[8], SIGNAL(valueChanged(int)), this, SLOT(setZScale(int)) );
    connect(JahSlidersLCD[8], SIGNAL(valueChanged(int)), JahSliders[8], SLOT(setValue(int)) );

    if (ModuleName == "Animation" || ModuleName == "Text")
    {
        connect( m_text_extrude_slider, SIGNAL(valueChanged(int)), JahSliders[8], SLOT( setValue(int) ) );
        connect( JahSliders[8], SIGNAL(valueChanged(int)), m_text_extrude_slider, SLOT( setValue(int) ) );
    }


    //needs a position locator as well...
    JahModuleLabelText[3] = jt->tr("TRANSPARENCY");

    //trans slider
    JahSliders[9] = new QSlider( theuiframe );
    JahSlidersLCD[9] = new InputLCD( theuiframe );
    JahSlidersLabel[9] = new QLabel ( theuiframe);
    JahSlidersLabelText[9]="Transparency";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, JahSliders[9], 180, y_position, 10, 21, 0, 100, JahSlidersLCD[9],
                        JahSlidersLabel[9], JahSlidersLabelText[9], 9);

    connect( JahSliders[9],   SIGNAL(valueChanged(int)), JahSlidersLCD[9], SLOT(setValue(int)) );
    connect(JahSliders[9], SIGNAL(valueChanged(int)), this, SLOT(setAlpha(int)) );
    connect(JahSlidersLCD[9], SIGNAL(valueChanged(int)), this, SLOT(setAlpha(int)) );
    connect(JahSlidersLCD[9], SIGNAL(valueChanged(int)), JahSliders[9], SLOT(setValue(int)) );

    for (int i = 0; i <= 9; i++)
    {
        connect(JahSlidersLCD[i], SIGNAL(valueChanged(int)), JahSliders[i], SLOT(setValue(int)) );
    }
}
