/*******************************************************************************
**
** The source file for the Jahshaka Jahroutines module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahcreate.h"
#include "effect.h"
#include "paint.h"
#include "character.h"
#include "anime.h"
#include "edit.h"
#include "tracker.h"
#include "MainPlayer.h"

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

	// Will give the editor the highest priority when its active (since it correctly
	// handles key presses - ignoring them if they don't match)
	if ( module == EDITING )
		JahEditingMod->keyPressEvent( k );

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

	        case 'e':  {    jtrace->debug("pressed (e)dit");
	                        if (module != EDITING)
	                        {
	                            changeModeEditing();
	                        }
	                        break;
	                }

	        case 'p':  {    jtrace->debug("pressed (p)paint");
	                        if (module != PAINT)
	                        {
	                            changeModePaint();
	                        }
	                        break;
	                }

	        case 't':  {    jtrace->debug("pressed (t)ext");
	                        if (module != TEXTCG)
	                        {
	                            changeModeText();
	                        }
	                        break;
	                }

	        case 'c':  {    jtrace->debug("pressed (c)olor");
	                        if (module != COLORIZE)
	                        {
	                            changeModeColorize();
	                        }
	                        break;
	                }

	        case 'k':  {    jtrace->debug("pressed (k)eyer");
	                        if (module != KEYER)
	                        {
	                            changeModeKeyer();
	                        }
	                        break;
	                }

#ifdef USE_NETWORK
	        case 'n':  {    jtrace->debug("pressed (n)network");
	                        if (module != NETWORK)
	                        {
	                            changeModeNetworkCom();
	                        }
	                        break;
	                }
#endif
	        case 'f':  {    jtrace->debug("pressed e(f)fects");
	                        if (module != EFFECTS)
	                        {
	                            changeModeEffects();
	                        }
	                        break;
	                }

			case 'g':  {
						   GetDesktop( );
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

	                        if (module == EFFECTS)
	                            JahEffectsMod->startanimation();

	                        if (module == EDITING)
	                            JahEditingMod->startanimation();

	                        if (module == PAINT)
	                            JahPaintMod->startanimation();

	                        break;
	                }

	        default :  {
							// All modules should be listed here
							if ( module == ANIMATION )
	                            JahAnimationMod->keyPressEvent( k );
							else if ( module == EFFECTS )
	                            JahEffectsMod->keyPressEvent( k );
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
		else if ( module == EFFECTS )
			JahEffectsMod->keyPressEvent( k );
	}

}

void JahControl::processKeyRelease(QKeyEvent *k)
{
	// All modules should be listed here too
	if ( module == ANIMATION )
		JahAnimationMod->keyReleaseEvent( k );
	else if ( module == EFFECTS )
		JahEffectsMod->keyReleaseEvent( k );
	else if ( module == EDITING )
		JahEditingMod->keyReleaseEvent( k );
}

