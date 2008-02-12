
#include "jahfileiconview.h"
#include <qpainter.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qdragobject.h>
#include <qmessagebox.h>
#include <qevent.h>
#include <qpopupmenu.h>
#include <qcursor.h>
#include <qapplication.h>
#include <qwmatrix.h>
#include <qregexp.h>

#include <qpainter.h>
#include "assetstringfunctions.h"

#include "AssetManager.h"
#include "DataSource.h"
using namespace olib::openassetlib;

#include <stdlib.h>


static const char * file_icon[]={
    "32 32 17 1",
    "# c #000000",
    "a c #ffffff",
    "j c #808080",
    "n c #a0a0a4",
    "g c #c0c0c0",
    "m c #004000",
    "o c #000000",
    "l c #004040",
    "k c #404000",
    "i c #c0c000",
    "h c #ffff00",
    "b c #ffffc0",
    "e c #ff8000",
    "f c #c05800",
    "c c #ffa858",
    "d c #ffdca8",
    ". c None",
    "................................",
    "................................",
    "................................",
    "................................",
    ".............#....###...........",
    "...###......#a##.#aba##.........",
    "..#cdb#....#aaaa#aaaaaa##.......",
    "..#ecdb#..#aaaa#aaaaaaaba##.....",
    "..#fecdb##aaaa#aaaaaaaaaaab##...",
    "...#fecdb#aaa#aaaaaaabaabaaaa##.",
    "....#fecdb#a#baaaaa#baaaaaabaaa#",
    ".....#fecdb#aaaaab#a##baaaaaaa#.",
    ".....##fecdb#bbba#aaaa##baaab#..",
    "....#bb#fecdb#ba#aaaaaaa##aa#...",
    "...#bbbb#fecdb##aaabaaaaaa##....",
    "..#bbbb#b#fecdb#aaaaaaabaaaa##..",
    ".#bbbb#bbb#fecdg#aaaaaaaaaaaba#.",
    "#hhbb#bbbbb#fegg#iiaaaaaaaaaaaa#",
    "#jhhhklibbbk#ggj#aaiiaaaaaaaaa#j",
    ".#mjhhhkmikab####aaabiiaaaaaa#j.",
    "...##jhhhmaaibbaaiibaaaiiaab#n..",
    ".....##j#baaaiiabaaiibaabaa#n...",
    "......##baibaabiibaaaiiabb#j....",
    "......#bbbbiiaabbiiaaaaabon.....",
    ".....#bbbbbbbiiabbaiiaab#n......",
    ".....#jbbbbbbbbiibaabba#n.......",
    "......##jbbbbbbbbiiaabmj........",
    "........##jbbbbbbbbbb#j.........",
    "..........##nbbbbbbbmj..........",
    "............##jbbbb#j...........",
    "..............#mjj#n............",
    "................##n............."};

static const char * folder_icon[]={
    "32 32 11 1",
    "# c #000000",
    "b c #c0c000",
    "d c #585858",
    "a c #ffff00",
    "i c #400000",
    "h c #a0a0a4",
    "e c #000000",
    "c c #ffffff",
    "f c #303030",
    "g c #c0c0c0",
    ". c None",
    "...###..........................",
    "...#aa##........................",
    ".###baaa##......................",
    ".#cde#baaa##....................",
    ".#cccdeebaaa##..##f.............",
    ".#cccccdeebaaa##aaa##...........",
    ".#cccccccdeebaaaaaaaa##.........",
    ".#cccccccccdeebababaaa#.........",
    ".#cccccgcgghhebbbbbbbaa#........",
    ".#ccccccgcgggdebbbbbbba#........",
    ".#cccgcgcgcgghdeebiebbba#.......",
    ".#ccccgcggggggghdeddeeba#.......",
    ".#cgcgcgcggggggggghghdebb#......",
    ".#ccgcggggggggghghghghd#b#......",
    ".#cgcgcggggggggghghghhd#b#......",
    ".#gcggggggggghghghhhhhd#b#......",
    ".#cgcggggggggghghghhhhd#b#......",
    ".#ggggggggghghghhhhhhhdib#......",
    ".#gggggggggghghghhhhhhd#b#......",
    ".#hhggggghghghhhhhhhhhd#b#......",
    ".#ddhhgggghghghhhhhhhhd#b#......",
    "..##ddhhghghhhhhhhhhhhdeb#......",
    "....##ddhhhghhhhhhhhhhd#b#......",
    "......##ddhhhhhhhhhhhhd#b#......",
    "........##ddhhhhhhhhhhd#b#......",
    "..........##ddhhhhhhhhd#b#......",
    "............##ddhhhhhhd#b###....",
    "..............##ddhhhhd#b#####..",
    "................##ddhhd#b######.",
    "..................##dddeb#####..",
    "....................##d#b###....",
    "......................####......"};


static const char * link_icon[]={
    "32 32 12 1",
    "# c #000000",
    "h c #a0a0a4",
    "b c #c00000",
    "d c #585858",
    "i c #400000",
    "c c #ffffff",
    "e c #000000",
    "g c #c0c0c0",
    "a c #ff0000",
    "f c #303030",
    "n c white",
    ". c None",
    "...###..........................",
    "...#aa##........................",
    ".###baaa##......................",
    ".#cde#baaa##....................",
    ".#cccdeebaaa##..##f.............",
    ".#cccccdeebaaa##aaa##...........",
    ".#cccccccdeebaaaaaaaa##.........",
    ".#cccccccccdeebababaaa#.........",
    ".#cccccgcgghhebbbbbbbaa#........",
    ".#ccccccgcgggdebbbbbbba#........",
    ".#cccgcgcgcgghdeebiebbba#.......",
    ".#ccccgcggggggghdeddeeba#.......",
    ".#cgcgcgcggggggggghghdebb#......",
    ".#ccgcggggggggghghghghd#b#......",
    ".#cgcgcggggggggghghghhd#b#......",
    ".#gcggggggggghghghhhhhd#b#......",
    ".#cgcggggggggghghghhhhd#b#......",
    ".#ggggggggghghghhhhhhhdib#......",
    ".#gggggggggghghghhhhhhd#b#......",
    ".#hhggggghghghhhhhhhhhd#b#......",
    ".#ddhhgggghghghhhhhhhhd#b#......",
    "..##ddhhghghhhhhhhhhhhdeb#......",
    "############hhhhhhhhhhd#b#......",
    "#nnnnnnnnnn#hhhhhhhhhhd#b#......",
    "#nnnnnnnnnn#hhhhhhhhhhd#b#......",
    "#nn#nn#nnnn#ddhhhhhhhhd#b#......",
    "#nn##n##nnn###ddhhhhhhd#b###....",
    "#nnn#####nn#..##ddhhhhd#b#####..",
    "#nnnnn##nnn#....##ddhhd#b######.",
    "#nnnnn#nnnn#......##dddeb#####..",
    "#nnnnnnnnnn#........##d#b###....",
    "############..........####......"};

