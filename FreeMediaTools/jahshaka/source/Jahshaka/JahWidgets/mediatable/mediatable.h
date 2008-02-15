/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DESKTOPSIDETABLE_H
#define DESKTOPSIDETABLE_H

#include <qtable.h>
#include <qcanvas.h>

#include "assetdata.h"
#include "assettable.h"

class FigureEditor;

class QPushButton;
class QLabel;

// Custom table item, which knows about an asset.
class MediaTableItem : public QTableItem
{
public:
    MediaTableItem( QTable *table, assetData assetdata );

    assetData theasset;

};

// Used by JahDesktopSideTable. Its a QTable with Dnd feature.
class MediaTable : public QTable
{
    Q_OBJECT

public:
    MediaTable(int rows=0, int cols=0, QWidget* parent=0, const char* name=0);

signals:
    void gotFocus( );

protected:
    virtual void keyPressEvent ( QKeyEvent * e ); // BAZ TEST
    void contentsMousePressEvent ( QMouseEvent *e );
    void dragMoveEvent( QDragMoveEvent *e );
    void dragEnterEvent( QDragEnterEvent *e );
    void focusInEvent( QFocusEvent * ) { emit gotFocus( ); }

    // we dont actually accept drops, see comment in constr code.
    void contentsDropEvent ( QDropEvent * e );

    //! Overide so we can change some paint stuff.
    void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );
    
    //for relative pixmap location
    QString JahBasePath;

};

//! A simple widget to display asset information
class AssetDisplay : public QWidget
{
public:
  AssetDisplay( QWidget* parent, const char* name = 0 );
  ~AssetDisplay();

    void setAssetData( const assetData& );
    virtual void setFont( const QFont& );

private:
    QLabel* m_info;
};

// A media browser, with list of open assets, Dnd features, and a preview with play controls. 
class JahDesktopSideTable : public QWidget, public AssetTable
{
    Q_OBJECT
    
public:
    JahDesktopSideTable(int rows=0, QWidget* parent=0, const char* name=0);
    virtual ~JahDesktopSideTable( );
    
    void addAsset(int index, int globaltablenumber, assetData thedata, QImage image);
    
    int jahnumRows;
    
    bool removeItem(int theitem);
    bool clearAll(void);
    
    // returns the value from the table.
    QString text( int row, int col ) { return thetable->text( row, col ); };
    
    // switches on/off the 'empty' backdrop
    void showBackdrop( bool on = true ); 
    
    MediaTable *getTable( ) { return thetable; }
    int find( assetData );
    
	void setAssetTracking( bool );
	void selectAsset( assetData & );

signals:
    void currentChanged( int, int );
    void assetSelected( assetData );

public slots:
    // Shows the item at the specified row (in table) in the figure canvas view.
    /*! If row is out of range then a missing media pixmap is shown. 
        Col is ignored at present (its there for poss future use, and so that the arguments match
        most signals from QTable).
    */
    void showItemInFigure( int row, int col );
    
    void stopAnimation();
    
    void playControlClicked();
    void stopControlClicked();    

    // Shows/hides the player controls. Usually called automatically by showItemInFigure.
    void showPlayerControls( bool show );
    
    void renameAsset( int id, QString newname );
    
    void receivedFocus( );
    
private slots:
    void onUpdateFrameOffset( assetData );
 
protected:

    MediaTable *thetable;
    QCanvas thecanvas;
    FigureEditor *figure;
    
    QPushButton *pbplay;
    QPushButton *pbstop;
    QPushButton *pbffwd;
    QPushButton *pbfrev;
    QLabel* m_timecode;

    QLabel *lspacer1;
    QLabel *lspacer2;
    QLabel *topspacer;
    
    AssetDisplay* m_assetDisplay;
    
    bool clipplayable;
    
private:
    
    bool mousepressed;
	bool m_asset_tracking;

    //for relative pixmap location
    QString JahBasePath;
};


#endif




