/*******************************************************************************
**
** The source file for the Jahshaka effects objects module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "glworld.h"
#include "glworldlists.h"
#include "glcore.h"
#include "jahrender.h"
#include "jahpluginlib.h"

#include "blurlib.h" //for the gussian blur effect
#include <qpopupmenu.h>
#include <qpushbutton.h>
#include <qlistview.h>

void
GLWorld::chooseNewCPUEffect(LayerListEntry* parent,
			    QWidget* thebutton,
			    int x_offset,
			    int y_offset)
{
    JahLayer* image_layer = parent->getJahLayer();

    if ( !image_layer->containsCpuEffects() )
    {
        *image_layer->getCompositeQimagePtr() = image_layer->textureData.objectImage.copy();
        *image_layer->getCompositeQimageFlippedPtr() = 
            image_layer->getCompositeQimagePtr()->mirror(false,true);
        image_layer->setContainsCpuEffects(true);
    }

    QPopupMenu *menu = new QPopupMenu( 0 );

    int total_effect_count = getNumberEmbeddedCPUEffects();

    // Generate the pop-up menu
    int menu_item_number = 0;
    int first_effect_index = EffectInfo::CPU_TYPE_START + 1;

    for (int i = first_effect_index; i < EffectInfo::MESH_TYPE_START; i++)
    {
        //if (i != EffectInfo::CHROMA_KEYER_CPU_TYPE)
        //{
            // There are some issues with the chroma keyer CPU layer code that need to
            // be worked out, so don't include it in the menu

            EffectInfo* effect = m_aCPUEffectsSelectionVector[i];
            EffectInfo::EFFECT_TYPE effect_type = effect->getType();

            menu->insertItem( QString( (*effect->getName(effect_type)).data() ), menu_item_number, 0 );
            menu_item_number++;
        //}
    }

    // Add the plugins to the menu
    if (m_pJPlugIn->vfxPluginsCounter > 0)
    {
        total_effect_count = addPluginsToMenu(JAH_CPU_PLUGIN, total_effect_count, menu);
    }

    // Select an effect from the menu    
    menu->setMouseTracking( TRUE );

    QPoint menu_point(0, thebutton->height() + 1);

    if (x_offset)
    {
        menu_point.setX(x_offset);
        menu_point.setY(y_offset);
    }

    int effect_id = menu->exec( thebutton->mapToGlobal(menu_point) );

	addCPUEffectLayer(effect_id, parent->getJahLayer(), menu->text(effect_id).data() ); 
    delete menu;
}

void                
GLWorld::addCPUEffectLayer(int effect_id, 
			   JahLayer* parent_jah_layer,
			   const char* effect_name)
{
    QString title;
    EffectInfo::EFFECT_TYPE effect_type = EffectInfo::NOT_A_TYPE;
    std::string guid = "";
    int plugin_id = -1;
    bool added = TRUE;
    int first_effect_index = EffectInfo::CPU_TYPE_START + 1;

    if ( effect_id != -1 && parent_jah_layer->layerAssetStatus )
    {
        int vector_index = effect_id + first_effect_index;

        if ( vector_index < EffectInfo::MESH_TYPE_START )
        {
            effect_type = m_aCPUEffectsSelectionVector[vector_index]->getType();
            title = QString( 
			    (*m_aCPUEffectsSelectionVector[vector_index]
			     ->getName(effect_type)).data() );
        }
        else
        {
            QString thename = effect_name;
            plugin_id = m_pJPlugIn->getPluginId(thename, JAH_CPU_PLUGIN);

            if (plugin_id >= 0)
            {
                title = m_pJPlugIn->vfxPlugins->at(plugin_id)->getPluginName();
                effect_type = EffectInfo::JAH_PLUGIN_TYPE;
                guid = std::string(
				   m_pJPlugIn->
				   vfxPlugins->
				   at(plugin_id)->
				   getGuid().data()
				   );
            }
            else
            {
                added = FALSE;
            }
        }
    }

    if (added && (effect_id >= 0) && parent_jah_layer->layerAssetStatus )
    {
		addEffectLayer(parent_jah_layer->getLayerListEntry(), title, effect_type, plugin_id, guid);
        parent_jah_layer->setEffectsSlidersHaveChanged(true);
        updateGL();
    }

    m_bSlidersHaveChanged = true;
}


////////////////////////////////////////////////
//qt image processing based functions

void GLWorld::doSwapRGB(JahLayer*, QImage& theimage)
{

    theimage = theimage.swapRGB();

}

//need to add controls here
void GLWorld::doMirror(JahLayer*, QImage& theimage)
{
    //need to add ui controls for this
    bool VERT = TRUE;
    bool HORIZ = TRUE;

    theimage = theimage.mirror(VERT,HORIZ);

}

void GLWorld::doNegative(JahLayer*, QImage& theimage)
{

    theimage.invertPixels();

}

////////////////////////////////////////////////
//math and processor based functions here

void GLWorld::doGrey(JahLayer* base_layer, QImage& pix)
{
    int r,g,b,a,grey;
    QString conv,text;

    //note that this crashes when x or y are over the scope
    //for some reason this happens with the default image

    //int yy = (int)layers[1].textureData.height;
    //int xx = (int)layers[1].textureData.width;
    int yy = (int)base_layer->textureData.height;
    int xx = (int)base_layer->textureData.width;

    for ( int y=0; y<yy; y++ )
    {
        for ( int x=0; x<xx; x++ )
        {
            //if we can do this one SCANLINE at a time it would be faster
            //uchar *pe = pix.scanLine(y);

            QRgb *p = (QRgb *)pix.scanLine(y) + x; //debug("here1");

            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p);
            a=qAlpha(*p);
            

            grey=( (r+g+b)/3 ); //debug("here3");
            *p = qRgba(grey,grey,grey,a);
        }
    }

}

void GLWorld::doContrast(JahLayer* base_layer, QImage& pix) //was double power
{
    //define variables
    int yy,xx;              //source buffer width and height

    float contrast;
    bool contrastmode;
    int cval;

    int r,g,b,a;              //pixel variables

    //get source buffer size
    yy = (int)base_layer->textureData.height;
    xx = (int)base_layer->textureData.width;

    //get contrast value
    contrast = (getCurrentEffectLayer()->layernodes->m_node->rx) / 255;

    //catch default startup value
    if (contrast==0) contrast=1;

    //figure out cmode using slider need a checkbox!
    cval = (int)(getCurrentEffectLayer()->layernodes->m_node->sx);
    if (cval>=0) contrastmode = true; else contrastmode=false;

    //start main loop
    if (contrastmode)
    {
        //faster matrix controlled contrast
        int Contrast_transform[256];

        for(int i=0;i<256;i++)
        {
            if(i<(int)(128.0f+128.0f*tan(contrast))&&i>(int)(128.0f-128.0f*tan(contrast)))
                Contrast_transform[i]=(int)((i-128)/tan(contrast)+128);
            else if(i>=(int)(128.0f+128.0f*tan(contrast)))
                Contrast_transform[i]=255;
            else
                Contrast_transform[i]=0;
        }

        for ( int y=0; y<yy; y++ )
        {
            for ( int x=0; x<xx; x++ )
            {
                //this is the source and destination pixel
                QRgb *p = (QRgb *)pix.scanLine(y) + x;
                r=qRed(*p);
                g=qGreen(*p);
                b=qBlue(*p);
                a=qAlpha(*p);

                //now write out the destination pixel
                *p = qRgba(Contrast_transform[r],Contrast_transform[g],Contrast_transform[b],a);

            }
        }
    }
    else
    {
        //slower math controlled contrast
        for ( int y=0; y<yy; y++ )
        {
            for ( int x=0; x<xx; x++ )
            {
                QRgb *p = (QRgb *)pix.scanLine(y) + x;
                r=qRed(*p);
                g=qGreen(*p);
                b=qBlue(*p);
                a=qAlpha(*p);

                int r1 = (int)( 255 * pow(  float(float(r) / 255.0) , contrast ));
                int g1 = (int)( 255 * pow(  float(float(g) / 255.0) , contrast ));
                int b1 = (int)( 255 * pow(  float(float(b) / 255.0) , contrast ));

                //some saftey tests here to make sure colors dont go out of range
                if (r1>255) r1=255;   if (g1>255) g1=255;   if (b1>255) b1=255;
                if (r1<0)   r1=0;     if (g1<0)   g1=0;     if (b1<0)   b1=0;

                *p = qRgba(r1,g1,b1,a);
            }
        }
    }
}

void GLWorld::doBrightness(JahLayer* base_layer,  QImage& pix) //was double power
{
    //define variables
    int yy,xx;              //source buffer width and height

    float brightness;

    int r,g,b,a;              //pixel variables

    //get source buffer size
    yy = (int)base_layer->textureData.height;
    xx = (int)base_layer->textureData.width;

    //get contrast value
    brightness = (getCurrentEffectLayer()->layernodes->m_node->rx);

    //catch default startup value
    //if (brightness==0) brightness=1;

    int Light_transform[256];

    for(int i=0;i<256;i++)
    {
        Light_transform[i]=(int)(i+brightness);
        if(Light_transform[i]>255)
            Light_transform[i]=255;
        if(Light_transform[i]<0)
            Light_transform[i]=0;
    }

    for ( int y=0; y<yy; y++ )
    {
        for ( int x=0; x<xx; x++ )
        {
            //this is the source and destination pixel
            QRgb *p = (QRgb *)pix.scanLine(y) + x;
            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p);
            a=qAlpha(*p);

            //now write out the destination pixel
            *p = qRgba(Light_transform[r],Light_transform[g], Light_transform[b], a);
        }
    }

}

void GLWorld::doMatrixSharpen(JahLayer* base_layer,  QImage& pix) //was double power
{

    //debug("sharpen");
    //define variables
    int yy,xx;              //source buffer width and height

    int r,g,b, a;              //pixel variables

    //get source buffer size
    yy = (int)base_layer->textureData.height;
    xx = (int)base_layer->textureData.width;

    const int sharp_w = 3;
    const int sharp_h = 3;

    int sumr=0;
    int sumg=0;
    int sumb=0;

    // int sharpen_filter[sharp_w][sharp_h]={{0,-1,0},{-1,5,-1},{0,-1,0}};
    //the correct filter
    int sharpen_filter[sharp_w][sharp_h]={{-1,-1,-1},{-1,9,-1},{-1,-1,-1}};


    //try this
    //int sharpen_filter[sharp_w][sharp_h]={{-1,0, 1},{-2,0,2},{-1,0,1}};

    //get sharp value
    int sharp_sum = (int)(getCurrentEffectLayer()->layernodes->m_node->rx+1);
    if( sharp_sum <1) sharp_sum = 1;

    //problem with matrix effects is we have to build a new image with the data
    //since sampling is done across 3 rows at a time
    QImage* buffer = new QImage( xx, yy, 32 );

    for ( int y=1; y<yy-1; y++ )
    {
        for ( int x=1; x<xx-1; x++ )
        {
            sumr = 0;

            for(int k=0;k<sharp_w;k++)
            {
                for(int l=0;l<sharp_h;l++)
                {
                    int xval = x- ((sharp_w-1)>>1)+k;
                    int yval = y- ((sharp_h-1)>>1)+l;

                    if (xval>=(xx-1)) xval = xx-2;
                    if (yval>=(yy-1)) yval = yy-2;

                    if (yval<1) yval = 1;
                    if (xval<1) xval = 1;

                    QRgb *sp = (QRgb *)pix.scanLine(yval) + xval;
                    r=qRed(*sp);
                    g=qGreen(*sp);
                    b=qBlue(*sp);

                    sumr+=r*sharpen_filter[k][l];
                    sumg+=g*sharpen_filter[k][l];
                    sumb+=b*sharpen_filter[k][l];

                }
            }

            QRgb *p = (QRgb *)buffer->scanLine(y) + x;
            a = qAlpha(*p);

            sumr/=sharp_sum;
            sumb/=sharp_sum;
            sumg/=sharp_sum;

            //now write out the destination pixel
            *p = qRgba(sumr,sumg,sumb, a);

        }
    }

    //now we must copy buffer into pix
    pix = buffer->copy();

	delete buffer;

}

void GLWorld::doMatrixBlur(JahLayer* base_layer,  QImage& pix) //was double power
{
    //define variables
    int yy,xx;              //source buffer width and height
    int r,g,b,a;              //pixel variables

    //get source buffer size
    yy = (int)base_layer->textureData.height;
    xx = (int)base_layer->textureData.width;

    const int gauss_width = 7;

    int gauss_fact[gauss_width]={1,6,15,20,15,6,1};
    //int gauss_sum=64;
    //get contrast value
    int gauss_sum = (int)(getCurrentEffectLayer()->layernodes->m_node->rx+64);

    int sumr=0;
    int sumg=0;
    int sumb=0;

    int x,y,k;

    //problem with matrix effects is we have to build a new image with the data
    //since sampling is done across 3 rows at a time
    QImage* buffer = new QImage( pix.width(), pix.height(), pix.depth() );

    //first we do a horizontal blur
    for (  y=1; y<yy-1; y++ )
    {
        for (  x=1; x<xx-1; x++ )
        {
            sumr=0; sumg=0; sumb=0;

            for( k=0;k<gauss_width;k++)
            {
                int xval = x-((gauss_width-1)>>1)+k;
                int yval = y;

                if (xval>=(xx-1)) xval = xx-2;
                if (yval>=(yy-1)) yval = yy-2;

                if (yval<1) yval = 1;
                if (xval<1) xval = 1;

                QRgb *sp = (QRgb *)pix.scanLine(yval) + xval;
                r=qRed(*sp);
                g=qGreen(*sp);
                b=qBlue(*sp);

                sumr+=r*gauss_fact[k];
                sumg+=g*gauss_fact[k];
                sumb+=b*gauss_fact[k];
            }


            QRgb *p = (QRgb *)buffer->scanLine(y) + x;

            //now write out the destination pixel
			if(gauss_sum != 0)
			{
				*p = qRgb(sumr/gauss_sum,sumg/gauss_sum,sumb/gauss_sum);
			}
			else
			{
				*p = qRgb(0,0,0);
			}
        }
    }

    //now we do a vertical blur
    for (  y=1; y<yy-1; y++ )
    {
        for (  x=1; x<xx-1; x++ )
        {
            sumr=0; sumg=0; sumb=0;

            for( k=0;k<gauss_width;k++)
            {
                int xval = x;
                int yval = y-((gauss_width-1)>>1)+k;

                if (xval>=(xx-1)) xval = xx-2;
                if (yval>=(yy-1)) yval = yy-2;

                if (yval<1) yval = 1;
                if (xval<1) xval = 1;

                QRgb *sp = (QRgb *)buffer->scanLine(yval) + xval;

                r=qRed(*sp);
                g=qGreen(*sp);
                b=qBlue(*sp);

                sumr+=r*gauss_fact[k];
                sumg+=g*gauss_fact[k];
                sumb+=b*gauss_fact[k];
            }


            QRgb *p = (QRgb *)pix.scanLine(y) + x;
            a = qAlpha(*p);

            //now write out the destination pixel
            *p = qRgba(sumr/gauss_sum,sumg/gauss_sum,sumb/gauss_sum, a);
        }
    }
    //now we must copy buffer into pix
    // pix = buffer->copy();

}



void GLWorld::doMatrixEmboss(JahLayer* base_layer,  QImage& pix) //was double power
{
    //define variables
    int yy,xx;              //source buffer width and height

    int r,g,b,a;              //pixel variables
    a = 0;

    //get source buffer size
    yy = (int)base_layer->textureData.height;
    xx = (int)base_layer->textureData.width;

    const int emboss_w = 3;
    const int emboss_h = 3;

    int sumr=0;
    int sumg=0;
    int sumb=0;
    //int grey;

    //original emboss
    int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};

    //int emboss_filter[emboss_w][emboss_h]={{6,12,6},{12,25,12},{6,12,6}};
    //int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};
    //int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};
    //int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};
    //int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};

    //int emboss_sum=1;
    int emboss_sum = (int)(getCurrentEffectLayer()->layernodes->m_node->rx / 50.0f) + 1;

    if (emboss_sum<1) emboss_sum = 1;

    //first we convert to greysacle...
    /*
    for ( int y1=0; y1<yy; y1++ )
    {
    for ( int x1=0; x1<xx; x1++ )
    {
    //if we can do this one SCANLINE at a time it would be faster
    //uchar *pe = pix.scanLine(y);

    QRgb *p = (QRgb *)pix.scanLine(y1) + x1; //debug("here1");

    r=qRed(*p);
    g=qGreen(*p);
    b=qBlue(*p); //debug("here2");

    grey=( (r+g+b)/3 ); //debug("here3");
    *p = qRgb(grey,grey,grey);
    }
    }
    */
    //then we emboss
    //problem with matrix effects is we have to build a new image with the data
    //since sampling is done across 3 rows at a time
    QImage* buffer = new QImage( xx, yy, 32 );

    for ( int y=1; y<yy-1; y++ )
    {
        for ( int x=1; x<xx-1; x++ )
        {
            sumr = 0;

            //added these
            sumg = 0;
            sumb = 0;

            for(int k=0;k<emboss_w;k++)
            {
                for(int l=0;l<emboss_h;l++)
                {
                    int xval = x-( (emboss_w-1)>>1)+k;
                    int yval = y-( (emboss_h-1)>>1)+l;

                    QRgb *sp = (QRgb *)pix.scanLine(yval) + xval;

                    r=qRed(*sp);
                    g=qGreen(*sp);
                    b=qBlue(*sp);
                    a = qAlpha(*sp);

                    sumr+=r*emboss_filter[k][l];
                    sumg+=g*emboss_filter[k][l];
                    sumb+=b*emboss_filter[k][l];

                }
            }

            QRgb *p = (QRgb *)buffer->scanLine(y) + x;

            sumr/=emboss_sum; sumr+=128;
            if(sumr>255) sumr=255; if(sumr<0)   sumr=0;

            //added these
            sumg/=emboss_sum; sumg+=128;
            if(sumg>255) sumg=255; if(sumg<0)   sumg=0;

            sumb/=emboss_sum; sumb+=128;
            if(sumb>255) sumb=255; if(sumb<0)   sumb=0;

            //now write out the destination pixel
            //*p = qRgb(sumr,sumr,sumr);

            *p = qRgba(sumr,sumg,sumb, a);
        }
    }

    //now we must copy buffer into pix
    pix = buffer->copy();
	delete buffer;

}

