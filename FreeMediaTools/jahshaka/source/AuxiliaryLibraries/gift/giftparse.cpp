/***************************************************************************
                          giftparse.cc  -  Functions for parsing giFT commands
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

#include <qvaluestack.h>

#include "giftparse.h"

namespace Apollon
{

CommandElement::CommandElement() :
  type(typePlain)
{
}

CommandElement::~CommandElement()
{
}

bool CommandElement::parseElements(QString command)
{
	QString statement;
	CommandElementList::Iterator it;

	while(command = command.stripWhiteSpace(), !command.isEmpty())
	{
		statement = Command::stripNextStatement(command);

		if(statement == "")
		{
			return false;
		}
		else if(statement == "}")
		{
			return true;
		}
		else if(statement[0] == '(')
		{
			if(!(type & typeHasElements))
				break;

			(*it).value = Command::unescapeStr(statement.mid(1, statement.length() - 2));
			(*it).type |= typeHasValue;
		}
		else if(statement[0] == '{')
		{
			if(!(type & typeHasElements))
				break;

			(*it).type |= typeHasElements;
			if((*it).parseElements(statement.mid(1)) == false)
				return false;
		}
		else
		{
			it = elements.append(CommandElement());
			(*it).name = statement;
		}
	}

	return false;
}

Command::Command(QString command) :
  type(typePlain)
{
	bool ok = false;
	QString statement;
	QString *currentValue = &value;
	uint *currentType = &type;
	CommandElementList::Iterator it;

	command = command.stripWhiteSpace();
	if((name = Command::stripNextStatement(command)) == "")
		return;

	while(command = command.stripWhiteSpace(), !command.isEmpty())
	{
		statement = Command::stripNextStatement(command);

		if(statement == "")
		{
			break;
		}
		else if(statement == ";")
		{
			ok = true;
			break;
		}
		else if(statement[0] == '(')
		{
			*currentValue = unescapeStr(statement.mid(1, statement.length() - 2));
			*currentType |= typeHasValue;
		}
		else if(statement[0] == '{')
		{
			if(!(type & typeHasElements))
				break;

			(*it).type |= typeHasElements;
			if((*it).parseElements(statement.mid(1)) == false)
				break;
		}
		else
		{
			type |= typeHasElements;
			it = elements.append(CommandElement());
			(*it).name = statement;
			currentValue = &(*it).value;
			currentType = &(*it).type;
		}
	}

	if(ok == false)
	{
		type = typePlain;
		name = "";
		value = "";
		elements.clear();
	}
}

Command::~Command()
{
	elements.clear();
}

QString Command::stripNextStatement(QString &command)
{
	char closeTag = ' ';
	uint i = 0;
	QString result;

	if(command[(int)i] == '(')
		closeTag = ')';
	else if(command[(int)i] == '{')
		closeTag = '}';

	for(; i < command.length(); i++)
	{
		if(closeTag == ' ')
		{
			if(command[(int)i].isSpace() || command[(int)i] == '(' || command[(int)i] == '{' ||
			   command[(int)i] == '}' || command[(int)i] == ';')
				break;
		}
		else
		{
			if(command[(int)i] == closeTag && command[(int)i - 1] != '\\')
			{
				i++;
				break;
			}
		}
	}

	if(closeTag != ' ' && command[(int)i] != closeTag)
		return QString("");

	result = command.left(i);
	command = command.mid(i);

	return result;
}

QString Command::unescapeStr(QString string)
{
	for(uint i = 0; i < string.length(); i++)
	{
		if(string[(int)i] == '\\')
		{
			string = string.remove(i, 1);
		}
	}

	return string;
}

}
