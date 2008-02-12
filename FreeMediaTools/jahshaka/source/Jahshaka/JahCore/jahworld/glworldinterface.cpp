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
#include <sliderbutton.h>

void GLWorld::buildController( QHBox* controller) 
{
    m_nAStartFrame=1;  m_nAEndFrame=100;  m_nAAnimFrame=1;

    QVBox* top = new QVBox( controller );

    // Container widget
    QWidget *transportContainer = new QWidget( top, "page" );
    QHBoxLayout *transportLayout = new QHBoxLayout( transportContainer );
    
    transportLayout->addStretch( 10 );
    
    // Transport buttons
    m_pScrubFirst = new JahToolButton( transportContainer, "controllerrewindbutton" );
    m_pScrubFirst->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubFirst, m_qsJahBasePath+"Pixmaps/player/rewind_off.png", m_qsJahBasePath+"Pixmaps/player/rewind_on.png" );
    connect( m_pScrubFirst,  SIGNAL(clicked()), SLOT(firstframeanimation())  );
    transportLayout->addWidget( m_pScrubFirst );
    
    m_pScrubPrevious = new JahToolButton( transportContainer, "controllerpreviousbutton" );
    m_pScrubPrevious->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubPrevious, m_qsJahBasePath+"Pixmaps/player/frewind_off.png", m_qsJahBasePath+"Pixmaps/player/frewind_on.png" );
    connect( m_pScrubPrevious,  SIGNAL(clicked()), SLOT(previousframeanimation())  );
    transportLayout->addWidget( m_pScrubPrevious );
    
    m_pScrubStop = new JahToolButton( transportContainer, "controllerstopbutton" );
    m_pScrubStop->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubStop, m_qsJahBasePath+"Pixmaps/player/stop_off.png", m_qsJahBasePath+"Pixmaps/player/stop_on.png" );
    connect( m_pScrubStop,  SIGNAL(clicked()), SLOT(stopanimation())  );
    transportLayout->addWidget( m_pScrubStop );
    
    m_pScrubPlay = new JahToolButton( transportContainer, "play" );
    m_pScrubPlay->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubPlay, m_qsJahBasePath+"Pixmaps/player/play_off.png", m_qsJahBasePath+"Pixmaps/player/play_on.png" );
    connect( m_pScrubPlay,  SIGNAL(clicked()), SLOT(startanimation())  );
    transportLayout->addWidget( m_pScrubPlay );
    
    m_pScrubNext = new JahToolButton( transportContainer, "controllernextbutton" );
    m_pScrubNext->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubNext, m_qsJahBasePath+"Pixmaps/player/ffoward_off.png", m_qsJahBasePath+"Pixmaps/player/ffoward_on.png" );
    connect( m_pScrubNext,  SIGNAL(clicked()), SLOT(nextframeanimation())  );
    transportLayout->addWidget( m_pScrubNext );
    
    m_pScrubLast = new JahToolButton( transportContainer, "controllerffworardbutton" );
    m_pScrubLast->setFixedSize( 25, 29 );
    JahFormatter::addJahPlayerButton( m_pScrubLast, m_qsJahBasePath+"Pixmaps/player/foward_off.png",  m_qsJahBasePath+"Pixmaps/player/foward_on.png" );
    connect( m_pScrubLast,  SIGNAL(clicked()), SLOT(lastframeanimation())  );
    transportLayout->addWidget( m_pScrubLast );

    transportLayout->addSpacing( 10 );

    // Time code display
    m_pAnimFrameLCD = new SupergRangeControl( transportContainer, "timecodedisplay" );
    m_pAnimFrameLCD->setFormatter( TimecodeValueFormatter() );
    JahFormatter::configure( m_pAnimFrameLCD, 1, 1 );
    connect( m_pAnimFrameLCD,   SIGNAL(valueChanged(int)),	 SLOT(sliderselected()) );
    connect(m_pAnimFrameLCD, SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged( int )) );
    transportLayout->addWidget( m_pAnimFrameLCD );
    