void GLWorld::doEdge(JahLayer* base_layer,  QImage& pix)
{
    int r,g,b,a;
    //int grey;
    QString conv,text;
    bool overlay;

    overlay = true; //false is black background, true adds effect to image

    int yy = (int)base_layer->textureData.height;
    int xx = (int)base_layer->textureData.width;

    int r1, g1, b1;
    int r2, g2, b2;
    int addvalx,addvaly;

    int sensitivity = (int)getCurrentEffectLayer()->layernodes->m_node->rx+1;
    int edgecolor   = (int)getCurrentEffectLayer()->layernodes->m_node->ry+50;
    int backcolor   = (int)getCurrentEffectLayer()->layernodes->m_node->rz;

    for ( int y=0; y<yy; y++ )
    {
        for ( int x=0; x<xx; x++ )
        {
            //this is the source and destination pixel
            QRgb *p = (QRgb *)pix.scanLine(y) + x;
            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p);

            //bounds checking here for algorithim
            if (y==yy-1) addvaly = 0; else addvaly = 1;
            if (x==xx-1) addvalx = 0; else addvalx = 1;

            //these are neighbouring pixels for the algorithim
            QRgb *p1 = (QRgb *)pix.scanLine(y) + x+addvalx;
            r1=qRed(*p1);
            g1=qGreen(*p1);
            b1=qBlue(*p1);

            QRgb *p2 = (QRgb *)pix.scanLine(y+addvaly) + x;
            r2=qRed(*p2);
            g2=qGreen(*p2);
            b2=qBlue(*p2);
            a=qAlpha(*p2);

            //this is the main algorithim
            if((sqrtf((r-r1)*(r-r1)+(g-g1)*(g-g1)+(b-b1)*(b-b1))>=sensitivity)||
                (sqrtf((r-r2)*(r-r2)+(g-g2)*(g-g2)+(b-b2)*(b-b2))>=sensitivity))
            {
                r = r+edgecolor; if (r>255) r=255;
                g = g+edgecolor; if (g>255) g=255;
                b = b+edgecolor; if (b>255) b=255;
            }
            else
            {
                r = r-backcolor; if (r<0) r=0;
                g = g-backcolor; if (g<0) g=0;
                b = b-backcolor; if (b<0) b=0;
            }

            //now write out the destination pixel
            *p = qRgba(r,g,b,a);

        }
    }

}

