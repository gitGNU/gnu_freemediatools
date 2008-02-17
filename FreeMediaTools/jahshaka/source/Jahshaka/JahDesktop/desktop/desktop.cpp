/*******************************************************************************
**
** The source file for the Jahshaka desktop database
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

//custom widgets
//this needs to be here or vc7 breaks on compile

#include <algorithm>

#include <qhbox.h>
#include <qlayout.h>
#include "mediatable.h"
#include "desktop.h"
#include "jahtranslate.h"
#include "jahpreferences.h"
#include "jahtracer.h"
#include "jahdatabase.h"
#include "filmstrip.h"
#include "imageitem.h"

JahDesktop::JahDesktop(QHBox* parent, QHBox* parent1, QHBox*,  
                       QHBox* leftcontroller, QHBox* rightcontroller,
                       const char* name, int * globalclipnumber)
	: m_started( false )
{
	AssetTables::getInstance( )->registerTable( this );

    ModuleName = name;

    jtrace = JahTrace::getInstance();	//set up tracer
    jtrace->debug("Setting up the Desktop Module");
    jt = JahTranslate::getInstance();	//set up translator

    //initialize JahBasePath
    JahPrefs& jprefs = JahPrefs::getInstance();
    JahBasePath   = jprefs.getBasePath().data();
    JahMediaPath  = jprefs.getMediaPath().data();

    /////////////////////////////////////////////////////////////////
    // interface work

    //build the left and right menus
    buildInterfaceMenus(leftcontroller,rightcontroller);

    //hook up desktop switching between canvas and table
    themaindesktopLayout = new QHBoxLayout( 0, "maindesktopLayout");
    theDesktopLayout = new QWidgetStack(parent);
    themaindesktopLayout->addWidget( theDesktopLayout );

    ////////////////////////////////////////////////////////////////
    // set up desktop display objects

    //initialize the desktop display
    initDesktopCanvas();

    //initialize the desktop table
    initDesktopTable();

    //set some variables
    clipnumber = globalclipnumber;

    //initialize the desktop mediatable
    initializeMediaTable(parent1);

    ////////////////////////////////////////////////////////////////
    //initialize the database and read in clips

    //this is where we would use our new database object
    theDatabase = new JahDatabase();
    theDatabase->InitializeDatabase();

    ////////////////////////////////////////////////////////////////
    //activate default ui selection the canvas
    theDesktopLayout->raiseWidget(0);

	setCurrentLoadDirectory(JahMediaPath + QString( "/media/"));
}

JahDesktop::~JahDesktop()
{
    ////////////////////////////////////////////////////////
    //JahDesktop Destructor
    QCanvasItemList l=canvas->allItems();
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
	{
        if ( (*it)->rtti() == imageRTTI )
		{
			ImageItem* pImageItem = ((ImageItem*)(*it));
			assetData asset = pImageItem->getClip();
			if(pImageItem->filmStrip() != 0)
			{
				asset.frameOffset = pImageItem->filmStrip()->getCentralFrameNumber();
				slotUpdateFrameOffset(asset);
			}
		}
	}

	//should check to make sure database is open
    theDatabase->CloseDatabase(*clipnumber);

	delete mediatable;
	delete theDatabase;
	delete themaindesktopLayout;
}

QCanvasItemList JahDesktop::allItems( )
{
	return canvas->allItems();
}

void JahDesktop::activate( )
{
	if ( m_started ) return;
	m_started = true;

    //reads all the assets in from the database and adds them 
    //to the desktop canvas and table
	if ( JahPrefs::getInstance().getQuickStart( ) )
    	RebuildDesktop();
}

//routine to create the main desktop canves
void JahDesktop::initDesktopCanvas() 
{
    thedesktopdisplay1 = new QHBox (theDesktopLayout, "desktopcanvas");
    theDesktopLayout->addWidget(thedesktopdisplay1,0);
    
    //build the main canvas object first
    //this is the one that needs to support drag n drop
    canvas = new QCanvas(thedesktopdisplay1); // canvas = new QCanvas(this);
    QColor back;    back.setHsv(35,35,35);
    canvas->setBackgroundColor(back);
    canvas->resize( 2048, 1536 );

    //build the node editor for the canvas
    editor = new FigureEditor(canvas,thedesktopdisplay1); 
    editor->setHScrollBarMode( FigureEditor::AlwaysOff );
    editor->setVScrollBarMode( FigureEditor::AlwaysOff );
    editor->showBackgroundSprite ( true );

    connect ( editor, SIGNAL(updateClipPosition(assetData)),	this, SLOT( updateClipPosition(assetData)) );
    connect ( editor, SIGNAL(updateClipSize(assetData)),		this, SLOT( updateClipSize(assetData)) );
    connect ( editor, SIGNAL(updateLockStatus(assetData)),		this, SLOT( updateLockStatus(assetData)) );
    connect ( editor, SIGNAL(updateFilmstripStatus(assetData)),	this, SLOT( updateFilmstripStatus(assetData)) );
    connect ( editor, SIGNAL(signalUpdateFrameOffset(assetData)),		this, SLOT( slotUpdateFrameOffset(assetData)) );
}

//accessed from the module switcher routine in JahControl::stopAnimation()
void JahDesktop::stopanimation()
{
    editor->playControlStop();
    mediatable->stopAnimation();
}

//routine to create the main desktop table
void JahDesktop::initDesktopTable()
{

    thedesktopdisplay2 = new QHBox (theDesktopLayout, "desktoptable");
    theDesktopLayout->addWidget(thedesktopdisplay2,1);

    //this is hardcoded for now... need to fix this
    const int numRows = 40;
    const int numCols = 11;

    table = new JahDesktopTable( numRows, numCols, thedesktopdisplay2 );

    connect ( table, SIGNAL(currentChanged ( int, int )), this, SLOT( selectTableRow(int,int)) );
    connect ( editor, SIGNAL(itemRenamed( int, QString )), table, SLOT( renameAsset(int, QString))); 

    table->show();

    globaltablenumber = 0;
}

//routine to add a asset to the desktop table
void JahDesktop::addItemToDesktopTable(int index, assetData thedata, QImage image )
{
    table->addAsset(index, globaltablenumber,thedata, image);

    //hack to also add asset to the mediatable
    mediatable->addAsset(index, globaltablenumber,thedata, image);

    //whenever a asset is added increment table number
    globaltablenumber += 1;
}

void JahDesktop::selectTableRow(int row, int )
{
    QString text = table->text(row, 2);

    int tableSelect = text.toInt();
	assetData asset = theDatabase->getImageFromDatabase( tableSelect );
	AssetTables::getInstance( )->setSelectedAsset( asset );
}

void JahDesktop::assetSelected( assetData asset )
{
	AssetTables::getInstance( )->setSelectedAsset( asset );
}

//////////////////////////////////////////////////
// the new mediatable is initalized here
void JahDesktop::initializeMediaTable( QHBox* parentbox)
{
    mediatable = new JahDesktopSideTable( 0, parentbox );
    connect ( mediatable, SIGNAL(assetSelected ( assetData )), this, SLOT( assetSelected(assetData)) );
    connect ( editor, SIGNAL(itemRenamed( int, QString )), mediatable, SLOT( renameAsset(int, QString))); 
}
 
//used to flip desktop display modes
void JahDesktop::toggleDesktopImages()
{
    theDesktopLayout->raiseWidget(0);
}

void JahDesktop::toggleDesktopTable()
{
    theDesktopLayout->raiseWidget(1);
}


void JahDesktop::clearAll() 
{

    //create a popup window saying are you really sure?
    //do a qdialog...
    //set up tracer

  jtrace->info( "JahDesktop::","called clear all...");

    int status = QMessageBox::critical( 0, "Clearing The Desktop",
                                           "You are about to Clear the Desktop!\n\n"
                                           "This will Erase all clips and images\n"
                                           "in your projects Renders directory!\n\n"
                                           "Export files you want to keep first!\n\n"
                                           "Do you still want to Clear All?\n",
                                           QMessageBox::Yes | QMessageBox::Default,
                                           QMessageBox::No | QMessageBox::Escape, 0 );


    switch (status) {

     case QMessageBox::Yes    : { clearTheDesktop(); break; }

     case QMessageBox::No     : { break; }

     case QMessageBox::Cancel : { break; }

     default :                  { break; }

   }

}

struct sort_by_item
{
	bool operator( )( const ImageItem* const left, const ImageItem* const right )
	{
		return *left < *right;
	}
};

void JahDesktop::tidyDesktop()
{
	int status = QMessageBox::question( 0, "Tidy the Desktop",
										   "This will move and re-size all clips\n"
										   "and images on your desktop.\n\n"
										   "Choose Yes to tidy-up the desktop.\n",
										   QMessageBox::Yes | QMessageBox::Default,
										   QMessageBox::No | QMessageBox::Escape, 0 );

	if ( status == QMessageBox::Yes )
	{
		int counter = 0 ;
		QCanvasItemList l=canvas->allItems();

		std::vector< ImageItem * > list;

		for (QCanvasItemList::Iterator it= l.end(); it!= l.begin(); )
		{
			it --;
			if ( (*it)->rtti() == imageRTTI )
				list.push_back( (ImageItem*)(*it) );
		}

		std::sort( list.begin( ), list.end( ), sort_by_item( ) );

		counter = list.size( );

		editor->hideDecorations(); // (we cant easily resize border or text etc from here)
		editor->showHidePlayerControls( false );// (ditto)
		
		int full_w = editor->visibleWidth( );
		int full_h = editor->visibleHeight( );

		int itwidth = 180, itheight = 180;
		int wgap = 15, hgap = 15;
		bool fits = false;

		while( !fits && itwidth > 20 )
		{
			int per_row = full_w / ( itwidth + wgap );
			int per_column = full_h / ( itheight + hgap );
			fits = ( per_row * per_column ) >= counter;
			if ( !fits )
			{
				itwidth -= itwidth / 10;
				itheight -= itheight / 10;
				wgap = wgap > 0 ? wgap - 1 : 0;
				hgap = hgap > 0 ? hgap - 1 : 0;
			}
		}

		int deltay = ( itheight + hgap );
		int itemsperrow = int ( editor->visibleWidth() / ( itwidth + wgap ) ); 

		assetData itemData;
		int itn = 0;
		int irow = 0, icol = 0;

		for ( std::vector< ImageItem * >::iterator it = list.begin(); it != list.end(); it ++ )
		{
			assetData itemdata;
			ImageItem *item= (*it); 
			itemdata = item->getClip();

			if ( item->filmStrip() != 0 )
			{
				item->deleteFilmStrip( );
				itemdata.filmstrip = false;
				updateFilmstripStatus( itemdata );
			}

			int newx = wgap + icol * ( itwidth + wgap );
			int newy = hgap + irow * ( deltay );
			bool resize = false;

			if ( !( item->size().width() == itwidth || item->size().height() == itheight ) )
			{
				item->resize( itwidth, itheight );
				itemdata.xsize = item->width();
				itemdata.ysize = item->height();
				updateClipSize( itemdata );
				resize = true;
			}

			if ( item->x() != newx || item->y() != newy ) 
			{
				item->setX( newx );
				item->setY( newy );
				itemdata.xpos = item->x();
				itemdata.ypos = item->y();
				updateClipPosition(itemdata);
			}

			if ( resize && item->filmStrip() == 0 )
			{
				editor->swapItem( item ); // swaps the globalitem and resets assetExchange
				editor->advanceClipFrame (0, false); // trigger a re-scaled image, since we re-sized item.	   
			}

			itn++;
			icol++;
			if ( icol % itemsperrow == 0)
			{   
				icol = 0;
				irow++;
			} 
		}

		if(qApp->hasPendingEvents())
			qApp->processEvents();
	}

	editor->hideDecorations(); // (we cant easily resize border or text etc from here)
	editor->showHidePlayerControls( false );// (ditto)
	editor->ensureVisible( 0, 0 );
	canvas->setAllChanged();
}

void JahDesktop::slotExpand()
{   // TODO collapse onto the frame closest to border. Requires new function in filmstrip. 

    if ( !getCurrentItem() )
        return;

    assetData itemdata;
    itemdata = getCurrentItem()->getClip();
                 
    if ( getCurrentItem()->filmStrip() != 0 ) // already expander
    {   getCurrentItem()->deleteFilmStrip();
        editor->bringToFront( getCurrentItem() );
    }
    else
    {
        if ( ( itemdata.theCategory == VideoCategory::CLIP || itemdata.theCategory == VideoCategory::MEDIA) )
        {
            if ( editor->filmStripAllowed() )
				editor->createFilmStrip(editor->visibleWidth());
        }
    }
}

//this is used when we rebuild the database as it doesnt append the clipnumber to the images
ImageItem* JahDesktop::addImage( assetData thedata, QImage theimage )
{
    //we need to scale the proxy size here instead of using the default 0.25% reduction
    //float wratio = (180/(float)theimage.width() );
    //float hratio = (120/(float)theimage.height() );

    QString theclipnumber;
    theclipnumber.setNum(thedata.clipnumber);

    QString clipper1; clipper1= thedata.clipname;

    img = new QImage[2];
    //img[1] = theimage.smoothScale( int(theimage.width()*wratio), int(theimage.height()*hratio), QImage::ScaleFree );
   	img[1] = theimage.scale( int(thedata.xsize), int(thedata.ysize), QImage::ScaleMin );

    // we need to update this so that we dont have to send the thedata.Image just thedata...
    ImageItem *i = new ImageItem(thedata, img[1],canvas, clipper1);
      i->setPen( QPen(QColor(200,0,0), 6) );
      i->setZ(1);
      i->setX(thedata.xpos);
      i->setY(thedata.ypos);
      i->show();

    delete [] img;      // is this necessary as img is not a pointer???

	return i;
}

//this is used to add a image to the desktop
ImageItem* JahDesktop::addImage2( assetData thedata, QImage theimage )
{
    //we need to scale the proxy size here instead of using the default 0.25% reduction
    //float wratio = (180/(float)theimage.width() );
    //float hratio = (120/(float)theimage.height() );

    QString theclipnumber;
    theclipnumber.setNum(thedata.clipnumber);

    QString clipper1; clipper1= thedata.clipname;
    clipper1.append(theclipnumber);

    img = new QImage[2];
    //img[1] = theimage.smoothScale( int(theimage.width()*wratio), int(theimage.height()*hratio), QImage::ScaleMin );
	img[1] = theimage.scale( int(thedata.xsize), int(thedata.ysize), QImage::ScaleMin );

    // we need to update this so that we dont have to send the thedata.Image just thedata...
    ImageItem *i = new ImageItem(thedata, img[1],canvas, clipper1);
      i->setPen( QPen(QColor(200,0,0), 6) );
      //i->setZ(1);
      i->setX(thedata.xpos);
      i->setY(thedata.ypos);
      editor->bringToFront( (ImageItem*) i );
      i->show();

    delete [] img;      // is this necessary as img is not a pointer???
	return i;
}

//this is used to add a image to the desktop
ImageItem* JahDesktop::addImage2( assetData thedata, double& xsize, double& ysize, QImage theimage )
{
    QString theclipnumber;
    theclipnumber.setNum(thedata.clipnumber);

    QString clipper1; clipper1= thedata.clipname;
    clipper1.append(theclipnumber);

    float ratio = (float)theimage.width() / (float)theimage.height();
	int w = 180;
	int h = int( w / ratio );

	if ( h > 120 )
	{
		h = 120;
		w = int( h * ratio );
	}

    img = new QImage[2];
    img[1] = theimage.scale( w, h, QImage::ScaleMin );

    // we need to update this so that we dont have to send the thedata.Image just thedata...
    ImageItem *i = new ImageItem(thedata, img[1],canvas, clipper1);
      i->setPen( QPen(QColor(200,0,0), 6) );
      //i->setZ(1);
      i->setX(thedata.xpos);
      i->setY(thedata.ypos);
      editor->bringToFront( (ImageItem*) i );
      i->show();

    delete [] img;      // is this necessary as img is not a pointer???

    xsize = i->width();
    ysize = i->height();

	return i;
}



ImageItem* JahDesktop::getCurrentItem()
{

    return editor->globalitem;

}

void JahDesktop::addRectangle()
{
    QCanvasPolygonalItem *i = new QCanvasRectangle( 0,0, 180,120,canvas);
    i->setBrush( QColor(50,0,200) );
    i->setPen( QPen(QColor(0,50,200), 6) );
    i->setZ(1);
    i->show();
}

////////////////////////////////////////////////////////////
//this should return the last selected clip?
//weather its in the table or on the desktop...

assetData JahDesktop::getVideoData(bool & status)
{
	assetData &asset = AssetTables::getInstance( )->getSelectedAsset( );
	status = asset.getFullFileName( ) != "";
    return asset;
}

void JahDesktop::selectNextAsset( )
{
	int current = mediatable->getTable( )->currentRow( );
	if ( current >= 0 && current < mediatable->getTable( )->numRows( ) - 1 )
		mediatable->getTable( )->selectRow( current + 1 );
	else
		mediatable->getTable( )->selectRow( 0 );
}

void JahDesktop::syncMediaTable( assetData data )
{
	int index = mediatable->find( data );
	if ( index != mediatable->getTable( )->currentRow( ) && index >= 0 && index < mediatable->getTable( )->numRows( ) )
		mediatable->getTable( )->selectRow( index );
}

void JahDesktop::setClipname(QString& newname)
{
     editor->setItemtext(newname);
}

void JahDesktop::doRefresh(void)
{
    //canvas->setAllChanged();
}

QString JahDesktop::getClipname()
{
    return editor->imagetext;
}

////////////////////////////////////////////////////////
// was supposed to be for drag and drop on the desktop
// but that needs to actually be in the canvas itself
// so we need to subclass the qcanvas, add drag n drop 
//and use the new class for this to work
void JahDesktop::dragEnterEvent(QDragEnterEvent* event)
{
    //debug("drag event");

    event->accept(
        QTextDrag::canDecode(event) ||
        QImageDrag::canDecode(event)
    );
}

void JahDesktop::dropEvent(QDropEvent* event)
{
    QImage image;
    QString text;

    //debug("drop event");

    if ( QImageDrag::decode(event, image) )
    {
        //debug("drop image event");
    }
    else if ( QTextDrag::decode(event, text) )
    {
       //debug("drop text event");
    }
}

void JahDesktop::selectAsset( assetData &asset )
{
	// Horrible hack - ignore coming from the editor media table
	if ( asset.extension != ".jfx" )
	{
		QCanvasItemList l=canvas->allItems();

		// Try to have the desktop agree on the selected asset even though it 
		// won't reflect it in the ui (see comments below)
		for (QCanvasItemList::Iterator it= l.end(); it!= l.begin(); )
		{
			it --;
			if ( (*it)->rtti() == imageRTTI )
			{
				ImageItem *item= ( ImageItem * )(*it); 
				assetData itemdata = item->getClip();
				if ( asset.clipnumber == itemdata.clipnumber )
				{
					if ( item != editor->globalitem )
					{
						// Sigh, because we can't actually properly select something without a mouse
						// click, the best we can do is hide the decorations associated to the previous item
						editor->hideDecorations( );
						editor->showHidePlayerControls( false );
						editor->ensureVisible( 0, 0 );
						editor->swapItem( item );
					}
					break;
				}
			}
		}
	}
}



QString JahDesktop::getModuleName()
{
  return ModuleName;
}


bool BackgroundSprite::isValid ()
    {
      if (image())
	{
	  return !image()->isNull();
	}
      return false;
    }
