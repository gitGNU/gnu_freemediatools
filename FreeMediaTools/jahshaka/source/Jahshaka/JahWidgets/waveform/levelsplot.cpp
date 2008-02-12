
#include <qmap.h>
#include <qpixmap.h>
#include <qimage.h>
#include <qevent.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qfont.h>
#include <math.h>

#include "levelsplot.h"

LevelsPlot::LevelsPlot(QWidget *parent, const char *name)
    : QFrame(parent, name, WNoAutoErase), realtime(true)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    setFrameShape(Box);
    setFrameShadow(Sunken);

    thelevel = LevelsPlot::Luminosity;

    clearEchoes();
    mustGenerateBackground = true;

    //need preferences object to find this
    
    //set the base path for relative pixmap location
    JahPrefs& jprefs = JahPrefs::getInstance();    
    QString JahBasePath = jprefs.getBasePath( ).data();

    //JahBasePath+ "Pixmaps/Widgets/plotbg.png"
    gridpixmap = QPixmap( JahBasePath+ "Pixmaps/widgets/plotbg.png" ); // TODO draw a grid in mem?

    setMaximumWidth(256); // TODO allow scaling?
    setMaximumHeight(gridpixmap.height());
}


LevelsPlot::~LevelsPlot()
{
}

void LevelsPlot::setLevel( Level level ) 
{ 
    thelevel = level;
    repaint();
}

bool LevelsPlot::setTheImage( QImage *image ) 
{ 
	populateLevels( image );
	mustGenerateBackground = true;
	update();

    return true;
}

void LevelsPlot::rescanImage()
{
    mustGenerateBackground = true;
    update();
}

