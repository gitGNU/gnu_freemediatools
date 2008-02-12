/*******************************************************************************
**
** The source file for the Jahshaka jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahnodeeditor.h"

NodeEditor::NodeEditor(	QCanvas* c, QWidget* parent, const char* name, WFlags f) :
    QCanvasView(c,parent,name,f)
{
    //itemtext = new QCanvasText( c);
    lastselected = 0;

}

void NodeEditor::clear()
{
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();
    for (; it != list.end(); ++it) {
	if ( *it )
	    delete *it;
    }
    lastselected = 0;
}

void NodeEditor::contentsMousePressEvent(QMouseEvent* e)
{
//debug("mouse pressed");

    QPoint p = inverseWorldMatrix().map(e->pos());

    QCanvasItemList l=canvas()->collisions(p);

    QString convert;

    moving = 0;
    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) 
    {

        if ( (*it)->rtti() == nodeRTTI ) {
            NodeItem *item= (NodeItem*)(*it);
            // debug(item->nodename);
            emit nodeClicked( item->layernumber );
            QString conv;
            // debug ("mouse press nodeitem just emitted layernumber " + conv.setNum( item->layernumber ) );
            // If its a node, bail out after here (ignore non-node item, e.g. the item text etc)
	        moving = (NodeItem*)(*it);
            moving_start = p;
            moving->setSelected( true );
            
            if ( lastselected != 0 && lastselected != moving )
                lastselected->setSelected( false );
            lastselected = moving;
            continue;
        }   
    }
    canvas()->update(); // to redraw (de)selected nodes.
}
void NodeEditor::clearSelection() 
{   
    if ( lastselected != 0 )
        lastselected->setSelected( false );
    lastselected = 0;
    canvas()->update();
}

void NodeEditor::contentsMouseMoveEvent(QMouseEvent* e)
{

    if ( moving )
	{

        QPoint p = inverseWorldMatrix().map(e->pos());
        moving->moveBy(p.x() - moving_start.x(),
                p.y() - moving_start.y());
       // moving->itemtext->moveBy(p.x() - moving_start.x(),
         //       p.y() - moving_start.y());

        moving_start = p;
        canvas()->update();
    }

	//if we are moving the last node or first node in
	//a keyframe layout we need to also move the + and -
	//infinity nodes...
	//may be easier to make them unique?
}
/*
void NodeEditor::contentsMouseReleaseEvent(QMouseEvent* e)
{
        debug("released");

        //QPoint p = inverseWorldMatrix().map(e->pos());
        //(p.x(),p.y());
        //moving_start = p;
        //canvas()->update();


	//if we are moving the last node or first node in
	//a keyframe layout we need to also move the + and -
	//infinity nodes...
	//may be easier to make them unique?
}
*/
