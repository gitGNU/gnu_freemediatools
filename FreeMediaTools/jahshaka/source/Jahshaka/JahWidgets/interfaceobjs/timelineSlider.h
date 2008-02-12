
#ifndef TIMELINE_SLIDER_H
#define TIMELINE_SLIDER_H


// qt
#include <qwidget.h>

// std
#include <memory>

/// timeline based slider
class TimelineSlider : public QWidget
{
    Q_OBJECT

public:
    TimelineSlider( QWidget* parent = 0, const char* name = 0 );
    ~TimelineSlider();

    /// maintain interface with SlickSlider
    void setRange ( int minval, int maxval );
    void setMinValue ( int val );
    void setMaxValue ( int val );
    int minValue();
    int maxValue();
    int value();

    void setFPS( int, bool drop = true );

    virtual QSize sizeHint() const;
    virtual QSize minimumSizeHint() const;

public slots:
    void setValue ( int val );

signals:
    void sliderPressed();
    void sliderReleased();
    void sliderMoved( int );
    void valueChanged( int );
    void minChanged( int );
    void maxChanged( int );

protected:
    virtual void mousePressEvent( QMouseEvent *e );
    virtual void mouseReleaseEvent( QMouseEvent *e );
    virtual void mouseMoveEvent( QMouseEvent *e );
    virtual void paintEvent( QPaintEvent* );
    virtual void resizeEvent( QResizeEvent* );

private:
    struct PrivateTimelineSlider;
    std::auto_ptr< PrivateTimelineSlider > m_impl;
};

#endif // TIMELINE_SLIDER_H
