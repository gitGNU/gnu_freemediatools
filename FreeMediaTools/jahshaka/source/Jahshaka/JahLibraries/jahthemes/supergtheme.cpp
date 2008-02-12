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

#include "supergtheme.h"

#include <qapplication.h>
#include <qcombobox.h>
#include <qpainter.h>
#include <qdrawutil.h>
#include <qpixmap.h>
#include <qpalette.h>
#include <qwidget.h>
#include <qlabel.h>
#include <qimage.h>
#include <qpushbutton.h>
#include <qwidget.h>
#include <qrangecontrol.h>
#include <qscrollbar.h>
#include <qslider.h>
#include <qtabbar.h>
#include <qlistview.h>
#include <qbitmap.h>
#include <qmenubar.h>
#include <limits.h>

#include <qcleanuphandler.h>

static const int windowsItemFrame		=  2; // menu item frame width
static const int windowsSepHeight		=  2; // separator item height
static const int windowsItemHMargin		=  3; // menu item hor text margin
static const int windowsItemVMargin		=  2; // menu item ver text margin
static const int windowsArrowHMargin		=  6; // arrow horizontal margin
static const int windowsTabSpacing		= 12; // space between text and tab
static const int windowsCheckMarkHMargin	=  2; // horiz. margins of check mark
static const int windowsRightBorder		= 12; // right border on windows
static const int windowsCheckMarkWidth		= 12; // checkmarks width on windows

SuperGStyle::SuperGStyle() 
    : QCommonStyle() 
{
    qApp->setFont( QFont( "verdana", qApp->font().pointSize()+0 ), TRUE );
}

/*!
  Reimplementation from QStyle
*/
void SuperGStyle::polish( QApplication *app)
{
    QCommonStyle::polish( app );

    oldPalette = app->palette();
    
    QPalette op = app->palette();
    
    // set basic colors here (brushes follow after).
    op.setColor(QColorGroup::Background, QColor( 67,67,67 ) );  //SG
    op.setColor(QColorGroup::Base, QColor( 71, 71, 71 ) ); // SG
    op.setColor(QColorGroup::Text, QColor( 228,228,228 )); // SG
    op.setColor(QColorGroup::HighlightedText, QColor( 38,38,39 )); // SG (ok?) 
    op.setColor(QColorGroup::ButtonText, QColor( 228,228,228 )); // SG
    op.setColor(QColorGroup::Highlight, QColor( 160,160,160 )); // SG (ok?)
    op.setColor(QColorGroup::Foreground, QColor( Qt::white )); // SG
    op.setColor(QColorGroup::Mid, QColor( 50, 50, 50)); // SG was 51, then 38. NB canvas uses it
    op.setColor(QColorGroup::Midlight, QColor( 85, 85, 85)); // SG (ok?)
    op.setColor(QColorGroup::Button, QColor( 67, 67, 67 )); // SG 
    op.setColor(QColorGroup::Dark, QColor( 10, 10, 10)); // SG
    op.setColor(QColorGroup::Light, QColor( 122, 122, 122)); // SG
    op.setColor(QColorGroup::Shadow, QColor( 38, 38, 38) ); // SG (ok?)
    
    QColor backCol( op.active().background() ); // was( 15,15,15 );

    QColorGroup active( op.active().foreground(),
                        QBrush( op.active().button() ),
                        QBrush( op.active().light() ),
                        QBrush( op.active().dark() ),
                        QBrush( op.active().mid() ),
                        op.active().text(),
                        Qt::white,
                        op.active().base(),
                        QBrush( backCol )
        );

    active.setColor( QColorGroup::ButtonText, op.active().buttonText()  );  //?buttontext ok?
    active.setColor( QColorGroup::Shadow, op.active().shadow()  );
    active.setColor( QColorGroup::Highlight, op.active().highlight()  );
    active.setColor( QColorGroup::Midlight, op.active().midlight()  );

    QColorGroup disabled ( op.disabled().foreground(),
                           QBrush( op.disabled().button() ),
                           // QBrush( QColor( 0, 255, 0 ) ),
                           QBrush( op.disabled().light() ),
                           op.disabled().dark(),
                           QBrush( op.disabled().mid() ),
                           QColor( "gray50" ),   // was op.disabled().text(). deal with this better?
                           Qt::white,
                           op.disabled().base(),
                           QBrush( backCol )
        );

    disabled.setColor( QColorGroup::ButtonText, op.active().buttonText().dark( 115 )  );
    disabled.setColor( QColorGroup::Shadow,  op.active().shadow()  );
    disabled.setColor( QColorGroup::Highlight,  op.active().highlight()  );
    
    QPalette newPalette( active, disabled, active );
    app->setPalette( newPalette, TRUE );
}

void SuperGStyle::polish( QWidget* w )
{
    QCommonStyle::polish( w );

    if ( w->inherits( "SupergRangeControl" ) && ( w->palette() == qApp->palette() ) )
    {
        // flip the text and background colours
        QPalette p = w->palette();
        QColorGroup active = p.active();
        QColorGroup inactive = p.inactive();
        QColorGroup disabled = p.disabled();

        active.setColor( QColorGroup::Background, active.light() );
        inactive.setColor( QColorGroup::Background, inactive.light() );
        disabled.setColor( QColorGroup::Background, disabled.light() );
        active.setColor( QColorGroup::Text, active.shadow().dark() );
        inactive.setColor( QColorGroup::Text, inactive.shadow().dark() );
        disabled.setColor( QColorGroup::Text, disabled.shadow().dark() );


        w->setPalette( QPalette( active, disabled, inactive ), true );
    }
}

void SuperGStyle::polishPopupMenu( QPopupMenu* p)
{
#ifndef QT_NO_POPUPMENU
    if ( !p->testWState( WState_Polished ) )
        p->setCheckable( TRUE );
#endif
}

/*!
  Reimplementation from QStyle
*/
void SuperGStyle::unPolish( QApplication *app)
{
    app->setPalette(oldPalette, TRUE);
    app->setFont( app->font(), TRUE );
}

