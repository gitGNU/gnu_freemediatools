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
#include <qstylefactory.h>
// This is the ui file for the keyer color chooser widget. The main functions for normal use are
// setColor() to set the base color, and minColor()/maxColor() which return the max and min colors
// based on the current slider settings.

void KeyerColorWidget::init()
{
    connect ( qtColorTriangle, SIGNAL( colorChanged(const QColor &) ), this, SLOT ( colorTriangleChanged( const QColor & )));
    connect ( qtColorTriangle, SIGNAL( colorChanged(const QColor &) ), this, SLOT ( setColorBarMin( const QColor & )));
    connect ( qtColorTriangle, SIGNAL( colorChanged(const QColor &) ), this, SLOT ( setColorBarMax( const QColor & )));
 
   // make sure the min and max bars start the right color.
    setColorBarMin( qtColorTriangle->color() );
    setColorBarMax( qtColorTriangle->color() );

    // make sure the spin boxes start at the right values.
    QColor col;
    int h = 0;
    int s = 134;
    int v = 180;
    col.setHsv( h, s, v );
    setColor( col );

}

void KeyerColorWidget::colorTriangleChanged( const QColor &col )
{
    //qDebug( "in colorTriangleChanged\n ") ;
    int h, s, v;
    col.getHsv( h, s, v );
    spinBoxH->setValue( h );
    spinBoxS->setValue( s );
    spinBoxV->setValue( v );
   
}

void KeyerColorWidget::buttonPressRGB()
{
    mainWidgetStack->raiseWidget( 1 );
}

void KeyerColorWidget::setHValueSpin( int value )
{
    spinBoxH->setValue( value );
}

void KeyerColorWidget::setSValueSpin( int value )
{
    spinBoxS->setValue( value );
}

void KeyerColorWidget::setLValueSpin( int value )
{
    spinBoxS->setValue( value );
}

void KeyerColorWidget::buttonPressHSL()
{
      mainWidgetStack->raiseWidget( 0 );
}

void KeyerColorWidget::setColorBarMin( const QColor &col )
{
    mincol = col;
    int h, s, v;
    int newh, news, newv;
    col.getHsv( h, s, v );
    // Prevent invalid HSV values
    newh = h - sliderH->value();
    news = s - sliderS->value();
    newv = v - sliderV->value();
    if ( newh < 0 ) newh = 0;
    if ( news < 0 ) news = 0;
    if ( newv < 0 ) newv = 0;
   
    mincol.setHsv( newh, news, newv );
    colorBarLabelMin->setPaletteBackgroundColor( mincol );
    if ( newv > 128 )
        colorBarLabelMin->setPaletteForegroundColor( Qt::black );
    else
        colorBarLabelMin->setPaletteForegroundColor( Qt::white );

    QString tips;
    QString conv1, conv2, conv3;
    conv1.setNum(newh); conv2.setNum(news); conv3.setNum(newv);
    tips = "HSV: " + conv1 + " " + conv2 + " " + conv3;
    QToolTip::remove(colorBarLabelMin);
    QToolTip::add(colorBarLabelMin, tips);
}

void KeyerColorWidget::setColorBarMax( const QColor & col )
{
    maxcol = col;
    int h, s, v;
    int newh, news, newv;
    col.getHsv( h, s, v );
     // Prevent invalid HSV values
    newh = h + sliderH->value();
    news = s + sliderS->value();
    newv = v + sliderV->value();
    if ( newh > 359 ) newh = 359;
    if ( news > 255 ) news = 255;
    if ( newv > 255 ) newv = 255;
    
    maxcol.setHsv( newh, news, newv );
    colorBarLabelMax->setPaletteBackgroundColor( maxcol );
    if ( newv > 128 )
        colorBarLabelMax->setPaletteForegroundColor( Qt::black );
    else
        colorBarLabelMax->setPaletteForegroundColor( Qt::white );

    QString tips;
    QString conv1, conv2, conv3;
    conv1.setNum(newh); conv2.setNum(news); conv3.setNum(newv);
    tips = "HSV: " + conv1 + " " + conv2 + " " + conv3;
    QToolTip::remove(colorBarLabelMax);
    QToolTip::add(colorBarLabelMax, tips);
}

void KeyerColorWidget::softenSliderValueChanged( int )
{
}

void KeyerColorWidget::sliderValueChanged( int )
{
    // (we dont care about the value sent, all sliders are connected to here)
    setColorBarMin( qtColorTriangle->color() );
    setColorBarMax( qtColorTriangle->color() );
}

// Use this function to get the min colour
QColor KeyerColorWidget::minColor()
{
    return colorBarLabelMin->paletteBackgroundColor();
}

// Use this function to get the max colour
QColor KeyerColorWidget::maxColor()
{
    return colorBarLabelMax->paletteBackgroundColor();
}


void KeyerColorWidget::spinBoxChanged( int )
{
    // (we dont care about the value sent, all spinboxes are connected to here)
    QColor col; 
    col.setHsv( spinBoxH->value(), spinBoxS->value(), spinBoxV->value() );

    // Special function to ensure H constant. 
    qtColorTriangle->setColorSilentlyConstantHue( col ); // (silently, no signals)


}

void KeyerColorWidget::setColor( QColor & col )
{
    qtColorTriangle->setColor( col );   
}
