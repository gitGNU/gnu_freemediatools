
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef MESHES_INC_
#define MESHES_INC_

#include <openobjectlib/sg/core.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC mesh : public node
{
public:
	static const int TRIANGLES		= 0x00;
	static const int TRIANGLESTRIP	= 0x01;
	static const int POLYGONS		= 0x02;
	static const int UNKNOWN		= 0xff;
	
public:
	explicit mesh( );
	virtual ~mesh( );
	
	IMPLEMENT_INDEXABLE_CLASS

	virtual bool single_index( ) const { return true; }

public:
	int  prim_type( ) const { return prim_type_; }
	int& prim_type( )		{ return prim_type_; }

private:
	int prim_type_;
};

struct SG_DECLSPEC multi_index_mesh : public mesh
{
	explicit multi_index_mesh( );
	virtual ~multi_index_mesh( );
	
	IMPLEMENT_INDEXABLE_CLASS
	
	virtual bool single_index( ) const { return false; }
};

struct SG_DECLSPEC uv_set : public node
{
	explicit uv_set( );
	virtual ~uv_set( );
};

struct SG_DECLSPEC attribute_array : public node
{
	explicit attribute_array( );
	virtual ~attribute_array( );
};

typedef boost::shared_ptr<mesh>					mesh_ptr;
typedef boost::shared_ptr<multi_index_mesh>		multi_index_mesh_ptr;
typedef boost::shared_ptr<uv_set>				uv_set_ptr;
typedef boost::shared_ptr<attribute_array>		attribute_array_ptr;

typedef openpluginlib::value_property<mesh_ptr>				v_mesh_ptr;
typedef openpluginlib::value_property<multi_index_mesh_ptr>	v_multi_index_mesh_ptr;
typedef openpluginlib::value_property<uv_set_ptr>			v_uv_set_ptr;
typedef openpluginlib::value_property<attribute_array_ptr>	v_attribute_array_ptr;

typedef openpluginlib::multi_value_property<uv_set_ptr>				m_uv_set_ptr;
typedef openpluginlib::multi_value_property<attribute_array_ptr>	m_attribute_array_ptr;
typedef openpluginlib::multi_value_property<v_uv_set_ptr>			mv_uv_set_ptr;
typedef openpluginlib::multi_value_property<v_attribute_array_ptr>	mv_attribute_array_ptr;

} } }

#endif
