/*******************************************************************************
**
** File part of the 'timeline' project.
** Copyright (C) 2005-2006 Visual Media Fx Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef TIMELINE_H
#define TIMELINE_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qdatetime.h>

//! A time/frame ruler widget with zoom and scroll fns. Capable of displaying a variety of timecode formats. 
class Ruler : public QWidget
{
Q_OBJECT

public:
    Ruler( QWidget* parent = 0, const char* name = 0 );
    ~Ruler();

    int framesPerSecond() { return _fps; };
    void setFramesPerSecond( int num );

    void setDropFrame( bool set ) { _dropFrame = set; };

    // Returns ms per frame.
    double msPerFrame() { return _mspf;} ; 

    QSize sizeHint() { return QSize(180, 50 ); }; 
 
    // TODO this fn doesnt set any of the other required settings or WFlags! see constr.
    void setDrawFancyBg( bool set ) { _drawfancybg = set ;};

    // These 2 functions return frame numbers adjusted for drop frames. 
    static int dropFrameIncrease( int frame );
    static int dropFrameDecrease( int frame );

    // Returns the string representation of the frame. Will use drop frame if set.
    QString frameToString( int frame );

public slots:

    // Zooms by the given amout. scalef actually scales the 'pixelsPerFrame' setting to produce the zoom effect.
    void zoom( double scalef );
    // Convenience slot, zooms in by a fixed amount.
    void zoomIn() { zoom( 0.833333 ); };
    // Convenience slot, zooms out by a fixed amount
    void zoomOut() { zoom( 1.2 ); };

    // Changes the start of the ruler by a fixed amount  (100 pixels).
    void stepRight();
    void stepLeft();

    //! Sets the max frame number, e.g. the length of the project timelime. 
    /*! The ruler cant go beyond this. 
        Doesn't force a repaint in case the user shortens a project.
    */
    void setMaxFrames( double frame );

    // Used for setting time code (fps and df settings) from string. e.g. used by menu/combo box. 
    void setTimeCodeFromString( const QString & );

    //! Used to set the now line pixel position via external change. TODO should we deal in frames or pixels?
   // TO TEST void updateNowLinePixel( double x );

signals:

    void nowChanged( int ); // TODO Should these be in a parent class?
    void nowStringChanged( const QString& );
    void nowLinePixelMoved( double x ); // x is pixel units.

    // Emitted when the interval or start position of the major ticks changed. e.g. picked up by the trackscanvasview.
    void majorTicksChanged( double firsttickframe, double frameinterval );

    // Emitted when the start frame changes, e.g. the ruler is scrolled.
    void startFrameChanged( double );

    // TODO delete or emit this (or move to parent?)
    void pixelsPerFrameChanged( double );

protected slots:

protected:

	virtual void paintEvent ( QPaintEvent * );
    void paintTickMarks( QPainter &painter );

    // tick painter.
    void paintFrameTick( QPainter &painter, double frame, bool minor = false );

    // now marker painter. its basically a triangle, but can be changed easily.
    void paintNowMarker( QPainter &painter, int xpos, int nowsize );

    // overide, so we can tell the bg to regenerate
    virtual void resizeEvent( QResizeEvent *);

    // Works out the tick positions and types based on the current settings.
    void determineTicks();

    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
	void mouseReleaseEvent(QMouseEvent*);

    void wheelEvent ( QWheelEvent * e );

private:

    // draws the background pixmap, if an update is needed. 
    void genBackground();
    bool mustGenerateBackground;

    // Checks the tick spacings and first ticks to draw.
    bool mustDetermineTicks;

    // Checks if the major/minor ticks will fit based on current settings.
    /*! All 3 args are in milliseconds. major and minor are then the tick intervals. 
        _maxticks must already be set.
    */
    bool tickChecker( double totalms, double major, double minor );

    bool _drawfancybg;
    QPixmap bg;
    QColor _uppercol;
    QColor _lowercol;
    QColor _foregroundcol;
    QColor _highlightcol;
    QColor _framecol; 
    QColor _nowmarkercol; 
    QColor _nowmarkerpencol;
    
    int _fps; // frames per second. MOVE
    double _mspf; // ms per frame is re-calulated when _fps is set.  // MOVE
    bool _dropFrame; // use only for 30fps df timecode.  // MOVE

    // We work in frames and subframes, hence floating point values for all these.
    //TODO all these to move out, to share data with other widgets.
    double _startFrame;  // MOVE
    double _maxFrames; // MOVE 
    double _firstMajorTickFrame; 
    double _firstMinorTickFrame;
    double _pixelsPerFrame; // MOVE
    double _majorTickFrames;  
    double _minorTickFrames;
    double _nowxpos;
    double _nowframe; // MOVE

    int _majorTickHeight;
    int _minorTickHeight;

    double _maxTicksPerPixel;

    // Mouse movement
    bool _dragging;
    QPoint _dragpoint;
    bool _draggingnow;

    //QPixmap _nowpixmap;  // Testing
    int _nowsize;
    int _nowsizehalf;

};

#endif
