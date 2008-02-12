/****************************************************************************
**
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*****************************************************************************/

#ifndef SUPERGRANGECONTROL_H
#define SUPERGRANGECONTROL_H

#include <qwidget.h>
#include <qrangecontrol.h>
#include <qtimer.h>
#include <qpoint.h>

// std
#include <memory>

class IntValueFormatter;

//! A custom widget to implement the integer range control for Super G theme.
/*  Behaviour is similar to a slider, except there is no handle, the whole width can be 
    clicked and moved. Hence, class cannot derive from QSlider. Moreover, there are
    two types of limit. The normal maxValue/minValue are honoured, but optionally the
    user is allowed to go beyond theses in one or both directions. 
    
    A marker shows the current value (if in visible range), along with the text value. 
    The amount of movement is proportional to the pixles moved unless shift key is 
    pressed (then its +/- 1). The 2 arrow controls also change the value by +/- 1 
    The scroll wheel (with and without shift) and other keys also implemented.

    Note, you *must* set setVisibleMinValue and setVisibleMaxValue as well as the usual
    setMinValue and setMaxValue if you do not use the contructor that takes them all, or
    if you change them after construction. Also, by default, setExpandableRight and 
    setExpandableLeft are set to false, the control will not expand. If you enable them, 
    you want to also set maxexpandvalue and minexpandvalue.
    
*/
class SupergRangeControl : public QWidget, private QRangeControl
{
    Q_OBJECT

public:
    SupergRangeControl( QWidget *parent=0, const char *name=0 );
    ~SupergRangeControl();

    virtual void setTracking( bool enable );
    bool tracking() const;

    void setRange( int min, int max );
    void setMax( int );
    void setMin( int );

    /// Allow the integer value to be formatted however we may please
    void setFormatter( const IntValueFormatter& );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

    typedef enum Default
    {
        None,
        Zero,
        Min,
        Max
    };

    // Allow default gravity to be specified
    void setDefault( Default val );

public slots:
    void setValue( int );

signals:
    void valueChanged( int value );
    void inputPressEvent( QMouseEvent * );
    void inputPressEvent( );
    void inputReleaseEvent( );

protected:
    void valueChange();

    void keyPressEvent( QKeyEvent* );
    void keyReleaseEvent( QKeyEvent* );
    void mousePressEvent( QMouseEvent* );
    void mouseReleaseEvent( QMouseEvent* );
    void mouseMoveEvent( QMouseEvent* );
    void mouseDoubleClickEvent ( QMouseEvent* );
    void wheelEvent( QWheelEvent* );

    virtual void paintEvent( QPaintEvent* );

protected slots:
    //! Deals with pressing and holding arrow controls
    void mouseHoldHelperSlot();

private:
    enum ArrowDirection
    {
        EAST,
        WEST
    };

    void paintTriangle( QPainter &painter, const QPoint& tip, int size, ArrowDirection ) const;

    QPoint pressedAt;
    bool track;
    bool mousePressed;
    bool shift;
    Default m_default;

    bool leftarrowpressed;
    bool rightarrowpressed;
    QTimer* mousehold;

    // keep track of how wide the last text displayed was
    int m_textWidth;

    std::auto_ptr< IntValueFormatter > m_formatter;
};

#endif
