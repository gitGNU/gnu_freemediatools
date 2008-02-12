/*******************************************************************************
**
** The source file for the Jahshaka dirview module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "dirview.h"
#include <stdexcept>

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qpixmap.h>
#include <qevent.h>
#include <qpoint.h>
#include <qmessagebox.h>
#include <qdragobject.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qstringlist.h>
#include <qapplication.h>
#include <qheader.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qregexp.h>

#include "Asset.h"
using namespace olib::openassetlib;

#include <qcombobox.h>
#include "iconprovider.h"
#include "embedded-images.h" // from qembed script

static const char* folder_closed_xpm[]={
    "16 16 9 1",
    "g c #808080",
    "b c #c0c000",
    "e c #c0c0c0",
    "# c #000000",
    "c c #ffff00",
    ". c None",
    "a c #585858",
    "f c #a0a0a4",
    "d c #ffffff",
    "..###...........",
    ".#abc##.........",
    ".#daabc#####....",
    ".#ddeaabbccc#...",
    ".#dedeeabbbba...",
    ".#edeeeeaaaab#..",
    ".#deeeeeeefe#ba.",
    ".#eeeeeeefef#ba.",
    ".#eeeeeefeff#ba.",
    ".#eeeeefefff#ba.",
    ".##geefeffff#ba.",
    "...##gefffff#ba.",
    ".....##fffff#ba.",
    ".......##fff#b##",
    ".........##f#b##",
    "...........####."};

static const char* folder_open_xpm[]={
    "16 16 11 1",
    "# c #000000",
    "g c #c0c0c0",
    "e c #303030",
    "a c #ffa858",
    "b c #808080",
    "d c #a0a0a4",
    "f c #585858",
    "c c #ffdca8",
    "h c #dcdcdc",
    "i c #ffffff",
    ". c None",
    "....###.........",
    "....#ab##.......",
    "....#acab####...",
    "###.#acccccca#..",
    "#ddefaaaccccca#.",
    "#bdddbaaaacccab#",
    ".eddddbbaaaacab#",
    ".#bddggdbbaaaab#",
    "..edgdggggbbaab#",
    "..#bgggghghdaab#",
    "...ebhggghicfab#",
    "....#edhhiiidab#",
    "......#egiiicfb#",
    "........#egiibb#",
    "..........#egib#",
    "............#ee#"};

static const char * folder_locked[]={
    "16 16 10 1",
    "h c #808080",
    "b c #ffa858",
    "f c #c0c0c0",
    "e c #c05800",
    "# c #000000",
    "c c #ffdca8",
    ". c None",
    "a c #585858",
    "g c #a0a0a4",
    "d c #ffffff",
    "..#a#...........",
    ".#abc####.......",
    ".#daa#eee#......",
    ".#ddf#e##b#.....",
    ".#dfd#e#bcb##...",
    ".#fdccc#daaab#..",
    ".#dfbbbccgfg#ba.",
    ".#ffb#ebbfgg#ba.",
    ".#ffbbe#bggg#ba.",
    ".#fffbbebggg#ba.",
    ".##hf#ebbggg#ba.",
    "...###e#gggg#ba.",
    ".....#e#gggg#ba.",
    "......###ggg#b##",
    ".........##g#b##",
    "...........####."};

static const char * pix_file []={
    "16 16 7 1",
    "# c #000000",
    "b c #ffffff",
    "e c #000000",
    "d c #404000",
    "c c #c0c000",
    "a c #ffffc0",
    ". c None",
    "................",
    ".........#......",
    "......#.#a##....",
    ".....#b#bbba##..",
    "....#b#bbbabbb#.",
    "...#b#bba##bb#..",
    "..#b#abb#bb##...",
    ".#a#aab#bbbab##.",
    "#a#aaa#bcbbbbbb#",
    "#ccdc#bcbbcbbb#.",
    ".##c#bcbbcabb#..",
    "...#acbacbbbe...",
    "..#aaaacaba#....",
    "...##aaaaa#.....",
    ".....##aa#......",
    ".......##......."};

QPixmap *folderLocked = 0;
QPixmap *folderClosed = 0;
QPixmap *folderOpen = 0;
QPixmap *fileNormal = 0;

//?IconProvider*  iconprovider = 0;

/*****************************************************************************
 *
 * Class Directory
 *
 *****************************************************************************/

