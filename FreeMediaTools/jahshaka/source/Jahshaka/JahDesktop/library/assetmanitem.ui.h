/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/
#include <qfont.h>
#include <qfontmetrics.h>
#include <qplatinumstyle.h>
#include <qwindowsstyle.h>
#include <qstyle.h>
#include <qstylefactory.h>
#include <qobjectlist.h>
#include <qpainter.h>
#include <qrect.h>
#include <qtable.h>
#include <qfileinfo.h>
#include <qiconview.h>
#include "jahfileiconview.h"
#include "assetstringfunctions.h"

void AssetManItem::init()
{
    // comment out these 2 lines to hide the drop shadow effect
    //dropShadowPixmapLabel->setEnabled( false );
    //dropShadowPixmapLabel->setFixedHeight(0);

    //    dummyLabel1->hide();

    resetAllFields();

    // We want to overide jah system colours. Our UI colors are ignored, have to edit palette here.
    QStyle *platinum=QStyleFactory::create("platinum");
    if (platinum)
    {  this->setStyle( platinum );
    }

    QPalette p = this->palette();
    p.setColor(QColorGroup::Text, Qt::black); 
    p.setColor(QColorGroup::Background, QColor( 192, 192, 192)); 
    this->setPalette(p);

    setGroupBackgroundColor( 0, QColor(192, 192, 192 ) );

    QFont font = nameLineEdit->font();
    font.setPointSize( font.pointSize() + 2 );
    nameLineEdit->setFont ( font );

    mainFrame1->hide();
    mainFrame2->hide();
    resize( width(), groupHeight(0) );
    fullExpandButton->hide();
    collapseButton->hide();

    expandLevel = 0;

    expand();

    // could make sure no extra pixels to reduce height of group0?
    /* QFontMetrics fm( dateCreatedLabel->font() );
    dateCreatedLabel->setFixedHeight( fm.height() -2 );
    nameLineEdit->setFixedHeight (fm.height() -2 );
    expandButton->setFixedHeight (fm.height() -2 );
    */
}


void AssetManItem::expand()
{
    firstExpandButton->hide();

    if ( expandLevel == 0 )
    {   mainFrame0->show();
    mainFrame1->show();
    setGroupBackgroundColor( 0, QColor(143, 168, 198 ) );
    setGroupBackgroundColor( 1, QColor(143, 168, 198 ) );
    fullExpandButton->hide();
    collapseButton->show();
    expandButton->setText("Minimise Info");
    expandLevel = 1; 
    emit assetExpanded( assetNumber );
    }
    else if ( expandLevel == 1 )
    {   mainFrame0->show();
    mainFrame1->hide();
    setGroupBackgroundColor( 0, QColor(192, 192, 192 ) );
    fullExpandButton->show();
    collapseButton->hide();
    expandLevel = 0;
    expandButton->setText("Expand Info");
    repaint();
    emit assetExpanded( assetNumber );   
    }

}

int AssetManItem::groupHeight( int groupnum )
{
    if ( groupnum == 0 )
    {  
        return ( dropShadowPixmapLabel->height() + 
            nameLineEdit->sizeHint().height() ); // was + dateCreatedLabel->sizeHint().height() but moved
        //return ( dropShadowPixmapLabel->sizeHint().height() + 
        //  nameLineEdit->sizeHint().height() + dateCreatedLabel->sizeHint().height() + 0 );
    }
    else if ( groupnum == 1 )
    {   
        return ( fileTypeLabel->sizeHint().height() + trtLabel->sizeHint().height() + framesizeLabel->sizeHint().height() + compressionLabel->sizeHint().height()  + 0 );
        //return ( fileTypeLabel->height() + trtLabel->height() + framesizeLabel->height() + compressionLabel->height()  + 0 );
    }

    else if ( groupnum == 2 )
        return ( tempLabel->sizeHint().height() + 0 );

    else return 0; 
}

void AssetManItem::setGroupBackgroundColor( int,  QColor )
{
    //TEST  for now, return...!!
    return;
    // ******************************

#if 0
    /// QPalette p = this->palette();
    // p.setColor(QColorGroup::Background, col );
    if ( groupnum == 0 )
    { 
        QPalette p = mainFrame0->palette();
        p.setColor(QColorGroup::Background, col );
        p.setColor(QColorGroup::Text, Qt::black);
        mainFrame0->setPalette(p);
        mainFrame0->setBackgroundColor( col );

        const QObjectList *childw = mainFrame0->children();

        QObjectList::Iterator it = childw->begin();
        while ( it != childw->end() )
        {   
            if ((*it)->isWidgetType() )
            {
                QWidget* mywidget = (QWidget*)(*it);
                mywidget->setBackgroundColor( col );
                p = mywidget->palette();
                p.setColor(QColorGroup::Background, col );
                p.setColor(QColorGroup::Button, col );
                mywidget->setPalette(p);
            }
            ++it;
        }
    }
#endif
}

