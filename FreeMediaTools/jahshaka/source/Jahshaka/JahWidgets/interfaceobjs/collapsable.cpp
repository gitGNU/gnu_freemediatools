
#include "collapsable.h"

// qt
#include <qlayout.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qstring.h>
#include <qlabel.h>
#include <qtabwidget.h>
#include <qobjectlist.h>
#include <qsizepolicy.h>

// local
#include <jahpreferences.h>
#include <jahformatter.h>

struct Collapsable::PrivateCollapsable
{
    PrivateCollapsable( Collapsable* p )
        : button( new QPushButton( p ) ),
          label( new QLabel( p ) ),
          open( true ),
          pixmapPath( QString( JahPrefs::getInstance().getBasePath().data() ) + "Pixmaps/widgets/" ),
          initialized( false ),
          parent( p )
    {
        // set the mainlayout for p
        layout = new QVBoxLayout( p );

        widgetLayout = new QVBoxLayout();
        JahFormatter::setMarginAndSpacing( widgetLayout );

        size = QSize( 1, 1 );
        button->setFixedSize( getPixmapForState( open ).size() + QSize( 2, 2 ) );
        button->setFlat( true );
        button->setPixmap( getPixmapForState( open ) );

        label->setBackgroundMode( Qt::PaletteShadow );
        button->setBackgroundMode( Qt::PaletteShadow );
    }

    QPixmap getPixmapForState( bool open )
    {
        if ( open )
        {
            return QPixmap( pixmapPath + "collapse-close.png" );
        }

        return QPixmap( pixmapPath + "collapse-open.png" );

    }

    void forAllExternalChildren( void (QWidget::* fn)() )
    {
        QObjectList* children = parent->queryList( "QWidget", 0, true, false );
        
        for ( QObject* obj = children->first(); obj; obj = children->next() )
        {
            if ( obj == button || obj == label )
            {
                continue;
            }
            
            ( static_cast< QWidget* >( obj )->*fn )();
        }
    }

    QBoxLayout* layout;
    QBoxLayout* widgetLayout;
    QPushButton* button;
    QLabel* label;
    bool open;
    QString pixmapPath;
    bool initialized;
    QSize size;

private:
    QWidget* parent;
};

Collapsable::Collapsable( QWidget* parent, const char* name )
    : QFrame( parent, name ),
      m_d( new PrivateCollapsable( this ) )
{
    
    enabledChange( true );
    connect( m_d->button, SIGNAL( clicked() ), this, SLOT( toggleState() ) );        
    setSizePolicy( QSizePolicy( QSizePolicy::Maximum, QSizePolicy::Preferred ) );
}

Collapsable::~Collapsable()
{}

void Collapsable::paintEvent( QPaintEvent* e )
{
    // draw a banner along the top, the size of the button
    QWidget::paintEvent( e );
    QPainter p( this );

    QColorGroup cg = palette().disabled();
    if ( isEnabled() )
    {
        cg = palette().active();
    }
    
    p.setPen( Qt::NoPen );
    p.setBrush( cg.shadow() );
    const int titleBarHeight = m_d->size.height();
    p.drawRect( 0, 0, width(), titleBarHeight );

    p.setPen( cg.shadow() );
    p.setBrush( Qt::NoBrush );
    p.drawRect( 0, 0, width(), height() );
}

void Collapsable::enabledChange( bool )
{
    // propogate any changes in palette through
    QPalette buttonPalette = palette();
    buttonPalette.setColor( QPalette::Active, QColorGroup::Button, buttonPalette.active().shadow() );
    buttonPalette.setColor( QPalette::Inactive, QColorGroup::Button, buttonPalette.inactive().shadow() );
    buttonPalette.setColor( QPalette::Disabled, QColorGroup::Button, buttonPalette.disabled().shadow() );

    m_d->button->setPalette( buttonPalette );
    m_d->label->setPalette( palette() );
}

void Collapsable::add( QWidget* w )
{
    if ( !m_d->initialized )
    {
        QBoxLayout* topLayout = new QHBoxLayout();
        m_d->layout->addLayout( topLayout );
        m_d->layout->addLayout( m_d->widgetLayout );

        topLayout->addSpacing( 5 );
        topLayout->addWidget( m_d->label );
        topLayout->addStretch();
        topLayout->addWidget( m_d->button );
        topLayout->addSpacing( 2 );

        m_d->size = m_d->button->size();

        m_d->initialized = true;
    }

    m_d->widgetLayout->addWidget( w );
}

void Collapsable::add( QTabWidget* w )
{
    if ( !m_d->initialized )
    {
        w->setCornerWidget( m_d->button );
        m_d->initialized = true;
    }

    m_d->widgetLayout->addWidget( w );
}

void Collapsable::setTitle( const QString& s )
{
    m_d->label->setText( s );
    update();
}

void Collapsable::toggleState()
{
    if ( m_d->open )
    {
        // close; hide all widgets
        m_d->forAllExternalChildren( &QWidget::hide );
        m_d->open = false;
        m_d->layout->setResizeMode( QLayout::Fixed );
    }
    else
    {
        m_d->forAllExternalChildren( &QWidget::show );
        m_d->open = true;
        m_d->layout->setResizeMode( QLayout::Auto );
    }

    m_d->button->setPixmap( m_d->getPixmapForState( m_d->open ) );
    m_d->layout->invalidate();
    setMinimumSize( sizeHint() );

    update();
}