Directory::Directory( Directory * parent,  QString filename, Type type )
	: QListViewItem( parent )
	, dsource(0)
	, f(filename)
	, p( parent )
	, showDirsOnly( parent->showDirsOnly )
	, pix( 0 )
	, dtype(type)
	, sizetoshow(0)
{
    init();
}

Directory::Directory( QListView * parent, QString filename, Type type )
    : QListViewItem( parent )
	, dsource(0)
	, f(filename)
	, p(0)
	, showDirsOnly( ( (DirectoryView*)parent )->showDirsOnly() )
	, pix( 0 )
	, dtype( type )
	, sizetoshow(0)
{
    init();
}

void Directory::init()
{
    if ( dirType() == FileFolder || dirType() == DriveRoot )
        readable = QDir( fullName() ).isReadable();
    else 
        readable = true; // assume anything else is 'readable' for now.
    
   setPixmap();
}

void Directory::setPixmap( QPixmap *px )
{
    pix = px;
    setup();
    widthChanged( 0 );
    invalidateHeight();
    repaint();
}

void Directory::setPixmap()
{

	if ( dirType() == FileFolder  )
	{ 
		if ( !readable )
		{   
			setPixmap( folderLocked );
		}
		else
		{   
			QFileInfo fi("/"); 
			setPixmap( IconProvider::instance().pixmap( &fi, true ));

			/* else
			{   //printf("no icon provider\n"); 
			setPixmap( folderClosed );
			}
			*/

		}
	}// TODO FINISH ALL ICONS?
	else if ( dirType() == DriveRoot ||dirType() == ComputerRoot )
	{ 
		setPixmap( IconProvider::instance().hardDriveSmallPixmap()  );

	}
}


const QPixmap *Directory::pixmap( int i ) const
{
    if ( i )
	return 0;
    return pix;
}

void Directory::setOpen( bool o )
{
    if  ( dirType() == FileFolder )    //&& !QDir(fullName()).isRoot() )  // old
    {
		if ( o )
		{      
			QFileInfo fi("/"); 
			setPixmap( IconProvider::instance().pixmap( &fi, true )); // TO FIX, OPEN!?

			/*  else
			{   
			setPixmap( folderOpen );
			}
			*/
		}
		else 
		{
			QFileInfo fi("/"); 
			setPixmap( IconProvider::instance().pixmap( &fi, true )); 

			/* else
			{   
			setPixmap( folderClosed );
			}
			*/
		}
    }
    else if ( dirType() == Collection )
    {
        setPixmap( new QPixmap( qembed_findImage("collection-iconc") ));
        //printf("just set icon for %s\n", text(0).latin1() );
    }

    if ( o && !childCount() ) {
	QString s( fullName() );
	QDir thisDir( s );
	if ( !thisDir.isReadable() ) {
	    readable = FALSE;
	    setExpandable( FALSE );
	    return;
	}

	listView()->setUpdatesEnabled( FALSE );
    // NB we disabled sorting on the listview, so they get added in the order we send them, hence
    // need reverse name sort order here.
    const QFileInfoList * files = thisDir.entryInfoList( QDir::DefaultFilter, QDir::Name | QDir::Reversed | QDir::IgnoreCase );
	if ( files ) {
	    QFileInfoListIterator it( *files );
      //  it = files->last();
	    QFileInfo * fi;
	    while( (fi=it.current()) != 0 ) {
		++it;
		if ( fi->fileName() == "." || fi->fileName() == ".." )
		    ; // nothing
		else if ( fi->isSymLink() && !showDirsOnly ) {
		    FileItem *item = new FileItem( this, fi->fileName(),  // NB we no longer show files in the dirlist.
						     "Symbolic Link" );
		    item->setPixmap( fileNormal );
		}
		else if ( fi->isDir() )
        {    new Directory( this, fi->fileName() );
        }
		else if ( !showDirsOnly ) {
		    FileItem *item
			= new FileItem( this, fi->fileName(),
					     fi->isFile()?"File":"Special" );
		    
			item->setPixmap( IconProvider::instance().pixmap( fi, true ));

			// else
			//   item->setPixmap( fileNormal );
		}
	    }
	}
	listView()->setUpdatesEnabled( TRUE );
    }
    QListViewItem::setOpen( o );
}


void Directory::setup()
{
    setExpandable( TRUE );
    QListViewItem::setup();
}