void SuperGStyle::drawPrimitive( PrimitiveElement pe,
                                 QPainter *p,
                                 const QRect &r,
                                 const QColorGroup &cg,
                                 SFlags flags, const QStyleOption& opt ) const
{
    switch( pe ) {
    case PE_ScrollBarSubLine:
        if (flags & Style_Down) {
            p->setPen( cg.dark() );
            p->setBrush( cg.brush( QColorGroup::Button ) );
            p->drawRect( r );
        } else
            drawPrimitive(PE_ButtonBevel, p, r, cg, flags | Style_Raised);

	drawPrimitive(((flags & Style_Horizontal) ? PE_ArrowLeft : PE_ArrowUp),
		      p, r, cg, flags);
	break;

    case PE_ScrollBarAddLine:
        if (flags & Style_Down) {
            p->setPen( cg.dark() );
            p->setBrush( cg.brush( QColorGroup::Button ) );
            p->drawRect( r );
        } else
            drawPrimitive(PE_ButtonBevel, p, r, cg, flags | Style_Raised);

	drawPrimitive(((flags & Style_Horizontal) ? PE_ArrowRight : PE_ArrowDown),
		      p, r, cg, flags);
	break;

    case PE_ScrollBarAddPage:
    case PE_ScrollBarSubPage:
    {
        QBrush br;
        QColor c = p->backgroundColor();
        p->setPen( Qt::NoPen );
	p->setBackgroundMode(OpaqueMode);

        if (flags & Style_Down) {
            br = QBrush( cg.shadow() );
            p->setBackgroundColor( cg.dark() );
            p->setBrush( QBrush(cg.shadow(), Dense4Pattern) );
        } else {
            br = QBrush( c );
            p->setBrush(br);
        }

	p->drawRect( r );
        p->setBackgroundColor(c);

	p->setPen( QPen( cg.shadow(), 1 ) );      
	
	if ( flags & Style_Horizontal )
	{
	    p->drawLine( r.topLeft(), r.topRight() );
	    p->drawLine( r.bottomLeft(), r.bottomRight() );
	}
	else
	{
	    p->drawLine( r.topLeft(), r.bottomLeft() );
	    p->drawLine( r.topRight(), r.bottomRight() );
	}

        break;
    }

    case PE_ScrollBarSlider:
    {
        p->setPen(NoPen);
        p->setBrush( cg.mid() );
        p->drawRect( r );

	// draw some grooves
	p->setPen( cg.button() );

	static const int GROOVE_SPACING = 4;
	if ( flags & Style_Horizontal )
	{
	    const int center = r.width()/2 + r.left();
	    const int bottom = r.bottom() - 2;
	    const int top = r.top() + 2;
	    p->drawLine( center - GROOVE_SPACING, bottom, center - GROOVE_SPACING, top );
	    p->drawLine( center, bottom, center, top );
	    p->drawLine( center + GROOVE_SPACING, bottom, center + GROOVE_SPACING, top );
	}
	else
	{
	    const int center = r.bottom() - r.height()/2;
	    const int left = r.left() + 2;
	    const int right = r.right() - 2;
	    p->drawLine( left, center - GROOVE_SPACING, right, center - GROOVE_SPACING );
	    p->drawLine( left, center, right, center );
	    p->drawLine( left, center + GROOVE_SPACING, right, center + GROOVE_SPACING );
	}
	
	break;
    }

    case PE_HeaderSection: 
    case PE_ButtonBevel:
    case PE_ButtonCommand:
    case PE_ButtonTool:
    case PE_ButtonDropDown:
    case PE_ButtonDefault:
    {   
        p->fillRect( r, QBrush( cg.button() ) );
        if ( flags & QStyle::Style_Raised )
            drawSuperGBox( p, QRect( r.left(), r.top(), r.width(), r.height() ), cg, true );
        break;
    }

    case PE_PanelPopup:
    {   
        p->fillRect( r, QBrush( cg.button() ));
        p->setPen( cg.light() );
        p->setBrush( cg.button() );

        if ( m_menuItem.isNull() )
        {
            p->drawRoundRect ( r, 5, 5 );
        }
        else
        {
            const int CORNER_SIZE = 2;
            p->drawLine( r.topLeft(), QPoint( r.left(), r.bottom() - CORNER_SIZE ) );
            p->drawArc( r.left(), r.bottom() - CORNER_SIZE, CORNER_SIZE, CORNER_SIZE, 180, 90 );
            p->drawLine( QPoint( r.left() + CORNER_SIZE, r.bottom() ), 
                         QPoint( r.right() - CORNER_SIZE, r.bottom() ) );
            p->drawArc( r.right() - CORNER_SIZE, r.bottom(), CORNER_SIZE, CORNER_SIZE, 270, 90 );
            p->drawLine( QPoint( r.right(), r.bottom() - CORNER_SIZE ), 
                         QPoint( r.right(), r.top() ) );
            p->drawArc( r.right(), r.top() + CORNER_SIZE, CORNER_SIZE, CORNER_SIZE, 0, 90 );
            p->drawLine( QPoint( r.right() - CORNER_SIZE, r.top() ), 
                         QPoint( r.left() + m_menuItem.width(), r.top() ) );
        }
        break;
    }
    
    case PE_PanelMenuBar: 
    {   
        p->fillRect( r, QBrush( cg.button() ));
        p->setPen( cg.light() );
        p->setBrush( cg.button() );
        p->drawRoundRect ( r, 5, 5 );
        break;
    }

    case PE_Panel:
    case PE_PanelTabWidget:
    case PE_PanelLineEdit:
    case PE_PanelGroupBox:
    {
        p->fillRect( r, QBrush( cg.background() ) );
        p->setPen( cg.shadow() );
        p->setBrush( cg.background() );
        p->drawRect( r );
        break;
    }
    
    // This is a Super G tick for check boxes etc (not used in popupmenus though).
    case PE_Indicator:
    {
        p->setPen( cg.mid() );
        p->setBrush( cg.light() );
        p->drawRect( r.x(), r.y(), r.width(), r.height() );
        
        // Note, see the SGI style code for some checkmark hints.
        // Note, the lines are doubled up to make thicker.
        static const QCOORD nochange_mark[] = { 3,5, 9,5,  3,6, 9,6 };
        static const QCOORD check_mark[] = { 3,4,5,9,  5,9,9,1   ,  2,4,4,9,   4,9,8,1};
        
        if ( !(flags & Style_Off) ) 
        {
            int x1 = r.x();
            int y1 = r.y();
            if ( flags & Style_Down ) {
                x1++;
                y1++;
            }
            QPointArray amark;
            if ( flags & Style_On ) {
                amark = QPointArray( sizeof(check_mark)/(sizeof(QCOORD)*2),
                                     check_mark );
                // not great
                flags ^= Style_On;
                flags ^= Style_Down;
            } else if ( flags & Style_NoChange ) {
                amark = QPointArray( sizeof(nochange_mark)
                                     / (sizeof(QCOORD) * 2),
                                     nochange_mark );
            }
            
            amark.translate( x1 + 1, y1 + 1 ); // translate the tick when user clicks etc.
            
            // This does translated shadow ticks, changing pen
            p->setPen( cg.light().dark(115) );
            amark.translate( -1, 0 );
            p->drawLineSegments( amark );
            amark.translate( +2, +1 );
            p->drawLineSegments( amark );
            
            amark.translate( -1, -1 );
            p->setPen( cg.mid() ); 
            p->drawLineSegments( amark ); // main tick
            
        }
        
        break; 
    }
    
    case PE_Splitter:
    {
        p->setPen( cg.midlight() );
        p->setBrush( cg.button() );
        p->drawRect( r );
        p->setPen( cg.light() );
        
        if ( flags & Style_Horizontal )
            p->drawLine( r.x(), r.y()+1, r.x(), r.y() + r.height()-1 -1 );
        else
            p->drawLine( r.x()+1, r.y(), r.x() + r.width()-1-1, r.y() );
        
        p->setPen( cg.shadow() );
        if ( flags & Style_Horizontal )
            p->drawLine( r.x() + r.width() -1, r.y()+1, r.x()+ r.width()-1, r.y()+ r.height() -1-1 );
        else
            p->drawLine( r.x()+1, r.y()+r.height()-1, r.x()+r.width()-1-1, r.y()+r.height()-1 );
        
        // We now draw some idented notches. They fill 1/4 of the space, and hence start 3/8 along the splitter.
        int x1, y1, i;
        int nnotch;
        if ( flags & Style_Horizontal )
        {   
            x1 = r.x() + 3; 
            y1 = r.y() + 3* int(r.height()/8);
            nnotch = int( r.height()/(4*10 ));
        }
        else
        { 
            x1 = r.x() + 3* int(r.width()/8); 
            y1 = r.y() + 4;
            nnotch = int( r.width()/(4*10 ));
        }
        
#define DRAWNOTCH \
            p->setPen( cg.light() );\
            p->drawPoint( x1, y1 ); p->drawPoint( x1+1, y1 ); p->drawPoint( x1, y1 +1 ); p->drawPoint( x1+1, y1+1 );\
            p->setPen( cg.shadow() );\
            p->drawPoint( x1-1, y1 ); p->drawPoint( x1-1, y1+1 ); p->drawPoint( x1, y1 -1 ); p->drawPoint( x1+1, y1-1 );\
            p->setPen( cg.midlight() );\
            p->drawPoint( x1+2, y1 ); p->drawPoint( x1+2, y1+1 );
        //p->drawPoint( x1, y1 +2 ); p->drawPoint( x1+1, y1+2 );
        
        for ( i = 0; i < nnotch; i++ )
        {   
            DRAWNOTCH;
            if ( flags & Style_Horizontal )
                y1 += 10;
            else
                x1 += 10;
        }
        
        break;
    }
    
    case PE_ArrowUp: // all fall through (NB see SGIstyle for tips).
    case PE_ArrowDown:
    case PE_ArrowLeft:
    case PE_ArrowRight:
    {
        QPointArray a;	// our arrow polygon
        switch ( pe ) {
        case PE_ArrowUp:
            a.setPoints( 3, 0,-4, -5,4, 4,4 );
            break;
        case PE_ArrowDown:
            a.setPoints( 3, 0,4, -4,-3, 4,-3 );
            break;
        case PE_ArrowLeft:
            a.setPoints( 3, -4,0, 4,-5, 4,4 );
            break;
        case PE_ArrowRight:
            a.setPoints( 3, 4,0, -4,-5, -4,4 );
            break;
        default:
            return;
        }
        
        p->save();
        a.translate( r.x()+r.width()/2, r.y()+r.height()/2 );
        p->setBrush( flags & Style_Enabled ? cg.shadow() : cg.button() );
        p->setPen( cg.shadow() );
        p->drawPolygon( a ); // draw arrow
        p->restore();
    }
    break;

    case PE_TabBarBase:
    {
        p->setPen( QPen::NoPen );
        p->setBrush( cg.shadow() );
        p->drawRect( r );

        p->setPen( cg.midlight() );
        p->drawLine( r.left() + 1, r.bottom(), r.right() - 1, r.bottom() );
    }
    break;
    
    default:
        QCommonStyle::drawPrimitive( pe, p, r, cg, flags, opt );
        break;
    }
}

