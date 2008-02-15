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
  selectionList( int l, bool s) ;
  
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
  LayerListEntry( int layer, const QString& name );
  LayerListEntry(const QString& name );
  ~LayerListEntry();

  /////////////////////////////////////////////////////
  //for the layer

  QString         name()   const;
  int             layer()  const;  
  JahLayer*       getJahLayer();

  JahLayer*       thelayer;

    /////////////////////////////////////////////////////
    //for the layer effects

    //1. add layer effect

	//we should set the effect name and number at the same time?

  EffectLayer* addEffectLayer(EffectInfo::EFFECT_TYPE effect_type, bool status = true, int pluginid = 0);
  int getNumberOfEffects();
  bool fxLayerStatus(int effect_number);
  int fxPluginNumber(int effect_number);
  QPtrList<EffectLayer>* getEffectsList() ;
  EffectInfo::EFFECT_TYPE getEffectType(int effect_number);
  bool getLayerStatus(void);
public:
    QString         thelayername;

private:

    //for the layers
    int             thelayernumber;

    // This list shouldn't exist FIXME 
    QPtrList<EffectLayer>*              m_effects_list;
};

#endif // GLWORLDHEADERS_H

