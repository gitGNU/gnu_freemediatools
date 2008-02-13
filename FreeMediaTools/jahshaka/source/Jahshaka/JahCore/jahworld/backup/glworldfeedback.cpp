#include "glworld.h"
#include <qpopupmenu.h>
#include "glworldlists.h"
#include "glcore.h"
#include "jahrender.h"
#include <qcursor.h>
#include <qlabel.h>


// Subroutine to determine which object is selected in
// On Screen selections
void GLWorld::contextMenuEvent(QContextMenuEvent * e)
{
    jtrace->debug("GLWorld","QContextMenuEvent()");

    e->accept();		// put this at the start or end?

    QPopupMenu* contextMenu = new QPopupMenu( this );
    Q_CHECK_PTR( contextMenu );
    QLabel *caption = new QLabel( "<font color=darkblue><u><b>"
        "Context Menu</b></u></font>", this );
    caption->setAlignment( Qt::AlignCenter );
    contextMenu->insertItem( caption );
    contextMenu->insertItem( "Add layer",  this, SLOT(addLayer())  );

    QPopupMenu *submenu = new QPopupMenu( this );
    Q_CHECK_PTR( submenu );
    submenu->insertItem( "Translate",   this, SLOT(toolTranslate()) );
    submenu->insertItem( "Rotate",      this, SLOT(toolRotate()) );
    submenu->insertItem( "Scale",       this, SLOT(toolScale()) );

    contextMenu->insertItem( "Tools", submenu );


    contextMenu->exec( QCursor::pos() );

    delete contextMenu;
}

void GLWorld::mousePressEvent( QMouseEvent * ) {

    /////////// check for object selection
    m_pJtrace->debug("GLWorld::","mousePressEvent() on screen selection");

    updateGL();

    m_bMousePressed = TRUE;

}

void GLWorld::processSelection(GLuint *pSelectBuff)
{
    int id;
	int count;
    // How many names on the name stack
    count = pSelectBuff[0];
    // Bottom of the name stack
    id = pSelectBuff[3];

    m_pJtrace->debug("GLWorld::processSelection ...you clicked",id);

    //QString clicked;  qDebug("you clicked"+clicked.setNum(id) );

//we are getting the wrong id and it crashes


    if ( id < int( getLayerList()->count() ) )
    {
        getActiveJahLayer()->selected=FALSE;
        LayerListEntry* layer_list_entry = getLayerList()->at(id);
        setActiveJahLayer( layer_list_entry->getJahLayer() );
        getActiveJahLayer()->selected=TRUE;
    }
    else
    {
        m_pJtrace->debug("GLWorld::processSelection error, selection out of range",id);
    }
}

void GLWorld::mouseReleaseEvent( QMouseEvent * ) 
{
    //m_pJtrace->debug("GLWorld::","mouseReleaseEvent() on screen selection");
    m_bMousePressed = FALSE; 
}

//need to update this routine for camera moves
void GLWorld::mouseMoveEvent( QMouseEvent *e ) 
{
    //m_pJtrace->debug("GLWorld::","mouseMoveEvent() on screen selection");

    if ( m_MMode == MOFF || m_bMousePressed==FALSE) return;

    int x = e->x();
    int y = e->y();

    if (getActiveJahLayer() == m_pCamera) 
    {   
        mouseMoveCamera(x,y);            
    }
    else                   
    { 
        mouseMoveLayer(getActiveJahLayer(), x, y); 
    }

    updateGL();

    //updateSliders();
    updateSliderValues();
    slotUpdateListViewCheckBoxes();

    m_nLastX = x;
    m_nLastY = y;

}

void GLWorld::keyPressEvent( QKeyEvent * )
{
    m_pJtrace->debug( "GLWorld::", "pressed key in module "+
		      m_qsModuleName);

    //need to send this back up to the parent?
}