void SuperGStyle::drawControl( ControlElement element,
                               QPainter *p,
                               const QWidget *widget,
                               const QRect &r,
                               const QColorGroup &cg,
                               SFlags how,
                               const QStyleOption& opt ) const
{
    switch( element ) {
    case CE_MenuBarItem:
    {
        QCommonStyle::drawControl( element, p, widget, r, cg, how, opt );
        if ( how & QStyle::Style_Active )
        {
            // draw a highlight over the top
            p->setPen( cg.light() );
            p->drawLine( r.bottomLeft(), r.topLeft() );
            p->drawLine( r.topLeft(), r.topRight() );
            p->drawLine( r.topRight(), r.bottomRight() );

            m_menuItem = r;
        }
        else
        {
            // m_menuItem = QRect();
        }
        break;
    }
    case CE_PopupMenuItem:
    {
        // taken from qwindowsstyle
        if (! widget || opt.isDefault())
            break;
        
        const QPopupMenu *popupmenu = (const QPopupMenu *) widget;

        QMenuItem *mi = opt.menuItem();
        if ( !mi )
            break;

        int tab = opt.tabWidth();
        int maxpmw = opt.maxIconWidth();
        bool dis = !(how&Style_Enabled);
        bool checkable = popupmenu->isCheckable();
        bool act = how & Style_Active;
        int x, y, w, h;
        
        r.rect(&x, &y, &w, &h);
        x += 1;
        y += 1;
        w -= 2;
        h -= 2;

        if ( checkable ) {
            maxpmw = QMAX( maxpmw, 20 );
        }

        int checkcol = maxpmw;
        
        if ( mi && mi->isSeparator() ) {                    // draw separator
            p->setPen( cg.dark() );
            p->drawLine( x, y, x+w, y );
            p->setPen( cg.light() );
            p->drawLine( x, y+1, x+w, y+1 );
            return;
        }

        QBrush fill = (act ?
                       cg.brush( QColorGroup::Highlight ) :
                       cg.brush( QColorGroup::Button ));
        p->fillRect( x, y, w, h, fill);
        
        if ( !mi )
            return;
        
        int xpos = x;
        QRect vrect = visualRect( QRect( xpos, y, checkcol, h ), r );
        int xvis = vrect.x();
        if ( mi->isChecked() ) {
            if ( act && !dis )
                // drawSuperGBox( p, QRect( r.left(), r.top(), r.width(), r.height() ), cg, false );
                qDrawShadePanel( p, xvis, y, checkcol, h,
                                 cg, TRUE, 1, &cg.brush( QColorGroup::Button ) );
            else {
                QBrush fill( cg.light(), Dense4Pattern );
                // set the brush origin for the hash pattern to the x/y coordinate
                // of the menu item's checkmark... this way, the check marks have
                // a consistent look
                QPoint origin = p->brushOrigin();
                p->setBrushOrigin( xvis, y );
                qDrawShadePanel( p, xvis, y, checkcol, h, cg, TRUE, 1,
                                 &fill );
                // restore the previous brush origin
                p->setBrushOrigin( origin );
            }
        } else if (! act)
            p->fillRect(xvis, y, checkcol , h, cg.brush( QColorGroup::Button ));
        
        if ( mi->iconSet() ) {              // draw iconset
            QIconSet::Mode mode = dis ? QIconSet::Disabled : QIconSet::Normal;
            if (act && !dis )
                mode = QIconSet::Active;
            QPixmap pixmap;
            if ( checkable && mi->isChecked() )
                pixmap = mi->iconSet()->pixmap( QIconSet::Small, mode, QIconSet::On );
            else
                pixmap = mi->iconSet()->pixmap( QIconSet::Small, mode );
            int pixw = pixmap.width();
            int pixh = pixmap.height();
            QRect pmr( 0, 0, pixw, pixh );
            pmr.moveCenter( vrect.center() );
            p->setPen( cg.text() );
            p->drawPixmap( pmr.topLeft(), pixmap );
            
            fill = (act ?
                    cg.brush( QColorGroup::Highlight ) :
                    cg.brush( QColorGroup::Button ));
            int xp = xpos + checkcol + 1;
            p->fillRect( visualRect( QRect( xp, y, w - checkcol - 1, h ), r ), fill);
        } else  if ( checkable ) {  // just "checking"...
            if ( mi->isChecked() ) {
                int xp = xpos + windowsItemFrame;
                
                SFlags cflags = Style_Default;
                if (! dis)
                    cflags |= Style_Enabled;
                if (act)
                    cflags |= Style_On;
                
                drawPrimitive(PE_CheckMark, p,
                              visualRect( QRect(xp, y + windowsItemFrame,
                                                checkcol - 2*windowsItemFrame,
                                                h - 2*windowsItemFrame), r ), cg, cflags);
            }
        }
        
        p->setPen( act ? cg.highlightedText() : cg.buttonText() );
        
        QColor discol;
        if ( dis ) {
            discol = cg.text();
            p->setPen( discol );
        }
        
        int xm = windowsItemFrame + checkcol + windowsItemHMargin;
        xpos += xm;
        
        vrect = visualRect( QRect( xpos, y+windowsItemVMargin, w-xm-tab+1, h-2*windowsItemVMargin ), r );
        xvis = vrect.x();
        if ( mi->custom() ) {
            p->save();
            if ( dis && !act ) {
                p->setPen( cg.light() );
                mi->custom()->paint( p, cg, act, !dis,
                                     xvis+1, y+windowsItemVMargin+1, w-xm-tab+1, h-2*windowsItemVMargin );
                p->setPen( discol );
            }
            mi->custom()->paint( p, cg, act, !dis,
                                 xvis, y+windowsItemVMargin, w-xm-tab+1, h-2*windowsItemVMargin );
            p->restore();
        }
        QString s = mi->text();
        if ( !s.isNull() ) {                        // draw text
            int t = s.find( '\t' );
            int text_flags = AlignVCenter|ShowPrefix | DontClip | SingleLine;
            if (!styleHint( SH_UnderlineAccelerator, widget, QStyleOption::Default, NULL ))
                text_flags |= NoAccel;
            text_flags |= (QApplication::reverseLayout() ? AlignRight : AlignLeft );
            if ( t >= 0 ) {                         // draw tab text
                int xp = x + w - tab - windowsItemHMargin - windowsItemFrame + 1 - 20;
                int xoff = visualRect( QRect( xp, y+windowsItemVMargin, tab, h-2*windowsItemVMargin ), r ).x();
                if ( dis && !act ) {
                    p->setPen( cg.light() );
                    p->drawText( xoff+1, y+windowsItemVMargin+1, tab, h-2*windowsItemVMargin, text_flags, s.mid( t+1 ));
                    p->setPen( discol );
                }
                p->drawText( xoff, y+windowsItemVMargin, tab, h-2*windowsItemVMargin, text_flags, s.mid( t+1 ) );
                s = s.left( t );
            }
            if ( dis && !act ) {
                p->setPen( cg.light() );
                p->drawText( xvis+1, y+windowsItemVMargin+1, w-xm-tab+1, h-2*windowsItemVMargin, text_flags, s, t );
                p->setPen( discol );
            }
            p->drawText( xvis, y+windowsItemVMargin, w-xm-tab+1, h-2*windowsItemVMargin, text_flags, s, t );
        } else if ( mi->pixmap() ) {                        // draw pixmap
            QPixmap *pixmap = mi->pixmap();
            if ( pixmap->depth() == 1 )
                p->setBackgroundMode( OpaqueMode );
            p->drawPixmap( xvis, y+windowsItemFrame, *pixmap );
            if ( pixmap->depth() == 1 )
                p->setBackgroundMode( TransparentMode );
        }
        if ( mi->popup() ) {                        // draw sub menu arrow
            int dim = (h-2*windowsItemFrame) / 2;
            PrimitiveElement arrow;
            arrow = ( QApplication::reverseLayout() ? PE_ArrowLeft : PE_ArrowRight );
            xpos = x+w - windowsArrowHMargin - windowsItemFrame - dim;
            vrect = visualRect( QRect(xpos, y + h / 2 - dim / 2, dim, dim), r );
            if ( act ) {
                QColorGroup g2 = cg;
                g2.setColor( QColorGroup::ButtonText, g2.highlightedText() );
                drawPrimitive(arrow, p, vrect,
                              g2, dis ? Style_Default : Style_Enabled);
            } else {
                drawPrimitive(arrow, p, vrect,
                              cg, dis ? Style_Default : Style_Enabled );
            }
        }
        
        break;
    }

    case CE_TabBarTab:
    { 
        const QTabBar *tabbar;
        tabbar = (const QTabBar*)widget;
        const QTab * tab = opt.tab();
        if ( !tab )
            break;
        bool selected = ( tabbar->currentTab() == tab->identifier() );
//         QImage tabbarimageleft;
//         QImage tabbarimageright;
        if ( selected )
        {    
//             // NB QPainter scales images sooo badly. We split in two to try make it better.
//             tabbarimageleft = qembed_findImage("tab-current-left" );
//             tabbarimageright = qembed_findImage("tab-current-right" );
//             p->drawImage( QRect( r.left() + 3, r.top(), r.width() - tabbarimageright.width(), r.height() ), tabbarimageleft );
//             p->drawImage( QRect( r.left() + r.width() - tabbarimageright.width(), r.top(), tabbarimageright.width(), r.height() ), tabbarimageright );          
            p->fillRect( r.x(), r.y(), r.width(), r.height() - 1, cg.shadow() );
            
            const int INSET = 6;
            QPointArray points( 6 );
            points.putPoints( 0, 6,
                              r.left() + 1, r.bottom(),
                              r.left() + INSET, r.bottom(),
                              r.left() + INSET, r.top() + INSET,
                              r.right() - 3*r.height()/5, r.top() + INSET,
                              r.right() - INSET, r.bottom(),
                              r.right(), r.bottom() );
            p->setBrush( cg.button() );
            p->setPen( cg.button() );
            p->drawPolygon( points );
            
            // now highlight
            p->setPen( QPen( cg.midlight() ) );
            p->drawLine( points[0], points[1] );
            p->drawLine( points[1], QPoint( points[2].x(), points[2].y() + 1 ) );
            p->drawLine( QPoint( points[2].x() + 1, points[2].y() ), QPoint( points[3].x() - 2, points[3].y() ) ); // this is the right hand top corner
            p->drawLine( QPoint( points[3].x() - 1, points[3].y() + 1 ), QPoint( points[3].x(), points[3].y() + 2 ) );
            p->drawLine( QPoint( points[3].x(), points[3].y() + 2 ), points[4] );
            p->drawLine( points[4], points[5] );
        }
        else
        {   
            p->setPen ( cg.shadow() );
            p->setBrush( cg.shadow() );
            p->drawRect( r.x(), r.y(), r.width(), r.height() - 1 );
            
            p->setPen( cg.midlight() );
            p->drawLine( r.left() + 1, r.bottom(), r.right(), r.bottom() );
        }
        
        
        break;
    }
    case CE_TabBarLabel: // the label within a QTab.
    {
        const QTabBar *tabbar;
        tabbar = (const QTabBar*)widget;
        const QTab * tab = opt.tab();
        if ( !tab )
            break;
        p->setPen( cg.shadow()  );
        bool current = ( tabbar->currentTab() == tab->identifier() );
        if ( current )
            p->setPen( cg.text() ); 
        else
            p->setPen( cg.midlight() );

        QFontMetrics fm( p->fontMetrics() );
        int textHeight = fm.height();
        int textWidth = fm.width( tab->text() );
        int xoffset = ( r.width() - textWidth ) / 2;
        int yoffset = r.height() - textHeight;

        if ( xoffset < 0 ) xoffset = 0;
        if ( yoffset < 0 ) yoffset = 0;

        if ( current )
            p->drawText( r.x() + xoffset, r.bottom(), tab->text()  );
        else
            p->drawText( r.x() + xoffset, r.bottom() - yoffset, tab->text()  );

        break;
    }
    
    default:
        QCommonStyle::drawControl( element, p, widget, r, cg, how, opt );
        break;
    }
}

