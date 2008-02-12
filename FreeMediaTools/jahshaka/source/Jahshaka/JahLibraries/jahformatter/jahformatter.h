
/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHFORMATTER_H
#define JAHFORMATTER_H

#include <qstring.h>
#include <qpalette.h>

class QCheckBox;
class QSlider;
class QLabel;
class QDial;
class QString;
class QPushButton;
class QObject;
class QStyleFactory;
class QToolButton;
class QFrame;
class QLayout;
class QBoxLayout;
class QListView;
class QWidget;

class InputLCD;
class JahToolButton;
class SupergRangeControl;

class JahFormatter 
{

public:
    JahFormatter();
    
    ~JahFormatter();
    

    static void addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
                                    int w, int h, QString icon );
    
    static void addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
                                    int w, int h, QString iconOff, QString iconOn );

    static void addJahPlayerButton( JahToolButton * button, const QString &iconOff, const QString &iconOn );
    
    static void addJahPlayerButton( JahToolButton * button, QString thetext, int x1, int y1, int x2, int y2,
                                    int w, int h );
    
    static void addButton( QPushButton * button, int x, int y, int w, int h, QString text );
    static void addCheckButton( QCheckBox * button, int x, int y, int w, int h, QString text );
    static void addToolButton( QToolButton * button, int x, int y, int w, int h, QString icon );
    static void addToolButton( QToolButton * button, int x, int y, int w, int h  );
    static void addIconButton( QPushButton * button, int x, int y, int w, int h , QString icon );
    static void addIconButton( QPushButton * button, int x, int y, int w, int h  );

    static void addSlider( QSlider * slider, int x, int y, int w, int h, int range);
    static void addRangeSlider( QSlider * slider, int x, int y, int w, int h, int rangemin, int rangemax);
    static void addSliderLcd( InputLCD * lcd );
    static void addSliderLcd( InputLCD * lcd, int x, int y, int w, int h, int range);
    static void addLabel( QLabel * label, int x, int y, int w, int h, QString text, int size);

    static void addLcdLabel( QFrame * frame,
                             InputLCD * lcd, int x, int y, int w, int h, int range,
                             QString labeltext, int size );
    
    static void addLabelLcd( QFrame * frame,
                             InputLCD * lcd, int x, int y, int w, int h, int range,
                             QString labeltext, int size );

    static void addSliderAndLcd( QSlider * slider, int x, int y, int w, int h, int range,
                                 InputLCD * lcd );

    static void addSliderAndLcd( QSlider * slider, int x, int y, int w, int h, int rangemin, int rangemax,
                                 InputLCD * lcd );

    static void addSliderLcdLabel( QFrame * frame,
                                   QSlider  * slider, int x, int y, int w, int h, int range,
                                   InputLCD * lcd,
                                   QString labeltext, int size );
    
    static void addSliderLcdLabel( QFrame * frame,
                                   QSlider  * slider, int x, int y, int w, int h,
                                   int rangemin, int rangemax,
                                   InputLCD * lcd,
                                   QString labeltext, int size );
    
    static void addSliderLcdLabel( QFrame * frame,
                                   QSlider  * slider, int x, int y, int w, int h,
                                   int rangemin, int rangemax, int preset,
                                   InputLCD * lcd,
                                   QString labeltext, int size );
    
    static void addDialLcdLabel  ( QFrame * frame,
                                   QDial  * slider, int x, int y, int w, int h, int range,
                                   InputLCD * lcd,
                                   QString labeltext, int size );
    
    //new routines
    static void addJahSliderGroup( QFrame * frame,
                                   QSlider  * slider, int x, int y, int w, int h, int range,
                                   InputLCD * lcd,
                                   QLabel * label,QString labeltext, int size );
    
    static void addJahSliderGroup( QFrame * frame,
                                   QSlider  * slider, int x, int y, int w, int h, int rangemin, int rangemax,
                                   InputLCD * lcd,
                                   QLabel * label,QString labeltext, int size );
    
    
    static void configure( SupergRangeControl* c, int def, int min = -(INT_MAX - 1) , int max = (INT_MAX - 1) );

    static void setMarginAndSpacing( QLayout* l );
    static void setMarginAndSpacingSmall( QLayout* l );
    static void setMarginAndSpacingLarge( QLayout* l );

    static void setMargin( QLayout* l );
    static void setMarginSmall( QLayout* l );
    static void setMarginLarge( QLayout* l );

    static void setSpacing( QLayout* l );
    static void setSpacingSmall( QLayout* l );
    static void setSpacingLarge( QLayout* l );

    static void addSpacing( QBoxLayout* );
    static void addSpacingSmall( QBoxLayout* );

    static void setListViewAsSingleColumn( QListView* );
    static void setListViewAsDualColumn( QListView* );

    static QWidget* createSeparator( QWidget* parent, bool vertical = true );

    static void swap( QPalette& p, QColorGroup::ColorRole a, QColorGroup::ColorRole b );

};


#endif






