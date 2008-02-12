/***************************************************************************
                          apollontab.h  -  description
                             -------------------
begin		: Dec 05 2003
copyright	: (C) 2002 by Bastiaan Naber, (C) 2002 by Robert Wittams
email		: bastiaan@ricardis.tudelft.nl
description	: 

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef APOLLONTAB_H
#define APOLLONTAB_H

#include <qwidget.h>

class giFTConnection;

class ApollonTab : public QWidget
{
	Q_OBJECT

	public:
		ApollonTab(QWidget *parent = 0, const char *name = 0, giFTConnection *gcn = 0);
		~ApollonTab();

		QString &status() { return m_status; }

	protected:
		QString         m_status;
		giFTConnection *m_gcn;

		void setStatus(QString status );

	signals:
		void statusMessage( QString string );
		void browseUser( const QString& user );
};

#endif