void GLWorld::doColorSelect(JahLayer* base_layer,  QImage& pix)
{
    int r,g,b,a;
    //int grey;
    QString conv,text;
    bool overlay;

    overlay = true; //false is black background, true adds effect to image

    int yy = (int)base_layer->textureData.height;
    int xx = (int)base_layer->textureData.width;

    //int r1, g1, b1;
    //int r2, g2, b2;
    //int addvalx,addvaly;

    int sensitivity = (int)getCurrentEffectLayer()->layernodes->m_node->rx;

    int r0   = (int)getCurrentEffectLayer()->layernodes->m_node->rx;
    int g0   = (int)getCurrentEffectLayer()->layernodes->m_node->ry;
    int b0   = (int)getCurrentEffectLayer()->layernodes->m_node->rz;

    for ( int y=0; y<yy; y++ )
    {
        for ( int x=0; x<xx; x++ )
        {
            //this is the source and destination pixel
            QRgb *p = (QRgb *)pix.scanLine(y) + x;
            r=qRed(*p);
            g=qGreen(*p);
            b=qBlue(*p);
            a=qAlpha(*p);

            //this is the main algorithim
            if(sqrtf((r-r0)*(r-r0)+(g-g0)*(g-g0)+(b-b0)*(b-b0))<=sensitivity)
            {
                r=0; //r = r+edgecolor; if (r>255) r=255;
                g=0; //g = g+edgecolor; if (g>255) g=255;
                b=0; //b = b+edgecolor; if (b>255) b=255;
            }
            else
            {
                r=255; //r = r-backcolor; if (r<0) r=0;
                g=255; //g = g-backcolor; if (g<0) g=0;
                b=255; //b = b-backcolor; if (b<0) b=0;
            }

            //now write out the destination pixel
            *p = qRgba(r,g,b,a);

        }
    }

}