static const char * folder_locked_icon[]={
    "32 32 12 1",
    "# c #000000",
    "g c #808080",
    "h c #c0c0c0",
    "f c #c05800",
    "c c #ffffff",
    "d c #585858",
    "b c #ffa858",
    "a c #ffdca8",
    "e c #000000",
    "i c #a0a0a4",
    "j c #c0c0c0",
    ". c None",
    "...###..........................",
    "...#aa##........................",
    ".###baaa##......................",
    ".#cde#baaa##....................",
    ".#cccdeeba#######...............",
    ".#cccccde##fffff##..............",
    ".#cccccc##fffgggg#..............",
    ".#ccccccc#ffg####a##............",
    ".#ccccchc#ffg#eebbaa##..........",
    ".#ccccccc#ffg#ddeebbba##........",
    ".#ccchccc#ffg#ihddeebbba##......",
    ".#cccccaa#ffg#ihhhddeeba##......",
    ".#chchhbbaafg#ihhhihidebb#......",
    ".#cchccbbbbaa#ihhihihid#b#......",
    ".#chchhbb#bbbaaiihihiid#b#......",
    ".#hchhcbb#fbbbafhiiiiid#b#......",
    ".#chchhbb#ffgbbfihiiiid#b#......",
    ".#hhhhhbb#ffg#bfiiiiiid#b#......",
    ".#hhhhhbbaffg#bfiiiiiid#b#......",
    ".#iihhhjbbaab#bfiiiiiid#b#......",
    ".#ddiihhh#bbbabfiiiiiid#b#......",
    "..##ddiih#ffbbbfiiiiiid#b#......",
    "....##ddi#ffg#biiiiiiid#b#......",
    "......##d#ffg#iiiiiiiid#b#......",
    "........##ffg#iiiiiiiid#b#......",
    ".........#ffg#iiiiiiiid#b#......",
    ".........#ffg#ddiiiiiid#b###....",
    ".........##fg###ddiiiid#b#####..",
    "...........####.##ddiid#b######.",
    "..................##dddeb#####..",
    "....................##d#b###....",
    "......................####......"};

static QPixmap *iconFolderLockedLarge = 0;
static QPixmap *iconFolderLarge = 0;
static QPixmap *iconFileLarge = 0;
static QPixmap *iconLinkLarge = 0;
static QPixmap *iconFolderLockedSmall = 0;
static QPixmap *iconFolderSmall = 0;
static QPixmap *iconFileSmall = 0;
static QPixmap *iconLinkSmall = 0;



static void cleanup()
{
    delete iconFolderLockedLarge;
    iconFolderLockedLarge = 0;
    delete iconFolderLarge;
    iconFolderLarge = 0;
    delete iconFileLarge;
    iconFileLarge = 0;
    delete iconLinkLarge;
    iconLinkLarge = 0;
    delete iconFolderLockedSmall;
    iconFolderLockedSmall = 0;
    delete iconFolderSmall;
    iconFolderSmall = 0;
    delete iconFileSmall;
    iconFileSmall = 0;
    delete iconLinkSmall;
    iconLinkSmall = 0;
}

//#ifdef WIN32
IconProvider *QtFileIconViewItem::iconprovider = 0;
//#endif

/*****************************************************************************
 *
 * Class QtFileIconDrag
 *
 *****************************************************************************/

QtFileIconDrag::QtFileIconDrag( QWidget * dragSource, const char* name )
    : QIconDrag( dragSource, name )
{
}

const char* QtFileIconDrag::format( int i ) const
{
    if ( i == 0 )
    return "application/x-qiconlist";
    else if ( i == 1 )
    return "text/uri-list";
    else
    return 0;
}

QByteArray QtFileIconDrag::encodedData( const char* mime ) const
{
    QByteArray a;
    if ( QString( mime ) == "application/x-qiconlist" ) {
    a = QIconDrag::encodedData( mime );
    } else if ( QString( mime ) == "text/uri-list" ) {
    QString s = urls.join( "\r\n" );
    a.resize( s.length() );
    memcpy( a.data(), s.latin1(), s.length() );
    }
    return a;
}

bool QtFileIconDrag::canDecode( QMimeSource* e )
{
    return e->provides( "application/x-qiconlist" ) ||
    e->provides( "text/uri-list" );
}

void QtFileIconDrag::append( const QIconDragItem &item, const QRect &pr,
                 const QRect &tr, const QString &url )
{
    QIconDrag::append( item, pr, tr );
    urls << url;
}

/*****************************************************************************
 *
 * Class QtFileIconViewItem
 *
 *****************************************************************************/

QtFileIconViewItem::QtFileIconViewItem( QtFileIconView *parent, QFileInfo *fi )
    : QIconViewItem( parent, fi->fileName() )
	, itemFileName( fi->filePath() )
	, itemFileInfo( fi )
	, checkSetText( FALSE )
	, passet(0)
	, thumbnail(0)
	, banner(0)
{
    initFileInfo( fi );
	init();
}

QtFileIconViewItem::QtFileIconViewItem( QtFileIconView *parent, Asset* asset, QFileInfo *fi )
    : QIconViewItem( parent )
	, itemFileName( asset->filename().c_str() )
	, itemFileInfo( fi )
	, checkSetText( FALSE )
	, passet(asset)
	, thumbnail(0)
	, banner(0)
{
    initFileInfo( fi );
	init();
}

void QtFileIconViewItem::init()
{
	vm = QtFileIconView::Large;
	viewModeChanged( ( (QtFileIconView*)iconView() )->viewMode() );
}

void QtFileIconViewItem::initFileInfo( QFileInfo * )
{

    if ( itemFileInfo->isDir() )
    itemType = Dir;
    else if ( itemFileInfo->isFile() )
    itemType = File;
    if ( itemFileInfo->isSymLink() )
    itemType = Link;

    viewModeChanged( ( (QtFileIconView*)iconView() )->viewMode() );

    if ( itemFileInfo->fileName() == "." ||
     itemFileInfo->fileName() == ".." )
    setRenameEnabled( FALSE );

    checkSetText = TRUE;

    QObject::connect( &timer, SIGNAL( timeout() ),
              iconView(), SLOT( openFolder() ) );

}

