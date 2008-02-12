#ifndef JAHPROGRESSBAR_H
#define JAHPROGRESSBAR_H

#include <qprogressbar.h>
#include <qwidget.h>
#include <qframe.h>
#include <qimage.h>
#include <qcolor.h>

// A QProgressBar, with custom paintEvent so it looks like a 'Bluecurve-style' bar.
// Note, currently its fixed height (21 pixels).
class JahProgressBar : public QProgressBar
{
    Q_OBJECT
public:
    JahProgressBar( QWidget *parent, const char *name );
    JahProgressBar(int totalSteps, QWidget *parent, const char *name = 0);
    ~JahProgressBar();

    QSize sizeHint() const;
    
protected:
    void paintEvent( QPaintEvent *e);
    void resizeEvent(QResizeEvent *);

};

#endif
