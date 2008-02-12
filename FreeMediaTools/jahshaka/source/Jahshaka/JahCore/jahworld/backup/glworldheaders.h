/*******************************************************************************
**
** The header file for the Jahshaka glworld module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLWORLDHEADERS_H
#define GLWORLDHEADERS_H

#include "coreeffects.h"

///////////////////////////////////////////
// linked list to store layers
/*
class NewLayerList
{
public:
    NewLayerList( int layer, const QString& name )
    {
        thelayer = new JahLayer;
        thelayername=name;
        thelayernumber=layer;

        hasEffects = false;
        numberOfEffects = 0;

    }

    /////////////////////////////////////////////////////
    //for the layer
    QString         name()   const  { return thelayername;  }
    int             layer()  const  { return thelayernumber;  }

    JahLayer        *thelayer;

    /////////////////////////////////////////////////////
    //for the layer effects

    //1. add layer effect

	//we should set the effect name and number at the same time?

    bool addEffectLayer(FxCategory theEffect, bool status = true, int pluginid = 0)
    {
        //create the fx list if this is the first effect
        if (!hasEffects)
        {
            layereffects = new QPtrList<FxLayerList>;
            layereffects->setAutoDelete( TRUE );     // delete items when they are removed
			
			hasEffects=true;
		}

        layereffects->append( new FxLayerList(numberOfEffects) );
        layereffects->at(numberOfEffects)->fxlayer->objtype = theEffect;
        layereffects->at(numberOfEffects)->fxlayer->layerStatus = status;

		//so the effect knows what it snumber is for indexing?
        layereffects->at(numberOfEffects)->fxlayer->effectnumber = numberOfEffects;

		//this is for plugin effects
        layereffects->at(numberOfEffects)->fxlayer->pluginnumber = pluginid;

        numberOfEffects++;

        return true;
    }


	inline bool layerHasFx()
    {
		return hasEffects;
	}

	inline int getNumberOfEffects()
    {
		return numberOfEffects;
	}

	inline bool fxLayerStatus(int fxl)
    {
		return layereffects->at(fxl)->fxlayer->layerStatus;
	}

	inline int fxPluginNumber(int fxl)
    {
		return layereffects->at(fxl)->fxlayer->pluginnumber;
	}

	inline  void setFxLayerStatus(int fxl, bool status)
    {
		layereffects->at(fxl)->fxlayer->layerStatus = status;
	}

	inline FxCategory fxLayerCategory::TYPE(int fxl)
    {
		if ( fxl<0 || fxl>numberOfEffects)
			return NOFX;

		if (layereffects->at(fxl))
			return layereffects->at(fxl)->fxlayer->objtype;
		else
			return NOFX;
	
	}

	inline bool getLayerStatus(void)
    {
		return thelayer->layerStatus;
	}

    //2. turn effect on/off
    //3. remove layer effect

private:

    //for the layers
    QString         thelayername;
    int             thelayernumber;

    //for the linked list for dynamic effects for layers
    QPtrList<FxLayerList> *layereffects;

    bool hasEffects;
    int numberOfEffects;

};



///////////////////////////////////////////////
//old array to sort layer rendering on the fly

struct worldSort
{
    int       laynum;
    GLfloat    depth;
};
*/
#endif // GLWORLDHEADERS_H




















































