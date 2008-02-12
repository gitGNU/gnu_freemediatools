
// Copyright (C) 2000-2006 VM Inc.
// Released under the GPL.

#ifndef STRETCHABLE_BUTTON_H
#define STRETCHABLE_BUTTON_H

// qt
#include <qpushbutton.h>
#include <qpixmap.h>

/// A button that can display a large pixmap, but upon which you 
/// can nominate sections to be expandable/collapsable.
/// This is achieved by taking a small vertical slice of the nominated
/// section and generating a new infill of the appropriate size e.g.:
///
/// The following is a pixmap, the ++ section can be modified. 
/// *****+++++***
/// *****+++++***
///
/// The generated section would look like:
/// +
/// +
/// 
/// and the button may appears as:
/// *****++***
/// *****++***
/// (contracted), or
/// *****++++++++***
/// *****++++++++***
///
/// This will only work horizontally
class StretchableButton : public QPushButton
{
public:
    StretchableButton( QWidget* parent = 0, const char* name = 0);

    void setPixmap( const QPixmap&, const QPixmap& );
    void setStretchableRange( int min, int max );

    virtual QSize minimumSizeHint() const;

protected:
    virtual void paintEvent( QPaintEvent* );
    virtual void resizeEvent( QResizeEvent* );
    virtual void enterEvent( QEvent* );
    virtual void leaveEvent( QEvent* );

private:
    QPixmap m_pixmap;
    QPixmap m_default;
    QPixmap m_hovered;

    int m_min, m_max;
};

#endif // STRETCHABLE_BUTTON_H
