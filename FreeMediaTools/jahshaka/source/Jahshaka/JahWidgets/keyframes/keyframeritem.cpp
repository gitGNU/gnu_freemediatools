/*******************************************************************************
**
** The header file for the node class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyframeritem.h"
#include "gpumathlib.h"

///////////////////////////////////////////////
// for the keyframe item

void KeyframeItem::moveBy(double dx, double dy)
{
//	debug("in keyframe moveby");
    QCanvasEllipse::moveBy( dx, dy );
    
    QColor point_color = brush().color();

    int canvas_height_div_2 = canvas()->height() / 2;

    if (   point_color == KEYFRAMER_SCALE_X_COLOR 
        || point_color == KEYFRAMER_SCALE_Y_COLOR 
        || point_color == KEYFRAMER_SCALE_Z_COLOR )
    {
        *m_keyframe_value_ptr = 1.0f + ( ( ( -y() + canvas_height_div_2 + Globals::getKeyframerYOffset() ) * KEYFRAMER_Y_SCALE_FACTOR / Globals::getKeyframerZoomFactor() / 100.0) );
    }
    else
    {
        *m_keyframe_value_ptr = ( ( -y() + canvas_height_div_2 + Globals::getKeyframerYOffset() ) * KEYFRAMER_Y_SCALE_FACTOR / Globals::getKeyframerZoomFactor() );
    }
    
    QPtrListIterator<KeyEdgeItem> it1( inList );
    KeyEdgeItem *edge;

    while (( edge = it1.current() )) 
    {
        ++it1;
        edge->setToPoint( int(x()), int(y()) );
    }
    
    QPtrListIterator<KeyEdgeItem> it2( outList );
    
    while (( edge = it2.current() )) 
    {
        ++it2;
        edge->setFromPoint( int(x()), int(y()) );   
    }
}



KeyframeItem::KeyframeItem( QColor color, QCanvas *canvas, float* keyframe_value_ptr )
    : QCanvasEllipse( 6, 6, canvas )
{
    setPen( QPen(Qt::white) );
    setBrush( QBrush(color) );
    setZ( 128 );
    m_keyframe_value_ptr = keyframe_value_ptr;
}

////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////
//

int KeyEdgeItem::c = 0;

void KeyEdgeItem::moveBy(double, double)
{
    //nothing
}

KeyEdgeItem::KeyEdgeItem( KeyframeItem *from, KeyframeItem *to, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    from->addOutEdge( this );
    to->addInEdge( this );
    setPoints( int(from->x()), int(from->y()), int(to->x()), int(to->y()) );
    setZ( 127 );
}


KeyEdgeItem::KeyEdgeItem( int x, int y, KeyframeItem *to, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    //from->addOutEdge( this );
    to->addInEdge( this );
    setPoints( x, y, int(to->x()), int(to->y()) );
    setZ( 127 );
}

KeyEdgeItem::KeyEdgeItem( KeyframeItem *from, int x, int y, QCanvas *canvas )
    : QCanvasLine( canvas )
{
    c++;
    setPen( QPen(Qt::white) );
    setBrush( QBrush(Qt::red) );
    from->addOutEdge( this );
    //to->addInEdge( this );
    setPoints( int(from->x()), int(from->y()), x, y );
    setZ( 127 );
}

void KeyEdgeItem::setFromPoint( int x, int y )
{
    setPoints( x,y, endPoint().x(), endPoint().y() );
}

void KeyEdgeItem::setToPoint( int x, int y )
{
    setPoints( startPoint().x(), startPoint().y(), x, y );
}

int KeyEdgeBezierItem::m_count = 0;

void 
KeyEdgeBezierItem::setStartPoint(int x, int y)
{
    getPointArray()->setPoint(0, x, y);
}

void 
KeyEdgeBezierItem::setIntermediatePointOne(int x, int y)
{
    getPointArray()->setPoint(1, x, y);
}

void 
KeyEdgeBezierItem::setIntermediatePointTwo(int x, int y)
{
    getPointArray()->setPoint(2, x, y);
}

void 
KeyEdgeBezierItem::setEqualIntermediatePoints(int x, int y)
{
    getPointArray()->setPoint(1, x, y);
    getPointArray()->setPoint(2, x, y);
}

void 
KeyEdgeBezierItem::setEndPoint(int x, int y)
{
    getPointArray()->setPoint(3, x, y);
}

int 
KeyEdgeBezierItem::getCount() 
{   
    return m_count; 
}

void 
KeyEdgeBezierItem::moveBy(double, double)
{
}