void GLWorld::doCPUColorize(JahLayer* base_layer,  QImage& pix)
{   //debug (" in GLWorld::doCPUColorize " );
    float4          rgba;
    float4          color_scale;
    float4          enhance;
    float4          hsv;

    motionNode* sliders = getCurrentEffectLayer()->layernodes->m_node;

    color_scale.x = sliders->rx / 4.0f;
    color_scale.y = sliders->ry / 4.0f;
    color_scale.z = sliders->rz / 4.0f;
    float brightness = sliders->ty / 4.0f;  // brightness is transY.

    hsv.x = (sliders->sx - 1) / 10; // -0.5 to +0.5
    hsv.y = ( (sliders->sy - 1) * 2 / 5 ) + 1.0 ; // changed range. See sathelperslot too  
    hsv.z = (sliders->sz - 1) * 100;

    // hue offset.
    enhance.x = (sliders->tx ); // 0-3600
    // strength slider (NB we cant really have negative values, but deal with it later. keep centred around zero).
    enhance.z = ( sliders->tz ); // 0-100 original, shifted
  
    color_scale.w = 0.0f;

    float gamma = sliders->Alpha / 100.0f;
    float gamma_coefficient = 1.0f;

    int height = (int)base_layer->textureData.height;
    int width = (int)base_layer->textureData.width;

    bool do_gamma_correction = false;

    if (sliders->Alpha <= 99.0f || sliders->Alpha >= 101.0f)
    {
        do_gamma_correction = true;
        gamma_coefficient = powf(255.0f, gamma - 1.0f);
    }

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; x++ )
        {
            QRgb* pixel = (QRgb*)pix.scanLine(y) + x;
            rgba.x = float( qRed(*pixel) );
            rgba.y = float( qGreen(*pixel) );
            rgba.z = float( qBlue(*pixel) );

            rgba += brightness;
            rgba += color_scale;

            color_enhance2(rgba, hsv, enhance );

            if (do_gamma_correction)
            {
                rgba = float4::pow( rgba, gamma );

                if (gamma_coefficient != 0.0f)
                {
                    rgba /= gamma_coefficient;
                }
            }

            rgba.clamp();

            rgba.w = float( qAlpha(*pixel) );

            *pixel = qRgba(int(rgba.x), int(rgba.y), int(rgba.z), int(rgba.w) );
        }
    }
}