QRect SuperGStyle::querySubControlMetrics( ComplexControl control,
                                           const QWidget *widget,
                                           SubControl sc,
                                           const QStyleOption &opt ) const
{
    if ( control == CC_Slider  )
    {
        const QSlider * sl = (const QSlider *) widget;
        switch ( sc ) {
        case SC_SliderHandle: 
        {
            int sliderPos = 0;
            int len = pixelMetric( PM_SliderLength, sl );
            int thickness = pixelMetric( PM_SliderControlThickness, sl );

            // ensure the handle is always centred
            int inset = QMIN( widget->width(), widget->height() ) - thickness;
            inset = QMAX( 1, inset/2 );

            sliderPos = sl->sliderStart();
            
            if ( sl->orientation() == Horizontal )
                return QRect( sliderPos, inset, len, thickness );

            return QRect( inset, sliderPos, thickness, len ); 
        }
        case SC_SliderGroove: 
        {
            int thickness = pixelMetric( PM_SliderThickness, sl );
            if ( sl->orientation() == Horizontal )
                return QRect( 0, ( sl->height() - thickness )/2, sl->width(), thickness );

                return QRect( ( sl->width() - thickness )/2, 0, thickness, sl->height() );
        }
            
        default:
            break;
        }

    }

    return QCommonStyle::querySubControlMetrics( control, widget, sc, opt );
}

