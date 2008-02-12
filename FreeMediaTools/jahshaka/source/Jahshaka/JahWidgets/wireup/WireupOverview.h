/*******************************************************************************
**
** File part of the 'wireup' project.
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef WIREUPOVERVIEW_H
#define WIREUPOVERVIEW_H

#include <qcanvas.h>
#include <qimage.h>

// RTTI helper
static const int overviewRTTI = 850009;

//! This class generates an overview sprite of a wire up.
/*! Updates are performed using regenerate(), but this should be done sparingly (e.g. via a QTimer, or only
    on mouserelease event) since it is not particularly cheap to perform.
    We decided against using a scaled QCanvasView model/view since the matrix scaling is expensive and
    it looks pretty ugly to try to scale everything down to an overview.
*/
class WireUpOverview : public QCanvasRectangle 
{
   // Q_OBJECT  //?
public:
    //! Constr. Note, the initial QCanvasPixmapArray is replaces as soon as generatePixmap is called.
    WireUpOverview( QCanvas * canvas );
    ~WireUpOverview();

    //! Redraws everything. Call sparingly.
    void regenerate();

    //! Overide base class, so we can deal with some extra issues for changing size.
    void setSize ( int w, int h );
    void setScaleFactor( double scalef );
    //! Sets the rectangle that shows the current visible contents.
    void setContentsRect( QRect contents );

    //! Returns a contents rectangle point, based on pixel position in the overview.
    /*! TODO should return centre?
    */
    QPoint pixelToContentsPoint( QPoint pos );

    int rtti () const { return overviewRTTI; };

protected:

    //! makes the whole new pixmap ready for drawShape.
    void generatePixmap();

    // overide the base class, so we can draw ourselves.
    void drawShape( QPainter & painter );

    //! Checks the canvas for the boundaries of all items.
    /*! NB the value is also cached into _boundary.
    */
    QRect findCanvasBoundary();

    //! Using current boundary and scale settings, converts the chosen point on the main canvas.
    QPoint convertToOverviewCoordinates( QPoint pt );

private:

    QCanvas* _canvas;
    int _width; 
    int _height;
    double _scalef;
    QRect _contentsRect;
    QRect _boundary; // boundary of gadget items.
    QPixmap pm;
    QImage _bg;

    int _topborder; // space taken by our grabbable 'title bar'.

};

#endif
