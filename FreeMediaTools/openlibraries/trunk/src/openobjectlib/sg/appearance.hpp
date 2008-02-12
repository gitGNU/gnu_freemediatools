
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef APPEARANCE_INC_
#define APPEARANCE_INC_

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef HAVE_OPENIMAGELIB
#include <openimagelib/il/il.hpp>
#endif

#include <openobjectlib/sg/core.hpp>

namespace olib { namespace openobjectlib { namespace sg {

// forward declaration
class renderer;

struct SG_DECLSPEC appearance : public node
{
	explicit appearance( );
	virtual ~appearance( );
};

struct SG_DECLSPEC material : public node
{
	explicit material( );
	virtual ~material( );
};

class SG_DECLSPEC texture : public node
{
public:
	explicit texture( );
	virtual ~texture( );
	
public:
	virtual bool update( );

#ifdef HAVE_OPENIMAGELIB
	openimagelib::il::image_type_ptr image( bool release = true );
#endif

private:
#ifdef HAVE_OPENIMAGELIB
	openimagelib::il::image_type_ptr im_;
#endif
	bool ml_;
};

struct SG_DECLSPEC texture_transform : public node
{
	explicit texture_transform( );
	virtual ~texture_transform( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC blend : public node
{
	explicit blend( );
	virtual ~blend( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC cull_mode : public node
{
	explicit cull_mode( );
	virtual ~cull_mode( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC depth_test : public node
{
	explicit depth_test( );
	virtual ~depth_test( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC front_face : public node
{
	explicit front_face( );
	virtual ~front_face( );
	IMPLEMENT_INDEXABLE_CLASS
};

typedef boost::shared_ptr<appearance>			appearance_ptr;
typedef boost::shared_ptr<material>				material_ptr;
typedef boost::shared_ptr<texture>				texture_ptr;
typedef boost::shared_ptr<texture_transform>	texture_transform_ptr;
typedef boost::shared_ptr<blend>				blend_ptr;
typedef boost::shared_ptr<cull_mode>			cull_mode_ptr;
typedef boost::shared_ptr<depth_test>			depth_test_ptr;
typedef boost::shared_ptr<front_face>			front_face_ptr;




class v_appearance_ptr : public openpluginlib::value_property<appearance_ptr>
{
public:
  v_appearance_ptr();
  v_appearance_ptr(appearance_ptr &);
};

class v_material_ptr : public openpluginlib::value_property<material_ptr>
{
public:
  v_material_ptr();
  v_material_ptr(material_ptr &);

};

class m_texture_transform_ptr : public  openpluginlib::multi_value_property<texture_transform_ptr>  
{
public:
  m_texture_transform_ptr();
  m_texture_transform_ptr(texture_transform_ptr&);
};


class v_texture_ptr : public openpluginlib::value_property<texture_ptr>				
{
public:
  v_texture_ptr();
  v_texture_ptr(olib::openobjectlib::sg::texture_ptr& p);
    //    openpluginlib::value_property<texture_ptr>(p)
};

class v_texture_transform_ptr : public openpluginlib::value_property<texture_transform_ptr> {};
class v_blend_ptr: public openpluginlib::value_property<blend_ptr>				{};
class v_cull_mode_ptr: public openpluginlib::value_property<cull_mode_ptr>			{};
class v_depth_test_ptr: public openpluginlib::value_property<depth_test_ptr>			{};
class v_front_face_ptr: public openpluginlib::value_property<front_face_ptr>			{};
//class m_texture_transform_ptr: public openpluginlib::multi_value_property<texture_transform_ptr>  {};
class mv_texture_ptr: public openpluginlib::multi_value_property<v_texture_ptr>			{};


class m_texture_ptr : public openpluginlib::multi_value_property<texture_ptr>
{

};

//class v_texture_ptr :  public openpluginlib::value_property<texture_ptr>{};


//namespace olib { namespace openobjectlib { namespace sg {
//class mv_texture_ptr : public openpluginlib::multi_value_property<v_texture_ptr>{};


} } }


namespace olib {
  namespace openobjectlib {
    namespace sg {
      namespace SGANON
      {
	struct query_traits
	{
	  query_traits( const olib::openpluginlib::wstring& filename );
	  olib::openpluginlib::wstring libname( ) const;
	  olib::openpluginlib::wstring to_match( ) const;
	  olib::openpluginlib::wstring type( ) const;
	  int merit( ) const;
	  olib::openpluginlib::wstring filename_;
	};


      };
    }
  }
}


#endif
