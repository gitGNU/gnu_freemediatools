
// Copyright (C) 2000-2006 VM Inc.
// Released under the GPL.

#include <algorithm>

#include "stretchableButton.h"

StretchableButton::StretchableButton( QWidget* parent, const char* name )
    : QPushButton( parent, name ),
      m_min( 0 ),
      m_max( 0 )
{
    setFlat( true );
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
}

void StretchableButton::setPixmap( const QPixmap& def, const QPixmap& hovered )
{
    m_default = def;
    m_hovered = hovered;

    m_pixmap = m_default;

    setMinimumSize( m_pixmap.width() - ( m_max - m_min ), m_pixmap.height() );
    update();
}

void StretchableButton::setStretchableRange( int min, int max )
{
    if ( min < 0 ) min = 0;
    if ( max < 0 ) max = 0;

    m_min = min;
    m_max = max;
    if ( m_min > m_max )
    {
        std::swap( m_min, m_max );
    }
    setMinimumSize( m_pixmap.width() - ( m_max - m_min ), m_pixmap.height() );

    update();
}

QSize StretchableButton::minimumSizeHint() const
{
    if ( !m_pixmap.width() )
    {
        return m_pixmap.size();
    }

    return QSize( m_pixmap.width() - ( m_max - m_min ), m_pixmap.height() );
}

void StretchableButton::paintEvent( QPaintEvent* )
{
    if ( m_min == 0 && m_max == 0 )
    {
        bitBlt( this, 0, 0, 
                &m_pixmap, 0, 0, m_pixmap.width(), m_pixmap.height(), 
                Qt::CopyROP );
        return;
    }

    QPixmap filler( 1, m_pixmap.height() );
    QPixmap p( width(), height() );
    
    // get filler
    copyBlt( &filler, 0, 0, &m_pixmap, m_min, 0, 1, m_pixmap.height() );

    // work out how much filler we need
    const int fillerDistance = width() - minimumSizeHint().width();
    
    // get left
    copyBlt( &p, 0, 0, &m_pixmap, 0, 0, m_min, m_pixmap.height() );

    // filler
    int i = 0;
    while ( i < fillerDistance )
    {
        copyBlt( &p, m_min + i++, 0, &filler, 0, 0, 1, filler.height() );
    }

    // right
    copyBlt( &p, m_min + fillerDistance, 0, &m_pixmap, m_max, 0, m_pixmap.width() - m_max, m_pixmap.height() );

    bitBlt( this, 0, 0, 
            &p, 0, 0, width(), height(), 
            Qt::CopyROP );

}

void StretchableButton::resizeEvent( QResizeEvent* e )
{
    QPushButton::resizeEvent( e );
    update();
}

void StretchableButton::enterEvent( QEvent* e )
{
    QPushButton::enterEvent( e );
    m_pixmap = m_hovered;
    update();
}

void StretchableButton::leaveEvent( QEvent* e )
{
    QPushButton::leaveEvent( e );
    m_pixmap = m_default;
    update();    
}
