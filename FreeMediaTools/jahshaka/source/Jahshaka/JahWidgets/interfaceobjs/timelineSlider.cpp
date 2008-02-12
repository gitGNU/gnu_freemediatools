
#include "timelineSlider.h"

// local
#include "valueFormatters.h"

// qt
#include <qpainter.h>
#include <qfontmetrics.h>
#include <qfont.h>
#include <qpen.h>
#include <qbrush.h>
#include <qpoint.h>
#include <qpointarray.h>
#include <qpixmap.h>

// std
#include <algorithm>

struct TimelineSlider::PrivateTimelineSlider
{
    PrivateTimelineSlider( TimelineSlider* p )
        : parent( p ),
          min( 0 ),
          max( 0 ),
          v( 0 ),
          fps( 30 ),
          drop( true ),
          shouldTrack( false ),
          fontDirty( true ),
          requestedHeight( 0 )
        {}

    void updateValue()
    {
        // ensure min and max are the right way around...
        if ( min > max )
        {
            std::swap( min, max );
        }

        // clamp value to range
        int nv = std::max( std::min( v, max ), min );
        if ( nv != v )
        {
            v = nv;
            emit parent->valueChanged( v );
        }
    }

    int convertToValue( int val, int range ) const
    {
        return (int)( min + ( (double)val/range ) * ( max - min ) );
    }

    int convertToPixel( int val, int range ) const
    {
        return (int)( range * ( val - min ) / (double)(max - min ) );
    }

    QRect currentMarkerRect() const
    {
        return QRect( convertToPixel( v, parent->width() ) - parent->height(), 0, 2*parent->height(), parent->height() );
    }

    QFont getFontForHeight( int h )
    {
        if ( h < 5 )
        {
            return font;
        }

        if ( fontDirty || h != requestedHeight )
        {
            QSize s;
            QFont f( parent->font() );
            f.setPointSize( 1 );

            while ( s.height() < h )
            {
                f.setPointSize( f.pointSize() + 1 );
                QFontMetrics fm( f );
                s = fm.size( Qt::AlignLeft, "00" );
            }

            // and back off one notch
            font = f;
            font.setPointSize( font.pointSize() - 1 );

            requestedHeight = h;
            fontDirty = false;
        }

        return font;
    }
                    

    TimelineSlider* parent;

    // min, max, v are all in frames
    int min;
    int max;
    int v;

    int fps;
    bool drop;

    // mouse tracking
    bool shouldTrack;

    // font calculations
    bool fontDirty;
    int requestedHeight;
    QFont font;
};

TimelineSlider::TimelineSlider( QWidget* parent, const char* name )
    : QWidget( parent, name ),
      m_impl( new PrivateTimelineSlider( this ) )
{
    setBackgroundMode( Qt::NoBackground );
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding ) );
}

TimelineSlider::~TimelineSlider()
{}

void TimelineSlider::setRange ( int minval, int maxval )
{
    setMinValue( minval );
    setMaxValue( maxval );
}

void TimelineSlider::setMinValue( int val )
{
    if ( val > m_impl->max )
    {
        m_impl->max = val;
    }

    m_impl->min = val;
    m_impl->updateValue();
    emit minChanged( m_impl->min );
    update();
}

void TimelineSlider::setMaxValue( int val )
{
    if ( val < m_impl->min )
    {
        m_impl->min = val;
    }

    m_impl->max = val;
    m_impl->updateValue();
    emit maxChanged( m_impl->max );
    update();
}

int TimelineSlider::minValue()
{
    return m_impl->min;
}

int TimelineSlider::maxValue()
{
    return m_impl->max;
}

int TimelineSlider::value()
{
    return m_impl->v;
}

void TimelineSlider::setFPS( int fps, bool drop )
{
    m_impl->fps = fps;
    m_impl->drop = drop;
    update();
}

QSize TimelineSlider::sizeHint() const
{
    return minimumSizeHint();
}

QSize TimelineSlider::minimumSizeHint() const
{
    return QSize( 100, 25 );
}

void TimelineSlider::setValue( int val )
{
    if ( val == m_impl->v )
    {
        return;
    }

    m_impl->v = val;
    m_impl->updateValue();

    if ( m_impl->shouldTrack )
    {
        repaint();
    }
    else
    {
        update();
    }

    emit valueChanged( m_impl->v );
}

