/*******************************************************************************
**
** The header file for the Jahshaka keyframer jahkeyframes file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef KEYFRAMEEDITOR_H
#define KEYFRAMEEDITOR_H

#include <qcanvas.h>
#include <qwmatrix.h>
#include <qstring.h>
#include <qstringlist.h>

// std
#include <map>
#include <vector>

///////////////////////////////////////////
// for the canvas activity

class KeyframeEditor : public QCanvasView {

    Q_OBJECT

public:
    const static QString ALL_KEY;

    KeyframeEditor(QCanvas*, QWidget* parent=0, const char* name=0, WFlags f=0);
    void cleareditor();

    void add( const QString& key, QCanvasItem* item );

public slots:
    void show( const QStringList& keys );

signals:
    void status(const QString&);
    void signalKeyframeChanged(void);

protected:
    void contentsMousePressEvent(QMouseEvent*);
    void contentsMouseMoveEvent(QMouseEvent*);
    virtual void resizeEvent ( QResizeEvent* );

private:
    QCanvasItem* moving;
    QPoint moving_start;

    typedef std::vector< QCanvasItem* > QCanvasItemVector;
    typedef std::map< QString, QCanvasItemVector > KeyItemMap;
    KeyItemMap keyItemMap;

    double zoomlevel;
};



#endif
