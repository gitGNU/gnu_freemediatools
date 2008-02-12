/*******************************************************************************
**
** The header file for the Jahshaka widget module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "jahdataio.h"

////////////////////////////////////////////////////////
//from charutilities
	
char * JahDataIO::stripExtension(					// ao_dest_name and a_source_name can be the same
	char * const & ao_dest_name,		// make sure it is allocated, use c_max_path
	char const * const & a_source_name)
{
	ASSERT(ao_dest_name);
	ASSERT(a_source_name);

	strcpy(ao_dest_name, a_source_name);

	char * l_tmp;

	if ( (l_tmp = strrchr(ao_dest_name, (int) * ".") ) != 0 )
	{
        // make sure that this dot is not in a directory name
        if ( ( strchr(l_tmp, (int) * "/" ) == 0 )
          && ( strchr(l_tmp, (int) * "\\") == 0 ) )
        {
            * l_tmp = 0;
        }
	}
	return ao_dest_name; 
}

char * JahDataIO::getExtension(					// ao_dest_extension and a_source_name can be the same
	char * const & ao_dest_extension,	// make sure it is allocated, use c_max_path
	char const * const & a_source_name)
{
	ASSERT(ao_dest_extension);
	ASSERT(a_source_name);

	const char * l_tmp;

	if ( ( l_tmp = strrchr(a_source_name, (int) * ".") ) != 0 )
	{
        // make sure that this dot is not in a directory name
        if ( (strchr(l_tmp, (int) * "/" ) == 0 )
          && (strchr(l_tmp, (int) * "\\" ) == 0 ) )
        {
			l_tmp++; // we don't want the point
            strcpy(ao_dest_extension, l_tmp);
        }
		else
		{
			* ao_dest_extension = 0;
		}
	}
	return ao_dest_extension; 
}

bool JahDataIO::checkExtension ( 
    char const * const & a_filename,
    char const * const & a_extension)
{
	ASSERT(a_filename);
	ASSERT(a_extension);

	char l_extension[c_max_path];

	strcpy(l_extension,".");
	strcat(l_extension,a_extension);

	int l_length_ext = ( int ) strlen(l_extension);

	char * l_p_ext;

	for ( l_p_ext = l_extension + 1 ; l_p_ext < l_extension + l_length_ext; l_p_ext++ ) 
	{
		if ( islower(*l_p_ext) ) 
		{
			*l_p_ext = (char) toupper(*l_p_ext);
		}
	}

	char l_end_of_filename[c_max_path];

	strcpy(l_end_of_filename, a_filename+strlen(a_filename) - strlen(l_extension) );

	char * l_p_end_of_filename;

	for ( l_p_end_of_filename = l_end_of_filename + 1;
		  l_p_end_of_filename < l_end_of_filename + l_length_ext;
		  l_p_end_of_filename++)
	{
		//if ( islower(*l_p_end_of_filename) )
		{
			*l_p_end_of_filename = (char) toupper(*l_p_end_of_filename);
		}
	}

	if ( strcmp(l_end_of_filename, l_extension) == 0 ) 
	{
		return true;
	}
	else
	{
		return false;
	}
}

char * JahDataIO::insureExtension(							// ao_dest_name_with_extension and a_source_name can be the same
	char * const & ao_dest_name_with_extension, // make sure it is allocated, use c_max_path
	char const * const & a_source_name,			
	char const * const & a_extension)
{  
	ASSERT(ao_dest_name_with_extension);
	ASSERT(a_source_name);
	ASSERT(a_extension);

	if (checkExtension(a_source_name,a_extension) == false)
	{
		stripExtension(ao_dest_name_with_extension,a_source_name);
		strcat(ao_dest_name_with_extension,".");
		strcat(ao_dest_name_with_extension,a_extension);
	}
	else 
	{
		strcpy(ao_dest_name_with_extension,a_source_name);
	}

	return ao_dest_name_with_extension;
}

char * JahDataIO::getPathWithoutFinalSlash(		// ao_dest_path and a_source_name can be the same
    char * const & ao_dest_path,		// make sure it is allocated, use c_max_path
    char const * const & a_source_name)
{
	ASSERT(ao_dest_path);
	ASSERT(a_source_name);

    const char * l_tmp = strrchr(a_source_name, (int) * "/"); 

    if  ( ! l_tmp )  
    {
      l_tmp = strrchr(a_source_name, (int) * "\\"); 
    }

    if  ( l_tmp )
    {
        strncpy(ao_dest_path, a_source_name, l_tmp - a_source_name); 
        ao_dest_path[l_tmp - a_source_name] = 0; 
    }
    else
    {
        ao_dest_path[0] = 0; 
    }

	return ao_dest_path; 
}

char * JahDataIO::toLowercaseInPlace(char * const & ai_string)
{
	if ( ai_string )
	{
		char * pname = ai_string ;

		for ( pname = ai_string ; pname < ai_string + strlen ( ai_string ) ; pname ++ ) 

		if ( isupper ( * pname ) ) 
		{
			* pname = ( char ) tolower ( * pname ) ;
		}
	}

	return ai_string;
}
