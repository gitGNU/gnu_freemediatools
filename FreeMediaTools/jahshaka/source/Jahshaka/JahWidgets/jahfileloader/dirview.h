/*******************************************************************************
**
** The header file for the Jahshaka dirview module 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DIRVIEW_H
#define DIRVIEW_H

#include <qlistview.h>
#include <qstring.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qtimer.h>


//this is for assetmanagement support
#include "DataSource.h"

class QWidget;
class QDragEnterEvent;
class QDragMoveEvent;
class QDragLeaveEvent;
class QDropEvent;

class FileItem : public QListViewItem
{
public:
    FileItem( QListViewItem *parent, const QString &s1, const QString &s2 )
	: QListViewItem( parent, s1, s2 ), pix( 0 ) {}

    const QPixmap *pixmap( int i ) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QListViewItem::setPixmap;
#endif
    void setPixmap( QPixmap *p );

private:
    QPixmap *pix;

};

class Directory : public QListViewItem
{
public:

    enum Type{ FileFolder, Collection, SearchResult, LocalDataSource, RemoteDataSource, 
               ComputerRoot, DriveRoot, SearchRoot, MediaRoot, TrashCan };

    Directory( QListView * parent, QString filename, Type type = FileFolder );
    Directory( Directory * parent, QString filename, Type type = FileFolder );

   // Directory( Directory * parent, const QString& filename, const QString &col2 ) // TODO remove this constr?
//	: QListViewItem( parent, filename, col2 ), pix( 0 ) {}

    QString text( int column ) const;
    QString text( int column, bool showstatus ) const;

    void setType( Type type ) { dtype = type; setPixmap(); };
    Type dirType() const { return dtype;};

    // Sets the name. Use this instead of calling setText directly!
    //only works on windows using the ==, settext needed on linux?
    #ifdef WIN32
    void setName( const QString& text ) { f = text; };
    #else
    void setName( const QString& text ) { f.setName(text); };
    #endif
    
    QString fullName();
	QString getRoot( ) { return f.name( ); }

    // Returns true if network library is readable (false if not logged in, server down etc).
    bool isReadable() const { return readable; };
    // For folders, the readable flag is set based on whether the folder can be accessed. For other items, 
    // it can be set directly here (e.g. if a server is disconnected, set readable to false ).
    void setReadable( bool state ) { readable = state; };

    // A 'size' to show in brackets after the name. Not all items actually know their size until opened.
    void setSizeToShow( int size ) { sizetoshow = size; } ;
    int sizeToShow() { return sizetoshow; };

    void setOpen( bool );
    void setup();
    void setShowDirsOnly( bool state ) { showDirsOnly = state; };

    const QPixmap *pixmap( int i ) const;

    // For settting the data source. TODO: just via constr?

    void setDataSource( olib::openassetlib::DataSource *ds ) { dsource = ds; };
    // Note, can return zero if no data source.
    //olib::openassetlib::
    olib::openassetlib::DataSource* dataSource() { return dsource; };

    // Adds collection items as child of this item, if any exist. Applies to LocalDataSource and RemoteDataSource types only.
    void populateCollections();
    

#if !defined(Q_NO_USING_KEYWORD)
    using QListViewItem::setPixmap;
#endif
    void setPixmap( QPixmap *p );
	void setPixmap( int col, const QPixmap &pm ) { QListViewItem::setPixmap( col, pm ); };

    void paintCell(  QPainter * p, const QColorGroup & cg, int column, int width, int align );

protected:

    void init();

    // sets the pixmap based on dirtype.
    void setPixmap();

private:

    olib::openassetlib::DataSource * dsource;

    QFile f;
    Directory * p;
    
    // readable flag is used for folders and servers, the latter may be disconnected etc.
    bool readable;
    
    bool showDirsOnly;
    QPixmap *pix;

    Type dtype;

    int sizetoshow;

};

class DirectoryView : public QListView
{
    Q_OBJECT

public:
    DirectoryView( QWidget *parent = 0, const char *name = 0, bool sdo = FALSE );
    bool showDirsOnly() { return dirsOnly; }
    void setShowDirsOnly( bool state ) { dirsOnly = state; };

public slots:
    void setDir( const QString & );

    // Asset Management slots. All work on the currently SELECTED directory ( watch out if we switch to
    // multiple selection mode at some point). 
    void newCollection();
    void renameCollection();
    void deleteCollection( Directory *dir );
    void deleteSelectedCollection();
    void collapseCurrentBranch();


signals:
    void folderSelected( const QString & );
    void collectionDeleted();

protected slots:
    void slotFolderSelected( QListViewItem * );
    void slotFolderSelectedViaPress( int button, QListViewItem * item, const QPoint & pos, int c );

    // To avoid sending too many signals we only call this selection slot after a delay (same trick is 
    // used in Windows, 'explorer/folder view'. This is not called when Dnd is opening the folders.
    void delayedSlotFolderSelection();
    void selectionChangedHelperSlot();

    void openFolder();

    void contextMenuCreator( QListViewItem *, const QPoint &, int );

protected:
    void contentsDragEnterEvent( QDragEnterEvent *e );
    void contentsDragMoveEvent( QDragMoveEvent *e );
    void contentsDragLeaveEvent( QDragLeaveEvent *e );
    void contentsDropEvent( QDropEvent *e );
    void contentsMouseMoveEvent( QMouseEvent *e );
    void contentsMousePressEvent( QMouseEvent *e );
    void contentsMouseReleaseEvent( QMouseEvent *e );
    void contentsMouseDoubleClickEvent ( QMouseEvent * e );  // (double clicks now ignored).

    void keyPressEvent ( QKeyEvent * e );

private:


    QString fullPath(QListViewItem* item);
    bool dirsOnly;
    QListViewItem *oldCurrent; // used for drag opening
    QListViewItem *dropItem;
    QListViewItem *oldSelected; // used for clicking/selected.
    QTimer* autoopen_timer;
    QTimer* selectopen_timer;
    QPoint presspos;
    bool mousePressed;


};

#endif
