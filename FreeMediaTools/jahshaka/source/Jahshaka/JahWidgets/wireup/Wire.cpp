/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "Wire.h"
#include <qcanvas.h>
#include <qpixmap.h>
#include <qpainter.h>
#include <qpoint.h>
#include <qpointarray.h>
#include <qfont.h>
#include <math.h>

int Wire::_thickness = 2;
bool Wire::_simplelines = false;
bool Wire::_orientvertical = true;

// A bezier wire, which joins 2 sockets. 
Wire::Wire( QCanvas *canvas ) : QCanvasRectangle(canvas)
{
	init(); 	
}

void Wire::init()
{	
    hide();
    setIsMoving( false ); // (sets our pen color too)

    QPointArray initpts( 4 );  
    pts = initpts;
      
    // our bounding rect is only known after points are set.
    move(0, 0);
    setSize(0, 0);

    _insocket = 0;
    _outsocket = 0;

    _zprefered = z(); // we store this, so we can move wire to front, then back to where it was
}

Wire::~Wire()
{
	// QtCanvas deletes the canvas items for us -- 
	// we only need to worry about extra memory we have used.
}

void Wire::setBezierPoint( uint index, int x, int y )
{
    pts.setPoint( index, x, y );
    updateBoundingRect();
    update();
}


void Wire::setBezierPoint( uint index, const QPoint &pt )
{
    pts.setPoint( index, pt );
    updateBoundingRect();
    update();
}

void Wire::stretchBezierEnd( const QPoint &pt )
{

    int xgap = abs( pts.point(0).x() - pts.point(3).x() );
    xgap /=2;
    int ygap = abs( pts.point(0).y() - pts.point(3).y() );
    ygap /=2;
    if (xgap> 200 ) xgap = 200;
    if (ygap> 200 ) ygap = 200;
    pts.setPoint( 3, pt );
  
     // The mid points depend on orientation
    if ( verticalOrientation() )
    {   pts.setPoint( 1, pts.point(0) + QPoint(0, xgap+ygap) );
        pts.setPoint( 2, pts.point(3) - QPoint(0, xgap+ygap) );
    }
    else
    {   pts.setPoint( 1, pts.point(0) + QPoint( xgap+ygap, 0) );
        pts.setPoint( 2, pts.point(3) - QPoint( xgap+ygap, 0) );
    }

    updateBoundingRect();
}


void Wire::stretchBezierStart( const QPoint &pt )
{
    int xgap = abs( pts.point(0).x() - pts.point(3).x() );
    xgap /=2;
    int ygap = abs( pts.point(0).y() - pts.point(3).y() );
    ygap /=2;
    if (xgap> 200 ) xgap = 200;
    if (ygap> 200 ) ygap = 200;
    pts.setPoint( 0, pt );

    // The mid points depend on orientation
    if ( verticalOrientation() )
    {   pts.setPoint( 1, pts.point(0) + QPoint(0, xgap+ygap) );
        pts.setPoint( 2, pts.point(3) - QPoint(0, xgap+ygap) );
    }
    else
    {   pts.setPoint( 1, pts.point(0) + QPoint( xgap+ygap, 0) );
        pts.setPoint( 2, pts.point(3) - QPoint( xgap+ygap, 0) );
    }


    updateBoundingRect();
}


void Wire::moveBy(double dx, double dy)
{
     QCanvasRectangle::moveBy( dx, dy ); 
     // (dont need to re-check bounding region limits, the QCanvasRectangle just moves it).
}

void Wire::setPoint( uint index, int x, int y )
{
    pts.setPoint( index, x, y );
}

void Wire::setPoint( uint index, const QPoint &pt )
{
    pts.setPoint( index, pt );
}

void Wire::setControlPoints( QPointArray &a )
{
    pts = a;
    updateBoundingRect();
    update();
}

void Wire::drawShape ( QPainter & p )
{
    // (Note, pen color is set in setIsMoving fn).

    if ( !_ismoving )
    {   if (  isEnabled() )
            setPen( QPen ( QColor( "DarkGreen" ).light(200), _thickness  )); // to match enabled sockets
        else    
            setPen( QPen ( QColor( "gray50" ), _thickness  ));
    }

    if ( !_simplelines )
        p.drawCubicBezier( pts );
    else 
    {   // (these lines look fairly rubbish, could do something much better).  
        QPoint mid1 = 0.5*(pts.point(3) + pts.point(2));
        QPoint mid2 = 0.5*(pts.point(1) + pts.point(0));

        p.drawLine( pts.point(0), mid2 );
        p.drawLine( mid2, mid1 );
        p.drawLine( mid1, pts.point(3) );
  
        /* // These 3 lines define the bezier -- useful for debugging. 
        p.drawLine( pts.point(2), pts.point(3) );
        p.drawLine( pts.point(0), pts.point(1) );
        p.drawLine( pts.point(1), pts.point(2) );
        */
    }
    
    // TODO must draw at other end if pulling other end!
    if ( _ismoving )
    {   // Previously, we used a small block:
        //p.fillRect ( QRect(pts.point(3)-QPoint(5, 5), QSize(10, 10) ), QBrush (Qt::gray) );

        // Instead, we now use a small circle:
        p.setBrush( p.pen().color() );
        QPoint endpt;
        if ( outSocket() )
            endpt = pts.point(3);
        else endpt = pts.point(0);
        p.drawChord ( QRect(endpt -QPoint(5, 5), QSize(10, 10) ), 0, 5760 );
    }

}


void Wire::updateBoundingRect()
{
    int tx, ty;
    bool first = true; 
    int minx = 0, maxx = 0, miny = 0, maxy = 0;
    for ( int ipt = 0; ipt < pts.size(); ipt++ )
    {
        pts.point(ipt, &tx, &ty );
        if ( first || tx < minx ) minx = tx;
        if ( first || tx > maxx ) maxx = tx;
        if ( first || ty < miny ) miny = ty;
        if ( first || ty > maxy ) maxy = ty;
        first = false; 
    }
    // Since our pen width is >1, we must widen the bounding box a little (e.g. on Mac OS X we get 
    // artefacts at ends of wires due to bounding box being too small).
    minx -= 5;
    miny -= 5;
    maxx += 5;
    maxy += 5;

    // set posn and size of our (invisible) rectangle bounding box.
    QCanvasRectangle::move( minx, miny ); 
    QCanvasRectangle::setSize( (maxx - minx), (maxy - miny ) );
}

void Wire::setIsMoving( bool status )
{
    _ismoving = status;
    if ( _ismoving )
    {   
        setPen( QPen ( QColor( "DarkGreen" ).light(200), _thickness  )); // to match socket
        setZ( 1000000.0 ); //front
    }
    else
    {
        setPen( QPen ( QColor( "gray50" ), _thickness  ));
        setZ( _zprefered );
    }
} 

Socket* Wire::connectedSocket() 
{
    if ( inSocket() != 0 )
        return inSocket();
    else if ( outSocket() != 0 )
        return outSocket();

    return 0;
}

void Wire::setZ( double value )
 {
    QCanvasRectangle::setZ( value );
 }

void Wire::setVerticalOrientation( bool set )
{ 
    printf (" Wire::setVerticalOrientation %i \n", set );
    _orientvertical = set; 
}



