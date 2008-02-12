/*******************************************************************************
**
** The header file for the node class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHNODESITEM_H
#define JAHNODESITEM_H

#include <qpopupmenu.h>
//#include <qmainwindow.h>
#include <qintdict.h>
#include <qcanvas.h>

//?#include "assetdata.h"

class NodesEdgeItem;
class NodeItem;


class NodesEdgeItem: public QCanvasLine
{
public:
    NodesEdgeItem( NodeItem*, NodeItem*, QCanvas *canvas );
    NodesEdgeItem( int x, int y, NodeItem*, QCanvas *canvas ); //null header
    NodesEdgeItem( NodeItem*, int x, int y, QCanvas *canvas ); //null header

    void setFromPoint( int x, int y ) ;
    void setToPoint( int x, int y );

    static int count() { return c; }
    void moveBy(double dx, double dy);

private:
    static int c;
};



static const int nodeRTTI = 985376;

// TODO change baseclass to QCanvasSprite if we stick with sprite, not canvasellipe
class NodeItem: public QCanvasEllipse
{
public:
    NodeItem( QCanvas *canvas, QString name, int effectnum );
    ~NodeItem();

    void hide(); // reimpl to include child items

    int rtti () const { return nodeRTTI; }
    QString nodename;

    int layernumber;

    void addInEdge( NodesEdgeItem *edge ) { inList.append( edge ); }
    void addOutEdge( NodesEdgeItem *edge ) { outList.append( edge ); }

    void moveBy(double dx, double dy);

    void setImage( QImage im, QCanvas *canvas );
    //void setImage( assetData clip );
    void setName( QString name );

    void setLayerNumber( int num ) { layernumber = num; };

    void setSelected ( bool state );

    //    QPoint center() { return boundingRect().center(); }
    QCanvasText*    itemtext;
    QCanvasText*    itemtextshadow;
    QCanvasSprite *oval;

    QCanvasPixmapArray *ovalpm;

    QCanvasPixmapArray *pmimage;
    QCanvasSprite *imagesprite;
    QCanvasPixmap *pmap;

private:

    void recenterText(); // used when text changes.

    QPtrList<NodesEdgeItem> inList;
    QPtrList<NodesEdgeItem> outList;



};


#endif

