/*******************************************************************************
**
** The source file for the Jahshaka glworldlayer.cpp module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include <qcheckbox.h>

//////////////////////////////////////////////
// new function added that helps to configure
// layers before building them

JahLayer* GLWorld::configureLayer(QString cname, 
                             bool cstatus, bool cblend, bool cdepth,
                             bool cassetstatus, QString cfilename, QString cextension )
{
    LayerListEntry* new_layerlistentry = new LayerListEntry(cname);
    getLayerList()->append(new_layerlistentry);
    JahLayer* new_layer = new_layerlistentry->thelayer;
    setActiveJahLayer(new_layer);

    new_layer->layerStatus = cstatus;
    new_layer->blend = cblend;
    new_layer->depth = cdepth;

    new_layer->layerAssetStatus = cassetstatus;

    if (cfilename == 0)
    {
        new_layer->asset.filename = JahBasePath+"Pixmaps/jahlayer";
        new_layer->asset.extension = ".bmp";
    }
    else
    {
        new_layer->asset.filename = cfilename;
        new_layer->asset.extension = cextension;
    }

    return new_layer;
}

//////////////////////////////////////////////
// function thats used to create a new layer
// when you add a layer in a module

bool 
GLWorld::buildLayer(JahLayer* layer, QString layerName, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype ) 
{
    //first we set up default variables
    layer->asset.clipnumber   = getLayerList()->count() - 1;
    layer->layertype          = layertype;
    layer->objtype            = objtype;

    //layer->asset.clipname     = layerName;
    layer->layername          = layerName;

    bool itworked = TRUE;

    ////////////////////////////////////////
    //now we create the layer

    switch (layertype) 
    {
        case LayerCategory::LAYER    :  
        {    
            //we should check to see if no path is set...
            layer->drawtheLayer = TRUE;

            //we only auto-create single frame layers
            //but the .jah loader uses this so we override for clips
            if (!layer->asset.videotex)
            {
                layer->m_inframe    = 1;
                layer->m_outframe   = 1;
            }

            //load the image into the layer
            //debug("adding layer");
            layer->LoadLayerAsset(VideoCategory::IMAGE);
            layer->asset.theType = VideoType::STILL;

            //now we make the texture
            layer->makeTexture();

            //debug("added layer");

            break; 
        }

        case LayerCategory::PARTICLE :     
        {   
            //layer->addParticleObj();   
            layer->loadParticles = true;   
            break; 
        }	

        case LayerCategory::OBJECT  :     
        {   
            if (!layer->asset.location) 
            { 
                layer->asset.location  = JahMediaPath;
            }

            //this adds a 3dsmax type object
            if ( objtype==ObjectCategory::OBJECT) 
            {
                layer->loadObject = true;                                            
            }

            break; 
        }

        case LayerCategory::FRAME    :     {   break; }

        case LayerCategory::TEXT     :     
        {   
            layer->updateFont = true; 

            layer->drawtheLayer = TRUE;

            //only do this if it has a asset assigned to it!
            if (layer->layerAssetStatus)
            {
                layer->assignAsset(layer->asset, VideoCategory::IMAGE);
                layer->LoadLayerAsset(VideoCategory::IMAGE);
                layer->makeTexture();
            }

            break; 
        }

        case LayerCategory::GRID     :     {    break; }

        case LayerCategory::LIGHT    :     
        {  
            break; 
        }

        default       :                     
        {   
            itworked = FALSE;
            //debug("object type not supported"); 

            break; 
        }

    }

    return itworked;

}

//////////////////////////////////////////////
// function thats used to create a layer
// when you load it in from disk ie from a scene file


bool 
GLWorld::loadLayer( JahLayer* jah_layer, std::string layer_name, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype, 
                   bool isasset, assetData asset, bool haskey, assetData keyasset)
{
    LayerListEntry* layer_list_entry = jah_layer->getLayerListEntry();
    int layer_number = getLayerList()->find(layer_list_entry);
    QString name_qstring = QString( layer_name.data() );
    return loadLayer(layer_number, name_qstring, layertype, objtype, isasset, asset, haskey, keyasset);
}

bool GLWorld::loadLayer( int nl, QString layerName, LayerCategory::TYPE layertype, ObjectCategory::TYPE objtype, 
                        bool isasset, assetData asset, bool haskey, assetData keyasset ) 
{
    JahLayer* jah_layer = getLayerList()->at(nl)->thelayer;
    ////////////////////////////////////////
    //first we set up default variables
    jah_layer->layertype          = layertype;
    jah_layer->objtype            = objtype;
    jah_layer->layername          = layerName;
    jah_layer->layernodes->m_node->layer_enabled = false;

    bool itworked = TRUE;

    ////////////////////////////////////////
    //now we create the layer

    switch (layertype) 
    {
        case LayerCategory::LAYER    :  
        {    
            //we assume that all loaded LAYERS have a asset assigned

            //first we process the IMAGE layer
            if (isasset)
            {
                jah_layer->drawtheLayer = TRUE;
                jah_layer->assignAsset(asset, VideoCategory::IMAGE);
                jah_layer->LoadLayerAsset(VideoCategory::IMAGE);
                jah_layer->makeTexture();
                jah_layer->setFirstPass(true);
            }

            //first we process the IMAGE layer
            if (haskey)
            {
                //jah_layer->drawtheKey = TRUE;
                jah_layer->assignAsset(keyasset, VideoCategory::KEY);
                jah_layer->LoadLayerAsset(VideoCategory::KEY);
                jah_layer->makeTexture();
            }

            if (jah_layer->lighting)
            {
                jah_layer->option2 = true;
                //JahModuleOption[1]->setChecked(true); never do this it break the player
                //                                      ui settings are maintained in the modules
            }

            break; 
        }

        case LayerCategory::PARTICLE :     
        {   
            jah_layer->loadParticles = true; 
            //jah_layer->addParticleObj(); 

            if (isasset)
            {
                jah_layer->drawtheLayer = TRUE;
                jah_layer->assignAsset(asset, VideoCategory::IMAGE);
                jah_layer->LoadLayerAsset(VideoCategory::IMAGE);
                jah_layer->makeTexture();
            }             

            break; 
        }	

        case LayerCategory::OBJECT  :     
        {   
            if (!jah_layer->asset.location) 
            { 
                jah_layer->asset.location  = JahMediaPath;
            }

            //this adds a obj type object   
            /*
            if ( objtype==ObjectCategory::OBJOBJECTS) 
            {
            jah_layer->loadObject = true;
            //jah_layer->loadOBJ(
            //jah_layer->objfilename+jah_layer->objextension);   
            }
            */
            //this adds a 3dsmax type object
            if ( objtype==ObjectCategory::OBJECT) 
            {
                jah_layer->loadObject = true;
                //jah_layer->loadMaxOBJ(
                //jah_layer->objfilename+jah_layer->objextension);   
            }

            if (isasset)
            {
                jah_layer->drawtheLayer = TRUE;
                jah_layer->assignAsset(asset, VideoCategory::IMAGE);
                jah_layer->LoadLayerAsset(VideoCategory::IMAGE);
                jah_layer->makeTexture();
            }

            break; 
        }

        case LayerCategory::TEXT     :     
        {   //FIXME
            jah_layer->updateFont = true; 
            //if ( jah_layer->loadFontText()) 
            //{ 
            //    jah_layer->drawText(); 
            //}

            if (isasset)
            {
                jah_layer->drawtheLayer = TRUE;
                jah_layer->assignAsset(asset, VideoCategory::IMAGE);
                jah_layer->LoadLayerAsset(VideoCategory::IMAGE);
                jah_layer->makeTexture();
            }

            if (jah_layer->lighting)
            {
                jah_layer->option2 = true;

#ifndef JAHPLAYER
                if (ModuleName != "Text")
                {
                    JahModuleOption[1]->setChecked(true);
                }

#endif
            }

            break; 
        }

        case LayerCategory::GRID     :     {   break; }
        case LayerCategory::FRAME    :     {   break; }

        default       :     
        {   
            itworked = FALSE;
            //debug("object type not supported"); 

            break; 
        }
    }

    return itworked;

}
/////////////////////////////////////////////////////////////
//switch the layer object type but we dont mess with layer 0
//as its the world object

void GLWorld::ChangeObjectLayer(void)    
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::MESHLAYER;  
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}

void GLWorld::ChangeObjectCube(void)     
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::CUBE;      
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}

void GLWorld::ChangeObjectCylinder(void) 
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::CYLINDER;      
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}

void GLWorld::ChangeObjectSphere(void)   
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::SPHERE;      
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}

void GLWorld::ChangeObjectBezier(void)   
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::BEZIERS;      
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}


/////////////////////////////////////////////////////////////////
//we need to initialize the mesh if we plan to use it...

void GLWorld::ChangeObjectMesh(void)     
{ 
    if (getActiveJahLayer()->objtype == ObjectCategory::LARGE_LAYER)
    {
        return;
    }

    JahLayer* active_layer = getActiveJahLayer();
    JahLayer* previous_active_layer = active_layer;

    if (active_layer->objtype == ObjectCategory::EFFECT)
    {
        active_layer = active_layer->getParent();
        setActiveJahLayer(active_layer);
    }

    if (getActiveJahLayer() != camera) 
    { 
        getActiveJahLayer()->objtype = ObjectCategory::MESHLAYER;      
        updateGL(); 
    } 

    setActiveJahLayer(previous_active_layer);
}
