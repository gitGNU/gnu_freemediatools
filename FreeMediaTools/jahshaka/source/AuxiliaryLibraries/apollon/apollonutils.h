/***************************************************************************
                          apollonutils.h  -  description
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

#ifndef APOLLONUTILS_H
#define APOLLONUTILS_H

#include "qpixmap.h"
#include "qpainter.h"

class QString;

class ApollonUtils
{
public:
	static QString convertSize(unsigned long size);
	static QString calcBandwidth(int transmitted, int time);
	static QString bitrateHuman(unsigned long rate);
	static QString numberHuman(unsigned long size);
	static QString timeHuman(unsigned long secs);
	static bool readFile(QString &filename, QStringList &list);
	static bool containsMultiple(const QString &haystack, const QString &needle, bool all = true);
    static QPixmap& gradient(QPixmap &pixmap, const QColor &ca, const QColor &cb, bool eff, int ncols);

};

#endif
