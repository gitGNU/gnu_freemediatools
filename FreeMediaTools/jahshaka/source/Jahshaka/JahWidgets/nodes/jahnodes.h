/*******************************************************************************
**
** The header file for the jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNODES_H
#define JAHNODES_H

#include <qcanvas.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qcanvas.h>
#include <qwmatrix.h>
#include <qimage.h>

#include "jahnodesitem.h"
#include "jahnodeeditor.h"

#include "jahtracer.h"

///////////////////////////////////////////
// main class
class LayerList
{
public:
    LayerList( const QString& name, int layer ) { n=name; l=layer; mynode=0; }
    QString     name()   const                    { return n; }
    int         layer() const                     { return l; }
    NodeItem * nodeItem() const                   { return mynode; };
    void setNodeItem( NodeItem *node )          { mynode = node; };
    void setLayerNumber ( int num )             { l = num; };
private:
    QString     n;
    int         l;
    NodeItem *mynode; // set in buildNodes, zero initially.
};


class JahNodes : public QWidget {
    Q_OBJECT

public:
    JahNodes(QWidget* parent=0, const char* name=0, WFlags f=0);
    ~JahNodes();

    NodeEditor *nodeeditor; // made public for easier access

    // This image (if set) will be used for the top node. A copy is resized here.
    void setMainImage( QImage im );

    void setAssetName( QString name ) { assetname = name; };

public slots:
    void addNodes(int x, QString name);

    // Clears the xth node in the list. 
    // The first effect is number 0. You cant delete the 'image' node using the top image node using this fn. 
    void delNodes( int x );
    void clear(); // TODO: tidy bad function

    void selectNode( int x );

    void rebuildNodes() { buildNodes(""); };
    
private slots:

    void buildNodes(QString name);
    void drawLine(int x1, int y1, int x2, int y2);
    void buildInterface();

    void enlarge();
    void shrink();
    void zoomIn();
    void zoomOut();

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

    QCanvas* canvas;

    QPtrList<LayerList> *list;

    QPtrList<NodesEdgeItem> edgelist;

    QBrush *tb;
    QPen *tp;

    int nodenumber;

    // TODO: dont store here, just in nodeitem. Or store assetData here.
    QImage mainimage;
    // Store a 'base' asset name, prob set to the image name.
    // TODO: dont store here, just in nodeitem?
    QString assetname;

};

#endif