void QtFileIconViewItem::paintItem( QPainter *p, const QColorGroup &cg )
{
    p->save();

    if ( itemFileInfo->isSymLink() ) {
    QFont f( p->font() );
    f.setItalic( TRUE );
    p->setFont( f );
    }

    QtFileIconView * qiv = (QtFileIconView*)iconView();
    // We changed the selected look of the item in 'image' mode.
    if(isSelected() && qiv->viewMode() == QtFileIconView::Image)
    {
        // Assuming that myPen is a QPen with
        // the right color and style
        //QRect pixmaprect = QRect( pixmapRect(false).topLeft() 
        QRect pixmaprect = pixmapRect(false);
        p->drawPixmap( QRect( pixmaprect.left() +1, pixmaprect.top(), pixmaprect.width()-2, pixmaprect.height()-2) ,*pixmap());
        QRect textrect = textRect(false);  
        p->fillRect( textrect, QBrush( cg.highlight()  ));
        p->drawText( QRect( textrect.left() + 2, textrect.top(), textrect.width() -2, textrect.height() )  ,Qt::AlignAuto , text() );

        int darken = 100;
        for ( int ir = 0; ir < 3; ir++ )
        {
            //p->setPen( cg.highlight().dark(200 - (ir*50)) );
            if ( ir == 0 )
                darken = 250;
            else if ( ir == 1 )
                darken = 150;
            else if ( ir == 2 )
                darken = 80;
     
            p->setPen( cg.highlight().dark( darken ));
            p->drawRect( QRect( pixmaprect.left() + ir, pixmaprect.top() + ir, pixmaprect.width()- (ir*2), pixmaprect.height()- (ir*2) ));
        }

        // round off inner most and outer most rects
        p->setPen( cg.base() );
        p->drawPoint( pixmaprect.topLeft() ); 
        p->drawPoint( pixmaprect.bottomLeft() );
        p->drawPoint( pixmaprect.topRight() );
        p->drawPoint( pixmaprect.bottomRight() );
        p->setPen( cg.highlight().dark( 100 ) );
        p->drawPoint( pixmaprect.left() +2, pixmaprect.top() + 2 );
        p->drawPoint( pixmaprect.left() +2, pixmaprect.bottom() - 2 ); 
        p->drawPoint( pixmaprect.right() -2, pixmaprect.top() + 2 ); 
        p->drawPoint( pixmaprect.right() -2, pixmaprect.bottom() - 2 ); 
    }
	else if ( qiv->viewMode() == QtFileIconView::Browser )
	{

		// We dont use QIconViewItem default text drawing, its all done on the banner pixmap.
		QRect pixmaprect = pixmapRect(false);
		p->drawPixmap( pixmaprect ,*pixmap());

		if ( isSelected() )
		{   QColor keepcol = p->pen().color();
			int darken = 100;
			for ( int ir = 0; ir < 3; ir++ )
			{
				if ( ir == 0 )
					darken = 250;
				else if ( ir == 1 )
					darken = 150;
				else if ( ir == 2 )
					darken = 80;

				p->setPen( cg.highlight().dark( darken ));
				p->drawRect( QRect( pixmaprect.left() + ir, pixmaprect.top() + ir, pixmaprect.width()- (ir*2), pixmaprect.height()- (ir*2) ));
			}
			// round off inner most and outer most rects
			p->setPen( cg.base() );
			p->drawPoint( pixmaprect.topLeft() ); 
			p->drawPoint( pixmaprect.bottomLeft() );
			p->drawPoint( pixmaprect.topRight() );
			p->drawPoint( pixmaprect.bottomRight() );
			p->setPen( cg.highlight().dark( 100 ) );
			p->drawPoint( pixmaprect.left() +2, pixmaprect.top() + 2 );
			p->drawPoint( pixmaprect.left() +2, pixmaprect.bottom() - 2 ); 
			p->drawPoint( pixmaprect.right() -2, pixmaprect.top() + 2 ); 
			p->drawPoint( pixmaprect.right() -2, pixmaprect.bottom() - 2 ); 
			p->setPen( keepcol );
		}
	}
    else
    {
        QIconViewItem::paintItem ( p,cg);
    }

    p->restore();
    
}

void QtFileIconViewItem::calcRect( const QString &text_ )
{
	//qDebug(" QtFileIconViewItem::calcRect " );

	QIconViewItem::calcRect( text_ );

	if ( ( (QtFileIconView*)iconView() )->viewMode() ==  QtFileIconView::Browser )
	{   QRect rec = textRect();
	rec.setWidth( 0 );
	rec.setHeight( 0 );
	setTextRect( rec );
	}


}

void QtFileIconViewItem::viewModeChanged( QtFileIconView::ViewMode m )
{
    vm = m;
    setDropEnabled( itemType == Dir && QDir( itemFileName ).isReadable() );
	/*   if ( m == QtFileIconView::Browser )
	{   
	QRect trect = textRect();
	trect.setWidth( iconView()->width() );
	setTextRect( trect );
	}
	*/
    calcRect();
}

QPixmap *QtFileIconViewItem::pixmap() const
{
    
    
    switch ( itemType ) {
    case Dir:
        {
            if ( !QDir( itemFileName ).isReadable() ) {
                if ( vm == QtFileIconView::Small )
                    return iconFolderLockedSmall; // TO FIX
                else
                    return iconFolderLockedLarge; // TO FIX
            } else 
            {
				if ( vm == QtFileIconView::Small )
				{   
					return ( IconProvider::instance().pixmap(itemFileInfo, true) );
					//    else return iconFolderSmall;
				}
				else
				{           
					return ( IconProvider::instance().pixmap(itemFileInfo, false) );
					//       else return iconFolderLarge;
				}

            }
        }
    case Link:
        {
            if ( vm == QtFileIconView::Small )
                return iconLinkSmall;
            else
                return iconLinkLarge;
        }
    default:
        {
            if ( vm == QtFileIconView::Small )
                return iconFileSmall;
            else if ( vm == QtFileIconView::Image )
            {
                QString tmpString(itemFileName);
                
                // check its a format we can process using QImage.
                if ( QImage::imageFormat( tmpString ) )
                {
                    if ( thumbnail ) // cached already.
                        return thumbnail;


                    generateThumbnail(tmpString);
                    if ( !thumbnail )
                        return iconFileLarge;
                    else
                        return thumbnail;
                }
                else
                {

                    return iconFileLarge;
                }

            }
			else if ( vm == QtFileIconView::Browser )
			{
				QString tmpString(itemFileName);

				// check its a format we can process using QImage.
				if ( QImage::imageFormat( tmpString ) )
				{
					generateThumbnail(tmpString);
				}


				generateBanner();
				return banner;


			}
            else
                return iconFileLarge;
        }
    }
}

