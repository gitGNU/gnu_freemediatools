/***************************************************************************
                          giftparse.h  -  Functions for parsing giFT commands
                             -------------------
    begin                : Wed Oct 29 2003
    copyright            : (C) 2003 by Arend van Beelen jr.
    email                : arend@auton.nl
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef __GIFTPARSE_H
#define __GIFTPARSE_H

#include <qstring.h>

namespace Apollon
{

class CommandElement;

typedef QValueList<CommandElement> CommandElementList;

class CommandElement
{
	public:
		enum Types { typePlain = 0x00, typeHasValue = 0x01, typeHasElements = 0x02 };

		CommandElement();
		~CommandElement();

		bool parseElements(QString command);

		uint               type;
		QString            name;
		QString            value;
		CommandElementList elements;
};

class Command
{
	friend class CommandElement;

	public:
		enum Types { typePlain = 0x00, typeHasValue = 0x01, typeHasElements = 0x02 };

		Command(QString command);
		~Command();

		uint               type;
		QString            name;
		QString            value;
		CommandElementList elements;

	private:
		static QString stripNextStatement(QString &command);
		static QString unescapeStr(QString string);
};

}

#endif
