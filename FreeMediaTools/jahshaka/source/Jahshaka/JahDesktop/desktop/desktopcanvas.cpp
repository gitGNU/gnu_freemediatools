/*******************************************************************************
**
** The source file for the Jahshaka desktop canvas file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "desktopcanvas.h"
#include "jahpreferences.h"
#include "desktop.h"
#include <qcursor.h>
#include "filmstrip.h"
#include "imageitem.h"
#include <cmath>
#include <iostream>

static const char *hand_cursor_xpm[]={
"16 16 3 1",
"# c #000000",
". c None",
"a c #ffffff",
".......##.......",
"...##.#aa###....",
"..#aa##aa#aa#...",
"..#aa##aa#aa#.#.",
"...#aa#aa#aa##a#",
"...#aa#aa#aa#aa#",
".##.#aaaaaaa#aa#",
"#aa##aaaaaaaaaa#",
"#aaa#aaaaaaaaa#.",
".#aaaaaaaaaaaa#.",
"..#aaaaaaaaaaa#.",
"..#aaaaaaaaaa#..",
"...#aaaaaaaaa#..",
"....#aaaaaaa#...",
".....#aaaaaa#...",
".....#aaaaaa#..."};




FigureEditor::FigureEditor( QCanvas* c, QWidget* parent,
                            const char* name, WFlags f ) 
    : QCanvasView( c,parent,name,f ), 
      playercontrolsmode( FigureEditor::AlwaysHide )
{

    ourexchange = 0;

    height=1;   //what happens if this goes over the max value for a int? crash!!!
    //ImageItem globalitem;

    bordercreated = FALSE;
    move = FALSE;
    scrub = 0;
    resize = 0;
    border = 0; itemtext = 0; itemtextshadow = 0;

    globalitem = 0; // at start nothing is selected.
    clipplaying = false;

    JahPrefs& jprefs = JahPrefs::getInstance();    
    QString location = jprefs.getBasePath( ).data();
    
    //playerpixmap = new QCanvasPixmapArray(QString( jprefs.getBasePath( ).c_str( ) ) + "Pixmaps/desktop/desktopplay.png");
    playerpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktopplay.png");
    stoppixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktopstop.png");
    backpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktopback.png");
    rewindpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktoprreverse.png");
    forwardpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktopforward.png");
    fforwardpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/desktopfforward.png");

    backgroundpixmap = new QCanvasPixmapArray(location + "Pixmaps/desktop/jah-bighead-canvasbg.png");



    playercontrols = new PlayControlSprite( playerpixmap, canvas() ); 
    rewindcontrol = new RewindControlSprite( rewindpixmap, canvas() ); 
    backcontrol = new BackControlSprite( backpixmap, canvas() ); 
    forwardcontrol = new ForwardControlSprite( forwardpixmap, canvas() ); 
    fforwardcontrol = new FForwardControlSprite( fforwardpixmap, canvas() ); 
    
    backgroundsprite = 0;

    showHidePlayerControls( false );

    // For smooth updates, we set this to the same rate as we call the slot to play clip frames.
    // Note, assets have no clue about their original framerate, we can only choose a typical value here.
    {
        assetData dummy;
        canvas()->setUpdatePeriod( 40 ); // was 100, which was slow but easier on the cpu.
    }

    // default options
    setSelectionAllowed ( true );
    setResizingAllowed ( true );
    setMovingAllowed( true );
    setPlayerControlsMode ( FigureEditor::Reveal );
    setItemBorderEnabled ( true );
    setFilmStripAllowed ( true );
}

FigureEditor::~FigureEditor()
{
    // QtCanvas deletes the canvas items for us -- 
    // we only need to worry about extra memory we have used.
    delete ourexchange;
    delete playerpixmap;
    delete stoppixmap;
    delete backpixmap;
    delete rewindpixmap;
    delete forwardpixmap;
    delete fforwardpixmap;
    delete backgroundpixmap;
}

void FigureEditor::setPlayerControlsMode ( FigureEditor::PlayerControlsMode mode )
{ 
    playercontrolsmode = mode; 
}

void FigureEditor::showBackgroundSprite ( bool on )
{

  if (backgroundsprite)
    {
    if ( !backgroundsprite->isValid() )
      {
	if (backgroundpixmap->isValid())
	  {
	    backgroundsprite = new BackgroundSprite( backgroundpixmap, canvas() );
	  }
      }
    }
  else
    {
      backgroundsprite = new BackgroundSprite( backgroundpixmap, canvas() );
    }
    
    if ( on )
    {   
      try {

	if (backgroundsprite->isValid())
	  {
	    backgroundsprite->show();
	    
	    backgroundsprite->move( int ( visibleWidth()/2 - backgroundsprite->width()/2 ) ,
				    int ( visibleHeight()/2 - backgroundsprite->height()/2 ) );
	    backgroundsprite->setZ(-999);
	  }
	else
	  {
	    std::cerr << "WOOPS... jah did not display" << endl;
	  }
      } catch (...)
	{
	  std::cerr << "WOOPS... jah did not display" << endl;
	}
      
    }
    else
      {
	if (backgroundpixmap->isValid())
	  {
	    backgroundsprite->hide();
	  }
      }
   
    //update();
}

void FigureEditor::resizeEvent( QResizeEvent * e )
{
    QCanvasView::resizeEvent ( e );
    if ( backgroundsprite )
        showBackgroundSprite( true ); // moves the bg sprite to the new center.
}


void FigureEditor::contentsMousePressEvent(QMouseEvent* e)
{

    // creating border anywhere else ie in constructor crashes app...
    // we want to use a better method anyways so aint gonna waste no more time on this
    // ideas are just to draw selection boundary
    // moving->setSelected(1);  // QRect myframe = moving->boundingRect();

    QCanvasItemList l=canvas()->collisions(e->pos());
    assetData itemData;

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    {
        int iRTTI = (*it)->rtti();
        QCanvasItem *canvitem = (*it);

        // If we clicked on a filmstrip image, pretend we clicked on the parent item.
        if ( iRTTI == filmstripRTTI )
        {   
       		moving_start = e->pos();
            FilmStrip *film = (FilmStrip*)(canvitem);
            iRTTI = imageRTTI;  // kinda ugly code...
            canvitem = (QCanvasItem*)film->imageItem();
        }

        // do the same for the film frames
        if ( iRTTI == filmframeRTTI )
        {   
            FilmFrame *film = (FilmFrame*)(canvitem);
            iRTTI = imageRTTI;  // kinda ugly code...
            canvitem = (QCanvasItem*)film->imageItem();            
        }


        if ( iRTTI == imageRTTI )
        {
            ImageItem *item= (ImageItem*)(canvitem);
            if ( item->filmStrip() != 0 )
                QApplication::setOverrideCursor( QCursor( QPixmap(hand_cursor_xpm) ) );

            // Previously there was a hit() test here, to check for clicking transparant pixels, 
            // but now disabled so we can click anywhere in items rectangle.

            /* // DISABLED PREVIOUS hit check
            if ( !item->hit( e->pos() ) || !selectionallowed )
            {
                continue;
            }
            */
            if ( !selectionallowed )
                continue;

            move = 1;

            bool sameclip = false; // want to check if the same clip is clicked again.
            if ( globalitem == (ImageItem*)(canvitem) )
				sameclip = true;

            globalitem = (ImageItem*)(canvitem);

            imagetext = item->getText();

            selectedClip = item->getClip();
            selectedClip.xpos = item->x();
            selectedClip.ypos = item->y();

            itemData = item->getClip();
			AssetTables::getInstance( )->setSelectedAsset( itemData );
            
            // If its a movie clip, move player controls to this item, and call the reveal slot.
            if ( itemData.theCategory == VideoCategory::CLIP || itemData.theCategory == VideoCategory::MEDIA )
            {	
                if ( !sameclip )
                {   
                    playerControlsMoveTo( int( -6 + globalitem->x()+ globalitem->width()/2.0 ), int( globalitem->y() + globalitem->height() - playercontrols->height() ) ); 
                    showHidePlayerControls( true );
                    playercontrols->setSequence( playerpixmap );
                    revealPlayerControls(); //assumes globalitem is set (which it was, above), and start position ok.
                    clipplaying = false;
                    delete ourexchange;
                    ourexchange = 0;
                }
            }
            else
            {
                showHidePlayerControls( false ); // hide controls. NB they could still be 'revealing' from another item's click.
                playercontrols->setSequence( playerpixmap ); 
                clipplaying = false; 
                //canvas()->update();
            }
            
        } // TODO should these be ahead of imageRTTI check, since they are 'above' other items?
        else if ( iRTTI == playcontrolRTTI  )  
        {    
            playControlClicked();
            break;
        }
        else if ( iRTTI == forwardcontrolRTTI  )  
        {    
            forwardControlClicked();
            break;
        }
        else if ( iRTTI == backcontrolRTTI  )  
        {    
            backControlClicked();
            break;
        }
        else if ( iRTTI == rewindcontrolRTTI  )  
        {    
            rewindControlClicked();
            break;
        }
        else if ( iRTTI == fforwardcontrolRTTI  )  
        {    
            fForwardControlClicked();
            break;
        }

        if ( iRTTI == imageRTTI ) // else we get 'Unknown' type when clicking border etc
        {
            QColor bordercolor;
            QString thetype;
            ImageItem *item= (ImageItem*)(canvitem);
            
            if (!bordercreated)
            {
                
                border = new QCanvasRectangle( 0,0, item->dieWidth(), item->dieHeight(),canvas());
                
                //its a messy hack and needs to be cleaned up
                switch ( itemData.theCategory )
                {
                case VideoCategory::IMAGE    : { thetype="Image :"; bordercolor.setRgb(0,50,200); break; }
                case VideoCategory::CLIP     : { thetype="Clip  :";bordercolor.setRgb(0,200,50); break; }
                case VideoCategory::AUDIO    : { thetype="Audio :";bordercolor.setRgb(200,50,0); break; }
                case VideoCategory::MEDIA    : { thetype="Media :";bordercolor.setRgb(200,0,50); break; }
                    //case KEY      : { thetype="Key   :";bordercolor.setRgb(0,50,200); break; }
                    //case CLIPKEY  : { thetype="ClipKey :";bordercolor.setRgb(0,200,50); break; }
                    
                default       : 
                { 
                    thetype="Unknown"; bordercolor.setRgb(200,0,50); 
                    break; 
                }
                }
                border->setBrush( bordercolor );
                border->setPen( QPen( bordercolor, 6) );
                border->setZ(1);
                
                itemtextshadow = new QCanvasText( canvas());
                itemtextshadow->setColor( QColor(0,0,0) );
                itemtextshadow->setText(thetype+imagetext);
                itemtextshadow->setZ(1);
                itemtext = new QCanvasText( canvas());
                itemtext->setColor( QColor(200,200,200) );
                itemtext->setText(thetype+imagetext);
                itemtext->setZ(1);
                
                bordercreated = TRUE;
            }
            
            
            
            //use a switch here
            //dont think we need to deal with keys at the desktop level
            switch ( itemData.theCategory )
            {
            case VideoCategory::IMAGE    : { thetype="Image :"; bordercolor.setRgb(0,50,200); break; }
            case VideoCategory::CLIP     : { thetype="Clip  :";bordercolor.setRgb(0,200,50); break; }
            case VideoCategory::AUDIO    : { thetype="Audio :";bordercolor.setRgb(200,50,0); break; }
            case VideoCategory::MEDIA    : { thetype="Media :";bordercolor.setRgb(200,0,50); break; }
                //case KEY      : { thetype="Key   :";bordercolor.setRgb(0,50,200); break; }
                //case CLIPKEY  : { thetype="ClipKey :";bordercolor.setRgb(0,200,50); break; }
                
            default       : { thetype="Unknown"; bordercolor.setRgb(200,0,50); break; }
                
            }
            
            itemtextshadow->setText(thetype+imagetext);
            itemtext->setText(thetype+imagetext);
            
            border->setBrush( bordercolor );
            border->setPen( QPen( bordercolor, 6) );
            
            // If we click the upper part of the image, allow click-drag as usual, 
            // but if we click on lower part (the progress bar) do scrubbing.
            int clicky  = mapFromParent( e->pos() ).y();
            int itemy = int( (canvitem)->y() );
            int ycoord =  clicky - itemy; // i.e. top will be 0.
            int clickx = mapFromParent( e->pos() ).x();
            int xcoord = int( clickx - (canvitem)->x() );
            
            moving_start = e->pos(); // keep track of mouse move point in either case.
            
            //moving is set to canvas item referenced by *it
            moving = canvitem;
            
            // Check region of image clicked, e.g. lower (progress bar), lower-right corner, or main part of image.
            // Note, use of diewidth, I couldnt find a way around that. 
            int progbarspace = 18;
           
            // dont allow resize etc if filmstrip showing.
            if ( item->filmStrip() != 0 )
            {    
                move = true;
                scrub = 0;
                resize = 0;
            }
            else if ( resizeallowed && 
                      item->width() - item->dieWidthGap() - xcoord < progbarspace && 
                      (*it)->boundingRect().height() + item->dieHeightGap() - ycoord < progbarspace ) // lower right corner
            { 
                QApplication::setOverrideCursor( QCursor(Qt::SizeFDiagCursor) );
                move = false;
                scrub = 0;
                resize = canvitem;
                showHidePlayerControls( false );
            }
            else if ( (itemData.theCategory == VideoCategory::CLIP || itemData.theCategory == VideoCategory::MEDIA) &&
                      (*it)->boundingRect().height() + item->dieHeightGap() - ycoord < progbarspace ) // progress bar region clicked
            { 
                QApplication::setOverrideCursor( QCursor(Qt::SizeHorCursor) );
                move = false;
                resize = 0;
                scrub = canvitem;
                clipplaying = false; // clip may have been playing already
                playercontrols->setSequence( playerpixmap ); 
            }
            else if ( movingallowed )// any other region on item, do normal move.
            {  
                move = true;
                scrub = 0;
                resize = 0;
            }
            else // do nothing.
            {   
                move = false;
                resize = 0;
                scrub = 0;
            }
         

            //now we position border and bring selected item and
            //border to the front of others on the canvas
            border->setX( moving->x() );
            border->setY( moving->y() );
            border->setSize( item->dieWidth(), item->dieHeight() );
            height++; border->setZ(height);
            
            itemtextshadow->setX( moving->x() -1 );
            itemtextshadow->setY( moving->y() -1 );
            itemtext->setX( moving->x() );
            itemtext->setY( moving->y() );
            
            // move to front
            if ( item->filmStrip() == 0 )
            {   
                //border->setZ(height); // BAZ NEW
                height++; moving->setZ(height);
                height++; itemtextshadow->setZ(height);
                height++; itemtext->setZ(height);
                if ( borderenabled )
                {	
                    border->show();
                    itemtextshadow->show();
                    itemtext->show();
                }
                else
                {  
                    border->hide();
                    itemtextshadow->hide();
                    itemtext->hide();
                }
            }
            else
            {
                // NB moving item z not changed. TODO: how to get it back on top when filmstrip gone?
                height++; border->setZ( height );
                height++; item->filmStrip()->setBlackBackZ(height);

                height++; item->filmStrip()->setFramesZ(height);
                  
                height++; itemtextshadow->setZ(height);
			    height++; itemtext->setZ(height);
                border->hide();
                if ( borderenabled ) // (decided to no longer show border for filmstrip, just text).
                {         
                    itemtextshadow->show();
                    itemtext->show();
                }
                else
                {   
                    itemtextshadow->hide();
                    itemtext->hide();
                }
            }
            
            //canvas()->update();
            
            return;
        }
    }
    moving = 0;
    
    //debug("clicked on desktop itself not a item");
}

