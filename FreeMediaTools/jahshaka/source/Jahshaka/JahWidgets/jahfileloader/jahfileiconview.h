/*******************************************************************************
**
** The header file for the Jahshaka jahfilewindow module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHFILEICONVIEW_H
#define JAHFILEICONVIEW_H 

//see
//void QFileDialog::setContentsPreview ( QWidget * w, QFilePreview * preview ) 

#include <qiconset.h>
#include <qstring.h>
#include <qfileinfo.h>
#include <qdir.h>
#include <qtimer.h>
#include <qiconview.h>

#include "Asset.h"
#include "iconprovider.h" // BAZ TEST

class QtFileIconView;
class QDragObject;
class QResizeEvent;

/*****************************************************************************
 *
 * Class QtFileIconDrag
 *
 *****************************************************************************/

class QtFileIconDrag : public QIconDrag
{
    Q_OBJECT

public:
    QtFileIconDrag( QWidget * dragSource, const char* name = 0 );

    const char* format( int i ) const;
    QByteArray encodedData( const char* mime ) const;
    static bool canDecode( QMimeSource* e );
    void append( const QIconDragItem &item, const QRect &pr, const QRect &tr, const QString &url );

private:
    QStringList urls;

};

/*****************************************************************************
 *
 * Class QtFileIconView
 *
 *****************************************************************************/

class QtFileIconViewItem;
class QtFileIconView : public QIconView
{
    Q_OBJECT

public:
    QtFileIconView(  QWidget *parent = 0, const char *name = 0, const QString &dir = "" );

    enum ViewMode { Large, Small, Image, Browser };
    enum MediaFilter { AllMedia, ImageMedia, VideoMedia, AllFiles };

    void setViewMode( ViewMode m );
    void setMediaFilter( MediaFilter mf );
    ViewMode viewMode() const { return vm; }
    void setOpenItem( QtFileIconViewItem *i ) {openItem = i;}

    QPixmap generateFadedThumb( QPixmap* pix );
   
    bool clickstatus;

    // Sets the name filter ('glob') for the view.
    void setNameFilter( const QString& filter ) { nameFilter = filter + QString( " " ) + filter.upper( ); }

///////////////////////////////////////////////////
// had to move this code so it can be commented out! if newassetlib isnt defined
// the moc process doesnt recognize #ifdefs in slots so i made it public instead

void readAssets(  std::list<olib::openassetlib::Asset*> assets );
 
public slots:
    void setDirectory( const QString &dir );
    void setDirectory( const QDir &dir );

    void showCollection( QString path ); // path of form "Data source$collection name". TODO use proper uri?

    void newDirectory();
    QDir currentDir();
    QDir previousDir();
   

    void viewLarge();
    void viewSmall();
    void viewImage();
	void viewBrowser();

    void showMediaFilesOnly();
    void showImageFilesOnly();
    void showVideoFilesOnly();
    void showAudioFilesOnly();
    void showAllFiles();

    void clearAll() { clear(); };

signals:
    void directoryChanged( const QString & );
    void startReadDir( int dirs );
    void readNextDir();
    void readDirDone();
    void enableUp();
    void disableUp();
    void enableBack();
    void disableBack();
    void enableMkdir();
    void disableMkdir();
    
    //added by jahshaka
    void loadthisfile( QString );
    void setfilename( QString );

protected slots:
    void itemDoubleClicked( QIconViewItem *i );
    void itemClicked( QIconViewItem *i );

    void slotDropped( QDropEvent *e, const QValueList<QIconDragItem> & );


    void viewBottom();
    void viewRight();
    void flowEast();
    void flowSouth();
    void itemTextTruncate();
    void itemTextWordWrap();
    void sortAscending();
    void sortDescending();
    void arrangeItemsInGrid() {
	QIconView::arrangeItemsInGrid( TRUE );
    }

    void slotRightPressed( QIconViewItem *item );
    void openFolder();

protected:
    void readDir( const QDir &dir );
    virtual QDragObject *dragObject();

    virtual void keyPressEvent( QKeyEvent *e );
	virtual void resizeEvent( QResizeEvent * e );

    void addToDirHistory( QDir dir );

    QDir viewDir;
    QDir oldviewDir; // used for going 'back'. TO REMOVE
    QList<QDir> dirHistory; 

    QString nameFilter;
    QStringList nameFilterAsList;
    void updateNameFilterList();
    bool checkAgainstNameFilter( const QString& filename ); // to check asset names

    int newFolderNum;
    QSize sz;
    QPixmap pix;
    ViewMode vm;
    QtFileIconViewItem *openItem;

};

/*****************************************************************************
 *
 * Class QtFileIconViewItem
 *
 *****************************************************************************/

class QtFileIconViewItem : public QIconViewItem
{
    friend class QtFileIconView;

public:
    enum ItemType {
        File = 0,
        Dir,
        Link
    };

    QtFileIconViewItem( QtFileIconView *parent, QFileInfo *fi );
	QtFileIconViewItem( QtFileIconView *parent, olib::openassetlib::Asset *passet, QFileInfo *fi );

	void init();

    virtual ~QtFileIconViewItem();

    ItemType type() const
    { return itemType; }
    QString filename() const { return itemFileName; }

    virtual bool acceptDrop( const QMimeSource *e ) const;

    virtual void setText( const QString &text );
    virtual QPixmap *pixmap() const;

    virtual void dragEntered();
    virtual void dragLeft();

    void viewModeChanged( QtFileIconView::ViewMode m );
   // void paintItem( QPainter *p, const QColorGroup &cg );

    void setSelected( bool s ); // TEST OVERRIDE
    void setSelected( bool s, bool cb ); // TEST OVERRIDE

    bool isAsset() { return passet; };

    olib::openassetlib::Asset* asset() { return passet; };

protected:
    void initFileInfo( QFileInfo *fi );

    virtual void dropped( QDropEvent *e, const QValueList<QIconDragItem> & );

    void generateThumbnail( const QString &filename ) const;
	void generateBanner() const;

    virtual void paintItem ( QPainter * p, const QColorGroup & cg );
	virtual void calcRect(const QString &text_ = QString::null);

    QString itemFileName;
    QFileInfo *itemFileInfo;
    ItemType itemType;
    bool checkSetText;
    QTimer timer;
    QtFileIconView::ViewMode vm;

    olib::openassetlib::Asset* passet; // only set for assets

    mutable QPixmap *thumbnail; // thumbnail cache
	mutable QPixmap *banner; // banner cache for brower

    static IconProvider *iconprovider; // BAZ
};

#endif
