/*******************************************************************************
**
** The header file for the Jahshaka glworld module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworldlists.h"

///////////////////////////////////////////
// linked list to store selected layers
// updated frequently
// its a mess because we store layer numbers in reverse according 
// to the position in the list
// ie layer 4 is the 4th layer etc
// but qt stores it the other way so 4 = 0 if there are 4 layers


ActiveLayerList::ActiveLayerList(  )
{ 
    numLayers   = 0;
    totalLayers = 0;

    ///////////////////////////////////////////////
    // the linked list is here
    activelayerlist = new QPtrList<int>;
    activelayerlist->setAutoDelete( TRUE );     // delete items when they are removed

}

int             ActiveLayerList::getNumLayers()                      { return numLayers;  }

void            ActiveLayerList::setTotalLayers( int lay)                 { totalLayers = lay;  }

int ActiveLayerList::getFinalSelection(void)
{ 
    int layer = numLayers -1;
    int lay;

    if (activelayerlist->at(layer))
    {
        lay = *activelayerlist->at(layer);
        return  ( totalLayers - lay )-1;
    }
    else
    {
        return 0;
    }
}

int ActiveLayerList::getFinalSelection(int lay)
{ 
    return  ( totalLayers - lay )-1;
}


int ActiveLayerList::getLayers(int layer)
{
    int lay;
    lay = *activelayerlist->at(layer);
    return  ( totalLayers - lay )-1;
}

void ActiveLayerList::addLayer(int layer)
{
    activelayerlist->append( new int(layer));
    numLayers +=1;
}

bool ActiveLayerList::getMultiSelect(void)
{
    if(getNumLayers()>1)
        return true;
    else
        return false;
}

void ActiveLayerList::clearList(void)
{
    if (numLayers>0)
    {
        activelayerlist->clear();
        numLayers=0;
    }
}

void ActiveLayerList::printInfo(void)
{
    QString conv;
    int lay;


    for ( uint i = 0; i <= activelayerlist->count(); ++i )
    {
        if ( activelayerlist->at(i) )
        {
            lay = *activelayerlist->at(i);
        }
    }
}


EffectLayer* 
LayerListEntry::addEffectLayer(EffectInfo::EFFECT_TYPE effect_type, bool status, int pluginid)
{
    EffectLayer* new_effect_layer = new EffectLayer;

    new_effect_layer->setType(effect_type);
    new_effect_layer->layerStatus = status;
    new_effect_layer->setPluginNumber(pluginid);

    QPtrList<EffectLayer>* effects_list = thelayer->getEffectsList();

    effects_list->append(new_effect_layer);
    return new_effect_layer;
}