void LevelsPlot::genBackground()
{
    bg = QPixmap( size() );
    bg.fill(colorGroup().base() );

    QPainter painter( &bg );

    //QColor color( palette().active().highlight() );
    QColor color = penColor();

    painter.setBrush( Qt::SolidPattern );
    painter.setPen(QPen( color, 1, SolidLine ));   
    painter.setBrush( color );

    // loop across the histogram width. Assumes width is exactly 256, no scaling options yet.
    int colwidth = 1;
    int colheight, colheight2, colheight3, colheight4;
    int quarterh = int( height() / 4);
    for ( int x = 0; x < 256; x++ )
    {   
        if ( thelevel == Reds || thelevel == Greens || thelevel == Blues || thelevel == Luminosity )
        {   colheight = columnHeight( thelevel, x );
            painter.fillRect( x, height()- colheight, colwidth, colheight, QBrush( color ) ); 
        }   
        else if ( thelevel == RGBStack )
        {   colheight = int(columnHeight( Reds, x ) / 3);
            colheight2 = int(columnHeight( Greens, x ) / 3);
            colheight3 = int(columnHeight( Blues, x ) / 3);
            painter.fillRect( x, height() - colheight, colwidth, colheight, QBrush( Qt::red ));  
            painter.fillRect( x, height() - colheight2 - colheight, colwidth, colheight2, QBrush( Qt::green ));
            painter.fillRect( x, height() - colheight3 - colheight - colheight2, colwidth, colheight3, QBrush( Qt::blue ));
        }
        else if ( thelevel == SplitV )
        {   colheight = int(columnHeight( Reds, x ) / 4);
            colheight2 = int(columnHeight( Greens, x ) / 4);
            colheight3 = int(columnHeight( Blues, x ) / 4);
            colheight4 = int(columnHeight( Luminosity, x ) / 4);
            painter.fillRect( x, height() - colheight4 - 3*quarterh, colwidth, colheight4, QBrush( Qt::white ));
            painter.fillRect( x, height() - colheight - 2*quarterh, colwidth, colheight, QBrush( Qt::red ));  
            painter.fillRect( x, height() - colheight2 - quarterh, colwidth, colheight2, QBrush( Qt::green ));
            painter.fillRect( x, height() - colheight3, colwidth, colheight3, QBrush( Qt::blue ));
        }
        else if ( thelevel == SplitH )
        {   
            if ( x < 64 )
            {   int quartx = x*4;
                colheight = columnHeight( Luminosity, quartx );
                colheight2 = columnHeight( Luminosity, quartx+1 );
                colheight3 = columnHeight( Luminosity, quartx+2 );
                colheight4 = columnHeight( Luminosity, quartx+3 );
                int averageh = int( (colheight+colheight2+colheight3+colheight4) /4 );
                painter.fillRect( x, height()- averageh, colwidth, averageh, QBrush( Qt::white ) );
            }
            else if ( x < 128 )
            {   int quartx = (x - 64)*4;
                colheight = columnHeight( Reds, quartx );
                colheight2 = columnHeight( Reds, quartx+1 );
                colheight3 = columnHeight( Reds, quartx+2 );
                colheight4 = columnHeight( Reds, quartx+3 );
                int averageh = int( (colheight+colheight2+colheight3+colheight4) /4 );
                painter.fillRect( x, height()- averageh, colwidth, averageh, QBrush( Qt::red ) );
            }
            else if ( x < 192 )
            {   int quartx = (x - 128)*4;
                colheight = columnHeight( Greens, quartx );
                colheight2 = columnHeight( Greens, quartx+1 );
                colheight3 = columnHeight( Greens, quartx+2 );
                colheight4 = columnHeight( Greens, quartx+3 );
                int averageh = int( (colheight+colheight2+colheight3+colheight4) /4 );
                painter.fillRect( x, height()- averageh, colwidth, averageh, QBrush( Qt::green ) );
            }
            else
            {   int quartx = (x - 192)*4;
                colheight = columnHeight( Blues, quartx );
                colheight2 = columnHeight( Blues, quartx+1 );
                colheight3 = columnHeight( Blues, quartx+2 );
                colheight4 = columnHeight( Blues, quartx+3 );
                int averageh = int( (colheight+colheight2+colheight3+colheight4) /4 );
                painter.fillRect( x, height()- averageh, colwidth, averageh, QBrush( Qt::blue ) );
            }
        }
       
       
       // TO FIX - echo bars.
       //if ( echoes[x] > 1 )
       //   { painter.setPen( QPen( black, 4, SolidLine ));
       //    painter.drawPoint( x, height() - echoes[x]);
       // }
    }
    
    // finished, so blit screen
    painter.drawPixmap( 0, 0, gridpixmap );
    painter.end();
    mustGenerateBackground = false;
}

void LevelsPlot::paintEvent ( QPaintEvent * )
{
    if ( mustGenerateBackground )
        genBackground();

    // For now, just let qt clear our widget, we'll paint directly (seems fine for non-fancy bg rect)
    QPainter paint( this );
    // paint the pre-drawn bg onto ourselves.
    paint.drawPixmap( rect(), bg );
    paint.end();

}

void LevelsPlot::clearEchoes()
{
    for( int l = 0; l < 256; l++ )
    {
        echoes[l] = 0;
    }
}

