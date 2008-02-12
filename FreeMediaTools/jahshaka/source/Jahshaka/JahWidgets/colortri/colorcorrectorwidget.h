/****************************************************************************
** Form interface generated from reading ui file 'colorcorrectorwidget.ui'
**
** Created: Mon 1. Aug 14:46:00 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef COLORCORRECTORWIDGET_H
#define COLORCORRECTORWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QtColorTriangle;
class QButtonGroup;
class QRadioButton;
class QPushButton;
class QSlider;
class QLabel;
class QFrame;

class ColorCorrectorWidget : public QWidget
{
    Q_OBJECT

public:
    ColorCorrectorWidget( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~ColorCorrectorWidget();

    QButtonGroup* buttonGroup1;
    QRadioButton* radioButtonMaster;
    QRadioButton* radioButtonShadows;
    QRadioButton* radioButtonMidtones;
    QRadioButton* radioButtonHighlights;
    QtColorTriangle* qtColorTriangle;
    QPushButton* resetButton;
    QSlider* redSlider;
    QLabel* textLabel2_5;
    QSlider* blueSlider;
    QLabel* textLabel1_4;
    QLabel* textLabel1_2_2;
    QLabel* textLabel1_2_3;
    QLabel* textLabel1_2;
    QLabel* textLabel1_3;
    QSlider* hSlider;
    QFrame* line1;
    QSlider* vSlider;
    QSlider* brightnessSlider;
    QSlider* sSlider;
    QLabel* textLabel2_4;
    QSlider* tintSlider;
    QSlider* strengthSlider;
    QLabel* textLabel2_3_3;
    QSlider* greenSlider;
    QLabel* textLabel2_3_2;
    QSlider* gammaSlider;
    QLabel* gammaLabel;

public slots:
    virtual void colorTriangleChanged( const QColor & col );
    virtual void setColor( QColor & col );
    virtual void resetSliders();
    virtual void setRGB( const QColor & col );

protected:
    QGridLayout* ColorCorrectorWidgetLayout;
    QVBoxLayout* buttonGroup1Layout;
    QHBoxLayout* layout3;
    QSpacerItem* spacer10;
    QGridLayout* layout5;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;

    void init();

};

#endif // COLORCORRECTORWIDGET_H
