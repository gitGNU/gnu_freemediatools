/*******************************************************************************
**
** The source file for the Jahshaka RenderObject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <jahrender.h>

////////////////////////////////////////////////////
// this routine draws layers and objects
// it calls the necessary object routine which then draws itself

void JahRender::makeObject( void ) 
{	
    //first we select the texture map for the layer this should be in the loops so its not called 2 much
    //only also if a asset is present...

    if (thelayer->layerAssetStatus || thelayer->keyAssetStatus)
    {
        thelayer->setLayerTexture(); 
    }

    switch ( thelayer->layertype ) 
    {
        case LayerCategory::LAYER     :  
        {  
            if (thelayer->drawtheLayer) 
            {
                switch ( thelayer->objtype ) 
                {
                    //background layers
                    case ObjectCategory::BACKDROP     :     {    makeBackground();          break; } // no trim
                    case ObjectCategory::MAINBACK     :     {    makeBlackBackground();     break; } //w red trim

                    case ObjectCategory::JAHLAYER     :     {    makeLayer(thelayer);       break; }
                    case ObjectCategory::MESHLAYER    :     {    makeMeshLayer();           break; }
                    case ObjectCategory::LARGE_LAYER  :     {    makeLargeLayer(thelayer);  break; }

                    case ObjectCategory::CUBE         :     {	 makeCube();				break; }
                    case ObjectCategory::SPHERE       :     {	 makeSphere();              break; }
                    case ObjectCategory::CYLINDER     :     {    makeCylinder();            break; }
                    case ObjectCategory::BEZIERS      :     {    makeBezier();              break; }

                    case ObjectCategory::TITLEGRID    :     {    makeTitleGrid();           break; }

                    case ObjectCategory::JAHFOG       :     {    /*layer->makeFog();*/      break; }
                    default                           :     {                               break; }
                }

                break; 
            }
        }

        case LayerCategory::PARTICLE  :    
        {  
            if (thelayer->loadParticles)
            {
                thelayer->loadParticles = false;
                addParticleObj();
            }	

            makeParticle();   
            break; 
        }



        case LayerCategory::LIGHT     :    
        { 
            JahLayer* parent = thelayer->getParent();

            if ( parent && ( parent->layerIsEnabled() || parent->isCameraLayer() ) )
            {
                drawLight(); 
            }

            break; 
        }

        case LayerCategory::OBJECT   :    
        { 
            if (thelayer->loadObject)
            {
                thelayer->loadObject = false;
                loadObj( );	// calls openObject
            }	

            makeObj( ); //  calls drawObject();                
            break; 
        }

        //frame objecttype is obsolete was replaced by titlegrid
        case LayerCategory::FRAME     :  
        {  
            makeTitleGrid();           
            break; 
        }

        case LayerCategory::TEXT      :  
        {     //we dont want to do this every time so we need a flag...
            //ie loadFontText
            if (thelayer->updateFont)
            {
                thelayer->updateFont = false;
                makeText();
            }
            else
            {
                drawText();
            }

            break; 
        }

        case LayerCategory::GRID      :  {  makeGrid();                 break; }

        default        :  {                                   break; }
    }

    //layer should clean up after itself
    if (thelayer->layerAssetStatus || thelayer->keyAssetStatus)
    {
        glBindTexture(GL_TEXTURE_2D, 0); 
    }
}

////////////////////////////////////////////////////
// this routine draws the key since it is actually
// a separate layer

void JahRender::makeKey(float offset) 
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, thelayer->textureKey.texID );

    switch ( thelayer->objtype ) 
    {
        case ObjectCategory::JAHLAYER :
        case ObjectCategory::MESHLAYER :
        {    
            makeKeyLayer(offset);            
            break; 
        }
        default          :     
        {                                      
            break; 
        }
    }

    glDisable(GL_TEXTURE_2D);
}


////////////////////////////////////////////////////
// this routine draws the outline or object control / GUI widgets
// these subroutines shouldnt be in the layer code...
// should be here or in the world as its a gui element
// but currently they are attached to the layers themselves

// we could query the layer and get its position and draw the elements here...

void JahRender::makeTheLayerOutline( void) 
{
    switch ( thelayer->layertype ) 
    {
        case LayerCategory::LAYER     :  
        {  
            if (thelayer->objtype==ObjectCategory::MAINBACK || thelayer->objtype==ObjectCategory::BACKDROP) 
            {
                //makes the blue one
                makeBackdropOutline(); 
            }
            else 
            {

                switch(theToolMode)
                {
                case MTRANSLATE : { makeLayerTranslate();   break; } 
                case MSCALE     : { makeLayerScale();   break; } 
                case MROTATE    : { makeLayerRotate();    break; } 
                case MOFF       : { break; }
                }
            }
            break;
        }
        case LayerCategory::LIGHT     :  
        {  
            //makeLayerRotate();
            break;
        }

        case LayerCategory::TEXT      :  
        {    
            break;
        }

        default        :  {                               break; }
    }
}