void SuperGStyle::drawComplexControl( ComplexControl cc,
                                      QPainter *p,
                                      const QWidget *widget,
                                      const QRect &r,
                                      const QColorGroup &cg,
                                      SFlags how,
                                      SCFlags sub,
                                      SCFlags subActive,
                                      const QStyleOption& opt ) const
{
    switch ( cc ) {
    case CC_Slider:
    {
        // NB you can paint a full custom slider here.
        QRect handle = querySubControlMetrics( CC_Slider, widget,
                                               SC_SliderHandle, opt);

        QRect groove = querySubControlMetrics( CC_Slider, widget,
                                               SC_SliderGroove, opt);

        if ( sub & SC_SliderGroove )
        {   
            // fill the rect to avoid artefacts from regions we dont paint.
            p->setBrush( cg.button() );
            p->setPen( cg.button() );
            p->drawRect( r );
            
            // Groove
            p->setPen( QPen( cg.shadow(), 1 ) );
            p->drawRect( groove );
        }         
        if ( (sub & SC_SliderHandle) && handle.isValid() )
        {
            p->setBrush( cg.shadow() );
            p->setPen( QPen::NoPen );
            p->drawRect( handle );
        }
        
        break;           
    }
    case CC_ComboBox:
    {
        const QComboBox *cmb = ( const QComboBox* ) widget;
        
        p->setBrush( cg.button() );
        p->setPen( Qt::NoPen );
        p->drawRect( r.x(), r.y(), r.width(), r.height() );
        p->setPen( cg.shadow() );
        
        p->drawRoundRect( r.x(), r.y(), r.width(), r.height()-1, 5, 60 );
        
        p->drawLine( r.x() + r.width() - 19, r.y()+1, r.x() + r.width() - 19, r.y() + r.height() -2 ); // line before the arrow
        
        // line under arrow
        p->setPen( cg.shadow() );
        p->drawLine( r.x() + r.width() - 14,  r.y() + r.height() -6,  r.x() + r.width() - 6,  r.y() + r.height() -6 );
        
        // Highlights/shadows
        p->setPen( cg.shadow().light( 145) );
        p->drawLine( r.x() + 5, r.y()+r.height()-1, r.x() + r.width() -6, r.y()+r.height()-1 );
        p->drawLine( r.x() + r.width() - 14,  r.y() + r.height() -6 +1,  r.x() + r.width() - 6,  r.y() + r.height() -6 +1);
        
        
        p->setPen( cg.text() );  // WHY? restores p.
        
        drawPrimitive( PE_ArrowDown, p,
                       QRect( r.x() + r.width() - 2 - 16 + 2,
                              r.y() +1 + 2, 16 - 4, r.height() - 4 -4 ),
                       cg,
                       cmb->isEnabled() ? Style_Enabled : Style_Default,
                       opt );
        
        break;
    }
    
    case CC_ListView:
    {   
        // (code started as direct rip from QCommonStyle -- its pretty ugly...)
        QColor lvpen( cg.shadow() ); // used for all lines/markers/
        if (  sub & SC_ListView ) {
            QCommonStyle::drawComplexControl( cc, p, widget, r, cg, how, sub, subActive, opt );
        }
        if ( sub & ( SC_ListViewBranch | SC_ListViewExpand ) ) {
            if (opt.isDefault())
                break;
            
            QListViewItem *item = opt.listViewItem(),
                *child = item->firstChild();
            
            int y = r.y();
            int c;
            int dotoffset = 0;
            QPointArray dotlines;
            if ( subActive == (uint)SC_All && sub == SC_ListViewExpand ) {
                c = 2;
                dotlines.resize(2);
                dotlines[0] = QPoint( r.right(), r.top() );
                dotlines[1] = QPoint( r.right(), r.bottom() );
            } else {
                int linetop = 0, linebot = 0;
                // each branch needs at most two lines, ie. four end points
                dotoffset = (item->itemPos() + item->height() - y) %2;
                dotlines.resize( item->childCount() * 4 );
                c = 0;
                
                // skip the stuff above the exposed rectangle
                while ( child && y + child->height() <= 0 ) {
                    y += child->totalHeight();
                    child = child->nextSibling();
                }
                
                int bx = r.width() / 2;
                
                // paint stuff in the magical area
                QListView* v = item->listView();
                while ( child && y < r.height() ) {
                    if (child->isVisible()) {
                        int lh;
                        if ( !item->multiLinesEnabled() )
                            lh = child->height();
                        else
                            lh = p->fontMetrics().height() + 2 * v->itemMargin();
                        lh = QMAX( lh, QApplication::globalStrut().height() );
                        if ( lh % 2 > 0 )
                            lh++;
                        linebot = y + lh/2;
                        if ( (child->isExpandable() || child->childCount()) &&
                             (child->height() > 0) ) 
                        {
                            p->setPen( lvpen );
                            p->setBrush( lvpen );
                            QPointArray tri( 0 );
                            int xmv = 2;
                            if ( !child->isOpen() )
                                tri.putPoints( 0, 4,   bx-4+xmv, linebot-4,  bx-4+6+xmv, linebot -4+3,   bx-4+xmv, linebot-4+6,   bx-4+xmv, linebot-4  );
                            else
                            {  
                                xmv = 1; 
                                tri.putPoints( 0, 4,   bx-4+xmv, linebot-4 ,  bx-4+3+xmv, linebot -4+6,   bx-4+6+xmv, linebot-4,   bx-4+xmv, linebot-4  );
                            }
                            p->drawPolygon( tri, false, 0, 4 );
                            
                            // dotlinery
                            dotlines[c++] = QPoint( bx, linetop );
                            dotlines[c++] = QPoint( bx, linebot - 4 );
                            dotlines[c++] = QPoint( bx + 5, linebot );
                            dotlines[c++] = QPoint( r.width(), linebot );
                            linetop = linebot + 5;
                        } else {
                            // just dotlinery
                            dotlines[c++] = QPoint( bx+1, linebot -1);
                            dotlines[c++] = QPoint( r.width(), linebot -1);
                        }
                        y += child->totalHeight();
                    }
                    child = child->nextSibling();
                }
                
                // Expand line height to edge of rectangle if there's any
                // visible child below
                while ( child && child->height() <= 0)
                    child = child->nextSibling();
                if ( child )
                    linebot = r.height();
                
                if ( linetop < linebot ) {
                    dotlines[c++] = QPoint( bx, linetop );
                    dotlines[c++] = QPoint( bx, linebot );
                }
            }
            p->setPen( lvpen );
            
            static QBitmap *verticalLine = 0, *horizontalLine = 0;
            static QCleanupHandler<QBitmap> qlv_cleanup_bitmap;
            if ( !verticalLine ) {
                // make 128*1 and 1*128 bitmaps that can be used for
                // drawing the right sort of lines.
                // Updated from QCommonStyle: default used dotted lines, we dont.
                verticalLine = new QBitmap( 1, 129, TRUE );
                horizontalLine = new QBitmap( 128, 1, TRUE );
                QPainter p;
                p.begin( verticalLine );
                p.setPen( lvpen );
                p.drawLine( 0, 0, 0, 128 );
                p.end();
                
                QApplication::flushX();
                verticalLine->setMask( *verticalLine );
                p.begin( horizontalLine );
                p.setPen( lvpen );
                p.drawLine( 0, 0, 128, 0 );
                p.end();
                QApplication::flushX();
                horizontalLine->setMask( *horizontalLine );
                qlv_cleanup_bitmap.add( &verticalLine );
                qlv_cleanup_bitmap.add( &horizontalLine );
            }
            
            int line; // index into dotlines
            if ( sub & SC_ListViewBranch ) for( line = 0; line < c; line += 2 ) {
                // assumptions here: lines are horizontal or vertical.
                // lines always start with the numerically lowest
                // coordinate.
                
                // point ... relevant coordinate of current point
                // end ..... same coordinate of the end of the current line
                // other ... the other coordinate of the current point/line
                if ( dotlines[line].y() == dotlines[line+1].y() ) {
                    int end = dotlines[line+1].x();
                    int point = dotlines[line].x();
                    int other = dotlines[line].y();
                    while( point < end ) {
                        int i = 128;
                        if ( i+point > end )
                            i = end-point;
                        p->drawPixmap( point, other, *horizontalLine,
                                       0, 0, i, 1 );
                        point += i;
                    }
                } else {
                    int end = dotlines[line+1].y();
                    int point = dotlines[line].y();
                    int other = dotlines[line].x();
                    int pixmapoffset = ((point & 1) != dotoffset ) ? 1 : 0;
                    while( point < end ) {
                        int i = 128;
                        if ( i+point > end )
                            i = end-point;
                        p->drawPixmap( other, point, *verticalLine,
                                       0, pixmapoffset, 1, i );
                        point += i;
                    }
                }
            }
        }
	break;	
    }
    default:
        QCommonStyle::drawComplexControl( cc, p, widget, r, cg, how, sub, subActive, opt );
        break;
    }
}


