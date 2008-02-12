/****************************************************************************
** Form implementation generated from reading ui file 'colorcorrectorwidget.ui'
**
** Created: Mon 1. Aug 14:46:01 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "colorcorrectorwidget.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qslider.h>
#include <qlabel.h>
#include <qframe.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "qtcolortriangle.h"
#include "colorcorrectorwidget.ui.h"

/*
 *  Constructs a ColorCorrectorWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
ColorCorrectorWidget::ColorCorrectorWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "ColorCorrectorWidget" );
    ColorCorrectorWidgetLayout = new QGridLayout( this, 1, 1, 11, 6, "ColorCorrectorWidgetLayout"); 

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 11 );
    buttonGroup1Layout = new QVBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    radioButtonMaster = new QRadioButton( buttonGroup1, "radioButtonMaster" );
    radioButtonMaster->setChecked( TRUE );
    buttonGroup1Layout->addWidget( radioButtonMaster );

    radioButtonShadows = new QRadioButton( buttonGroup1, "radioButtonShadows" );
    radioButtonShadows->setEnabled( FALSE );
    buttonGroup1Layout->addWidget( radioButtonShadows );

    radioButtonMidtones = new QRadioButton( buttonGroup1, "radioButtonMidtones" );
    radioButtonMidtones->setEnabled( FALSE );
    buttonGroup1Layout->addWidget( radioButtonMidtones );

    radioButtonHighlights = new QRadioButton( buttonGroup1, "radioButtonHighlights" );
    radioButtonHighlights->setEnabled( FALSE );
    buttonGroup1Layout->addWidget( radioButtonHighlights );

    ColorCorrectorWidgetLayout->addMultiCellWidget( buttonGroup1, 0, 1, 0, 0 );

    qtColorTriangle = new QtColorTriangle( this, "qtColorTriangle" );
    qtColorTriangle->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 4, 4, qtColorTriangle->sizePolicy().hasHeightForWidth() ) );
    qtColorTriangle->setMinimumSize( QSize( 120, 120 ) );

    ColorCorrectorWidgetLayout->addMultiCellWidget( qtColorTriangle, 0, 1, 1, 1 );

    layout3 = new QHBoxLayout( 0, 0, 6, "layout3"); 
    spacer10 = new QSpacerItem( 20, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    layout3->addItem( spacer10 );

    resetButton = new QPushButton( this, "resetButton" );
    layout3->addWidget( resetButton );

    ColorCorrectorWidgetLayout->addLayout( layout3, 1, 2 );

    layout5 = new QGridLayout( 0, 1, 1, 0, 6, "layout5"); 

    redSlider = new QSlider( this, "redSlider" );
    redSlider->setMinValue( -500 );
    redSlider->setMaxValue( 500 );
    redSlider->setOrientation( QSlider::Horizontal );

    layout5->addWidget( redSlider, 0, 0 );

    textLabel2_5 = new QLabel( this, "textLabel2_5" );

    layout5->addWidget( textLabel2_5, 1, 5 );

    blueSlider = new QSlider( this, "blueSlider" );
    blueSlider->setMinValue( -500 );
    blueSlider->setMaxValue( 500 );
    blueSlider->setOrientation( QSlider::Horizontal );

    layout5->addWidget( blueSlider, 2, 0 );

    textLabel1_4 = new QLabel( this, "textLabel1_4" );

    layout5->addWidget( textLabel1_4, 0, 5 );

    textLabel1_2_2 = new QLabel( this, "textLabel1_2_2" );

    layout5->addWidget( textLabel1_2_2, 1, 1 );

    textLabel1_2_3 = new QLabel( this, "textLabel1_2_3" );

    layout5->addWidget( textLabel1_2_3, 2, 1 );

    textLabel1_2 = new QLabel( this, "textLabel1_2" );

    layout5->addWidget( textLabel1_2, 0, 1 );

    textLabel1_3 = new QLabel( this, "textLabel1_3" );

    layout5->addWidget( textLabel1_3, 0, 3 );

    hSlider = new QSlider( this, "hSlider" );
    hSlider->setMinValue( -500 );
    hSlider->setMaxValue( 500 );
    hSlider->setLineStep( 10 );
    hSlider->setPageStep( 50 );
    hSlider->setOrientation( QSlider::Horizontal );
    hSlider->setTickmarks( QSlider::NoMarks );
    hSlider->setTickInterval( 30 );

    layout5->addWidget( hSlider, 0, 4 );

    line1 = new QFrame( this, "line1" );
    line1->setFrameShape( QFrame::HLine );
    line1->setFrameShadow( QFrame::Sunken );
    line1->setFrameShape( QFrame::HLine );

    layout5->addMultiCellWidget( line1, 3, 3, 0, 4 );

    vSlider = new QSlider( this, "vSlider" );
    vSlider->setMinValue( -500 );
    vSlider->setMaxValue( 500 );
    vSlider->setLineStep( 10 );
    vSlider->setPageStep( 50 );
    vSlider->setValue( 0 );
    vSlider->setOrientation( QSlider::Horizontal );
    vSlider->setTickmarks( QSlider::NoMarks );
    vSlider->setTickInterval( 30 );

    layout5->addWidget( vSlider, 2, 4 );

    brightnessSlider = new QSlider( this, "brightnessSlider" );
    brightnessSlider->setMinValue( -500 );
    brightnessSlider->setMaxValue( 500 );
    brightnessSlider->setOrientation( QSlider::Horizontal );

    layout5->addWidget( brightnessSlider, 2, 2 );

    sSlider = new QSlider( this, "sSlider" );
    sSlider->setMinValue( -500 );
    sSlider->setMaxValue( 500 );
    sSlider->setLineStep( 10 );
    sSlider->setPageStep( 50 );
    sSlider->setValue( 0 );
    sSlider->setOrientation( QSlider::Horizontal );
    sSlider->setTickmarks( QSlider::NoMarks );
    sSlider->setTickInterval( 30 );

    layout5->addWidget( sSlider, 1, 4 );

    textLabel2_4 = new QLabel( this, "textLabel2_4" );

    layout5->addWidget( textLabel2_4, 1, 3 );

    tintSlider = new QSlider( this, "tintSlider" );
    tintSlider->setMinValue( -500 );
    tintSlider->setMaxValue( 500 );
    tintSlider->setOrientation( QSlider::Horizontal );

    layout5->addWidget( tintSlider, 1, 2 );

    strengthSlider = new QSlider( this, "strengthSlider" );
    strengthSlider->setEnabled( TRUE );
    strengthSlider->setMinValue( -500 );
    strengthSlider->setMaxValue( 500 );
    strengthSlider->setOrientation( QSlider::Horizontal );

    layout5->addWidget( strengthSlider, 0, 2 );

    textLabel2_3_3 = new QLabel( this, "textLabel2_3_3" );

    layout5->addWidget( textLabel2_3_3, 2, 5 );



    greenSlider = new QSlider( this, "greenSlider" );
    greenSlider->setMinValue( -500 );
    greenSlider->setMaxValue( 500 );
    greenSlider->setOrientation( QSlider::Horizontal );
    layout5->addWidget( greenSlider, 1, 0 );
    textLabel2_3_2 = new QLabel( this, "textLabel2_3_2" );
    layout5->addWidget( textLabel2_3_2, 2, 3 );

    gammaSlider = new QSlider( this, "gammaSlider" );
    gammaSlider->setMinValue( 1 );
    gammaSlider->setMaxValue( 500 );
    gammaSlider->setValue(100);
    gammaSlider->setOrientation( QSlider::Horizontal );
    layout5->addWidget( gammaSlider, 4, 0 );
    gammaLabel = new QLabel( this, "gammaLabel" );
    layout5->addWidget( gammaLabel, 4, 1 );

    ColorCorrectorWidgetLayout->addLayout( layout5, 0, 2 );
    languageChange();
    resize( QSize(626, 164).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( resetButton, SIGNAL( clicked() ), this, SLOT( resetSliders() ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
ColorCorrectorWidget::~ColorCorrectorWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void ColorCorrectorWidget::languageChange()
{
    setCaption( tr( "ColorCorrectorWidget" ) );
    buttonGroup1->setTitle( tr( "Range" ) );
    radioButtonMaster->setText( tr( "Master" ) );
    radioButtonShadows->setText( tr( "Shadows" ) );
    radioButtonMidtones->setText( tr( "Midtones" ) );
    radioButtonHighlights->setText( tr( "Highlights" ) );
    resetButton->setText( tr( "Reset" ) );
    textLabel2_5->setText( tr( "S" ) );
    textLabel1_4->setText( tr( "H" ) );
    textLabel1_2_2->setText( tr( "G" ) );
    textLabel1_2_3->setText( tr( "B" ) );
    textLabel1_2->setText( tr( "R" ) );
    textLabel1_3->setText( tr( "Str" ) );
    textLabel2_4->setText( tr( "Tint" ) );
    textLabel2_3_3->setText( tr( "V" ) );
    textLabel2_3_2->setText( tr( "Br" ) );
    gammaLabel->setText("Gamma");
}

