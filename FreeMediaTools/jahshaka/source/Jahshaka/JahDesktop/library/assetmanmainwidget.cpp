/****************************************************************************
** Form implementation generated from reading ui file 'assetmanmainwidget.ui'
**
** Created: Fri Oct 21 16:48:59 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "assetmanmainwidget.h"

#include <qvariant.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtoolbutton.h>
#include <qcombobox.h>
#include <qsplitter.h>
#include <qhbox.h>
#include <qwidgetstack.h>
#include <qtable.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qimage.h>
#include <qpixmap.h>

#include "AssetManager.h"
#include "dirview.h"
#include "jahfileiconview.h"
#include "jahprogressbar.h"
#include "advancedsearchform.h"
#include "assetmanmainwidget.ui.h"

#include "assetmanmainwidget.images.h"

/*
 *  Constructs a AssetManMainWidget as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 */
AssetManMainWidget::AssetManMainWidget( QWidget* parent, QWidget* userspace, 
									    QWidget* leftmenuspace,QWidget* rightmenuspace,
									    const char* name, WFlags fl )
    : QWidget( parent, name, fl ),
      image0( (const char **) image0_data ),
      image1( (const char **) image1_data ),
      image2( (const char **) image2_data ),
      image4( (const char **) image4_data )
{
    QImage img;
    img.loadFromData( image3_data, sizeof( image3_data ), "PNG" );
    image3 = img;
    img.loadFromData( image5_data, sizeof( image5_data ), "PNG" );
    image5 = img;
    img.loadFromData( image6_data, sizeof( image6_data ), "PNG" );
    image6 = img;
    img.loadFromData( image7_data, sizeof( image7_data ), "PNG" );
    image7 = img;
    img.loadFromData( image8_data, sizeof( image8_data ), "PNG" );
    image8 = img;

    if ( !name )
	setName( "AssetManMainWidget" );




    QVBoxLayout* privateLayoutWidget = new QVBoxLayout( parent ); //splitter2


    //AssetManMainWidgetLayout = new QVBoxLayout( this, 0, 0, "AssetManMainWidgetLayout"); 

    layout11 = new QHBoxLayout( 0, 0, 6, "layout11"); 

    layout16 = new QGridLayout( 0, 1, 1, 0, 0, "layout16"); 

    pixmapLabel1_2 = new QLabel( this, "pixmapLabel1_2" );
    pixmapLabel1_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, pixmapLabel1_2->sizePolicy().hasHeightForWidth() ) );
    pixmapLabel1_2->setMinimumSize( QSize( 0, 6 ) );
    pixmapLabel1_2->setMaximumSize( QSize( 32767, 6 ) );
    pixmapLabel1_2->setPixmap( image0 );
    pixmapLabel1_2->setScaledContents( TRUE );

    layout16->addWidget( pixmapLabel1_2, 0, 2 );

    advancedSearchButton = new QPushButton( this, "advancedSearchButton" );
    advancedSearchButton->setMinimumSize( QSize( 0, 27 ) );
    advancedSearchButton->setMaximumSize( QSize( 32767, 27 ) );
    advancedSearchButton->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    advancedSearchButton->setPaletteBackgroundColor( QColor( 51, 102, 255 ) );
    QPalette pal;
    QColorGroup cg;
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Light, QColor( 178, 197, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 114, 149, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 25, 51, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 34, 68, 170) );
    cg.setColor( QColorGroup::Text, white );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, white );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setActive( cg );
    cg.setColor( QColorGroup::Foreground, black );
    cg.setColor( QColorGroup::Button, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Light, QColor( 178, 197, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 89, 131, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 25, 51, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 34, 68, 170) );
    cg.setColor( QColorGroup::Text, white );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, white );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setInactive( cg );
    cg.setColor( QColorGroup::Foreground, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Button, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Light, QColor( 178, 197, 255) );
    cg.setColor( QColorGroup::Midlight, QColor( 89, 131, 255) );
    cg.setColor( QColorGroup::Dark, QColor( 25, 51, 127) );
    cg.setColor( QColorGroup::Mid, QColor( 34, 68, 170) );
    cg.setColor( QColorGroup::Text, white );
    cg.setColor( QColorGroup::BrightText, white );
    cg.setColor( QColorGroup::ButtonText, QColor( 128, 128, 128) );
    cg.setColor( QColorGroup::Base, white );
    cg.setColor( QColorGroup::Background, QColor( 51, 102, 255) );
    cg.setColor( QColorGroup::Shadow, black );
    cg.setColor( QColorGroup::Highlight, QColor( 0, 0, 128) );
    cg.setColor( QColorGroup::HighlightedText, white );
    cg.setColor( QColorGroup::Link, black );
    cg.setColor( QColorGroup::LinkVisited, black );
    pal.setDisabled( cg );
    advancedSearchButton->setPalette( pal );
    QFont advancedSearchButton_font(  advancedSearchButton->font() );
    advancedSearchButton_font.setPointSize( 7 );
    advancedSearchButton_font.setUnderline( TRUE );
    advancedSearchButton->setFont( advancedSearchButton_font ); 
    advancedSearchButton->setCursor( QCursor( 13 ) );
    advancedSearchButton->setFlat( TRUE );

    layout16->addMultiCellWidget( advancedSearchButton, 0, 2, 4, 4 );

    pixmapLabel1 = new QLabel( this, "pixmapLabel1" );
    pixmapLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 1, 0, pixmapLabel1->sizePolicy().hasHeightForWidth() ) );
    pixmapLabel1->setMaximumSize( QSize( 32767, 5 ) );
    pixmapLabel1->setPixmap( image1 );
    pixmapLabel1->setScaledContents( TRUE );

    layout16->addWidget( pixmapLabel1, 2, 2 );

    pixmapLabel2 = new QLabel( this, "pixmapLabel2" );
    pixmapLabel2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapLabel2->sizePolicy().hasHeightForWidth() ) );
    pixmapLabel2->setMinimumSize( QSize( 0, 27 ) );
    pixmapLabel2->setMaximumSize( QSize( 32767, 27 ) );
    pixmapLabel2->setPixmap( image2 );
    pixmapLabel2->setScaledContents( TRUE );

    layout16->addMultiCellWidget( pixmapLabel2, 0, 2, 3, 3 );

    searchLineEdit = new QLineEdit( this, "searchLineEdit" );
    searchLineEdit->setEnabled( TRUE );
    searchLineEdit->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)0, 1, 0, searchLineEdit->sizePolicy().hasHeightForWidth() ) );
    searchLineEdit->setMinimumSize( QSize( 150, 16 ) );
    searchLineEdit->setMaximumSize( QSize( 200, 16 ) );
    searchLineEdit->setPaletteForegroundColor( QColor( 0, 0, 0 ) );
    searchLineEdit->setPaletteBackgroundColor( QColor( 255, 255, 255 ) );
    searchLineEdit->setFrameShape( QLineEdit::NoFrame );
    searchLineEdit->setFrame( FALSE );

    layout16->addWidget( searchLineEdit, 1, 2 );

    textLabel1 = new QLabel( this, "textLabel1" );
    textLabel1->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)0, 0, 0, textLabel1->sizePolicy().hasHeightForWidth() ) );
    textLabel1->setMinimumSize( QSize( 60, 27 ) );
    textLabel1->setPaletteForegroundColor( QColor( 255, 255, 255 ) );
    textLabel1->setPaletteBackgroundColor( QColor( 51, 102, 255 ) );
    QFont textLabel1_font(  textLabel1->font() );
    textLabel1_font.setFamily( "Arial" );
    textLabel1_font.setPointSize( 10 );
    textLabel1_font.setBold( TRUE );
    textLabel1->setFont( textLabel1_font ); 
    textLabel1->setAlignment( int( QLabel::AlignVCenter | QLabel::AlignRight ) );

    layout16->addMultiCellWidget( textLabel1, 0, 2, 0, 0 );

    pixmapLabel2_3 = new QLabel( this, "pixmapLabel2_3" );
    pixmapLabel2_3->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapLabel2_3->sizePolicy().hasHeightForWidth() ) );
    pixmapLabel2_3->setMinimumSize( QSize( 42, 27 ) );
    pixmapLabel2_3->setMaximumSize( QSize( 42, 27 ) );
    pixmapLabel2_3->setPixmap( image3 );
    pixmapLabel2_3->setScaledContents( TRUE );

    layout16->addMultiCellWidget( pixmapLabel2_3, 0, 2, 5, 5 );

    pixmapLabel2_2 = new QLabel( this, "pixmapLabel2_2" );
    pixmapLabel2_2->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, pixmapLabel2_2->sizePolicy().hasHeightForWidth() ) );
    pixmapLabel2_2->setMinimumSize( QSize( 0, 0 ) );
    pixmapLabel2_2->setMaximumSize( QSize( 32767, 27 ) );
    pixmapLabel2_2->setPixmap( image4 );
    pixmapLabel2_2->setScaledContents( TRUE );

    layout16->addMultiCellWidget( pixmapLabel2_2, 0, 2, 1, 1 );
    layout11->addLayout( layout16 );

    connectServerButton = new QToolButton( this, "connectServerButton" );
    connectServerButton->setMaximumSize( QSize( 32767, 27 ) );
    connectServerButton->setBackgroundMode( QToolButton::PaletteBackground );
    connectServerButton->setIconSet( QIconSet( image5 ) );
    connectServerButton->setAutoRaise( TRUE );
    layout11->addWidget( connectServerButton );

    backButton = new QToolButton( this, "backButton" );
    backButton->setEnabled( FALSE );
    backButton->setBackgroundMode( QToolButton::PaletteBackground );
    backButton->setIconSet( QIconSet( image6 ) );
    backButton->setAutoRaise( TRUE );
    layout11->addWidget( backButton );

    upButton = new QToolButton( this, "upButton" );
    upButton->setEnabled( FALSE );
    upButton->setMaximumSize( QSize( 32767, 35 ) );
    upButton->setBackgroundMode( QToolButton::PaletteBackground );
    upButton->setIconSet( QIconSet( image6 ) );
    upButton->setAutoRaise( TRUE );
    layout11->addWidget( upButton );

    viewsButton = new QPushButton( this, "viewsButton" );
    viewsButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, viewsButton->sizePolicy().hasHeightForWidth() ) );
    viewsButton->setMinimumSize( QSize( 35, 0 ) );
    viewsButton->setMaximumSize( QSize( 45, 32767 ) );
    viewsButton->setPixmap( image7 );
    viewsButton->setFlat( TRUE );
    layout11->addWidget( viewsButton );

    mediaViewsButton = new QPushButton( this, "mediaViewsButton" );
    mediaViewsButton->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 0, 0, mediaViewsButton->sizePolicy().hasHeightForWidth() ) );
    mediaViewsButton->setMinimumSize( QSize( 45, 0 ) );
    mediaViewsButton->setMaximumSize( QSize( 45, 32767 ) );
    mediaViewsButton->setPixmap( image8 );
    mediaViewsButton->setFlat( TRUE );
    layout11->addWidget( mediaViewsButton );

    driveCombo = new QComboBox( FALSE, this, "driveCombo" );
    driveCombo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)0, (QSizePolicy::SizeType)0, 0, 0, driveCombo->sizePolicy().hasHeightForWidth() ) );
    driveCombo->setMinimumSize( QSize( 10, 0 ) );
    driveCombo->setMaximumSize( QSize( 60, 32767 ) );
    driveCombo->setEditable( TRUE );
    layout11->addWidget( driveCombo );

    pathCombo = new QComboBox( FALSE, this, "pathCombo" );
    pathCombo->setEnabled( TRUE );
    pathCombo->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)1, (QSizePolicy::SizeType)0, 1, 0, pathCombo->sizePolicy().hasHeightForWidth() ) );
    pathCombo->setMinimumSize( QSize( 150, 0 ) );
    pathCombo->setMaximumSize( QSize( 200, 32767 ) );
    pathCombo->setEditable( TRUE );
    layout11->addWidget( pathCombo );

    dummyLabel = new QLabel( this, "dummyLabel" );
    dummyLabel->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, dummyLabel->sizePolicy().hasHeightForWidth() ) );
    dummyLabel->setMinimumSize( QSize( 15, 0 ) );
    layout11->addWidget( dummyLabel );
    privateLayoutWidget->addLayout( layout11 );
    spacer9 = new QSpacerItem( 8, 2, QSizePolicy::Fixed, QSizePolicy::Minimum );
    privateLayoutWidget->addItem( spacer9 );

	///////////////////////////////////////////////////////////////////
	// this is the directory listing // 
	widgetStack1 = new QWidgetStack( leftmenuspace, "widgetStack1" );
	widgetStack1->setMinimumSize( QSize( 150, 0 ) );

    dirlistStackPage = new QWidget( widgetStack1, "dirlistStackPage" );
    dirlistStackPageLayout = new QVBoxLayout( dirlistStackPage, 11, 6, "dirlistStackPageLayout"); 

    dirlist = new DirectoryView( dirlistStackPage, "dirlist" );
    dirlist->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, dirlist->sizePolicy().hasHeightForWidth() ) );
    dirlist->setMinimumSize( QSize( 100, 100 ) );
    dirlistStackPageLayout->addWidget( dirlist );
    widgetStack1->addWidget( dirlistStackPage, 0 );

    advancedSearchStackPage = new QWidget( widgetStack1, "advancedSearchStackPage" );
    advancedSearchStackPageLayout = new QGridLayout( advancedSearchStackPage, 1, 1, 11, 6, "advancedSearchStackPageLayout"); 

    advancedSearchForm1 = new advancedSearchForm( advancedSearchStackPage, "advancedSearchForm1" );

    advancedSearchStackPageLayout->addWidget( advancedSearchForm1, 0, 0 );
    widgetStack1->addWidget( advancedSearchStackPage, 1 );

	/////////////////////////////////////////////////////////////////////
	//trying something here mon -  to separate the assetview from the fileview
    filelist = new DirectoryView( rightmenuspace, "dirlist" );
    filelist->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 0, filelist->sizePolicy().hasHeightForWidth() ) );
    filelist->setMinimumSize( QSize( 100, 100 ) );


	//////////////////////////////////////////////////////////////////
	// the fileview widget starts here

    fileview = new QtFileIconView( parent, "fileview" );
    //fileview = new QtFileIconView( privateLayoutWidget, "fileview" );
    //fileview->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)5, (QSizePolicy::SizeType)5, 1, 1, fileview->sizePolicy().hasHeightForWidth() ) );
    //fileview->setMinimumSize( QSize( 100, 100 ) );

    privateLayoutWidget->addWidget( fileview );
    //layout19->addMultiCellWidget( fileview, 1, 1, 0, 3 );

	///////////////////////////////////////////////////////////////////////////
	//this is the container for the progress bar and load button 

	QHBox * thebox = new QHBox (parent);
    
	progress = new JahProgressBar( thebox, "progress" );
    progress->setMinimumSize( QSize( 150, 0 ) );
    progress->setMaximumSize( QSize( 250, 32767 ) );

    //spacer2 = new QSpacerItem( 461, 16, QSizePolicy::Expanding, QSizePolicy::Minimum );

    closeButton = new QPushButton( thebox, "closeButton" );
    closeButton->setMinimumSize( QSize( 300, 0 ) );
    closeButton->setMaximumSize( QSize( 350, 32767 ) );

	privateLayoutWidget->addWidget( thebox );

	//hacked into the old asset loader in jahlibrary.cpp
    connect( closeButton, SIGNAL( clicked() ), this, SLOT( returnFile( ) ) );
    
	
	/////////////////////////////////////////////////////////////////////
	//we want to put the asset table in the footer of the application mon
    assetTable = new QTable( userspace, "assetTable" ); //

    assetTable->setSizePolicy( QSizePolicy( (QSizePolicy::SizeType)7, (QSizePolicy::SizeType)7, 0, 0, assetTable->sizePolicy().hasHeightForWidth() ) );
    assetTable->setMaximumSize( QSize( 32767, 350 ) );
    assetTable->setNumRows( 0 );
    assetTable->setNumCols( 1 );
    assetTable->setShowGrid( FALSE );
    //assetTable->hide();
    //AssetManMainWidgetLayout->addWidget( splitter3 );

	//////////////////////////////////////////////////////////////////////
	
	
	languageChange();
    resize( QSize(832, 687).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( fileview, SIGNAL( directoryChanged(const QString&) ), this, SLOT( directoryChanged(const QString&) ) );
    connect( fileview, SIGNAL( startReadDir(int) ), this, SLOT( slotStartReadDir(int) ) );
    connect( fileview, SIGNAL( readNextDir() ), this, SLOT( slotReadNextDir() ) );
    connect( fileview, SIGNAL( readDirDone() ), this, SLOT( slotReadDirDone() ) );
    connect( advancedSearchButton, SIGNAL( clicked() ), this, SLOT( advandedSearchClicked() ) );
    connect( dirlist, SIGNAL( folderSelected(const QString&) ), fileview, SLOT( setDirectory(const QString&) ) );

    connect( filelist, SIGNAL( folderSelected(const QString&) ), fileview, SLOT( setDirectory(const QString&) ) );

	connect( searchLineEdit, SIGNAL( returnPressed() ), this, SLOT( executeSimpleSearch() ) );
    
	connect( dirlist, SIGNAL( selectionChanged(QListViewItem*) ), advancedSearchForm1, SLOT( listViewSelectionChangedSlot(QListViewItem*) ) );
    
	connect( upButton, SIGNAL( clicked() ), this, SLOT( cdUp() ) );
    connect( dirlist, SIGNAL( collectionDeleted() ), fileview, SLOT( clearAll() ) );
    connect( fileview, SIGNAL( enableUp() ), this, SLOT( enableUp() ) );
    connect( fileview, SIGNAL( disableUp() ), this, SLOT( disableUp() ) );
    connect( connectServerButton, SIGNAL( clicked() ), this, SLOT( connectToServerDialog() ) );
    connect( pathCombo, SIGNAL( activated(const QString&) ), fileview, SLOT( setDirectory(const QString&) ) );
    connect( fileview, SIGNAL( enableBack() ), this, SLOT( enableBack() ) );
    connect( backButton, SIGNAL( clicked() ), this, SLOT( cdBack() ) );
    connect( fileview, SIGNAL( disableBack() ), this, SLOT( disableBack() ) );

    // tab order
    setTabOrder( searchLineEdit, advancedSearchButton );
    setTabOrder( advancedSearchButton, assetTable );
    setTabOrder( assetTable, closeButton );
    init();
}