/*!
  Draw a metallic button, sunken if \a sunken is TRUE, horizontal if
  /a horz is TRUE.
*/

void SuperGStyle::drawStyledButton( QPainter *p, int x, int y, int w, int h,
                                    bool sunken, bool horz, bool flat  ) const
{
    
    //if ( !flat ) 
    drawStyledFrame( p, x, y, w, h );
    drawStyledGradient( p, x, y, w, h, sunken, horz, flat );
}





void SuperGStyle::drawStyledFrame( QPainter *p, int x, int y, int w, int h ) const
{
    /*
      QColor top1("#878769691515");
      QColor top2("#C6C6B4B44949");
      
      QColor bot2("#70705B5B1414");
      QColor bot1("#56564A4A0E0E"); //first from the bottom
    */
    
    QColor top1("#8e9293");
    QColor top2("#646a6a");
    
    QColor bot2("#2f3032");
    QColor bot1("#141414"); //first from the bottom
    
    
    
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    
    //frame:
    
    p->setPen( top1 );
    p->drawLine( x, y2, x, y );
    p->drawLine( x, y, x2-1, y );
    p->setPen( top2 );
    p->drawLine( x+1, y2 -1, x+1, y+1 );
    p->drawLine( x+1, y+1 , x2-2, y+1 );
    
    p->setPen( bot1 );
    p->drawLine( x+1, y2, x2, y2 );
    p->drawLine( x2, y2, x2, y );
    p->setPen( bot2 );
    p->drawLine( x+1, y2-1, x2-1, y2-1 );
    p->drawLine( x2-1, y2-1, x2-1, y+1 );
    
    
}


