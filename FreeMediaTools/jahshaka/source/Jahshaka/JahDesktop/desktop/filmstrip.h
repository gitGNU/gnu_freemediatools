#ifndef FILMSTRIP_H
#define FILMSTRIP_H

#include <qcanvas.h>
#include "openmedialib.h"

class ImageItem;

static const int filmframeRTTI = 984379;

// A frame from the film strip. used by FilmStrip class.
class FilmFrame : public QCanvasSprite
{
public:
    FilmFrame ( ImageItem *image, QCanvasPixmapArray * a, QCanvas * canvas );
    ~FilmFrame();

    int number() { return fnum;} ;
    void setNumber( int num ) { fnum = num; } ;

    // returns the parent imageitem that we are expanded from
    ImageItem* imageItem() { return imageitem; } ;

    int rtti () const { return filmframeRTTI; };

protected:

    // overide the base class, so we can draw some extra stuff.
    void draw( QPainter & painter );

private:
    int fnum;
    ImageItem* imageitem;
};


static const int filmstripRTTI = 984377; 

// A filmstrip class that expands frames from an image item and lets the user scroll through them,
// generating frames either side on demand. NB dual inheritance, need QObject for signals, unless we do throu canvasview.
class FilmStrip : public QObject, public QCanvasRectangle 
{
    Q_OBJECT

public:

    // stripwidth is the approx number of pixels wide you need the filmstrip to be, e.g. get from canvasview::visibleWidth(). 
    // The code will add at least one offscreen frame at each end, so the create/destroy frame effect is hidden.
    // Zoom is the frame frequency, e.g. 1 for every frame, 2 for every other frame, and so on.
    FilmStrip( ImageItem *image, QCanvas *canvas, int stripwidth, int zoom = 1 );

    ~FilmStrip();

    int rtti () const { return filmstripRTTI; };

    void hide();

    // this moves all items, including the main bounding rectangle. See moveFramesBy().
    void moveBy( double dx, double dy );
    
    void moveFramesBy( double dx, double dy = 0 );

    
    // sets the z of the black bg rectangle to value and sets the z of the frames to value +1. 
    void setZ( double value );
    // sets the z of the black bg rectangle to value.
    void setBlackBackZ( double value );
    // sets the z of the frames only to value. Useful if you want something in front of the black bg but not the frames.
    void setFramesZ( double value );

    // returns the parent imageitem that we are expanded from
    ImageItem* imageItem() { return imageitem; }

    // Sets the velocity. Does not trigger velocity move itself.
    // values greater than 12 are clamped to 12. Negative velocity is to left. 
    void setVelocity ( double value );
	double getVelocity( ) { return xvelocity; }

	int getCentralFrameNumber() { return (leftmostframenum + ((rightmostframenum - leftmostframenum)/2)); };

signals:

    // emitted when a frame is clicked on. TODO emit this!
    // NOT IMPL YET void frameClicked( int framenumber );

public slots:

    // starts a velocity move. Slowing down is automatic. 
    void velocityMove();


protected:

    QList<QCanvasItem> frameholder;

    void initFrames();

    FilmFrame* createFrameCanvasSprite( QImage theimage );

    // TODO tidy fns. This fn is duplicate of the one in FigureEditor!
    QCanvasPixmapArray * createScaledPixmapArray ( QImage theimage );

    QImage getFrameImage( int framenum );

    // makes frames on demand. Returns an empty image if the frame requested is out of range.
    QCanvasSprite * getNewFrame( int frametoget );

    // We overide the drawing. We could draw nothing for our rectangle, or something fancier on top?
    void drawShape ( QPainter & p ); 

    void updateBlackStrip();

private:
    // all filmstrips are associated with a parent image item, set in constr.
    ImageItem *imageitem;

    // frames to show (either side of orig image item). NB cant be changed after constr.
    int ftoshow;
    int framewidth; 

    // keep track of our leftmost and rightmost X values and frame num, so we know when to get new ones. Set by initFrames.
    int leftmostframex;
    int rightmostframex;
    int leftmostframenum; 
    int rightmostframenum;
    QCanvasItem* rightmostframe;
    QCanvasItem* leftmostframe;
    QCanvasRectangle *blackstrip; // new. Was using basclass as bg, but hard to resize.
    int centralframex;
    bool sticky;
    double stickysnap;

    int framespacex; // pixel gap between frames. e.g. 2
    int zoomlevel; // default 1.
    int framespacey; // pixel gap at top and bottom of frames. e.g. 5. 

    double zframeheight; // stores z height of frames, as last set.

    bool frameinitdone;
    int stripwidth; 

    double xvelocity;
    int timerinterval;

    bool justdeletedframe;

    assetExchange *ourexchange;
};

#endif

