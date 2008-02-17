
#include "valueFormatters.h"

// qt
#include <qstring.h>

// std
#include <cmath>

TimecodeValueFormatter::TimecodeValueFormatter()
    : m_fps( 30 ),
      m_dropFrame( true )
{}

TimecodeValueFormatter::TimecodeValueFormatter( int fps, bool drop )
    : m_fps( fps ),
      m_dropFrame( drop )
{
}

void TimecodeValueFormatter::setFPS( int fps, bool drop )
{
    m_fps = fps;
    m_dropFrame = drop;
}

int TimecodeValueFormatter::correct( int frame ) const
{
    // rather clumsy...
    // cap the maximum frame number (at c.a. 2.7h) to avoid 
    // a very long loop if used with e.g. a range control
    // with no upper limit.
    // Would be better to have properly bound properties.
    if ( m_dropFrame && m_fps == 30 && frame < 300000 )
    {
        // Courtesy of Mark McGrath - added by Charles Yates
        const int min = m_fps * 60;
        const int ten = m_fps * 600;
        int prev_val = frame;
        int prev_adj = 0;
        
        do
        {
            prev_val = frame;
            int mins = frame / min;
            int tens = frame / ten;
            int adj = 2 * ( mins - tens );
            frame += adj - prev_adj;
            prev_adj = adj;
        } 
        while( frame != prev_val );
    }
    return frame;
}

QString TimecodeValueFormatter::convert( int frame ) const
{
    int val = correct( abs( frame ) );
    int ff = val % m_fps;
    int ss = ( val / m_fps ) % 60; 
    int mm = ( val / ( m_fps * 60 ) ) % 60;
    int hh = ( val / ( m_fps * 3600 ) );

    QString s;
    s.sprintf( "%c%d:%02d:%02d%c%02d", frame < 0 ? '-' : '0', hh, mm, ss, m_dropFrame ? ';' : ':', ff );
    return s;
}

IntValueFormatter* TimecodeValueFormatter::clone() const
{
    return new TimecodeValueFormatter( *this );
}

IntValueFormatter::~IntValueFormatter()
{

}

IntValueFormatter::IntValueFormatter()
{

}