void SuperGStyle::drawStyledGradient( QPainter *p, int x, int y, int w, int h,
                                      bool sunken, bool horz, bool flat  ) const
    
{
    QColor highlight("#7f8283" );
    QColor subh1( "#656b6b" );
    QColor subh2( "#5d6363" );
    
    QColor topgrad( "#53585b" );
    QColor botgrad( "#343839" );
    
    // Hmm, disable this bit of 'flat' for now, draw it the same (see button code, we dont draw a frame if flat)
    if ( 0 && flat && !sunken ) {
        p->fillRect( x + 2, y + 2, w - 4,h -4, topgrad );
    } else {
        // highlight:
        int i = 0;
        int x1 = x + 2;
        int y1 = y + 2;
        int x2 = x + w - 1;
        int y2 = y + h - 1;
        if ( horz )
            x2 = x2 - 2;
        else
            y2 = y2 - 2;
        
#define DRAWGLINE if (horz) \
    p->drawLine( x1, y1+i, x2, y1+i ); \
         else \
         p->drawLine( x1+i, y1, x1+i, y2 ); \
         i++;
        
        if ( !sunken ) {
            p->setPen( highlight );
            DRAWGLINE;
            DRAWGLINE;
            p->setPen( subh1 );
            DRAWGLINE;
            p->setPen( subh2 );
            DRAWGLINE;
        }
        // gradient:
        int ng = (horz ? h : w) - 8; // how many lines for the gradient?
        
        int h1, h2, s1, s2, v1, v2;
        if ( !sunken ) {
            topgrad.hsv( &h1, &s1, &v1 );
            botgrad.hsv( &h2, &s2, &v2 );
        } else {
            botgrad.hsv( &h1, &s1, &v1 );
            topgrad.hsv( &h2, &s2, &v2 );
            if ( v1 > 10 ) v1 -= 10; // darken slightly, makes headers & toggle buttons more clearly selected
            if ( v2 > 10 ) v2 -= 10;
        }
        
        if ( ng > 1 ) {	
            for ( int j =0; j < ng; j++ ) {
                p->setPen( QColor( h1 + ((h2-h1)*j)/(ng-1),
                                   s1 + ((s2-s1)*j)/(ng-1),
                                   v1 + ((v2-v1)*j)/(ng-1),  QColor::Hsv ) );
                DRAWGLINE;
            }
        } else if ( ng == 1 ) {
            p->setPen( QColor((h1+h2)/2, (s1+s2)/2, (v1+v2)/2, QColor::Hsv) );
            DRAWGLINE;
        }
        if ( sunken ) {
            p->setPen( subh2 );
            DRAWGLINE;
            
            p->setPen( subh1 );
            DRAWGLINE;
            
            p->setPen( highlight );
            DRAWGLINE;
            DRAWGLINE;
        }
    }    
}

void SuperGStyle::drawStyledCoolGradient( QPainter *p, int x, int y, int w, int h, bool horz  ) const
{
    
    // unlike main drawStyledGradient this only has one form, no 'sunken' etc.
    
    QColor topgrad( "#2c373b" );
    QColor botgrad( "#4a6266" );
    
    // highlight:
    int i = 0;
    int x1 = x + 2;
    int y1 = y + 2;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    if ( horz )
        x2 = x2 - 2;
    else
        y2 = y2 - 2;
    
#define DRAWCLINE if (horz) \
    p->drawLine( x1, y1+i, x2, y1+i ); \
         else \
         p->drawLine( x1+i, y1, x1+i, y2 ); \
         i++;
    
    // gradient:
    int ng = (horz ? h : w) - 8; // how many lines for the gradient?
    
    int h1, h2, s1, s2, v1, v2;
    
    botgrad.hsv( &h1, &s1, &v1 );
    topgrad.hsv( &h2, &s2, &v2 );
    
    if ( ng > 1 ) {	
        for ( int j =0; j < ng; j++ ) {
            p->setPen( QColor( h1 + ((h2-h1)*j)/(ng-1),
                               s1 + ((s2-s1)*j)/(ng-1),
                               v1 + ((v2-v1)*j)/(ng-1),  QColor::Hsv ) );
            DRAWCLINE;
        }
    } else if ( ng == 1 ) {
        p->setPen( QColor((h1+h2)/2, (s1+s2)/2, (v1+v2)/2, QColor::Hsv) );
        DRAWCLINE;
    }
}

int SuperGStyle::styleHint( StyleHint sh, const QWidget* w, const QStyleOption& so, QStyleHintReturn* shr ) const
{
    int ret;

    switch (sh) {
    case SH_ScrollView_FrameOnlyAroundContents:
        ret = 1;
        break;

    case SH_TabBar_PreferNoArrows:
        ret = 1;
        break;

    default:
	ret = QCommonStyle::styleHint( sh, w, so, shr );
	break;
    }

    return ret;
}

int SuperGStyle::pixelMetric( PixelMetric metric, const QWidget *w ) const
{
	int s;
	
    switch ( metric ) {
    case PM_MenuBarFrameWidth:
        return 2;

    case PM_MenuBarItemSpacing:
        return 5;

    case PM_TabBarTabVSpace:
        return 15; // SG. our tabs are taller than normal.

    case PM_SplitterWidth:
        return 9; // SG we need room for a slightly nicer splitter.

    case PM_SliderLength:
    case PM_SliderControlThickness:
        // make the thumb square and slightly inset
        s = pixelMetric( PM_SliderThickness, w );
        return s - 4;

    case PM_SliderThickness:
        return QMIN( 20, QMIN( w->width(), w->height() ) );

    case PM_TabBarBaseHeight:
    case PM_TabBarBaseOverlap:
	return 50;

    case PM_TabBarTabHSpace:
        return 50;

    default:
        return QCommonStyle::pixelMetric( metric, w );
    }
}