void QtFileIconViewItem::generateThumbnail( const QString &filename ) const
{
    if ( thumbnail )
        return;

    QImage *theimage = new QImage;
    if ( !theimage->load( filename ) )
    { 
        delete theimage;
        
        //return iconFileLarge;
    }

    float ratio = (float)theimage->width() / (float)theimage->height();
    int w = 65; // was 100
    int h = int( w / ratio );

    if ( h > 65 )
    {
        h = 65;
        w = int( h * ratio );
    }

    QImage scaledimg;
    // NB smoothScale is good, but slow. Could switch to scale() or have option.
    // OR try to find visible items (findFirstVisibleItem) to update thumbs as user scrolls?
    // OR do that in paint fn? 
    scaledimg = theimage->smoothScale( w, h, QImage::ScaleMin );

    thumbnail = new QPixmap;
    thumbnail->convertFromImage( scaledimg );
    delete theimage; 


}

void QtFileIconViewItem::generateBanner() const
{
	//qDebug( "in generateBanner" );

	if ( banner ) 
		return;

	int wid = ( iconView()->width() < 700 ? 650 : iconView()->width() - 50 ); 
	int hgt = iconView()->gridY() - 20;
	banner = new QPixmap( wid, hgt );
	QPainter paint(banner);
	//paint.eraseRect( 0, 0, wid, hgt );
	paint.fillRect( 0, 0, wid, hgt, QBrush( iconView()->colorGroup().base() ));
	paint.setBrush( iconView()->colorGroup().background() );       
	paint.drawRoundRect( 0, 0, wid, hgt, 1, 8 ); 

	int framew = 100;
	int frameh = 80;

	paint.setPen( QColor( 30, 27, 42 ) );
	paint.setBrush( QColor( 30, 27, 42 ) );
	if ( thumbnail )
	{
		framew = thumbnail->width() + 16;
		frameh = thumbnail->height() + 16;
	}

	int dy = int(( hgt - frameh )/2); 
	paint.drawRoundRect( 10, dy, framew, frameh, 5, 5 );
	if ( thumbnail )
		paint.drawPixmap( QRect( 10 + 8, dy + 8, thumbnail->width(), thumbnail->height() ), *thumbnail );

	// Now write the text info.
	paint.setPen( Qt::white );
	QFont fnt = paint.font();
	//fnt.setFamily( "Tahoma" );  
	fnt.setPointSize( fnt.pointSize() + 3 );
	// fnt.setPixelSize( fnt.pixelSize() - 5);
	//fnt.setBold( true );
	fnt.setWeight( QFont::DemiBold );
	fnt.setStyleStrategy( QFont::NoAntialias );
	paint.setFont( fnt );
	int xmargin = framew + 20;
	int ygap = fnt.pointSize() + 6;
	paint.drawText( xmargin, dy + 8, text() );

	//fnt.setPointSize( fnt.pointSize() - 2 );
	// fnt.setBold ( false );
	fnt.setWeight( QFont::Normal );
	paint.setFont( fnt );
	QString line1 = "Date created: " + AssetStringFunctions::formattedDateString( *itemFileInfo );
	line1 += QString("   ") + "Frame size: size here ";
	line1 += QString("   ") + "File size: " + AssetStringFunctions::fileSizeString( *itemFileInfo );
	paint.drawText( xmargin, dy + 8 + ygap, line1 );
	QString line2 = "Description: description to go here once its in the database...";
	paint.drawText( xmargin, dy + 8 + ygap + ygap, line2 );


	paint.end();

}

void QtFileIconViewItem::setSelected( bool s )
{
    QIconViewItem::setSelected( s );
}

void QtFileIconViewItem::setSelected( bool s, bool cb )
{
    QIconViewItem::setSelected( s, cb );
}

QtFileIconViewItem::~QtFileIconViewItem()
{
    delete itemFileInfo;
    if ( thumbnail )
        delete thumbnail; 

	if ( banner )
		delete banner; 
}

void QtFileIconViewItem::setText( const QString &text )
{
    if ( checkSetText ) {
    if ( text == "." || text == "." || text.isEmpty() )
        return;
    QDir dir( itemFileInfo->dir() );
    if ( dir.rename( itemFileInfo->fileName(), text ) ) {
        itemFileName = itemFileInfo->dirPath( TRUE ) + "/" + text;
        delete itemFileInfo;
        itemFileInfo = new QFileInfo( itemFileName );
        QIconViewItem::setText( text );
    }
    } else {
    QIconViewItem::setText( text );
    }
}

bool QtFileIconViewItem::acceptDrop( const QMimeSource *e ) const
{
    if ( type() == Dir && e->provides( "text/uri-list" ) &&
     dropEnabled() )
        return TRUE;

    return FALSE;
}

void QtFileIconViewItem::dropped( QDropEvent *e, const QValueList<QIconDragItem> & )
{
    timer.stop();

    // For now, we just ignore drops on items (but dont disable the drops totally since we get
    // ugly 'dont drop' cursor?  To reinstate drops, just comment out the return and deal with the code after.
    e->ignore();
    return;

    // Regular DnD code below here, most of it is for *files*, wont apply to collections etc.

    if ( !QUriDrag::canDecode( e ) ) {
    e->ignore();
    return;
    }

    QStrList lst;
    QUriDrag::decode( e, lst );

    QString str;
    if ( e->action() == QDropEvent::Copy )
    str = "Copy\n\n";
    else
    str = "Move\n\n";
    for ( uint i = 0; i < lst.count(); ++i )
    str += QString( "   %1\n" ).arg( lst.at( i ) );
    str += QString( "\n"
            "To\n\n"
            "	%1" ).arg( filename() );

    QMessageBox::information( iconView(), e->action() == QDropEvent::Copy ? "Copy" : "Move" , str, "Not Implemented" );
    if ( e->action() == QDropEvent::Move )
    QMessageBox::information( iconView(), "Remove" , str, "Not Implemented" );
    e->acceptAction();
}

void QtFileIconViewItem::dragEntered()
{
    if ( type() != Dir ||
     type() == Dir && !QDir( itemFileName ).isReadable() )
    return;

    ( (QtFileIconView*)iconView() )->setOpenItem( this );
    timer.start( 1500 );
}

void QtFileIconViewItem::dragLeft()
{
    if ( type() != Dir ||
     type() == Dir && !QDir( itemFileName ).isReadable() )
    return;

    timer.stop();
}


/*****************************************************************************
 *
 * Class QtFileIconView
 *
 *****************************************************************************/

