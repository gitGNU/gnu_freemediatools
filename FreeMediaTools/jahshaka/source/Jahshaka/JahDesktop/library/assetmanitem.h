/****************************************************************************
** Form interface generated from reading ui file 'assetmanitem.ui'
**
** Created: Fri Oct 21 16:48:59 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ASSETMANITEM_H
#define ASSETMANITEM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include <qiconview.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QFrame;
class QLineEdit;
class QPushButton;
class QTextEdit;

class AssetManItem : public QWidget
{
    Q_OBJECT

public:
    AssetManItem( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~AssetManItem();

    QLabel* dropShadowPixmapLabel;
    QFrame* mainFrame0;
    QLineEdit* nameLineEdit;
    QLabel* dateCreatedLabel;
    QLabel* dateCreatedValueLabel;
    QLabel* sizeLabel;
    QLabel* sizeValueLabel;
    QPushButton* expandButton;
    QPushButton* collapseButton;
    QPushButton* fullExpandButton;
    QPushButton* firstExpandButton;
    QFrame* mainFrame1;
    QLabel* previewLabel;
    QLabel* textLabel3;
    QTextEdit* descriptionTextEdit;
    QLabel* compressionValueLabel;
    QLabel* framesizeValueLabel;
    QLabel* fileTypeLabel;
    QLabel* compressionLabel;
    QLabel* framesizeLabel;
    QLabel* trtValueLabel;
    QLabel* trtLabel;
    QLabel* fileTypeValueLabel;
    QLabel* tagsLabel;
    QLabel* textLabel3_4_3_2_2;
    QLabel* formatLabel;
    QLabel* formatValueLabel;
    QLabel* licenceValueLabel;
    QLabel* tagValueLabel;
    QLabel* audioValueLabel;
    QLabel* audioLabel;
    QFrame* mainFrame2;
    QLabel* tempLabel;
    QLabel* textLabel3_3_2_6;
    QLabel* dummyLabel1;

    int assetNumber;
    int expandLevel;

    virtual int groupHeight( int groupnum );
    virtual void setThumbImage( QImage theimage );

public slots:
    virtual void expand();
    virtual void setGroupBackgroundColor( int groupnum, QColor col );
    virtual void setInfoFromFile( const QString & filename );
    virtual void resetAllFields();
    virtual void setInfoFromIconViewItem( QIconViewItem * item );
    virtual void newSlot();

signals:
    void assetExpanded(int);

protected:
    QVBoxLayout* AssetManItemLayout;
    QHBoxLayout* mainFrame0Layout;
    QSpacerItem* spacer2_3;
    QSpacerItem* spacer2_2;
    QSpacerItem* spacer2;
    QHBoxLayout* mainFrame1Layout;
    QSpacerItem* spacer9;
    QVBoxLayout* layout13;
    QHBoxLayout* layout12;
    QGridLayout* layout9;
    QGridLayout* layout12_2;
    QHBoxLayout* mainFrame2Layout;
    QSpacerItem* spacer28;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;
    QPixmap image3;
    QPixmap image4;

    void init();

};

#endif // ASSETMANITEM_H
