/*******************************************************************************
**
** The source file for the Jahshaka desktop canvas file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/


#include "desktoptable.h"
#include "desktop.h"
#include "valueFormatters.h"

JahDesktopTable::JahDesktopTable( int, int, QWidget* parent, const char* name) :
 QTable(parent,name)
 {
	setNumCols( 8 );

    setFocusStyle(QTable::FollowStyle);
    setSelectionMode(QTable::SingleRow);

    QHeader *header = horizontalHeader();
        header->setLabel( 0,  QObject::tr( "Icon" ),		60  );
        header->setLabel( 1,  QObject::tr( "Name" ),		180 );
        header->setLabel( 2,  QObject::tr( "Num" ),			40  );
        header->setLabel( 3,  QObject::tr( "Location" ),	200 );
        header->setLabel( 4,  QObject::tr( "Filename" ),	180 );
        header->setLabel( 5,  QObject::tr( "Ext" ),			60  );
        header->setLabel( 6,  QObject::tr( "Start" ),       100  );
        header->setLabel( 7,  QObject::tr( "End" ),			100  );
    header->setMovingEnabled(FALSE);

    setLeftMargin(0);
    verticalHeader()->hide();

    jahnumRows = 0;
    
    // let the longest columns stretch to fill space
    setColumnStretchable ( 3, true ); 

    show();

    setDragEnabled(TRUE);
}


void JahDesktopTable::addAsset(int, int, assetData thedata, QImage theimage)
{
	//need to add the entireline here
	setNumRows(jahnumRows + 1);
    setRowHeight ( jahnumRows, 40 );
	
    //used to scale image down to size
    float ratio = (float)theimage.width() / (float)theimage.height();
	int w = 60;
	int h = int( w / ratio );

	if ( h > 40 )
	{
		h = 40;
		w = int( h * ratio );
	}

    QPixmap pix = theimage.scale( w, h );

    //used for num to text conversion as table dopesnt seem to hold integers
    QString conv;

	TimecodeValueFormatter formatter;

    setPixmap( jahnumRows, 0 , pix );
    setText  ( jahnumRows, 1 , thedata.clipname );
    setText  ( jahnumRows, 2 , conv.setNum(thedata.clipnumber) );
    setText  ( jahnumRows, 3 , thedata.location );
    setText  ( jahnumRows, 4 , thedata.filename );
    setText  ( jahnumRows, 5 , thedata.extension );
    setText  ( jahnumRows, 6 , formatter.convert(thedata.startframe) );
    setText  ( jahnumRows, 7 , formatter.convert(thedata.endframe) );

	setRowReadOnly(jahnumRows, true);

	jahnumRows +=1 ;
}

QDragObject * JahDesktopTable::dragObject()
{
    QImage image;
    QPixmap pix;

    pix = pixmap (currentRow(),0);
    image = pix;

    return new QImageDrag(image,this);
}


bool JahDesktopTable::removeItem(int theitem)
{
	//QString rere;
	//debug("--------------------------------");
	//debug("calling remove for item id"+rere.setNum(theitem));

    bool itworked = false;

    //theitem -=1;

    //we need to scan all rows
    //and find out which row contains theitem in col 10
    int therow = 0;
    QString thetext;
    int theval;

    //should be start row to end row
    for (int x=0; x<=jahnumRows; x++)
    {
        //thetext = text(x,10);
        thetext = text(x,2);
        theval = thetext.toInt();

        if (theval == theitem)
        {
            therow = x;
            itworked = true;
			break;
        }
    }

    if (!itworked)
    {
        //debug("it didnt work");
    }
    else
    {
		removeRow(therow);
		selectRow( therow );
		jahnumRows -=1;
    }

    return itworked;
}

void JahDesktopTable::renameAsset( int id, QString newname )
{
    for ( int row = 0; row < numRows(); row++ ) 
	{
		if ( id == (text( row, 2 )).toInt() )
        {   setText( row, 1, newname);
            update();
        }
    }
}

void JahDesktopTable::paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg )
{
    QColorGroup ourcg( cg );
    if ( row % 2 ) // alternate colors
        ourcg.setColor( QColorGroup::Base, cg.base().dark( 110 ) ) ;  
    QTable::paintCell( p, row, col, cr, selected, ourcg );
}

bool JahDesktopTable::clearAll(void)
{
	setNumRows( 0 );
	jahnumRows = 0;
	show();
	assetData dummy;
	AssetTables::getInstance( )->setSelectedAsset( dummy );
	return true;
}

