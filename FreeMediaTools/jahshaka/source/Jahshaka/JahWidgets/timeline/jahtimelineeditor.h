/*******************************************************************************
**
** The header file for the Jahshaka jahtimelineeditor file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHTIMELINEEDITOR_H
#define JAHTIMELINEEDITOR_H

#include "widget.h"
#include "jahtracer.h"

#include "openmedialib.h"

static const int imageRTTI2  = 984376;
static const int clipRTTI2   = 984010;
const bool no_writing2 = FALSE;

class EditItem: public QCanvasRectangle {
public:
    EditItem( assetData full, QCanvas *canvas, int layer, int x1, int y1, int x2, int y2 );

    int rtti () const { return clipRTTI2; }

    assetData itemdata;
    int itemlayer;
};


class EditEditor : public QCanvasView {
    Q_OBJECT

public:
    EditEditor(QCanvas*, QWidget* parent=0, const char* name=0, WFlags f=0);
    void cleareditor();
    void clearItem();
    void setItemtext(QString&);

    QImage globalimage;
    QString imagetext;
    EditItem *globalitem;

    assetData selectedClip;

    //temporary workaround here as sgi dont check for infunc creation properly
    bool bordercreated;
    bool move;

    void refresh();

protected:
    void contentsMousePressEvent(QMouseEvent*);
    void contentsMouseReleaseEvent(QMouseEvent*);
    void contentsMouseMoveEvent(QMouseEvent*);

signals:
    void status(const QString&);
    void updateEditanimframe(int);
    void updateSetinFrames(int,int);

private:
	////////////////////////////////////////
	//pointer to tracer singleton
	JahTrace			* jtrace;

    QCanvasItem* moving;
    QPoint moving_start;
    int height;

    QCanvasPolygonalItem  *border;
    QCanvasItem* borderframe;
    QCanvasText* itemtext;
};


#endif