QString Directory::fullName()
{
    // Note, some items arent really folders -- we can deal with those too. Since we can only deal in 
    // text (due to Dnd and mimetypes from other widgets) we have to use some 'magic' characters to 
    // designate collections and normal folders etc. 
    // '?' is used for search results inside search root.
    // '$' is used for a collection inside a library (local or remote).

    QString s;
    if ( p ) 
	{
        QString prepend;
        
        if ( p->dirType() == FileFolder || p->dirType() == DriveRoot )
        {    prepend = p->fullName();
             s = prepend;
             s.append( "/" );
             s.append( f.name() );
             s.append( "/" );
        }
        else if ( p->dirType() == LocalDataSource || p->dirType() == RemoteDataSource )
        {
            prepend = p->fullName();
            prepend.append("$");
            s = prepend;
            s.append( f.name() );

        }
        else if ( p->dirType() == SearchRoot )
        {   prepend = p->fullName();
            prepend.append("?");
            s = prepend;
            s.append( f.name() );
        }
        else
        {   s = f.name();
            
        }

        s.append( "/" );
        



        /* OLD WAY
        if ( p->dirType() != FileFolder  && p->dirType() != DriveRoot && p->dirType() != LocalDataSource )
            prepend = ""; // dont want to add anything
        
      
            

        s = prepend;
        s.append( f.name() );
	    s.append( "/" );
        */
        
    } 
	else 
	{
		s = f.name();
    }
    return s;
}

QString Directory::text( int column ) const
{   return text( column, true );
}

QString Directory::text( int column, bool showstatus ) const
{
    // For certain types we show the 'contents' size too.
    QString conv;
    QString append; 
    if ( showstatus && column == 0 && sizetoshow >= 0 )
    {    conv.setNum( sizetoshow );
         append = " (" + conv + ")";
    }

    if ( column == 0 && ( dirType() == SearchRoot || dirType() == Collection ))
	    return (f.name() + append);
    else if ( column == 0 &&  ( dirType() == RemoteDataSource ) )
    {   QString status;
        if ( showstatus )
            status = ( isReadable() ?  append :  " (disconnected)   " );
        return ( f.name() +  status );
    }
    else if ( column == 0 && dirType() == LocalDataSource )
    {   return f.name();
    }
    else if ( column == 0 )
        return f.name();
    else if ( column == 1 && dirType() == FileFolder && readable )
	    return "Directory" ;
    else if ( column == 1 && dirType() == DriveRoot )
	    return "Drive" ;
    else if ( column == 1 && dirType() == Collection )
	    return "Collection";
    else if ( column == 1 && dirType() == SearchResult )
	    return "Search Result";
    else if ( column == 1 && dirType() == RemoteDataSource )
        return "Network library";
    else if ( column == 1 && dirType() == LocalDataSource )
	    return "Local library";
    else if ( column == 1 && dirType() == MediaRoot )
	    return "Media";
    else if ( column == 1 && dirType() == SearchRoot )
	    return "";

    return "";
}

void Directory::populateCollections()
{
	if ( dirType() != LocalDataSource && dirType() != RemoteDataSource )
        return;

    std::list<std::string>	collections = dataSource()->collectionNames();
    if ( collections.empty() ) 
        return;

    std::list<std::string>::iterator it = collections.begin();
    while ( it != collections.end() )
    {
        std::string colname = *it;
        Directory* collection = new Directory( this, colname.c_str(), Directory::Collection);

        collection->setDataSource( this->dataSource() );	
        collection->setSizeToShow( collection->dataSource()->numAssets( colname.c_str() ));

        
        ++it;
    }
    setSizeToShow( collections.size() );
}

void Directory::paintCell(  QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
    QColorGroup changecg( cg );

    if ( column %2 ) // could change bg color of this column
	{	
		//changecg.setColor( QColorGroup::Base, Qt::red );
    }

	// Let the base class do the rest of the work:
	QListViewItem::paintCell( p, changecg, column, width, align);
}


/*****************************************************************************
 *
 * Class DirectoryView
 *
 *****************************************************************************/

