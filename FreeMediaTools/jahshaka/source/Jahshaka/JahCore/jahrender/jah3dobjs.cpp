/*******************************************************************************
**
** The source file for the Jahshaka core 3dobject file
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <qstring.h>
#include <openobjectlib.h>
#include <jahrender.h>
#include <glworld.h>
#include <glcore.h>

////////////////////////////////////////////////
// 3d object loaders

namespace opl = olib::openpluginlib;
namespace sg  = olib::openobjectlib::sg;

bool JahRender::loadObj( void )
{
	std::string filename = thelayer->objfilename.data();
	std::string extension = thelayer->objextension.data();
	filename += extension;

	thelayer->addObjectObj();

	return thelayer->jahobject->openObject(filename);
}

void JahRender::makeObj( void )
{
	sg::camera_ptr  cam;
	sg::scene_ptr   scene;
	sg::bbox_volume bv;

	glPushAttrib( GL_ALL_ATTRIB_BITS );

	if( glUseProgramObjectARB )	
		glUseProgramObjectARB( 0 );

	glDisable( GL_BLEND );
	glEnable( GL_LIGHTING );

    objectData* jahobject = thelayer->jahobject;
    if( !jahobject )
        return;

	scene = jahobject->getScene( );
	if( scene )
	{
	  scene->update( ); // this was a linker problem :
		bv = scene->get_world_bounds( );
	}

	glMatrixMode( GL_MODELVIEW );
	glPushMatrix( );
	
	float scale = Globals::getCameraDistance( ) / ( 2.0f * ( bv.get_radius( ) + 0.01f ) );
	glScalef( scale, scale, scale );

	thelayer->jahobject->drawObject();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix( );

	if( glUseProgramObjectARB )	
		glUseProgramObjectARB( 0 );
	
	glPopAttrib( );
}
