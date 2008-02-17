#include "customlistviewitem.h"
#include <qpixmap.h>
#include <qlistview.h>
#include <qevent.h>
#include <qcolor.h>
#include <qpalette.h>
#include <qapplication.h>

/* XPM */
static const char * eye16_xpm[] = {
"20 12 17 1",
" 	c None",
".	c #464845",
"+	c #494B48",
"@	c #4D4E4C",
"#	c #4F514E",
"$	c #555754",
"%	c #595A58",
"&	c #5C5D5B",
"*	c #616260",
"=	c #646563",
"-	c #666865",
";	c #696B68",
">	c #6C6D6B",
",	c #6F706E",
"'	c #727471",
")	c #757774",
"!	c #797B78",
"                    ",
"      &,)))'-$      ",
"    =)=>!>,,=,;#    ",
"   ,=@>*@.@@,%&,*   ",
" #)&.%,.*,>@@)@@=>  ",
" !%..*&%!!!=@,@..=- ",
" =*..=&*!!!>.>@@.>$ ",
"  ,@@&=@!!!&@)@.%,  ",
"  %,&@,%@&@.=*.%)#  ",
"   @,>=,=&&>>&,>    ",
"     @>)!))),>@     ",
"                    "};

const QPixmap* CustomListViewItem::ouricon_ = 0;	

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CustomCheckListItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CustomCheckListItem::CustomCheckListItem ( QCheckListItem * parent, const QString & text ) :
                        QCheckListItem( parent, text, QCheckListItem::CheckBoxController )
{
    init();
}

CustomCheckListItem::CustomCheckListItem( QListView * parent, const QString & text, QCheckListItem::Type tt ) :
                        QCheckListItem( parent, text, tt )
{
    init();
}

CustomCheckListItem::CustomCheckListItem ( QCheckListItem * parent, QListViewItem* after, const QString & text, QCheckListItem::Type tt ) :
    QCheckListItem( parent, after, text, tt )
{
    init();
}

CustomCheckListItem::CustomCheckListItem ( QListView * parent, QListViewItem* after, const QString & text, QCheckListItem::Type tt ) :
    QCheckListItem( parent, after, text, tt )
{
    init();
}


void CustomCheckListItem::paintCell ( QPainter * p, const QColorGroup & cg, int column, int width, int align )
{
    
    if ( column == 0 )
    {
        //if ( listView() )
        //    p->fillRect( 0,0, width, height(), cg.base() );//( p, QRect( 0, 0, width, height() ) );
        p->drawText( 25, 10, text( column) ); // FIX ME
        p->drawPixmap ( 0,0, eyeicon_ );
        if ( !isOn() )
        {
            p->setPen( QPen( cg.shadow(), 2 ) );
            p->drawLine( 4, 10, 16, 2 ); // diag line
        }
    }
    else
        QCheckListItem::paintCell( p,  cg, column, width, align );

}

void CustomCheckListItem::paintFocus ( QPainter * p, const QColorGroup & cg, const QRect & r )
{
    QCheckListItem::paintFocus( p, cg, r );
}

void CustomCheckListItem::init()
{
    setOn( true );
    eyeicon_ = QPixmap ( eye16_xpm );
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// CustomListViewItem
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

CustomListViewItem::CustomListViewItem( QListView* parentv ) : QListViewItem( parentv )
{
	initIcons();
	initColors();
	greyout_ = false;
	highlight_ = false;
// TODO
	if ( !1 )
		setPixmap(0, *ouricon_ );
}

CustomListViewItem::CustomListViewItem( CustomListViewItem* parenti ) : QListViewItem( parenti )
{
	initIcons();
	initColors();
	greyout_ = false;
	highlight_ = false;
    p_ = parenti;
    
// TODO
	if ( 0 )
		setPixmap(0, *ouricon_ );
}

void CustomListViewItem::initIcons()
{

}

void CustomListViewItem::initColors()
{
	if ( !bgColor1_.isValid() )
    {	
        bgColor2_ = qApp->palette().color( QPalette::Active, QColorGroup::Base );
        bgColor1_ = qApp->palette().color( QPalette::Active, QColorGroup::Base ).dark(110);
    }
}

void CustomListViewItem::setGreyedOut( bool enable)
{
	greyout_ = enable; 
}

void CustomListViewItem::setHighlighted( bool enable)
{
	highlight_ = enable; 
}

void CustomListViewItem::paintCell( QPainter * p, const QColorGroup & cg,
			       int column, int width, int align )

{
    QColorGroup changecg( cg );
	QFont ourfont = p->font();
	
	if ( highlight_ )
	{	QFont newfont( ourfont );
		newfont.setWeight( QFont::Bold );
		p->setFont( newfont );
	}
	
	if ( greyout_ )
		changecg.setColor( QColorGroup::Text, QColor( "gray" ) );
    // Commented out for now -- can change column colors per column like this:
    /*
    if ( column %2 )
        changecg.setColor( QColorGroup::Base, Qt::red );
    */

    // Flip colors (NB Qt Designer does it like this)
	changecg.setColor( QColorGroup::Base, bgColor() );

	QListViewItem::paintCell( p, changecg, column, width, align);

	p->setFont( ourfont ); // get back orig font.
}

QColor CustomListViewItem::bgColor()
{
	bgColorSwitcher();
	return mybgColor_;
}

void CustomListViewItem::bgColorSwitcher()
{
  if ( listView()->firstChild() == this ) 
    {   mybgColor_ = bgColor1_;
        return;
    }

    QListViewItemIterator it( this );
    --it; // look at previous item
    if ( it.current() ) 
	{
        if ( ( ( CustomListViewItem*)it.current() )->mybgColor_ == bgColor1_ )
            mybgColor_ = bgColor2_;
        else
            mybgColor_ = bgColor1_;
    } else {
        mybgColor_ = bgColor1_;
    }
}

bool CustomListViewItem::isGreyedOut()
{
	return greyout_;
}

bool CustomListViewItem::isHighlighted()
{
	return highlight_;
}


