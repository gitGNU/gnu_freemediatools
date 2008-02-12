/***************************************************************************
                          apollontab.cpp  -  description
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

//#include <klocale.h>

#include <qlayout.h>

#include "apollontab.h"

ApollonTab::ApollonTab(QWidget *parent, const char *name, giFTConnection *gcn) :
  QWidget(parent, name),
  m_gcn(gcn)
{
	setStatus("Ready");

	QBoxLayout *layout = new QVBoxLayout(this);
	layout->setAutoAdd(true);
}

ApollonTab::~ApollonTab()
{
}

void ApollonTab::setStatus(QString status)
{
   m_status = status;
   
   emit statusMessage(m_status);
}