void GLWorld::doGaussianBlur(JahLayer* base_layer,  QImage& pix) {

    // GimpPixelRgn src_rgn, dest_rgn;
    int                     width, height;
    int                     bytes;
    int                     has_alpha;
    uchar*                  dst; 
    uchar*                  dst_ptr;
    uchar*                  src;
    uchar*                  src_ptr;
    int*                    buf; 
    int*                    buf_ptr; 
    int*                    bb;
    int                     pixels;
    int                     total = 1;
    int                     i, row, column, b;
    int                     start, end;
    int*                    curve;
    int*                    sum_first = NULL;
    int*                    sum = NULL;
    int                     val;
    int                     length = 0;
    int                     initial_p, initial_m;
    double                  std_dev;
    double                  horz;
    double                  vert;
    double                  temp;
    Gaussian_blur_c         blur;
    QImage                  newimage; 
    uint                    pixel_data;

    if (pix.numColors() != 0)
    {
        return;
    }

    horz =  (double)abs((int)getCurrentEffectLayer()->layernodes->m_node->rx) / 2.0;
    vert =  (double)abs((int)getCurrentEffectLayer()->layernodes->m_node->ry) / 2.0;

    if (horz <= 0.0 && vert <= 0.0)
    {
        return;
    }

    //gimp_drawable_mask_bounds (drawable->drawable_id, &x1, &y1, &x2, &y2);

    width = (int)base_layer->textureData.width;
    height = (int)base_layer->textureData.height;

    if (width < 1 || height < 1) { return; }

    bytes = 4;  // Always with QT (including alpha)
    has_alpha = TRUE;

    buf = new int [MAX(width, height) * 2];
	buf_ptr = buf;
    src = new uchar [MAX(width, height) * bytes];
    dst = new uchar [MAX(width, height) * bytes];

    //  First the vertical pass  
    if (vert > 0.0)
    {
        vert = fabs(vert) + 1.0;
        std_dev = sqrt (-(vert * vert) / (2 * log(1.0 / 255.0)));

        curve = blur.makeCurve(std_dev, &length);
        //sum = g_new (int, 2 * length + 1);
        //sum = (int*)calloc((2 * length) + 1, sizeof(int));
        sum_first = new int [(2 * length) + 1];
		sum = sum_first;

        sum[0] = 0;

        for (i = 1; i <= length*2; i++)
        {
            sum[i] = curve[i - length - 1] + sum[i-1];
        }

        sum += length;

        total = sum[length] - sum[-length];

        for (column = 0; column < width; column++)
        {
            //gimp_pixel_rgn_get_col (&src_rgn, src, column + x1, y1, (y2 - y1));

            // Gimp uses red, green, blue, alpha and Qt uses alpha, red, green,
            // blue, so the ordering of the bytes needs to be changed for gimp
            // ported routines.
            for (row = 0; row < height; row++)
            {
                (*(uint*)&src[row * bytes]) = (uint)pix.pixel(column, row);;
            }

            if (has_alpha)
            {
                blur.multiplyAlpha(src, height, bytes);
            }

            src_ptr = src;
            dst_ptr = dst;

            for (b = 0; b < bytes; b++)
            {
                initial_p = src_ptr[b];
                initial_m = src_ptr[(height-1) * bytes + b];

                //  Determine a run-length encoded version of the row  
                blur.runLengthEncode(src_ptr + b, buf_ptr, bytes, height);

                for (row = 0; row < height; row++)
                {
                    start = (row < length) ? -row : -length;
                    end = (height <= (row + length) ? (height - row - 1) : length);

                    val = 0;
                    i = start;
                    bb = buf + ((row + i) * 2);

                    if (start != -length)
                    {
                        val += initial_p * (sum[start] - sum[-length]);
                    }

                    while (i < end)
                    {
                        pixels = bb[0];
                        i += pixels;

                        if (i > end)
                        {
                            i = end;
                        }

                        val += bb[1] * (sum[i] - sum[start]);
                        bb += (pixels * 2);
                        start = i;
                    }

                    if (end != length)
                    {
                        val += initial_m * (sum[length] - sum[end]);
                    }

                    dst_ptr[row * bytes + b] = val / total;
                }
            }

            if (has_alpha)
            {
                blur.separateAlpha(dst, height, bytes);
            }

            //gimp_pixel_rgn_set_col (&dest_rgn, dst, column + x1, y1, (y2 - y1));
            for (row = 0; row < height; row++)
            {
                pixel_data = *(uint*)&dst[row * bytes + 0];
                //newimage.setPixel(column, row, pixel_data);
                pix.setPixel(column, row, pixel_data);
            }
        }

		if (sum_first != NULL)
		{
			delete [] sum_first;
			sum_first = NULL;
			sum = NULL;
		}
    }

	//  Now the horizontal pass  
    if (horz > 0.0)
    {
        horz = fabs(horz) + 1.0;

        //if (horz != vert)
        //{
            temp = -(horz * horz) / (2 * log(1.0 / 255.0));
            std_dev = sqrt(temp);

            curve = blur.makeCurve(std_dev, &length);

            sum_first = new int[(2 * length) + 1];
			sum = sum_first;

            sum[0] = 0;

            for (i = 1; i <= length*2; i++)
            {
                sum[i] = curve[i-length-1] + sum[i-1];
            }

            sum += length;

            total = sum[length] - sum[-length];
//        }

        for (row = 0; row < height; row++)
        {
            //gimp_pixel_rgn_get_row (&src_rgn, src, x1, row + y1, (x2 - x1));
            for (column = 0; column < width; column++)
            {
                (*(uint*)&src[column * bytes]) = (uint)pix.pixel(column, row);;

            } 

            if (has_alpha)
            {
                blur.multiplyAlpha(src, width, bytes);
            }

            src_ptr = src;
            dst_ptr = dst;

            for (b = 0; b < bytes; b++)
            {
                initial_p = src_ptr[b];
                initial_m = src_ptr[(width-1) * bytes + b];

                // Determine a run-length encoded version of the row
                blur.runLengthEncode(src_ptr + b, buf_ptr, bytes, width);

                for (column = 0; column < width; column++)
                {
                    start = (column < length) ? -column : -length;
                    end = (width <= (column + length)) ? (width - column - 1) : length;

                    val = 0;
                    i = start;
                    bb = buf + (column + i) * 2;

                    if (start != -length)
                    {
                        val += initial_p * (sum[start] - sum[-length]);
                    }

                    while (i < end)
                    {
                        pixels = bb[0];
                        i += pixels;

                        if (i > end)
                        {
                            i = end;
                        }

                        val += bb[1] * (sum[i] - sum[start]);
                        bb += (pixels * 2);
                        start = i;
                    }

                    if (end != length)
                    {
                        val += initial_m * (sum[length] - sum[end]);
                    }

                    dst_ptr[column * bytes + b] = val / total;
                }
            }

            if (has_alpha)
            {
                blur.separateAlpha(dst, width, bytes);
            }

            //gimp_pixel_rgn_set_row (&dest_rgn, dst, x1, row + y1, (x2 - x1));
            for (column = 0; column < width; column++)
            {
                pixel_data = *(uint*)&dst[column * bytes + 0];
                pix.setPixel(column, row, pixel_data);
            } 
        }

		if (sum_first != NULL)
		{
			delete [] sum_first;
		}
    }

    delete [] buf;
    delete [] src;
    delete [] dst;

}


