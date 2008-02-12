/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahformatter.h"
#include "supergrangecontrol.h"

#include <qcheckbox.h>
#include <qslider.h>
#include <qlabel.h>
#include <qdial.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qobject.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qheader.h>

#include "widget.h"
#include "InputLCD.h"

//==============================================================================
JahFormatter::JahFormatter()
{
}

JahFormatter::~JahFormatter()
{
}

/////////////////////////////////////////////////////////////////////////////
// These are interface building subrutines to save space
// in all module interface building code

void JahFormatter::addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
                                  int w, int h, QString icon ) 
{

    button->setToggleButton(false);
    button->setFixedSize(w,h);
    button->setUsesBigPixmap(true);
    button->setGeometry( QRect( x1, y1, x2, y2 ) );

    QIconSet *icoSet;
    icoSet = new QIconSet( QPixmap(icon) );
    icoSet->setPixmap    ( QPixmap(icon), QIconSet::Large, QIconSet::Active);
    icoSet->setPixmap    ( QPixmap(icon), QIconSet::Large, QIconSet::Disabled);
    icoSet->setPixmap    ( QPixmap(icon), QIconSet::Small, QIconSet::Active);
    icoSet->setPixmap    ( QPixmap(icon), QIconSet::Small, QIconSet::Disabled);
    button->setIconSet(*icoSet);
    delete icoSet;

}

void JahFormatter::addJahPlayerButton( JahToolButton * button, int x1, int y1, int x2, int y2,
                                  int, int, QString iconOff, QString iconOn ) 
{

    button->setToggleButton(false);
    //button->setFixedSize(w,h);
    button->setUsesBigPixmap(true);
    button->setGeometry( QRect( x1, y1, x2, y2 ) );

    QIconSet *icoSet;
    icoSet = new QIconSet( QPixmap(iconOff) );
    icoSet->setPixmap    ( QPixmap(iconOn), QIconSet::Large, QIconSet::Active);
    icoSet->setPixmap    ( QPixmap(iconOff), QIconSet::Large, QIconSet::Disabled);
    icoSet->setPixmap    ( QPixmap(iconOn), QIconSet::Small, QIconSet::Active);
    icoSet->setPixmap    ( QPixmap(iconOff), QIconSet::Small, QIconSet::Disabled);
    button->setIconSet(*icoSet);
    delete icoSet;

}

void JahFormatter::addJahPlayerButton( JahToolButton *button, const QString &iconOff, const QString &iconOn ) 
{
    button->setToggleButton(false);
    button->setUsesBigPixmap(true);

    QIconSet *icoSet = new QIconSet( QPixmap(iconOff) );
    icoSet->setPixmap( iconOn, QIconSet::Large, QIconSet::Active);
    icoSet->setPixmap( iconOff, QIconSet::Large, QIconSet::Disabled);
    icoSet->setPixmap( iconOn, QIconSet::Small, QIconSet::Active);
    icoSet->setPixmap( iconOff, QIconSet::Small, QIconSet::Disabled);
    button->setIconSet( *icoSet );
    delete icoSet;
}

void JahFormatter::addJahPlayerButton( JahToolButton * button, QString text, int x1, int y1, int x2, int y2,
                                  int, int ) 
{

    //button->setToggleButton(false);
    //button->setFixedSize(w,h);
    //button->setUsesBigPixmap(true);
    button->setGeometry( QRect( x1, y1, x2, y2 ) );
    button->setText(  QObject::tr( text  ));

}

void JahFormatter::addButton( QPushButton * button, int x, int y, int w, int h, QString text ) 
{

    button->setGeometry( QRect( x, y, w, h ) );
    button->setText( QObject::tr( text  ) );

}

void JahFormatter::addCheckButton( QCheckBox * button, int x, int y, int w, int h, QString text ) 
{

    button->setGeometry( QRect( x, y, w, h ) );
    button->setText( QObject::tr( text  ) );

}

