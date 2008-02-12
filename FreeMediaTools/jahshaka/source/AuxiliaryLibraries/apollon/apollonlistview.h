/***************************************************************************
                          Apollonlistview.h  -  description
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

#ifndef APOLLONLISTVIEW_H
#define APOLLONLISTVIEW_H

#include <qlistview.h>
#include <qsize.h>
#include <qevent.h>


class ApollonListView : public QListView
{
	public:
		ApollonListView(QWidget* parent);

		QSize sizeHint() const;
		QSize minimumSizeHint() const;
		QSize minimumSize() const;
		
};

#endif
