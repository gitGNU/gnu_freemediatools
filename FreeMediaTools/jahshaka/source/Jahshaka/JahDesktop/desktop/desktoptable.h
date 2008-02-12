/*******************************************************************************
**
** The header file for the Jahshaka player module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef DESKTOPTABLE_H
#define DESKTOPTABLE_H

#include <qmessagebox.h>
#include <qwidgetstack.h>
#include <qtable.h>
#include <qdragobject.h>

class JahTranslate;
class assetData;

class JahDesktopTable : public QTable {
  Q_OBJECT

public:
  JahDesktopTable(int rows=0, int cols=0, QWidget* parent=0, const char* name=0);

  void addAsset(int index, int globaltablenumber, assetData thedata, QImage);

  int jahnumRows;

  bool removeItem(int theitem);
  bool clearAll(void);

public slots:
      void renameAsset( int id, QString newname );

protected:

  //! Overide so we can change some paint stuff.
  void paintCell ( QPainter * p, int row, int col, const QRect & cr, bool selected, const QColorGroup & cg );

 QDragObject * dragObject();

};






#endif