void AssetManItem::setThumbImage( QImage theimage )
{
    if ( theimage.isNull() )
        return;

    float ratio = (float)theimage.width() / (float)theimage.height();
    int w = 159;
    int h = int( w / ratio );

    if ( h > 104 )
    {
        h = 104;
        w = int( h * ratio );
    }

    QPixmap pix = theimage.smoothScale( w, h );

    // Previously, used a png backdrop. Looked pretty good, could keep as option. 

    // QImage img("assetmanitem-bg.png");   // TO EMBED
    //QImage img("assetmanitem-bg-noshadow.png"); // no shadow version

    // We now want the bg to be able to exactly fit the image we just scaled + its own border/shadow etc.
    // These border values will change if you change the bg pixmap!
    //QPixmap bg = img.smoothScale( w + 13, h + 10);

    // New method paints a bg rect internally.
    QPixmap bg( w+16, h+16 );
    QPainter p;
    p.begin(&bg);
    p.fillRect( 0, 0, bg.width(), bg.height(), colorGroup().background() );
    p.setPen( QColor( 30, 27, 42 ) );
    p.setBrush( QColor( 30, 27, 42 ) );
    p.drawRoundRect( 0, 0, bg.width(), bg.height(), 5, 5 );

    int dx = 8;
    int dy = 8;
    QRect pixmaprect( dx, dy, pix.width(), pix.height() );
    p.drawPixmap( pixmaprect, pix );

    // round off corners
    //p->setPen( cg.base() );
    p.drawPoint( pixmaprect.topLeft() ); 
    p.drawPoint( pixmaprect.bottomLeft() );
    p.drawPoint( pixmaprect.topRight() );
    p.drawPoint( pixmaprect.bottomRight() );

    p.end();

    previewLabel->setPixmap( bg );
}


void AssetManItem::setInfoFromFile( const QString& filename )
{
    QString conv;
    // We set what we can from the filename. Description/meta data fields are disabled (cant store them!).
    QFileInfo fi( filename );
    QImage image ( filename ); 
    if ( !fi.exists() )
    { resetAllFields();
    return;
    }
    // For now, can only process QImages
    if ( image.isNull() )
    { resetAllFields();
    return;
    }

    descriptionTextEdit->setText( "(only assets in a database can have this information)");

    nameLineEdit->setText( "   " +  fi.fileName() );

    if ( !image.isNull() )
    { setThumbImage( image );
    fileTypeValueLabel->setText( "Image");
    }
    else
    { // SET DEFAULT THUMB?
    }

    // Set the labels:
    dateCreatedValueLabel->setText( AssetStringFunctions::formattedDateString(fi ) );
    framesizeValueLabel->setText( AssetStringFunctions::frameSizeString( image ) );
    sizeValueLabel->setText( AssetStringFunctions::fileSizeString( fi ) );

}

void AssetManItem::resetAllFields()
{
    QString st = "-";
    nameLineEdit->setText("(No item to view)");
    dateCreatedValueLabel->setText( st );
    sizeValueLabel->setText(st);
    descriptionTextEdit->setText("");
    descriptionTextEdit->setEnabled( false );
    fileTypeValueLabel->setText( st );
    trtValueLabel->setText(st);
    framesizeValueLabel->setText(st);
    compressionValueLabel->setText(st);
    tagValueLabel->setText( st );
    formatValueLabel->setText( st );
    audioValueLabel->setText(st );
    licenceValueLabel->setText( st );

    nameLineEdit->setReadOnly( true );
    descriptionTextEdit->setReadOnly( true );

    QImage emptyimage( "emptymedia.png" ); // TODO embed
    setThumbImage( emptyimage );

}


void AssetManItem::setInfoFromIconViewItem( QIconViewItem *item )
{
    // This fn assumes we are using the jahiconview. 
    if ( item == 0 )
    { resetAllFields();
    return;
    }

    QtFileIconViewItem *fitem = dynamic_cast<QtFileIconViewItem*>( item ); 
    if ( fitem == 0 )
    { return;
    }

    setInfoFromFile( fitem->filename() );

}


void AssetManItem::newSlot()
{

}