DirectoryView::DirectoryView( QWidget *parent, const char *name, bool sdo )
    : QListView( parent, name ), dirsOnly( sdo ), oldCurrent( 0 ),
      dropItem( 0 ), oldSelected( 0 ), autoopen_timer( 0 ),
	  selectopen_timer( 0 ), mousePressed( FALSE )
{
    QFont ourfont( this->font() );
    ourfont.setPointSize( ourfont.pointSize() +2  );
    setFont( ourfont );

    addColumn( "Name" );
    addColumn( "Type" );
    setSelectionMode( QListView::Single );  
    setShowSortIndicator(true);

    autoopen_timer = new QTimer( this );
    selectopen_timer = new QTimer( this );

    if ( !folderLocked ) {
	folderLocked = new QPixmap( folder_locked );
	folderClosed = new QPixmap( folder_closed_xpm );
	folderOpen = new QPixmap( folder_open_xpm );
	fileNormal = new QPixmap( pix_file );
    }

    setRootIsDecorated( true );


    connect(this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int)),
         this, SLOT ( contextMenuCreator( QListViewItem*, const QPoint&, int)));
    setAcceptDrops( TRUE );
    viewport()->setAcceptDrops( TRUE );

    connect( autoopen_timer, SIGNAL( timeout() ),
	     this, SLOT( openFolder() ) );


    // We want folders to open when single-clicking or when selected via keyboard, but only after 
    // a delay if keyboard used (helper slot deals with that).
    //connect( this, SIGNAL( doubleClicked( QListViewItem * ) ),  // Double-click signal no longer used.
	//     this, SLOT( slotFolderSelected( QListViewItem * ) ) );
    connect( this, SIGNAL( returnPressed( QListViewItem * ) ),
	     this, SLOT( slotFolderSelected( QListViewItem * ) ) );
   
    
     connect( this, SIGNAL( mouseButtonPressed( int, QListViewItem*, const QPoint&, int ) ),
	     this, SLOT( slotFolderSelectedViaPress( int, QListViewItem*, const QPoint&, int ) ) );
//OLD    connect( this, SIGNAL( pressed( QListViewItem * ) ),
//	     this, SLOT( slotFolderSelected( QListViewItem * ) ) );



    connect( this, SIGNAL( selectionChanged()), this, SLOT( selectionChangedHelperSlot()));
    connect( selectopen_timer, SIGNAL( timeout() ),
	     this, SLOT( delayedSlotFolderSelection() ) );
}


static const int autoopenTime = 750;
static const int selectopenTime = 750;

void DirectoryView::selectionChangedHelperSlot()
{
    selectopen_timer->start(selectopenTime); //will fire off delayedFolderSelection after short time.
}

void DirectoryView::slotFolderSelected( QListViewItem *i )
{
    //printf("in slotFolderSelected\n");
    selectopen_timer->stop();

    Directory *dir = dynamic_cast<Directory*>(i);
    if ( !dir ) 
        return;

    // check for network library
    if ( dir->dirType() == Directory::RemoteDataSource && !dir->isReadable() )
    {   
        //ConnectDialog *con = new ConnectDialog(0);
        //con->addressComboBox->setCurrentText( dir->text(0, false) );
        //con->exec();
    }
    
    if ( !i || !showDirsOnly() )
    {  //printf("*** dirview cant emit... %i \n", showDirsOnly());
        return;
    }

    // NB we always check the network drive items, even if clicked before.
    // If we click the same item again, just make sure its open, dont trigger other signals.
    if ( oldSelected == i && false)
    {   dir->setOpen( true );
        return;
    }

    //printf("dirview emiting folderSelected %s \n", dir->fullName().latin1() );
    emit folderSelected( dir->fullName() );
    dir->setOpen( true );

    oldSelected = i; 
}

void DirectoryView::slotFolderSelectedViaPress( int button, QListViewItem * item, const QPoint &, int ) 
{
    //printf("in slotFolderSelectedViaClick button is %i \n", button );
    if ( button == 1 )
        slotFolderSelected( item );
}

void DirectoryView::delayedSlotFolderSelection()
{
    //printf(" in delayedSlotFolderSelection\n" );
    selectopen_timer->stop();
    if ( oldSelected == currentItem() )
        return;
    Directory *dir = dynamic_cast<Directory*>(currentItem());
    if ( dir && currentItem() )
    {    folderSelected( dir->fullName() );
         // NO, keyboard does not open folders. dir->setOpen( true );
    }
    oldSelected = currentItem(); 
}

void DirectoryView::openFolder()
{
    autoopen_timer->stop();
    if ( dropItem && !dropItem->isOpen() ) {
	dropItem->setOpen( TRUE );
	dropItem->repaint();
    }
}

void DirectoryView::contentsDragEnterEvent( QDragEnterEvent *e )
{
    if ( !QUriDrag::canDecode(e) ) {
	e->ignore();
	return;
    }

    oldCurrent = currentItem();

    QListViewItem *i = itemAt( contentsToViewport(e->pos()) );
    if ( i ) {
	dropItem = i;
	autoopen_timer->start( autoopenTime );
    }
}


