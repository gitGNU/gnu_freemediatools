

//#define HAVE_GL_GLEW_H


#include <openpluginlib/pl/openpluginlib.hpp>
#include <openobjectlib/sg/sg.hpp>
#include <openobjectlib.h>
#include <openmedialib/openmedialib.h>
#include <openmedialib/mediaobject/mediaobject.h>
#include <openimagelib/openimagelib.h>

#include <openpluginlib/pl/discovery_traits.hpp>
#include <openpluginlib/pl/openpluginlib.hpp>

// Own Include
#include "openobjectlib.h"


#include <openpluginlib/pl/bind_info.hpp>
// Other Includes
#include <openpluginlib/pl/property.hpp>

//olib::openpluginlib::property_typename test;
//#pragma warning "mike after the property"
#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/sg/shaders.hpp>
#include "openobjectplugins.h"

#include <openobjectlib/sg/hw_GL_renderer.hpp>

#include <qvaluelist.h>
#include <qfile.h>
#include <cmath>

#include <boost/thread/thread.hpp>
#include <boost/thread/condition.hpp>

#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/GL_utility.hpp>

#include <openmedialib/ml/ml.hpp>

#include <qgl.h>


#include <qtimer.h>
#include <qdragobject.h>
#include <qmime.h>
#include <qstrlist.h>
#include <qevent.h>
#include <qcursor.h>
#include <qmessagebox.h>
#include <cmath>
#include <iostream>
#include <deque>



//for the mediatable


#include <iostream>

#include <limits>
#include <cmath>
#include <openmedialib/ml/ml.hpp>
#include <openmedialib/ml/openmedialib_plugin.hpp>
#include <openpluginlib/pl/openpluginlib.hpp>
#include <openpluginlib/pl/utf8_utils.hpp>
#include <deque>

#include <iostream>

void todo ()
{
  std::cerr << "TODO " << std::endl;
}


#ifdef NEED_SCENE
//#include <openobjectlib/
namespace olib
{

  namespace openobjectlib
  {
    
    namespace sg
    {
      
      ///home/mdupont/experiments/video/js/jahshaka/source/Jahshaka/JahCore/jahrender/jah3dobjs.cpp:54: undefined reference to `olib::openobjectlib::sg::scene::update()'
      bool scene::update()      {  
	todo ();
      };
      
      
      //olib::openobjectlib::sg::
      
      ///home/mdupont/experiments/video/js/jahshaka/source/Jahshaka/JahCore/jahrender/jah3dobjs.cpp:37: undefined reference to `olib::openobjectlib::sg::bbox_volume::bbox_volume()'

      bbox_volume::bbox_volume()      {             

	todo ();
      }
      
      



      //SG_DECLSPEC hw_GL_renderer_ptr create_hw_GL_renderer( );
      //../../../../openlibraries/trunk/src/openobjectlib/sg/hw_GL_renderer.hpp:94: error: ambiguates old declaration 'olib::openobjectlib::sg::hw_GL_renderer_ptr olib::openobjectlib::sg::create_hw_GL_renderer()'
      hw_GL_renderer_ptr create_hw_GL_renderer()
      {  

	hw_GL_renderer_ptr mynull;
	todo ();
	return mynull;
	
      };

      //../../../../openlibraries/trunk/src/openobjectlib/sg/hw_GL_renderer.hpp:36: error: candidate is: bool olib::openobjectlib::sg::hw_GL_renderer::render(const olib::openobjectlib::sg::node_ptr&)
      bool hw_GL_renderer::render(boost::shared_ptr<olib::openobjectlib::sg::node> const&)
      {  
	todo ();
	return false;
      };
      

      ///home/mdupont/experiments/video/js/jahshaka/source/Jahshaka/JahCore/jahrender/jah3dobjs.cpp:55: undefined reference to `olib::openobjectlib::sg::scene::get_world_bounds() const'

      bbox_volume  scene::get_world_bounds() const
      {
	bbox_volume ret;
	todo ();

	return ret;
      };

      ///home/mdupont/experiments/video/js/jahshaka/source/Jahshaka/JahCore/jahrender/jah3dobjs.cpp:61: undefined reference to `olib::openobjectlib::sg::bbox_volume::get_radius() const
      //	float get_radius( ) const;
      float bbox_volume::get_radius() const
      {
	todo ();
	return 0;
      };

      // declared openlibraries/trunk/src/openobjectlib/sg/sg.hpp
      bool init()
      {
	todo ();
      }
    }
  }
}
bool olib::openobjectlib::init()
{
  todo ();
}

#endif


// new constructors
olib::openobjectlib::sg::v_appearance_ptr::v_appearance_ptr()
{

}


olib::openpluginlib::property_typename::property_typename(wchar_t const* pName)
  : 
  openpluginlib::wstring(pName) // just pass the constructor along
{
  
  //this has been called 
  /*

typedef openpluginlib::wstring                                                 property_typename;

    stack :
  #0  todo () at linker.cpp:78
#1  0x080ea2cf in property_typename (this=0x84c2b48) at linker.cpp:164
#2  0x08332c07 in __static_initialization_and_destruction_0 (__initialize_p=1, __priority=65535) at property.hpp:271
#3  0x0833329b in global constructors keyed to _ZN9JahRender7loadObjEv () at jah3dobjs.cpp:74
#4  0x08351102 in __do_global_ctors_aux ()
#5  0x0807b344 in _init ()
#6  0x08351059 in __libc_csu_init ()
#7  0xb70103f1 in __libc_start_main () from /lib/i686/cmov/libc.so.6
#8  0x08084461 in _start ()
 p *((this->_M_dataplus)._M_p) =0

  */
  std::wcerr << "create a wstring/property_typename" 
	    <<"test2:" 
	    << c_str()  
	    << std::endl;

  //  todo ();
}

