/*******************************************************************************
**
** The source file for the Jahshaka jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyframeeditor.h"
#include "keyframeritem.h"

// qt
#include <widget.h>

// std
#include <algorithm>

const QString KeyframeEditor::ALL_KEY = "all";

KeyframeEditor::KeyframeEditor(	QCanvas* c, QWidget* parent, const char* name, WFlags f) :
    QCanvasView(c,parent,name,f),
    zoomlevel( 4.0 )
{
    setHScrollBarMode( KeyframeEditor::AlwaysOff );
    setVScrollBarMode( KeyframeEditor::AlwaysOff );
}

void KeyframeEditor::cleareditor()
{
    // Clears the keyframes and axis, but not the playline.
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
    if ( *it && (*it)->rtti() != sliderRTTI2 )
        delete *it;
    }
    
    keyItemMap.clear();
}

void KeyframeEditor::add( const QString& key, QCanvasItem* item )
{
    keyItemMap[ key ].push_back( item );
}

void KeyframeEditor::show( const QStringList& keys )
{
    KeyItemMap::iterator I = keyItemMap.begin();
    KeyItemMap::iterator E = keyItemMap.end();

    for ( ; I!=E; ++I )
    {
        QCanvasItemVector::iterator II = I->second.begin();
        QCanvasItemVector::iterator EE = I->second.end();

        if ( keys.contains( I->first ) || keys.contains( ALL_KEY ) )
        {
            std::for_each( II, EE, std::mem_fun( &QCanvasItem::show ) );
        }
        else
        {
            std::for_each( II, EE, std::mem_fun( &QCanvasItem::hide ) );
        }
    }

    canvas()->update();
}


void KeyframeEditor::contentsMousePressEvent(QMouseEvent* e)
{
    QPoint p = inverseWorldMatrix().map(e->pos());

    QCanvasItemList l=canvas()->collisions(p);

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) {
//	if ( (*it)->rtti() == imageRTTI ) {
//	    ImageItem *item= (ImageItem*)(*it);
    // if ( !item->hit( p ) )
//		 continue;
//	}
        if ( (*it)->rtti() == keyframeRTTI )
        {   moving = *it;
            moving_start = p;
            return;
        }
    }
    moving = 0;
}

void KeyframeEditor::contentsMouseMoveEvent(QMouseEvent* move_event)
{
    if ( moving ) 
    {
        QPoint point = inverseWorldMatrix().map(move_event->pos());
        /*moving->moveBy(p.x() - moving_start.x(),
        p.y() - moving_start.y());
        */
        // For now, allow y axis (value) movement, until we deal with moving keyframe times.
        int delta_y = point.y() - moving_start.y();

        moving->moveBy(0, delta_y);
        moving_start = point;
        canvas()->update();
        emit signalKeyframeChanged();
    }

    //if we are moving the last node or first node in
    //a keyframe layout we need to also move the + and -
    //infinity nodes...
    //may be easier to make them unique?
}

void KeyframeEditor::resizeEvent ( QResizeEvent* )
{
    canvas()->resize( contentsRect().width(), contentsRect().height() );
    canvas()->update();
}