void DirectoryView::contentsDragMoveEvent( QDragMoveEvent *e )
{
    if ( !QUriDrag::canDecode(e) ) {
	e->ignore();
	return;
    }

    QPoint vp = contentsToViewport( ( (QDragMoveEvent*)e )->pos() );
    QListViewItem *i = itemAt( vp );
    if ( i ) {
	setSelected( i, TRUE );
	e->accept();
	if ( i != dropItem ) {
	    autoopen_timer->stop();
	    dropItem = i;
	    autoopen_timer->start( autoopenTime );
	}
	switch ( e->action() ) {
	case QDropEvent::Copy:
	    break;
	case QDropEvent::Move:
	    e->acceptAction();
	    break;
	case QDropEvent::Link:
	    e->acceptAction();
	    break;
	default:
	    ;
	}
    } else {
	e->ignore();
	autoopen_timer->stop();
	dropItem = 0;
    }
}

void DirectoryView::contentsDragLeaveEvent( QDragLeaveEvent * )
{
    autoopen_timer->stop();
    dropItem = 0;

    setCurrentItem( oldCurrent );
    setSelected( oldCurrent, TRUE );
}

void DirectoryView::contentsDropEvent( QDropEvent *e )
{
    autoopen_timer->stop();

    if ( !QUriDrag::canDecode(e) ) {
        e->ignore();
        return;
    }

    QListViewItem *item = itemAt( contentsToViewport(e->pos()) );
    Directory *dir = dynamic_cast<Directory*>(item);

    if ( dir ) {

        //?QStrList lst;
        QStringList lst;

        QUriDrag::decodeToUnicodeUris( e, lst );

        QString str;

        switch ( e->action() ) {
        case QDropEvent::Copy:
            str = "Copy";
            break;
        case QDropEvent::Move:
            str = "Move";
            e->acceptAction();
            break;
        case QDropEvent::Link:
            str = "Link";
            e->acceptAction();
            break;
        default:
            str = "Unknown";
        }

        str += "\n\n";

        e->accept(); // dont always accept?

        //for ( uint i = 0; i < lst.count(); ++i ) 
        for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
        {
            //  QString filename = lst.at( i );
            QString filename = *it;
            str += filename + "\n";
        }
        str += QString( "\nTo\n\n   %1" )
            .arg( fullPath(item) );

        //printf(" dropped onto %s \n", dropItem->text(0).latin1() );
        //printf(" drop old current name  %s \n", oldCurrent->text(0).latin1() );

        // Deal with each item type drop.
        // Currently, the drop from *ourselves* can only be one item, and we know the last selected item in 
        // oldCurrent, hence we know the original drag item. If we allow mult select, we need 
        // to change this. 
        Directory * olddir = dynamic_cast<Directory*>( oldCurrent );
        bool warningshown = false;

        if ( dir->dirType() == Directory::Collection )
        {   for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
            {
                QString filename = *it;
                QString tidyfilename = filename.replace("file:///", "" ); // QFile doesnt like the file:/// ?
                QFileInfo fi( tidyfilename );
                //printf("file dropped was %s\n", filename.latin1() );
                QFile file (tidyfilename );
                if ( file.exists() )
                {
                    //printf("drop to collection %s \n", filename.latin1() );
                    Asset *asset = new Asset( (fi.dirPath() + "/").latin1(), fi.fileName().latin1(), 0, 0 );
                    bool ok = true;
                    try 
                    {   
						dir->dataSource()->addAsset( dir->text(0, false ).latin1(), asset );
                    }
                    catch(std::exception& e)
                    {   if ( !warningshown )
                            QMessageBox::information( this, "Error" , e.what(), "OK"  );
                        warningshown = true;
                        ok = false;
                    }
                    catch ( ... )
                    {   QMessageBox::information( this, "Error" , "Problem connecting to local database.", "OK"  );
                        ok = false;
                    }

                    if ( ok )
                    {   dir->setSizeToShow( dir->sizeToShow() + 1 );
                       
                    }

                }
                else
                {    //printf("file %s does not exist, ignoring.\n", filename.latin1() );
                }
            
            }
           
            // Update our view, and tell other widgets that we updated.
            dir->repaint();
            // TODO should be refresh if displaying on RHS, not reload all again?
            emit folderSelected( dir->fullName() ); 
        }
		
        if ( dir->dirType() == Directory::TrashCan )
        {   
            for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it )
            {
                QString filename = *it;
                //printf(" dropped item is %s \n", filename.latin1() );
            }

            // NO to fix. Could drag other items!
            if ( olddir && olddir->dirType() == Directory::Collection )
            {
                bool ok = true; 

                switch( QMessageBox::information( this, "Delete collection",
                    "Are you sure you want to delete this collection? Assets no longer\n"
                    "in any collection will be removed from the data base.",
                    QMessageBox::Ok, QMessageBox::Cancel ) ) 
                { 
                case QMessageBox::Ok: 
                    {
                        deleteCollection( olddir );
                        break;
                    }
                default :
                    {   ok = false;
                        break;
                    }
                }
            }
        }
    } 
    else
        e->ignore();

}