QtFileIconView::QtFileIconView( QWidget *parent, const char *name, const QString &dir )
    : QIconView( parent, name )
	, viewDir( dir )
	, nameFilter( "*" )
	, newFolderNum( 0 )
{
    if ( !iconFolderLockedLarge ) {
    qAddPostRoutine( cleanup );
    QWMatrix m;
    m.scale( 0.6, 0.6 );
    QPixmap iconpix( folder_locked_icon );
    iconFolderLockedLarge = new QPixmap( folder_locked_icon );
    iconpix = iconpix.xForm( m );
    iconFolderLockedSmall = new QPixmap( iconpix );
    iconpix = QPixmap( folder_icon );
    iconFolderLarge = new QPixmap( folder_icon );
    iconpix = iconpix.xForm( m );
    iconFolderSmall = new QPixmap( iconpix );
    iconpix = QPixmap( file_icon );
    iconFileLarge = new QPixmap( file_icon );
    iconpix = iconpix.xForm( m );
    iconFileSmall = new QPixmap( iconpix );
    iconpix = QPixmap( link_icon );
    iconLinkLarge = new QPixmap( link_icon );
    iconpix = iconpix.xForm( m );
    iconLinkSmall = new QPixmap( iconpix );
    }

    vm = Large;

    setGridX( 85 );
    setResizeMode( Adjust );
    setWordWrapIconText( FALSE );

    showMediaFilesOnly(); // default to media files. Makes us quicker too :)

    connect( this, SIGNAL( clicked( QIconViewItem * ) ),      this, SLOT( itemClicked( QIconViewItem * ) ) );
    connect( this, SIGNAL( doubleClicked( QIconViewItem * ) ),this, SLOT( itemDoubleClicked( QIconViewItem * ) ) );
    connect( this, SIGNAL( returnPressed( QIconViewItem * ) ),this, SLOT( itemDoubleClicked( QIconViewItem * ) ) );
    connect( this, SIGNAL( dropped( QDropEvent *, const QValueList<QIconDragItem> & ) ), this, SLOT( slotDropped( QDropEvent *, const QValueList<QIconDragItem> & ) ) );
    connect( this, SIGNAL( contextMenuRequested( QIconViewItem *, const QPoint & ) ),    this, SLOT( slotRightPressed( QIconViewItem * ) ) );

    setHScrollBarMode( Auto );
    setVScrollBarMode( Auto );
    // Orig setSelectionMode ( Single );
    setSelectionMode( QIconView::Extended );

    setAutoArrange( TRUE );
    setSorting( TRUE );
    openItem = 0;
    
    //for some reason a double click issues a single click 
    //before and afer the double click event causing a crash in the 2nd single click
    //is this a qt bug our our code?
    clickstatus = 0;

	// setPaletteBackgroundPixmap( QPixmap( "icon-view-bg.png" ) );

}

void QtFileIconView::openFolder()
{
    if ( !openItem )
    return;
    if ( openItem->type() != QtFileIconViewItem::Dir ||
     openItem->type() == QtFileIconViewItem::Dir &&
     !QDir( openItem->itemFileName ).isReadable() )
    return;

    openItem->timer.stop();
    setDirectory( openItem->itemFileName );
}

void QtFileIconView::setDirectory( const QString &dir )
{  
    //printf("setDirectory string %s \n",  dir.latin1() );

    // check for special items.
    if ( dir.contains( "$" ) )
    {    //printf("its a collection: %s \n", dir.latin1() );
         showCollection( dir );
    }
    else 
    {    //printf("Treating as folder: %s \n", dir.latin1() );

         oldviewDir = viewDir;
         viewDir = QDir( dir );
         readDir( viewDir );
    }
}

void QtFileIconView::setDirectory( const QDir &dir )
{
    //(I think other overload is normally called)
   
    //printf("setDirectory qdir %s \n",  dir.path().latin1() );
    //if ( dir.path() == "My Media")
    //    actualdir = QDir("c:/jahstorage/media");

    oldviewDir = viewDir;
    viewDir = dir;
    readDir( viewDir );

}

///////////////////////////////////////////////////
// need to rewrite this code so it can be commented out!
//if newassetlib isnt defined
void QtFileIconView::readAssets(  std::list<Asset*> assets )
{

    emit disableUp();
    emit disableBack();

    viewDir = QDir( "INVALID" ); // to avoid refreshing later (not a good method!).
    oldviewDir = viewDir;

    //printf("in readAssets, list size is %i \n", assets.size() );
    std::list<Asset*>::iterator it = assets.begin();
    if ( assets.size() == 0 )
        return; 

    emit startReadDir( assets.size() );
    clear();
    // TODO emit 'directoryChanged' ?
    QFileInfo fi;
    
    // TODO setUpdatesEnabled( false );
    while ( it != assets.end() )
    {
        std::string fulln = (*it)->path() + (*it)->filename();
        //printf( "adding asset %s \n", fulln.c_str() );
        QFileInfo fi( QString(fulln.c_str()) );
        //fi = QString(
        emit readNextDir();
        // Check against name filter. We use the same one as for the QDir file reader.
        bool ok = checkAgainstNameFilter( QString( (*it)->filename().c_str() ));
        if ( ok )
            new QtFileIconViewItem( this, new QFileInfo( fi ) );
        ++it;
        //No flickers. qApp->processEvents();
    }
    emit readDirDone();
    
    // TODO
    //setUpdatesEnabled( true );
    //repaint();

}

void QtFileIconView::showCollection( QString path )
{
    // path of form "Data source$collection name".
    QString tidypath = path.stripWhiteSpace();
    tidypath = tidypath.replace( "/", "" );
    tidypath = tidypath.replace( "\\", "" );

    QStringList lst = QStringList::split( QString("$"), tidypath );
    QString dsname = lst.first(); 
    QString colname = lst.last();

    clear();
    std::list<DataSource*>::const_iterator it = AssetManager::instance().dataSourceListBeginIterator();
    DataSource *ds = 0;
    while ( it != AssetManager::instance().dataSourceListEndIterator() )
    {   if ( (*it)->name() == dsname.latin1() )
            ds = *it; 
            
        ++it;
    }

    if ( ds == 0 )
    {   //printf("cannot find data source with name %s \n", dsname.latin1() );
        return;
    }
    else
    {   //printf( "found data source with name %s \n", dsname.latin1() );

        DataSource::Query::Condition* condition = new DataSource::Query::Condition( "Collection Name", colname.latin1(), DataSource::Query::Condition::EqualTo ); 
        DataSource::Query query( condition );
        std::list<Asset*> hits = ds->search( query );
        printf("Hits found = %i \n", hits.size() );
        delete condition;
        readAssets( hits );
      
        // We must delete our asset list.
        while( !hits.empty() )
        {   delete *(hits.begin() );
            hits.pop_front();
        }


    }
}



