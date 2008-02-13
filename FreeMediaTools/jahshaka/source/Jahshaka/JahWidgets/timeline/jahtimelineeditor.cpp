/*******************************************************************************
**
** The source file for the Jahshaka timeline object
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahtimelineeditor.h"

///////////////////////////////////////////////////////////////////
//need to create a class like this one that stores the video boxes

EditItem::EditItem( assetData full, QCanvas *canvas, int layer, int x1, int y1, int x2, int y2 )
        : QCanvasRectangle( x1 ,y1, x2, y2, canvas ) 
{
    itemdata = full;
    itemlayer = layer;
}


///////////////////////////////////////////////////////////////////
//the main editeditor class code starts here

EditEditor::EditEditor( QCanvas* c, QWidget* parent, const char* name, WFlags f) 
        : QCanvasView(c,parent,name,f) 
{
    jtrace = JahTrace::getInstance();	//set up tracer

    height=1;   //what happens if this goes over the max value for a int? crash!!!

    bordercreated = FALSE;
    move = FALSE;
}



void EditEditor::contentsMousePressEvent(QMouseEvent* e) 
{
    QCanvasItemList l=canvas()->collisions(e->pos());
	qDebug("contents mouse pressed");

    int loopval=0;
    QString loopvald;

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) 
    {
        loopval=(*it)->rtti();

		if ( (*it)->rtti() == clipRTTI2 ) 
        {
			EditItem *item= (EditItem*)(*it);

			if ( item->isSelected() )
			 continue; else qDebug("selected");
		}

	    moving = *it;
	    moving_start = e->pos();

        //debug("loop val:"+loopvald.setNum(loopval));
        if (loopval==clipRTTI2) 
        {
            //debug("clipname:"+jitem->itemdata.clipname);
        }

	    return;
    }

    moving = 0;
}

void EditEditor::cleareditor() 
{
    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();

    for (; it != list.end(); ++it) 
    {
        if ( *it ) 
        {
            delete *it;
        }
    }

    canvas()->update();
    height=1;    //reset this variable when we clear the desktop
}


void EditEditor::clearItem() 
{
    if (  moving ) 
    {
        delete moving;   //delete selectedClip;
        //delete border;
        //delete itemtext;
        border->hide();   
        itemtext->hide();
        
        canvas()->update();
    }
}

void EditEditor::refresh() {
  
  canvas()->update();

}

void EditEditor::setItemtext(QString&) {}

void EditEditor::contentsMouseReleaseEvent(QMouseEvent*) 
{
    //need to release move here?
    //move = 0;
    //moving = 0;
    //canvas()->update();

}

void EditEditor::contentsMouseMoveEvent(QMouseEvent* e) 
{
  jtrace->debug("EditEditor::","Moving mouse");

    QString loopvald;
    //debug("rtti val:"+loopvald.setNum(moving->rtti()));

	if( !moving ) return;

    //7 is for lines in general ie the rest of our interface that we dont want people to move
    if (moving->rtti()==7) return;

    int eposx;

    if ( moving ) 
    {
        moving->moveBy(e->pos().x() - moving_start.x(),      0 );  //limits to move on x only

        //here we move video layers up or down by one video line
        //int ymov = (e->pos().y()); //get current position
        //if ( (ymov / 27)==0 ) {  //25 = layer height
        //    moving->moveBy(0,  e->pos().y() - moving_start.y());	   //limits move to y only
        //}

        moving_start = e->pos();
        canvas()->update();

        eposx = e->pos().x();
        //debug("moving"+loopvald.setNum(eposx));
    }

    //this is if we are scrubbing the clip in the timeline
    if (moving->rtti()==sliderRTTI2) 
    {
        eposx = e->pos().x();

        if (eposx==0 || eposx==1) {} else { eposx=eposx/5; }
        emit updateEditanimframe(eposx); //5 is the magnification level

    }

    //this is if we are moving the clip in the timeline
    if (moving->rtti()==clipRTTI2) 
    {
        eposx = e->pos().x();

        //we grab the x position of the videobar here and use this
        double xpos = moving->x();
        //debug("moving timeline"+loopvald.setNum(eposx));

        if (xpos==0 || xpos==1) {} else { xpos=xpos/5; }
        //now we need to slip the video using the value in eposx
        EditItem *jitem= (EditItem*)(moving);

        emit updateSetinFrames(( int )xpos,jitem->itemlayer);

        //how do we set the activelayer from the clip?
        //we need to store layernumber in the EditItem
        //emit updateEditanimframe(eposx); //5 is the magnification level
        //jitem->itemdata.clipname); itemlayer
    }

//e->pos().y() - moving_start.y()
}