void DirectoryView::keyPressEvent ( QKeyEvent * e )
{
  //  if ( e->key() == Qt::Key_Control )
    //    controlPressed = !controlPressed;

    if ( !currentItem() )
        return; 

    Directory *dir = dynamic_cast<Directory*>( currentItem() );
    if ( !dir )
        return;

    // Could add various CNTRL-N options here. See right-click context menu also.

    if ( e->key() == Qt::Key_Delete && dir->dirType() == Directory::Collection )
        deleteSelectedCollection();
    else if ( e->key() == Qt::Key_N  && e->state() == Qt::ControlButton )
    {   //printf( "ctrl n\n" );
        if ( dir->dirType() == Directory::LocalDataSource )
            newCollection();
    }
    else 
        QListView::keyPressEvent( e );

}

QString DirectoryView::fullPath(QListViewItem* item)
{
    // Add all the parents to our path. 
    // Note, we can deal with special cases of item too.
    //printf("in fullPath %s\n" , item->text(0).latin1() );

    QString fullpath = item->text(0);
    while ( (item=item->parent()) ) {
	
    if ( item ) // check for type?  // was if item->parent()
        fullpath = item->text(0) + "/" + fullpath; 
    else
	    fullpath = item->text(0) + fullpath;
    }
    return fullpath;
}

void DirectoryView::contentsMousePressEvent( QMouseEvent* e )
{
    QListView::contentsMousePressEvent(e);
    
    QPoint p( contentsToViewport( e->pos() ) );
    QListViewItem *i = itemAt( p );
    if ( !i ) 
        return;
    else 
    {
	// if the user clicked into the root decoration of the item, don't try to start a drag!
	if ( p.x() > header()->cellPos( header()->mapToActual( 0 ) ) +
	     treeStepSize() * ( i->depth() + ( rootIsDecorated() ? 1 : 0) ) + itemMargin() ||
	     p.x() < header()->cellPos( header()->mapToActual( 0 ) ) ) 
        {
	        presspos = e->pos();
	        mousePressed = TRUE;
	    }
      /*  TO REMOVE
        Directory *dir = 0;
        dir = dynamic_cast<Directory*>(i);
        emit directoryClicked();
        */
    }
}

void DirectoryView::contentsMouseDoubleClickEvent ( QMouseEvent * e )
{
    // We could do something else double clicks, or just duplicate single click behaviour.
    e->ignore();
}

void DirectoryView::contentsMouseMoveEvent( QMouseEvent* e )
{
    if ( mousePressed && ( presspos - e->pos() ).manhattanLength() > QApplication::startDragDistance() ) {
	mousePressed = FALSE;
	QListViewItem *item = itemAt( contentsToViewport(presspos) );
	if ( item ) {
	    QString source = fullPath(item);
        if ( 1 || QFile::exists(source)  ){ //  TO FIX! fullpath will contain our special strings.
		    QUriDrag* ud = new QUriDrag(viewport());
		    ud->setUnicodeUris( source );
		    if ( ud->drag() )
		        QMessageBox::information( this, "Drag source",
					      QString("Delete ")+source, "Not implemented" );
	        }
	    }
    }
}

