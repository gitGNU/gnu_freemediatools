/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "WireupOverview.h"
#include "Gadget.h"
#include "wireup-embedded-images.h"
#include <qpainter.h>
#include <qpalette.h>

WireUpOverview::WireUpOverview( QCanvas *canvas ) : QCanvasRectangle(canvas),
                        _canvas( canvas ), _width( 125 ), _height( 125 ), _scalef( 0.2 ), _contentsRect( 0,0,125, 175 ),
                        _topborder( 5 )
{
    pm.resize( _width, _height );

    _bg = QImage( qembed_findImage( "overview-bg" ) );
    QCanvasRectangle::setSize( _width, _height );
    setPen( QColor( Qt::gray ) );
    regenerate();
}

WireUpOverview::~WireUpOverview()
{   
}

void WireUpOverview::regenerate()
{
    generatePixmap();
    update();
    canvas()->update();
}

void WireUpOverview::setSize( int w, int h )
{
    QCanvasRectangle::setSize( w, h );
    pm.resize( w, h );
    regenerate();
}

void WireUpOverview::setScaleFactor( double scalef )
{
    _scalef = scalef;
    regenerate();
}

void WireUpOverview::setContentsRect( QRect contents )
{
    _contentsRect = contents;
    regenerate();
}

QPoint WireUpOverview::pixelToContentsPoint( QPoint pos )
{
    QPoint pnt;
    pnt.setX( (pos.x() -x() + _boundary.left()) / _scalef );
    pnt.setY( (pos.y() -y() + _boundary.top() -5 )/ _scalef );

    return pnt;
}

void WireUpOverview::generatePixmap()
{
    printf(" in WireUpOverviewSprite::generatePixmap \n" );
    
    _boundary = findCanvasBoundary();

    printf(" _boundary %i %i  %i %i \n", _boundary.left(), _boundary.top(), _boundary.width(), _boundary.height() );
    
    // Make sure the contents are in the pic? Not sure this is a good effect, the zoom changes as we scroll.
  /*  if ( _contentsRect.left() < _boundary.left() )
        _boundary.setLeft( _contentsRect.left() - 10 );
    if ( _contentsRect.right() > _boundary.right() )
        _boundary.setRight( _contentsRect.right() + 10 );
    if ( _contentsRect.top() < _boundary.top() )
        _boundary.setTop( _contentsRect.top() - 10 );
    if ( _contentsRect.bottom() < _boundary.bottom() )
        _boundary.setBottom( _contentsRect.bottom() + 10 ); 
	*/
  
    QPainter painter;
    painter.begin( &pm, this );
    painter.setPen( QPen( Qt::gray ));
    //painter.setBrush( QPen( Qt::black ));
    painter.fillRect( 0, 0, pm.width(), pm.height(), QBrush( Qt::black ));
    painter.drawImage( QRect( 0, 0, pm.width(), pm.height() ), _bg );

    // double scalef = double (boundary.width()) / ( pm.width() * 20.0 );

    // Work out a scale. (NB cached into _scalef).
    _scalef =  double( pm.width()+pm.height() ) / ( 1.70 * double(_boundary.width()+_boundary.height()) );

    if ( _scalef > 0.18 )
        _scalef = 0.18;
    

    printf( "_scalef is %f \n", float( _scalef ) );

    QCanvasItemList ilist = _canvas->allItems();
    for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
    {
        int iRTTI = (*it)->rtti();

        if ( iRTTI == gadgetRTTI && (*it)->isVisible() )
        {   
            Gadget* gadge = dynamic_cast<Gadget*>( *it );
    
            painter.setPen( QPen( Qt::gray ) );
            if ( gadge->type() == Gadget::CIRCULAR_GADGET )
                  painter.drawEllipse( _scalef*(int(gadge->x()) - _boundary.left()), _scalef*(int(gadge->y()) - _boundary.top() ) +_topborder, 
                                _scalef*(int(gadge->width()) ), _scalef*( int(gadge->height()) ) );
            else
                painter.drawRect( _scalef*(int(gadge->x()) - _boundary.left()), _scalef*(int(gadge->y()) - _boundary.top() ) +_topborder, 
                                _scalef*(int(gadge->width()) ), _scalef*( int(gadge->height()) ) );
            //printf( "drawrect %f %f %f %f \n", gadge->x() - boundary.left(), gadge->y() - boundary.top(), 
            //    gadge->x() + gadge->width() - boundary.left(), gadge->y() + gadge->height() - boundary.top() );
            
            // Draw the wires from our output sockets (dont bother with drawing sockets themselves).
            Socket *sock;
            Wire *wire;
            QList<Socket> lsockets = gadge->outputSockets();
            QList<Socket>::iterator sockit =  lsockets.begin();     
            while ( sockit != lsockets.end() )
            {
                sock = (*sockit);
                // Now get the wires
                QList<Wire> lwires = sock->wires();
                QList<Wire>::iterator wireit =  lwires.begin();  
                while ( wireit != lwires.end() )
                {
                    wire = (*wireit);
                    if ( wire->isVisible() )
                    {   QPointArray bezpts = wire->bezierPoints().copy(); // need deep copy, else disaster ;)
                        // convert bez points to our coords
                        bezpts.setPoint( 0, convertToOverviewCoordinates( bezpts.point(0) ) );
                        bezpts.setPoint( 1, convertToOverviewCoordinates( bezpts.point(1) ) );
                        bezpts.setPoint( 2, convertToOverviewCoordinates( bezpts.point(2) ) );
                        bezpts.setPoint( 3, convertToOverviewCoordinates( bezpts.point(3) ) );

                        // draw it!
                        painter.setPen( QPen( QColor( 100, 100, 75 ) ));
                        painter.drawCubicBezier( bezpts );
                    }

                    ++wireit;
                }

                sockit++;
            }

        }
    }

    // Finally paint the contents rect
    painter.setPen( QPen( QColor( 77, 100, 106 ) ));
    painter.drawRect( _scalef*(_contentsRect.left()- _boundary.left()), _scalef*(_contentsRect.top()- _boundary.top()) +_topborder,
                    _scalef*(_contentsRect.width()), _scalef*(_contentsRect.height()) );

    painter.end();
    
}

void WireUpOverview::drawShape( QPainter & painter )
{
    painter.drawPixmap( x(), y(), pm );
}

QRect WireUpOverview::findCanvasBoundary()
{
    int left = 10000; 
    int right = -10000;
    int top = 10000;
    int bottom = -10000;

    QCanvasItemList ilist = _canvas->allItems();
    for (QCanvasItemList::Iterator it=ilist.begin(); it!=ilist.end(); ++it)
    {
        if ( (*it)->rtti() == gadgetRTTI && (*it)->isVisible() )
        {
            if ( (*it)->x() < left )
                left = (*it)->x();
            if ( (*it)->x() > right )
                right = (*it)->x();
            if ( (*it)->y() < top )
                top = (*it)->y();
            if ( (*it)->y() > bottom )
                bottom = (*it)->y();
        }
    }
    // Add a small border so its not tight to the items
    left -= 5;
    right += 5;
    top -= 5;
    bottom += 5;
    return QRect( QPoint( left, top ), QPoint( right, bottom) );
}

QPoint WireUpOverview::convertToOverviewCoordinates( QPoint pt )
{
    QPoint newpt = QPoint( _scalef*( pt.x()- _boundary.left()), _scalef*(pt.y()- _boundary.top()) +_topborder );
    return newpt;
}
