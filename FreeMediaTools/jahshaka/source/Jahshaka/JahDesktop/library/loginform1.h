/****************************************************************************
** Form interface generated from reading ui file 'loginform1.ui'
**
** Created: Fri Oct 21 16:48:57 2005
**      by: The User Interface Compiler ($Id$)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef CONNECTDIALOG_H
#define CONNECTDIALOG_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QButtonGroup;
class QComboBox;
class QLineEdit;
class QPushButton;

class ConnectDialog : public QDialog
{
    Q_OBJECT

public:
    ConnectDialog( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~ConnectDialog();

    QLabel* pixmapLabel1;
    QButtonGroup* buttonGroup1;
    QComboBox* comboBox2;
    QComboBox* addressComboBox;
    QLabel* textLabel5;
    QLabel* textLabel4;
    QLineEdit* lineEdit2;
    QComboBox* comboBox3;
    QLabel* dummyLabel;
    QPushButton* pushButton1;
    QPushButton* pushButton2;
    QLabel* textLabel6;
    QLabel* pixmapLabel3;

public slots:
    virtual void cancelPressed();
    virtual void connectPressed();

protected:
    QGridLayout* ConnectDialogLayout;
    QSpacerItem* spacer11;
    QSpacerItem* spacer3_2_2;
    QSpacerItem* spacer13_2;
    QSpacerItem* spacer13;
    QSpacerItem* spacer3_3;
    QSpacerItem* spacer3_2;
    QSpacerItem* spacer3;
    QSpacerItem* spacer3_4;
    QSpacerItem* spacer1_3_3;
    QHBoxLayout* buttonGroup1Layout;
    QGridLayout* layout2;
    QSpacerItem* spacer1_3_2;
    QSpacerItem* spacer1_3;

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;

    void init();

};

#endif // CONNECTDIALOG_H