void TimelineSlider::mousePressEvent( QMouseEvent* e )
{
    emit sliderPressed();
    
    QRect marker( m_impl->currentMarkerRect() );
    m_impl->shouldTrack = marker.contains( e->pos() );

    e->accept();
}

void TimelineSlider::mouseReleaseEvent( QMouseEvent* e )
{
    emit sliderReleased();

    if ( m_impl->shouldTrack )
        setValue( m_impl->convertToValue( e->pos().x(), width() ) );

    m_impl->shouldTrack = false;
    e->accept();
}

void TimelineSlider::mouseMoveEvent( QMouseEvent* e )
{    
    if ( e->state() & Qt::LeftButton && m_impl->shouldTrack )
    {
        setValue( m_impl->convertToValue( e->pos().x(), width() ) );
        e->accept();
        emit sliderMoved( m_impl->v );
    }
}

static int findNiceInterval( int s, int fps )
{
    // align to s
    while ( s % fps )
    {
        ++s;
    }
//     // then, ensure 1s is the minimum gap
//     while ( ( s / fps ) % 1 )
//     {
//         ++s;
//     }

    return s;
}

void TimelineSlider::paintEvent ( QPaintEvent* e )
{
    QWidget::paintEvent( e );
    
    QPixmap buffer( width(), height() );
    QPainter p( &buffer );

    static const int PADDING = height() / 20;
    static const int HALF_HEIGHT = height() / 2;

    // use a fixed font size
    QFont f( m_impl->getFontForHeight( HALF_HEIGHT - 2*PADDING ) );
    p.setFont( f );

    // fill the background
    p.setBrush( colorGroup().button().light() );
    p.setPen( colorGroup().shadow() );    
    p.drawRect( 0, 0, width(), height() );

    // split the timeline vertically, draw timecodes in the lower half
    TimecodeValueFormatter fmt( m_impl->fps, m_impl->drop );
    QFontMetrics fm( p.fontMetrics() );

    // find the minimum spacing: two timecodes
    QSize fontSize = fm.size( Qt::AlignLeft, fmt.convert( 0 ) );
    int spacing = m_impl->convertToValue( 2 * fontSize.width(), width() );

    // round up to next nice value
    spacing = std::max( spacing, m_impl->fps );
    spacing = findNiceInterval( spacing, m_impl->fps );

    // draw timecodes
    p.setBrush( colorGroup().button() );
    p.setPen( QPen::NoPen );

    QRect lower( 1, HALF_HEIGHT, width() - 2, HALF_HEIGHT );
    p.drawRect( lower );
    
    int t = findNiceInterval( m_impl->min, m_impl->fps );

    p.setPen( colorGroup().text() );
    while ( t < m_impl->max )
    {
        int pos = m_impl->convertToPixel( t, width() );
        p.drawLine( pos, 1, pos, height() - 1 );
        fontSize = fm.size( Qt::AlignLeft, fmt.convert( t ) );
        p.drawText( pos + PADDING, 
                    height() - PADDING - fontSize.height(), 
                    fontSize.width(),
                    fontSize.height(),
                    Qt::AlignLeft, fmt.convert( t ) );

        t += spacing;
    }

    // draw marker
    const int HALF_MARKER_WIDTH = HALF_HEIGHT/2;
    QPointArray marker( 5 );
    marker.putPoints( 0, 5, 
                      HALF_MARKER_WIDTH, 1,
                      HALF_MARKER_WIDTH, HALF_MARKER_WIDTH,
                      0, HALF_HEIGHT,
                      -HALF_MARKER_WIDTH, HALF_MARKER_WIDTH,
                      -HALF_MARKER_WIDTH, 1 );
                      
    p.setPen( QColor( 150, 0, 50 ) );
    p.setBrush( QColor( 150, 0, 50 ) );

    const int markerPos = m_impl->convertToPixel( m_impl->v, width() );
    p.translate( markerPos, 0 );
    p.drawPolygon( marker );
    p.drawLine( 0, 1, 0, height() - 1 );
    fontSize = fm.size( Qt::AlignLeft, fmt.convert( m_impl->v ) );
    p.drawText( HALF_MARKER_WIDTH + PADDING,
                0,
                fontSize.width(),
                fontSize.height(), 
                Qt::AlignLeft, fmt.convert( m_impl->v ) );

    bitBlt( this, 0, 0, 
            &buffer, 0, 0, width(), height(), 
            Qt::CopyROP );

}

void TimelineSlider::resizeEvent( QResizeEvent* )
{
    m_impl->fontDirty = true;
}
