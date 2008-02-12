#ifndef LEVELSPLOT_H
#define LEVELSPLOT_H

#include <qwidget.h>
#include <qframe.h>
#include <qimage.h>
#include <qcolor.h>
#include <qcheckbox.h>

//needed to find images
#include "jahpreferences.h" 

class LevelsPlot : public QFrame
{
    Q_OBJECT
public:
    LevelsPlot(QWidget *parent = 0, const char *name = 0);
    ~LevelsPlot();

    QSize sizeHint() const;

    enum Level { Luminosity, Reds, Greens, Blues, RGBStack, SplitH, SplitV } thelevel; 

    void setLevel( Level level );
    Level level() { return thelevel; };

    int meanLuminosity() { return ( int )meanLumin; };
    int meanRedLevel() { return ( int )meanRed; };
    int meanGreenLevel() { return ( int )meanGreen; };
    int meanBlueLevel() { return ( int )meanBlue; };

public slots:
    // Set a new image. Returns 0 if problem with image. 
    // TODO force 32 bit? To rescan a changed image, use the rescanImage slot.
    bool setTheImage( QImage *image );

    // Scans the image for new data. setImage must be called already.
    void rescanImage();

protected:
    void paintEvent( QPaintEvent *e);
    void resizeEvent(QResizeEvent *);

private:

    // Scans the current pixmap and re-sets the luminosity array. Returns true if image loaded ok.
    bool populateLevels( QImage * );

    bool realtime;

    int columnHeight( Level lev, int x );

    void clearEchoes();

    QColor penColor();

  //  QImage bg;
    QColor c;

    // draws the 'background' pixmap, if an update is needed. 
    void genBackground();
    bool mustGenerateBackground;
    QPixmap bg;

    QPixmap gridpixmap;

    // arrays for levels: luminosities / reds / greens / blues.
   int lumins[256];
   int reds[256];
   int greens[256];
   int blues[256];

   int echoes[256];

   // keeps stats for each set of levels
   int maxLumin;
   double meanLumin;
   int medianLumin;

   int maxRed;
   double meanRed;
   int medianRed;

   int maxGreen;
   double meanGreen;
   int medianGreen;

   int maxBlue;
   double meanBlue;
   int medianBlue;

   int numpixels;
    
   // bool mustGenerateBackground;
};

#endif
