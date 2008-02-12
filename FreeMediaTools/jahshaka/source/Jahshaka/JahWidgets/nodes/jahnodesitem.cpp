/*******************************************************************************
**
** The header file for the node class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qfontmetrics.h>
#include <qimage.h>
#include "jahnodesitem.h"

//for preferences global
#include "jahpreferences.h"
#include "jahformatter.h"

///////////////////////////////////////////////
// for the node item

void NodeItem::moveBy(double dx, double dy)
{
    QCanvasEllipse::moveBy( dx, dy );
    itemtext->moveBy( dx, dy );
    itemtextshadow->moveBy( dx, dy );
    oval->moveBy( dx, dy );
    if ( imagesprite )
        imagesprite->moveBy( dx, dy );

    QPtrListIterator<NodesEdgeItem> it1( inList );
    NodesEdgeItem *edge;
    while (( edge = it1.current() )) {
	++it1;
	edge->setToPoint( int(x()), int(y()) );
    }
    QPtrListIterator<NodesEdgeItem> it2( outList );
    while (( edge = it2.current() )) {
	++it2;
	edge->setFromPoint( int(x()), int(y()) );
    }
}



NodeItem::NodeItem( QCanvas *canvas, QString name, int num )
    : QCanvasEllipse( 97, 57, canvas )
{

    // We should probably not have this class as an ellipse, since have now changed 
    // to pixmaps, and the bounding area is still ellipse. 

    nodename = name;
    layernumber = num;

    //for the ellipse
    setPen( QPen::NoPen );
    setBrush( QBrush::NoBrush );
    setZ( 127 );


    //for the text label
    itemtext = new QCanvasText( canvas);
    itemtext->setColor( QColor(200,200,200) );
    itemtext->setText(name);
    itemtextshadow = new QCanvasText( canvas);
    itemtextshadow->setColor( QColor(20,20,20) );
    itemtextshadow->setText(name);
  
    // Center the text.
    // (would be better to overide setText, then re-center text if name changes?).
    QFontMetrics qfm( itemtext->font() );
    int wid = qfm.width( itemtext->text() );
    int hgt = qfm.height();
    itemtext->setX( x() - int(wid/2) );
    itemtext->setY(y() - int (hgt/2) );
    itemtext->setZ(129);
    itemtext->show();
    itemtextshadow->setX( x() - int(wid/2) +1 );
    itemtextshadow->setY(y() - int (hgt/2) +1);
    itemtextshadow->setZ(128);
    itemtextshadow->show();

    // pixmap nicer instead of basic shapes (but not scalable). NB base class still ellipse...

    JahPrefs& jprefs = JahPrefs::getInstance();
    QString basepath( jprefs.getBasePath( ).data() );
    ovalpm = new QCanvasPixmapArray( basepath + "Pixmaps/desktop/node-styled.png" );

    oval = new QCanvasSprite( ovalpm, canvas );
    oval->moveBy( -67, -40 ); // shift by half our size
    oval->setZ(127);
    oval->show();

    // hide the ellipse? TODO: change baseclass to sprite if we really want the sprite not ellipse
    this->setZ(120);

    // These are used by special image nodes only (see setImage).
  //  pmap = new QCanvasPixmap();
    pmimage = new QCanvasPixmapArray();
    imagesprite = new QCanvasSprite( pmimage, canvas );
    imagesprite->hide();

}

NodeItem::~NodeItem()
{
    // (qcanvas wont deal with all our 'child' items?
}

void NodeItem::hide()
{
    // Until crashes with delete are sorted, we'll do hiding...
  //?  inList.clear();
  //?  outList.clear();
    QCanvasEllipse::hide();
    move(-50,-50); // Why? I think bounding area prob with ellipse :(
    itemtext->hide();
    itemtextshadow->hide();
    if ( imagesprite )
        imagesprite->hide();
    oval->hide();
 
}

void NodeItem::setSelected( bool state )
{
    if ( state )
        itemtext->setColor(  QColor( Qt::white ) );
    else
        itemtext->setColor(  QColor(160,160,160) );

    QCanvasItem::setSelected( state );
    // ( canvas()->update() to be done by calling function when appropriate).
}

void NodeItem::setImage( QImage im, QCanvas *canvas )
{
    pmap = new QCanvasPixmap( im ); // MEM LEAK?
    pmimage->setImage(0, pmap );
    if ( !imagesprite )
         imagesprite = new QCanvasSprite( pmimage, canvas );
    else
        imagesprite->setSequence( pmimage );
    imagesprite->show();

    imagesprite->setX( x()  - imagesprite->width()/2 - 5 );
    imagesprite->setY( y() - imagesprite->height() - 8 );
    imagesprite->show();

    JahPrefs& jprefs = JahPrefs::getInstance();
    QString basepath( jprefs.getBasePath( ).data() );

    ovalpm->readPixmaps( basepath + "Pixmaps/desktop/widenode-styled-green.png" );
    oval->setSequence( ovalpm );
    oval->move( x() - 78, y() - 10 ); // shift by approx half our size
    oval->setZ(127);
    oval->show();
}

void NodeItem::setName( QString name )
{
    itemtext->setText(name);
    itemtextshadow->setText(name);
    recenterText();
}

void NodeItem::recenterText()
{ 
    QFontMetrics qfm( itemtext->font() );
    int wid = qfm.width( itemtext->text() );
    int hgt = qfm.height();
    itemtext->setX( x() - int(wid/2) );
    itemtext->setY(y() - int (hgt/2) );
    itemtext->show();
    itemtextshadow->setX( x() - int(wid/2) +1 );
    itemtextshadow->setY(y() - int (hgt/2) +1);
}


///////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//

int NodesEdgeItem::c = 0;



void NodesEdgeItem::moveBy(double, double)
{
    //nothing
}


NodesEdgeItem::NodesEdgeItem( NodeItem *from, NodeItem *to, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    from->addOutEdge( this );
    to->addInEdge( this );
    setPoints( int(from->x()), int(from->y()), int(to->x()), int(to->y()) );
    setZ( 126 );
}

NodesEdgeItem::NodesEdgeItem( int x, int y, NodeItem *to, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    //from->addOutEdge( this );
    to->addInEdge( this );
    setPoints( x, y, int(to->x()), int(to->y()) );
      //setPen( *tp );
    //setBrush( *tb );  //setZ( 127 );
}

NodesEdgeItem::NodesEdgeItem( NodeItem *from, int x, int y, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    from->addOutEdge( this );
    //to->addInEdge( this );
    setPoints( int(from->x()), int(from->y()), x, y );
    setZ( 126 );
}



void NodesEdgeItem::setFromPoint( int x, int y )
{
    setPoints( x,y, endPoint().x(), endPoint().y() );
}

void NodesEdgeItem::setToPoint( int x, int y )
{
    setPoints( startPoint().x(), startPoint().y(), x, y );
}

