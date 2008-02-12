/***************************************************************************
                          Apollonlistview.cpp  -  description
                             -------------------
    begin                : Thr 14 2003
    copyright            : (C) 2003 by David Sansome
    email                : me@davidsansome.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

 #include "apollonlistview.h"
 
ApollonListView::ApollonListView(QWidget* parent)
 : QListView(parent)
{
}


QSize ApollonListView::sizeHint() const
{
	return QSize(1, 1);
}

QSize ApollonListView::minimumSizeHint() const
{
	return QSize(1, 1);
}

QSize ApollonListView::minimumSize() const
{
	return QSize(1, 1);
}