//../../../../openlibraries/trunk/src/openobjectlib/sg/scene.hpp:21: error: candidate is: olib::openobjectlib::sg::bbox_volume olib::openobjectlib::sg::scene::get_world_bounds() const



#ifdef DONT_LINK

namespace pl = olib::openpluginlib;
namespace ml = olib::openmedialib::ml;
namespace il = olib::openimagelib::il;


namespace olib
{
  namespace openmedialib
  {
    

      ///mnt/sda10/mdupont/experiments/video/js/jahshaka/source/Jahshaka/JahDesktop/encoder/../../JahDesktop/player/qtplayer.h:115: undefined reference to `olib::openmedialib::ml::create_input(std::basic_string<char, std::char_traits<char>, std::allocator<char> > const&)'


    //../../../../openlibraries/trunk/src/openmedialib/ml/utilities.cpp : defined here... why does the linker not work?
    //../../../../openlibraries/trunk/src/openmedialib/ml/utilities.hpp:19: error: ambiguates old declaration 'olib::openmedialib::ml::input_type_ptr olib::openmedialib::ml::create_input(const olib::openpluginlib::string&)'
    //ML_DECLSPEC input_type_ptr create_input( const openpluginlib::string & );
    // frML_DECLSPEC input_type_ptr create_input( const pl::wstring &resource )

namespace ml {
  //  olib::openmedialib::ml::create_input ()
  // see /mnt/sda10/mdupont/experiments/video/js/openlibraries/trunk/
  //src/openmedialib/ml/utilities.cpp

#ifdef DUPLICATE_DEF
    input_type_ptr 
    create_input(
		 std::basic_string<
		 char,
		 std::char_traits<char>, 
		 std::allocator<char> 
		 >
		 const& resource)
      { 
	//		todo ();
	olib::openpluginlib::wstring temp(resource.begin(),resource.end());

	/*

linker.cpp:229: error: 
invalid initialization of reference of type 
'const olib::openpluginlib::wstring&' 
from expression of type 
'const std::basic_string<char, std::char_traits<char>, std::allocator<char> >'
*/
	input_type_ptr result = create_delayed_input( temp);
	if ( result )
		result->init( );
	return result;

      };

#endif
};


  } // openmedialib
}// olib


#ifdef DUPLICATE_DEF
olib::openimagelib::il::image_type_ptr
olib::openimagelib::il::allocate(
std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > const&, int, int)
      { 

		todo ();
      };

#endif
#ifdef DUPLICATE_DEF
olib::openimagelib::il::image_type_ptr
olib::openimagelib::il::convert(
		     boost::shared_ptr<
		     olib::openimagelib::il::image<
		     unsigned char,
		     olib::openimagelib::il::surface_format,
		     olib::openimagelib::il::default_storage>
		     > const&,
		     std::basic_string<wchar_t,
		     std::char_traits<wchar_t>,
		     std::allocator<wchar_t>
		     > const&,
		     int)
    { 
      	todo ();
    };
#endif

//IL_DECLSPEC image_type_ptr field( const image_type_ptr &im, int field );
//../../../../openlibraries/trunk/src/openimagelib/il/utility.hpp:50: error: ambiguates old declaration 'olib::openimagelib::il::image_type_ptr olib::openimagelib::il::field(const olib::openimagelib::il::image_type_ptr&, int)'
#ifdef DUPLICATE_DEF
olib::openimagelib::il::image_type_ptr
olib::openimagelib::il::field
(boost::shared_ptr<
	    olib::openimagelib::il::image<unsigned char,
	    olib::openimagelib::il::surface_format,
	    olib::openimagelib::il::default_storage>

	    > const&, int)
    { 
      	todo ();
 };
#endif
#ifdef DUPLICATE_DEF

olib::openimagelib::il::image_type_ptr
olib::openimagelib::il::deinterlace
(
			 boost::shared_ptr<
			 olib::openimagelib::il::image<unsigned char,
			 olib::openimagelib::il::surface_format,
			 olib::openimagelib::il::default_storage> > const&)
      
{

  todo ();
}
#endif
#endif


/*
#include "DataSource.h"

namespace olib
{
  namespace openassetlib
  {
    DataSource::Query::Condition::Condition(std::basic_string<char, std::char_traits<char>, std::allocator<char> >, std::basic_string<char, std::char_traits<char>, std::allocator<char> >, olib::openassetlib::DataSource::Query::Condition::Comparitor) throw();

{

}
  }
}
*/

#include <openassetlib/al/al.hpp>
#include <openassetlib/al/set.hpp>

/*
namespace olib
{
  namespace openpluginlib
  {
    discovery<olib::openassetlib::al::oml_query_traits>::discovery(olib::openassetlib::al::oml_query_traits const&)
    {
      
    }
  }
}
*/
/*

namespace olib
{
  namespace openassetlib
  {
    AssetManager::instance()
    {
      
    }
  }
}


*/
