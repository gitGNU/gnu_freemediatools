/****************************************************************************
**
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
** Parts of this file modified  from 'themes' example file, (c) Trolltech AS.
** Original copyright notice states that:
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
** Acknowledgements:
** ================
**
** This style contains elements taken from designs submitted to the 
** Jahshaka UI contest, in particular the SuperG design by Ron7.
** This is only a partial implementation, to be built upon later. 
** 
**     -- many thanks for all the great contributions, expect to see 
**        more of your ideas in future updates.
**
*****************************************************************************/

#ifndef SUPERGTHEME_H
#define SUPERGTHEME_H


#include <qpalette.h>
#include <qpixmap.h>

#include <qpopupmenu.h>
#include <qcommonstyle.h>

/*  A custom style for v2. It derives from Windows style for no particular reason, the other styles in Jahshaka
    do the same. Qt docs recommend either deriving from WindowsStyle
    or MotifStyle, since the others may or may not be configured as plugins.
*/
class SuperGStyle : public QCommonStyle
{

public:
    SuperGStyle();
    void polish( QApplication* );
    void polish( QWidget* );
    void unPolish( QApplication* );
    void polishPopupMenu( QPopupMenu* p);

    void drawPrimitive( PrimitiveElement pe,
			QPainter *p,
			const QRect &r,
			const QColorGroup &cg,
			SFlags flags = Style_Default,
			const QStyleOption& = QStyleOption::Default) const;

    void drawControl( ControlElement element,
		      QPainter *p,
		      const QWidget *widget,
		      const QRect &r,
		      const QColorGroup &cg,
		      SFlags how = Style_Default,
		      const QStyleOption& = QStyleOption::Default ) const;

    QRect querySubControlMetrics( ComplexControl control,
                                  const QWidget *widget,
                                  SubControl sc,
                                  const QStyleOption &opt ) const;

    void drawComplexControl( ComplexControl cc,
			     QPainter *p,
			     const QWidget *widget,
			     const QRect &r,
			     const QColorGroup &cg,
			     SFlags how = Style_Default,
			     SCFlags sub = SC_All,
			     SCFlags subActive = SC_None,
			     const QStyleOption& = QStyleOption::Default ) const;

    virtual int styleHint( StyleHint sh, const QWidget* w, const QStyleOption&, QStyleHintReturn* ) const;
    
    virtual int pixelMetric( PixelMetric, const QWidget * ) const;
    
    virtual QSize sizeFromContents ( ContentsType contents, 
                                     const QWidget* widget, 
                                     const QSize & contentsSize,
                                     const QStyleOption & opt = QStyleOption::Default ) const;

private:
    void drawStyledFrame(  QPainter *p, int x, int y, int w, int h ) const;
    void drawStyledGradient( QPainter *p, int x, int y, int w, int h,
			  bool sunken, bool horz, bool flat=FALSE ) const;
    
    // A bluey-green sunken bg gradient, e.g. for scroll bars, sliders etc
    void drawStyledCoolGradient( QPainter *p, int x, int y, int w, int h, bool horz  ) const;
    
    void drawStyledButton( QPainter *p, int x, int y, int w, int h,
			  bool sunken, bool horz, bool flat=FALSE ) const;

    void drawSuperGBox( QPainter *p, const QRect &r, const QColorGroup &cg, bool highlight = true, 
                    bool fill = false, bool invertcolor = false, bool ison = false, bool isflat = false ) const;

    //! Handle for slider primarily.
    // TODO: vertical orientation...
    void drawStyledHandle( QPainter *p, int x, int y, int w, int h,
			   bool horz ) const;

    QPalette oldPalette;
    
    mutable QRect m_menuItem;
};

#endif