QString FigureEditor::categoryName( VideoCategory::TYPE cat )
{
    QString thetype;
    switch ( cat )
	{
	    case VideoCategory::IMAGE    : { thetype="Image :"; break; }
		case VideoCategory::CLIP     : { thetype="Clip  :"; break; }
		case VideoCategory::AUDIO    : { thetype="Audio :"; break; }
		case VideoCategory::MEDIA    : { thetype="Media :"; break; }
		default       : { thetype="Unknown"; break; }
		}
    return thetype;
}

void FigureEditor::contentsMouseDoubleClickEvent ( QMouseEvent*e )
{  
    // Currently, if its a CLIP/MEDIA, we play it, or expand it in a filmstip if that feature
    // is enabled. 
    // Note: a little sneaky, we treat the same as playcontrol click, and assume
    // the item/globalitem is set up.

    // We do nothing if the item is not a playable clip.

    QCanvasItemList l=canvas()->collisions(e->pos());
    assetData itemData;

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    {
        // TODO double click on filmstrip (NB cant assume globalitem will be the right one!)
        if ( (*it)->rtti() == imageRTTI ) // (could double click on a playcontrol)
        {
            if ( globalitem == (ImageItem*)(*it) ) 
                // BAZ TEST FILMSTRIP INSTEAD          playControlClicked();
            {
                assetData adata = globalitem->getClip();
                if ( ( adata.theCategory == VideoCategory::CLIP || adata.theCategory == VideoCategory::MEDIA) )
                {
                    if ( !filmstripallowed || !movingallowed )
                        playControlClicked();
                    else if ( globalitem->filmStrip() == 0 )
                    {   // TODO try to use assetexchange in filmstrip? I assume we must close our asset exchange stream
                        // here and let filmstrip do its own? I get exception when trying this.
                        /*delete ourexchange;
                        ourexchange = 0;
                        clipplaying = false;
                        */
                        if ( clipplaying )
                        {	clipplaying = false;
                            playercontrols->setSequence( playerpixmap );
                        }

						createFilmStrip(visibleWidth());
                    }
                    else
						deleteFilmStrip();
                }
                
                break;
            }
        }
        else if ( (*it)->rtti() == filmframeRTTI )
        {   // treat same as double clicking the imageitem it, using normal globalitem assumption...
            FilmFrame *frame = (FilmFrame*)(*it);
            advanceClipFrame ( -globalitem->currentFrameNumber() + frame->number(), false );

            if ( globalitem->filmStrip() != 0 )
				deleteFilmStrip();
            break;
        }
    }
}