void DirectoryView::contextMenuCreator( QListViewItem *item, const QPoint &, int )
{
    //printf("right pressed  %i \n", col );

    QPopupMenu* contextMenu = new QPopupMenu( this );
    //QLabel *caption = new QLabel( "<b>"
    //    "Context Menu</b>", this );
    //caption->setAlignment( Qt::AlignCenter );
    //contextMenu->insertItem( caption );

    Directory *dir = 0;
    dir = (Directory*)(item);
    if ( item == 0 )
    {    
         QPixmap pm = QPixmap( qembed_findImage("minidb-16w-add") );
         contextMenu->insertItem( QIconSet( pm ), "Add new data source...",  this, SLOT(TODO())  );
    }
    else if ( dir->dirType() == Directory::LocalDataSource )
    {
        contextMenu->insertItem( "New collection",  this, SLOT(newCollection())  );
    }
    else if ( dir->dirType() == Directory::RemoteDataSource )
    {
        contextMenu->insertItem( "Connect...",  this, SLOT(TODO())  );
        contextMenu->insertItem( "Disconnect",  this, SLOT(TODO())  );
    }
    else if ( dir->dirType() == Directory::Collection )
    {
        contextMenu->insertItem( "Rename collection",  this, SLOT(renameCollection())  );
        contextMenu->insertItem( "Delete collection",  this, SLOT(deleteSelectedCollection())  );
    }
    else
        contextMenu->insertItem( "(No extra options for this item)" ); 

    if ( dir != 0 && dir->dirType() == Directory::FileFolder && item != 0 && !( item->parent() == 0 && !item->isOpen() ))
    {   
        contextMenu->insertSeparator();
        contextMenu->insertItem( "Collapse branch", this, SLOT(collapseCurrentBranch()) );
    }

	contextMenu->exec( QCursor::pos() );

    delete contextMenu;
}

void DirectoryView::contentsMouseReleaseEvent( QMouseEvent * )
{
    mousePressed = FALSE;
}

void DirectoryView::setDir( const QString &s )
{
    QListViewItemIterator it( this );
    ++it;

    for ( ; it.current(); ++it ) {
	it.current()->setOpen( FALSE );
    }

    QStringList lst( QStringList::split( "/", s ) );
    QListViewItem *item = firstChild();
    QStringList::Iterator it2 = lst.begin();

    for ( ; it2 != lst.end() && *it2 != ""; ++it2 ) 
	{
		while ( item ) 
		{
			if ( item->text( 0 ) == *it2 ) 
			{
				item->setOpen( TRUE );
				break;
			}
			item = item->itemBelow();
		}
    }

    if ( item )
	setCurrentItem( item );
}

void DirectoryView::newCollection()
{
    //printf("DirectoryView::newCollection\n" );

    QListViewItem *itm = selectedItem();
    if ( !itm )
        return;
    Directory *dir = dynamic_cast<Directory*>(itm);
    if (!dir )
        return;

    QDialog *di = new QDialog(0, "", true);
    QVBoxLayout *box = new QVBoxLayout( di );
    QLabel *lab = new QLabel( di );
    box->addWidget( lab );
    lab->setText( "Enter a name for the collection.\nNames must be unique.");
    QLineEdit *le = new QLineEdit( di );
    box->addWidget( le );
    QString defaultname = "My Collection" ;
    QString checkname = defaultname;
    // Check its not already a collection in this data source, else add a number to name?
    //std::list<std::string>collections = dir->dataSource()->GetCollectionNames();

    le->setText( checkname );
    le->selectAll();
    QPushButton *pb = new QPushButton( di );
    pb->setText( "    OK    " );
    connect ( pb, SIGNAL( clicked()), di, SLOT( accept()));
    box->addWidget( pb );
    box->setMargin(40);
    box->setSpacing(15);
    di->setCaption( "New collection" );
    int choice = di->exec();
    const std::string thename = le->text();
    bool ok = true;
    
    if ( le->text().stripWhiteSpace().length() == 0 ||  le->text().contains( QRegExp( "[^a-zA-Z0-9_\\s]" ) ) ) 
    {  QMessageBox::information( this, "Error" , "The name must contain letters, numbers, or underscores only.", "OK"  );
       ok = false;
    }

    if ( choice == QDialog::Accepted && le->text().length() > 0 )
    {   try
        {   
			dir->dataSource()->createCollection( thename );
             //printf("just created collection %s\n", thename);

        }
        catch(std::exception& e)
        {   //printf("create collection failed\n");
            QMessageBox::information( this, "Error" , e.what(), "OK" );
            ok = false;
        }
    }
    
    if ( ok )
    {   
		Directory* collection = new Directory( dir, le->text(), Directory::Collection);
        collection->setDataSource( dir->dataSource() );
        collection->setPixmap( new QPixmap( qembed_findImage("collection-iconc") ));
        dir->setOpen( true );
    }
}

