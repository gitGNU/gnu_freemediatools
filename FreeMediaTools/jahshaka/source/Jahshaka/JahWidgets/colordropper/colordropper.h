#ifndef COLORDROPPER_H
#define COLORDROPPER_H

#include <qwidget.h>
#include <qpixmap.h>
#include <qcolor.h>
#include <qpoint.h>

class QObject;
class QEvent;

// A simple color dropper button with magnifier.
// will work with all windows.
// while this is in existance, no events will be
// delivered to the rest of the application
class ColorDropper :  public QWidget
{
    Q_OBJECT

public:
    ColorDropper();
    ~ColorDropper();
    
    virtual bool eventFilter( QObject* watched, QEvent* e );

    static QPixmap colorDropperImage();

signals:
    // Emitted when the user chooses a color. Its up to the application to use it!  
    void colorPicked( const QColor& );

protected:
    virtual void paintEvent( QPaintEvent* );    

private:
    bool m_globalMouseTracking;
    QPoint m_pos;
};

#endif

