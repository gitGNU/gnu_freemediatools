/****************************************************************************
** Form interface generated from reading ui file 'advancedsearchform.ui'
**
** Created: Fri Oct 21 16:49:00 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef ADVANCEDSEARCHFORM_H
#define ADVANCEDSEARCHFORM_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>
#include "DataSource.h"
#include "Asset.h"

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QTable;
class QPushButton;
class QToolButton;
class QSpinBox;
class QListViewItem;
using namespace olib::openassetlib;

class advancedSearchForm : public QWidget
{
    Q_OBJECT

public:
    advancedSearchForm( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~advancedSearchForm();

    QLabel* textLabel2;
    QTable* table;
    QPushButton* doAdvancedSearchButton;
    QPushButton* cancelAdvancedSearchButton;
    QToolButton* addConditionButton;
    QToolButton* deleteConditionButton;
    QLabel* dummyLabel;
    QSpinBox* hitLimitSpinBox;

public slots:
    virtual void addCondition();
    virtual void deleteSelectedCondition();
    virtual void listViewSelectionChangedSlot( QListViewItem * i );
    virtual void executeSearch();

signals:
    void hitsToShow( std::list<olib::openassetlib::Asset*>);

protected:
    std::list<olib::openassetlib::Asset*> hits;
    olib::openassetlib::DataSource *datasource;

    QGridLayout* advancedSearchFormLayout;
    QSpacerItem* spacer3;
    QHBoxLayout* layout8;
    QSpacerItem* spacer4;
    QHBoxLayout* layout12;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

    void init();

};

#endif // ADVANCEDSEARCHFORM_H
