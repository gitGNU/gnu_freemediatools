/*******************************************************************************
**
** The source file for the Jahshaka jahnodes class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "keyframer.h"

// qt
#include <qlistbox.h>
#include <qstringlist.h>

// canvas was 700x200
// main frame was 650x180 (!)
// look out for the magic numbers!

const char* items[] = { "tx", "ty", "tz",
			"rx", "ry", "rz",
			"sx", "sy", "sz",
			NULL };

JahKeyframer::JahKeyframer(QWidget*, QWidget* parent, const char* name, WFlags f) 
    : QWidget(parent,name,f),
      zoomlevel( 4.0 ),
      m_zoom_in_y_button( NULL ),
      m_zoom_out_y_button( NULL ),
      m_shift_y_up_button( NULL ),
      m_shift_y_down_button( NULL )
{
    jtrace = JahTrace::getInstance();	//set up tracer
    
    keyframeruiLayout = new QHBoxLayout( this, 11, 6, "keyframeruiLayout"); 

    traces = new QListBox( this );
    traces->setSelectionMode( QListBox::Extended );
    connect( traces, SIGNAL( highlighted( const QString& ) ), this, SLOT( traceSelectionChanged() ) );
    keyframeruiLayout->addWidget( traces, 0 );

    traces->insertItem( "all" );
    unsigned int i=0;
    while( items[i] )
	traces->insertItem( items[i++] );
    traces->setSelected( 0, true );

    canvas = new QCanvas( this );
    connect( canvas, SIGNAL( resized() ), this, SLOT( canvasResized() ) );

    QColor back;
    back.setHsv(35,35,35);
    canvas->setBackgroundColor(back);

    keyeditor = new KeyframeEditor( canvas, this );
    connect( keyeditor, SIGNAL( signalKeyframeChanged() ), this, SLOT( slotKeyframeChanged() ) );
    keyframeruiLayout->addWidget( keyeditor, 10 );

    keycontrolsLayout = new QVBoxLayout();
    keyframeruiLayout->addLayout( keycontrolsLayout );

    QSpacerItem* spacer_1 = new QSpacerItem( 21, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    keycontrolsLayout->addItem( spacer_1 );

    m_zoom_in_y_button = new QPushButton( this, "m_zoom_in_y_button" );
    m_zoom_in_y_button->setFixedSize( QSize( 32, 32 ) );
    keycontrolsLayout->addWidget( m_zoom_in_y_button );
    connect( m_zoom_in_y_button, SIGNAL(clicked()), SLOT(slotZoomInY() ) );

    m_zoom_out_y_button = new QPushButton( this, "m_zoom_out_y_button" );
    m_zoom_out_y_button->setFixedSize( QSize( 32, 32 ) );
    keycontrolsLayout->addWidget( m_zoom_out_y_button );
    connect( m_zoom_out_y_button, SIGNAL(clicked()), SLOT(slotZoomOutY() ) );

    QSpacerItem* spacer_2 = new QSpacerItem( 21, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    keycontrolsLayout->addItem( spacer_2 );

    m_shift_y_up_button = new QPushButton( this, "m_shift_y_up_button" );
    m_shift_y_up_button->setFixedSize( QSize( 32, 32 ) );
    keycontrolsLayout->addWidget( m_shift_y_up_button );
    connect( m_shift_y_up_button, SIGNAL(clicked()), SLOT(slotShiftYUp() ) );

    m_shift_y_down_button = new QPushButton( this, "m_shift_y_down_button" );
    m_shift_y_down_button->setFixedSize( QSize( 32, 32 ) );
    keycontrolsLayout->addWidget( m_shift_y_down_button );
    connect( m_shift_y_down_button, SIGNAL(clicked()), SLOT(slotShiftYDown() ) );

    QSpacerItem* spacer_3 = new QSpacerItem( 21, 41, QSizePolicy::Minimum, QSizePolicy::Expanding );
    keycontrolsLayout->addItem( spacer_3 );

    languageChange();

    static int r=24;
    srand(++r);

    clear();

    buildInterface();
    addPlayLine();
    m_current_nodelist = NULL;
}

void JahKeyframer::languageChange()
{
    //setCaption( tr( "Form1" ) );
    m_zoom_in_y_button->setText( tr( "+" ) );
    m_zoom_out_y_button->setText( tr( "-" ) );
    m_shift_y_up_button->setText( tr( "Up" ) );
    m_shift_y_down_button->setText( tr( "Dn" ) );
}

JahKeyframer::~JahKeyframer()
{   //delete printer; 
}

void JahKeyframer::clear() 
{   
    m_interfaceH.reset( NULL );
    m_interfaceV.reset( NULL );
    m_center_line.reset(NULL);
    keyeditor->cleareditor();
}

void JahKeyframer::enlarge()
{    
    //canvas->resize(canvas.width()*4/3, canvas.height()*4/3);
}

void JahKeyframer::shrink()
{    
    //canvas->resize(canvas.width()*3/4, canvas.height()*3/4);
}

void JahKeyframer::zoomIn()
{   
    QWMatrix m = keyeditor->worldMatrix();    
    m.scale( 2.0, 2.0 );    
    keyeditor->setWorldMatrix( m );
}

void JahKeyframer::zoomOut()
{    
    QWMatrix m = keyeditor->worldMatrix();    
    m.scale( 0.5, 0.5 );    
    keyeditor->setWorldMatrix( m );
}


//qt signals are here
void JahKeyframer::emitAddKeyframe(void)    { emit AddKeyframe(); }
void JahKeyframer::emitRemoveKeyframe(void) { emit RemoveKeyframe(); }
void JahKeyframer::emitPrevKeyframe(void)   { emit PrevKeyframe(); }
void JahKeyframer::emitNextKeyframe(void)   { emit NextKeyframe(); }

//this one draws random keyframes for testing purposes
void JahKeyframer::updateKeyframes()
{ 
    clear();
    buildInterface();

    if ( m_current_nodelist )
    {
        buildKeyframes( getMaxFrame(), getCurrentFrame(), m_current_nodelist );
    }

    traceSelectionChanged();
    canvas->setAllChanged();
    canvas->update();

    keyeditor->update();
}

void JahKeyframer::updateKeyframes(int max_frame, int current_frame, QPtrList<motionNode>* nodelist)
{   
    clear();
    buildInterface();
    buildKeyframes(max_frame, current_frame, nodelist);
    setMaxFrame(max_frame);
    setCurrentFrame(current_frame);
    m_current_nodelist = nodelist;

    canvas->setAllChanged();
    canvas->update();

    keyeditor->update();
}

void JahKeyframer::buildInterface()
{
    if ( m_interfaceH.get() || m_interfaceV.get() || m_center_line.get() )
    {
        return;
    }

    const int OFFSET = 3;
    int w = canvas->width();
    int h = canvas->height();
    //int insetW = w - OFFSET;

    m_interfaceV.reset( new QCanvasLine(canvas) );
    m_interfaceV->setPen( QPen(Qt::green) );
    m_interfaceV->setPoints( OFFSET, 0, OFFSET, h );
    m_interfaceV->setZ(128);
    m_interfaceV->show();

    m_center_line.reset( new QCanvasLine(canvas) );
    m_center_line->setPen( QPen(Qt::green) );
    m_center_line->setPoints( 0, (h / 2) + Globals::getKeyframerYOffset(), 
                              w, (h / 2) + Globals::getKeyframerYOffset() );
    m_center_line->setZ(128);
    m_center_line->show();
}

void JahKeyframer::drawLine(int x1, int y1, int x2, int y2)
{
    QCanvasLine* i = new QCanvasLine(canvas);
    i->setPoints( x1, y1,	  x2,y2 );
    i->setPen( QPen(Qt::white) );
    i->setZ(128);
    i->show();

}

//////////////////////////////////////////
// adds the playbar vertical line
void JahKeyframer::addPlayLine()
{
    playline.reset( new SliderItem(canvas) );
    playline->setPoints(0, 0, 0, canvas->height() );

    playline->setPen( QPen(QColor(100,0,0), 6) );
    playline->setZ(3);
    playline->show();
}

void JahKeyframer::movePlayLine( int keyframe )
{
    //printf("movePlayLine keyframe %i\n", keyframe );
    if ( playline.get() )
    {   
        int x_location = int( double(keyframe) * zoomlevel );
        x_location = jahstd::Max<int>(x_location, 5);
        playline->move( x_location, playline->y() );
        
        canvas->update();
    }
}

//initially this will be display only :)
//with zoom and scroll and timeline 
void JahKeyframer::buildKeyframes()
{
    //jtrace->debug("JahKeyframer::buildKeyframes()","in empty buildkeyframes");
}

//initially this will be display only :)
//with zoom and scroll and timeline 
void JahKeyframer::buildKeyframes(int max_frame, int current_frame, QPtrList<motionNode> * nodelist)
{
    jtrace->debug( "JahKeyframer::","running buildKeyframes");
    jtrace->debug( "numnodes", nodelist->count());

    buildKeyframeSequence(max_frame, current_frame, "tx", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "ty", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "tz", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "rx", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "ry", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "rz", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "sx", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "sy", nodelist );
    buildKeyframeSequence(max_frame, current_frame, "sz", nodelist );
}

void JahKeyframer::buildKeyframeSequence(int, int current_frame, QString property_qstring, QPtrList<motionNode> * nodelist)
{
    if ( !nodelist->count() )
    {
        return;
    }

    int nodecount = nodelist->count();
    std::string property_string = property_qstring.data();

    motionNode* motion_node = nodelist->last();
    int last_keyframe_time = motion_node->m_frame_number;

    zoomlevel = 4.0;

    if ( nodelist->count() )
    {
        if (last_keyframe_time > 0.0)
        {
            zoomlevel = (double)canvas->width() / (last_keyframe_time + 1.0);
        }
    }

    movePlayLine(current_frame);

    QMemArray<KeyframeItem*> lastRow(nodecount);

    KeyframeItem* previous_keyframe_item = NULL;

    motionNode* keyframe = nodelist->first();

    QColor point_color = pointColor(property_string);

    for ( ; keyframe; keyframe = nodelist->next() )
    {
        float* keyframe_value_ptr = propertyPtr(property_string, keyframe);
        KeyframeItem* new_keyframe_item = new KeyframeItem(point_color, canvas, keyframe_value_ptr);
	keyeditor->add( property_qstring, new_keyframe_item );

        int xval; 
        int yval = 0;

        xval = int( double(keyframe->m_frame_number) * zoomlevel );

        float current_keyframe_value = *keyframe_value_ptr;

        if (property_qstring == "sx" || property_qstring == "sy" || property_qstring == "sz")
        {
            yval = -(int)( (current_keyframe_value - 1.0f) * 100.0f * Globals::getKeyframerZoomFactor() / KEYFRAMER_Y_SCALE_FACTOR );
        }
        else
        {
            yval = -(int)( current_keyframe_value * Globals::getKeyframerZoomFactor() / KEYFRAMER_Y_SCALE_FACTOR );
        }
        
        yval += ( (canvas->height() / 2) + Globals::getKeyframerYOffset() );

        new_keyframe_item->move( xval, yval);

        // This is necessary to restore because the move() call will change the keyframe value
        *keyframe_value_ptr = current_keyframe_value;

        KeyEdgeItem* new_key_edge_item = NULL;

        //if i=0 first keyframe draw line from offscreen infinity to current point
        if ( keyframe == nodelist->getFirst() )  
        { 
            //new_key_edge_item = new KeyEdgeItem( 0, yval, new_keyframe_item, canvas );
        }

        //if i=nodes last keyframe draw line to offscreen infinity from current point
        if ( keyframe == nodelist->getLast() ) 
        { 
            //new_key_edge_item = new KeyEdgeItem( new_keyframe_item, 800, yval, canvas );
        }

        if ( previous_keyframe_item ) 
        { 
            new_key_edge_item = new KeyEdgeItem( previous_keyframe_item, new_keyframe_item, canvas );
	    keyeditor->add( property_qstring, new_key_edge_item );
        }

        if ( keyframe != nodelist->getFirst() ) 
        {
            lastRow[nodelist->at()-1] = previous_keyframe_item;
        }

        previous_keyframe_item = new_keyframe_item;

        if (new_key_edge_item)
        {
            new_key_edge_item->show();
        }

        if (new_keyframe_item)
        {
            new_keyframe_item->show();
        }
    }
}

QColor 
JahKeyframer::pointColor(std::string property)
{
    if ( property == "tx" )
    {
        return (Qt::red);
    }
    else if ( property == "ty" )
    {
        return (Qt::green);
    }
    else if ( property == "tz" )
    {
        return (Qt::blue);
    }
    else if ( property == "rx" )
    {
        return (Qt::cyan);
    }
    else if ( property == "ry" )
    {
        return (Qt::magenta);
    }
    else if ( property == "rz" )
    {
        return (Qt::yellow);
    }
    else if ( property == "sx" )
    {
        return (KEYFRAMER_SCALE_X_COLOR);
    }
    else if ( property == "sy" )
    {
        return (KEYFRAMER_SCALE_Y_COLOR);
    }
    else if ( property == "sz" )
    {
        return (KEYFRAMER_SCALE_Z_COLOR);
    }
    else 
    {
        return (Qt::red);
    }
}

float* 
JahKeyframer::propertyPtr(std::string property, motionNode* keyframe)
{
    if ( property == "tx" )
    {
        return ( &keyframe->tx );
    }
    else if ( property == "ty" )
    {
        return ( &keyframe->ty );
    }
    else if ( property == "tz" )
    {
        return ( &keyframe->tz ); 
    }
    else if ( property == "rx" )
    {
        return ( &keyframe->rx ); 
    }
    else if ( property == "ry" )
    {
        return ( &keyframe->ry ); 
    }
    else if ( property == "rz" )
    {
        return ( &keyframe->rz ); 
    }
    else if ( property == "sx" )
    {
        return ( &keyframe->sx ); 
    }
    else if ( property == "sy" )
    {
        return ( &keyframe->sy ); 
    }
    else if ( property == "sz" )
    {
        return ( &keyframe->sz ); 
    }
    else 
    {
        return NULL; 
    }
}

void JahKeyframer::canvasResized()
{
    m_interfaceV.reset( NULL );
    m_interfaceH.reset( NULL );
    m_center_line.reset( NULL );
    playline.reset( NULL );
    
    buildInterface();
    addPlayLine();  

    if ( m_current_nodelist )
    {
        updateKeyframes( getMaxFrame(), getCurrentFrame(), m_current_nodelist );
    }
}

void
JahKeyframer::slotKeyframeChanged()
{
    emit signalKeyframeChanged();
}

void
JahKeyframer::slotZoomInY()
{
    float zoom_factor = Globals::getKeyframerZoomFactor();
    zoom_factor *= 1.5f;
    Globals::setKeyframerZoomFactor(zoom_factor);
    updateKeyframes();
}

void
JahKeyframer::slotZoomOutY()
{
    float zoom_factor = Globals::getKeyframerZoomFactor();
    zoom_factor /= 1.5f;
    Globals::setKeyframerZoomFactor(zoom_factor);
    updateKeyframes();
}

void
JahKeyframer::slotShiftYUp()
{
    int offset = Globals::getKeyframerYOffset();
    offset -= 5;
    Globals::setKeyframerYOffset(offset);
    updateKeyframes();
}

void
JahKeyframer::slotShiftYDown()
{
    int offset = Globals::getKeyframerYOffset();
    offset += 5;
    Globals::setKeyframerYOffset(offset);
    updateKeyframes();
}

void JahKeyframer::traceSelectionChanged()
{
    // find all selected items
    QStringList selected;
    for ( unsigned int i=0; i<traces->count(); ++i )
    {
	if ( traces->isSelected( i ) )
	    selected.append( traces->text( i ) );
    }

    keyeditor->show( selected );
}
