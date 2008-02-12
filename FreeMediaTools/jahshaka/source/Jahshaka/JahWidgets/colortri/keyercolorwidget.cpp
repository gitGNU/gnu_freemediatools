/****************************************************************************
** Form implementation generated from reading ui file 'keyercolorwidget.ui'
**
** Created: Wed 7. Sep 11:57:42 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "keyercolorwidget.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qslider.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qspinbox.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "qtcolortriangle.h"
#include "colordropper.h"
#include "keyercolorwidget.ui.h"

/*
 *  Constructs a KeyerColorWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
KeyerColorWidget::KeyerColorWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "KeyerColorWidget" );
    KeyerColorWidgetLayout = new QGridLayout( this, 1, 1, 11, 6, "KeyerColorWidgetLayout"); 

    pushButtonHSL = new QPushButton( this, "pushButtonHSL" );

    KeyerColorWidgetLayout->addWidget( pushButtonHSL, 0, 0 );

    mainWidgetStack = new QWidgetStack( this, "mainWidgetStack" );
    mainWidgetStack->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 1, mainWidgetStack->sizePolicy().hasHeightForWidth() ) );

    WStackPage = new QWidget( mainWidgetStack, "WStackPage" );
    WStackPageLayout = new QGridLayout( WStackPage, 1, 1, 0, 6, "WStackPageLayout"); 

    layout5 = new QGridLayout( 0, 1, 1, 0, 6, "layout5"); 

    sliderV = new QSlider( WStackPage, "sliderV" );
    sliderV->setMaxValue( 127 );
    sliderV->setMinValue( 1 );
    sliderV->setValue( 64 );
    sliderV->setOrientation( QSlider::Horizontal );

    layout5->addWidget( sliderV, 2, 1 );

    colorBarLabelMin = new QLabel( WStackPage, "colorBarLabelMin" );
    colorBarLabelMin->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 1, colorBarLabelMin->sizePolicy().hasHeightForWidth() ) );
    colorBarLabelMin->setMinimumSize( QSize( 50, 0 ) );
    colorBarLabelMin->setPaletteBackgroundColor( QColor( 205, 236, 235 ) );
    colorBarLabelMin->setFrameShape( QLabel::Panel );
    colorBarLabelMin->setFrameShadow( QLabel::Raised );
    colorBarLabelMin->setAlignment( int( QLabel::AlignCenter ) );

    layout5->addMultiCellWidget( colorBarLabelMin, 0, 2, 0, 0 );

    sliderH = new QSlider( WStackPage, "sliderH" );
    sliderH->setMaxValue( 90 );
    sliderH->setMinValue( 1 );
    sliderH->setValue( 45 );
    sliderH->setOrientation( QSlider::Horizontal );

    layout5->addWidget( sliderH, 0, 1 );

    sliderS = new QSlider( WStackPage, "sliderS" );
    sliderS->setMaxValue( 127 );
    sliderS->setMinValue( 1 );
    sliderS->setValue( 64 );
    sliderS->setOrientation( QSlider::Horizontal );

    layout5->addWidget( sliderS, 1, 1 );

    colorBarLabelMax = new QLabel( WStackPage, "colorBarLabelMax" );
    colorBarLabelMax->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 1, colorBarLabelMax->sizePolicy().hasHeightForWidth() ) );
    colorBarLabelMax->setMinimumSize( QSize( 50, 0 ) );
    colorBarLabelMax->setPaletteBackgroundColor( QColor( 236, 175, 198 ) );
    colorBarLabelMax->setFrameShape( QLabel::Panel );
    colorBarLabelMax->setFrameShadow( QLabel::Raised );
    colorBarLabelMax->setAlignment( int( QLabel::AlignCenter ) );

    layout5->addMultiCellWidget( colorBarLabelMax, 0, 2, 2, 2 );

    WStackPageLayout->addMultiCellLayout( layout5, 2, 3, 1, 1 );

    softenSlider = new QSlider( WStackPage, "softenSlider" );
    softenSlider->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, softenSlider->sizePolicy().hasHeightForWidth() ) );
    softenSlider->setMaxValue( 255 );
    softenSlider->setOrientation( QSlider::Horizontal );
    softenSlider->setTickmarks( QSlider::Below );

    WStackPageLayout->addWidget( softenSlider, 2, 2 );

    showAlphaCheckBox = new QCheckBox( WStackPage, "showAlphaCheckBox" );
    WStackPageLayout->addWidget( showAlphaCheckBox, 3, 2 );

    selectColorsIndividuallyCheckBox = new QCheckBox( WStackPage, "selectColorsIndividuallyCheckBox" );
    WStackPageLayout->addWidget( selectColorsIndividuallyCheckBox, 4, 2 );


    textLabel5_2 = new QLabel( WStackPage, "textLabel5_2" );
    textLabel5_2->setAlignment( int( QLabel::AlignCenter ) );

    WStackPageLayout->addWidget( textLabel5_2, 1, 1 );

    textLabel5_3 = new QLabel( WStackPage, "textLabel5_3" );
    textLabel5_3->setAlignment( int( QLabel::AlignCenter ) );

    WStackPageLayout->addWidget( textLabel5_3, 1, 2 );

    layout6 = new QGridLayout( 0, 1, 1, 0, 6, "layout6"); 

    qtColorTriangle = new QtColorTriangle( WStackPage, "qtColorTriangle" );
    qtColorTriangle->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 4, 4, qtColorTriangle->sizePolicy().hasHeightForWidth() ) );
    qtColorTriangle->setMinimumSize( QSize( 50, 50 ) );

    layout6->addWidget( qtColorTriangle, 1, 0 );

    textLabel5 = new QLabel( WStackPage, "textLabel5" );
    textLabel5->setAlignment( int( QLabel::AlignCenter ) );

    layout6->addMultiCellWidget( textLabel5, 0, 0, 0, 1 );

    layout1 = new QGridLayout( 0, 1, 1, 0, 6, "layout1"); 

    spinBoxV = new QSpinBox( WStackPage, "spinBoxV" );
    spinBoxV->setMaxValue( 255 );

    layout1->addWidget( spinBoxV, 2, 1 );

    textLabelH = new QLabel( WStackPage, "textLabelH" );

    layout1->addWidget( textLabelH, 0, 0 );

    textLabelS = new QLabel( WStackPage, "textLabelS" );

    layout1->addWidget( textLabelS, 1, 0 );

    spinBoxS = new QSpinBox( WStackPage, "spinBoxS" );
    spinBoxS->setMaxValue( 255 );

    layout1->addWidget( spinBoxS, 1, 1 );

    spinBoxH = new QSpinBox( WStackPage, "spinBoxH" );
    spinBoxH->setMaxValue( 360 );
    spinBoxH->setMinValue( 0 );

    layout1->addWidget( spinBoxH, 0, 1 );

    textLabelV = new QLabel( WStackPage, "textLabelV" );

    layout1->addWidget( textLabelV, 2, 0 );

    layout6->addLayout( layout1, 1, 1 );

    WStackPageLayout->addMultiCellLayout( layout6, 0, 4, 0, 0 );
    mainWidgetStack->addWidget( WStackPage, 0 );

    WStackPage_2 = new QWidget( mainWidgetStack, "WStackPage_2" );
    WStackPageLayout_2 = new QGridLayout( WStackPage_2, 1, 1, 11, 6, "WStackPageLayout_2"); 

    textLabel4 = new QLabel( WStackPage_2, "textLabel4" );

    WStackPageLayout_2->addWidget( textLabel4, 0, 0 );
    spacer2 = new QSpacerItem( 20, 91, QSizePolicy::Minimum, QSizePolicy::Expanding );
    WStackPageLayout_2->addItem( spacer2, 1, 0 );
    mainWidgetStack->addWidget( WStackPage_2, 1 );

    KeyerColorWidgetLayout->addMultiCellWidget( mainWidgetStack, 0, 3, 1, 1 );

    pushButtonRGB = new QPushButton( this, "pushButtonRGB" );
    pushButtonRGB->setEnabled( FALSE );

    KeyerColorWidgetLayout->addWidget( pushButtonRGB, 1, 0 );
    spacer1 = new QSpacerItem( 21, 20, QSizePolicy::Minimum, QSizePolicy::Expanding );
    KeyerColorWidgetLayout->addItem( spacer1, 3, 0 );

    colorDropper = new QPushButton( this, "colorDropper" );
    colorDropper->setPixmap( ColorDropper::colorDropperImage() );

    KeyerColorWidgetLayout->addWidget( colorDropper, 2, 0 );
    languageChange();
    resize( QSize(620, 178).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( pushButtonHSL, SIGNAL( clicked() ), this, SLOT( buttonPressHSL() ) );
    connect( pushButtonRGB, SIGNAL( clicked() ), this, SLOT( buttonPressRGB() ) );
    connect( sliderH, SIGNAL( valueChanged(int) ), this, SLOT( sliderValueChanged(int) ) );
    connect( sliderS, SIGNAL( valueChanged(int) ), this, SLOT( sliderValueChanged(int) ) );
    connect( sliderV, SIGNAL( valueChanged(int) ), this, SLOT( sliderValueChanged(int) ) );
    connect( spinBoxH, SIGNAL( valueChanged(int) ), this, SLOT( spinBoxChanged(int) ) );
    connect( spinBoxS, SIGNAL( valueChanged(int) ), this, SLOT( spinBoxChanged(int) ) );
    connect( spinBoxV, SIGNAL( valueChanged(int) ), this, SLOT( spinBoxChanged(int) ) );
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
KeyerColorWidget::~KeyerColorWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void KeyerColorWidget::languageChange()
{
    setCaption( tr( "KeyerColorWidget" ) );
    pushButtonHSL->setText( tr( "Use HSL" ) );
    QToolTip::add( sliderV, tr( "Value min/max." ) );
    colorBarLabelMin->setText( tr( "Min" ) );
    QToolTip::add( colorBarLabelMin, tr( "Color at the lower limit, based on HSV sliders and current color." ) );
    QToolTip::add( sliderH, tr( "Hue min/max." ) );
    QToolTip::add( sliderS, tr( "Saturation min/max." ) );
    colorBarLabelMax->setText( tr( "Max" ) );
    QToolTip::add( colorBarLabelMax, tr( "Color at the upper limit, based on HSV sliders and current color." ) );
    QToolTip::add( softenSlider, tr( "Percentage soften." ) );
    showAlphaCheckBox->setText( tr( "Show alpha" ) );
    selectColorsIndividuallyCheckBox->setText( tr( "Select colors individually" ) );
    textLabel5_2->setText( tr( "Color Range" ) );
    textLabel5_3->setText( tr( "Soften" ) );
    textLabel5->setText( tr( "Source color" ) );
    textLabelH->setText( tr( "H" ) );
    textLabelS->setText( tr( "S" ) );
    textLabelV->setText( tr( "V" ) );
    textLabel4->setText( tr( "(not implemeted yet)" ) );
    pushButtonRGB->setText( tr( "Use RGB" ) );
    QToolTip::add( colorDropper, tr( "Color dropper" ) );
}

