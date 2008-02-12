/****************************************************************************
** $Id: charcoal.cpp,v 1.1 2005/06/15 11:24:22 jahshaka Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "charcoal.h"

#ifndef QT_NO_STYLE_WINDOWS

/////////////////////////////////////////////////////////
#include "charcoal.xpm"

////////////////////////////////////////////////////////
// charcoal style here

//CharcoalStyle::CharcoalStyle() : QPlatinumStyle() { }
CharcoalStyle::CharcoalStyle() : QWindowsStyle() { }

/*!
  Reimplementation from QStyle
 */
void CharcoalStyle::polish( QApplication *app)
{
    oldPalette = app->palette();

    // we simply create a nice QColorGroup with a couple of fancy
    // pixmaps here and apply to it all widgets
    QPixmap background(charcoal_xpm);
    
    QColor blacker ("#505050505050"); //same as topgrad below
    
    QPixmap button( 1, 1 ); button.fill( blacker );
    QPixmap dark( 1, 1 ); dark.fill( blacker );
    QPixmap mid( 1, 1 ); mid.fill( blacker );
    QPixmap light( 1, 1 ); light.fill( blacker );

    QPalette op = app->palette();

    QColor backCol( 50,50,50 );
    //QColor backCol( 0,0,0 );

    QColorGroup active (op.active().foreground(),
                        QBrush(op.active().button(),button),
                        QBrush(op.active().light(), light),
                        QBrush(op.active().dark(), dark),
                        QBrush(op.active().mid(), mid),
                        op.active().text(), Qt::white,  //as white
                        op.active().base(),//            QColor(236,182,120),
                        QBrush(backCol, background) );
    //active.setColor( QColorGroup::ButtonText,  Qt::white  );
    active.setColor( QColorGroup::Light,     Qt::black  );
    active.setColor( QColorGroup::Midlight,  Qt::black  );
    active.setColor( QColorGroup::Dark,      Qt::black  );
    active.setColor( QColorGroup::Mid,       Qt::black  );
    active.setColor( QColorGroup::Shadow,    Qt::black  );

    QColorGroup disabled   (op.disabled().foreground(),
                            QBrush(op.disabled().button(),button),
                            QBrush(op.disabled().light(), light),
                            op.disabled().dark(),
                            QBrush(op.disabled().mid(), mid),
                            op.disabled().text(), Qt::white,
                            op.disabled().base(),//          QColor(236,182,120),
                            QBrush(backCol, background) );
    //disabled.setColor( QColorGroup::ButtonText,  Qt::white  );
    //disabled.setColor( QColorGroup::Shadow,  Qt::black  );
    disabled.setColor( QColorGroup::Light,     Qt::black  );
    disabled.setColor( QColorGroup::Midlight,  Qt::black  );
    disabled.setColor( QColorGroup::Dark,      Qt::black  );
    disabled.setColor( QColorGroup::Mid,       Qt::black  );
    disabled.setColor( QColorGroup::Shadow,    Qt::black  );

    QPalette newPalette( active, disabled, active );
    app->setPalette( newPalette, TRUE );
}

/*!
  Reimplementation from QStyle
 */
void CharcoalStyle::unPolish( QApplication *app)
{
    app->setPalette(oldPalette, TRUE);
    app->setFont( app->font(), TRUE );
}

/*!
  Reimplementation from QStyle
 */
void CharcoalStyle::polish( QWidget* w)
{

   // the polish function sets some widgets to transparent mode and
    // some to translate background mode in order to get the full
    // benefit from the nice pixmaps in the color group.

    if (w->inherits("QPushButton")){
    w->setBackgroundMode( QWidget::NoBackground );
    return;
    }

    if ( !w->isTopLevel() ) {
    if ( w->backgroundPixmap() )
        w->setBackgroundOrigin( QWidget::WindowOrigin );
    }
}

void CharcoalStyle::unPolish( QWidget* w)
{

   // the polish function sets some widgets to transparent mode and
    // some to translate background mode in order to get the full
    // benefit from the nice pixmaps in the color group.

    if (w->inherits("QPushButton")){
    w->setBackgroundMode( QWidget::PaletteButton );
    return;
    }
    if ( !w->isTopLevel() ) {
    if ( w->backgroundPixmap() )
        w->setBackgroundOrigin( QWidget::WidgetOrigin );
    }

}

#endif
