/****************************************************************************
** $Id: charcoal.h,v 1.1 2005/06/15 11:24:22 jahshaka Exp $
**
** Definition of the Metal Style for the themes example
**
** Created : 979899
**
** Copyright (C) 1997 by Trolltech AS.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
****************************************************************************/
#ifndef CHARCOAL_H
#define CHARCOAL_H

#include <qapplication.h>
#include <qcombobox.h>
#include <qpainter.h>
#include <qdrawutil.h> // for now
#include <qpixmap.h> // for now
#include <qpalette.h> // for now
#include <qwidget.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qrangecontrol.h>
#include <qscrollbar.h>
#include <qslider.h>

#include <limits.h>

#include <qplatinumstyle.h>

#ifndef QT_NO_STYLE_WINDOWS

/////////////////////////////////////////////
//problems with dynamically loading the sgi here
//so we default to platinum...

//class CharcoalStyle : public QPlatinumStyle
class CharcoalStyle : public QWindowsStyle
{
public:
    CharcoalStyle();
    void polish( QApplication*);
    void unPolish( QApplication*);
    void polish( QWidget* );
    void unPolish( QWidget* );

private:

    QPalette oldPalette;
};

#endif

#endif
