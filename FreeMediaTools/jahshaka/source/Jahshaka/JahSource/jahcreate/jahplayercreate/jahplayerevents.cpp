/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahplayercreate.h"
//#include "effect.h"
//#include "paint.h"
//#include "character.h"
#include "anime.h"
//#include "edit.h"
//#include "tracker.h"

//this is to pass data down to the modules
void JahControl::spaceballmoved(double x, double y, double z, double a, double b , double c)
{
//switch on module for the return    are we using mode or module?
    switch(module) {

        case ANIMATION : { JahAnimationMod->spaceEvent  ( x,y,z,a,b,c ); break; }
        case PLAYER    : {  break; }
        case DESKTOP   : {  break; }
        case PAINT     : {  break; }
        case EFFECTS   : {  break; }
        case TEXTCG    : {  break; }
        case COLORIZE  : {  break; }
        case EDITING   : {  break; }

        default        : {  break; }
    }

}

//we detect keypresses in Jah above so we have to process them here
void JahControl::processKeyPress(QKeyEvent *k)
{
	// NB: keyboard handling is inconsistent - too many widgets grab the keyboard focus

	// I also suspect that the override of the keyPressEvent on the main window is breaking
	// the 'natural' passing of the keyboard event to the active widget (but fixing this 
	// will lead to other issues relating to 'greedy' widgets which don't handle the 
	// ignore/accept state correctly [such as QTable]).

	// For now, we will mark all events as ignored
	k->ignore( );



	// Only soak up ignored normal keypresses here - everything else is 
	// in the domain of the current module
	if ( !k->isAccepted( ) && k->state( ) == 0 )
	{
		// Now accept it by default
		k->accept( );

		// Process the key press and in the default case, set it back to ignored
	    switch ( tolower(k->ascii()) )
	    {
	    case 'd':      {    jtrace->debug("pressed (d)esktop");
	                        if (module != DESKTOP)
	                        {
	                            changeModeDesktop();
	                        }
	                        break;
	                }

	        case 'a':  {    jtrace->debug("pressed (a)animation");
	                        if (module != ANIMATION)
	                        {
	                            changeModeAnimation();
	                        }
	                        break;
	                }

	     


	        case 'n':  {    jtrace->debug("pressed (n)network");
	                        if (module != NETWORK)
	                        {
	                            //changeModeNetworkCom();
	                        }
	                        break;
	                }





			case 'h':  {
						   hidetheUI();
						   break;
					}

	        case ' ':  {
	                        jtrace->debug("pressed space");
	                        static bool playmode = false;

	                        if (module == DESKTOP)
	                        {
	                            changeModePlayer();
	                        }

	                        if (module == PLAYER)
	                        {
	                            playmode = !playmode;

	                            if (playmode)
	                                JahPlayerMod->LoopForward();
	                            else
	                                JahPlayerMod->Stop();

	                        }

	                        if (module == ANIMATION)
	                            JahAnimationMod->startanimation();


	                        break;
	                }

	        default :  {
							// All modules should be listed here
							if ( module == ANIMATION )
	                            JahAnimationMod->keyPressEvent( k );
							else
								k->ignore( );
							break;
					   }
	    }
	}
	else if ( !k->isAccepted( ) )
	{
		// All modules should be listed here too and they should
		// all handle the ignore case correctly...
		if ( module == ANIMATION )
			JahAnimationMod->keyPressEvent( k );
	}

}

void JahControl::processKeyRelease(QKeyEvent *k)
{
	// All modules should be listed here too
	if ( module == ANIMATION )
		JahAnimationMod->keyReleaseEvent( k );

}

