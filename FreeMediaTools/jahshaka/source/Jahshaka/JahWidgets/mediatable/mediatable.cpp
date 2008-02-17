/*******************************************************************************
**
** The source file for the Jahshaka desktop canvas file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "mediatable.h"

#include <qlayout.h>
#include <qstyle.h>
#include <desktopcanvas.h>
#include <imageitem.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qdragobject.h>
#include <qwidgetstack.h>
#include <qmessagebox.h>
#include <qiconset.h>

#include "jahtracer.h"
#include "jahpreferences.h"
#include "valueFormatters.h"

#include <openmedialib.h>

MediaTableItem::MediaTableItem( QTable *table, assetData assetdata ) 
    : QTableItem ( table, QTableItem::Never ),
      theasset( assetdata )
{
        
    // nothing else.
}

MediaTable::MediaTable(int rows, int cols, QWidget* parent, const char* name ) : QTable( rows, cols, parent, name )
{
    // We dont really 'accept drops', but if we look like we do then the dragobject draws the 
    // nice pixmap instead of ugly 'dont drop' cursor.
    setAcceptDrops( true );
    setHScrollBarMode( QScrollView::AlwaysOff ); // (NB we need vertical scrollbars).
    
    //set the base path for relative pixmap location
    JahPrefs& jprefs = JahPrefs::getInstance();    
    JahBasePath = jprefs.getBasePath( ).data();
}


void MediaTable::contentsMousePressEvent( QMouseEvent * e )
{
    // argh, qt dnd code is weird...

    QTable::contentsMousePressEvent( e ); // allow table to select its stuff first.
    QPixmap pix;

    if ( currentRow() < 0 || e->button() != LeftButton ) 
        return;
    
    pix = pixmap (currentRow(),0);
    QImage image = pix.convertToImage();

    // Assume the pixmap is fairly small (its already scaled in the table), else should do something 
    // like: convertFromImage(pix.convertToImage().smoothScale(pix.width()/3,pix.height()/3));
    QPixmap pm = pix;

    QDragObject *drobj = new QImageDrag( image, this );
    drobj->setPixmap ( pm , QPoint ( pm.width()/2, pm.height()/2 ));
    drobj->dragCopy();
}

void MediaTable::dragMoveEvent( QDragMoveEvent *e )
{
    e->acceptAction( e->action() == QDropEvent::Copy );
}

void MediaTable::dragEnterEvent( QDragEnterEvent *e )
{
    e->accept();
}

void MediaTable::contentsDropEvent ( QDropEvent * e )
{
    e->ignore();
}

void MediaTable::paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg )
{
    QColorGroup ourcg( cg );
    if ( row % 2 ) // alternate colors
        ourcg.setColor( QColorGroup::Base, cg.base().dark( 110 ) ) ;  
    QTable::paintCell( p, row, col, cr, selected, ourcg );
}

void MediaTable::keyPressEvent ( QKeyEvent * e )
{   // (this is an attempt to prevent QTable eating keyboard events (like shift key) which other widgets are better to process).
    if ( e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Right || e->key() == Qt::Key_Left 
        || e->key() == Qt::Key_PageDown || e->key() == Qt::Key_PageUp || e->key() == Qt::Key_Home || e->key() == Qt::Key_End )
        QTable::keyPressEvent( e ); 
    else 
        e->ignore();
}


AssetDisplay::~AssetDisplay()
{
  delete m_info;
}

AssetDisplay::AssetDisplay( QWidget* parent, const char* name )
    : QWidget( parent, name )
{
    QBoxLayout* l = new QVBoxLayout( this );
    l->setMargin( 5 );
    m_info = new QLabel( this );
    l->addWidget( m_info );
    
    setFont( font() );
}

void AssetDisplay::setFont( const QFont& fn_ )
{
    // make the text slightly smaller
    QFont fn( fn_ );
    fn.setPointSize( fn.pointSize() - 1 );
    QWidget::setFont( fn );
}

void AssetDisplay::setAssetData( const assetData& d )
{
    switch ( d.theCategory )
    {
    case VideoCategory::MEDIA:
    {
        projectData projectdata;
        TimecodeValueFormatter fmt( projectdata.getFPS( JahPrefs::getInstance().getJahResolution() ) );

        QString text( tr( "file type: %1\n"
                          "depth: %2\n"
                          "resolution: %3\n"
                          "length: %4" )
                      .arg( QString( VideoType::getName( d.theType ).c_str( ) ).lower() )
                      .arg( "n/a" )
                      .arg( QString( "%1x%2" )
                            .arg( d.xNativeSize )
                            .arg( d.yNativeSize ) )
                      .arg( QString( "%1/%2 frames" )
                            .arg( fmt.convert( d.getNumFrames() ) )
                            .arg( d.getNumFrames() ) )
            );
        m_info->setText( text );
        break;
    }

    default:
        m_info->setText( QString() );
        break;
    }
}

void setupButton( QPushButton* b, QString active, QString disabled )
{
    QString JahBasePath = JahPrefs::getInstance().getBasePath().data();

    QIconSet icon;
    icon.setPixmap( QPixmap( JahBasePath + active ), QIconSet::Small, QIconSet::Normal );
    icon.setPixmap( QPixmap( JahBasePath + disabled ), QIconSet::Small, QIconSet::Disabled );
    b->setIconSet( icon );
    b->setFlat( true );
    b->setFixedSize( QSize( 20, 20 ) );
}

JahDesktopSideTable::JahDesktopSideTable( int, QWidget* parent, const char* name) 
    : QWidget(parent,name)
    , m_asset_tracking( true )
    , topspacer(0)
    ,
      thetable(0),
      figure(0),
      pbplay(0),
      pbstop(0),
      pbffwd(0),
      pbfrev(0),
      m_timecode(0),
      lspacer1(0),
      lspacer2(0),
      m_assetDisplay(0)
{
	AssetTables::getInstance( )->registerTable( this );

	setMinimumWidth( 180 );

    //set the base path for relative pixmap location
    JahPrefs& jprefs = JahPrefs::getInstance();    
    JahBasePath = jprefs.getBasePath( ).data();
    
    QGridLayout *grid = new QGridLayout( this, 4, 3 );
    
    figure = new FigureEditor ( &thecanvas, this );
    QColor back;    back.setHsv(35,35,35);
    thecanvas.setBackgroundColor( back );
    figure->setBackgroundColor( back );
    figure->setFixedSize( 180, 120 );
    figure->setFrameShape( QFrame::NoFrame );
    thecanvas.resize( 180, 120 );
    figure->setHScrollBarMode( QScrollView::AlwaysOff );
    figure->setVScrollBarMode( QScrollView::AlwaysOff );
    // Change the default figureeditor behaviour
    figure->setResizingAllowed ( false );
    figure->setMovingAllowed ( false );
    figure->setPlayerControlsMode ( FigureEditor::AlwaysHide );
    figure->setItemBorderEnabled ( false );
    
    grid->addWidget( figure, 0, 1, Qt::AlignHCenter | Qt::AlignVCenter );

    thetable = new MediaTable ( 0, 0, this );
    grid->addMultiCellWidget( thetable, 3, 3, 0, 2 );

    thetable->setFocusStyle(QTable::FollowStyle);
    thetable->setSelectionMode(QTable::SingleRow);

    // The buttons.
    QBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing( 1 );
    grid->addLayout( buttonLayout, 1, 1 );

    lspacer1 = new QLabel( this, "");
    //lspacer1->setFixedHeight( 35 );
    buttonLayout->addWidget( lspacer1 );
    lspacer1->show();

    pbfrev = new QPushButton ( this );
    setupButton( pbfrev, "Pixmaps/desktop/rewind-to-beginning-mini.png", "Pixmaps/desktop/grey-rewind-to-beginning-mini.png" );
    buttonLayout->addWidget( pbfrev );

    pbstop = new QPushButton ( this );
    setupButton( pbstop, "Pixmaps/desktop/stop-mini.png", "Pixmaps/desktop/grey-stop-mini.png" );
    buttonLayout->addWidget( pbstop );
    
    // insert time code widget here
    m_timecode = new QLabel( this );
    buttonLayout->addWidget( m_timecode );

    pbplay = new QPushButton ( this );
    setupButton( pbplay, "Pixmaps/desktop/play-mini.png", "Pixmaps/desktop/grey-play-mini.png" );
    buttonLayout->addWidget( pbplay );

    pbffwd = new QPushButton ( this );
    setupButton( pbffwd, "Pixmaps/desktop/forward-to-end-mini.png", "Pixmaps/desktop/grey-forward-to-end-mini.png" );
    buttonLayout->addWidget( pbffwd );

    lspacer2 = new QLabel( this, "");
    //lspacer2->setFixedHeight( 35 );
    buttonLayout->addWidget( lspacer2 );
    lspacer2->show();

    // force this layout to be as small as possible
    buttonLayout->setResizeMode( QLayout::Fixed );

    showPlayerControls( false ); // hide at start.

    // insert the assetDisplay
    m_assetDisplay = new AssetDisplay( this );
    grid->addWidget( m_assetDisplay, 2, 1 );

    // insert a table
    thetable->setTopMargin( 0 ); // 0 table header space.

    thetable->setLeftMargin(0);
    thetable->verticalHeader()->hide();

    jahnumRows = 0;
    thetable->setNumCols(3);

    thetable->setColumnStretchable ( 1, true ); // clip name column fills the space

    thetable->show();
    figure->show();
    m_assetDisplay->show();

    thetable->setDragEnabled( true );

    // Make the figure change when a new item is clicked
    connect ( thetable, SIGNAL( currentChanged ( int, int )), this, SLOT( showItemInFigure ( int, int )));
    connect ( thetable, SIGNAL( gotFocus( ) ), this, SLOT( receivedFocus( ) ) );

    // Connect the play buttons to the canvas view slots. Play button gets its own slot here.
    connect ( pbplay, SIGNAL( clicked()), this, SLOT( playControlClicked()));
    connect ( pbstop, SIGNAL( clicked()), this, SLOT( stopControlClicked()));
    connect ( pbffwd, SIGNAL( clicked()), figure, SLOT( fForwardControlClicked()));
    connect ( pbfrev, SIGNAL( clicked()), figure, SLOT( rewindControlClicked()));

    // connect up monitoring of the frame, and initialize
    connect ( figure, SIGNAL( updateFrameOffset( assetData ) ), this, SLOT( onUpdateFrameOffset( assetData ) ) );

    projectData projectdata;
    TimecodeValueFormatter fmt( projectdata.getFPS( JahPrefs::getInstance().getJahResolution() ) );
    m_timecode->setText( fmt.convert( 0 ) );

    // pass on a signal from the table
    connect ( thetable, SIGNAL( currentChanged( int, int )), this, SIGNAL( currentChanged(int, int )));

    // show the backdrop
    showBackdrop( true );

    clipplayable = false;
}

JahDesktopSideTable::~JahDesktopSideTable( )
{
  delete thetable;
  delete figure;
  delete pbplay;
  delete pbstop;
  delete pbffwd;
  delete pbfrev;
  delete m_timecode;
  delete lspacer1;
  delete lspacer2;
  //  delete topspacer;
  //==16764== Process terminating with default action of signal 11 (SIGSEGV)
    //==16764==  Access not within mapped region at address 0x39
    //==16764==    at 0x84171D7: JahDesktopSideTable::~JahDesktopSideTable() (mediatable.cpp:309)
  delete m_assetDisplay;

}


void JahDesktopSideTable::setAssetTracking( bool on ) 
{ 
	m_asset_tracking = on;
	if ( on )
	{
		m_assetDisplay->show( );
		m_timecode->show( );
		pbplay->show( );
		pbstop->show( );
		pbffwd->show( );
		pbfrev->show( );
		figure->show( );
	}
	else
	{
		m_assetDisplay->hide( );
		m_timecode->hide( );
		pbplay->hide( );
		pbstop->hide( );
		pbffwd->hide( );
		pbfrev->hide( );
		figure->hide( );
	}
}

void JahDesktopSideTable::selectAsset( assetData &asset )
{
	if ( m_asset_tracking && asset.extension != ".jfx" )
	{
		blockSignals( true );
		int row = find( asset );
		if ( row >= 0 && thetable->currentRow( ) != row )
			thetable->setCurrentCell( row, 0 );
		blockSignals( false );
	}
}

void JahDesktopSideTable::showBackdrop( bool on )
{
    QPixmap tile;
    if ( on )
        tile = QPixmap( JahBasePath+ "Pixmaps/jah-emptymediatable.png" );
    thecanvas.setBackgroundPixmap ( tile );
}

int JahDesktopSideTable::find( assetData thedata )
{
    int row = -1;
    
    if ( thetable->numRows( ) > 0 )
    {
        for ( row = 0; row < thetable->numRows( ); row ++ )
        {
            MediaTableItem *item = dynamic_cast<MediaTableItem*>(thetable->item( row, 0 )); // must be first col!
            if ( thedata.clipnumber == item->theasset.clipnumber )
                break;
        }
    }

    return row;
}

void JahDesktopSideTable::addAsset( int, int, assetData thedata, QImage theimage )
{
    // for the moment, just set native size information here... it's not persisted
    // but should be
    thedata.xNativeSize = theimage.width();
    thedata.yNativeSize = theimage.height();

    //////////////////////////////////////////
    //need to add the entireline here
    
    thetable->setNumRows(jahnumRows+1);
    thetable->setRowHeight ( jahnumRows, 40 );
	
    thetable->setColumnWidth ( 0,    60  );
    thetable->setColumnWidth ( 1,    165 ); // (vscrollbar takes up some space in table)

    // Be aware of 'hack' here: we store asset id in 3rd column, but hide it. Then, the 
    // mediatable can behave exactly the same as the desktop table, ie locate items by id.
    thetable->hideColumn ( 2 );
    //    debug("added row");

    ///////////////////////////////////////////

    float ratio = (float)theimage.width() / (float)theimage.height();
    int w = 60;
    int h = int( w / ratio );
    
    if ( h > 40 )
    {
        h = 40;
        w = int( h * ratio );
    }
    
    QPixmap pix = theimage.scale( w, h );

    //used for num to text conversion as table dopesnt seem to hold integers
    QString conv;

    MediaTableItem *mitem = new MediaTableItem ( thetable, thedata );
    mitem->setPixmap (pix );
    thetable->setItem ( jahnumRows, 0,  mitem );
    conv.setNum( thedata.clipnumber );
    thetable->setText( jahnumRows, 1 , thedata.clipname );

    // store the id in spare column, same as desktoptable does.
    thetable->setText( jahnumRows, 2 , conv.setNum(thedata.clipnumber) );
    
    thetable->setRowReadOnly(jahnumRows, true);
    // thetable->setCurrentCell( jahnumRows, 0 ); // could show newly loaded item in figure?
    if ( jahnumRows == 0 )
        showItemInFigure( 0, 0 );

    jahnumRows +=1 ;

    //debug("added asset");
}

void JahDesktopSideTable::renameAsset( int id, QString newname )
{
    for ( int row = 0; row < thetable->numRows(); row++ ) 
    {
        if ( id == (thetable->text( row, 2 )).toInt() )
        {   thetable->setText( row, 1, newname);
        }
    }
}

void JahDesktopSideTable::showItemInFigure( int row, int )
{  
    // If the selected row is invalid clear and show the backdrop
    if  ( row >= thetable->numRows() )
    {
        figure->clear();
        showBackdrop( true );
        return; 
    }

    // This is fine as long as we just add MediaTableItems to column 1 of the table.
    MediaTableItem *item = dynamic_cast<MediaTableItem*>(thetable->item( row, 0 )); // must be first col!

	// Sanity checks on the item
    if ( !item )
    {
        figure->clear( );
        showBackdrop( true );
        return;
    }

    // Get the asset data of the item
    assetData thedata = item->theasset;

    m_assetDisplay->setAssetData( thedata );
    onUpdateFrameOffset( assetData() );
    
    // Only refresh the item if the assets are different
    if ( m_asset_tracking && ( figure->globalitem == NULL || thedata.getFullFileName( ) != figure->globalitem->getClip( ).getFullFileName( ) ) )
    {
        figure->clear();
    	showBackdrop( false );
        
    	assetExchange exchange;
        
    	if ( thedata.theCategory == VideoCategory::CLIP || thedata.theCategory == VideoCategory::MEDIA )
    	{   
            showPlayerControls( true );
            clipplayable = true; 
    	}
    	else
    	{    
            showPlayerControls( false );
            clipplayable = false ;
    	}
    
    	QImage img;
    	QImage theimage = exchange.getImage(thedata, thedata.startframe + thedata.frameOffset);
    	float ratio = (float)theimage.width() / (float)theimage.height();
        int w = 180;
        int h = int( w / ratio );
        
        if ( h > 120 )
        {
            h = 120;
            w = int( h * ratio );
        }
        
    	img = theimage.scale( w, h, QImage::ScaleMin );

    	//linux didnt like this so we had to go with the separate qstring below
    	//ImageItem* i = new ImageItem(thedata, img, &thecanvas, QString("name") );
    
    	QString thename="name";
    	ImageItem* i = new ImageItem(thedata, img, &thecanvas, thename );
   
    
    	figure->swapItem( i );
    	i->enableResizeHandle ( false );
    	int xgap = int( (figure->size().width() - i->width())/2 ); 
    	int ygap = int( (figure->size().height() - i->height())/2 );
    	if ( xgap>0 ) i->setX( xgap );
    	if ( ygap>0 ) i->setY( ygap );
    	i->show();
    }
    
    // Always notify the desktop which asset is active
    emit assetSelected( thedata );
}

void JahDesktopSideTable::receivedFocus( )
{
    if ( thetable->currentRow( ) >= 0 )
        showItemInFigure( thetable->currentRow( ), 0 );
}

void JahDesktopSideTable::showPlayerControls( bool show )
{
	if ( m_asset_tracking )
	{
    	pbplay->show();
    	pbstop->show();
    	pbffwd->show();
    	pbfrev->show();

    	pbplay->setEnabled( show );
    	pbstop->setEnabled( show );
    	pbffwd->setEnabled( show );
    	pbfrev->setEnabled( show );
	}
}


bool JahDesktopSideTable::removeItem(int theitem)
{
    // Note, this is same behavious as desktoptable, except here the 
    // asset id is in our hidden column 2.
	
    //QString rere;
	//debug("--------------------------------");
	//debug("calling remove for item id"+rere.setNum(theitem));

    bool itworked = false;

    //theitem -=1;

    //we need to scan all rows
    //and find out which row contains theitem in col 10
    int therow = 0;
    QString thetext;
    int theval;

    //should be start row to end row
    for (int x=0; x<=jahnumRows; x++)
    {
        //thetext = text(x,10);
        thetext = thetable->text(x,2);
        theval = thetext.toInt();

        if (theval == theitem)
        {
            therow = x;
            itworked = true;
        }
    }

    if (!itworked)
    {
        //debug("it didnt work");
    }
    else
    {
		thetable->removeRow(therow);
		jahnumRows -=1;
    }

    if ( jahnumRows == 0 )
    {   showBackdrop( true );
        figure->clear();
    }

    return itworked;
}

bool JahDesktopSideTable::clearAll(void)
{
    thetable->setNumRows( 0 );
    jahnumRows = 0;
    show();
    figure->clear();
    showBackdrop( true );
    return true;
}

void JahDesktopSideTable::playControlClicked()
{
    if ( !clipplayable ) return;
    figure->playControlPlay();
}

void JahDesktopSideTable::stopControlClicked()
{
    figure->playControlStop();
}

void JahDesktopSideTable::stopAnimation()
{
    if ( !clipplayable ) return;
    figure->playControlStop();
}

void JahDesktopSideTable::onUpdateFrameOffset( assetData d )
{
    projectData projectdata;
    TimecodeValueFormatter fmt( projectdata.getFPS( JahPrefs::getInstance().getJahResolution() ) );
    
    m_timecode->setText( fmt.convert( d.frameOffset ) );
}

void MediaTable::focusInEvent( QFocusEvent * ) { emit gotFocus( ); }
QString JahDesktopSideTable::text( int row, int col ) { return thetable->text( row, col ); };
MediaTable *JahDesktopSideTable::getTable( ) { return thetable; }