void FigureEditor::createFilmStrip(int stripWidth)
{
    globalitem->deleteFilmStrip();
    globalitem->createFilmStrip(stripWidth);
    globalitem->filmStrip()->setBlackBackZ(++height);
	if(border != NULL)
	{
		border->setZ(++height);
		border->hide(); // decided not to show border.
	}

    globalitem->filmStrip()->setFramesZ(++height);

	if(itemtextshadow != NULL)
		itemtextshadow->setZ(++height);

	if(itemtext != NULL)
		itemtext->setZ(++height);

	emit updateFilmstripStatus(globalitem->getClip());
}

void FigureEditor::deleteFilmStrip()
{
    border->show();
	globalitem->deleteFilmStrip();
	globalitem->setZ(++height);
	if(itemtextshadow)
		itemtextshadow->setZ(++height);
	if(itemtext)
		itemtext->setZ(++height);

	emit updateFilmstripStatus(globalitem->getClip());
}

void FigureEditor::clear()
{
    //debug("clearing the list");
    move = 0;
    moving = 0;
    resize = 0;
    scrub = 0;

    QCanvasItemList list = canvas()->allItems();
    QCanvasItemList::Iterator it = list.begin();

    for (; it != list.end(); ++it) 
	{    // ( dont clear controls. Change if required ).
        if ( *it && (*it)->rtti() != playcontrolRTTI && (*it)->rtti() != rewindcontrolRTTI &&
            (*it)->rtti() != forwardcontrolRTTI && (*it)->rtti() != backcontrolRTTI &&
             (*it)->rtti() != fforwardcontrolRTTI  && (*it)->rtti() != bgRTTI )  
		{
            (*it)->setAnimated( false );
			(*it)->hide();
			(*it)->setCanvas( 0 );
            delete *it;
        }
    }

    // Tidy up other bits on canvas
    //playercontrols->hide();
    showHidePlayerControls(false );
    clipplaying = false;

    //canvas()->update();
    //update();

    bordercreated = FALSE;
    move = FALSE;

    height=1;    //reset this variable when we clear the desktop

	globalitem = NULL;
}

