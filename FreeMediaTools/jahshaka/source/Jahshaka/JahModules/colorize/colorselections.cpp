/*******************************************************************************
**
** The source file for the Jahshaka color selections module
** The Jahshaka Project
** Copyright (C) 2000-2004 The Jahshaka Project
** Released under the GNU General Public License
**
*******************************************************************************/

#include "color.h"

////////////////////////////////////////////////////////////////////////////
// for cpu based effects
//need to rename the routine to be addLayerCPU or something more relevant
/*
void GLColorize::addLayer(void)
{
#if 0
   //add on demand layers...
   bool added = TRUE;
   QPopupMenu *menu = new QPopupMenu( 0 );

    int existingitems = 0;

    //math based functions
    menu->insertItem( "Grey",existingitems,0 );                     existingitems+=1;
    menu->insertItem( "Select Colors",existingitems,0 );            existingitems+=1;
    menu->insertItem( "Contrast",existingitems,0 );                 existingitems+=1;
    menu->insertItem( "Brightness",existingitems,0 );               existingitems+=1;
    menu->insertItem( "SwapRGB",existingitems,0 );                  existingitems+=1;
    menu->insertItem( "Negative",existingitems,0 );                 existingitems+=1;
    menu->insertItem( "Colorize",existingitems,0 );                 existingitems+=1;

   //////////////////////////////////////////////////////////////////////////////////
   //check for plugins now - make this a subroutine please and just pass what we need to pass

   //the class of plugin
    int usingclass = 1;

    int totalitems  = existingitems -1;

    if (jplugin->vfxPluginsCounter>0)
    {
        totalitems = addPluginsToMenu(usingclass, existingitems, menu);
    }

	//if the gpu is active you cant see/use cpu effects...
    if (gpuactive)
    {
        for (int i=0; i<=totalitems; i++)
            menu->setItemEnabled(i,false);
    }

    ////////////////////////////////////////////////////////////////////////////////

    menu->setMouseTracking( TRUE );
    int id = menu->exec( AddButton->mapToGlobal( QPoint( 0, AddButton->height() + 1 ) ) );

    QString rere;
    //debug("the id is:"+rere.setNum(id));

    QString layNum,temp;
    QString title;

    if ( id != -1 && initialized )
    {

      m_number_of_layers++; 
	  layNum.setNum(m_number_of_layers);

      switch(id) {

         case 0  : {  title = "Grey";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(GREY,true);

                      break; }

         case 1  : {  title = "Select Colors";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(SELECTCOLORS,true);

                      break; }

         case 2 : {   title="Contrast";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(CONTRAST,true);

                      break; }

         case 3 : {   title="Brightness";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(BRIGHTNESS,true);

                      break; }

         case 4  : { title="SwapRGB";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(SWAPRGB,true);

                      break; }

        case  5  : {  title = "Negative";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(NEGATIVE,true);

                      break; }

        case  6  : {  title = "Colorize";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(CPUCOLORIZE,true);

                      break; }

         default : { added = false; break; }
      }

        //////////////////////////////////////////////////////
        // wasnt in the switch check if its a plugn
        if (!added)
        {
            QString thename = menu->text(id);
            int itemid = jplugin->getPluginId(thename,usingclass);

            if (itemid>=0)
            {
                title = jplugin->vfxPlugins->at(itemid)->getPluginName();
                m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                m_layerlist->at(m_number_of_layers)->thelayer->layername = title;

				m_layerlist->at(1)->addEffectLayer(JAHPLUGIN,true, itemid);

                added = TRUE;
            }
            else
            {
                added = FALSE;
            }
        }
    }

    /////////////////////////////////////////////////////
    // add it to the system
    if (added && (id>=0) && initialized)
    {
        m_layerlist->at(m_number_of_layers)->thelayer->layertype  = LayerCategory::LAYER;
        m_layerlist->at(m_number_of_layers)->thelayer->objtype = ObjectCategory::EFFECT;

		//force layer 1 to stay as a ObjectCategory::MESHLAYER
		//this should not be necessary
        m_layerlist->at(1)->thelayer->objtype = ObjectCategory::MESHLAYER;
        
		//add the layer to the ui and the system
		addTheLayer(m_number_of_layers,title);

    }

    ///////////////////////////////////////////////////
    // clean up on the way out
    delete menu;
#endif
}

void GLColorize::addRTLayer(void)
{
#if 0
   //add on demand layers...
   bool added = TRUE;
   QPopupMenu *menu = new QPopupMenu( 0 );

    int existingitems = 0;

    menu->insertItem( "RT Colorize",existingitems,0 );              existingitems+=1;
    menu->insertItem( "RT RGB2Lum",existingitems,0 );               existingitems+=1;

    ///////////////////////////////////////////////////////////////////

    int usingclass = 3;
    int totalitems  = existingitems -1;

    if (jplugin->vfxPluginsCounter>0)
    {
        totalitems = addPluginsToMenu(usingclass, existingitems, menu);
    }

    ///////////////////////////////////////////////////////////////////

    menu->setMouseTracking( TRUE );
    int id = menu->exec( AddRTButton->mapToGlobal( QPoint( 0, AddRTButton->height() + 1 ) ) );
    QString title;


	//the effects layer is layer 1



    if ( id != -1 && initialized ) {

      m_number_of_layers+=1; //really number of effects...

      switch(id) {

        case  0  : {  title = "RT Color Matrix";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(COLORMATRIX,true);

                      break; }

        case  1  : {  title = "RT RGB2Luminance";
                      m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                      m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
					  m_layerlist->at(1)->addEffectLayer(RTCOLORLUM,true);

                      break; }


         default : { added = false; break; }
      }

      //////////////////////////////////////////////////////
      // wasnt in the switch check if its a plugn

      if (!added)
      {
        QString thename = menu->text(id);
        int itemid = jplugin->getPluginId(thename,usingclass);

        if (itemid>=0)
        {
            title = jplugin->vfxPlugins->at(itemid)->getPluginName();
            m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
            m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
			
			m_layerlist->at(1)->addEffectLayer(JAHPLUGIN,true, itemid);
            
            added = TRUE;
        }
        else
        {
            added = FALSE;
        }

     }

    }

    delete menu;

    ////////////////////////////////////////////////////
    //add it to the system now

    if (added && (id>=0) && initialized )
    {
        m_layerlist->at(m_number_of_layers)->thelayer->layertype = LayerCategory::LAYER;
        m_layerlist->at(m_number_of_layers)->thelayer->objtype = ObjectCategory::EFFECT; 

		//force layer 1 to stay as a ObjectCategory::MESHLAYER
		//this should not be necessary
        m_layerlist->at(1)->thelayer->objtype = ObjectCategory::MESHLAYER;
        
		//add the layer to the ui and the system
		addTheLayer(m_number_of_layers,title);

    }
#endif
}

void GLColorize::addGpuLayer(void)
{
#if 0
   //add on demand layers...
   bool added = TRUE;

   QPopupMenu *menu = new QPopupMenu( 0 );

    int existingitems = 0;

    if (nv_gpusupport)
    {
        menu->insertItem( "NV30::Charcoal", existingitems,0 );           existingitems+=1;
        //menu->insertItem( "NV30::Earthquake NV30 GPU", existingitems,0 );   existingitems+=1;
    }

   /////////////////////////////////////////////////////////////////////////////////////////////////////////
   //little security mechanism here
   //if there are no plugins skip the following and head straight to the existing defs...
   //if (jplugin->vfxPluginsCounter==0) return;

    int usingclass = 2;

    int totalitems  = existingitems -1;

    if (jplugin->vfxPluginsCounter>0)
    {
        totalitems = addPluginsToMenu(usingclass, existingitems, menu);
    }

    if (!gpuactive)
    {
        for (int i=0; i<=totalitems; i++)
            menu->setItemEnabled(i,false);
    }

    ////////////////////////////////////////////////////////////////////////////////

    menu->setMouseTracking( TRUE );
    int id = menu->exec( AddGpuButton->mapToGlobal( QPoint( 0, AddGpuButton->height() + 1 ) ) );

    QString layNum,temp;
    QString title;

    if ( id != -1 && initialized ) {

      m_number_of_layers+=1; layNum.setNum(m_number_of_layers);

      switch(id) {

         case 0 : {   
                     if (nv_gpusupport)
                     {
                         title="Charcoal NV30";
                         m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                         m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
       					 
						 m_layerlist->at(1)->addEffectLayer(CHARCOAL_NV30_GPU,true);

                         break;
                     }
                  }


         default : { added = false;  break; }
      }

        //////////////////////////////////////////////////////
        // wasnt in the switch check if its a plugn

        if (!added)
        {
            QString thename = menu->text(id);
            int itemid = jplugin->getPluginId(thename,usingclass);

            if (itemid>=0)
            {
                title = jplugin->vfxPlugins->at(itemid)->getPluginName();
                m_layerlist->append( new LayerListEntry(m_number_of_layers, title) );
                m_layerlist->at(m_number_of_layers)->thelayer->layername = title;
			    
				m_layerlist->at(1)->addEffectLayer(JAHPLUGIN,true, itemid);

                added = TRUE;
            }
            else
            {
                added = FALSE;
            }
        }
    }

    ////////////////////////////////////////////////////
    //add it to the system now

    if (added && (id>=0) && initialized)
    {
        m_layerlist->at(m_number_of_layers)->thelayer->layertype = LayerCategory::LAYER;
        m_layerlist->at(m_number_of_layers)->thelayer->objtype = ObjectCategory::EFFECT;

		//force layer 1 to stay as a ObjectCategory::MESHLAYER
		//this should not be necessary
        m_layerlist->at(1)->thelayer->objtype = ObjectCategory::MESHLAYER;
        
		//add the layer to the ui and the system
		addTheLayer(m_number_of_layers,title);

    }

    ///////////////////////////////////////////////////
    //clean up on the way out
    delete menu;
#endif
}

/////////////////////////////////////////////////////////////////////////////////
// for the plugins
//
//we will filter out non color plugins here
//

int GLColorize::addPluginsToMenu(int usingclass, int existingitems, QPopupMenu *menu)
{
    //math based functions
    //int plugcount = 0;
    QPopupMenu *  submenu[10];
    QString     * submenutitle[10];
    int submenucount = 0;

    int menuitem = existingitems;

    for (int x=0; x<jplugin->vfxPluginsCounter; x++)
    {
        // first check for family
        //  if fmaily exists we add plugin there
        //      then check for group
        //          if group exists we add plugin in family, in group

        QString name, family, group;

        name   = jplugin->vfxPlugins->at(x)->getPluginName();
        family = jplugin->vfxPlugins->at(x)->getPluginFamily();
        group  = jplugin->vfxPlugins->at(x)->getPluginGroup();

        //////////////////////////////////////////////////////////////
        //need to get plugin type so we can put it in the right menu ie gpu etc
        //need to also get arb or nv support...
        JAH_PLUGIN_CLASS pluginclass;
        pluginclass = jplugin->vfxPlugins->at(x)->getPluginClass();
        if (pluginclass!=usingclass) continue;


        ///////////////////////////////////////////////////////////////
        //plugin isnt part of a family we put it in the base menu
        if (family=="NULL")
        {
            menu->insertItem( name,menuitem,0 );
        }
        else
        {
            //////////////////////////////////////////////////////////////
            //its part of a family we put it in a submenu
            //need to check if the submenu already exists for reusability
            bool exists = false;;
            int submenuexists;

            if (submenucount>0)
            {
                for (int i=0; i<submenucount; i++)
                {
                    if ( *submenutitle[i]==family )
                    {
                        exists = true;
                        submenuexists = i;
                    }
                    if (exists) break;
                }
            }

            if (exists)
            {
                submenu[submenuexists]->insertItem( name, menuitem, 0 );
            }
            else
            {
                submenutitle[submenucount] = new QString(family);
                submenu[submenucount] = new QPopupMenu(0);

                Q_CHECK_PTR( submenu[submenucount] );
                submenu[submenucount]->insertItem( name, menuitem, 0 );

                menu->insertItem( family, submenu[submenucount] );
                submenucount++;
            }
        }
        //means we added it so increment
        menuitem ++;
    }

    return menuitem;
}
*/