//////////////////////////////////////////////////////
// we need to hack this in here when jahlibrary.cpp is obsoleted - its the load routine
void AssetManMainWidget::returnFile( void )
{ 
	/*
	QtFileIconView *files = fileView();
	
	const QtFileIconViewItem *item = ( const QtFileIconViewItem * )files->firstItem( );

	m_selected_items.clear( );

	for ( item = ( const QtFileIconViewItem * )files->firstItem( ); 
		  item != NULL; 
		  item = ( const QtFileIconViewItem * )item->nextItem( ) )
		if ( item->isSelected( ) && QFileInfo( item->filename( ) ).exists( ) )
			m_selected_items.append( item->filename( ) );
*/ 

//	debug("hit load");

	emit loadButton();

}
/*
 *  Destroys the object and frees any allocated resources
 */
AssetManMainWidget::~AssetManMainWidget()
{
	delete fileview;
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void AssetManMainWidget::languageChange()
{
    setCaption( tr( "Asset manager" ) );
    advancedSearchButton->setText( tr( "Advanced" ) );
    pixmapLabel2->setText( QString::null );
    searchLineEdit->setText( QString::null );
    textLabel1->setText( tr( "Search" ) );
    pixmapLabel2_3->setText( QString::null );
    connectServerButton->setText( QString::null );
    QToolTip::add( connectServerButton, tr( "Connect to new server" ) );
    backButton->setText( QString::null );
    upButton->setText( QString::null );
    viewsButton->setText( QString::null );
    QToolTip::add( viewsButton, tr( "Choose view options" ) );
    mediaViewsButton->setText( QString::null );
    QToolTip::add( mediaViewsButton, tr( "Choose files to show" ) );
    dummyLabel->setText( QString::null );
    //progressLabel->setText( QString::null );
    closeButton->setText( tr( "Load" ) );
}