//onscreen select tools and widgets
void GLWorld::noTool( void ) 
{ 
    m_MMode = MOFF;  
    updateToolDisplay(); 
    updateGL(); 
}
void GLWorld::toolTranslate( void ) 
{ 
    m_MMode = MTRANSLATE;  
    updateToolDisplay(); 
    updateGL(); 
}


void GLWorld::toolScale( void )     
{ 
    m_MMode = MSCALE;      
    updateToolDisplay(); 
    updateGL(); 
}

void GLWorld::toolRotate( void )    
{
    m_MMode = MROTATE;     
    updateToolDisplay(); 
    updateGL(); 
}



void GLWorld::updateToolDisplay(void)
{
    m_pRenderSpace->setToolMode(m_MMode);
}

void 
GLWorld::mouseMoveLayer (int, int, int)
{
}

void 
GLWorld::mouseMoveLayer ( JahLayer* jah_layer, int x, int y )
{
    motionNode* motion_node = jah_layer->layernodes->m_node;

    switch ( m_MMode ) {

        case MROTATE    : { if ( x>m_nLastX)      { motion_node->ry +=10.0f; }
                            else if ( x<m_nLastX) { motion_node->ry -=10.0f; }
                            if ( y>m_nLastY)      { motion_node->rx +=10.0f; }
                            else if ( y<m_nLastY) { motion_node->rx -=10.0f; }
                            break; }

        case MTRANSLATE : { if ( x>m_nLastX)      { motion_node->tx +=5.0f; }
                            else if ( x<m_nLastX) { motion_node->tx -=5.0f; }
                            if ( y>m_nLastY)      { motion_node->ty -=5.0f; }
                            else if ( y<m_nLastY) { motion_node->ty +=5.0f; }
                            break; }

        case MSCALE     : { if ( x>m_nLastX)      { motion_node->sx +=0.1f; }
                            else if ( x<m_nLastX) { motion_node->sx -=0.1f; }
                            if ( y>m_nLastY)      { motion_node->sy -=0.1f; }
                            else if ( y<m_nLastY) { motion_node->sy +=0.1f; }
                            break; }

        case MOFF       : {  break; }

        default         : {  break; }

    }

}

void GLWorld::mouseMoveCamera( int x,int y )
{

    //this will be used to apply zdepth to movements for sensitivity
    GLfloat coef;  
    coef = m_pCamera->layernodes->m_node->tz;             //        (GLfloat)(degrees )

    switch ( m_MMode ) {

        case MROTATE    : { if ( x>m_nLastX)         { m_pCamera->layernodes->m_node->ry +=10.0f; }
                            else if ( x<m_nLastX)    { m_pCamera->layernodes->m_node->ry -=10.0f; }
                            if ( y>m_nLastY)         { m_pCamera->layernodes->m_node->rx +=10.0f; }
                            else if ( y<m_nLastY)    { m_pCamera->layernodes->m_node->rx -=10.0f; }
                            break; }

        case MTRANSLATE : { if ( x>m_nLastX)         { m_pCamera->layernodes->m_node->tx +=5.0f; }
                            else if ( x<m_nLastX)    { m_pCamera->layernodes->m_node->tx -=5.0f; }
                            if ( y>m_nLastY)         { m_pCamera->layernodes->m_node->ty -=5.0f; }
                            else if ( y<m_nLastY)    { m_pCamera->layernodes->m_node->ty +=5.0f; }
                            break; }

        case MSCALE     : { if ( x>m_nLastX)         { m_pCamera->layernodes->m_node->sx +=0.1f; }
                            else if ( x<m_nLastX)    { m_pCamera->layernodes->m_node->sx -=0.1f; }
                            if ( y>m_nLastY)         { m_pCamera->layernodes->m_node->sy -=0.1f; }
                            else if ( y<m_nLastY)    { m_pCamera->layernodes->m_node->sy +=0.1f; }
                            break; }

        case MOFF       : { break; }

        default         : { break; }

    }

}








