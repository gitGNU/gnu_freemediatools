/*******************************************************************************
**
** The source file for the Jahshaka jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <jahnodes.h>

//for preferences global
#include "jahpreferences.h"
#include "jahformatter.h"

static uint mainCount = 0;

JahNodes::JahNodes(QWidget* parent, const char*, WFlags )
	: tb( NULL )
	, tp( NULL )
{

    jtrace = JahTrace::getInstance();	//set up tracer
	
    canvas = new QCanvas(parent);
    QColor back;
    back.setHsv(35,35,35);
    canvas->setBackgroundColor(back);
    canvas->resize( 1270, 700 );

    nodeeditor = new NodeEditor(canvas,parent);
    nodeeditor->resize(1270, 700 );  
    nodeeditor->setHScrollBarMode( NodeEditor::AlwaysOff );
    nodeeditor->setVScrollBarMode( NodeEditor::AlwaysOff );

    static int r=24;   srand(++r);

    mainCount++;

    buildInterface();

    nodenumber = 0;

    ///////////////////////////////////////////////
    // the linked list is here
    list = new QPtrList<LayerList>;
    list->setAutoDelete( TRUE );     // delete items when they are removed

}


JahNodes::~JahNodes()
{
}

void JahNodes::clear() 
{ 
    setAssetName( "Empty Layer" );
    JahPrefs& jprefs = JahPrefs::getInstance();
    QString JahBasePath( jprefs.getBasePath( ).data() );
    QImage theimage = QImage( JahBasePath + "Pixmaps/jahlayer.png"); 
    QImage imgx = theimage.smoothScale( 100, 65, QImage::ScaleMin );
    setMainImage( imgx );

    for ( int i = 0;  i < nodenumber; i++ )
        delNodes( i );

    nodenumber = 0;

    //  Delete all lines, buildNodes will remake the line network.
    QCanvasItemList l = canvas->allItems();
     for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) 
     {  (*it)->hide(); // TODO remove this and see the bugs...
        (*it)->move( -100, -100); // ditto. canvas ellipse bound region prob, but its our fault for painting outside it.
        if ( (*it)->rtti() == QCanvasLine::RTTI )
        {        (*it)->hide();
                //(*it)->setCanvas(0); 
                //delete (*it); // safe? TOFIX
        }
     }

    delete list;
    list = new QPtrList<LayerList>;
    list->setAutoDelete( TRUE );     // delete items when they are removed
    
    canvas->setAllChanged();
    canvas->update();
    nodeeditor->update();

}

void JahNodes::enlarge()
{    //canvas.resize(canvas.width()*4/3, canvas.height()*4/3);
}

void JahNodes::shrink()
{    //canvas.resize(canvas.width()*3/4, canvas.height()*3/4);
}

void JahNodes::zoomIn()
{    //QWMatrix m = editor->worldMatrix();    m.scale( 2.0, 2.0 );    editor->setWorldMatrix( m );
}

void JahNodes::zoomOut()
{    //QWMatrix m = editor->worldMatrix();    m.scale( 0.5, 0.5 );    editor->setWorldMatrix( m );
}


void JahNodes::buildInterface()
{
}

void JahNodes::setMainImage( QImage img )
{
    QImage imgx = img.smoothScale( 100, 65, QImage::ScaleMin );
	mainimage = imgx;
}

void JahNodes::drawLine(int x1, int y1, int x2, int y2)
{
    QCanvasLine* i = new QCanvasLine(canvas);
    i->setPoints( x1, y1,	  x2,y2 );
    i->setPen( QPen(Qt::white) );
    i->setZ(128);
    i->show();

}

//initially this will be display only :)
//with zoom and scroll and timeline 

void JahNodes::addNodes(int x, QString name)
{
    //first add to list of effects
    LayerList* new_layer_list = new LayerList(name, x);
    list->append(new_layer_list);

    //the increment effect number
   nodenumber++;

    //rebuilds the visual representation
    buildNodes(name);

    nodeeditor->clearSelection();
    selectNode( x );
}

void JahNodes::selectNode( int num )
{   // This function is hard work, we dont actually store our own list of nodes, have to ask canvas...
    
    QCanvasItemList l=canvas->allItems();

    for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) 
    {
         if ( (*it)->rtti() == nodeRTTI ) 
         {
            NodeItem *item= (NodeItem*)(*it);
            if ( item->layernumber == num + 1 )
            {   if ( nodeeditor->lastSelected() != 0 && nodeeditor->lastSelected() != item )
                    nodeeditor->clearSelection();
                item->setSelected( true );
                nodeeditor->setLastSelected( item ); 
                //printf("selectNode %i \n", num );
            }
         }
    }
    canvas->update();
}


void JahNodes::delNodes(int x )  
{   // TODO finish/test this fn.
 
    if ( x > nodenumber )
        return;

    nodeeditor->clearSelection(); // cant trust it after deletion.

    QString name =list->at(x)->name();

    // Deal with our on-screen node item
    list->at(x)->nodeItem()->hide();

    // Loop through the nodes below us to downgrade their layer numbers.
    // (so bad to have to do this! pointers to layers seems way better than indexing ...)
    for ( int i = x; i < nodenumber; i++ )
    {
        if ( i != x && i != nodenumber && i < int( list->count() ) && list->at(i) )
            list->at(i)->setLayerNumber( list->at(i)->layer() - 1 );
    }
 
    delete list->at(x)->nodeItem();
    list->at(x)->setNodeItem(0);

   list->remove ( list->at(x) );

    //printf("JahNodes delNodes %i \n", x );

    //the increment effect number
   nodenumber--;

   //  Delete all lines, buildNodes will remake the line network.
    QCanvasItemList l = canvas->allItems();
     for (QCanvasItemList::Iterator it=l.begin(); it!=l.end(); ++it) 
     {
        if ( (*it)->rtti() == QCanvasLine::RTTI )
        {        (*it)->hide();
                //(*it)->setCanvas(0);
                //delete (*it); // safe? TOFIX
        }
     }

    //rebuilds the visual representation
    buildNodes(""); // TODO more needed here
}

////////////////////////////////////////////////////
// we can store position data as well
//and use a signal / slot to update it when changed

void JahNodes::buildNodes(QString)
{
    //jtrace->debug( "JahNodes::","building nodes");
    
    int x0 = 100;    int y0 = 100;

    if ( !tb ) tb = new QBrush( Qt::blue );
    if ( !tp ) tp = new QPen( Qt::white );

    int nodecount = 0;

    const int dist = 100;

    //number of nodes
    int   nodes = nodenumber;

    QString conv;
    //conv.setNum( nodenumber );
    //debug ("nodenumber in buildNodes is " + conv );

    //use an array is cool
    //as we use an array for nodes right now
    QMemArray<NodeItem*> lastRow(nodes);
    //printf("nodenumber %i \n", nodenumber );
    NodeItem *prev = 0;
    //debug(">>> starting loop");
    for ( int i = 0; i < nodes; i++ )
    //for ( int i = nodes; i >= 0; i-- )
    {
        if ( i < int( list->count() ) && list->at(i) )
        {
            //printf("at(i) %i \n", i );

            QString name =list->at(i)->name();
            int thelayer = list->at(i)->layer();
            if (thelayer > 1 ) thelayer--; // the first layer is the image, the next is a container layer(?)

            //debug("get node item");
            NodeItem *el;
            bool newnode = false;
            if ( list->at(i)->nodeItem() == 0 ) // (has no nodeitem yet)
            {    el = new NodeItem( canvas,name,thelayer );
                 newnode = true;
                 //printf("made new node %i \n", i );
               
                 list->at(i)->setNodeItem( el ); 
                 //printf("for list set the node %i \n", i );
                
            }
            else
            {
                el = list->at(i)->nodeItem();
                el->setName( name );
                el->setLayerNumber ( thelayer );
            }

             //printf("about to check image %i \n", i );
             
            // Use the image thumbnail, if one is set.
             if ( nodecount == 0 && !mainimage.isNull() )
                  el->setImage( mainimage, canvas );
             if ( nodecount == 0 && assetname.length() > 0 )
                  el->setName( assetname );
          
            //debug("got node item");
            //debug ( "just added new node i= " + conv.setNum(i) + " name " + name + " layernum " + conv.setNum( thelayer ) );
            nodecount++;

            if ( newnode )
            {  //printf("newnode %i \n", i );
                el->move( y0, x0 + i*dist   );
                if ( prev )
                    el->move ( prev->x(), prev->y() + dist );
            }

            if ( prev )
            {
                //printf("new edge %i \n", i );
                NodesEdgeItem *edge = new NodesEdgeItem( prev, el, canvas );
                edge->show();
            }

            if ( i > 0 ) lastRow[i-1] = prev;
            
        
            prev = el;

            el->show();
        }
    }

    jtrace->debug(">>> built nodes");

    nodeeditor->update();
    canvas->setAllChanged();
    canvas->update();
}