void FigureEditor::swapItem( ImageItem *item ) 
{
    delete ourexchange;
    ourexchange = NULL;
    globalitem = item;

	if ( item )
    	imagetext = globalitem->getText();
	else
		imagetext = "";
}

void FigureEditor::clearItem() 
{

    if (  moving ) {

		//delete selectedClip;
        delete moving;   
        //delete border;
        //delete itemtext;

        border->hide();  
        itemtextshadow->hide();
		itemtext->hide();
        playercontrols->hide();
		clipplaying = false;
        delete ourexchange;
        ourexchange = 0;

        //canvas()->update();
    }
}

bool FigureEditor::eraseItem(int clipnumber) 
{

    // creating border anywhere else ie in constructor crashes app...
    // we want to use a better method anyways so aint gonna waste no more time on this
    // ideas are just to draw selection boundary
    // moving->setSelected(1);  // QRect myframe = moving->boundingRect();
	//QString rere;
	//debug("removing item from desktop"+rere.setNum(clipnumber));

	bool foundclip = false;

	assetData itemData;
	QCanvasItem* theitem = NULL;
    QCanvasItemList l=canvas()->allItems();

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it)
    {

		if ( (*it)->rtti() == imageRTTI )
		{

			ImageItem *item= (ImageItem*)(*it);

			itemData = item->getClip();

			if (itemData.clipnumber==clipnumber)
			{
				foundclip = true;
				theitem = *it;
			}
		}
    }

	//debug("looped");

	if (foundclip)
	{
		//debug("found clip");
        delete theitem;  
        if ( globalitem == theitem )
            globalitem = 0; // else it could point to this removed item
		//debug("deleted clip");

		if(border)
			border->hide();   
		if(itemtext)
        {	itemtext->hide();
            itemtextshadow->hide();
        }

        // It could have been playing.
        clipplaying = false;
        playercontrols->setSequence( playerpixmap ); 
        showHidePlayerControls( false );

        delete ourexchange;
        ourexchange = 0;
        //canvas()->update();
		//debug("erased and updated");
	}

	return foundclip;
	//debug("removed item from desktop");

}

