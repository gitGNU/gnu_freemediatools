/*******************************************************************************
** Source file for
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifndef JAHBXMLNODE_H
#define JAHBXMLNODE_H

#include <string>

#include "bxmlnode.h"

namespace bxmlnode
{

bool getAttributeFromThisTag ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	std::string const & a_attribute_name ,
	std::string & ao_attribute_value ) ;

unsigned int getNumOfSimilarFirstLevelSubTags ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ) ;

bool getFirstLevelSubTagOccurence ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	unsigned int const & a_requested_occurence , // 0 = first , 1 = second , etc ...
	basicxmlnode * & ao_node ) ;

bool getAttributeFromFirstLevelSubTagOccurence ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	unsigned int const & a_requested_occurence , // 0 = first , 1 = second , etc ...
	std::string const & a_attribute_name ,
	std::string & ao_attribute_value ) ;

} //  namespace bxmlnode


#endif // JAHBXMLNODE_H