#ifndef JAHPLAYER
    if ( usesKeyFrames( ) )
    {
        transportLayout->addStretch( 5 );

        m_pKeyBack = new JahToolButton( transportContainer, "controllerrewindbutton" );
        m_pKeyBack->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( m_pKeyBack, m_qsJahBasePath+"Pixmaps/player/m_pKeyBack_off.png", m_qsJahBasePath+"Pixmaps/player/m_pKeyBack_on.png" );
        connect( m_pKeyBack,  SIGNAL(clicked()), SLOT(prevKeyframe())  );
        transportLayout->addWidget( m_pKeyBack );
	
        m_pKeyAdd = new JahToolButton( transportContainer, "controllerstopbutton" );
        m_pKeyAdd->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( m_pKeyAdd, m_qsJahBasePath+"Pixmaps/player/keyplus_off.png", m_qsJahBasePath+"Pixmaps/player/keyplus_on.png" );
        connect( m_pKeyAdd,  SIGNAL(clicked()), SLOT(addKeyframe())  );
        transportLayout->addWidget( m_pKeyAdd );
	
        m_pKeyRemove = new JahToolButton( transportContainer, "play" );
        m_pKeyRemove->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( m_pKeyRemove, m_qsJahBasePath+"Pixmaps/player/keyminus_off.png", m_qsJahBasePath+"Pixmaps/player/keyminus_on.png" );
        connect( m_pKeyRemove,  SIGNAL(clicked()), SLOT(removeKeyframe())  );
        transportLayout->addWidget( m_pKeyRemove );
	
        m_pKeyNext = new JahToolButton( transportContainer, "controllerffworardbutton" );
        m_pKeyNext->setFixedSize( 25, 29 );
        JahFormatter::addJahPlayerButton( m_pKeyNext, m_qsJahBasePath+"Pixmaps/player/keyforward_off.png",  m_qsJahBasePath+"Pixmaps/player/keyforward_on.png" );
        connect( m_pKeyNext,  SIGNAL(clicked()), SLOT(nextKeyframe())  );
        transportLayout->addWidget( m_pKeyNext );
	
        transportLayout->addSpacing( 10 );
    }
    else
    {
        transportLayout->addStretch( 10 );
    }

    m_pGetButton = new QPushButton( transportContainer, "GetButton" );
    m_pGetButton->setFixedSize( 120, 20 );
    m_pGetButton->setText( m_pJt->tr( "&Get Desktop"  ) );
    connect( m_pGetButton,  SIGNAL(clicked()), SLOT( grabDesktop())  );
    transportLayout->addWidget( m_pGetButton );

#else
    transportLayout->addStretch();
#endif

    // Container widget
    QWidget* sliderContainer = new QWidget( top );
    QHBoxLayout* sliderLayout = new QHBoxLayout( sliderContainer );

    QPalette p = palette();
    JahFormatter::swap( p, QColorGroup::Shadow, QColorGroup::Background );
    
    m_pControllerStartFrameControl = new SupergRangeControl( sliderContainer );
    m_pControllerStartFrameControl->setFormatter( TimecodeValueFormatter() );
    m_pControllerStartFrameControl->setPalette( p );
    m_pControllerStartFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    JahFormatter::configure( m_pControllerStartFrameControl, m_nAStartFrame, m_nAStartFrame );
    sliderLayout->addWidget( m_pControllerStartFrameControl );

    // Slider
    m_pControllerSlider = new TimelineSlider( sliderContainer, "m_pControllerSlider" );
    m_pControllerSlider->setMinValue( m_nAStartFrame );
    m_pControllerSlider->setMaxValue( m_nAEndFrame );
    m_pControllerSlider->setValue   ( m_nAAnimFrame );
    sliderLayout->addWidget( m_pControllerSlider );

    m_pControllerEndFrameControl = new SupergRangeControl( sliderContainer );
    m_pControllerEndFrameControl->setFormatter( TimecodeValueFormatter() );
    m_pControllerEndFrameControl->setPalette( p );
    m_pControllerEndFrameControl->setSizePolicy( QSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred ) );
    JahFormatter::configure( m_pControllerEndFrameControl, m_nAEndFrame, m_nAStartFrame );
    sliderLayout->addWidget( m_pControllerEndFrameControl );

    connect( m_pControllerSlider, SIGNAL( minChanged( int ) ), m_pControllerStartFrameControl, SLOT( setValue( int ) ) );
    connect( m_pControllerSlider, SIGNAL( maxChanged( int ) ), m_pControllerEndFrameControl, SLOT( setValue( int ) ) );
    connect( m_pControllerStartFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( updatesliderStartframe( int ) ) );
    connect( m_pControllerEndFrameControl, SIGNAL( valueChanged( int ) ), this, SLOT( updatesliderEndframe( int ) ) );

    connect( m_pControllerSlider, SIGNAL( sliderMoved(int) ),  SLOT(updateAnimation(int)) );
    connect( m_pControllerSlider, SIGNAL( valueChanged(int) ), SLOT(sliderValueChanged(int)) );
    connect( m_pControllerSlider, SIGNAL( valueChanged(int) ), m_pAnimFrameLCD, SLOT(setValue(int)) );
    connect( m_pAnimFrameLCD,  SIGNAL( valueChanged(int) ), m_pControllerSlider, SLOT(setValue(int)) );
}