void 
GLWorld::doChromaKeyerCPU(JahLayer* base_layer, QImage& pix) 
{
    motionNode* chroma_values = getCurrentEffectLayer()->layernodes->m_node;

    float red_low = chroma_values->rx;
    float red_high = chroma_values->tx;
    float green_low = chroma_values->ry;
    float green_high = chroma_values->ty;
    float blue_low = chroma_values->rz;
    float blue_high = chroma_values->tz;
    float soften_value = ( (chroma_values->sx - 1.0f) / 5.0f * 255.0f );

    float red;
    float green;
    float blue;
    float alpha;

    int height = (int)base_layer->textureData.height;
    int width = (int)base_layer->textureData.width;

    for ( int y = 0; y < height; y++ )
    {
        for ( int x = 0; x < width; x++ )
        {
            QRgb* pixel = (QRgb*)pix.scanLine(y) + x;
            red = float( qRed(*pixel) );
            green = float( qGreen(*pixel) );
            blue = float( qBlue(*pixel) );
            alpha = 255.0f;

            if (blue > green)
            {
                if (soften_value >= blue)
                {
                    blue = 0.0f;
                }
                else
                {
                    blue -= soften_value;
                }

                if (blue < green)
                {
                    blue = green;
                }
            }

            if (
                (red >= red_low && red <= red_high) &&
                (green >= green_low && green <= green_high) &&
                (blue >= blue_low && blue <= blue_high)
                )
            {
                alpha = 0.0f;
            }

            *pixel = qRgba(int(red), int(green), int(blue), int(alpha) );
        }
    }
}

