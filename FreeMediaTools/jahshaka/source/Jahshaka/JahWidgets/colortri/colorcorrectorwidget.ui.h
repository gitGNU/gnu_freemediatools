/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

// This is the ui file for the color corrector widget. To obtain the settings of the corrector wheel, 
// you can either query the sliders, or (better?) query colortriangle ('qtColorTriangle') directly using the 
// fns correctorSaturation, correctorHueOffset (the angle), correctorHueStrength (vector length) and 
// correctorHueShift (angle of outer donut rotation).

#include <qstylefactory.h>
#include "colortriangle.h"
#include "qtcolordisplay.h"


void ColorCorrectorWidget::init()
{
   qtColorTriangle->triangle->setMode( ColorTriangle::ColorCorrectorMode );
   qtColorTriangle->display->hide(); // hide the chosen color display

   // call resetSliders in case the ui file doesnt have right defaults:
    resetSliders();

    // make sure the spin boxes start at the right values.
    QColor col;
    int h = 0;
    int s = 134;
    int v = 180;
    col.setHsv( h, s, v );
    setColor( col );
      
    /* Not used now. The updates are done outside of this ui file.
    // Need to move the selector when the hue offset and strength sliders are used.
    connect(  hueOffsetSlider, SIGNAL( valueChanged( int )), qtColorTriangle->triangle, SLOT (setCorrectorHueOffset(int )));
    connect(  strengthSlider, SIGNAL( valueChanged( int )), qtColorTriangle->triangle, SLOT (setCorrectorHueStrength(int )));
    
    // Tell our sliders to update when the user interacts with the color wheel itself
    connect ( qtColorTriangle->triangle, SIGNAL( correctorHueStrengthChanged( int )), strengthSlider, SLOT( setValue( int ))); 
    connect ( qtColorTriangle->triangle, SIGNAL( correctorHueOffsetChanged( int )), hueOffsetSlider, SLOT( setValue( int ))); 
    */
}

void ColorCorrectorWidget::colorTriangleChanged( const QColor &col )
{
    int h, s, v;
    col.getHsv( h, s, v );
// ... TODO? Slot not used for color corrector.
   
}


void ColorCorrectorWidget::setColor( QColor & col )
{
    qtColorTriangle->setColor( col );   
}


void ColorCorrectorWidget::resetSliders()
{
    // This code forces the nodes to reset even if the sliders haven't changed
    // This is necessary because they aren't the only sliders controlling the colors
    redSlider->setValue ( 1 );
    greenSlider->setValue ( 1 );
    blueSlider->setValue ( 1 );
    
    brightnessSlider->setValue( 1 );
    tintSlider->setValue( 1 );
    strengthSlider->setValue( 1 );
    
    hSlider->setValue( 1 );
    sSlider->setValue( 1 );
    vSlider->setValue ( 1 );

    redSlider->setValue ( 0 );  
    greenSlider->setValue ( 0 );
    blueSlider->setValue ( 0 );
    
    brightnessSlider->setValue( 0 );
    tintSlider->setValue( 0 );
    strengthSlider->setValue( 0 );
    
    hSlider->setValue( 0 );
    sSlider->setValue( 0 );
    vSlider->setValue ( 0 );

    // Until hue shift hooked up, reset the wheel directly:
    qtColorTriangle->triangle->setCorrectorHueShift( 0 );
    qtColorTriangle->triangle->update();
}


void ColorCorrectorWidget::setRGB( const QColor &col )
{
    int h, s, v;
    QColor tcol = col;
    tcol.getHsv ( h, s, v);
    redSlider->setValue( tcol.red() * 4 - 500  );
    greenSlider->setValue( tcol.green() * 4 - 500  );
    blueSlider->setValue( tcol.blue() * 4 - 500 );
}