void QtFileIconView::newDirectory()
{
    setAutoArrange( FALSE  );
    selectAll( FALSE );
    if ( viewDir.mkdir( QString( "New Folder %1" ).arg( ++newFolderNum ) ) ) {
    QFileInfo *fi = new QFileInfo( viewDir, QString( "New Folder %1" ).arg( newFolderNum ) );
    QtFileIconViewItem *item = new QtFileIconViewItem( this, new QFileInfo( *fi ) );
    item->setKey( QString( "000000%1" ).arg( fi->fileName() ) );
    delete fi;
    repaintContents( contentsX(), contentsY(), contentsWidth(), contentsHeight(), FALSE );
    ensureItemVisible( item );
    item->setSelected( TRUE, TRUE );
    setCurrentItem( item );
    repaintItem( item );
    qApp->processEvents();
    item->rename();
    }
    setAutoArrange( TRUE );
}

QDir QtFileIconView::currentDir()
{
    return viewDir;
}

QDir QtFileIconView::previousDir()
{
    if ( dirHistory.count() == 0 )
        return QDir( "INVALID" );

    return *(dirHistory.getLast());
}

static bool isRoot( const QString &s )
{
#if defined(Q_OS_UNIX)
    if ( s == "/" )
    return TRUE;
#elif defined(Q_OS_WIN32)
    QString p = s;
    if ( p.length() == 3 &&
     p.right( 2 ) == ":/" )
    return TRUE;
    if ( p[ 0 ] == '/' && p[ 1 ] == '/' ) {
    int slashes = p.contains( '/' );
    if ( slashes <= 3 )
        return TRUE;
    if ( slashes == 4 && p[ (int)p.length() - 1 ] == '/' )
        return TRUE;
    }
#endif

    return FALSE;
}

void QtFileIconView::readDir( const QDir &dir )
{
    if ( !dir.isReadable() )
    return;

    if ( isRoot( dir.absPath() ) )
        emit disableUp();
    else
        emit enableUp();

    //dir.setMatchAllDirs( true ); JPN

    clear();

    if ( previousDir().exists() )
        emit enableBack();

    emit directoryChanged( dir.absPath() );

    const QFileInfoList *filist = dir.entryInfoList( nameFilter, QDir::DefaultFilter, QDir::DirsFirst | QDir::Name );

    emit startReadDir( filist->count() );

    QFileInfoListIterator it( *filist );
    QFileInfo *fi;
    bool allowRename = FALSE; 
    bool allowRenameSet = FALSE;

   //TODO  setUpdatesEnabled( false );
    while ( ( fi = it.current() ) != 0 ) 
    {
        ++it;

        // We now ignore all '.' and '..'
        if ( fi && ( fi->fileName() == ".." || fi->fileName() == "." ) )
            continue;

        if ( fi && fi->fileName() == ".." && ( fi->dirPath() == "/" || fi->dirPath().isEmpty() ) )
        {
            continue;
        }

        emit readNextDir();
        QtFileIconViewItem *item = new QtFileIconViewItem( this, new QFileInfo( *fi ) );
        // No flickers: qApp->processEvents();

        if ( fi->isDir() )
        {
            item->setKey( QString( "000000%1" ).arg( fi->fileName() ) );
        }
        else
        {
            item->setKey( fi->fileName() );
        }


        if ( !allowRenameSet ) 
        {
#if QT_SLOWNESS_BUG_FIXED
            if ( !QFileInfo(   fi->absFilePath() ).isWritable() 
                            || item->text() == "." 
                            || item->text() == ".." )
#else
            if (            false
                            || item->text() == "." 
                            || item->text() == ".." )
#endif
            {
                allowRename = FALSE;
            }
            else

            {
                allowRename = TRUE;
            }

            if ( item->text() == "." || item->text() == ".." )
            {
                allowRenameSet = FALSE;
            }
            else
            {
                allowRenameSet = TRUE;
            }
        }

        item->setRenameEnabled( allowRename );
    }

#if QT_SLOWNESS_BUG_FIXED
    if ( !QFileInfo( dir.absPath() ).isWritable() )
    {
        emit disableMkdir();
    }
    else
#endif
    {
        emit enableMkdir();
    }

    emit readDirDone();

    //TODO:
    //setUpdatesEnabled( true );
    //repaint();
}

void QtFileIconView::itemDoubleClicked( QIconViewItem *i )
{
    //debug("enter double clicked:");
    if (!i) return;
    
    QtFileIconViewItem *item = ( QtFileIconViewItem* )i;
    
    if ( item->type() == QtFileIconViewItem::Dir ) 
    {
      
        oldviewDir = viewDir;
        viewDir = QDir( item->filename() );      
        readDir( viewDir );
    } 
    else if ( item->type() == QtFileIconViewItem::Link)
    {
        if (QFileInfo( QFileInfo( item->filename() ).readLink() ).isDir() ) 
        {   
            oldviewDir = viewDir;
            viewDir = QDir( QFileInfo( item->filename() ).readLink() );
            readDir( viewDir );   
        }
    }
    else if ( item->type() == QtFileIconViewItem::File ) 
    
    {
        //debug("its a file:");
        QString filetoload = item->filename();
        loadthisfile(filetoload); //emits a signal
    }
    
    //debug("exit double clicked:");
    
    clickstatus = true;

}

void QtFileIconView::itemClicked( QIconViewItem *i )
{   
    if (!i) return;

    //debug("enter single clicked:"); 
    
    if (clickstatus) 
    {
        clickstatus = false;
        return;
    }
    
    QtFileIconViewItem *item = ( QtFileIconViewItem* )i; 
    
    if ( item->type() == QtFileIconViewItem::Dir ) 
    {
    
    } 
    else if ( item->type() == QtFileIconViewItem::Link )
    { 
         
        if ( QFileInfo( QFileInfo( item->filename() ).readLink() ).isDir() ) 
        {
            {
            }
        }
    }
    else if ( item->type() == QtFileIconViewItem::File ) 
    {
        //debug("clicked a file:");
        QString filename = item->filename();
        setfilename(filename); //emits a signal
    }
    //debug("exit single clicked:");
}

