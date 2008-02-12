/***************************************************************************
                          apollonutils.cpp  -  description
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

#include <qfile.h>
#include <qstring.h>
#include <qstringlist.h>

//#include <klocale.h>

#include "apollonutils.h"

QString ApollonUtils::convertSize( unsigned long size)
{
	float fsize;
	QString s;
	// Giga-byte
	if ( size >= 1073741824 )
	{
		fsize = (float) size / (float) 1073741824;
		if ( fsize > 1024 ) // Tera-byte
		s = QString( "%1 TB" ).arg((fsize / (float)1024), 0, 'f', 1);
		else
		s = QString( "%1 GB" ).arg(fsize, 0,'f',1);
	}
	// Mega-byte
	else if ( size >= 1048576 )
	{
		fsize = (float) size / (float) 1048576;
		s = QString( "%1 MB" ).arg(fsize,0,'f',1);
	}
	// Kilo-byte
	else if ( size > 1024 )
	{
		fsize = (float) size / (float) 1024;
		s = QString( "%1 KB" ).arg(fsize,0,'f',1);
	}
	// Just byte
	else
	{
		fsize = (float) size;
		s = QString( "%1 B" ).arg(fsize,0,'f',0);
	}
	return s;
}

QString ApollonUtils::calcBandwidth(int transmitted, int time)
{
	if(time == 0)
		return "--";

	float bandwidth = 0.97656 * (float) transmitted / (float) time;

	if(bandwidth < 0)
		bandwidth = 0;

	QString band = QString("%1").arg(bandwidth, 0, 'f', 2);

	if(band == "nan" || band == "0.00")
		band = "--";
	else
		band += " KB/s";

	return band;
}

QString ApollonUtils::bitrateHuman(unsigned long rate)
{
	if(rate / 1000000)
	{
		return QString("%1 Mb/s").arg(rate / 1000000);
	}
	else if(rate / 1000)
	{
		return QString("%1 Kb/s").arg(rate / 1000);
	}

	return QString("%1 b/s").arg(rate);
}

//
// This function is ported from giFToxic, (C) 2003 giFToxic team.
//
QString ApollonUtils::numberHuman(unsigned long size)
{
	QString format;
	QString result;
	QString qtys[3] = { "", " thousand", " million" };

	unsigned long divisor = 1;
	int type = 0;

	if(size >= 1000000)
	{
		divisor = 1000000;
		type = 2;
	}
	else if(size >= 1000)
	{
		divisor = 1000;
		type = 1;
	}

	unsigned long mod = size % divisor;

	format = (mod == 0 ? "%.0f" : "%.1f");
	format += qtys[type];
	result.sprintf(format, (float) size / divisor);
	return result;
}

//
// This function is ported from giFToxic, (C) 2003 giFToxic team.
//
QString ApollonUtils::timeHuman(unsigned long secs)
{
	int minutes;
	int hours;
	int days;
	QString result;

	minutes = secs / 60;
	hours = minutes / 60;
	days = hours / 24;

	if(!secs)
	{
		result = "0s";
	}
	else if(secs < 60)
	{
		result.sprintf("%lis", secs);
	}
	else if(minutes < 60)
	{
		result.sprintf("%im %lis", minutes, secs % 60);
	}
	else if(hours < 24)
	{
		result.sprintf("%ih %im", hours, minutes % 60);
	}
	else
	{
		result.sprintf("%id %ih", days, hours % 24);
	}

	return result;
}

bool ApollonUtils::readFile(QString &filename, QStringList &list)
{
	QFile file(filename);
	QString s;

	if(file.open(IO_ReadOnly) == false)
		return false;

	list.clear();
	while(!file.atEnd())
	{
		file.readLine(s, 1024);
		list.append(s);
	}
	file.close();

	return true;
}

bool ApollonUtils::containsMultiple(const QString &haystack, const QString &needle, bool all)
{
	QString needle_ = needle.simplifyWhiteSpace();

	if(needle_.contains(' '))
	{
		if(all)
		{
			return containsMultiple(haystack, needle_.section(' ', 0, 0), all) &&
			       containsMultiple(haystack, needle_.section(' ', 1), all);
		}
		else
		{
			return containsMultiple(haystack, needle_.section(' ', 0, 0), all) ||
			       containsMultiple(haystack, needle_.section(' ', 1), all);
		}
	}

	return haystack.contains(needle_, false);
}


QPixmap& ApollonUtils::gradient(QPixmap &pixmap, const QColor &ca,
     const QColor &cb, bool eff, int )
 {
     if(pixmap.depth() > 8 )

        //&& (eff == VerticalGradient || eff == HorizontalGradient))

        {


         int rDiff, gDiff, bDiff;
         int rca, gca, bca /*, rcb, gcb, bcb*/;

         register int x, y;

         rDiff = (/*rcb = */ cb.red())   - (rca = ca.red());
         gDiff = (/*gcb = */ cb.green()) - (gca = ca.green());
         bDiff = (/*bcb = */ cb.blue())  - (bca = ca.blue());

         register int rl = rca << 16;
         register int gl = gca << 16;
         register int bl = bca << 16;

         int rcdelta = ((1<<16) / (eff == true ? pixmap.height() : pixmap.width())) * rDiff;
         int gcdelta = ((1<<16) / (eff == true ? pixmap.height() : pixmap.width())) * gDiff;
         int bcdelta = ((1<<16) / (eff == true ? pixmap.height() : pixmap.width())) * bDiff;

         //int rcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * rDiff;
         //int gcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * gDiff;
         //int bcdelta = ((1<<16) / (eff == VerticalGradient ? pixmap.height() : pixmap.width())) * bDiff;

         QPainter p(&pixmap);
         //QPainter p;
         //p.begin(&pixmap);


         // these for-loops could be merged, but the if's in the inner loop
         // would make it slow
         switch(eff) {
         case true:
             for ( y = 0; y < pixmap.height(); y++ ) {
                 rl += rcdelta;
                 gl += gcdelta;
                 bl += bcdelta;

                 p.setPen(QColor(rl>>16, gl>>16, bl>>16));
                 p.drawLine(0, y, pixmap.width()-1, y);
             }
             break;
         case false:
             for( x = 0; x < pixmap.width(); x++) {
                 rl += rcdelta;
                 gl += gcdelta;
                 bl += bcdelta;

                 p.setPen(QColor(rl>>16, gl>>16, bl>>16));
                 p.drawLine(x, 0, x, pixmap.height()-1);
             }
             break;
         }
     }
     /*
     else {
         QImage image = KImageEffect::gradient(pixmap.size(), ca, cb,
                                               (KImageEffect::GradientType) eff, ncols);
         pixmap.convertFromImage(image);
     }
*/
     return pixmap;
 }