void FigureEditor::setItemtext(QString& newname) 
{
    if ( !globalitem ) 
        return;
//    debug("setting clipname");  debug(newname);
    globalitem->setText(newname);

    assetData itemData = globalitem->getClip();
    imagetext = globalitem->getText();
    if ( itemtextshadow )
        itemtextshadow->setText( categoryName( itemData.theCategory ) + globalitem->getText() );

    if ( itemtext )
        itemtext->setText( categoryName( itemData.theCategory ) + globalitem->getText() );

    emit itemRenamed( itemData.clipnumber, newname);
}



void FigureEditor::contentsMouseReleaseEvent(QMouseEvent*e) 
{
	if (moving) 
	{
		ImageItem *current = (ImageItem*)(moving); 

		// Check for filmstrip and set velocity
		if ( current->filmStrip() != 0 )
			current->filmStrip()->setVelocity( 0 );

		//update the clips position in the database
		if ( selectedClip.xpos != current->x( ) || selectedClip.ypos != current->y( ) )
		{
			selectedClip.xpos = current->x( );
			selectedClip.ypos = current->y( );
			emit updateClipPosition( selectedClip );
		}

		//need to release move here?
		move = 0;
  
        if ( resize )
        {   ImageItem *item= (ImageItem*)(resize); // ok?
            assetData itemdata;
            itemdata = item->getClip();
            int xdrag = e->pos().x() - moving_start.x();
            int ydrag = e->pos().y() - moving_start.y();
            item->resize( item->width() + xdrag, item->height() + ydrag);
			itemdata.xsize = item->width();
			itemdata.ysize = item->height();
            advanceClipFrame (0, false); // get a re-scaled image.         
            border->setSize( item->dieWidth(), item->dieHeight() );
            if ( itemdata.theCategory == VideoCategory::CLIP || itemdata.theCategory == VideoCategory::MEDIA )
            {   playerControlsMoveTo( int( -6 + item->x()+ item->width()/2.0 ), int( item->y() + item->height() - playercontrols->height() ) ); 
                revealPlayerControls();
                showHidePlayerControls( true );
            }

			if ( selectedClip.xsize != item->width( ) || selectedClip.ysize != item->height( ) )
			{
				selectedClip.xsize = item->width( );
				selectedClip.ysize = item->height( );
				emit updateClipSize( selectedClip );
			}
        }


        scrub = 0; // (scrub and resize also use the 'moving' item pointer).
        resize = 0;
        QApplication::restoreOverrideCursor();
		//moving = 0;
        
		//canvas()->update();
    }

}