bool LevelsPlot::populateLevels( QImage *theimage )
{
   // Warning: QImage must be 32 bit for this to work! (see QImage and QImage::scanLine Qt docs).
    
   // first clear the arrays.
   for( int l = 0; l < 256; l++ )
   {
        lumins[l] = 0;
        reds[l] = 0;
        greens[l] = 0;
        blues[l] = 0;
   }
   numpixels = 0;
   meanRed = 0; meanGreen = 0; meanBlue = 0;
   medianRed = 0; medianGreen = 0; medianBlue = 0;
   
   QString conv; 

   // scan the image in the usual Qt way, and populate the luminosity/color level arrays.
   uchar* scanLine;

  //WRONG! int medianpixel = int ( theimage->height() * theimage->width() / 2.0 ); 
   
   for( int y = 0; y < theimage->height(); y++ )
   {   
       scanLine = theimage->scanLine( y );
       for( int x = 0; x < theimage->width(); x++ )
       {
            QRgb *rgb = ( (QRgb*)scanLine + x ); // (must do this cast, see QImage docs).
            // (TODO only do calcs for current level, save some cpu?)
            int lumlevel = int( 0.3*qRed(*rgb) + 0.59*qGreen(*rgb) + 0.11*qBlue(*rgb) ); 
            // increment the counter for this luminance found for this pixel.
            if ( lumlevel < 0 || lumlevel > 255 )
                debug("luminosity level out of range");
            lumins[ lumlevel ]++; 
            reds[ qRed(*rgb) ]++;
            greens[ qGreen(*rgb) ]++;
            blues[ qBlue(*rgb) ]++;

            numpixels++;
        /*WRONG!    if ( numpixels == medianpixel )
            {   medianLumin = lumlevel;
                medianRed = qRed(*rgb);
                medianGreen = qGreen(*rgb);
                medianBlue = qRed(*rgb);
            }
            */
        }
  }
   // Now calc the stats from the final arrays.
    for( int l = 0; l < 256; l++ )
    {   if ( l == 0 ) 
        {
            maxLumin = lumins[l];
            maxRed = reds[l];
            maxGreen = greens[l];
            maxBlue = blues[l];
        }
        if ( lumins[l] > maxLumin ) maxLumin = lumins[l];
        if ( reds[l] > maxRed ) maxRed = reds[l];
        if ( greens[l] > maxGreen ) maxGreen = greens[l];
        if ( blues[l] > maxBlue ) maxBlue = blues[l];

        meanLumin+= lumins[l];
        meanRed+= reds[l];
        meanGreen+= greens[l];
        meanBlue+= blues[l];

    }
    
    meanLumin/=256.0; meanRed/=256.0; meanGreen/=256.0; meanBlue/=256.0;

    return 1;
}

int LevelsPlot::columnHeight( Level lev, int x )
{
    int colheight = 0;
    int full = height();
    int maxviewable;
    if ( lev == Luminosity )
    {   if ( maxLumin < height() )
            maxviewable = height();
        else maxviewable = maxLumin;
    }
    else if ( lev == Reds )
    {   if ( maxRed < height() )
            maxviewable = height();
        else maxviewable = maxRed;
    }
    else if ( lev == Greens )
    {   if ( maxGreen < height() )
            maxviewable = height();
        else maxviewable = maxGreen;
    }
    else if ( lev == Blues )
    {   if ( maxBlue < height() )
            maxviewable = height();
        else maxviewable = maxBlue;
    }
    else if ( lev == RGBStack || lev == SplitH )
    {   
        if ( maxRed < height() )
            maxviewable = height();
        else maxviewable = maxRed; // could do better...
    }
    else if ( lev == SplitV )
    {   maxviewable = width();
        full = width(); // since bars are other way round.
    }
   
  
    if ( lev == Luminosity ) colheight = int( full * lumins[x] / maxviewable );
    else if ( lev == Reds ) colheight = int( full * reds[x] / maxviewable ); 
    else if ( lev == Greens ) colheight = int( full * greens[x] / maxviewable );
    else if ( lev == Blues ) colheight = int( full * blues[x] / maxviewable );
    return colheight;
}

QColor LevelsPlot::penColor()
{
    QColor penccol = QColor( Qt::yellow );
    if ( thelevel == Luminosity )
        penccol = QColor( Qt::white );
    else if ( thelevel == Reds )
        penccol = QColor( Qt::red );
    else if ( thelevel == Greens )
        penccol = QColor( Qt::green );
    else if ( thelevel == Blues )
        penccol = QColor( Qt::blue );

    return penccol;
}

void LevelsPlot::resizeEvent(QResizeEvent *)
{
    mustGenerateBackground = true;
    //repaint();
}

QSize LevelsPlot::sizeHint() const
{
    return QSize(256, 50);
}

