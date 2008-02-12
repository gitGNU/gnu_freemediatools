/****************************************************************************
** Form implementation generated from reading ui file 'levelswidget.ui'
**
** Created: Thu 15. Sep 16:49:26 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

// WARNING! THIS FILE OUT OF SYNCH WITH UI. You can still use uic to generate layout code, 
// but there are extra functions pasted in...

#include "levelswidget.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qgroupbox.h>
#include <qcheckbox.h>
#include <qcombobox.h>
#include <qbuttongroup.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "levelsplot.h"

/*
 *  Constructs a LevelsWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
LevelsWidget::LevelsWidget( QWidget* parent, const char* name, WFlags fl )
    : QWidget( parent, name, fl )
{
    if ( !name )
	setName( "LevelsWidget" );
    LevelsWidgetLayout = new QGridLayout( this, 1, 1, 6, 6, "LevelsWidgetLayout"); 

    levelsPlot = new LevelsPlot( this, "levelsPlot" );
    levelsPlot->setMinimumWidth( 180 );

    LevelsWidgetLayout->addWidget( levelsPlot, 2, 1 );
    spacer1_2 = new QSpacerItem( 5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    LevelsWidgetLayout->addItem( spacer1_2, 2, 0 );
    spacer6 = new QSpacerItem( 10, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LevelsWidgetLayout->addItem( spacer6, 4, 1 );
    spacer1 = new QSpacerItem( 5, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    LevelsWidgetLayout->addItem( spacer1, 2, 2 );
    spacer5 = new QSpacerItem( 10, 16, QSizePolicy::Minimum, QSizePolicy::Expanding );
    LevelsWidgetLayout->addItem( spacer5, 0, 1 );

    groupBox1 = new QGroupBox( this, "groupBox1" );
    groupBox1->setColumnLayout(0, Qt::Vertical );
    groupBox1->layout()->setSpacing( 6 );
    groupBox1->layout()->setMargin( 6 );
    groupBox1Layout = new QHBoxLayout( groupBox1->layout() );
    groupBox1Layout->setAlignment( Qt::AlignTop );

    realTimeCheckBox = new QCheckBox( groupBox1, "realTimeCheckBox" );
    realTimeCheckBox->setChecked( FALSE );
    groupBox1Layout->addWidget( realTimeCheckBox );
    spacer3 = new QSpacerItem( 10, 20, QSizePolicy::Expanding, QSizePolicy::Minimum );
    groupBox1Layout->addItem( spacer3 );

    levelComboBox = new QComboBox( FALSE, groupBox1, "levelComboBox" );
    groupBox1Layout->addWidget( levelComboBox );

    LevelsWidgetLayout->addWidget( groupBox1, 1, 1 );

    buttonGroup1 = new QButtonGroup( this, "buttonGroup1" );
    buttonGroup1->setColumnLayout(0, Qt::Vertical );
    buttonGroup1->layout()->setSpacing( 6 );
    buttonGroup1->layout()->setMargin( 6 );
    buttonGroup1Layout = new QHBoxLayout( buttonGroup1->layout() );
    buttonGroup1Layout->setAlignment( Qt::AlignTop );

    textLabel1 = new QLabel( buttonGroup1, "textLabel1" );
    buttonGroup1Layout->addWidget( textLabel1 );

    meanLabel = new QLabel( buttonGroup1, "meanLabel" );
    buttonGroup1Layout->addWidget( meanLabel );

    textLabel3 = new QLabel( buttonGroup1, "textLabel3" );
    buttonGroup1Layout->addWidget( textLabel3 );

    medianLabel = new QLabel( buttonGroup1, "medianLabel" );
    buttonGroup1Layout->addWidget( medianLabel );

    LevelsWidgetLayout->addMultiCellWidget( buttonGroup1, 3, 3, 1, 1 );
    languageChange();
    //resize( QSize(317, 248).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( levelComboBox, SIGNAL( activated(const QString&) ), this, SLOT( levelChooser(const QString&) ) );
	connect( realTimeCheckBox, SIGNAL( toggled( bool ) ), this, SLOT( realTimeToggled( bool ) ) );

    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
LevelsWidget::~LevelsWidget()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void LevelsWidget::languageChange()
{
    setCaption( tr( "LevelsWidget" ) );
    //groupBox1->setTitle( tr( "Levels" ) );
    realTimeCheckBox->setText( tr( "On" ) );
    levelComboBox->clear();
    levelComboBox->insertItem( tr( "Luminosity" ) );
    levelComboBox->insertItem( tr( "Reds" ) );
    levelComboBox->insertItem( tr( "Greens" ) );
    levelComboBox->insertItem( tr( "Blues" ) );
    levelComboBox->insertItem( tr( "Vertical" ) );
    levelComboBox->insertItem( tr( "Horizontal" ) );
    levelComboBox->insertItem( tr( "RGB Stack" ) );
    //buttonGroup1->setTitle( tr( "Statistics" ) );
    textLabel1->setText( tr( "Mean" ) );
    meanLabel->setText( tr( "-" ) );
    textLabel3->setText( tr( "Median" ) );
    medianLabel->setText( tr( "-" ) );
}

void LevelsWidget::setTheImage( QImage *image ) 
{ 
    levelsPlot->setTheImage(image); 
}