void FigureEditor::contentsMouseMoveEvent(QMouseEvent* e) 
{
    if ( move ) 
	{   // was moving

        ImageItem *item= (ImageItem*)(moving); // this is true, 'moving' was set earlier.
        if ( item->filmStrip() == 0 ) 
        {
            //move clip and the background border etc
            moving->moveBy(e->pos().x() - moving_start.x(),
                         e->pos().y() - moving_start.y());

            border->moveBy(e->pos().x() - moving_start.x(),
                         e->pos().y() - moving_start.y());
           //text moveby
            itemtext->moveBy(e->pos().x() - moving_start.x(),
                         e->pos().y() - moving_start.y());
            itemtextshadow->moveBy(e->pos().x() - moving_start.x(),
                         e->pos().y() - moving_start.y());

            //  move the playercontrols too.
            playerControlsMoveBy( e->pos().x() - moving_start.x(), e->pos().y() - moving_start.y() );
        	moving_start = e->pos();
        }
        else if ( item->filmStrip() != 0 ) 
        {
            int movey  = mapFromParent( e->pos() ).y();
	        int dy = int( item->filmStrip()->y() - movey );
            if (  dy > 0 || dy < -item->filmStrip()->height() )
            {
                moving->moveBy( e->pos().x() - moving_start.x(),
                           e->pos().y() - moving_start.y());
                border->moveBy(0, e->pos().y() - moving_start.y());
                itemtext->moveBy(0, e->pos().y() - moving_start.y());
                itemtextshadow->moveBy(0, e->pos().y() - moving_start.y());
                
                //  move the playercontrols too.
                playerControlsMoveBy( 0, e->pos().y() - moving_start.y() );
        		moving_start = e->pos();
            	item->filmStrip()->setVelocity( 0 );
            }
		 	// movement when filmstrip is active is slightly different: x moves drag the strip, 
            // whilst y moves move all the items as usual, but only if the cursor is near top/bottom edge.
			else if ( item->filmStrip()->getVelocity( ) == 0.0 )
			{
            	item->filmStrip()->setVelocity( -1 );
				item->filmStrip( )->velocityMove( );
			}
			else if ( e->pos().x() - moving_start.x() )
			{
				int sign = e->pos().x() > moving_start.x() ? 4 : -4;
            	item->filmStrip()->setVelocity( sign * logf( fabsf( e->pos().x() - moving_start.x() ) ) );
			}
        }

        //canvas()->update();
    }
    else if ( scrub )
    {
	    ImageItem *item= (ImageItem*)(scrub); // ok?
	    int dframe = 0;
	    int xdrag = e->pos().x() - moving_start.x();
        int max_frames = item->getItemData()->getNumFrames();

	    if ( xdrag == 1 || xdrag == -1 || max_frames < 150 )
        {
	      dframe = xdrag;
        }
	    else if ( xdrag > 0 )
        {
	       dframe = int(max_frames / 50.0);
        }
	    else if ( xdrag < 0 )
        {
	        dframe = -1 * int(max_frames / 50.0);
        }

	    moving_start = e->pos();
	    advanceClipFrame( +dframe, false );
    }
    else if ( resize )
    {   
        ImageItem *item= (ImageItem*)(resize); // ok?
        int xdrag = e->pos().x() - moving_start.x();
        int ydrag = e->pos().y() - moving_start.y();
        item->resize( item->width() + xdrag, item->height() + ydrag);

        // To get a rescaled image, we must reload current frame. 
        // If this is inefficient, comment out next line and just do it on mouse release.
        advanceClipFrame (0, false);
        moving_start = e->pos();
        border->setSize( item->dieWidth(), item->dieHeight() );
    }
}

void FigureEditor::mouseStoppedSlot()
{
    //mousemovetimer.stop();
    //printf("mousemovetimer.stop\n\n");
}

int FigureEditor::getDesktopSelect()
{
    return selectedClip.clipnumber;

}

void FigureEditor::hideDecorations()
{
    if ( !bordercreated ) return;
    if ( border!= 0 ) border->hide();
    if ( itemtextshadow != 0 ) itemtextshadow->hide();
    if ( itemtext != 0 ) itemtext->hide();
}

