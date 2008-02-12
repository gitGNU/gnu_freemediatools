
// sg - A scene graph representation.

// Copyright (C) 2005-2006 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef SPATIAL_INC_
#define SPATIAL_INC_

#include <openobjectlib/sg/core.hpp>
#include <openobjectlib/sg/bbox_volume.hpp>

namespace olib { namespace openobjectlib { namespace sg {

class SG_DECLSPEC spatial : public node
{
public:
	explicit spatial( );
	virtual ~spatial( );
	
	const bbox_volume&	get_world_bounds( ) const { return vol_; }
	void set_world_bounds( const bbox_volume& vol ) { vol_ = vol; }
	
private:
	bbox_volume vol_;
};

struct SG_DECLSPEC group : public spatial
{
	explicit group( );
	virtual ~group( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC transform : public group
{
	explicit transform( );
	virtual ~transform( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC select : public group
{
	explicit select( );
	virtual ~select( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC HDR_group : public group
{
	explicit HDR_group( );
	virtual ~HDR_group( );

	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC shape : public spatial
{
	explicit shape( );
	virtual ~shape( );
	
	IMPLEMENT_INDEXABLE_CLASS
};

struct SG_DECLSPEC camera : public spatial
{
	explicit camera( );
	virtual ~camera( );
	
	IMPLEMENT_INDEXABLE_CLASS
	
	void look_at( const openpluginlib::vec3f& eye, const openpluginlib::vec3f& at, const openpluginlib::vec3f& up );
	void generalised_perspective( float xfov, float yfov, float nearF, float farF );
	void perspective( float yfov, float aspect, float nearF, float farF );
};

SG_DECLSPEC openpluginlib::matrixf view_from_camera( const camera& cam );

typedef boost::shared_ptr<spatial>	 spatial_ptr;
typedef boost::shared_ptr<group>	 group_ptr;
typedef boost::shared_ptr<transform> transform_ptr;
typedef boost::shared_ptr<select>	 select_ptr;
typedef boost::shared_ptr<shape>	 shape_ptr;
typedef boost::shared_ptr<camera>	 camera_ptr;

typedef openpluginlib::value_property<spatial_ptr>	v_spatial_ptr;
typedef openpluginlib::value_property<group_ptr>	v_group_ptr;
typedef openpluginlib::value_property<camera_ptr>	v_camera_ptr;

typedef openpluginlib::multi_value_property<spatial_ptr> m_spatial_ptr;

} } }

#endif
