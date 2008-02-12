/*******************************************************************************
**
** The header file for the Jahshaka glworld module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef GLWORLDLISTS_H
#define GLWORLDLISTS_H

#include <qptrlist.h>
#include <qstring.h>

#include "coreobject.h"
#include "coreeffects.h"

///////////////////////////////////////////
// linked list to store selected layers
// updated frequently
// its a mess because we store layer numbers in reverse according 
// to the position in the list
// ie layer 4 is the 4th layer etc
// but qt stores it the other way so 4 = 0 if there are 4 layers

class ActiveLayerList
{
public:
    ActiveLayerList();


    int             getNumLayers();
    void            setTotalLayers( int lay);

    int getFinalSelection(void);
    int getFinalSelection(int);

    int getLayers(int layer);
    void addLayer(int layer);
    
	bool getMultiSelect(void);

    void clearList(void);

    void printInfo(void);


private:

    int             numLayers;          //number of selected layers
    int             totalLayers;        //number of layers in listview
    QPtrList<int>   *activelayerlist;   //for the linked list for dynamic object layers

};

class selectionList
{
public:
    selectionList( int l, bool s) : layer(l), status(s)
    {}

    public:

    int       layer;
    bool      status;
};


//////////////////////////////////////////////////

///////////////////////////////////////////
// linked list to store layers

class LayerListEntry
{
public:
    LayerListEntry( int layer, const QString& name )
    {
        thelayer = new JahLayer;
        thelayer->layername = name;
        thelayername = name;
        thelayernumber = layer;
        thelayer->setLayerListEntry(this);

        m_effects_list = new QPtrList<EffectLayer>;
        m_effects_list->setAutoDelete( TRUE );
    }

    LayerListEntry(const QString& name )
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

    ~LayerListEntry()
    {
        delete m_effects_list;
		delete thelayer;
    }

    /////////////////////////////////////////////////////
    //for the layer
    QString         name()   const  { return thelayername;  }
    int             layer()  const  { return thelayernumber;  }

    JahLayer*       getJahLayer() { return thelayer; }
    JahLayer*       thelayer;

    /////////////////////////////////////////////////////
    //for the layer effects

    //1. add layer effect

	//we should set the effect name and number at the same time?

    EffectLayer* addEffectLayer(EffectInfo::EFFECT_TYPE effect_type, bool status = true, int pluginid = 0);

	inline int getNumberOfEffects()
    {
        return m_effects_list->count();
	}

	inline bool fxLayerStatus(int effect_number)
    {
        return m_effects_list->at(effect_number)->layerStatus;
	}

	inline int fxPluginNumber(int effect_number)
    {
		return m_effects_list->at(effect_number)->getPluginNumber();
	}


    inline QPtrList<EffectLayer>* getEffectsList() { return m_effects_list; }

	inline EffectInfo::EFFECT_TYPE getEffectType(int effect_number)
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

	inline bool getLayerStatus(void)
    {
		return thelayer->layerStatus;
	}

    //2. turn effect on/off
    //3. remove layer effect

public:
    QString         thelayername;

private:

    //for the layers
    int             thelayernumber;

    // This list shouldn't exist FIXME 
    QPtrList<EffectLayer>*              m_effects_list;
};

#endif // GLWORLDHEADERS_H