void FigureEditor::playControlClicked()
{
    // Filmstrip is dealt with differently to normal playback
    if ( globalitem->filmStrip() != 0 )
    {   
        globalitem->filmStrip()->moveFramesBy( -globalitem->width() -2, 0 ); // TODO: how to ensure it moves 1 frame?!?
        return;
    }

    if ( clipplaying )
    {	
        clipplaying = false;
        playercontrols->setSequence( playerpixmap ); 
    }
    else
    {	
        clipplaying = true;
        advanceClipFrame( +1, true ); // 'true' to keep playing.
        playercontrols->setSequence( stoppixmap ); // show a stop pixmap.
    }
}

//used to stop playing the desktop clip
//accessed from the module switcher routine via JahDesktop::stopanimation()
void FigureEditor::playControlStop()
{
    if ( clipplaying )
    {	
        clipplaying = false;
        playercontrols->setSequence( playerpixmap ); 
    }
}

//used to start (ony) playing the desktop clip
//accessed from the module switcher routine via JahDesktop::stopanimation()
void FigureEditor::playControlPlay()
{
    if ( !clipplaying )
    {	
        clipplaying = true;
        advanceClipFrame( +1, true ); // 'true' to keep playing.
        playercontrols->setSequence( stoppixmap ); // show a stop pixmap.
    }
}


void FigureEditor::forwardControlClicked()
{
    // Filmstrip is dealt with differently to normal playback
    if ( globalitem->filmStrip() != 0 )
    {   globalitem->filmStrip()->moveFramesBy( -globalitem->width() -2, 0 ); // TODO: how to ensure it moves 1 frame?!?
        return;
    }

    if ( clipplaying )
    {	clipplaying = false;
        playercontrols->setSequence( playerpixmap );	
    }
	
	advanceClipFrame ( +1, false ); // advances one frame.
}

void FigureEditor::backControlClicked()
{
    // Filmstrip is dealt with differently to normal playback
    if ( globalitem->filmStrip() != 0 )
    {   globalitem->filmStrip()->moveFramesBy( globalitem->width() + 4, 0 ); // TODO: how to ensure it moves 1 frame?!?
        return;
    }
    
    if ( clipplaying )
    {	clipplaying = false;
        playercontrols->setSequence( playerpixmap );	
    }

    advanceClipFrame ( -1, false ); // back one frame.
}

void FigureEditor::fForwardControlClicked()
{
    // Filmstrip is dealt with differently to normal playback
    if ( globalitem->filmStrip() != 0 )
    {   globalitem->deleteFilmStrip();
        advanceClipFrame ( -globalitem->currentFrameNumber() + globalitem->maxFrames(), false );
		createFilmStrip(visibleWidth());
        return;
    }

    if ( clipplaying )
    {	clipplaying = false;
        playercontrols->setSequence( playerpixmap );	
    }

    // again, we assume globalitem actually points to the item being expanded... which should be true, but a bit slack.
    advanceClipFrame ( -globalitem->currentFrameNumber() + globalitem->maxFrames(), false );    
}

void FigureEditor::rewindControlClicked()
{
    // Filmstrip is dealt with differently to normal playback
    if ( globalitem->filmStrip() != 0 )
    {   globalitem->deleteFilmStrip();
        advanceClipFrame ( 1 - globalitem->currentFrameNumber(), false ); // -ve the current frame
		createFilmStrip(visibleWidth());
        return;
    }

    if ( clipplaying )
    {	clipplaying = false;
        playercontrols->setSequence( playerpixmap );	
    }

    advanceClipFrame ( 1 - globalitem->currentFrameNumber(), false ); // -ve the current frame
}

void FigureEditor::advanceClipFrame( int dframe, bool keepplaying )
{
    // TODO: communicate with the main 'player' widget, so the frames showing are in synch.
    //debug("play control clicked");

    if ( !globalitem )
        return;
    if ( !globalitem->isVisible() )
        return;

    assetData itemData;
    //  assetExchange exchange;
    if ( !ourexchange )
        ourexchange = new assetExchange;

    QString imagetext;
    // TODO: what if globalitem not valid now (deleted etc?).
    imagetext = globalitem->getText();
    itemData = globalitem->getClip();

    QString frames;
    frames.setNum( itemData.getNumFrames() );
    //debug( frames + " max frames" );
	
    QImage theimage; 
    int current_frame = globalitem->currentFrameNumber();
    int max_frame = itemData.getMaxFrames();

    if ( dframe == 0 || current_frame <= max_frame )
    {	
        // Normally we do exchange.getImage, but since we are reading frames continuously we
        // use getStreamImage instead. Is it efficient enough / used correctly here? NB there 
        // is no open/close stream.
        int next_frame = current_frame + dframe;

        if (  next_frame <= itemData.getMaxFrames() && 
              next_frame >= globalitem->getClip( ).startframe )
        {
            globalitem->setCurrentFrameNumber(next_frame);       
        }
        else
        {
            // do nothing, leave clip on current (start or end) frame.
        }

        if ( clipplaying && next_frame > max_frame )
        {
            globalitem->setCurrentFrameNumber( globalitem->getClip( ).startframe ); // loop around
        }

        int frame_offset = next_frame - globalitem->getClip( ).startframe + 1;
        ourexchange->getStreamImagePtr( itemData, theimage, frame_offset );

        if ( theimage.isNull( ) ) 
        {
            theimage = ourexchange->getImage( itemData, 0 );
        }
        
        globalitem->setImage( theimage );
        itemData.frameOffset = globalitem->currentFrameNumber();
        emit signalUpdateFrameOffset( itemData );
    }

    if ( keepplaying && clipplaying ) // keep playing
    	QTimer::singleShot( 40, this, SLOT( advanceClipFrame() ) ); // was 
    
    canvas()->update();
    
    //debug( imagetext );
}


