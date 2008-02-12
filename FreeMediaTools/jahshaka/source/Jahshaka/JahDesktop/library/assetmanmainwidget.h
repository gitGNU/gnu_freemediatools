/****************************************************************************
** Form interface generated from reading ui file 'assetmanmainwidget.ui'
**
** Created: Fri Oct 21 16:48:58 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ASSETMANMAINWIDGET_H
#define ASSETMANMAINWIDGET_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include "dirview.h"
#include "AssetManager.h"
#include "Asset.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class DirectoryView;
class QtFileIconView;
class JahProgressBar;
class advancedSearchForm;
class QLabel;
class QPushButton;
class QLineEdit;
class QToolButton;
class QComboBox;
class QSplitter;
class QWidgetStack;
class QTable;

class AssetManMainWidget : public QWidget
{
    Q_OBJECT

public:
    AssetManMainWidget( QWidget* parent = 0,QWidget* userspace = 0, 
		                QWidget* leftmenuspace = 0, QWidget* rightmenuspace = 0,
						const char* name = 0, WFlags fl = 0 );
    ~AssetManMainWidget();

    QLabel* pixmapLabel1_2;
    QPushButton* advancedSearchButton;
    QLabel* pixmapLabel1;
    QLabel* pixmapLabel2;
    QLineEdit* searchLineEdit;
    QLabel* textLabel1;
    QLabel* pixmapLabel2_3;
    QLabel* pixmapLabel2_2;
    QToolButton* connectServerButton;
    QToolButton* backButton;
    QToolButton* upButton;
    QPushButton* viewsButton;
    QPushButton* mediaViewsButton;
    QComboBox* driveCombo;
    QComboBox* pathCombo;
    QLabel* dummyLabel;
    //QSplitter* splitter3;
    //QSplitter* splitter2;
    QWidgetStack* widgetStack1;
    QWidget* dirlistStackPage;
    DirectoryView* dirlist;  //should be changed to assetlist
    DirectoryView* filelist; //added for filesystem view only
    QWidget* advancedSearchStackPage;
    advancedSearchForm* advancedSearchForm1;
    QtFileIconView* fileview;
    QTable* assetTable;
    QLabel* progressLabel;
    JahProgressBar* progress;
    QPushButton* closeButton;

	//list for multiselect
	QValueList < QString > m_selected_items;
    QtFileIconView *fileView() { return fileview; }

public slots:
    virtual void populateDrivesCombo();
    virtual void changeWinDir( int drive );
    virtual void changePath( const QString & path );
    virtual void comboTextWasChanged( const QString & thedir );
    virtual void setPathCombo( QString path );
    virtual void directoryChanged( const QString & dir );
    virtual void slotStartReadDir( int dirs );
    virtual void slotReadNextDir();
    virtual void slotReadDirDone();
    virtual void loadThisFile( QString filename );
    virtual void setTheFileName( QString filename );
    virtual void advandedSearchClicked();
    virtual void advancedSearchCancelPressed();
    virtual void executeSimpleSearch();
    virtual void cdUp();
    virtual void enableUp();
    virtual void disableUp();
    virtual void connectToServerDialog();
    virtual void enableBack();
    virtual void disableBack();
    virtual void cdBack();

	virtual std::string locateAssetDatabase();

	//slot to load a file...
	void returnFile( void );

signals:
    void loadButton();

protected:
    Directory *root, *mediaroot;
    std::list<olib::openassetlib::Asset*> hits;

    QVBoxLayout* AssetManMainWidgetLayout;
    QSpacerItem* spacer9;
    QHBoxLayout* layout11;
    QGridLayout* layout16;
    QVBoxLayout* dirlistStackPageLayout;
    QGridLayout* advancedSearchStackPageLayout;
    QGridLayout* layout19;
    QHBoxLayout* layout7;
    QSpacerItem* spacer2;

protected slots:
    virtual void languageChange();

    virtual void populateDirView();


private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;
    QPixmap image5;
    QPixmap image6;
    QPixmap image7;
    QPixmap image8;
    QPixmap image9;

    void init();

};

#endif // ASSETMANMAINWIDGET_H
