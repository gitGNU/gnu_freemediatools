/****************************************************************************
** $Id: themes.cpp,v 1.7 2006/03/29 08:35:44 timdewhirst Exp $
**
** Copyright (C) 1992-2000 Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "themes.h"

#ifndef QT_NO_STYLE_WINDOWS
#include "wood.h"
#include "marble.h"
#include "brushmetal.h"
#include "charcoal.h"
#endif

#include "supergtheme.h"

// qt
#include <qpalette.h>
#include <qapplication.h>

JahThemes::JahThemes( QObject *parent, const char *name )
    : QObject( parent, name )
{
    appFont = QApplication::font();
}

void JahThemes::makeTheme(const QString &style)
{
    if(style == "SuperG" )
    {
        qApp->setStyle( new SuperGStyle );
        qApp->setFont( QFont( "helvetica", appFont.pointSize()+0 ), TRUE ); // SG ok?
    }
    if(style == "High Tech") 
    {
#ifndef QT_NO_STYLE_WINDOWS
        qApp->setStyle( new JahHighTech );
#endif
    } 
    else if( style == "Marble" ) 
    {
#ifndef QT_NO_STYLE_WINDOWS
        qApp->setStyle( new JahMetalStyle );
#endif
    } 
    else if( style == "Brush Metal" ) 
    {
#ifndef QT_NO_STYLE_WINDOWS
        qApp->setStyle( new MetalStyle );
#endif
    } 
    else if( style == "Charcoal" ) 
    {
#ifndef QT_NO_STYLE_WINDOWS
        qApp->setStyle( new CharcoalStyle );
#endif
    } 
    else 
    {
        qApp->setStyle(style);
        if(style == "Platinum") 
        {
            QPalette p( QColor( 239, 239, 239 ) );
            qApp->setPalette( p, TRUE );
            qApp->setFont( appFont, TRUE );
        } 
        else if(style == "Windows") 
        {
            qApp->setFont( appFont, TRUE );
        } 
        else if(style == "CDE") 
        {
            QPalette p( QColor( 75, 123, 130 ) );
            p.setColor( QPalette::Active, QColorGroup::Base, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Inactive, QColorGroup::Base, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Disabled, QColorGroup::Base, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Active, QColorGroup::Highlight, Qt::white );
            p.setColor( QPalette::Active, QColorGroup::HighlightedText, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Inactive, QColorGroup::Highlight, Qt::white );
            p.setColor( QPalette::Inactive, QColorGroup::HighlightedText, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Disabled, QColorGroup::Highlight, Qt::white );
            p.setColor( QPalette::Disabled, QColorGroup::HighlightedText, QColor( 55, 77, 78 ) );
            p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::white );
            p.setColor( QPalette::Active, QColorGroup::Text, Qt::white );
            p.setColor( QPalette::Active, QColorGroup::ButtonText, Qt::white );
            p.setColor( QPalette::Inactive, QColorGroup::Foreground, Qt::white );
            p.setColor( QPalette::Inactive, QColorGroup::Text, Qt::white );
            p.setColor( QPalette::Inactive, QColorGroup::ButtonText, Qt::white );
            p.setColor( QPalette::Disabled, QColorGroup::Foreground, Qt::lightGray );
            p.setColor( QPalette::Disabled, QColorGroup::Text, Qt::lightGray );
            p.setColor( QPalette::Disabled, QColorGroup::ButtonText, Qt::lightGray );
            qApp->setPalette( p, TRUE );
            qApp->setFont( QFont( "times", appFont.pointSize() ), TRUE );
        } 
        else if(style == "Motif" || style == "MotifPlus") 
        {
            QPalette p( QColor( 192, 192, 192 ) );
            qApp->setPalette( p, TRUE );
            qApp->setFont( appFont, TRUE );
        }
    }
}