void FigureEditor::revealPlayerControls()
{
    if ( !globalitem )
        return;

    int targety = int( globalitem->y() + globalitem->height() + 2 );  // where we want to be.
    int step = 0;
    
    int dist = int( targety - playercontrols->y() );

    if ( !playercontrolsmode == FigureEditor::Reveal )
        step = dist;
    else if ( dist > 20 ) step = 5; // you can tweak these pixel speeds if you like
    else if ( dist > 5 )
        step = 2;
    else if ( dist >0 )
        step = 1;
	
    //added a little accelerator here
    step = step *3;

    if ( playercontrols->y() < targety )
    {      
        playerControlsMoveBy( 0, step );

        playercontrols->setZ ( globalitem->z() ); 
        rewindcontrol->setZ ( globalitem->z() ); 
        backcontrol->setZ ( globalitem->z() ); 
        forwardcontrol->setZ ( globalitem->z() ); 
        fforwardcontrol->setZ ( globalitem->z() ); 

        QTimer::singleShot( 60, this, SLOT( revealPlayerControls() ) );
        //canvas()->update();
	}
}

void FigureEditor::playerControlsMoveBy( int x, int y )
{
    playercontrols->moveBy( x, y );
    rewindcontrol->moveBy( x, y);
    backcontrol->moveBy( x, y );
    forwardcontrol->moveBy( x, y );
    fforwardcontrol->moveBy( x, y );
}

void FigureEditor::playerControlsMoveTo( int x, int y )
{
    playercontrols->move( x, y );
    rewindcontrol->move( x - rewindcontrol->width() - backcontrol->width(), y );
    backcontrol->move( x - backcontrol->width(), y );
    forwardcontrol->move( x + playercontrols->width(), y );
    fforwardcontrol->move( x + playercontrols->width() + forwardcontrol->width(), y );
}

void FigureEditor::showHidePlayerControls( bool show )
{
    if ( playercontrolsmode == FigureEditor::AlwaysHide )
        show = false;

    if ( show )
    {
            playercontrols->show(); 
            rewindcontrol->show(); 
            backcontrol->show(); 
            forwardcontrol->show(); 
            fforwardcontrol->show(); 
    }
    else
    {
            playercontrols->hide(); 
            rewindcontrol->hide(); 
            backcontrol->hide(); 
            forwardcontrol->hide(); 
            fforwardcontrol->hide(); 
    }
}

void FigureEditor::bringToFront( ImageItem* item )
{
    height++; item->setZ(height);
}

PlayControlSprite::PlayControlSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}

RewindControlSprite::RewindControlSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}

BackControlSprite::BackControlSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}

ForwardControlSprite::ForwardControlSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}

FForwardControlSprite::FForwardControlSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}

BackgroundSprite::BackgroundSprite(  QCanvasPixmapArray * a, QCanvas * canvas )
        : QCanvasSprite( a, canvas )
{
  // (nothing extra needed)
}



// Setting setSelectionAllowed (false) prevents any interaction at all.
void FigureEditor::setSelectionAllowed ( bool enable )
{
  selectionallowed = enable; 
};

void FigureEditor::setResizingAllowed ( bool enable )
{
  resizeallowed = enable; 
};


void FigureEditor::setMovingAllowed( bool enable )
{ 
  movingallowed = enable; 
};

void FigureEditor::setItemBorderEnabled ( bool enable )
{
  borderenabled = enable;
};

void FigureEditor::setFilmStripAllowed( bool enable ) { 
  filmstripallowed = enable;
 };

bool FigureEditor::filmStripAllowed()
{ 
  return filmstripallowed; 
};


void FigureEditor::styleChange ( QStyle & )
{
  if (canvas() ) 
    canvas()->setBackgroundColor( qApp->palette().color( QPalette::Active, QColorGroup::Mid ));
};

  int PlayControlSprite::rtti () const { return playcontrolRTTI; }

  int RewindControlSprite::rtti () const { return rewindcontrolRTTI; }


  int BackControlSprite::rtti () const { return backcontrolRTTI; }

  int ForwardControlSprite::rtti () const { return forwardcontrolRTTI; }

  int FForwardControlSprite::rtti () const 
{ return fforwardcontrolRTTI; }


  int BackgroundSprite::rtti () const { return bgRTTI; }


  bool FigureEditor::clipIsPlaying() { return clipplaying; } ;
