/*******************************************************************************
**
** The header file for the jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNODEEDITOR_H
#define JAHNODEEDITOR_H

#include <qcanvas.h>
#include <qwmatrix.h>

#include "jahnodesitem.h"

///////////////////////////////////////////
// for the canvas activity

class NodeEditor : public QCanvasView {
    Q_OBJECT

friend class JahNodes;

public:
    NodeEditor(QCanvas*, QWidget* parent=0, const char* name=0, WFlags f=0);
    void clear();
    QCanvasText* itemtext;
    
protected:
    void contentsMousePressEvent(QMouseEvent*);
    void contentsMouseMoveEvent(QMouseEvent*);
    //void contentsMouseReleaseEvent(QMouseEvent*);

    void setLastSelected( NodeItem* item ) { lastselected = item; };
    NodeItem * lastSelected() { return lastselected; };
    void clearSelection();

signals:
    void status(const QString&);
    void nodeClicked( int layernumber );

private:
    //QCanvasItem* moving;
    NodeItem* moving;
    QPoint moving_start;

    NodeItem* lastselected;

};


#endif