void JahFormatter::addIconButton( QPushButton * button, int x, int y, int w, int h, QString icon ) 
{

  button->setGeometry( QRect( x, y, w, h ) );
	button->setIconSet(QIconSet( QPixmap(icon), QPixmap(icon) ));
	
}

void JahFormatter::addIconButton( QPushButton * button, int x, int y, int w, int h ) 
{

  button->setGeometry( QRect( x, y, w, h ) );
	//button->setIconSet(QIconSet( QPixmap(icon), QPixmap(icon) ));
	
}

void JahFormatter::addToolButton( QToolButton * button, int x, int y, int w, int h, QString icon ) 
{

    button->setGeometry( QRect( x, y, w, h ) );

	button->setToggleButton(true);
	button->setIconSet(QIconSet( QPixmap(icon), QPixmap(icon) ));
	
}

void JahFormatter::addToolButton( QToolButton * button, int x, int y, int w, int h ) 
{

    button->setGeometry( QRect( x, y, w, h ) );
	button->setToggleButton(true);
	button->setIconSet(QIconSet( QPixmap(), QPixmap() ));
	
}


void JahFormatter::addLabel( QLabel * label, int x, int y, int w, int h, QString text, int size) 
{

    label->setGeometry( QRect( x, y, w, h ) );
    label->setText( QObject::tr( text  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );
}

void JahFormatter::addSlider( QSlider * slider, int x, int y, int w, int h, int range) 
{
    slider->setGeometry( QRect( x, y, w, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( -(range) );
    slider->setMaxValue( range );

}

void JahFormatter::addSliderLcd( InputLCD * lcd )
{

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    lcd->setPalette( p, TRUE );
}

void JahFormatter::addSliderLcd( InputLCD * lcd, int x, int y, int w, int h, int )
{
    lcd->setGeometry( QRect( x, y, w, h ) );
    addSliderLcd( lcd );
}

void JahFormatter::addRangeSlider( QSlider * slider, int x, int y, int w, int h, int rangemin, int rangemax) 
{
    slider->setGeometry( QRect( x, y, w, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( rangemin );
    slider->setMaxValue( rangemax );

}


void JahFormatter::addLcdLabel( QFrame * frame,
                                InputLCD * lcd, int x, int y, int w, int h, int,
                                QString labeltext, int size ) {

    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x, y, w, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    lcd->setGeometry( QRect( x+w+5, y, 30, h ) );
    addSliderLcd( lcd );
}

void JahFormatter::addLabelLcd( QFrame * frame,
                                InputLCD * lcd, int x, int y, int w, int h, int,
                                QString labeltext, int size ) {

    lcd->setGeometry( QRect( x, y, 30, h ) );
    addSliderLcd( lcd );

    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x+30, y, w, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );


}

void JahFormatter::addSliderAndLcd( QSlider * slider, int x, int y, int w, int h, int range,
                                    InputLCD * lcd ) {

    slider->setGeometry( QRect( x, y, w, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( -(range) );
    slider->setMaxValue( range );

    lcd->setGeometry( QRect( x, y, w, h ) );
    addSliderLcd( lcd );
}

void JahFormatter::addSliderAndLcd( QSlider * slider, int x, int y, int w, int h, int rangemin, int rangemax,
                               InputLCD * lcd ) 
{

    slider->setGeometry( QRect( x, y, w, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( rangemin );
    slider->setMaxValue( rangemax );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    lcd->setGeometry( QRect( x+w, y, 30, h ) );
    addSliderLcd( lcd );
}


void JahFormatter::addDialLcdLabel(   QFrame * frame,
                                      QDial  * dial, int x, int y, int w, int h, int range,
                                      InputLCD * lcd,
                                      QString labeltext, int size) 
{

    dial->setGeometry( QRect( x+10, y, w+10, h+10 ) );
   // dial->setOrientation( QSlider::Horizontal );
    dial->setNotchesVisible ( true );
    dial->setMinValue( 0 );
    dial->setMaxValue( range );
    dial->setWrapping(true);
    
    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x, y+h+10, w/2, h/2 ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    lcd->setGeometry( QRect( x+(w/2)+5, y+h+15, w/2+10, h/3 ) );
    lcd->setMinInt( 0 );
    addSliderLcd( lcd );
}

void JahFormatter::addJahSliderGroup(    QFrame *,
                                         QSlider  * slider, int x, int y, int w, int h, int range,
                                         InputLCD * lcd,
                                         QLabel * label, QString labeltext, int size)
{

    //QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x + 10, y + 15, w + 100, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    slider->setGeometry( QRect( x + 10, y, w + 100, h-5 ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( -(range) );
    slider->setMaxValue( range );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active,   QColorGroup::Foreground, QColor( 0, 255, 0 ) );
    p.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 0, 255, 0 ) ); //Qt::green

    lcd->setGeometry( QRect( x + w + 115, y, 30, h ) );
    addSliderLcd( lcd );
    lcd->setPalette( p, TRUE );
}

void JahFormatter::addJahSliderGroup(    QFrame * ,
                                    QSlider  * slider, int x, int y, int w, int h, int rangemin, int rangemax,
                                    InputLCD * lcd,
                                    QLabel * label, QString labeltext, int size)
                                    {

    //QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x + 10, y + 15, w + 100, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    slider->setGeometry( QRect( x+10, y, w+100, h-5 ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( rangemin );
    slider->setMaxValue( rangemax );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active,   QColorGroup::Foreground, QColor( 0, 255, 0 ) );
    p.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 0, 255, 0 ) ); //Qt::green

    lcd->setGeometry( QRect( x + w + 115, y, 30, h ) );
    addSliderLcd( lcd );
    lcd->setPalette( p, TRUE );
}


//////////////////////////////////////////////////////////////////
//three overrides here!

void JahFormatter::addSliderLcdLabel( QFrame * frame,
                                 QSlider  * slider, int x, int y, int w, int h, int range,
                                 InputLCD * lcd,
                                 QString labeltext, int size)
                                 {

    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x, y, w, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    slider->setGeometry( QRect( x+10, y, w+100, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( -(range) );
    slider->setMaxValue( range );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active,   QColorGroup::Foreground, QColor( 0, 255, 0 ) );
    p.setColor( QPalette::Disabled, QColorGroup::Foreground, QColor( 0, 255, 0 ) ); //Qt::green

    lcd->setGeometry( QRect( x+130, y, 30, h ) );
    addSliderLcd( lcd );
    lcd->setPalette( p, TRUE );
}


void JahFormatter::addSliderLcdLabel( QFrame * frame,
                                      QSlider  * slider, int x, int y, int w, int h,
                                      int rangemin, int rangemax,
                                      InputLCD * lcd,
                                      QString labeltext, int size)
{

    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x, y, w, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    slider->setGeometry( QRect( x+10, y, w+100, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( rangemin );
    slider->setMaxValue( rangemax );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    lcd->setGeometry( QRect( x+130, y, w+30, h ) );
    addSliderLcd( lcd );
    lcd->setPalette( p, TRUE );
    lcd->setValue( rangemin );
}

void JahFormatter::addSliderLcdLabel( QFrame * frame,
                                      QSlider  * slider, int x, int y, int w, int h,
                                      int rangemin, int rangemax, int preset,
                                      InputLCD * lcd,
                                      QString labeltext, int size)
{

    QLabel * label = new QLabel(frame);

    label->setGeometry( QRect( x, y, w, h ) );
    label->setText( QObject::tr( labeltext  ) );
    label->setAlignment( int( QLabel::AlignCenter ) );
    QFont label_font(  label->font() );
    label_font.setPointSize( size );
    label->setFont( label_font );

    slider->setGeometry( QRect( x+10, y, w+100, h ) );
    slider->setOrientation( QSlider::Horizontal );
    slider->setMinValue( rangemin );
    slider->setMaxValue( rangemax );
    slider->setValue( preset );

    QPalette p( QColor( 0, 0, 0 ) );
    p.setColor( QPalette::Active, QColorGroup::Foreground, Qt::green );

    lcd->setGeometry( QRect( x+130, y, w+30, h ) );
    addSliderLcd( lcd );
    lcd->setPalette( p, TRUE );
    lcd->setValue( rangemin );
}

void JahFormatter::configure( SupergRangeControl* c, int def, int min, int max )
{
    c->setRange( min, max );
    c->setValue( def );
}

void JahFormatter::setMarginAndSpacing( QLayout* l )
{
    setMargin( l );
    setSpacing( l );
}

void JahFormatter::setMarginAndSpacingSmall( QLayout* l )
{
    setMarginSmall( l );
    setSpacingSmall( l );
}

void JahFormatter::setMarginAndSpacingLarge( QLayout* l )
{
    setMarginLarge( l );
    setSpacingLarge( l );
}

void JahFormatter::setMargin( QLayout* l )
{
    static const int MARGIN = 10;

    l->setMargin( MARGIN );
}

void JahFormatter::setMarginSmall( QLayout* l )
{
    static const int MARGIN = 2;

    l->setMargin( MARGIN );
}

void JahFormatter::setMarginLarge( QLayout* l )
{
    static const int MARGIN = 20;

    l->setMargin( MARGIN );
}

void JahFormatter::setSpacing( QLayout* l )
{
    static const int SPACING = 5;

    l->setSpacing( SPACING );
}

void JahFormatter::setSpacingSmall( QLayout* l )
{
    static const int SPACING = 1;

    l->setSpacing( SPACING );
}

void JahFormatter::setSpacingLarge( QLayout* l )
{
    static const int SPACING = 10;

    l->setSpacing( SPACING );
}

void JahFormatter::addSpacing( QBoxLayout* l )
{
    l->addSpacing( 20 );
}

void JahFormatter::addSpacingSmall( QBoxLayout* l )
{
    l->addSpacing( 6 );
}

void JahFormatter::setListViewAsSingleColumn( QListView* l )
{
    l->setFixedWidth( 166 );
    l->header()->setStretchEnabled( true, 0 );
    l->header()->setClickEnabled( false );
    l->setVScrollBarMode( QScrollView::AlwaysOn );
    l->setHScrollBarMode( QScrollView::AlwaysOff );
    // l->setFrameShape( QFrame::NoFrame );
}

void JahFormatter::setListViewAsDualColumn( QListView* l )
{
    l->setFixedWidth( 180 );
    l->header()->setStretchEnabled( false, 0 );
    l->header()->setStretchEnabled( false, 1 );
    l->header()->setClickEnabled( false );
    l->setColumnWidthMode( 0, QListView::Manual );
    l->setColumnWidthMode( 1, QListView::Manual );
    l->setVScrollBarMode( QScrollView::AlwaysOn );
    l->setHScrollBarMode( QScrollView::AlwaysOff );
    // l->setFrameShape( QFrame::NoFrame );
}

QWidget* JahFormatter::createSeparator( QWidget* parent, bool vertical )
{
    QFrame* d = new QFrame( parent, "separator" );
    d->setFrameShape( QFrame::StyledPanel );
    d->setFrameShadow( QFrame::Raised );
    if ( vertical )
    {
        d->setMaximumWidth( 2 );
    }
    else
    {
        d->setMaximumHeight( 2 );
    }

    return d;
}

static void swap( QColorGroup& cg, QColorGroup::ColorRole a, QColorGroup::ColorRole b )
{
    QColor tmp = cg.color( a );
    cg.setColor( a, cg.color( b ) );
    cg.setColor( b, tmp );
}

void JahFormatter::swap( QPalette& p, QColorGroup::ColorRole a, QColorGroup::ColorRole b )
{
    QColorGroup active = p.active();
    QColorGroup inactive = p.inactive();
    QColorGroup disabled = p.disabled();
    
    ::swap( active, a, b );
    ::swap( inactive, a, b );
    ::swap( disabled, a, b );

    p.setActive( active );
    p.setInactive( inactive );
    p.setDisabled( disabled );
}