void DirectoryView::renameCollection()
{
	//printf("DirectoryView::renameCollection\n" );
    QListViewItem *itm = selectedItem();
    if ( !itm )
        return;
    Directory *dir = dynamic_cast<Directory*>(itm);
    if (!dir )
        return;
    if ( dir->dirType() != Directory::Collection )
        return;

    QDialog *di = new QDialog(0, "", true);
    QVBoxLayout *box = new QVBoxLayout( di );
    QLabel *lab = new QLabel( di );
    box->addWidget( lab );
    lab->setText( "Enter a new name for the collection.\nNames must be unique.");
    QLineEdit *le = new QLineEdit( di );
    box->addWidget( le );
    std::string curname = dir->text(0, false);
    le->setText( QString(curname.c_str() ) );
    le->selectAll();
    QPushButton *pb = new QPushButton( di );
    pb->setText( "OK" );
    //QFontMetrics fm( pb->font());
    //pb->setFixedWidth( fm.width("the button width" )); 
    connect ( pb, SIGNAL( clicked()), di, SLOT( accept()));
    box->addWidget( pb );
    box->setMargin(40);
    box->setSpacing(15);
    di->setCaption( "Rename collection" );
    int choice = di->exec();
    const std::string thename = le->text();
    bool ok = true;

    if ( le->text().contains( QRegExp( "[^a-zA-Z0-9_\\s]" ) ) ) 
    {  QMessageBox::information( this, "Error" , "The name must contain letters, numbers, or underscores only.", "OK"  );
       ok = false;
    }

    if ( choice == QDialog::Accepted && le->text().length() > 0 && le->text().latin1() != curname )
    {   try
        {   dir->dataSource()->renameCollection( curname, thename );
            //printf("just renamed collection %s\n", thename.c_str() );
        }
        catch(std::exception& e)
        {   //printf("rename collection failed\n");
            QMessageBox::information( this, "Error" , e.what(), "OK"  );
            ok = false;
        }
    }
    if ( ok )
    {   dir->setName( QString(thename.c_str()) );
    dir->repaint();
    
    }
}

void DirectoryView::deleteCollection( Directory *dir )
{
    if ( dir->dirType() != Directory::Collection )
        return;

    std::string curname = dir->text(0, false);

    bool ok = true; 


    try
    {   
		dir->dataSource()->deleteCollection( curname );
        //printf("just deleted collection %s\n", thename.c_str() );	
    }
    catch(std::exception& e)
    {   //printf("delete collection failed\n");
        QMessageBox::information( this, "Error" , e.what(), "OK"  );
        ok = false;
    }

    if ( ok )
    {   // Update parent, and delete the item. 
        // Currently, assume parent is the data base item, but if we allow nested collections this wont be true.
        QListViewItem *pitem = dir->parent();
        Directory *pdir = dynamic_cast<Directory*>(pitem);
        if ( pdir ) 
        {   if ( pdir->sizeToShow() > 0 )
            {   pdir->setSizeToShow( pdir->sizeToShow() - 1 );
                pdir->repaint();
            }
        }
        // Signal prob not needed if we pick up the new selection, it should tell the iconview to update to that.
        emit collectionDeleted();

        delete dir;
    }
}

void DirectoryView::deleteSelectedCollection()
{
    //printf("DirectoryView::deleteCollection\n" );
    QListViewItem *itm = selectedItem();
    if ( !itm )
        return;
    Directory *dir = dynamic_cast<Directory*>(itm);
    if (!dir )
        return;
    if ( dir->dirType() != Directory::Collection )
        return;


    bool ok = true; 

    switch( QMessageBox::information( this, "Delete collection",
        "Are you sure you want to delete this collection? Assets no longer\n"
        "in any collection will be removed from the data base.",
        QMessageBox::Ok, QMessageBox::Cancel ) ) 
    { 
    case QMessageBox::Ok: 
        {
           //clearSelection();
           deleteCollection( dir );

           break;
        }
    default :
        {   ok = false;
            break;
        }
    }
}

void DirectoryView::collapseCurrentBranch()
{
    QListViewItem *item = currentItem();
    if ( !item )
        return;
    while ( item != 0 && item->parent() != 0  )
    {     
        item->setOpen( false );
        item = item->parent();
    }
}


void FileItem::setPixmap( QPixmap *p )
{
    pix = p;
    setup();
    widthChanged( 0 );
    invalidateHeight();
    repaint();
}


const QPixmap *FileItem::pixmap( int i ) const
{
    if ( i )
	return 0;
    return pix;
}