QSize SuperGStyle::sizeFromContents ( ContentsType contents, 
                                      const QWidget* w, 
                                      const QSize& contentsSize, 
                                      const QStyleOption & opt ) const
{
    switch( contents )
    {
    case CT_TabWidget:
    {
        QSize s( contentsSize );
        s.setHeight( s.height() - pixelMetric( PM_TabBarBaseHeight, w ) );
        return s;
    }
   
    default:
        break;
    };

    return QCommonStyle::sizeFromContents( contents, w, contentsSize, opt );
}

void SuperGStyle::drawStyledHandle( QPainter *p, int x, int y, int w, int h,
                                    bool horz ) const
{
    QColor highlight("#7f8283" );
    QColor subh1( "#dd6c00" ); //"#ff4201" );
    QColor subh2( "#f3ff00" );// was "#f3c700" );
    
    QColor topgrad( "#dd6c00" ) ; //"#ff7a01" );
    QColor botgrad( "#f3ff00" );// was "#f3c700" );
    
    int i = 0; // main counter for paint loop fn
    int nrw = 0; // option to narrow off
    int x1 = x + 2;
    int y1 = y + 2;
    int x2 = x + w - 1;
    int y2 = y + h - 1;
    if ( horz )
        x2 = x2 - 2;
    else
        y2 = y2 - 2;
    
    int nglim = w/2 +1 ;//? / -w +4; // TO FIX FOR VERT
    
#define DRAWHLINE if (horz) \
    p->drawLine( x1 + nrw, y1+i, x2 - nrw, y1+i ); \
         else \
         p->drawLine( x1+i, y1 + nrw, x1+i, y2 - nrw ); \
         i++; \
    if ( i > (h-nglim) /*&& ((nrw+1)*2)< w*/ ) \
        nrw++;
    
    
    // gradient:
    //int nglim = h -w; // TO FIX FOR VERT
    int ng = (horz ? h : w); //- nglim; // how many lines for the gradient?
    
    int h1, h2, s1, s2, v1, v2;
    
    topgrad.hsv( &h1, &s1, &v1 );
    botgrad.hsv( &h2, &s2, &v2 );
    
    
    if ( ng > 1 ) {	
        for ( int j =0; j < ng; j++ ) {
            p->setPen( QColor( h1 + ((h2-h1)*j)/(ng-1),
                               s1 + ((s2-s1)*j)/(ng-1),
                               v1 + ((v2-v1)*j)/(ng-1),  QColor::Hsv ) );
            DRAWHLINE;
        }
    } else if ( ng == 1 ) {
        p->setPen( QColor((h1+h2)/2, (s1+s2)/2, (v1+v2)/2, QColor::Hsv) );
        DRAWHLINE;
    }
    
    // Draw border around the combined shape
    p->setPen( QColor( 15, 15, 15 ) );
    p->setBrush( Qt::NoBrush );
    
    QPointArray tri( 0 );
    if ( horz )
    {   
        tri.putPoints( 0, 7,   x+1, y+1,   x+1, y+h+1-nglim,    x+nglim-1 -1,y+h,   x+nglim, y+h,   x+w-2, y+h+1-nglim,   x+w-2, y+1,   x+1,y+1);         
    }
    else 
    {   // TODO!
    }
    p->drawPolyline( tri, 0, 7 ); 
    
}

void SuperGStyle::drawSuperGBox( QPainter *p, 
                                 const QRect &r, 
                                 const QColorGroup &cg, 
                                 bool highlight, 
                                 bool fill, 
                                 bool invertcolor, 
                                 bool /*ison*/, 
                                 bool /*isflat*/ ) const
{
    // A rounded box, with some shadowing. See mockups, pretty subtle, nothing fancy. 
    // (NB dont even bother trying to scale a pixmap here, scaling in qt3 qpainter sucks. As does drawRoundRect.)
    
    p->setPen( invertcolor ? cg.light().light(160) : cg.shadow() );
    int corner = 2; // dont change this!

    // purely arbitrary minimum size
    const int MIN_SIZE = 25;
    if ( r.width() < MIN_SIZE || r.height() < MIN_SIZE )
    {
        // don't bother in this case, just draw a regular rect
        p->drawRect( r );
        return;
    }
    
    p->drawLine( r.x()+ corner, r.y(), r.x() + r.width() -1 - corner, r.y() );
    p->drawLine( r.x()+ corner, r.y()+r.height()-1 -2, r.x()+r.width() -1 - corner, r.y()+r.height()-1 -2);
    
    p->drawLine( r.x(), r.y()+corner, r.x(), r.y()+r.height() - corner -1   -1);
    p->drawLine( r.x() + r.width() /*-corner*/ -1, r.y() + corner, r.x() + r.width() /*- corner*/ -1, r.y() + r.height() - corner -1  -1 );
    
    // Fill 
    if ( fill )
    {
        p->setPen( invertcolor ? cg.light() : cg.button() );
        for ( int i = r.y() +1; i < r.y() + r.height() -3; ++i )
            p->drawLine( r.x()+ corner +0, i, r.x() + r.width() -1 - corner, i );
        
        // fiddly bits at edges
        p->drawLine( r.x() + corner -1, r.y() + corner -1, r.x()+r.width() - corner, r.y() + corner -1 );
        p->drawLine( r.x() + corner -1, r.y() + corner -1, r.x() + 1, r.y() + r.height() -4 );
        p->drawLine( r.x() + 2, r.y() + r.height() -4, r.x() + r.width() - corner -1, r.y() + r.height() -4 );
        p->drawLine( r.x()+r.width() - corner , r.y() + corner -1 ,  r.x() + r.width() - corner , r.y() + r.height() -4 );  
    }
    
    // Corners
    p->setPen( invertcolor ? cg.light().light(160) : cg.shadow() );
    p->drawPoint( r.x() + corner -1, r.y() + corner -1 ); p->drawPoint( r.x()+r.width() - corner, r.y() + corner -1 );
    p->drawPoint( r.x() + 1, r.y() + r.height() -2-2 ); p->drawPoint( r.x() + r.width() - corner, r.y() + r.height() -2-2 );

    // Highlight lines
    if ( highlight )
    {   
        p->setPen( invertcolor ? cg.light().dark(120) : cg.button().light(120) );
        p->drawLine( r.x()+ corner, r.y()+1, r.x()+r.width() -1 - corner, r.y() +1);
        p->drawLine( r.x()+ corner, r.y()+r.height() -1-1, r.x()+r.width() -1 - corner, r.y()+r.height() -1-1);
        // Highlight points below the corners
        p->drawPoint( r.x() + corner -1, r.y() + corner -1 +1 ); p->drawPoint( r.x()+r.width() - corner, r.y() + corner -1 +1 );
        p->drawPoint( r.x() + 1, r.y() + r.height() -2  ); p->drawPoint( r.x() + r.width() - corner, r.y() + r.height() -2 );
    }
}
