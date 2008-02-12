/****************************************************************************
** Form interface generated from reading ui file 'keyercolorwidget.ui'
**
** Created: Wed 7. Sep 11:57:36 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef KEYERCOLORWIDGET_H
#define KEYERCOLORWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QtColorTriangle;
class ColorDropper;
class QPushButton;
class QWidgetStack;
class QSlider;
class QLabel;
class QCheckBox;
class QSpinBox;

class KeyerColorWidget : public QWidget
{
    Q_OBJECT

public:
    KeyerColorWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~KeyerColorWidget();

    QPushButton* pushButtonHSL;
    QWidgetStack* mainWidgetStack;
    QWidget* WStackPage;
    QSlider* sliderV;
    QLabel* colorBarLabelMin;
    QSlider* sliderH;
    QSlider* sliderS;
    QLabel* colorBarLabelMax;
    QSlider* softenSlider;
    QCheckBox* showAlphaCheckBox;
    QCheckBox* selectColorsIndividuallyCheckBox;
    QLabel* textLabel5_2;
    QLabel* textLabel5_3;
    QtColorTriangle* qtColorTriangle;
    QLabel* textLabel5;
    QSpinBox* spinBoxV;
    QLabel* textLabelH;
    QLabel* textLabelS;
    QSpinBox* spinBoxS;
    QSpinBox* spinBoxH;
    QLabel* textLabelV;
    QWidget* WStackPage_2;
    QLabel* textLabel4;
    QPushButton* pushButtonRGB;
    QPushButton* colorDropper;

    QColor maxcol;
    QColor mincol;

public slots:
    virtual void colorTriangleChanged( const QColor & col );
    virtual void buttonPressRGB();
    virtual void setHValueSpin( int value );
    virtual void setSValueSpin( int value );
    virtual void setLValueSpin( int value );
    virtual void buttonPressHSL();
    virtual void setColorBarMin( const QColor & col );
    virtual void setColorBarMax( const QColor & col );
    virtual void sliderValueChanged( int value );
    virtual void softenSliderValueChanged( int value );
    virtual QColor minColor();
    virtual QColor maxColor();
    virtual void spinBoxChanged( int value );
    virtual void setColor( QColor & col );

protected:
    QGridLayout* KeyerColorWidgetLayout;
    QSpacerItem* spacer1;
    QGridLayout* WStackPageLayout;
    QGridLayout* layout5;
    QGridLayout* layout6;
    QGridLayout* layout1;
    QGridLayout* WStackPageLayout_2;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

    void init();

};

#endif // KEYERCOLORWIDGET_H
