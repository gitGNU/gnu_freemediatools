/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef WIREUPCANVAS_H
#define WIREUPCANVAS_H

#include <qcanvas.h>
#include <qevent.h>

/*! Class derived from QCanvas. 
    Useful if we want to add signals or other top level canvas features, which can be called from canvasitems.
*/
class WireupCanvas : public QCanvas {

    Q_OBJECT

    friend class Gadget; // so they can use our signals
    friend class Wire; // so they can use our signals
    friend class Socket; // so they can use our signals


public:
    WireupCanvas(QWidget* parent=0, const char* name=0);

signals:

    // TODO

protected: 
    void drawForeground ( QPainter & painter, const QRect & clip );

};

#endif
