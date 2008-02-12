
#include "jahheader.h"

// qt
#include <qmenubar.h>
#include <qlayout.h>
#include <qsizepolicy.h>
#include <qsize.h>

/// dummy subclass only present to gain access to menuContentsChanged
class MenuBar : public QMenuBar
{
    Q_OBJECT;

public:
    MenuBar( QWidget* parent ) 
        : QMenuBar( parent )
    {}

signals:
    void contentsChanged();

protected:
    virtual void menuContentsChanged()
    {
        QMenuBar::menuContentsChanged();
        emit contentsChanged();
    }

    virtual void menuStateChanged()
    {
        QMenuBar::menuStateChanged();
        emit contentsChanged();
    }
};

#include "jahheader.moc"

/// create a separator widget

QWidget* createSeparator( QWidget* parent )
{
    QFrame* sp = new QFrame( parent );
    sp->setFrameStyle(  QFrame::StyledPanel );
    sp->setFrameShadow( QFrame::Raised );

    return sp;
}

/// this is more complicated than it really needs to be, mainly thanks to 
/// QMenuBar being a bit too smart.
/// To get QMenuBar to play nicely, we have to put it in a placeholder, which 
/// it will then sit on top of. However, the size of this placeholder is
/// determined by the menubar...
JahHeader::JahHeader( QWidget* parent, const char* name )
    : QWidget( parent, name ),
      m_menuBar( NULL ),
      m_menuPlaceholder( NULL ),
      m_moduleSelectionPlaceholder( NULL ),
      m_feedbackPlaceholder( NULL )
{
    // layout as follows:
    // menubar | moduleSelectionPlaceholder ........ feedbackPlaceholder    
    QHBoxLayout* mainLayout = new QHBoxLayout( this );
    
    m_menuPlaceholder = new QWidget( this );
    MenuBar* menuBar = new MenuBar( m_menuPlaceholder );
    m_moduleSelectionPlaceholder = new QWidget( this );
    m_feedbackPlaceholder = new QWidget( this );

    // layout
    mainLayout->setSpacing( 10 ); // arbitrary constant to look "nice"
    mainLayout->setMargin( 3 );
    mainLayout->addWidget( m_menuPlaceholder );
    mainLayout->addWidget( createSeparator( this ) );
    mainLayout->addWidget( m_moduleSelectionPlaceholder );
    mainLayout->addWidget( createSeparator( this ) );
    mainLayout->addStretch();
    mainLayout->addWidget( m_feedbackPlaceholder );

    // add layouts for the placeholders, and ensure we use autolayout
    QHBoxLayout* l;
    l = new QHBoxLayout( m_moduleSelectionPlaceholder );
    l->setAutoAdd( true );
    l = new QHBoxLayout( m_feedbackPlaceholder );
    l->setAutoAdd( true );

    // make the menubar appear flat
    menuBar->setFrameStyle( QFrame::NoFrame );

    // connect up menubar to placeholder
    connect( menuBar, SIGNAL( contentsChanged() ), this, SLOT( onMenuContentsChanged() ) );
    
    // store menuBar
    m_menuBar = menuBar;

    // and ensure we don't allow vertical resize
    setSizePolicy( QSizePolicy( QSizePolicy::Expanding, QSizePolicy::Fixed ) );
}
    
QMenuBar* JahHeader::menuBar() const
{
    layout()->invalidate();
    return m_menuBar;
}

QWidget* JahHeader::moduleSelectionPlaceholder() const
{
    layout()->invalidate();
    return m_moduleSelectionPlaceholder;
}

QWidget* JahHeader::feedbackPlaceholder() const
{
    layout()->invalidate();
    return m_feedbackPlaceholder;
}

void JahHeader::onMenuContentsChanged()
{
    layout()->invalidate();

    // the +20 is an arbitrary padding number; Qt seems to require it as
    // if the menubar is contained within a widget of exactly the sizehint, 
    // the menubar will wrap.
    m_menuPlaceholder->setMinimumWidth( m_menuBar->sizeHint().width() + 20 );
}
