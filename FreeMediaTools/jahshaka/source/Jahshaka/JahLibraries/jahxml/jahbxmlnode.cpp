/*******************************************************************************
** Source file for
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahbxmlnode.h"
#include "bxmlnode.h"

namespace bxmlnode
{

bool getAttributeFromThisTag ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	std::string const & a_attribute_name ,
	std::string & ao_attribute_value )
{
	unsigned int i ;

	if ( !a_node ) return false;
	std::string l_tag = a_node -> tag ;
	if ( l_tag != a_tag_name ) return false;

	bool l_found_attribute = false ;

	for ( i = 0 ; a_node -> attrs [ i ] ; ++ i )
	{ 
		std::string l_Attribute = a_node -> attrs [ i ] ;
		if ( l_Attribute == a_attribute_name ) 
		{
			ao_attribute_value = a_node -> values [ i ] ;
			l_found_attribute = true;
			break ;
		}
	}

	return l_found_attribute ;
}


unsigned int getNumOfSimilarFirstLevelSubTags ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name )
{
	unsigned int i ;

	if ( !a_node ) return 0;

	unsigned int l_occurence = 0 ;
	for ( i = 0 ; a_node -> children [ i ] ; ++ i ) 
	{ 
		std::string l_tag = a_node -> children [ i ] -> tag ;
		if ( l_tag == a_tag_name )
		{
			l_occurence ++ ;
		}
	}
	
	return l_occurence ;
}

bool getFirstLevelSubTagOccurence ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	unsigned int const & a_requested_occurence , // 0 = first , 1 = second , etc ...
	basicxmlnode * & ao_node )
{
	unsigned int i ;

	if ( !a_node ) return false;
	
	ao_node = NULL ;

	bool l_found_tag = false ;

	unsigned int l_occurence = 0 ;
	for ( i = 0 ; a_node -> children [ i ] ; ++ i ) 
	{ 
		std::string l_tag = a_node -> children [ i ] -> tag ;
		if ( l_tag == a_tag_name )
		{
			if ( l_occurence == a_requested_occurence )
			{
				l_found_tag = true ;
				ao_node = a_node -> children [ i ] ;
				break ;
			}
			l_occurence ++ ;
		}
	}
	
	return l_found_tag ;
}

bool getAttributeFromFirstLevelSubTagOccurence ( 
	basicxmlnode const * const & a_node ,
	std::string const & a_tag_name ,
	unsigned int const & a_requested_occurence , // 0 = first , 1 = second , etc ...
	std::string const & a_attribute_name ,
	std::string & ao_attribute_value )
{
	bool l_found_tag ;
	basicxmlnode * l_sub_node ;
	l_found_tag = getFirstLevelSubTagOccurence ( 
		a_node ,
		a_tag_name ,
		a_requested_occurence , 
		l_sub_node ) ;

	bool & l_found_attribute = l_found_tag ;

	if ( l_found_tag == true )
	{
		l_found_attribute = getAttributeFromThisTag ( 
			l_sub_node ,
			a_tag_name ,
			a_attribute_name ,
			ao_attribute_value ) ;
	}

	return l_found_attribute ;
}


} //  namespace bxmlnode