QDragObject *QtFileIconView::dragObject()
{
    if ( !currentItem() )
    return 0;

    // BUG with viewport when scrolled.
    QPoint orig = viewportToContents( viewport()->mapFromGlobal( QCursor::pos() ) );
// almost    QPoint disp =  ( QCursor::pos() - viewport()->mapToGlobal( currentItem()->pixmapRect( false ).topLeft() ) );
    
    QPoint disp = QPoint( currentItem()->pixmapRect(false).width() / 2, currentItem()->pixmapRect(false).height() / 2 );
    
    QtFileIconDrag *drag = new QtFileIconDrag( viewport() );
 
    QPixmap thumb = generateFadedThumb( currentItem()->pixmap() );
    if ( thumb.isNull() )
        thumb = *currentItem()->pixmap();
    drag->setPixmap( thumb,  disp );
     
    //QIconViewItem* curitem = currentItem();
    for ( QtFileIconViewItem *item = (QtFileIconViewItem*)firstItem(); item; item = (QtFileIconViewItem*)item->nextItem() ) 
        {
          if ( item->isSelected()  ) // && item != currentItem() ) 
          {
              
              QIconDragItem id;
              id.setData( QCString( item->filename() ) );
             
              drag->append( id,
              QRect( item->pixmapRect( FALSE ).x() - orig.x(),
              item->pixmapRect( FALSE ).y() - orig.y(),
              item->pixmapRect().width(), item->pixmapRect().height() ),
              QRect( item->textRect( FALSE ).x() - orig.x(),
              item->textRect( FALSE ).y() - orig.y(),
              item->textRect().width(), item->textRect().height() ),
              QString( item->filename() ) );
              

      /*TEST        drag->append( id,
              QRect( item->pixmapRect( FALSE ).x() - orig.x(),
              item->pixmapRect( FALSE ).y() - orig.y(),
              item->pixmapRect().width(), item->pixmapRect().height() ),
              QRect( item->textRect( FALSE ).x() - orig.x(),
              item->textRect( FALSE ).y() - orig.y(),
              item->textRect().width(), item->textRect().height() ),
              QString( item->filename() ) );
            */

          }
    }

    return drag;
}

void QtFileIconView::keyPressEvent( QKeyEvent *e )
{
    if ( e->key() == Key_N &&
     ( e->state() & ControlButton ) )
    newDirectory();
    else
    QIconView::keyPressEvent( e );
}

void QtFileIconView::resizeEvent ( QResizeEvent * e )
{
	//qDebug(" in resizeEvent " );
	QIconView::resizeEvent( e );
	if ( viewMode() == Browser ) // always fill the x space.
	{    setGridX( viewport()->width() );
	}
}

void QtFileIconView::slotDropped( QDropEvent *e, const QValueList<QIconDragItem> & )
{
    // For now, we just ignore drops on items (but dont disable the drops totally since we get
    // ugly 'dont drop' cursor?  To reinstate drops, just comment out the return and deal with the code after.
    e->ignore();
    return;

    // Regular Dnd code below here.

    if ( openItem )
    openItem->timer.stop();
    if ( !QUriDrag::canDecode( e ) ) {
    e->ignore();
    return;
    }

    QStrList lst;
    QUriDrag::decode( e, lst );

    QString str;
    if ( e->action() == QDropEvent::Copy )
    str = "Copy\n\n";
    else
    str = "Move\n\n";
    for ( uint i = 0; i < lst.count(); ++i )
    str += QString( "   %1\n" ).arg( lst.at( i ) );
    str += QString( "\n"
            "To\n\n"
            "	%1" ).arg( viewDir.absPath() );

    QMessageBox::information( this, e->action() == QDropEvent::Copy ? "Copy" : "Move" , str, "Not Implemented" );
    if ( e->action() == QDropEvent::Move )
    QMessageBox::information( this, "Remove" , str, "Not Implemented" );
    e->acceptAction();
    openItem = 0;
}

void QtFileIconView::viewLarge()
{
    //weadded
    // Orig setSelectionMode ( Single );
    setSelectionMode( QIconView::Extended );
    
    setItemTextPos( Bottom );  
   
    sort( TRUE );
    
    setWordWrapIconText( FALSE );
    
    setHScrollBarMode( Auto );
    setVScrollBarMode( Auto );
    setArrangement( LeftToRight );
    
    setAutoArrange( TRUE );
    setSorting( TRUE );
	setItemsMovable( true );

    setResizeMode ( Adjust );
    setGridX(85);
    
    setViewMode( Large );
   
}

void QtFileIconView::viewSmall()
{
    //weadded
    // Orig setSelectionMode ( Single );
    setSelectionMode( QIconView::Extended );

    setItemTextPos( Right );  
    
    sort( TRUE );
    
    setWordWrapIconText( FALSE );
    
    setHScrollBarMode( Auto );
    setVScrollBarMode( Auto );
    setArrangement( TopToBottom );
    
    setAutoArrange( TRUE );
    setSorting( TRUE );
	setItemsMovable( true );

    setResizeMode( Adjust );
    setGridX(200);
    
    QString rere;
    
    setViewMode( Small );

}

void QtFileIconView::viewImage()
{
    //weadded
    // Orig setSelectionMode ( Single );
    setSelectionMode( QIconView::Extended );
    setItemTextPos( Bottom );  
   
    sort( TRUE );
    
    setWordWrapIconText( FALSE );
    
    setHScrollBarMode( Auto );
    setVScrollBarMode( Auto );
    setArrangement( LeftToRight );
    
    setAutoArrange( TRUE );
    setSorting( TRUE );
	setItemsMovable( true );

    setResizeMode ( Adjust );
    setGridX(95);
    setGridY(95);
    
    setViewMode( Image );
   
}

void QtFileIconView::viewBrowser()
{
	setSelectionMode( QIconView::Extended );
	setItemTextPos( Right );  

	sort( TRUE );

	setWordWrapIconText( FALSE );

	setHScrollBarMode( Auto );
	setVScrollBarMode( Auto );
	setArrangement( LeftToRight );

	setAutoArrange( TRUE );
	setSorting( TRUE );
	setItemsMovable( false ); // unlike other views, not movable.

	setResizeMode ( Adjust );
	setGridX( viewport()->width() );
	setGridY(120);

	setViewMode( Browser );

}

void QtFileIconView::showMediaFilesOnly()
{
    setNameFilter( "*.aac *.aif *.aiff *.amf *.ams *.ape *.au *.dbm *.dmf *.dsm *.far *.flac *.it *.mdl \
            *.med *.mid *.midi *.mod *.mol *.mp1 *.mp2 *.mp3 *.mp4 *.mpa *.mpc *.mpp *.mtm *.nst *.ogg  \
            *.okt *.psm *.ptm *.ra *.rmi *.s3m *.stm *.ult *.umx *.wav *.wma *.wow *.xm \
            *.bmp *.dcx *.emf *.gif *.ico *.jpeg *.jpg *.pct *.pcx *.pic *.pict *.png *.psd *.exr \
            *.psp *.tga *.tif *.tiff *.wmf *.xif *.rgb *.sgi \
			*.obj *.x3d *.3ds *.dae \
            *.asf *.avi *.divx *.m1v *.m2v *.mov *.mp1v *.mp2v *.mpe *.mpeg *.mpg *.mps *.mpv \
            *.mpv1 *.mpv2 *.ogm *.qt *.ram *.rm *.rv *.vivo *.vob *.wmv \
			*.jsf *.jfx *.jef *.westley *.mpl *.dv *.dif" );
    updateNameFilterList();
    if ( viewDir.exists() )
        readDir( viewDir );
}