void GLWorld::updatesliderStartframe( int startframe ) 
{
    if (startframe > m_nMaxFrame)
    {
        startframe = m_nMaxFrame;
    }

    m_pControllerSlider->setMinValue( startframe );
    m_nMinFrame = startframe;
}

void GLWorld::updatesliderEndframe( int endframe )     
{	
	m_pControllerSlider->setMaxValue( endframe );
	m_nMaxFrame = endframe;
}

void GLWorld::createAxisPanel ( QFrame* theuiframe)
{
    //////////////////////////////////////////////////////////////////////////////////////////
    // set up rotate sliders
    //set some flags
    m_bHasLabels = true;
    m_bHasSliders = true;
    //JahTranslate& m_pJt = JahTranslate::getInstance();

    //JahModuleLabel[0] = new QLabel( theuiframe, "RotateText_1" );
    //m_aJahModuleLabelTexts[0] = m_pJt->tr("ROTATE");

    //JahFormatter::addLabel( JahModuleLabel[0], 10, 10, 110, 21, m_aJahModuleLabelTexts[0], 9);

    int y_displacement = 35;
    int start_y_position = 30;
    int y_position = start_y_position;
    int range = 2000;

    m_aJahSliders[0] = new QSlider( theuiframe );
    m_aJahSliderLCDs[0] = new InputLCD( theuiframe );
    m_aJahSliderLabels[0] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[0]="X Rotation";

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[0], 0, y_position, 10, 21,  range, m_aJahSliderLCDs[0],
                        m_aJahSliderLabels[0], m_aJahSliderLabelTexts[0], 9);

    connect( m_aJahSliders[0],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[0], SLOT(setValue(int)) );
    connect(m_aJahSliders[0], SIGNAL(valueChanged(int)), this, SLOT(setXRotation(int)) );
    connect(m_aJahSliderLCDs[0], SIGNAL(valueChanged(int)), this, SLOT(setXRotation(int)) );
    connect(m_aJahSliderLCDs[0], SIGNAL(valueChanged(int)), m_aJahSliders[0], SLOT(setValue(int)) );

    m_aJahSliders[1] = new QSlider( theuiframe );
    m_aJahSliderLCDs[1] = new InputLCD( theuiframe );
    m_aJahSliderLabels[1] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[1]="Y Rotation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[1], 0, y_position, 10, 21,  range, m_aJahSliderLCDs[1],
                        m_aJahSliderLabels[1], m_aJahSliderLabelTexts[1], 9);

    connect( m_aJahSliders[1],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[1], SLOT(setValue(int)) );
    connect(m_aJahSliders[1], SIGNAL(valueChanged(int)), this, SLOT(setYRotation(int)) );
    connect(m_aJahSliderLCDs[1], SIGNAL(valueChanged(int)), this, SLOT(setYRotation(int)) );
    connect(m_aJahSliderLCDs[1], SIGNAL(valueChanged(int)), m_aJahSliders[1], SLOT(setValue(int)) );

    m_aJahSliders[2] = new QSlider( theuiframe );
    m_aJahSliderLCDs[2] = new InputLCD( theuiframe );
    m_aJahSliderLabels[2] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[2]="Z Rotation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[2], 0, y_position, 10, 21,  range, m_aJahSliderLCDs[2],
                        m_aJahSliderLabels[2], m_aJahSliderLabelTexts[2], 9);
    connect( m_aJahSliders[2],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[2], SLOT(setValue(int)) );
    connect(m_aJahSliders[2], SIGNAL(valueChanged(int)), this, SLOT(setZRotation(int)) );
    connect(m_aJahSliderLCDs[2], SIGNAL(valueChanged(int)), this, SLOT(setZRotation(int)) );
    connect(m_aJahSliderLCDs[2], SIGNAL(valueChanged(int)), m_aJahSliders[2], SLOT(setValue(int)) );


    //////////////////////////////////////////////////////////////////////////////////////////
    // set up translate sliders

    //JahModuleLabel[1] = new QLabel( theuiframe, "TranslateText_1" );
    //m_aJahModuleLabelTexts[1] = m_pJt->tr("TRANSLATE");

    //JahFormatter::addLabel( JahModuleLabel[1], 180, 10, 110, 21, m_aJahModuleLabelTexts[1], 9);

    m_aJahSliders[3] = new QSlider( theuiframe );
    m_aJahSliderLCDs[3] = new InputLCD( theuiframe );
    m_aJahSliderLabels[3] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[3]="X Translation";

    y_position = start_y_position;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[3], 180, y_position, 10, 21,  range, m_aJahSliderLCDs[3],
                        m_aJahSliderLabels[3], m_aJahSliderLabelTexts[3], 9);

    connect( m_aJahSliders[3],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[3], SLOT(setValue(int)) );
    connect( m_aJahSliders[3],   SIGNAL(valueChanged(int)), this, SLOT(setXTrans(int)) );
    connect( m_aJahSliderLCDs[3],   SIGNAL(valueChanged(int)), this, SLOT(setXTrans(int)) );
    connect(m_aJahSliderLCDs[3], SIGNAL(valueChanged(int)), m_aJahSliders[3], SLOT(setValue(int)) );

    m_aJahSliders[4] = new QSlider( theuiframe );
    m_aJahSliderLCDs[4] = new InputLCD( theuiframe );
    m_aJahSliderLabels[4] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[4]="Y Translation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[4], 180, y_position, 10, 21,  range, m_aJahSliderLCDs[4],
                        m_aJahSliderLabels[4], m_aJahSliderLabelTexts[4], 9);

    connect( m_aJahSliders[4],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[4], SLOT(setValue(int)) );
    connect(m_aJahSliders[4], SIGNAL(valueChanged(int)), this, SLOT(setYTrans(int)) );
    connect(m_aJahSliderLCDs[4], SIGNAL(valueChanged(int)), this, SLOT(setYTrans(int)) );
    connect(m_aJahSliderLCDs[4], SIGNAL(valueChanged(int)), m_aJahSliders[4], SLOT(setValue(int)) );

    m_aJahSliders[5] = new QSlider( theuiframe );
    m_aJahSliderLCDs[5] = new InputLCD( theuiframe );
    m_aJahSliderLabels[5] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[5]="Z Translation";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[5], 180, y_position, 10, 21,  range, m_aJahSliderLCDs[5],
                        m_aJahSliderLabels[5], m_aJahSliderLabelTexts[4], 9);

    connect( m_aJahSliders[5],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[5], SLOT(setValue(int)) );
    connect(m_aJahSliders[5], SIGNAL(valueChanged(int)), this, SLOT(setZTrans(int)) );
    connect(m_aJahSliderLCDs[5], SIGNAL(valueChanged(int)), this, SLOT(setZTrans(int)) );
    connect(m_aJahSliderLCDs[5], SIGNAL(valueChanged(int)), m_aJahSliders[5], SLOT(setValue(int)) );


    //////////////////////////////////////////////////////////////////////////////////////////
    // set up scale sliders

    //JahModuleLabel[2] = new QLabel( theuiframe, "ScaleText_1" );
    //m_aJahModuleLabelTexts[2] = m_pJt->tr("SCALE");

    //JahFormatter::addLabel( JahModuleLabel[2], 350, 10, 110, 21, m_aJahModuleLabelTexts[2], 9);

    m_aJahSliders[6] = new QSlider( theuiframe );
    m_aJahSliderLCDs[6] = new InputLCD( theuiframe );
    m_aJahSliderLabels[6] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[6]="X Scale";

    y_position = start_y_position;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[6], 350, y_position, 10, 21,  range, m_aJahSliderLCDs[6],
                        m_aJahSliderLabels[6],  m_aJahSliderLabelTexts[6], 9);

    connect( m_aJahSliders[6],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[6], SLOT(setValue(int)) );
    connect(m_aJahSliders[6], SIGNAL(valueChanged(int)), this, SLOT(setXScale(int)) );
    connect(m_aJahSliderLCDs[6], SIGNAL(valueChanged(int)), this, SLOT(setXScale(int)) );
    connect(m_aJahSliderLCDs[6], SIGNAL(valueChanged(int)), m_aJahSliders[6], SLOT(setValue(int)) );

    m_aJahSliders[7] = new QSlider( theuiframe );
    m_aJahSliderLCDs[7] = new InputLCD( theuiframe );
    m_aJahSliderLabels[7] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[7]="Y Scale";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[7], 350, y_position, 10, 21,  range, m_aJahSliderLCDs[7],
                        m_aJahSliderLabels[7], m_aJahSliderLabelTexts[7], 9);

    connect( m_aJahSliders[7],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[7], SLOT(setValue(int)) );
    connect(m_aJahSliders[7], SIGNAL(valueChanged(int)), this, SLOT(setYScale(int)) );
    connect(m_aJahSliderLCDs[7], SIGNAL(valueChanged(int)), this, SLOT(setYScale(int)) );
    connect(m_aJahSliderLCDs[7], SIGNAL(valueChanged(int)), m_aJahSliders[7], SLOT(setValue(int)) );

    m_aJahSliders[8] = new QSlider( theuiframe );
    m_aJahSliderLCDs[8] = new InputLCD( theuiframe );
    m_aJahSliderLabels[8] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[8]="Z Scale";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[8], 350, y_position, 10, 21,  range, m_aJahSliderLCDs[8],
                        m_aJahSliderLabels[8], m_aJahSliderLabelTexts[8], 9);

    connect( m_aJahSliders[8],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[8], SLOT(setValue(int)) );
    connect(m_aJahSliders[8], SIGNAL(valueChanged(int)), this, SLOT(setZScale(int)) );
    connect(m_aJahSliderLCDs[8], SIGNAL(valueChanged(int)), this, SLOT(setZScale(int)) );
    connect(m_aJahSliderLCDs[8], SIGNAL(valueChanged(int)), m_aJahSliders[8], SLOT(setValue(int)) );

    if (m_qsModuleName == "Animation" || m_qsModuleName == "Text")
    {
        connect( m_pTextExtrudeSlider,
		 SIGNAL(valueChanged(int)), m_aJahSliders[8], SLOT( setValue(int) ) );
        connect( m_aJahSliders[8], SIGNAL(valueChanged(int)), 
		 m_pTextExtrudeSlider, SLOT( setValue(int) ) );
    }


    //needs a position locator as well...
    m_aJahModuleLabelTexts[3] = m_pJt->tr("TRANSPARENCY");

    //trans slider
    m_aJahSliders[9] = new QSlider( theuiframe );
    m_aJahSliderLCDs[9] = new InputLCD( theuiframe );
    m_aJahSliderLabels[9] = new QLabel ( theuiframe);
    m_aJahSliderLabelTexts[9]="Transparency";

    y_position += y_displacement;

    JahFormatter::addJahSliderGroup(  theuiframe, m_aJahSliders[9], 180, y_position, 10, 21, 0, 100, m_aJahSliderLCDs[9],
                        m_aJahSliderLabels[9], m_aJahSliderLabelTexts[9], 9);

    connect( m_aJahSliders[9],   SIGNAL(valueChanged(int)), m_aJahSliderLCDs[9], SLOT(setValue(int)) );
    connect(m_aJahSliders[9], SIGNAL(valueChanged(int)), this, SLOT(setAlpha(int)) );
    connect(m_aJahSliderLCDs[9], SIGNAL(valueChanged(int)), this, SLOT(setAlpha(int)) );
    connect(m_aJahSliderLCDs[9], SIGNAL(valueChanged(int)), m_aJahSliders[9], SLOT(setValue(int)) );

    for (int i = 0; i <= 9; i++)
    {
        connect(m_aJahSliderLCDs[i], SIGNAL(valueChanged(int)), m_aJahSliders[i], SLOT(setValue(int)) );
    }
}
