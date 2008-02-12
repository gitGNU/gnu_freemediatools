/*******************************************************************************
**
** The header file for the node class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHKEYFRAMEITEM_H
#define JAHKEYFRAMEITEM_H

#include <qpopupmenu.h>
//#include <qmainwindow.h>
#include <qintdict.h>
#include <qcanvas.h>
#include <qpainter.h>
#include <qpointarray.h>

//#include <nodes.h>
class KeyEdgeItem;
class KeyframeItem;

static const float KEYFRAMER_Y_SCALE_FACTOR = 12.0f;
static const QColor KEYFRAMER_SCALE_X_COLOR = Qt::black;
static const QColor KEYFRAMER_SCALE_Y_COLOR = Qt::gray;
static const QColor KEYFRAMER_SCALE_Z_COLOR = Qt::white;


class KeyEdgeItem: public QCanvasLine
{
public:
    KeyEdgeItem( KeyframeItem*, KeyframeItem*, QCanvas *canvas );

    KeyEdgeItem( int x, int y, KeyframeItem*, QCanvas *canvas ); //null header

    KeyEdgeItem( KeyframeItem*, int x, int y, QCanvas *canvas ); //null header

    void setFromPoint( int x, int y ) ;
    void setToPoint( int x, int y );

    static int count() { return c; }
    void moveBy(double dx, double dy);

private:
    static int c;
};

class KeyEdgeBezierItem: public QPainter
{
public:
    KeyEdgeBezierItem( KeyframeItem*, int x, int y, QCanvas *canvas );

    void setStartPoint(int x, int y);
    void setIntermediatePointOne(int x, int y);
    void setIntermediatePointTwo(int x, int y);
    void setEqualIntermediatePoints(int x, int y);
    void setEndPoint(int x, int y);

    static int getCount();
    void moveBy(double dx, double dy);
    QPointArray* getPointArray() { return &m_point_array; }

private:
    static int m_count;
    QPointArray m_point_array;
};


static const int keyframeRTTI = 990000;

class KeyframeItem: public QCanvasEllipse
{
public:

    KeyframeItem( QColor color, QCanvas *canvas, float* keyframe_value_pointer );
    ~KeyframeItem() {}
    
    int rtti () const { return keyframeRTTI; }

    void addInEdge( KeyEdgeItem *edge ) { inList.append( edge ); }
    void addOutEdge( KeyEdgeItem *edge ) { outList.append( edge ); }
    void moveBy(double dx, double dy);

private:
    QPtrList<KeyEdgeItem> inList;
    QPtrList<KeyEdgeItem> outList;
    float* m_keyframe_value_ptr;
     
};



#endif

