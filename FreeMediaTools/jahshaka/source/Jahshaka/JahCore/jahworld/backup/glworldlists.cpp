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


LayerListEntry::LayerListEntry( int layer, const QString& name )
    {
        thelayer = new JahLayer;
        thelayer->layername = name;
        thelayername = name;
        thelayernumber = layer;
        thelayer->setLayerListEntry(this);

        m_effects_list = new QPtrList<EffectLayer>;
        m_effects_list->setAutoDelete( TRUE );
    }

LayerListEntry::LayerListEntry(const QString& name )
    {
        // (NOTE: layernumber never set if using this constr?)

        // Further note. Position in a list should never be kept as state
        // If a layer is inserted or deleted earlier in the list, the layernumber
        // becomes incorrect and potentially leads to all kinds of bugs.  The great thing about lists 
        // is that they manage themselves, and if you really want to know the layer number (why?)
        // you can find it using the list find() method. 
        // Tony

        thelayer = new JahLayer;
        thelayername = name;
        thelayer->layername = name;
        thelayer->setLayerListEntry(this);

        m_effects_list = new QPtrList<EffectLayer>;
        m_effects_list->setAutoDelete( TRUE );
    }

LayerListEntry::~LayerListEntry()
    {
        delete m_effects_list;
		delete thelayer;
    }

    /////////////////////////////////////////////////////
    //for the layer
    QString         LayerListEntry::name()   const  { return thelayername;  }
    int             LayerListEntry::layer()  const  { return thelayernumber;  }

    JahLayer*       LayerListEntry::getJahLayer() { return thelayer; }

    EffectLayer* LayerListEntry::addEffectLayer(EffectInfo::EFFECT_TYPE effect_type, bool status = true, int pluginid = 0);

	int LayerListEntry::getNumberOfEffects()
    {
        return m_effects_list->count();
	}

	bool LayerListEntry::fxLayerStatus(int effect_number)
    {
        return m_effects_list->at(effect_number)->layerStatus;
	}

int LayerListEntry::fxPluginNumber(int effect_number)
{
  return m_effects_list->at(effect_number)->getPluginNumber();
}


QPtrList<EffectLayer>* LayerListEntry::getEffectsList() 
{
  return m_effects_list; 
}

EffectInfo::EFFECT_TYPE LayerListEntry::getEffectType(int effect_number)
{
  if ( effect_number < 0 || effect_number > (int)m_effects_list->count() )
    {
      return EffectInfo::NOT_A_TYPE;
    }
  if (m_effects_list->at(effect_number))
    {
      return m_effects_list->at(effect_number)->objtype;
    }
  else
    {
      return EffectInfo::NOT_A_TYPE;
    }
}

bool LayerListEntry::getLayerStatus(void)
{
  return thelayer->layerStatus;
}



selectionList::selectionList( int l, bool s) : layer(l), status(s)
{}