void QtFileIconView::showImageFilesOnly()
{
    setNameFilter( "*.bmp *.dcx *.emf *.gif *.ico *.jpeg *.jpg *.pct *.pcx *.pic *.pict *.png *.psd \
            *.psp *.tga *.tif *.tiff *.wmf *.xif *.rgb *.sgi *.exr" ); 
    updateNameFilterList();
    if ( viewDir.exists() )
        readDir( viewDir );
}

void QtFileIconView::showVideoFilesOnly()
{
    setNameFilter( "*.asf *.avi *.divx *.m1v *.m2v *.mov *.mp1v *.mp2v *.mpe *.mpeg *.mpg *.mps *.mpv \
            *.mpv1 *.mpv2 *.ogm *.qt *.ram *.rm *.rv *.vivo *.vob *.wmv *.dv *.dif" ); 
    updateNameFilterList();
    
    if ( viewDir.exists() )
        readDir( viewDir );
}

void QtFileIconView::showAudioFilesOnly()
{
    setNameFilter( "*.aac *.aif *.aiff *.amf *.ams *.ape *.au *.dbm *.dmf *.dsm *.far *.flac *.it *.mdl \
            *.med *.mid *.midi *.mod *.mol *.mp1 *.mp2 *.mp3 *.mp4 *.mpa *.mpc *.mpp *.mtm *.nst *.ogg  \
            *.okt *.psm *.ptm *.ra *.rmi *.s3m *.stm *.ult *.umx *.wav *.wma *.wow *.xm" ); 
    updateNameFilterList();   
    
    if ( viewDir.exists() )
        readDir( viewDir );
}
void QtFileIconView::updateNameFilterList()
{
    nameFilterAsList = QStringList::split( QRegExp( "[\\s*]"),  nameFilter );
}

bool QtFileIconView::checkAgainstNameFilter( const QString& filename )
{
    if ( nameFilter.isEmpty() || nameFilter == "*" )
        return true;

    for ( QStringList::Iterator it = nameFilterAsList.begin(); it != nameFilterAsList.end(); ++it ) 
    {
        if ( filename.contains(*it) )
            return true;
    }

    return false; 
}

void QtFileIconView::showAllFiles()
{
    setNameFilter( "*");
    readDir( viewDir );
}

void QtFileIconView::viewBottom()
{
    setItemTextPos( Bottom );
}

void QtFileIconView::viewRight()
{
    setItemTextPos( Right );
}

void QtFileIconView::flowEast()
{
    setHScrollBarMode( AlwaysOff );
    setVScrollBarMode( Auto );
    setArrangement( LeftToRight );
}

void QtFileIconView::flowSouth()
{
    setVScrollBarMode( AlwaysOff );
    setHScrollBarMode( Auto );
    setArrangement( TopToBottom );
}

void QtFileIconView::sortAscending()
{
    sort( TRUE );
}

void QtFileIconView::sortDescending()
{
    sort( FALSE );
}

void QtFileIconView::itemTextTruncate()
{
    setWordWrapIconText( FALSE );
}

void QtFileIconView::itemTextWordWrap()
{
    setWordWrapIconText( TRUE );
}

void QtFileIconView::slotRightPressed( QIconViewItem *item )
{
    if ( !item ) { // right pressed on viewport
    QPopupMenu menu( this );

    menu.insertItem( "&Large view", this, SLOT( viewLarge() ) );
    menu.insertItem( "&Small view", this, SLOT( viewSmall() ) );
    menu.insertItem( "&Image view", this, SLOT( viewImage() ) );
    menu.insertSeparator();
    menu.insertItem( "Text at the &bottom", this, SLOT( viewBottom() ) );
    menu.insertItem( "Text at the &right", this, SLOT( viewRight() ) );
    menu.insertSeparator();
    menu.insertItem( "Arrange l&eft to right", this, SLOT( flowEast() ) );
    menu.insertItem( "Arrange t&op to bottom", this, SLOT( flowSouth() ) );
    menu.insertSeparator();
    menu.insertItem( "&Truncate item text", this, SLOT( itemTextTruncate() ) );
    menu.insertItem( "&Wordwrap item text", this, SLOT( itemTextWordWrap() ) );
    menu.insertSeparator();
    menu.insertItem( "Arrange items in &grid", this, SLOT( arrangeItemsInGrid() ) );
    menu.insertSeparator();
    menu.insertItem( "Sort &ascending", this, SLOT( sortAscending() ) );
    menu.insertItem( "Sort &descending", this, SLOT( sortDescending() ) );

    menu.setMouseTracking( TRUE );
    menu.exec( QCursor::pos() );
    } else { // on item
    QPopupMenu menu( this );

    int RENAME_ITEM = menu.insertItem( "Rename Item" );
    int REMOVE_ITEM = menu.insertItem( "Remove Item" );

    menu.setMouseTracking( TRUE );
    int id = menu.exec( QCursor::pos() );

    if ( id == -1 )
        return;

    if ( id == RENAME_ITEM && item->renameEnabled() ) {
        item->rename();
    } else if ( id == REMOVE_ITEM ) {
        delete item;
        QMessageBox::information( this, "Not implemented!", "Deleting files not implemented yet,\n"
                      "The item has only been removed from the view! " );
    }
    }
}

void QtFileIconView::setViewMode( ViewMode m )
{
    if ( m == vm )
    return;

    vm = m;
    QtFileIconViewItem *item = (QtFileIconViewItem*)firstItem();
    for ( ; item; item = (QtFileIconViewItem*)item->nextItem() )
    item->viewModeChanged( vm );

    arrangeItemsInGrid();
}

QPixmap QtFileIconView::generateFadedThumb( QPixmap *pixmap )
{   // TODO faded.

    QPixmap result;

    QImage img = pixmap->convertToImage();
    if ( !img.isNull() && img.depth() == 32 ) 
        img.setAlphaBuffer( true );

      if ( img.hasAlphaBuffer() && !img.isNull() && img.depth() == 32 ) // just do trickery if 32 bit with alpha. 
       {
        int pr, pg, pb, pa;
        int bright = 30;
        for ( int y=0; y<img.height(); y++ )
        {
            for ( int x=0; x<img.width(); x++ )
            {
                //this is the source and destination pixel
                QRgb *p = (QRgb *)img.scanLine(y) + x;
                pr=qRed(*p) + bright;
                pg=qGreen(*p) + bright;
                pb=qBlue(*p) + bright;
                pa = qAlpha(*p);
                if ( pr > 255 ) pr = 255;
                if ( pg > 255 ) pg = 255;
                if ( pb > 255 ) pb = 255;
                if ( pa > 0 ) pa = 155;

                *p = qRgba( pr,pg,pb, pa );
               
            }
        }
    }

    result.convertFromImage( img );
    return result;
}


