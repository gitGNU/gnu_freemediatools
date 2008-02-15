/*******************************************************************************
**
** The source file for the Jahshaka timeline object
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "jahtimeline.h"


EditDesktop::EditDesktop(QWidget* theparent, QWidget* parent, const char* name, WFlags f) :
    QWidget(parent,name,f) 
{
    clipnumber=1;
    canvas = new QCanvas(980,506);
    QColor back;
    back.setHsv(35,35,35);
    canvas->setBackgroundColor(back);
    canvas->resize( 980, 506 );

    editor = new EditEditor(canvas,this);
    editor->resize( 980, 506 );
    editor->setHScrollBarMode( EditEditor::AlwaysOff );
    editor->setVScrollBarMode( EditEditor::AlwaysOff );

    connect( editor,   SIGNAL(updateEditanimframe(int)),   theparent,   SLOT(updateEditanimframe(int)) );
    connect( editor,   SIGNAL(updateSetinFrames(int,int)), theparent,   SLOT(updateSetinFrames(int,int)) );

    init();
}

EditDesktop::~EditDesktop()
{
  delete canvas;
  delete editor;
}

void EditDesktop::init()
{
    clear();

    videolayer=1;   layer_height=25; layer_pos=0;

    //static int r=24;
    //srand(++r);

    int i;

    drawScale(canvas->width() );

    // create our video grid
    int numlines = (canvas->width() / 85);

    //these should not be moveable
    for ( i=1; i<numlines+2; i++) {
    addLines(canvas->width(),i );
    }

    addLine();
}


void EditDesktop::clear() 
{
    editor->cleareditor();
    //we need to erase all of the files here
}

void EditDesktop::addRectangle() 
{
    QCanvasPolygonalItem *i = new QCanvasRectangle( 0,0, 180,120,canvas);
    i->setBrush( QColor(50,0,200) );
    i->setPen( QPen(QColor(0,50,200), 6) );
    i->setZ(1);
    i->show();
}

QImage EditDesktop::getClip() 
{
    // change this to send a pointer instead of a qimage using globalobject!!!
    return editor->globalimage;
}

assetData EditDesktop::getVideoData() 
{
    return editor->selectedClip;
}

void EditDesktop::setClipname(QString& newname)
{
    editor->setItemtext(newname);
}

QString EditDesktop::getClipname() 
{
    // change this to send a pointer instead of a qimage using globalobject!!!
    return editor->imagetext;
}

/////////////////////////////////////////////
// draws the scale in the timeline

void EditDesktop::drawScale(int width)
{
    int scale = width / layer_height;

    QCanvasLine* i = new QCanvasLine(canvas);

    i->setPoints(0, 5, width, 5 );
    i->setPen( QPen(QColor(0,0,0), 3) );
    i->setZ(3);
    i->show();

    for ( int l=1; l<width; l+=scale ) {

        QCanvasLine* li = new QCanvasLine(canvas);
        li->setPoints(l, 5, l, 10 );
        li->setPen( QPen(QColor(0,0,0), 3) );
        li->setZ(3);
        li->show();
    }
}

//////////////////////////////////////////
// adds the playbar vertical line
void EditDesktop::addLine()
{
    QCanvasLine* i = new SliderItem(canvas);
    i->setPoints(canvas->width()/2, 0,
                canvas->width()/2, canvas->height() );

    i->setPen( QPen(QColor(100,0,0), 6) );
    i->setZ(3);
    i->show();
}



void EditDesktop::addLines(int, int numline)
{
    QCanvasLine* i = new QCanvasLine(canvas);

    int vpos = numline * (layer_height+2);

    i->setPoints( 0, vpos, canvas->width(), vpos );
    i->setPen( QPen(QColor(200,200,200), 2) );
    i->setZ(1);
    i->show();
}

void EditDesktop::addRectangle(int)
{
/*
    int vpos = numline * (layer_height+2);

    QCanvasPolygonalItem *i = new QCanvasRectangle(
        ((rand()%256)/2)     ,vpos+1,
        100+((rand()%256)*2) ,layer_height,         canvas);

    int z = 128+((rand()%256)/4);
    i->setBrush( QColor(z,z,z) );
    i->setPen( QPen(QColor(0,0,200), 1) );
    i->setZ(0);
    i->show();

    editor->refresh();
    */
}

///////////////////////////////////////////////////////////////////
//lets fix this routine asap!

//problem here is when we add the layer there is no videodata...

void EditDesktop::addEditLayer(int numline, bool, int, int endframe)
{
    //numline = numline - 1;

    //we need to read current videdata to get width
    int vpos = numline * (layer_height+2);
    int x1=1;
    int y1=vpos+1;
    int x2=(5*endframe)+1; //the 5 herer should be equalt to the timeline zoom
    int y2=layer_height;

    QCanvasPolygonalItem *i = new QCanvasRectangle( x1, y1, x2, y2, canvas);

    int z = 128; //+((rand()%256)/4);
    i->setBrush( QColor(z,z,z) );
    i->setPen( QPen(QColor(0,0,200), 1) );
    i->setZ(0);
    i->show();

    editor->refresh();
}

void EditDesktop::addEditLayer(int numline, assetData thedata)
{
    //numline = numline - 1;

    int vpos = numline * (layer_height+2);

    int x1=1;
    int y1=vpos+1;
    int x2=(5*thedata.endframe)+1; //the 5 herer should be equalt to the timeline zoom
    int y2=layer_height;

    //QString conv;
    //debug("numline:"+conv.setNum(numline));
    //debug("X1:"+conv.setNum(x1));    debug("Y1:"+conv.setNum(y1));
    //debug("X2:"+conv.setNum(x2));    debug("Y2:"+conv.setNum(y2));

    //numline is the layers number
    QCanvasPolygonalItem *i = new EditItem( thedata, canvas, numline, x1 ,y1, x2, y2 );

    int z = 128;
    //int z = 128+((rand()%256)/4);
    i->setBrush( QColor(z,z,z) );
    i->setPen( QPen(QColor(0,0,200), 1) );
    i->setZ(0);
    i->show();

    editor->refresh();
}


// MISSING FUNCTIONS
/*
EditDesktop::EditDesktop(QWidget*, QWidget*, char const*, unsigned int)
{  };


void EditDesktop::addEditLayer(int, bool, int, int)
{  };
*/
