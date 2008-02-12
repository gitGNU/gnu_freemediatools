
#include "eyeCheckListItem.h"

#include <jahpreferences.h>

EyeCheckListItem::EyeCheckListItem( QCheckListItem* parent, const QString& text, Type tt )
    : QCheckListItem( parent, text, tt )
{
    init();
}

EyeCheckListItem::EyeCheckListItem( QCheckListItem* parent, QListViewItem* after, const QString& text, Type tt )
    : QCheckListItem( parent, after, text, tt )
{
    init();
}

EyeCheckListItem::EyeCheckListItem( QListView* parent, const QString& text, Type tt )
    : QCheckListItem( parent, text, tt )
{
    init();
}

EyeCheckListItem::EyeCheckListItem( QListView* parent, QListViewItem* after, const QString& text, Type tt )
    : QCheckListItem( parent, after, text, tt )
{
    init();
}

bool EyeCheckListItem::eyeState() const 
{ 
    return m_open; 
}

void EyeCheckListItem::setEyeState( bool b ) 
{
    m_open = b;
    listView()->update();
}

void EyeCheckListItem::onPressed( QListViewItem* item, const QPoint&, int c )
{
    if ( dynamic_cast< EyeCheckListItem* >( item ) == this && c == 1 )
    {
	m_open = !m_open;
	setPixmap( 1, getPixmapForState( m_open ) );
	listView()->update();
    }
}

void EyeCheckListItem::init()
{
    m_pixmapPath = QString( JahPrefs::getInstance().getBasePath().data() ) + "Pixmaps/desktop/";
    m_open = true;
    setPixmap( 1, getPixmapForState( m_open ) );
    connect( listView(), SIGNAL( pressed( QListViewItem*, const QPoint&, int ) ),
	     this, SLOT( onPressed( QListViewItem*, const QPoint&, int ) ) );
}

QPixmap EyeCheckListItem::getPixmapForState( bool open )
{
    if ( open )
    {
	return QPixmap( m_pixmapPath + "eye-open.png" );
    }
    
    return QPixmap( m_pixmapPath + "eye-closed.png" );
    
}