void
GLWorld::CPUChromaKeyer(JahLayer* key_layer, int buffer_width, int buffer_height, GLuint key_texture_id)
{
    int lower_left_pixel_offset_x = ( m_pCore->getRenderWidth() - buffer_width ) / 2;
    int lower_left_pixel_offset_y = ( m_pCore->getRenderHeight() - buffer_height ) / 2;

    int key_texture_width;
    int key_texture_height;

    getPowerOfTwoTextureSize(key_texture_width, key_texture_height, buffer_width, buffer_height);

    int buffer_width_div_2 = buffer_width / 2;
    int buffer_height_div_2 = buffer_height / 2;

    int color_buffer_size = buffer_width * buffer_height * 4;
    int texture_size = key_texture_width * key_texture_height * 4;

    unsigned char* base_color_buffer = new unsigned char[texture_size];
    unsigned char* key_color_buffer = new unsigned char[texture_size];

    float key_texture_width_ratio = float(buffer_width) / float(key_texture_width);
    float key_texture_height_ratio = float(buffer_height) / float(key_texture_height);

    if (m_bUseTexturesForReadback)
    {
        readTextureSubImageUchar(
            key_texture_id, key_texture_width, key_texture_height,
            lower_left_pixel_offset_x, lower_left_pixel_offset_y,
            base_color_buffer, buffer_width, buffer_height);
    }
    else
    {
        glReadPixels(
            lower_left_pixel_offset_x,
            lower_left_pixel_offset_y,
            buffer_width,
            buffer_height,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            base_color_buffer);
    }

    if (key_layer->layerStatus)
    {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        m_pRenderSpace->drawLayer(m_pTitleSafe, true);  
        m_pRenderSpace->drawLayer(key_layer, true);

        if (m_bUseTexturesForReadback)
        {
            readTextureSubImageUchar(
                key_texture_id, key_texture_width, key_texture_height,
                lower_left_pixel_offset_x, lower_left_pixel_offset_y,
                key_color_buffer, buffer_width, buffer_height);
        }
        else
        {
            glReadPixels(
                lower_left_pixel_offset_x,
                lower_left_pixel_offset_y,
                buffer_width,
                buffer_height,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                key_color_buffer);
        }

        for (int i = 0; i < color_buffer_size; i += 4)
        {
            unsigned char* red      = &key_color_buffer[i];
            unsigned char* green    = &key_color_buffer[i + 1];
            unsigned char* blue     = &key_color_buffer[i + 2];
            unsigned char* alpha    = &key_color_buffer[i + 3];

            bool clear = *red == 0 && *green == 0 && *blue == 0;

            // If the key layer pixel falls within the key color range, replace
            // it with the corresponding base image pixel
            if (m_bSelectColorsIndividually && getCurrentEffectLayer()->layerStatus)
            {
                if (clear || (*red > m_ChromaKeyerRange.red_low && 
			      *red < m_ChromaKeyerRange.red_high) )
                {
                    *red = base_color_buffer[i];
                }

                if (clear || 
		    (*green != 0 && *green > m_ChromaKeyerRange.green_low
		     && *green < m_ChromaKeyerRange.green_high) )
                {
                    *green = base_color_buffer[i + 1];
                }

                if (clear || (*blue != 0 && 
			      *blue > m_ChromaKeyerRange.alpha_low && 
			      *blue < m_ChromaKeyerRange.alpha_high ) )
                {
                    *blue = base_color_buffer[i + 2];
                }

                if (clear || (*alpha != 0 && 
			      *alpha > m_ChromaKeyerRange.alpha_low && 
			      *alpha < m_ChromaKeyerRange.alpha_high) )
                {
                    *alpha = base_color_buffer[i + 3];
                }
            }
            else if (getCurrentEffectLayer()->layerStatus)
            {
                if (   
                    ( (*red > m_ChromaKeyerRange.red_low && 
		       *red < m_ChromaKeyerRange.red_high) &&
                      (*green > m_ChromaKeyerRange.green_low && 
		       *green < m_ChromaKeyerRange.green_high) &&
		      (*blue > m_ChromaKeyerRange.blue_low && 
		       *blue < m_ChromaKeyerRange.blue_high) ) || clear
                    )
                {
                    *((unsigned int*)&key_color_buffer[i]) = 
                        *((unsigned int*)&base_color_buffer[i]);
                }
            }
        }

        glBindTexture(GL_TEXTURE_2D, key_texture_id); 

        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, buffer_width, buffer_height,
            GL_RGBA, GL_UNSIGNED_BYTE, key_color_buffer);

        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0.0f, 0.0f);           
            glVertex3f(-buffer_width_div_2, -buffer_height_div_2, 0.0f);

            glTexCoord2f(key_texture_width_ratio, 0.0f);           
            glVertex3f(buffer_width_div_2, -buffer_height_div_2,  0.0f);

            glTexCoord2f(key_texture_width_ratio, key_texture_height_ratio);           
            glVertex3f(buffer_width_div_2, buffer_height_div_2,  0.0f);

            glTexCoord2f(0.0f, key_texture_height_ratio);           
            glVertex3f(-buffer_width_div_2, buffer_height_div_2,  0.0f);
        }
        glEnd(); // GL_QUADS
    }

    delete [] key_color_buffer;
    delete [] base_color_buffer;
}

