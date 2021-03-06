
// ofx - A ofx plugin to ml.

// Copyright (C) 2005-2007 Editopia Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.


/*
see :
http://openfx.sourceforge.net

*/
#ifdef HAVE_OPENFX

#if _MSC_VER >= 1400
#include <hash_map>
#else
#include <map>
#endif

#include <cstdarg>
#include <cstring>
#include <new>
#include <vector>
#include <limits>

#include <ofxCore.h>
#include <ofxImageEffect.h>
#include <ofxInteract.h>
#include <ofxParam.h>
#include <ofxProperty.h>
#include <ofxMessage.h>
#include <ofxMultiThread.h>

#include <openmedialib/ml/openmedialib_plugin.hpp>

#include <openpluginlib/pl/utf8_utils.hpp>
#include <openpluginlib/pl/pcos/property.hpp>

namespace pl	= olib::openpluginlib;
namespace pcos	= olib::openpluginlib::pcos;
namespace ml	= olib::openmedialib::ml;
namespace il	= olib::openimagelib::il;

namespace olib { namespace openmedialib { namespace ml {

namespace
{
	enum ofx_context { FILTER, TRANSITION, GENERATOR, GENERAL };
	
	void set_property_suite( OfxPropertySuiteV1& );
	void set_image_effect_suite( OfxImageEffectSuiteV1& );
	void set_parameter_suite( OfxParameterSuiteV1& );
	void set_memory_suite( OfxMemorySuiteV1& );
	void set_multithread_suite( OfxMultiThreadSuiteV1& );
	void set_interact_suite( OfxInteractSuiteV1& );
	void set_message_suite( OfxMessageSuiteV1& );

	// typedefs
	typedef pcos::property_container* property_container_ptr;
	
	// forward declarations
	class param;
	class param_set;
	class clip_instance;
	
	struct Ofx_base
	{
		virtual ~Ofx_base( ) { }
		virtual property_container_ptr get_property_set( ) = 0;
	};

	class param_descriptor : public Ofx_base
	{
	public:
		explicit param_descriptor( )
			: param_descriptor_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, param_descriptor_prop_name_( pcos::key::from_string( kOfxPropName ) )
			, param_descriptor_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
			, param_descriptor_prop_short_label_( pcos::key::from_string( kOfxPropShortLabel ) )
			, param_descriptor_prop_long_label_( pcos::key::from_string( kOfxPropLongLabel ) )
			, param_descriptor_prop_param_prop_type_( pcos::key::from_string( kOfxParamPropType ) )
			, param_descriptor_prop_param_prop_secret_( pcos::key::from_string( kOfxParamPropSecret ) )
			, param_descriptor_prop_param_prop_can_undo_( pcos::key::from_string( kOfxParamPropCanUndo ) )
			, param_descriptor_prop_param_prop_hint_( pcos::key::from_string( kOfxParamPropHint ) )
			, param_descriptor_prop_param_prop_script_name_( pcos::key::from_string( kOfxParamPropScriptName ) )
			, param_descriptor_prop_param_prop_parent_( pcos::key::from_string( kOfxParamPropParent ) )
			, param_descriptor_prop_param_prop_enabled_( pcos::key::from_string( kOfxParamPropEnabled ) )
			, param_descriptor_prop_param_prop_data_ptr_( pcos::key::from_string( kOfxParamPropDataPtr ) )
		{
			set_param_descriptor_properties( );
		}

		virtual property_container_ptr	get_property_set( ) { return &param_descriptor_props_; }
		virtual param*					create_instance( ) = 0;
	
	private:
		void set_param_descriptor_properties( )
		{
			param_descriptor_props_.append( param_descriptor_prop_type_ = pl::string( kOfxTypeParameter ) );
			param_descriptor_props_.append( param_descriptor_prop_name_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_label_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_short_label_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_long_label_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_secret_ = 0 );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_can_undo_ = 1 );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_hint_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_script_name_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_parent_ = pl::string( ) );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_enabled_ = 1 );
			param_descriptor_props_.append( param_descriptor_prop_param_prop_data_ptr_ = ( void* ) 0 );
		}
	
	private:
		pcos::property_container param_descriptor_props_;
		pcos::property param_descriptor_prop_type_;
		pcos::property param_descriptor_prop_name_;
		pcos::property param_descriptor_prop_label_;
		pcos::property param_descriptor_prop_short_label_;
		pcos::property param_descriptor_prop_long_label_;
		pcos::property param_descriptor_prop_param_prop_type_;
		pcos::property param_descriptor_prop_param_prop_secret_;
		pcos::property param_descriptor_prop_param_prop_can_undo_;
		pcos::property param_descriptor_prop_param_prop_hint_;
		pcos::property param_descriptor_prop_param_prop_script_name_;
		pcos::property param_descriptor_prop_param_prop_parent_;
		pcos::property param_descriptor_prop_param_prop_enabled_;
		pcos::property param_descriptor_prop_param_prop_data_ptr_;
	};
	
	class value_param_descriptor : public param_descriptor
	{
	public:
		explicit value_param_descriptor( )
			: value_param_descriptor_param_prop_animates_( pcos::key::from_string( kOfxParamPropAnimates ) )
			, value_param_descriptor_param_prop_is_animating_( pcos::key::from_string( kOfxParamPropIsAnimating ) )
			, value_param_descriptor_param_prop_is_auto_keying_( pcos::key::from_string( kOfxParamPropIsAutoKeying ) )
			, value_param_descriptor_param_prop_persistant_( pcos::key::from_string( kOfxParamPropPersistant ) )
			, value_param_descriptor_param_prop_evaluate_on_change_( pcos::key::from_string( kOfxParamPropEvaluateOnChange ) )
			, value_param_descriptor_param_prop_plugin_may_write_( pcos::key::from_string( kOfxParamPropPluginMayWrite ) )
			, value_param_descriptor_param_prop_cache_invalidation_( pcos::key::from_string( kOfxParamPropCacheInvalidation ) )
		{
			set_value_param_descriptor_properties( );
		}

	private:
		void set_value_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( value_param_descriptor_param_prop_animates_ = 1 );
			param_props_->append( value_param_descriptor_param_prop_is_animating_ = 0 );
			param_props_->append( value_param_descriptor_param_prop_is_auto_keying_ = 0 );
			param_props_->append( value_param_descriptor_param_prop_persistant_ = 1 );
			param_props_->append( value_param_descriptor_param_prop_evaluate_on_change_ = 1 );
			param_props_->append( value_param_descriptor_param_prop_plugin_may_write_ = 0 );
			param_props_->append( value_param_descriptor_param_prop_cache_invalidation_ = pl::string( kOfxParamInvalidateValueChange ) );
		}

	private:
		
		pcos::property value_param_descriptor_param_prop_animates_;
		pcos::property value_param_descriptor_param_prop_is_animating_;
		pcos::property value_param_descriptor_param_prop_is_auto_keying_;
		pcos::property value_param_descriptor_param_prop_persistant_;
		pcos::property value_param_descriptor_param_prop_evaluate_on_change_;
		pcos::property value_param_descriptor_param_prop_plugin_may_write_;
		pcos::property value_param_descriptor_param_prop_cache_invalidation_;
	};

	class string_param_descriptor : public value_param_descriptor
	{
	public:
		explicit string_param_descriptor( )
			: string_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, string_param_descriptor_param_prop_string_mode_( pcos::key::from_string( kOfxParamPropStringMode ) )
			, string_param_descriptor_param_prop_string_file_path_exists_( pcos::key::from_string( kOfxParamPropStringFilePathExists ) )
		{
			set_string_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_string_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( string_param_descriptor_param_prop_default_ = pl::string( ) );
			param_props_->append( string_param_descriptor_param_prop_string_mode_ = pl::string( kOfxParamStringIsSingleLine ) );
			param_props_->append( string_param_descriptor_param_prop_string_file_path_exists_ = 1 );
		}
		
	private:
		pcos::property string_param_descriptor_param_prop_default_;
		pcos::property string_param_descriptor_param_prop_string_mode_;
		pcos::property string_param_descriptor_param_prop_string_file_path_exists_;
	};
	
	class int_param_descriptor : public value_param_descriptor
	{
	public:
		explicit int_param_descriptor( )
			: int_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
		{
			set_int_param_descriptor_properties( );
		}

		virtual param* create_instance( );
		
	private:
		void set_int_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( int_param_descriptor_param_prop_default_ = 0 );
			param_props_->append( int_param_descriptor_param_prop_min_ = ( std::numeric_limits<int>::min )( ) );
			param_props_->append( int_param_descriptor_param_prop_max_ = ( std::numeric_limits<int>::max )( ) );
			param_props_->append( int_param_descriptor_param_prop_display_min_ = ( std::numeric_limits<int>::min )( ) );
			param_props_->append( int_param_descriptor_param_prop_display_max_ = ( std::numeric_limits<int>::max )( ) );
		}

	private:
		pcos::property int_param_descriptor_param_prop_default_;
		pcos::property int_param_descriptor_param_prop_min_;
		pcos::property int_param_descriptor_param_prop_max_;
		pcos::property int_param_descriptor_param_prop_display_min_;
		pcos::property int_param_descriptor_param_prop_display_max_;
	};

	class int2D_param_descriptor : public value_param_descriptor
	{
	public:
		explicit int2D_param_descriptor( )
			: int2D_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int2D_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int2D_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int2D_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int2D_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, int2D_param_descriptor_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_int2D_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_int2D_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( int2D_param_descriptor_param_prop_default_ = pcos::int_vec( 2, 0 ) );
			param_props_->append( int2D_param_descriptor_param_prop_min_ = pcos::int_vec( 2, ( std::numeric_limits<int>::min )( ) ) );
			param_props_->append( int2D_param_descriptor_param_prop_max_ = pcos::int_vec( 2, ( std::numeric_limits<int>::max )( ) ) );
			param_props_->append( int2D_param_descriptor_param_prop_display_min_ = pcos::int_vec( 2, ( std::numeric_limits<int>::min )( ) ) );
			param_props_->append( int2D_param_descriptor_param_prop_display_max_ = pcos::int_vec( 2, ( std::numeric_limits<int>::max )( ) ) );
			param_props_->append( int2D_param_descriptor_param_prop_dimension_label_ = pl::string_vec( 2 ) );
		}

	private:
		pcos::property int2D_param_descriptor_param_prop_default_;
		pcos::property int2D_param_descriptor_param_prop_min_;
		pcos::property int2D_param_descriptor_param_prop_max_;
		pcos::property int2D_param_descriptor_param_prop_display_min_;
		pcos::property int2D_param_descriptor_param_prop_display_max_;
		pcos::property int2D_param_descriptor_param_prop_dimension_label_;
	};

	class int3D_param_descriptor : public value_param_descriptor
	{
	public:
		explicit int3D_param_descriptor( )
			: int3D_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int3D_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int3D_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int3D_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int3D_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, int3D_param_descriptor_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_int3D_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_int3D_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( int3D_param_descriptor_param_prop_default_ = pcos::int_vec( 3, 0 ) );
			param_props_->append( int3D_param_descriptor_param_prop_min_ = pcos::int_vec( 3, ( std::numeric_limits<int>::min )( ) ) );
			param_props_->append( int3D_param_descriptor_param_prop_max_ = pcos::int_vec( 3, ( std::numeric_limits<int>::max )( ) ) );
			param_props_->append( int3D_param_descriptor_param_prop_display_min_ = pcos::int_vec( 3, ( std::numeric_limits<int>::min )( ) ) );
			param_props_->append( int3D_param_descriptor_param_prop_display_max_ = pcos::int_vec( 3, ( std::numeric_limits<int>::max )( ) ) );
			param_props_->append( int3D_param_descriptor_param_prop_dimension_label_ = pl::string_vec( 3 ) );
		}

	private:
		pcos::property int3D_param_descriptor_param_prop_default_;
		pcos::property int3D_param_descriptor_param_prop_min_;
		pcos::property int3D_param_descriptor_param_prop_max_;
		pcos::property int3D_param_descriptor_param_prop_display_min_;
		pcos::property int3D_param_descriptor_param_prop_display_max_;
		pcos::property int3D_param_descriptor_param_prop_dimension_label_;
	};

	class base_double_param_descriptor : public value_param_descriptor
	{
	public:
		explicit base_double_param_descriptor( )
			: base_double_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, base_double_param_descriptor_param_prop_increment_( pcos::key::from_string( kOfxParamPropIncrement ) )
			, base_double_param_descriptor_param_prop_digits_( pcos::key::from_string( kOfxParamPropDigits ) )
			, base_double_param_descriptor_param_prop_double_type_( pcos::key::from_string( kOfxParamPropDoubleType ) )
		{
			set_base_double_param_descriptor_properties( );
		}

	private:
		void set_base_double_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( base_double_param_descriptor_param_prop_default_ = 0.0 );
			param_props_->append( base_double_param_descriptor_param_prop_increment_ = 1.0 );
			param_props_->append( base_double_param_descriptor_param_prop_digits_ = 2 );
			param_props_->append( base_double_param_descriptor_param_prop_double_type_ = pl::string( kOfxParamDoubleTypePlain ) );
		}
	
	private:
		pcos::property base_double_param_descriptor_param_prop_default_;
		pcos::property base_double_param_descriptor_param_prop_increment_;
		pcos::property base_double_param_descriptor_param_prop_digits_;
		pcos::property base_double_param_descriptor_param_prop_double_type_;
	};
	
	class double_param_descriptor : public base_double_param_descriptor
	{
	public:
		explicit double_param_descriptor( )
			: double_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double_param_descriptor_param_prop_show_time_marker_( pcos::key::from_string( kOfxParamPropShowTimeMarker ) )
		{
			set_double_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_double_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( double_param_descriptor_param_prop_min_ = ( std::numeric_limits<double>::min )( ) );
			param_props_->append( double_param_descriptor_param_prop_max_ = ( std::numeric_limits<double>::max )( ) );
			param_props_->append( double_param_descriptor_param_prop_display_min_ = ( std::numeric_limits<double>::min )( ) );
			param_props_->append( double_param_descriptor_param_prop_display_max_ = ( std::numeric_limits<double>::max )( ) );
			param_props_->append( double_param_descriptor_param_prop_show_time_marker_ = 0 );
		}
	
	private:
		pcos::property double_param_descriptor_param_prop_min_;
		pcos::property double_param_descriptor_param_prop_max_;
		pcos::property double_param_descriptor_param_prop_display_min_;
		pcos::property double_param_descriptor_param_prop_display_max_;
		pcos::property double_param_descriptor_param_prop_show_time_marker_;
	};

	class double2D_param_descriptor : public base_double_param_descriptor
	{
	public:
		explicit double2D_param_descriptor( )
			: double2D_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double2D_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double2D_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double2D_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double2D_param_descriptor_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_double2D_param_descriptor_properties( );
		}

	private:
		void set_double2D_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( double2D_param_descriptor_param_prop_min_ = pcos::double_vec( 2, ( std::numeric_limits<double>::min )( ) ) );
			param_props_->append( double2D_param_descriptor_param_prop_max_ = pcos::double_vec( 2, ( std::numeric_limits<double>::max )( ) ) );
			param_props_->append( double2D_param_descriptor_param_prop_display_min_ = pcos::double_vec( 2, ( std::numeric_limits<double>::min )( ) ) );
			param_props_->append( double2D_param_descriptor_param_prop_display_max_ = pcos::double_vec( 2, ( std::numeric_limits<double>::max )( ) ) );
			param_props_->append( double2D_param_descriptor_param_prop_dimension_label_ = pl::string_vec( 2 ) );
		}
		
		virtual param* create_instance( );
	
	private:
		pcos::property double2D_param_descriptor_param_prop_min_;
		pcos::property double2D_param_descriptor_param_prop_max_;
		pcos::property double2D_param_descriptor_param_prop_display_min_;
		pcos::property double2D_param_descriptor_param_prop_display_max_;
		pcos::property double2D_param_descriptor_param_prop_dimension_label_;
	};

	class double3D_param_descriptor : public base_double_param_descriptor
	{
	public:
		explicit double3D_param_descriptor( )
			: double3D_param_descriptor_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double3D_param_descriptor_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double3D_param_descriptor_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double3D_param_descriptor_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double3D_param_descriptor_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_double3D_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_double3D_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( double3D_param_descriptor_param_prop_min_ = pcos::double_vec( 3, ( std::numeric_limits<double>::min )( ) ) );
			param_props_->append( double3D_param_descriptor_param_prop_max_ = pcos::double_vec( 3, ( std::numeric_limits<double>::max )( ) ) );
			param_props_->append( double3D_param_descriptor_param_prop_display_min_ = pcos::double_vec( 3, ( std::numeric_limits<double>::min )( ) ) );
			param_props_->append( double3D_param_descriptor_param_prop_display_max_ = pcos::double_vec( 3, ( std::numeric_limits<double>::max )( ) ) );
			param_props_->append( double3D_param_descriptor_param_prop_dimension_label_ = pl::string_vec( 3 ) );
		}
	
	private:
		pcos::property double3D_param_descriptor_param_prop_min_;
		pcos::property double3D_param_descriptor_param_prop_max_;
		pcos::property double3D_param_descriptor_param_prop_display_min_;
		pcos::property double3D_param_descriptor_param_prop_display_max_;
		pcos::property double3D_param_descriptor_param_prop_dimension_label_;
	};

	class rgb_param_descriptor : public value_param_descriptor
	{
	public:
		explicit rgb_param_descriptor( )
			: rgb_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_rgb_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );
		
	private:
		void set_rgb_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( rgb_param_descriptor_param_prop_default_ = pcos::double_vec( 3, 0.0 ) );
		}
		
	private:
		pcos::property rgb_param_descriptor_param_prop_default_;
	};

	class rgba_param_descriptor : public value_param_descriptor
	{
	public:
		explicit rgba_param_descriptor( )
			: rgba_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_rgba_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );

	private:
		void set_rgba_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( rgba_param_descriptor_param_prop_default_ = pcos::double_vec( 4, 0.0 ) );
		}
		
	private:
		pcos::property rgba_param_descriptor_param_prop_default_;
	};
	
	class boolean_param_descriptor : public value_param_descriptor
	{
	public:
		explicit boolean_param_descriptor( )
			: boolean_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_boolean_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );
	
	private:
		void set_boolean_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( boolean_param_descriptor_param_prop_default_ = 0 );
		}
	
	private:
		pcos::property boolean_param_descriptor_param_prop_default_;
	};

	class choice_param_descriptor : public value_param_descriptor
	{
	public:
		explicit choice_param_descriptor( )
			: choice_param_descriptor_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, choice_param_descriptor_param_prop_choice_option_( pcos::key::from_string( kOfxParamPropChoiceOption ) )
		{
			set_choice_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );
		
	private:
		void set_choice_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( choice_param_descriptor_param_prop_default_ = 0 );
			param_props_->append( choice_param_descriptor_param_prop_choice_option_ = pl::string_vec( ) );
		}

	private:
		pcos::property choice_param_descriptor_param_prop_default_;
		pcos::property choice_param_descriptor_param_prop_choice_option_;
	};

	class group_param_descriptor : public value_param_descriptor
	{
	public:
		explicit group_param_descriptor( )
		{ }
		
		virtual param* create_instance( );
	};

	class page_param_descriptor : public value_param_descriptor
	{
	public:
		explicit page_param_descriptor( )
			: page_param_descriptor_param_prop_type_( pcos::key::from_string( kOfxParamPropType ) )
			, page_param_descriptor_param_prop_child_( pcos::key::from_string( kOfxParamPropPageChild ) )
		{
			set_page_param_descriptor_properties( );
		}
		
		virtual param* create_instance( );
		
	private:
		void set_page_param_descriptor_properties( )
		{
			property_container_ptr param_props_ = get_property_set( );
			
			param_props_->append( page_param_descriptor_param_prop_type_ = pl::string( kOfxParamTypePage ) );
			param_props_->append( page_param_descriptor_param_prop_child_ = pl::string_vec( ) );
		}
		
	private:
		pcos::property page_param_descriptor_param_prop_type_;
		pcos::property page_param_descriptor_param_prop_child_;
	};
	
	class push_button_param_descriptor : public value_param_descriptor
	{
	public:
		explicit push_button_param_descriptor( )
		{ }
		
		virtual param* create_instance( );
	};

	class custom_param_descriptor : public value_param_descriptor
	{
	public:
		explicit custom_param_descriptor( )
		{ }
		
		virtual param* create_instance( );
	};

	class param_set_base : public Ofx_base
	{
	public:
		virtual OfxParamSetHandle get_param_set( ) = 0;
	};

	class param_set_descriptor : public param_set_base
	{
	public:
		typedef pl::string key_type;
		typedef boost::shared_ptr<param_descriptor> param_descriptor_ptr;

#if _MSC_VER >= 1400
		typedef stdext::hash_map<key_type, param_descriptor_ptr> container;
#else
		typedef std::map<key_type, param_descriptor_ptr> container;
#endif
		typedef container::const_iterator const_iterator;

	public:
		explicit param_set_descriptor( )
		{ }
		
		OfxStatus define( const pl::string& param_type, const pl::string& name );
		
		property_container_ptr find_property_set( const pl::string& name )
		{
			const_iterator I;
			if( ( I = params_.find( name ) ) != params_.end( ) )
				return I->second->get_property_set( );
			return 0;
		}

		virtual property_container_ptr	get_property_set( ) { return &param_set_descriptor_props_; }
		virtual OfxParamSetHandle		get_param_set( )	{ return ( OfxParamSetHandle ) this; }
		
		const_iterator begin( ) const { return params_.begin( ); }
		const_iterator end( )   const { return params_.end( ); }

		virtual param_set* create_instance( );

	private:
		container params_;
		pcos::property_container param_set_descriptor_props_;
	};

	class param : public Ofx_base
	{
	public:
		explicit param( param_descriptor& vpd )
			: param_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, param_prop_name_( pcos::key::from_string( kOfxPropName ) )
			, param_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
			, param_prop_short_label_( pcos::key::from_string( kOfxPropShortLabel ) )
			, param_prop_long_label_( pcos::key::from_string( kOfxPropLongLabel ) )
			, param_prop_param_prop_secret_( pcos::key::from_string( kOfxParamPropSecret ) )
			, param_prop_param_prop_can_undo_( pcos::key::from_string( kOfxParamPropCanUndo ) )
			, param_prop_param_prop_hint_( pcos::key::from_string( kOfxParamPropHint ) )
			, param_prop_param_prop_script_name_( pcos::key::from_string( kOfxParamPropScriptName ) )
			, param_prop_param_prop_parent_( pcos::key::from_string( kOfxParamPropParent ) )
			, param_prop_param_prop_enabled_( pcos::key::from_string( kOfxParamPropEnabled ) )
			, param_prop_param_prop_data_ptr_( pcos::key::from_string( kOfxParamPropDataPtr ) )
		{
			set_param_properties( vpd );
		}

		virtual property_container_ptr	get_property_set( ) { return &param_props_; }
		virtual pcos::property			get_value( ) = 0;
		virtual pl::string				get_type( )	 = 0;
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_param_properties( param_descriptor& vpd )
		{
			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pl::string type			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxPropType ) ).value<pl::string>( );
			pl::string name			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxPropName ) ).value<pl::string>( );
			pl::string label		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxPropLabel ) ).value<pl::string>( );
			pl::string short_label	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxPropShortLabel ) ).value<pl::string>( );
			pl::string long_label	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxPropLongLabel ) ).value<pl::string>( );
			int		   secret		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropSecret ) ).value<int>( );
			int		   can_undo		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropCanUndo ) ).value<int>( );
			pl::string hint			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropHint ) ).value<pl::string>( );
			pl::string script_name	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropScriptName ) ).value<pl::string>( );
			pl::string parent		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropParent ) ).value<pl::string>( );
			int		   enabled		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropEnabled ) ).value<int>( );
			void*	   data_ptr		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDataPtr ) ).value<void*>( );
			
			param_props_.append( param_prop_type_ = type );
			param_props_.append( param_prop_name_ = name );
			param_props_.append( param_prop_label_ = label );
			param_props_.append( param_prop_short_label_ = short_label );
			param_props_.append( param_prop_long_label_ = long_label );
			param_props_.append( param_prop_param_prop_secret_ = secret );
			param_props_.append( param_prop_param_prop_can_undo_ = can_undo );
			param_props_.append( param_prop_param_prop_hint_ = hint );
			param_props_.append( param_prop_param_prop_script_name_ = script_name );
			param_props_.append( param_prop_param_prop_parent_ = parent );
			param_props_.append( param_prop_param_prop_enabled_ = enabled );
			param_props_.append( param_prop_param_prop_data_ptr_ = data_ptr );
		}
	
	private:
		pcos::property_container param_props_;
		pcos::property param_prop_type_;
		pcos::property param_prop_name_;
		pcos::property param_prop_label_;
		pcos::property param_prop_short_label_;
		pcos::property param_prop_long_label_;
		pcos::property param_prop_param_prop_secret_;
		pcos::property param_prop_param_prop_can_undo_;
		pcos::property param_prop_param_prop_hint_;
		pcos::property param_prop_param_prop_script_name_;
		pcos::property param_prop_param_prop_parent_;
		pcos::property param_prop_param_prop_enabled_;
		pcos::property param_prop_param_prop_data_ptr_;
	};
	
	class value_param : public param
	{
	public:
		explicit value_param( value_param_descriptor& vpd )
			: param( vpd )
			, value_param_prop_animates_( pcos::key::from_string( kOfxParamPropAnimates ) )
			, value_param_prop_is_animating_( pcos::key::from_string( kOfxParamPropIsAnimating ) )
			, value_param_prop_is_auto_keying_( pcos::key::from_string( kOfxParamPropIsAutoKeying ) )
			, value_param_prop_persistant_( pcos::key::from_string( kOfxParamPropPersistant ) )
			, value_param_prop_evaluate_on_change_( pcos::key::from_string( kOfxParamPropEvaluateOnChange ) )
			, value_param_prop_plugin_may_write_( pcos::key::from_string( kOfxParamPropPluginMayWrite ) )
			, value_param_prop_cache_invalidation_( pcos::key::from_string( kOfxParamPropCacheInvalidation ) )
		{
			set_value_param_properties( vpd );
		}

	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_value_param_properties( value_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			int			animates			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropAnimates ) ).value<int>( );
			int			is_animating		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropIsAnimating ) ).value<int>( );
			int			is_auto_keying		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropIsAutoKeying ) ).value<int>( );
			int			persistant			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropPersistant ) ).value<int>( );
			int			evaluate_on_change	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropEvaluateOnChange ) ).value<int>( );
			int			may_write			= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropPluginMayWrite ) ).value<int>( );
			pl::string  cache_invalidation	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropCacheInvalidation ) ).value<pl::string>( );
			
			param_props_->append( value_param_prop_animates_ = animates );
			param_props_->append( value_param_prop_is_animating_ = is_animating );
			param_props_->append( value_param_prop_is_auto_keying_ = is_auto_keying );
			param_props_->append( value_param_prop_persistant_ = persistant );
			param_props_->append( value_param_prop_evaluate_on_change_ = evaluate_on_change );
			param_props_->append( value_param_prop_plugin_may_write_ = may_write );
			param_props_->append( value_param_prop_cache_invalidation_ = cache_invalidation );
		}

	private:
		pcos::property value_param_prop_animates_;
		pcos::property value_param_prop_is_animating_;
		pcos::property value_param_prop_is_auto_keying_;
		pcos::property value_param_prop_persistant_;
		pcos::property value_param_prop_evaluate_on_change_;
		pcos::property value_param_prop_plugin_may_write_;
		pcos::property value_param_prop_cache_invalidation_;
	};

	class string_param : public value_param
	{
	public:
		explicit string_param( string_param_descriptor& vpd )
			: value_param( vpd )
			, string_param_value_( pcos::key::from_string( "value" ) )
			, string_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) ) 
			, string_param_prop_string_mode_( pcos::key::from_string( kOfxParamPropStringMode ) )
			, string_param_prop_string_file_path_exists_( pcos::key::from_string( kOfxParamPropStringFilePathExists ) )
		{
			set_string_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return string_param_value_; }
		virtual pl::string		get_type( )	 { return kOfxParamTypeString; }
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_string_param_properties( string_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pl::string	def				 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pl::string>( );
			pl::string	mode			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropStringMode ) ).value<pl::string>( );
			int			file_path_exists = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropStringFilePathExists ) ).value<int>( );
			
			param_props_->append( string_param_value_ = def );
			param_props_->append( string_param_prop_default_ = def );
			param_props_->append( string_param_prop_string_mode_ = mode );
			param_props_->append( string_param_prop_string_file_path_exists_ = file_path_exists );
		}
	
	private:
		pcos::property string_param_value_;
		pcos::property string_param_prop_default_;
		pcos::property string_param_prop_string_mode_;
		pcos::property string_param_prop_string_file_path_exists_;
	};
	
	class int_param : public value_param
	{
	public:
		explicit int_param( int_param_descriptor& vpd )
			: value_param( vpd )
			, int_param_value_( pcos::key::from_string( "value" ) )
			, int_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
		{
			set_int_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return int_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeInteger; }
		
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_int_param_properties( int_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			int	def			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<int>( );
			int	min			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<int>( );
			int	max			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<int>( );
			int	display_min	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<int>( );
			int	display_max	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<int>( );
			
			param_props_->append( int_param_value_ = def );
			param_props_->append( int_param_prop_default_ = def );
			param_props_->append( int_param_prop_min_ = min );
			param_props_->append( int_param_prop_max_ = max );
			param_props_->append( int_param_prop_display_min_ = display_min );
			param_props_->append( int_param_prop_display_max_ = display_max );
		}

	private:
		pcos::property int_param_value_;
		pcos::property int_param_prop_default_;
		pcos::property int_param_prop_min_;
		pcos::property int_param_prop_max_;
		pcos::property int_param_prop_display_min_;
		pcos::property int_param_prop_display_max_;
	};

	class int2D_param : public value_param
	{
	public:
		explicit int2D_param( int2D_param_descriptor& vpd )
			: value_param( vpd )
			, int2D_param_value_( pcos::key::from_string( "value" ) )
			, int2D_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int2D_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int2D_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int2D_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int2D_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, int2D_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_int2D_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return int2D_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeInteger2D; }
		
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_int2D_param_properties( int2D_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pcos::int_vec  def				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::int_vec>( );
			pcos::int_vec  min				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<pcos::int_vec>( );
			pcos::int_vec  max				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<pcos::int_vec>( );
			pcos::int_vec  display_min		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<pcos::int_vec>( );
			pcos::int_vec  display_max		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<pcos::int_vec>( );
			pl::string_vec dimension_label	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDimensionLabel ) ).value<pl::string_vec>( );
			
			param_props_->append( int2D_param_value_ = def );
			param_props_->append( int2D_param_prop_default_ = def );
			param_props_->append( int2D_param_prop_min_ = min );
			param_props_->append( int2D_param_prop_max_ = max );
			param_props_->append( int2D_param_prop_display_min_ = display_min );
			param_props_->append( int2D_param_prop_display_max_ = display_max );
			param_props_->append( int2D_param_prop_dimension_label_ = dimension_label );
		}

	private:
		pcos::property int2D_param_value_;
		pcos::property int2D_param_prop_default_;
		pcos::property int2D_param_prop_min_;
		pcos::property int2D_param_prop_max_;
		pcos::property int2D_param_prop_display_min_;
		pcos::property int2D_param_prop_display_max_;
		pcos::property int2D_param_prop_dimension_label_;
	};

	class int3D_param : public value_param
	{
	public:
		explicit int3D_param( int3D_param_descriptor& vpd )
			: value_param( vpd )
			, int3D_param_value_( pcos::key::from_string( "value" ) )
			, int3D_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, int3D_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, int3D_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, int3D_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, int3D_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, int3D_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_int3D_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return int3D_param_value_; }
		virtual pl::string		get_type( )	 { return kOfxParamTypeInteger3D; }

	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_int3D_param_properties( int3D_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pcos::int_vec  def				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::int_vec>( );
			pcos::int_vec  min				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<pcos::int_vec>( );
			pcos::int_vec  max				= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<pcos::int_vec>( );
			pcos::int_vec  display_min		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<pcos::int_vec>( );
			pcos::int_vec  display_max		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<pcos::int_vec>( );
			pl::string_vec dimension_label	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDimensionLabel ) ).value<pl::string_vec>( );
			
			param_props_->append( int3D_param_value_ = def );
			param_props_->append( int3D_param_prop_default_ = def );
			param_props_->append( int3D_param_prop_min_ = min );
			param_props_->append( int3D_param_prop_max_ = max );
			param_props_->append( int3D_param_prop_display_min_ = display_min );
			param_props_->append( int3D_param_prop_display_max_ = display_max );
			param_props_->append( int3D_param_prop_dimension_label_ = dimension_label );
		}

	private:
		pcos::property int3D_param_value_;
		pcos::property int3D_param_prop_default_;
		pcos::property int3D_param_prop_min_;
		pcos::property int3D_param_prop_max_;
		pcos::property int3D_param_prop_display_min_;
		pcos::property int3D_param_prop_display_max_;
		pcos::property int3D_param_prop_dimension_label_;
	};

	class base_double_param : public value_param
	{
	public:
		explicit base_double_param( base_double_param_descriptor& vpd )
			: value_param( vpd )
			, base_double_param_prop_increment_( pcos::key::from_string( kOfxParamPropIncrement ) )
			, base_double_param_prop_digits_( pcos::key::from_string( kOfxParamPropDigits ) )
			, base_double_param_prop_double_type_( pcos::key::from_string( kOfxParamPropDoubleType ) )
		{
			set_base_double_param_properties( vpd );
		}
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_base_double_param_properties( base_double_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			double		increment	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropIncrement ) ).value<double>( );
			int			digits		= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDigits ) ).value<int>( );
			pl::string	double_type	= vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDoubleType ) ).value<pl::string>( );
			
			param_props_->append( base_double_param_prop_increment_ = increment );
			param_props_->append( base_double_param_prop_digits_ = digits );
			param_props_->append( base_double_param_prop_double_type_ = double_type );
		}
	
	private:
		pcos::property base_double_param_prop_increment_;
		pcos::property base_double_param_prop_digits_;
		pcos::property base_double_param_prop_double_type_;
	};
	
	class double_param : public base_double_param
	{
	public:
		explicit double_param( double_param_descriptor& vpd )
			: base_double_param( vpd )
			, double_param_value_( pcos::key::from_string( "value" ) )
			, double_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, double_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double_param_prop_show_time_marker_( pcos::key::from_string( kOfxParamPropShowTimeMarker ) )
		{
			set_double_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return double_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeDouble; }
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_double_param_properties( double_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			double	def			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<double>( );
			double	min			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<double>( );
			double	max			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<double>( );
			double	display_min	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<double>( );
			double	display_max	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<double>( );
			int		time_marker  = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropShowTimeMarker ) ).value<int>( );
			
			param_props_->append( double_param_value_ = def );
			param_props_->append( double_param_prop_default_ = def );
			param_props_->append( double_param_prop_min_ = min );
			param_props_->append( double_param_prop_max_ = max );
			param_props_->append( double_param_prop_display_min_ = display_min );
			param_props_->append( double_param_prop_display_max_ = display_max );
			param_props_->append( double_param_prop_show_time_marker_ = time_marker );
		}

	private:
		pcos::property double_param_value_;
		pcos::property double_param_prop_default_;
		pcos::property double_param_prop_min_;
		pcos::property double_param_prop_max_;
		pcos::property double_param_prop_display_min_;
		pcos::property double_param_prop_display_max_;
		pcos::property double_param_prop_show_time_marker_;
	};

	class double2D_param : public base_double_param
	{
	public:
		explicit double2D_param( double2D_param_descriptor& vpd )
			: base_double_param( vpd )
			, double2D_param_value_( pcos::key::from_string( "value" ) )
			, double2D_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, double2D_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double2D_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double2D_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double2D_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double2D_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_double2D_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return double2D_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeDouble2D; }
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_double2D_param_properties( double2D_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pcos::double_vec def			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::double_vec>( );
			pcos::double_vec min			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<pcos::double_vec>( );
			pcos::double_vec max			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<pcos::double_vec>( );
			pcos::double_vec display_min	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<pcos::double_vec>( );
			pcos::double_vec display_max	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<pcos::double_vec>( );
			pl::string_vec	 dimension_label = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDimensionLabel ) ).value<pl::string_vec>( );
			
			param_props_->append( double2D_param_value_ = def );
			param_props_->append( double2D_param_prop_default_ = def );
			param_props_->append( double2D_param_prop_min_ = min );
			param_props_->append( double2D_param_prop_max_ = max );
			param_props_->append( double2D_param_prop_display_min_ = display_min );
			param_props_->append( double2D_param_prop_display_max_ = display_max );
			param_props_->append( double2D_param_prop_dimension_label_ = dimension_label );
		}
	
	private:
		pcos::property double2D_param_value_;
		pcos::property double2D_param_prop_default_;
		pcos::property double2D_param_prop_min_;
		pcos::property double2D_param_prop_max_;
		pcos::property double2D_param_prop_display_min_;
		pcos::property double2D_param_prop_display_max_;
		pcos::property double2D_param_prop_dimension_label_;
	};

	class double3D_param : public base_double_param
	{
	public:
		explicit double3D_param( double3D_param_descriptor& vpd )
			: base_double_param( vpd )
			, double3D_param_value_( pcos::key::from_string( "value" ) )
			, double3D_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, double3D_param_prop_min_( pcos::key::from_string( kOfxParamPropMin ) )
			, double3D_param_prop_max_( pcos::key::from_string( kOfxParamPropMax ) )
			, double3D_param_prop_display_min_( pcos::key::from_string( kOfxParamPropDisplayMin ) )
			, double3D_param_prop_display_max_( pcos::key::from_string( kOfxParamPropDisplayMax ) )
			, double3D_param_prop_dimension_label_( pcos::key::from_string( kOfxParamPropDimensionLabel ) )
		{
			set_double3D_param_properties( vpd );
		}

		virtual pcos::property  get_value( ) { return double3D_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeDouble3D; }

	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_double3D_param_properties( double3D_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			pcos::double_vec def			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::double_vec>( );
			pcos::double_vec min			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMin ) ).value<pcos::double_vec>( );
			pcos::double_vec max			 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropMax ) ).value<pcos::double_vec>( );
			pcos::double_vec display_min	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMin ) ).value<pcos::double_vec>( );
			pcos::double_vec display_max	 = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDisplayMax ) ).value<pcos::double_vec>( );
			pl::string_vec	 dimension_label = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDimensionLabel ) ).value<pl::string_vec>( );
			
			param_props_->append( double3D_param_value_ = def );
			param_props_->append( double3D_param_prop_default_ = def );
			param_props_->append( double3D_param_prop_min_ = min );
			param_props_->append( double3D_param_prop_max_ = max );
			param_props_->append( double3D_param_prop_display_min_ = display_min );
			param_props_->append( double3D_param_prop_display_max_ = display_max );
			param_props_->append( double3D_param_prop_dimension_label_ = dimension_label );
		}
	
	private:
		pcos::property double3D_param_value_;
		pcos::property double3D_param_prop_default_;
		pcos::property double3D_param_prop_min_;
		pcos::property double3D_param_prop_max_;
		pcos::property double3D_param_prop_display_min_;
		pcos::property double3D_param_prop_display_max_;
		pcos::property double3D_param_prop_dimension_label_;
	};

	class rgb_param : public value_param
	{
	public:
		explicit rgb_param( rgb_param_descriptor& vpd )
			: value_param( vpd )
			, rgb_param_prop_value_( pcos::key::from_string( "value" ) )
			, rgb_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_rgb_param_properties( vpd );
		}

		virtual pcos::property  get_value( ) { return rgb_param_prop_value_; }
		virtual pl::string		get_type( )	 { return kOfxParamTypeRGB; }
		
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_rgb_param_properties( rgb_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );
			property_container_ptr vpd_set		= vpd.get_property_set( );
			
			pcos::double_vec def = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::double_vec>( );
			param_props_->append( rgb_param_prop_value_   = def );
			param_props_->append( rgb_param_prop_default_ = def );
		}

	private:
		pcos::property rgb_param_prop_value_;
		pcos::property rgb_param_prop_default_;
	};

	class rgba_param : public value_param
	{
	public:
		explicit rgba_param( rgba_param_descriptor& vpd )
			: value_param( vpd )
			, rgba_param_prop_value_( pcos::key::from_string( "value" ) )
			, rgba_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_rgba_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return rgba_param_prop_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeRGBA; }

	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_rgba_param_properties( rgba_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );
			property_container_ptr vpd_set		= vpd.get_property_set( );
			
			pcos::double_vec def = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<pcos::double_vec>( );
			param_props_->append( rgba_param_prop_value_   = def );
			param_props_->append( rgba_param_prop_default_ = def );
		}
		
	private:
		pcos::property rgba_param_prop_value_;
		pcos::property rgba_param_prop_default_;
	};
	
	class boolean_param : public value_param
	{
	public:
		explicit boolean_param( boolean_param_descriptor& vpd )
			: value_param( vpd )
			, boolean_param_value_( pcos::key::from_string( "value" ) )
			, boolean_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
		{
			set_boolean_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return boolean_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeBoolean; }
		
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_boolean_param_properties( boolean_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			int def = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<int>( );
			
			param_props_->append( boolean_param_value_ = def );
			param_props_->append( boolean_param_prop_default_ = def );
		}
		
	private:
		pcos::property boolean_param_value_;
		pcos::property boolean_param_prop_default_;
	};

	class choice_param : public value_param
	{
	public:
		explicit choice_param( choice_param_descriptor& vpd )
			: value_param( vpd )
			, choice_param_value_( pcos::key::from_string( "value" ) )
			, choice_param_prop_default_( pcos::key::from_string( kOfxParamPropDefault ) )
			, choice_param_prop_choice_option_( pcos::key::from_string( kOfxParamPropChoiceOption ) )
		{
			set_choice_param_properties( vpd );
		}

		virtual pcos::property	get_value( ) { return choice_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeChoice; }
		
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.
		void set_choice_param_properties( choice_param_descriptor& vpd )
		{
			property_container_ptr param_props_ = get_property_set( );

			property_container_ptr vpd_set = vpd.get_property_set( );
			
			int def = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropDefault ) ).value<int>( );
			pl::string_vec choice_vec = vpd_set->get_property_with_key( pcos::key::from_string( kOfxParamPropChoiceOption ) ).value<pl::string_vec>( );
			
			param_props_->append( choice_param_value_ = def );
			param_props_->append( choice_param_prop_default_ = def );
			param_props_->append( choice_param_prop_choice_option_ = choice_vec );
		}

	private:
		pcos::property choice_param_value_;
		pcos::property choice_param_prop_default_;
		pcos::property choice_param_prop_choice_option_;
	};

	class group_param : public value_param
	{
	public:
		explicit group_param( group_param_descriptor& vpd )
			: value_param( vpd )
			, group_param_value_( pcos::key::from_string( "value" ) )
		{ }
		
		virtual pcos::property	get_value( ) { return group_param_value_; }
		virtual pl::string		get_type( )	 { return kOfxParamTypeGroup; }

	private:
		pcos::property group_param_value_;
	};

	class page_param : public value_param
	{
	public:
		explicit page_param( page_param_descriptor& vpd )
			: value_param( vpd )
			, page_param_value_( pcos::key::from_string( "value" ) )
		{ }

		virtual pcos::property	get_value( ) { return page_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypePage; }
	
	private:
		// assign the instance properties from the descriptor. do not assign properties since
		// they have shallow copy semantics and clone will cause a leak.
		// therefore extract all the values and assign those. descriptors and instances are
		// meant to be separate entities and values should not be shared.

	private:
		pcos::property page_param_value_;
	};
	
	class push_button_param : public value_param
	{
	public:
		explicit push_button_param( push_button_param_descriptor& vpd )
			: value_param( vpd )
			, push_button_param_value_( pcos::key::from_string( "value" ) )
		{ }

		virtual pcos::property	get_value( ) { return push_button_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypePushButton; }
		
	private:
		pcos::property push_button_param_value_;
	};

	class custom_param : public value_param
	{
	public:
		explicit custom_param( custom_param_descriptor& vpd )
			: value_param( vpd )
			, custom_param_value_( pcos::key::from_string( "value" ) )
		{ }

		virtual pcos::property	get_value( ) { return custom_param_value_; }
		virtual pl::string		get_type( )  { return kOfxParamTypeCustom; }
		
	private:
		pcos::property custom_param_value_;
	};

	class param_set : public param_set_base
	{
	public:
		typedef pl::string key_type;
		typedef boost::shared_ptr<param> param_ptr;

#if _MSC_VER >= 1400
		typedef stdext::hash_map<key_type, param_ptr> container;
#else
		typedef std::map<key_type, param_ptr> container;
#endif
		typedef container::const_iterator const_iterator;

	public:
		explicit param_set( param_set_descriptor& psd )
		{
			set_parameters( psd );
		}
		
		virtual property_container_ptr	get_property_set( )	{ return &param_set_props_; }
		virtual OfxParamSetHandle		get_param_set( )	{ return ( OfxParamSetHandle ) this; }

		param* find_param( const pl::string& name )
		{
			const_iterator I;
			if( ( I = params_.find( name ) ) != params_.end( ) )
				return I->second.get( );
			return 0;
		}

	private:
		void set_parameters( const param_set_descriptor& psd )
		{
			for( param_set_descriptor::const_iterator I = psd.begin( ); I != psd.end( ); ++I )
				params_.insert( container::value_type( I->first, param_ptr( I->second->create_instance( ) ) ) );
		}

	private:
		container params_;
		pcos::property_container param_set_props_;
	};

	class image : public Ofx_base
	{
	public:
		explicit image( )
			: image_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, image_effect_prop_pixel_depth_( pcos::key::from_string( kOfxImageEffectPropPixelDepth ) )
			, image_effect_prop_components_( pcos::key::from_string( kOfxImageEffectPropComponents ) )
			, image_effect_prop_pre_multiplication_( pcos::key::from_string( kOfxImageEffectPropPreMultiplication ) )
			, image_effect_prop_render_scale_( pcos::key::from_string( kOfxImageEffectPropRenderScale ) )
			, image_prop_pixel_aspect_ratio_( pcos::key::from_string( kOfxImagePropPixelAspectRatio ) )
			, image_prop_data_( pcos::key::from_string( kOfxImagePropData ) )
			, image_prop_bounds_( pcos::key::from_string( kOfxImagePropBounds ) )
			, image_prop_region_of_definition_( pcos::key::from_string( kOfxImagePropRegionOfDefinition ) )
			, image_prop_row_bytes_( pcos::key::from_string( kOfxImagePropRowBytes ) )
			, image_prop_field_( pcos::key::from_string( kOfxImagePropField ) )
			, image_prop_unique_identifier_( pcos::key::from_string( kOfxImagePropUniqueIdentifier ) )
		{
			set_image_properties( );
		}

		virtual property_container_ptr get_property_set( )
		{ return &image_props_; }

		bool set_image( il::image_type_ptr im, const pl::string& depth )
		{
			image_effect_prop_pixel_depth_ = depth;

			pcos::int_vec bounds_and_rod( 4, 0 );
			bounds_and_rod[ 2 ] = im->width( );
			bounds_and_rod[ 3 ] = im->height( );

			image_prop_bounds_ = bounds_and_rod;
			image_prop_region_of_definition_ = bounds_and_rod;
			image_prop_data_ = ( void* ) im->data( );
			image_prop_row_bytes_ = im->pitch( );

			return true;
		}

	private:
		void set_image_properties( )
		{
			pcos::double_vec render_scale( 2 );
			render_scale[ 0 ] = 1.0;
			render_scale[ 1 ] = 1.0;
			
			image_props_.append( image_prop_type_ = pl::string( kOfxTypeImage ) );
			image_props_.append( image_effect_prop_pixel_depth_ = pl::string( kOfxBitDepthNone ) );
			image_props_.append( image_effect_prop_components_ = pl::string( kOfxImageComponentRGBA ) );
			image_props_.append( image_effect_prop_pre_multiplication_ = pl::string( kOfxImageUnPreMultiplied ) );
			image_props_.append( image_effect_prop_render_scale_ = render_scale );
			image_props_.append( image_prop_pixel_aspect_ratio_ = 1.0 );
			image_props_.append( image_prop_data_ = ( void* ) 0 );
			image_props_.append( image_prop_bounds_ = pcos::int_vec( 4 ) );
			image_props_.append( image_prop_region_of_definition_ = pcos::int_vec( 4 ) );
			image_props_.append( image_prop_row_bytes_ = 0 );
			image_props_.append( image_prop_field_ = pl::string( kOfxImageFieldNone ) );
			image_props_.append( image_prop_unique_identifier_ = pl::string( "No Unique Identifier" ) );
		}

	private:
		pcos::property_container image_props_;
		pcos::property image_prop_type_;
		pcos::property image_effect_prop_pixel_depth_;
		pcos::property image_effect_prop_components_;
		pcos::property image_effect_prop_pre_multiplication_;
		pcos::property image_effect_prop_render_scale_;
		pcos::property image_prop_pixel_aspect_ratio_;
		pcos::property image_prop_data_;
		pcos::property image_prop_bounds_;
		pcos::property image_prop_region_of_definition_;
		pcos::property image_prop_row_bytes_;
		pcos::property image_prop_field_;
		pcos::property image_prop_unique_identifier_;
	};

	class clip_descriptor : public Ofx_base
	{
	public:
		explicit clip_descriptor( )
			: clip_descriptor_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, clip_descriptor_prop_name_( pcos::key::from_string( kOfxPropName ) )
			, clip_descriptor_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
			, clip_descriptor_prop_short_label_( pcos::key::from_string( kOfxPropShortLabel ) )
			, clip_descriptor_prop_long_label_( pcos::key::from_string( kOfxPropLongLabel ) )
			, clip_descriptor_prop_supported_components_( pcos::key::from_string( kOfxImageEffectPropSupportedComponents ) )
			, clip_descriptor_prop_temporal_clip_access_( pcos::key::from_string( kOfxImageEffectPropTemporalClipAccess ) )
			, clip_descriptor_prop_optional_( pcos::key::from_string( kOfxImageClipPropOptional ) )
			, clip_descriptor_prop_field_extraction_( pcos::key::from_string( kOfxImageClipPropFieldExtraction ) )
			, clip_descriptor_prop_is_mask_( pcos::key::from_string( kOfxImageClipPropIsMask ) )
			, clip_descriptor_prop_supports_tiles_( pcos::key::from_string( kOfxImageEffectPropSupportsTiles ) )
		{
			set_clip_descriptor_properties( );
		}

		virtual property_container_ptr	get_property_set( ) { return &clip_descriptor_props_; }
		virtual clip_instance*			create_instance( );

	private:
		void set_clip_descriptor_properties( )
		{
			clip_descriptor_props_.append( clip_descriptor_prop_type_ = pl::string( kOfxTypeClip ) );
			clip_descriptor_props_.append( clip_descriptor_prop_name_ = pl::string( "" ) );
			clip_descriptor_props_.append( clip_descriptor_prop_label_ = pl::string( "" ) );
			clip_descriptor_props_.append( clip_descriptor_prop_short_label_ = pl::string( "" ) );
			clip_descriptor_props_.append( clip_descriptor_prop_long_label_ = pl::string( "" ) );
			clip_descriptor_props_.append( clip_descriptor_prop_supported_components_ = pl::string_vec( ) );
			clip_descriptor_props_.append( clip_descriptor_prop_temporal_clip_access_ = 0 );
			clip_descriptor_props_.append( clip_descriptor_prop_optional_ = 0 );
			clip_descriptor_props_.append( clip_descriptor_prop_field_extraction_ = pl::string( kOfxImageFieldDoubled ) );
			clip_descriptor_props_.append( clip_descriptor_prop_is_mask_ = 0 );
			clip_descriptor_props_.append( clip_descriptor_prop_supports_tiles_ = 1 );
		}

	// Clip descriptor properties.
	private:
		pcos::property_container clip_descriptor_props_;
		pcos::property clip_descriptor_prop_type_;
		pcos::property clip_descriptor_prop_name_;
		pcos::property clip_descriptor_prop_label_;
		pcos::property clip_descriptor_prop_short_label_;
		pcos::property clip_descriptor_prop_long_label_;
		pcos::property clip_descriptor_prop_supported_components_;
		pcos::property clip_descriptor_prop_temporal_clip_access_;
		pcos::property clip_descriptor_prop_optional_;
		pcos::property clip_descriptor_prop_field_extraction_;
		pcos::property clip_descriptor_prop_is_mask_;
		pcos::property clip_descriptor_prop_supports_tiles_;
	};

	class interact_descriptor : public Ofx_base
	{
	public:
		explicit interact_descriptor( )
			: interact_descriptor_prop_has_alpha_( pcos::key::from_string( kOfxInteractPropHasAlpha ) )
			, interact_descriptor_prop_bit_depth_( pcos::key::from_string( kOfxInteractPropBitDepth ) )
		{
			set_interact_descriptor_properties( );
		}
		
		virtual property_container_ptr	get_property_set( ) { return &interact_descriptor_props_; }

	private:
		void set_interact_descriptor_properties( )
		{
			interact_descriptor_props_.append( interact_descriptor_prop_has_alpha_ = 1 );
			interact_descriptor_props_.append( interact_descriptor_prop_bit_depth_ = 8 );
		}

	private:
		pcos::property_container interact_descriptor_props_;
		pcos::property interact_descriptor_prop_has_alpha_;
		pcos::property interact_descriptor_prop_bit_depth_;
	};
	
	class image_effect_descriptor : public param_set_descriptor
	{
	public:
		typedef pl::string key_type;
		typedef boost::shared_ptr<clip_descriptor> clip_descriptor_ptr;

#if _MSC_VER >= 1400
		typedef stdext::hash_map<key_type, clip_descriptor_ptr> container;
#else
		typedef std::map<key_type, clip_descriptor_ptr> container;
#endif
		typedef container::const_iterator const_iterator;

	public:
		explicit image_effect_descriptor( )
			: image_effect_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, image_effect_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
			, image_effect_prop_short_label_( pcos::key::from_string( kOfxPropShortLabel ) )
			, image_effect_prop_long_label_( pcos::key::from_string( kOfxPropLongLabel ) )
			, image_effect_prop_supported_contexts_( pcos::key::from_string( kOfxImageEffectPropSupportedContexts ) )
			, image_effect_prop_grouping_( pcos::key::from_string( kOfxImageEffectPluginPropGrouping ) )
			, image_effect_prop_single_instance_( pcos::key::from_string( kOfxImageEffectPluginPropSingleInstance ) )
			, image_effect_render_thread_safety_( pcos::key::from_string( kOfxImageEffectPluginRenderThreadSafety ) )
			, image_effect_prop_host_frame_threading_( pcos::key::from_string( kOfxImageEffectPluginPropHostFrameThreading ) )
			, image_effect_prop_overlay_interactV1_( pcos::key::from_string( kOfxImageEffectPluginPropOverlayInteractV1 ) )
			, image_effect_prop_supports_multiresolution_( pcos::key::from_string( kOfxImageEffectPropSupportsMultiResolution ) )
			, image_effect_prop_supports_tiles_( pcos::key::from_string( kOfxImageEffectPropSupportsTiles ) )
			, image_effect_prop_temporal_clip_access_( pcos::key::from_string( kOfxImageEffectPropTemporalClipAccess ) )
			, image_effect_prop_supported_pixel_depths_( pcos::key::from_string( kOfxImageEffectPropSupportedPixelDepths ) )
			, image_effect_prop_field_render_twice_always_( pcos::key::from_string( kOfxImageEffectPluginPropFieldRenderTwiceAlways ) )
			, image_effect_prop_supports_multiple_clip_depths_( pcos::key::from_string( kOfxImageEffectPropSupportsMultipleClipDepths ) )
			, image_effect_prop_supports_multiple_clip_pars_( pcos::key::from_string( kOfxImageEffectPropSupportsMultipleClipPARs ) )
			, image_effect_prop_clip_preferences_slave_param_( pcos::key::from_string( kOfxImageEffectPropClipPreferencesSlaveParam ) )
			, image_effect_prop_file_path_( pcos::key::from_string( kOfxPluginPropFilePath ) )
		{
			set_image_effect_descriptor_properties( );
		}

		virtual property_container_ptr get_property_set( ) { return &effect_descriptor_props_; }

		clip_descriptor* clip_get_handle( const pl::string& name )
		{	
			const_iterator I = clip_desc_.find( name );
			if( I != clip_desc_.end( ) )
				return I->second.get( );
			return 0;
		}
		
		clip_descriptor* append_clip( const pl::string& name )
		{
			clip_descriptor_ptr clip( new clip_descriptor( ) );
			clip_desc_.insert( container::value_type( name, clip ) );
			return clip.get( );
		}
		
		const_iterator clip_begin( ) const { return clip_desc_.begin( ); }
		const_iterator clip_end( )   const { return clip_desc_.end( ); }
		
		OfxPluginEntryPoint* overlay_interact( ) const
		{
			OfxPluginEntryPoint* interact_main = 
				( OfxPluginEntryPoint* ) effect_descriptor_props_.get_property_with_key( pcos::key::from_string( kOfxImageEffectPluginPropOverlayInteractV1 ) ).value<void*>( );
				
			return interact_main;
		}

	private:
		void set_image_effect_descriptor_properties( )
		{
			pl::string_vec pixel_depths;
			pixel_depths.push_back( kOfxBitDepthNone );
			
			effect_descriptor_props_.append( image_effect_prop_type_ = pl::string( kOfxTypeImageEffect ) );
			effect_descriptor_props_.append( image_effect_prop_label_ = pl::string( kOfxTypeImageEffect ) );
			effect_descriptor_props_.append( image_effect_prop_short_label_ = pl::string( kOfxTypeImageEffect ) );
			effect_descriptor_props_.append( image_effect_prop_long_label_ = pl::string( kOfxTypeImageEffect ) );
			effect_descriptor_props_.append( image_effect_prop_supported_contexts_ = pl::string_vec( ) );
			effect_descriptor_props_.append( image_effect_prop_grouping_ = pl::string( "" ) );
			effect_descriptor_props_.append( image_effect_prop_single_instance_ = 0 );
			effect_descriptor_props_.append( image_effect_render_thread_safety_ = pl::string( kOfxImageEffectRenderUnsafe ) );
			effect_descriptor_props_.append( image_effect_prop_host_frame_threading_ = 0 );
			effect_descriptor_props_.append( image_effect_prop_overlay_interactV1_ = 0 );
			effect_descriptor_props_.append( image_effect_prop_supports_multiresolution_ = 1 );
			effect_descriptor_props_.append( image_effect_prop_supports_tiles_ = 0 );
			effect_descriptor_props_.append( image_effect_prop_temporal_clip_access_ = 1 );
			effect_descriptor_props_.append( image_effect_prop_supported_pixel_depths_ = pixel_depths );
			effect_descriptor_props_.append( image_effect_prop_field_render_twice_always_ = 0 );
			effect_descriptor_props_.append( image_effect_prop_supports_multiple_clip_depths_ = 1 );
			effect_descriptor_props_.append( image_effect_prop_supports_multiple_clip_pars_ = 1 );
			effect_descriptor_props_.append( image_effect_prop_clip_preferences_slave_param_ = pl::string( "" ) );
			effect_descriptor_props_.append( image_effect_prop_file_path_ = pl::string( "" ) );
		}

	// Effect descriptor properties.
	private:
		pcos::property_container effect_descriptor_props_;
		pcos::property image_effect_prop_type_;
		pcos::property image_effect_prop_label_;
		pcos::property image_effect_prop_short_label_;
		pcos::property image_effect_prop_long_label_;
		pcos::property image_effect_prop_supported_contexts_;
		pcos::property image_effect_prop_grouping_;
		pcos::property image_effect_prop_single_instance_;
		pcos::property image_effect_render_thread_safety_;
		pcos::property image_effect_prop_host_frame_threading_;
		pcos::property image_effect_prop_overlay_interactV1_;
		pcos::property image_effect_prop_supports_multiresolution_;
		pcos::property image_effect_prop_supports_tiles_;
		pcos::property image_effect_prop_temporal_clip_access_;
		pcos::property image_effect_prop_supported_pixel_depths_;
		pcos::property image_effect_prop_field_render_twice_always_;
		pcos::property image_effect_prop_supports_multiple_clip_depths_;
		pcos::property image_effect_prop_supports_multiple_clip_pars_;
		pcos::property image_effect_prop_clip_preferences_slave_param_;
		pcos::property image_effect_prop_file_path_;
		
	private:
		container clip_desc_;
	};

	class clip_instance : public Ofx_base
	{
	public:
		explicit clip_instance( clip_descriptor& cd )
			: clip_instance_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, clip_instance_prop_name_( pcos::key::from_string( kOfxPropName ) )
			, clip_instance_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
			, clip_instance_prop_short_label_( pcos::key::from_string( kOfxPropShortLabel ) )
			, clip_instance_prop_long_label_( pcos::key::from_string( kOfxPropLongLabel ) )
			, clip_instance_prop_supported_components_( pcos::key::from_string( kOfxImageEffectPropSupportedComponents ) )
			, clip_instance_prop_temporal_clip_access_( pcos::key::from_string( kOfxImageEffectPropTemporalClipAccess ) )
			, clip_instance_prop_optional_( pcos::key::from_string( kOfxImageClipPropOptional ) )
			, clip_instance_prop_field_extraction_( pcos::key::from_string( kOfxImageClipPropFieldExtraction ) )
			, clip_instance_prop_is_mask_( pcos::key::from_string( kOfxImageClipPropIsMask ) )
			, clip_instance_prop_supports_tiles_( pcos::key::from_string( kOfxImageEffectPropSupportsTiles ) )
			, clip_instance_prop_pixel_depth_( pcos::key::from_string( kOfxImageEffectPropPixelDepth ) )
			, clip_instance_prop_components_( pcos::key::from_string( kOfxImageEffectPropComponents ) )
			, clip_instance_prop_unmapped_pixel_depth_( pcos::key::from_string( kOfxImageClipPropUnmappedPixelDepth ) )
			, clip_instance_prop_unmapped_components_( pcos::key::from_string( kOfxImageClipPropUnmappedComponents ) )
			, clip_instance_prop_premultiplication_( pcos::key::from_string( kOfxImageEffectPropPreMultiplication ) )
			, clip_instance_prop_pixel_aspect_ratio_( pcos::key::from_string( kOfxImagePropPixelAspectRatio ) )
			, clip_instance_prop_frame_rate_( pcos::key::from_string( kOfxImageEffectPropFrameRate ) )
			, clip_instance_prop_frame_range_( pcos::key::from_string( kOfxImageEffectPropFrameRange ) )
			, clip_instance_prop_field_order_( pcos::key::from_string( kOfxImageClipPropFieldOrder ) )
			, clip_instance_prop_connected_( pcos::key::from_string( kOfxImageClipPropConnected ) )
			, clip_instance_prop_unmapped_frame_range_( pcos::key::from_string( kOfxImageEffectPropUnmappedFrameRange ) )
			, clip_instance_prop_unmapped_frame_rate_( pcos::key::from_string( kOfxImageEffectPropUnmappedFrameRate ) )
			, clip_instance_prop_continuous_samples_( pcos::key::from_string( kOfxImageClipPropContinuousSamples ) )
		{
			set_clip_instance_properties( cd );
		}

		virtual property_container_ptr get_property_set( ) { return &clip_instance_props_; }

		void set_image( il::image_type_ptr im )
		{
			pl::string depth;
			
			im->bitdepth( ) == 8  ? depth = pl::string( kOfxBitDepthByte )  : 
			im->bitdepth( ) == 32 ? depth = pl::string( kOfxBitDepthFloat ) : depth = pl::string( kOfxBitDepthShort );
			
			clip_instance_prop_unmapped_pixel_depth_ = depth;
			clip_instance_prop_pixel_depth_			 = depth;
			
			im_.set_image( im, depth );
			img_ = im; 
		}
		
		property_container_ptr	get_image_property_set( ) { return im_.get_property_set( ); }
		il::image_type_ptr		get_image( )			  { return img_; }

	private:
		void set_clip_instance_properties( clip_descriptor& cd )
		{
			clip_instance_props_.append( clip_instance_prop_type_ = pl::string( kOfxTypeClip ) );
			clip_instance_props_.append( clip_instance_prop_name_ = pl::string( "" ) );
			clip_instance_props_.append( clip_instance_prop_label_ = pl::string( "" ) );
			clip_instance_props_.append( clip_instance_prop_short_label_ = pl::string( "" ) );
			clip_instance_props_.append( clip_instance_prop_long_label_ = pl::string( "" ) );
			clip_instance_props_.append( clip_instance_prop_supported_components_ = pl::string_vec( ) );
			clip_instance_props_.append( clip_instance_prop_temporal_clip_access_ = 0 );
			clip_instance_props_.append( clip_instance_prop_optional_ = 0 );
			clip_instance_props_.append( clip_instance_prop_field_extraction_ = pl::string( kOfxImageFieldDoubled ) );
			clip_instance_props_.append( clip_instance_prop_is_mask_ = 0 );
			clip_instance_props_.append( clip_instance_prop_supports_tiles_ = 0 );
			clip_instance_props_.append( clip_instance_prop_pixel_depth_ = pl::string( kOfxBitDepthNone ) );
			clip_instance_props_.append( clip_instance_prop_components_ = pl::string( kOfxImageComponentNone ) );
			clip_instance_props_.append( clip_instance_prop_unmapped_pixel_depth_ = pl::string( kOfxBitDepthNone ) );
			clip_instance_props_.append( clip_instance_prop_unmapped_components_ = pl::string( kOfxImageComponentRGBA ) );
			clip_instance_props_.append( clip_instance_prop_premultiplication_ = pl::string( kOfxImageOpaque ) );
			clip_instance_props_.append( clip_instance_prop_pixel_aspect_ratio_ = 1.0 );
			clip_instance_props_.append( clip_instance_prop_frame_rate_ = 0.0 );
			clip_instance_props_.append( clip_instance_prop_frame_range_ = pcos::double_vec( 2 ) );
			clip_instance_props_.append( clip_instance_prop_field_order_ = pl::string( kOfxImageFieldNone ) );
			clip_instance_props_.append( clip_instance_prop_connected_ = 0 );
			clip_instance_props_.append( clip_instance_prop_unmapped_frame_range_ = pcos::double_vec( 2 ) );
			clip_instance_props_.append( clip_instance_prop_unmapped_frame_rate_ = 0.0 );
			clip_instance_props_.append( clip_instance_prop_continuous_samples_ = 0 );			
		}
	
	// Clip instance properties.
	private:
		pcos::property_container clip_instance_props_;
		pcos::property clip_instance_prop_type_;
		pcos::property clip_instance_prop_name_;
		pcos::property clip_instance_prop_label_;
		pcos::property clip_instance_prop_short_label_;
		pcos::property clip_instance_prop_long_label_;
		pcos::property clip_instance_prop_supported_components_;
		pcos::property clip_instance_prop_temporal_clip_access_;
		pcos::property clip_instance_prop_optional_;
		pcos::property clip_instance_prop_field_extraction_;
		pcos::property clip_instance_prop_is_mask_;
		pcos::property clip_instance_prop_supports_tiles_;
		pcos::property clip_instance_prop_pixel_depth_;
		pcos::property clip_instance_prop_components_;
		pcos::property clip_instance_prop_unmapped_pixel_depth_;
		pcos::property clip_instance_prop_unmapped_components_;
		pcos::property clip_instance_prop_premultiplication_;
		pcos::property clip_instance_prop_pixel_aspect_ratio_;
		pcos::property clip_instance_prop_frame_rate_;
		pcos::property clip_instance_prop_frame_range_;
		pcos::property clip_instance_prop_field_order_;
		pcos::property clip_instance_prop_connected_;
		pcos::property clip_instance_prop_unmapped_frame_range_;
		pcos::property clip_instance_prop_unmapped_frame_rate_;
		pcos::property clip_instance_prop_continuous_samples_;
		
	private:
		image im_;
		il::image_type_ptr img_;
	};
	
	class image_effect_instance : public param_set_base
	{
	public:
		typedef pl::string key_type;
		typedef boost::shared_ptr<clip_instance> clip_instance_ptr;
		typedef boost::shared_ptr<param_set>	 param_set_ptr;

#if _MSC_VER >= 1400
		typedef stdext::hash_map<key_type, clip_instance_ptr> container;
#else
		typedef std::map<key_type, clip_instance_ptr> container;
#endif
		typedef container::iterator			iterator;
		typedef container::const_iterator	const_iterator;

	public:
		explicit image_effect_instance( )
			: image_effect_instance_prop_type_( pcos::key::from_string( kOfxPropType ) )
			, image_effect_instance_prop_context_( pcos::key::from_string( kOfxImageEffectPropContext ) )
			, image_effect_instance_prop_instance_data_( pcos::key::from_string( kOfxPropInstanceData ) )
			, image_effect_instance_prop_project_size_( pcos::key::from_string( kOfxImageEffectPropProjectSize ) )
			, image_effect_instance_prop_project_offset_( pcos::key::from_string( kOfxImageEffectPropProjectOffset ) )
			, image_effect_instance_prop_project_extent_( pcos::key::from_string( kOfxImageEffectPropProjectExtent ) )
			, image_effect_instance_prop_project_pixel_aspect_ratio_( pcos::key::from_string( kOfxImageEffectPropProjectPixelAspectRatio ) )
			, image_effect_instance_prop_effect_duration_( pcos::key::from_string( kOfxImageEffectInstancePropEffectDuration ) )
			, image_effect_instance_prop_sequential_render_( pcos::key::from_string( kOfxImageEffectInstancePropSequentialRender ) )
			, image_effect_instance_prop_frame_rate_( pcos::key::from_string( kOfxImageEffectPropFrameRate ) )
			, image_effect_instance_prop_is_interactive_( pcos::key::from_string( kOfxPropIsInteractive ) )
		{
			set_image_effect_instance_properties( );
		}

		void init( image_effect_descriptor& id )
		{
			param_set_.reset( id.create_instance( ) );
			
			for( image_effect_descriptor::const_iterator I = id.clip_begin( ); I != id.clip_end( ); ++I )
				clip_instance_.insert( container::value_type( I->first, clip_instance_ptr( I->second->create_instance( ) ) ) );
		}

		virtual property_container_ptr	get_property_set( ) { return &effect_instance_props_; }
		OfxParamSetHandle				get_param_set( )	{ return param_set_->get_param_set( ); }

		clip_instance* clip_get_handle( const pl::string& name )
		{	
			const_iterator I = clip_instance_.find( name );
			if( I != clip_instance_.end( ) )
				return I->second.get( );
			return 0;
		}

		bool set_source( il::image_type_ptr im ) { return set_clip_image( "Source", im ); }
		bool set_output( il::image_type_ptr im ) { return set_clip_image( "Output", im ); }
	
	private:
		bool set_clip_image( const pl::string& name, il::image_type_ptr im )
		{
			iterator I = clip_instance_.find( name );
			if( I != clip_instance_.end( ) )
			{
				I->second->set_image( im );
				return true;
			}
			
			return false;
		}
		
	private:
		void set_image_effect_instance_properties( )
		{
			effect_instance_props_.append( image_effect_instance_prop_type_ = pl::string( kOfxTypeImageEffectInstance ) );
			effect_instance_props_.append( image_effect_instance_prop_context_ = pl::string( kOfxImageEffectContextFilter ) );
			effect_instance_props_.append( image_effect_instance_prop_instance_data_ = static_cast<void*>( 0 ) );
			effect_instance_props_.append( image_effect_instance_prop_project_size_ = pcos::double_vec( 2 ) );
			effect_instance_props_.append( image_effect_instance_prop_project_offset_ = pcos::double_vec( 2 ) );
			effect_instance_props_.append( image_effect_instance_prop_project_extent_ = pcos::double_vec( 2 ) );
			effect_instance_props_.append( image_effect_instance_prop_project_pixel_aspect_ratio_ = 0.0 );
			effect_instance_props_.append( image_effect_instance_prop_effect_duration_ = 0.0 );
			effect_instance_props_.append( image_effect_instance_prop_sequential_render_ = 0 );
			effect_instance_props_.append( image_effect_instance_prop_frame_rate_ = 0.0 );
			effect_instance_props_.append( image_effect_instance_prop_is_interactive_ = 0 );
		}
	
	// Effect instance properties.
	private:
		pcos::property_container effect_instance_props_;
		pcos::property image_effect_instance_prop_type_;
		pcos::property image_effect_instance_prop_context_;
		pcos::property image_effect_instance_prop_instance_data_;
		pcos::property image_effect_instance_prop_project_size_;
		pcos::property image_effect_instance_prop_project_offset_;
		pcos::property image_effect_instance_prop_project_extent_;
		pcos::property image_effect_instance_prop_project_pixel_aspect_ratio_;
		pcos::property image_effect_instance_prop_effect_duration_;
		pcos::property image_effect_instance_prop_sequential_render_;
		pcos::property image_effect_instance_prop_frame_rate_;
		pcos::property image_effect_instance_prop_is_interactive_;

	private:
		container clip_instance_;
		param_set_ptr param_set_;
	};

	class interact_instance : public Ofx_base
	{
	public:
		explicit interact_instance( )
			: interact_instance_prop_effect_instance_( pcos::key::from_string( kOfxPropEffectInstance ) )
			, interact_instance_prop_instance_data_( pcos::key::from_string( kOfxPropInstanceData ) )
			, interact_instance_prop_pixel_scale_( pcos::key::from_string( kOfxInteractPropPixelScale ) )
			, interact_instance_prop_background_colour_( pcos::key::from_string( kOfxInteractPropBackgroundColour ) )
			, interact_instance_prop_viewport_size_( pcos::key::from_string( kOfxInteractPropViewportSize ) )
			, interact_instance_prop_has_alpha_( pcos::key::from_string( kOfxInteractPropHasAlpha ) )
			, interact_instance_prop_bit_depth_( pcos::key::from_string( kOfxInteractPropBitDepth ) )
			, interact_instance_prop_slave_to_param_( pcos::key::from_string( kOfxInteractPropSlaveToParam ) )
		{ }

		void							init( interact_descriptor& id, image_effect_instance& effect ) { set_interact_instance_properties( id, effect ); }
		virtual property_container_ptr	get_property_set( ) { return &interact_instance_props_; }
		
	private:
		void set_interact_instance_properties( interact_descriptor&, image_effect_instance& effect )
		{
			interact_instance_props_.append( interact_instance_prop_effect_instance_ = ( void* ) &effect );
			interact_instance_props_.append( interact_instance_prop_instance_data_ = ( void* ) 0 );
			interact_instance_props_.append( interact_instance_prop_pixel_scale_ = pcos::double_vec( 2, 0.0 ) );
			interact_instance_props_.append( interact_instance_prop_background_colour_ = pcos::double_vec( 3, 0.0 ) );
			interact_instance_props_.append( interact_instance_prop_viewport_size_ = pcos::int_vec( 2, 0 ) ); // deprecated
			interact_instance_props_.append( interact_instance_prop_has_alpha_ = 1 );
			interact_instance_props_.append( interact_instance_prop_bit_depth_ = 8 );
			interact_instance_props_.append( interact_instance_prop_slave_to_param_ = pl::string_vec( ) );
		}

	private:
		pcos::property_container interact_instance_props_;
		pcos::property interact_instance_prop_effect_instance_;
		pcos::property interact_instance_prop_instance_data_;
		pcos::property interact_instance_prop_pixel_scale_;
		pcos::property interact_instance_prop_background_colour_;
		pcos::property interact_instance_prop_viewport_size_;
		pcos::property interact_instance_prop_has_alpha_;
		pcos::property interact_instance_prop_bit_depth_;
		pcos::property interact_instance_prop_slave_to_param_;
	};
		
	OfxStatus param_set_descriptor::define( const pl::string& param_type, const pl::string& name )
	{
		typedef container::const_iterator const_iterator;
			
		const_iterator I;
		if( ( I = params_.find( name ) ) == params_.end( ) )
		{
			if( param_type == kOfxParamTypeInteger )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new int_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeInteger2D )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new int2D_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeInteger3D )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new int3D_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeDouble )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new double_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeDouble2D )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new double2D_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeDouble3D )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new double3D_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeRGB )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new rgb_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeRGBA )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new rgba_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeBoolean )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new boolean_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeChoice )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new choice_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeGroup )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new group_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypePage )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new page_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeCustom )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new custom_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypePushButton )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new push_button_param_descriptor( ) ) ) );
			}
			else if( param_type == kOfxParamTypeString )
			{
				params_.insert( container::value_type( name, param_descriptor_ptr( new string_param_descriptor( ) ) ) );
			}
		}
		else
		{
			// parameter already exists. if the type is the same return success. otherwise flag as an error.
		}
			
		return kOfxStatOK;
	}

	param* string_param_descriptor::create_instance( )			{ return new string_param( *this ); }
	param* int_param_descriptor::create_instance( )				{ return new int_param( *this ); }
	param* int2D_param_descriptor::create_instance( )			{ return new int2D_param( *this ); }
	param* int3D_param_descriptor::create_instance( )			{ return new int3D_param( *this ); }
	param* double_param_descriptor::create_instance( )			{ return new double_param( *this ); }
	param* double2D_param_descriptor::create_instance( )		{ return new double2D_param( *this ); }
	param* double3D_param_descriptor::create_instance( )		{ return new double3D_param( *this ); }
	param* rgb_param_descriptor::create_instance( )				{ return new rgb_param( *this ); }
	param* rgba_param_descriptor::create_instance( )			{ return new rgba_param( *this ); }
	param* boolean_param_descriptor::create_instance( )			{ return new boolean_param( *this ); }
	param* choice_param_descriptor::create_instance( )			{ return new choice_param( *this ); }
	param* group_param_descriptor::create_instance( )			{ return new group_param( *this ); }
	param* page_param_descriptor::create_instance( )			{ return new page_param( *this ); }
	param* push_button_param_descriptor::create_instance( )		{ return new push_button_param( *this ); }
	param* custom_param_descriptor::create_instance( )			{ return new custom_param( *this ); }
	param_set* param_set_descriptor::create_instance( )			{ return new param_set( *this ); }
	clip_instance* clip_descriptor::create_instance( )			{ return new clip_instance( *this ); }
}

class ML_PLUGIN_DECLSPEC ofx_filter : public filter_type, public OfxHost
{
public:
	explicit ofx_filter( OfxPlugin* plug, ofx_context context )
		: plug_( plug )
		, context_( context )
		, host_prop_type_( pcos::key::from_string( kOfxPropType ) )
		, host_prop_name_( pcos::key::from_string( kOfxPropName ) )
		, host_prop_label_( pcos::key::from_string( kOfxPropLabel ) )
		, host_prop_is_background_( pcos::key::from_string( kOfxImageEffectHostPropIsBackground ) )
		, host_prop_supports_overlays_( pcos::key::from_string( kOfxImageEffectPropSupportsOverlays ) )
		, host_prop_supported_pixel_depths_( pcos::key::from_string( kOfxImageEffectPropSupportedPixelDepths ) )
		, host_prop_supports_multiresolution_( pcos::key::from_string( kOfxImageEffectPropSupportsMultiResolution ) )
		, host_prop_supports_tiles_( pcos::key::from_string( kOfxImageEffectPropSupportsTiles ) )
		, host_prop_temporal_clip_access_( pcos::key::from_string( kOfxImageEffectPropTemporalClipAccess ) )
		, host_prop_supported_components_( pcos::key::from_string( kOfxImageEffectPropSupportedComponents ) )
		, host_prop_supported_contexts_( pcos::key::from_string( kOfxImageEffectPropSupportedContexts ) )
		, host_prop_supports_multiple_clip_depths_( pcos::key::from_string( kOfxImageEffectPropSupportsMultipleClipDepths ) )
		, host_prop_supports_multiple_clip_pars_( pcos::key::from_string( kOfxImageEffectPropSupportsMultipleClipPARs ) )
		, host_prop_setable_frame_rate_( pcos::key::from_string( kOfxImageEffectPropSetableFrameRate ) )
		, host_prop_setable_fielding_( pcos::key::from_string( kOfxImageEffectPropSetableFielding ) )
		, host_prop_supports_custom_interact_( pcos::key::from_string( kOfxParamHostPropSupportsCustomInteract ) )
		, host_prop_supports_string_animation_( pcos::key::from_string( kOfxParamHostPropSupportsStringAnimation ) )
		, host_prop_supports_choice_animation_( pcos::key::from_string( kOfxParamHostPropSupportsChoiceAnimation ) )
		, host_prop_supports_boolean_animation_( pcos::key::from_string( kOfxParamHostPropSupportsBooleanAnimation ) )
		, host_prop_supports_custom_animation_( pcos::key::from_string( kOfxParamHostPropSupportsCustomAnimation ) )
		, host_prop_max_parameters_( pcos::key::from_string( kOfxParamHostPropMaxParameters ) )
		, host_prop_max_pages_( pcos::key::from_string( kOfxParamHostPropMaxPages ) )
		, host_prop_page_row_column_count_( pcos::key::from_string( kOfxParamHostPropPageRowColumnCount ) )
		, host_prop_this_pointer_( pcos::key::from_string( "this_pointer" ) )
		, interact_main_( 0 )
		, render_prop_name_( pcos::key::from_string( kOfxPropName ) )
		, render_prop_time_( pcos::key::from_string( kOfxPropTime ) )
		, render_prop_field_to_render_( pcos::key::from_string( kOfxImageEffectPropFieldToRender ) )
		, render_prop_render_window_( pcos::key::from_string( kOfxImageEffectPropRenderWindow ) )
		, render_prop_render_scale_( pcos::key::from_string( kOfxImageEffectPropRenderScale ) )
		, sequence_prop_frame_range_( pcos::key::from_string( kOfxImageEffectPropFrameRange ) )
		, sequence_prop_frame_step_( pcos::key::from_string( kOfxImageEffectPropFrameStep ) )
		, sequence_prop_is_interactive_( pcos::key::from_string( kOfxPropIsInteractive ) )
		, sequence_prop_render_scale_( pcos::key::from_string( kOfxImageEffectPropRenderScale ) )
		, clip_preferences_prop_components_source_( pcos::key::from_string( "OfxImageClipPropComponents_Source" ) )
		, clip_preferences_prop_components_output_( pcos::key::from_string( "OfxImageClipPropComponents_Output" ) )
		, clip_preferences_prop_depth_source_( pcos::key::from_string( "OfxImageClipPropDepth_Source" ) )
		, clip_preferences_prop_depth_output_( pcos::key::from_string( "OfxImageClipPropDepth_Output" ) )
		, clip_preferences_prop_par_source_( pcos::key::from_string( "OfxImageClipPropPAR_Source" ) )
		, clip_preferences_prop_par_output_( pcos::key::from_string( "OfxImageClipPropPAR_Output" ) )
		, clip_preferences_prop_frame_rate_( pcos::key::from_string( kOfxImageEffectPropFrameRate ) )
		, clip_preferences_prop_field_order_( pcos::key::from_string( kOfxImageClipPropFieldOrder ) )
		, clip_preferences_prop_premultiplication_( pcos::key::from_string( kOfxImageEffectPropPreMultiplication ) )
		, clip_preferences_prop_continuous_samples_( pcos::key::from_string( kOfxImageClipPropContinuousSamples ) )
		, clip_preferences_prop_frame_varying_( pcos::key::from_string( kOfxImageEffectFrameVarying ) )
		, apply_clip_prefs_( false )
	{
		// set host properties and suite functions.
		set_host( );
		set_suites( );
		set_host_parameters( );

		// load and describe the plugin.
		plug_->mainEntry( kOfxActionLoad, 0, 0, 0 );
		plug_->mainEntry( kOfxActionDescribe, &image_effect_descriptor_, 0, 0 );
	
		describe_in_context( );

		// create instance from descriptor.
		image_effect_instance_.init( image_effect_descriptor_ );
		
		// create any overlay instances...
		if( ( interact_main_ = image_effect_descriptor_.overlay_interact( ) ) != 0 )
		{
			interact_main_( kOfxActionDescribe, &overlay_interact_descriptor_, 0, 0 );
			overlay_interact_instance_.init( overlay_interact_descriptor_, image_effect_instance_ );
		}
		
		plug_->mainEntry( kOfxActionCreateInstance, &image_effect_instance_, 0, 0 );
		if( interact_main_ )
			interact_main_( kOfxActionCreateInstance, &overlay_interact_instance_, 0, 0 );
		
		// get plugin preferences.
		set_clip_prefs_ctx( );
		if( plug_->mainEntry( kOfxImageEffectActionGetClipPreferences, &image_effect_instance_, 0, ( OfxPropertySetHandle ) &clip_preferences_props_filter_ ) == kOfxStatOK )
			apply_clip_prefs_ = true;
	}
	
	virtual ~ofx_filter( )
	{
		if( interact_main_ )
			interact_main_( kOfxActionDestroyInstance, &overlay_interact_instance_, 0, 0 );
		
		plug_->mainEntry( kOfxActionDestroyInstance, &image_effect_instance_, 0, 0 );
		plug_->mainEntry( kOfxActionUnload, 0, 0, 0 );
	}
	
	virtual const opl::wstring get_uri( ) const
	{ return pl::to_wstring( plug_->pluginIdentifier ); }

	virtual frame_type_ptr fetch( )
	{
		acquire_values( );

		frame_type_ptr result = fetch_from_slot( );
		if( result && result->get_image( ) )
		{
			il::image_type_ptr src = result->get_image( );
			il::image_type_ptr dst;
			
			if( context_ == FILTER )
				convert_to_filter_ctx( src, dst );
			
			pcos::property_container render_in_args;
			pcos::property_container render_out_args;

			pcos::int_vec render_win( 4, 0 );
			render_win[ 2 ] = src->width( );
			render_win[ 3 ] = src->height( );

			render_in_args.append( render_prop_time_ = 0.0 );
			render_in_args.append( render_prop_field_to_render_ = pl::string( kOfxImageFieldNone ) );
			render_in_args.append( render_prop_render_window_ = render_win );
			render_in_args.append( render_prop_render_scale_ = pcos::int_vec( 2, 1 ) );

			render_out_args.append( render_prop_name_ );
			render_out_args.append( render_prop_time_ = 0.0 );

			image_effect_instance_.set_source( src );
			image_effect_instance_.set_output( dst );

			OfxStatus is_identity = plug_->mainEntry( kOfxImageEffectActionIsIdentity, &image_effect_instance_, 
											( OfxPropertySetHandle ) &render_in_args, ( OfxPropertySetHandle ) &render_out_args );
			if( is_identity == kOfxStatReplyDefault )
			{
				pcos::property_container sequence_in_args;
				sequence_in_args.append( sequence_prop_frame_range_ = pcos::double_vec( 2, 1.0 ) );
				sequence_in_args.append( sequence_prop_frame_step_ = 1.0 );
				sequence_in_args.append( sequence_prop_is_interactive_ = 0 );
				sequence_in_args.append( sequence_prop_render_scale_ = pcos::double_vec( 2, 1.0 ) );

				plug_->mainEntry( kOfxImageEffectActionBeginSequenceRender, &image_effect_instance_, ( OfxPropertySetHandle ) &sequence_in_args, 0 );
				plug_->mainEntry( kOfxImageEffectActionRender, &image_effect_instance_, ( OfxPropertySetHandle ) &render_in_args, 0 );
				plug_->mainEntry( kOfxImageEffectActionEndSequenceRender, &image_effect_instance_, ( OfxPropertySetHandle ) &sequence_in_args, 0 );
			
				result->set_image( dst );
			}
			else if( is_identity == kOfxStatOK )
			{
				// select the appropriate clip from the out_args.
				pl::string out_clip = render_out_args.get_property_with_key( pcos::key::from_string( kOfxPropName ) ).value<pl::string>( );
				if( !out_clip.empty( ) )
				{
					clip_instance* out_instance = image_effect_instance_.clip_get_handle( out_clip );
					if( out_instance )
						result->set_image( out_instance->get_image( ) );
				}
			}
		}

		return result;
	}

private:
	// Host Fetch Suite.
	friend void* fetch_suite( OfxPropertySetHandle host, const char* suite_name, int suite_version )
	{
		property_container_ptr host_props_ptr = ( property_container_ptr ) host;
		if( host_props_ptr && suite_version == 1 )
		{
			ofx_filter* const plug = ( ofx_filter* const ) host_props_ptr->get_property_with_key( pcos::key::from_string( "this_pointer" ) ).value<void*>( );
		
			if( !strcmp( suite_name, kOfxImageEffectSuite ) )
				return ( void* ) &plug->image_effect_suite_;
			else if( !strcmp( suite_name, kOfxPropertySuite ) )
				return ( void* ) &plug->property_suite_;
			else if( !strcmp( suite_name, kOfxParameterSuite ) )
				return ( void* ) &plug->parameter_suite_;
			else if( !strcmp( suite_name, kOfxMemorySuite ) )
				return ( void* ) &plug->memory_suite_;
			else if( !strcmp( suite_name, kOfxMultiThreadSuite ) )
				return ( void* ) &plug->multithread_suite_;
			else if( !strcmp( suite_name, kOfxMessageSuite ) )
				return ( void* ) &plug->message_suite_;
			else if( !strcmp( suite_name, kOfxInteractSuite ) )
				return ( void* ) &plug->interact_suite_;
		}

		return 0;
	}

	void set_host( )
	{
		OfxHost::host = ( OfxPropertySetHandle ) &host_props_;
		OfxHost::fetchSuite = fetch_suite;
		
		plug_->setHost( this );
	}
	
	void set_host_parameters( )
	{
		pl::string_vec pixel_depths( 3 );
		pixel_depths[ 0 ] = pl::string( kOfxBitDepthByte );
		pixel_depths[ 1 ] = pl::string( kOfxBitDepthShort );
		pixel_depths[ 2 ] = pl::string( kOfxBitDepthFloat );
		
		pl::string_vec supported_components( 2 );
		supported_components[ 0 ] = kOfxImageComponentRGBA;
		supported_components[ 1 ] = kOfxImageComponentAlpha;
		
		pl::string_vec supported_contexts( 4 );
		supported_contexts[ 0 ] = kOfxImageEffectContextFilter;
		supported_contexts[ 1 ] = kOfxImageEffectContextTransition;
		supported_contexts[ 2 ] = kOfxImageEffectContextGenerator;
		supported_contexts[ 3 ] = kOfxImageEffectContextGeneral;
		
		host_props_.append( host_prop_type_ = pl::string( kOfxTypeImageEffectHost ) );
		host_props_.append( host_prop_name_ = pl::string( "org.openlibraries.ofx_host" ) );
		host_props_.append( host_prop_label_ = pl::string( "OpenLibraries OFX host" ) );
		host_props_.append( host_prop_is_background_ = 0 );
		host_props_.append( host_prop_supports_overlays_ = 1 );
		host_props_.append( host_prop_supported_pixel_depths_ = pixel_depths );
		host_props_.append( host_prop_supports_multiresolution_ = 1 );
		host_props_.append( host_prop_supports_tiles_ = 0 );
		host_props_.append( host_prop_temporal_clip_access_ = 0 );
		host_props_.append( host_prop_supported_components_ = supported_components );
		host_props_.append( host_prop_supported_contexts_ = supported_contexts );
		host_props_.append( host_prop_supports_multiple_clip_depths_ = 1 );
		host_props_.append( host_prop_supports_multiple_clip_pars_ = 1 );
		host_props_.append( host_prop_setable_frame_rate_ = 1 );
		host_props_.append( host_prop_setable_fielding_ = 1 );
		host_props_.append( host_prop_supports_custom_interact_ = 1 );
		host_props_.append( host_prop_supports_string_animation_ = 0 );
		host_props_.append( host_prop_supports_choice_animation_ = 0 );
		host_props_.append( host_prop_supports_boolean_animation_ = 0 );
		host_props_.append( host_prop_supports_custom_animation_ = 0 );
		host_props_.append( host_prop_max_parameters_ = -1 );
		host_props_.append( host_prop_max_pages_ = 0 );
		host_props_.append( host_prop_page_row_column_count_ = 0 );
		host_props_.append( host_prop_this_pointer_ = ( void* ) this );
	}
	
	void set_suites( )
	{
		set_property_suite( property_suite_ );
		set_image_effect_suite( image_effect_suite_ );
		set_parameter_suite( parameter_suite_ );
		set_memory_suite( memory_suite_ );
		set_multithread_suite( multithread_suite_ );
		set_interact_suite( interact_suite_ );
		set_message_suite( message_suite_ );
	}
	
	// For each of the contexts create a description of the effect.
	bool describe_in_context( )
	{
		pl::string context_str;
		if( context_ == FILTER )
			context_str = kOfxImageEffectContextFilter;
		
		pcos::property_container in_args;
		pcos::property context( pcos::key::from_string( kOfxImageEffectPropContext ) );
			
		in_args.append( context = context_str );
			
		return plug_->mainEntry( kOfxImageEffectActionDescribeInContext, &image_effect_descriptor_, ( OfxPropertySetHandle ) &in_args, 0 ) == kOfxStatOK;
	}

private:
	void set_clip_prefs_filter_ctx( )
	{
		clip_preferences_props_filter_.append( clip_preferences_prop_components_source_ = pl::string( ) );
		clip_preferences_props_filter_.append( clip_preferences_prop_components_output_ = pl::string( ) );
		clip_preferences_props_filter_.append( clip_preferences_prop_depth_source_ = pl::string( ) );
		clip_preferences_props_filter_.append( clip_preferences_prop_depth_output_ = pl::string( ) );
		clip_preferences_props_filter_.append( clip_preferences_prop_par_source_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_par_output_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_frame_rate_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_field_order_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_premultiplication_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_continuous_samples_ );
		clip_preferences_props_filter_.append( clip_preferences_prop_frame_varying_ );
	}
	
	void set_clip_prefs_generator_ctx( )
	{
	}
	
	void set_clip_prefs_ctx( )
	{
		if( context_ == FILTER ) set_clip_prefs_filter_ctx( );
		else if( context_ == GENERATOR ) set_clip_prefs_generator_ctx( );
	}
	
private:
	void convert_to_filter_ctx( il::image_type_ptr& src, il::image_type_ptr& dst )
	{
		if( apply_clip_prefs_ )
		{
			pl::string source_depth = clip_preferences_prop_depth_source_.value<pl::string>( );
			pl::string output_depth = clip_preferences_prop_depth_output_.value<pl::string>( );
		
			if( !source_depth.empty( ) )
			{
				if( source_depth == kOfxBitDepthByte  )		 src = il::convert( src, L"r8g8b8a8" );
				else if( source_depth == kOfxBitDepthShort ) src = il::convert( src, L"r16g16b16a16" );
				else if( source_depth == kOfxBitDepthFloat ) src = il::convert( src, L"r32g32b32a32f" );
			}
		
			if( !output_depth.empty( ) )
			{
				if( output_depth == kOfxBitDepthByte  )		 dst = il::allocate( L"r8g8b8a8", src->width( ), src->height( ) );
				else if( output_depth == kOfxBitDepthShort ) dst = il::allocate( L"r16g16b16a16", src->width( ), src->height( ) );
				else if( output_depth == kOfxBitDepthFloat ) dst = il::allocate( L"r32g32b32a32f", src->width( ), src->height( ) );
			}
			else
			{
				dst = il::allocate( src->pf( ), src->width( ), src->height( ) );
			}
		}
		else
		{
			if( src->is_float( ) )
			{
				src = il::convert( src, L"r32g32b32a32f" );
			}
			else
			{
				if( src->bitdepth( ) == 8 ) src = il::convert( src, L"r8g8b8a8" );
				else						src = il::convert( src, L"r16g16b16a16" );
			}
			
			dst = il::allocate( src->pf( ), src->width( ), src->height( ) );
		}
	}

private:
	OfxPlugin* plug_;
	ofx_context context_;

private:
	OfxPropertySuiteV1 property_suite_;
	OfxImageEffectSuiteV1 image_effect_suite_;
	OfxParameterSuiteV1 parameter_suite_;
	OfxMemorySuiteV1 memory_suite_;
	OfxMultiThreadSuiteV1 multithread_suite_;
	OfxInteractSuiteV1 interact_suite_;
	OfxMessageSuiteV1 message_suite_;

// Host properties.
private:
	pcos::property_container host_props_;
	pcos::property host_prop_type_;
	pcos::property host_prop_name_;
	pcos::property host_prop_label_;
	pcos::property host_prop_is_background_;
	pcos::property host_prop_supports_overlays_;
	pcos::property host_prop_supported_pixel_depths_;
	pcos::property host_prop_supports_multiresolution_;
	pcos::property host_prop_supports_tiles_;
	pcos::property host_prop_temporal_clip_access_;
	pcos::property host_prop_supported_components_;
	pcos::property host_prop_supported_contexts_;
	pcos::property host_prop_supports_multiple_clip_depths_;
	pcos::property host_prop_supports_multiple_clip_pars_;
	pcos::property host_prop_setable_frame_rate_;
	pcos::property host_prop_setable_fielding_;
	pcos::property host_prop_supports_custom_interact_;
	pcos::property host_prop_supports_string_animation_;
	pcos::property host_prop_supports_choice_animation_;
	pcos::property host_prop_supports_boolean_animation_;
	pcos::property host_prop_supports_custom_animation_;
	pcos::property host_prop_max_parameters_;
	pcos::property host_prop_max_pages_;
	pcos::property host_prop_page_row_column_count_;
	pcos::property host_prop_this_pointer_;

// Effect descriptor.
private:
	image_effect_descriptor image_effect_descriptor_;
	interact_descriptor		overlay_interact_descriptor_;
	
// Effect instance.
private:
	image_effect_instance image_effect_instance_;
	interact_instance	  overlay_interact_instance_;
	OfxPluginEntryPoint*  interact_main_;
	
// Render action properties.
private:
	pcos::property render_prop_name_;
	pcos::property render_prop_time_;
	pcos::property render_prop_field_to_render_;
	pcos::property render_prop_render_window_;
	pcos::property render_prop_render_scale_;

// Begin Sequence Render action properties.
private:
	pcos::property sequence_prop_frame_range_;
	pcos::property sequence_prop_frame_step_;
	pcos::property sequence_prop_is_interactive_;
	pcos::property sequence_prop_render_scale_;
	
// Clip Preferences action properties.
private:
	pcos::property_container clip_preferences_props_filter_;
	pcos::property clip_preferences_prop_components_source_;
	pcos::property clip_preferences_prop_components_output_;
	pcos::property clip_preferences_prop_depth_source_;
	pcos::property clip_preferences_prop_depth_output_;
	pcos::property clip_preferences_prop_par_source_;
	pcos::property clip_preferences_prop_par_output_;
	pcos::property clip_preferences_prop_frame_rate_;
	pcos::property clip_preferences_prop_field_order_;
	pcos::property clip_preferences_prop_premultiplication_;
	pcos::property clip_preferences_prop_continuous_samples_;
	pcos::property clip_preferences_prop_frame_varying_;
	bool apply_clip_prefs_;
};

namespace
{
	template<typename T, typename U>
	OfxStatus set_single_property( OfxPropertySetHandle properties, const char* property, int index, T value )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;
		
		pcos::property dst = prop_set->get_property_with_string( property );
		if( dst.is_a<U>( ) )
		{
			U val = dst.value<U>( );
			if( index >= static_cast<int>( val.size( ) ) )
				val.resize( index + 1 );
			
			val[ index ] = value;
			dst = val;
		}
		else
		{
			dst = value;
		}

		return kOfxStatOK;
	}

	template<typename T, typename U>
	OfxStatus set_multiple_property( OfxPropertySetHandle properties, const char* property, int count, T* value )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;
		
		pcos::property dst = prop_set->get_property_with_string( property );
		if( dst.is_a<U>( ) )
		{
			U val = dst.value<U>( );
			if( count >= static_cast<int>( val.size( ) ) )
				val.resize( count + 1 );
			
			for( int i = 0; i < count; ++i )
				val[ i ] = T( value[ i ] );
			dst = val;
		}

		return kOfxStatOK;
	}

	template<typename T, typename U>
	OfxStatus get_single_property( OfxPropertySetHandle properties, const char* property, int index, T* value )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;
		
		pcos::property src = prop_set->get_property_with_string( property );
		if( src.is_a<U>( ) )
		{
			U val = src.value<U>( );
			*value = val[ index ];
		}
		else
		{
			*value = src.value<T>( );
		}

		return kOfxStatOK;
	}

	template<typename T, typename U>
	OfxStatus get_multiple_property( OfxPropertySetHandle properties, const char* property, int count, T* value )
	{
		property_container_ptr prop_set = ( property_container_ptr ) properties;
		
		pcos::property src = prop_set->get_property_with_string( property );
		if( src.is_a<U>( ) )
		{
			U val = src.value<U>( );
			memcpy( value, &val[ 0 ], count * sizeof( T ) );
		}

		return kOfxStatOK;
	}
	
	// Property Suite.
	OfxStatus prop_set_pointer( OfxPropertySetHandle properties, const char* property, int index, void* value )
	{
		return set_single_property<void*, pcos::void_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_set_string( OfxPropertySetHandle properties, const char* property, int index, const char* value )
	{
		return set_single_property<pl::string, pl::string_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_set_double( OfxPropertySetHandle properties, const char* property, int index, double value )
	{
		return set_single_property<double, pcos::double_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_set_int( OfxPropertySetHandle properties, const char* property, int index, int value )
	{
		return set_single_property<int, pcos::int_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_set_stringN( OfxPropertySetHandle properties, const char* property, int count, const char** value )
	{
		return set_multiple_property<const char*, pl::string_vec>( properties, property, count, value );
	}
	
	OfxStatus prop_set_doubleN( OfxPropertySetHandle properties, const char* property, int count, double* value )
	{
		return set_multiple_property<double, pcos::double_vec>( properties, property, count, value );
	}
	
	OfxStatus prop_set_intN( OfxPropertySetHandle properties, const char* property, int count, int* value )
	{
		return set_multiple_property<int, pcos::int_vec>( properties, property, count, value );
	}

	OfxStatus prop_get_pointer( OfxPropertySetHandle properties, const char* property, int index, void** value )
	{
		return get_single_property<void*, pcos::void_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_get_string( OfxPropertySetHandle properties, const char* property, int index, char** value )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;

		pcos::property src = prop_set->get_property_with_string( property );
		if( src.is_a<pl::string_vec>( ) )
		{
			pl::string_vec* val = src.pointer<pl::string_vec>( );
			*value = const_cast<char*>( ( *val )[ index ].c_str( ) );
		}
		else
		{
			*value = const_cast<char*>( src.pointer<opl::string>( )->c_str( ) );
		}

		return kOfxStatOK;
	}
	
	OfxStatus prop_get_double( OfxPropertySetHandle properties, const char* property, int index, double* value )
	{
		return get_single_property<double, pcos::double_vec>( properties, property, index, value );
	}

	OfxStatus prop_get_int( OfxPropertySetHandle properties, const char* property, int index, int* value )
	{
		return get_single_property<int, pcos::int_vec>( properties, property, index, value );
	}
	
	OfxStatus prop_get_stringN( OfxPropertySetHandle properties, const char* property, int count, char** value )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;
		
		pcos::property src = prop_set->get_property_with_string( property );
		if( src.is_a<pl::string_vec>( ) )
		{
			pl::string_vec* val = src.pointer<pl::string_vec>( );
			for( int i = 0; i < count; ++i )
				value[ i ] = const_cast<char*>( ( *val )[ i ].c_str( ) );
		}

		return kOfxStatOK;
	}
	
	OfxStatus prop_get_doubleN( OfxPropertySetHandle properties, const char* property, int count, double* value )
	{
		return get_multiple_property<double, pcos::double_vec>( properties, property, count, value );
	}
	
	OfxStatus prop_get_intN( OfxPropertySetHandle properties, const char* property, int count, int* value )
	{
		return get_multiple_property<int, pcos::int_vec>( properties, property, count, value );
	}
	
	OfxStatus prop_get_dimension( OfxPropertySetHandle properties, const char* property, int* count )
	{
		pcos::property_container* prop_set = ( pcos::property_container* ) properties;
		
		pcos::property src = prop_set->get_property_with_string( property );
		if( src.is_a<pl::string_vec>( ) )
		{
			*count = static_cast<int>( src.value<pl::string_vec>( ).size( ) );
		}
		else if( src.is_a<pcos::int_vec>( ) )
		{
			*count = static_cast<int>( src.value<pcos::int_vec>( ).size( ) );
		}
		else if( src.is_a<pcos::double_vec>( ) )
		{
			*count = static_cast<int>( src.value<pcos::double_vec>( ).size( ) );
		}
		else if( src.is_a<pcos::void_vec>( ) )
		{
			*count = static_cast<int>( src.value<pcos::void_vec>( ).size( ) );
		}
		else
		{
			*count = 1;
		}
		
		return kOfxStatOK;
	}
	
	OfxStatus prop_reset( OfxPropertySetHandle properties, const char* property )
	{
		return kOfxStatOK;
	}
	
	// Memory Suite.
	OfxStatus memory_alloc( void* handle, size_t nbytes, void** allocated_data )
	{
		// NOTE: doesn't really matter if it fails or not. Just being pedantic here.
		char* ptr = new ( std::nothrow ) char [ nbytes ];
		if( !ptr )
			return kOfxStatErrMemory;
			
		*allocated_data = ptr;
		
		return kOfxStatOK;
	}
	
	OfxStatus memory_free( void* allocated_data )
	{
		delete[ ] ( char* ) allocated_data;
		
		return kOfxStatOK;
	}
	
	// Multithread Suite.
	OfxStatus multithread( OfxThreadFunctionV1 func, unsigned int nthreads, void* custom_arg )
	{
		return kOfxStatOK;
	}
	
	OfxStatus multithread_num_cpus( unsigned int* n_cpus )
	{
		// NOTE: Report one CPU for now. Care needs to be taken when dealing with Intel/AMD 32- or 64-bit archs.
#ifdef WIN32
		*n_cpus = 1;
#else
		*n_cpus = 1;
#endif
		return kOfxStatOK;
	}
	
	OfxStatus multithread_index( unsigned int* thread_index )
	{
		*thread_index = 0;

		return kOfxStatOK;
	}
	
	int multithread_is_spawned_thread( )
	{
		return 1;
	}
	
	OfxStatus mutex_create( const OfxMutexHandle* mutex, int lock_count )
	{
		return kOfxStatOK;
	}
	
	OfxStatus mutex_destroy( const OfxMutexHandle mutex )
	{
		return kOfxStatOK;
	}
	
	OfxStatus mutex_lock( const OfxMutexHandle mutex )
	{
		return kOfxStatOK;
	}
	
	OfxStatus mutex_unlock( const OfxMutexHandle mutex )
	{
		return kOfxStatOK;
	}

	OfxStatus mutex_trylock( const OfxMutexHandle mutex )
	{
		return kOfxStatOK;
	}
	
	// Interactive Suite.
	OfxStatus interact_swap_buffers( OfxInteractHandle )
	{
		return kOfxStatOK;
	}
	
	OfxStatus interact_redraw( OfxInteractHandle )
	{
		return kOfxStatOK;
	}
	
	OfxStatus interact_get_property_set( OfxInteractHandle interact_inst, OfxPropertySetHandle* property_set )
	{
		interact_instance* instance = ( interact_instance* ) interact_inst;
		
		*property_set = ( OfxPropertySetHandle ) instance->get_property_set( );
		
		return kOfxStatOK;
	}
	
	// Parameter Suite.
	OfxStatus param_define( OfxParamSetHandle param_set, const char* param_type, const char* name, OfxPropertySetHandle* property_set )
	{
		param_set_descriptor* param_set_desc = ( param_set_descriptor* ) param_set;
		
		OfxStatus status = param_set_desc->define( param_type, name );
		if(  status == kOfxStatOK && property_set )
			*property_set = ( OfxPropertySetHandle ) param_set_desc->find_property_set( name );
		
		return status;
	}
	
	OfxStatus param_get_handle( OfxParamSetHandle p_set, const char* name, OfxParamHandle* par, OfxPropertySetHandle* property_set )
	{
		param_set* par_set = ( param_set* ) p_set;
		
		param* p = par_set->find_param( name );
		if( !p )
			return kOfxStatErrBadHandle;

		if( par )
			*par = ( OfxParamHandle ) p;

		if( property_set )
			*property_set = ( OfxPropertySetHandle ) p->get_property_set( );

		return kOfxStatOK;
	}
	
	OfxStatus param_set_get_property_set( OfxParamSetHandle p_set, OfxPropertySetHandle* property_set )
	{
		param_set* par_set = ( param_set* ) p_set;

		if( property_set )
			*property_set = ( OfxPropertySetHandle ) par_set->get_property_set( );
		
		return kOfxStatOK;
	}
	
	OfxStatus param_get_property_set( OfxParamHandle par, OfxPropertySetHandle* property_set )
	{
		param* p = ( param* ) par;
		
		if( property_set )
			*property_set = ( OfxPropertySetHandle ) p->get_property_set( );
		
		return kOfxStatOK;
	}

	OfxStatus param_get_value( OfxParamHandle par, ... )
	{
		param* p = ( param* ) par;
		
		va_list ap;
		va_start( ap, par );
		
		pcos::property	val  = p->get_value( );
		pl::string		type = p->get_type( );
		
		if( type == kOfxParamTypeInteger || type == kOfxParamTypeBoolean || type == kOfxParamTypeChoice )
		{
			int* v;
			if( ( v = va_arg( ap, int* ) ) != 0 )
				*v = val.value<int>( );
		}
		else if( type == kOfxParamTypeInteger2D || type == kOfxParamTypeInteger3D )
		{
			int* v, i = 0;
			while( ( v = va_arg( ap, int* ) ) != 0 )
				*v = val.value<pcos::int_vec>( )[ i++ ];
		}
		else if( type == kOfxParamTypeDouble )
		{
			double* v;
			if( ( v = va_arg( ap, double* ) ) != 0 )
				*v = val.value<double>( );
		}
		else if( type == kOfxParamTypeDouble2D || type == kOfxParamTypeDouble3D || type == kOfxParamTypeRGB || type == kOfxParamTypeRGBA )
		{
			double* v, i = 0;
			while( ( v = va_arg( ap, double* ) ) != 0 )
				*v = val.value<pcos::double_vec>( )[ i++ ];
		}
		else if( type == kOfxParamTypeGroup )
		{
		}
		else if( type == kOfxParamTypePage )
		{
		}
		else if( type == kOfxParamTypeCustom )
		{
		}
		else if( type == kOfxParamTypePushButton )
		{
		}
		else if( type == kOfxParamTypeString )
		{
		}
		
		va_end( ap );
		
		return kOfxStatOK;
	}
	
	OfxStatus param_get_value_at_time( OfxParamHandle par, OfxTime time, ... )
	{
		param* p = ( param* ) par;
		
		va_list ap;
		va_start( ap, time );
		
		pcos::property	val  = p->get_value( );
		pl::string		type = p->get_type( );
		
		if( type == kOfxParamTypeInteger || type == kOfxParamTypeBoolean || type == kOfxParamTypeChoice )
		{
			int* v;
			if( ( v = va_arg( ap, int* ) ) != 0 )
				*v = val.value<int>( );
		}
		else if( type == kOfxParamTypeInteger2D || type == kOfxParamTypeInteger3D )
		{
			int* v, i = 0;
			while( ( v = va_arg( ap, int* ) ) != 0 )
				*v = val.value<pcos::int_vec>( )[ i++ ];
		}
		else if( type == kOfxParamTypeDouble )
		{
			double* v;
			if( ( v = va_arg( ap, double* ) ) != 0 )
				*v = val.value<double>( );
		}
		else if( type == kOfxParamTypeDouble2D || type == kOfxParamTypeDouble3D || type == kOfxParamTypeRGB || type == kOfxParamTypeRGBA )
		{
			double* v, i = 0;
			while( ( v = va_arg( ap, double* ) ) != 0 )
				*v = val.value<pcos::double_vec>( )[ i++ ];
		}
		else if( type == kOfxParamTypeGroup )
		{
		}
		else if( type == kOfxParamTypePage )
		{
		}
		else if( type == kOfxParamTypeCustom )
		{
		}
		else if( type == kOfxParamTypePushButton )
		{
		}
		else if( type == kOfxParamTypeString )
		{
		}
		
		va_end( ap );

		return kOfxStatOK;
	}
	
	OfxStatus param_get_derivative( OfxParamHandle param, OfxTime time, ... )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_get_integral( OfxParamHandle param, OfxTime t1, OfxTime t2, ... )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_set_value( OfxParamHandle par, ... )
	{
		param* p = ( param* ) par;
		
		va_list ap;
		va_start( ap, par );
		
		pcos::property	val  = p->get_value( );
		pl::string		type = p->get_type( );
		
		if( type == kOfxParamTypeInteger || type == kOfxParamTypeBoolean || type == kOfxParamTypeChoice )
		{
			int v;
			if( ( v = va_arg( ap, int ) ) != 0 )
				val = v;
		}
		else if( type == kOfxParamTypeInteger2D )
		{
		}
		else if( type == kOfxParamTypeInteger3D )
		{
		}
		else if( type == kOfxParamTypeDouble )
		{
			double v;
			if( ( v = va_arg( ap, double ) ) != 0 )
				val = v;
		}
		else if( type == kOfxParamTypeDouble2D )
		{
		}
		else if( type == kOfxParamTypeDouble3D )
		{
		}
		else if( type == kOfxParamTypeRGB )
		{
		}
		else if( type == kOfxParamTypeRGBA )
		{
		}
		else if( type == kOfxParamTypeGroup )
		{
		}
		else if( type == kOfxParamTypePage )
		{
		}
		else if( type == kOfxParamTypeCustom )
		{
		}
		else if( type == kOfxParamTypePushButton )
		{
		}
		else if( type == kOfxParamTypeString )
		{
		}
		
		va_end( ap );

		return kOfxStatOK;
	}
	
	OfxStatus param_set_value_at_time( OfxParamHandle par, OfxTime time, ... )
	{
		param* p = ( param* ) par;
		
		va_list ap;
		va_start( ap, time );
		
		pcos::property	val  = p->get_value( );
		pl::string		type = p->get_type( );
		
		if( type == kOfxParamTypeInteger )
		{
			int v;
			if( ( v = va_arg( ap, int ) ) != 0 )
				val = v;
		}
		else if( type == kOfxParamTypeInteger2D )
		{
		}
		else if( type == kOfxParamTypeInteger3D )
		{
		}
		else if( type == kOfxParamTypeDouble )
		{
			double v;
			if( ( v = va_arg( ap, double ) ) != 0 )
				val = v;
		}
		else if( type == kOfxParamTypeDouble2D )
		{
		}
		else if( type == kOfxParamTypeDouble3D )
		{
		}
		else if( type == kOfxParamTypeRGB )
		{
		}
		else if( type == kOfxParamTypeRGBA )
		{
		}
		else if( type == kOfxParamTypeBoolean )
		{
		}
		else if( type == kOfxParamTypeChoice )
		{
		}
		else if( type == kOfxParamTypeGroup )
		{
		}
		else if( type == kOfxParamTypePage )
		{
		}
		else if( type == kOfxParamTypeCustom )
		{
		}
		else if( type == kOfxParamTypePushButton )
		{
		}
		else if( type == kOfxParamTypeString )
		{
		}
		
		va_end( ap );

		return kOfxStatOK;
	}
	
	OfxStatus param_get_num_keys( OfxParamHandle, unsigned int* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_get_key_time( OfxParamHandle, unsigned int, OfxTime* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_get_key_index( OfxParamHandle, OfxTime, int, int* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_delete_key( OfxParamHandle, OfxTime )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_delete_all_keys( OfxParamHandle )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_copy( OfxParamHandle, OfxParamHandle, OfxTime, OfxRangeD* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_edit_begin( OfxParamSetHandle, const char* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus param_edit_end( OfxParamSetHandle )
	{
		return kOfxStatOK;
	}
	
	// Image Effect Suite.
	OfxStatus get_property_set( OfxImageEffectHandle image_effect, OfxPropertySetHandle* prop_handle )
	{
		Ofx_base* plug = ( Ofx_base* ) image_effect;
		*prop_handle = ( OfxPropertySetHandle ) plug->get_property_set( );
		return kOfxStatOK;
	}
	
	OfxStatus get_param_set( OfxImageEffectHandle image_effect, OfxParamSetHandle* param_set )
	{
		param_set_base* pset = ( param_set_base* ) image_effect;
		if( !pset )
			return kOfxStatErrBadHandle;
		
		if( param_set )
			*param_set = pset->get_param_set( );
		
		return kOfxStatOK;
	}
	
	OfxStatus clip_define( OfxImageEffectHandle image_effect, const char* name, OfxPropertySetHandle* property_set )
	{
		image_effect_descriptor* desc = ( image_effect_descriptor* ) image_effect;
		
		clip_descriptor* clip_desc = desc->clip_get_handle( name );
		if( !clip_desc )
			clip_desc = desc->append_clip( name );
			
		if( property_set )
			*property_set = ( OfxPropertySetHandle ) clip_desc->get_property_set( );

		return kOfxStatOK;
	}

	OfxStatus clip_get_handle( OfxImageEffectHandle image_effect, const char* name, OfxImageClipHandle* handle, OfxPropertySetHandle* property_set )
	{
		image_effect_instance* instance = ( image_effect_instance* ) image_effect;

		clip_instance* clip = instance->clip_get_handle( name );
		if( clip )
		{
			if( handle )
				*handle = ( OfxImageClipHandle ) clip;
		
			if( property_set )
				*property_set = ( OfxPropertySetHandle ) clip->get_property_set( );
		}

		return kOfxStatOK;
	}

	OfxStatus clip_get_property_set( OfxImageClipHandle clip, OfxPropertySetHandle* handle )
	{
		clip_instance* instance = ( clip_instance* ) clip;
		*handle = ( OfxPropertySetHandle ) instance->get_property_set( );
		return kOfxStatOK;
	}
	
	OfxStatus clip_get_image( OfxImageClipHandle clip, OfxTime time, OfxRectD* region, OfxPropertySetHandle* image_handle )
	{
		clip_instance* instance = ( clip_instance* ) clip;
		if( !instance )
			return kOfxStatErrBadHandle;

		*image_handle = ( OfxPropertySetHandle ) instance->get_image_property_set( );

		return kOfxStatOK;
	}
	
	OfxStatus clip_release_image( OfxPropertySetHandle )
	{
		return kOfxStatOK;
	}

	OfxStatus clip_get_region_of_definition( OfxImageClipHandle clip, OfxTime time, OfxRectD* bounds )
	{
		clip_instance* instance = ( clip_instance* ) clip;
		if( !instance )
			return kOfxStatErrBadHandle;

		property_container_ptr image = instance->get_image_property_set( );
		pcos::int_vec b = image->get_property_with_string( kOfxImagePropBounds ).value<pcos::int_vec>( );

		bounds->x1 = b[ 0 ];
		bounds->y1 = b[ 1 ];
		bounds->x2 = b[ 2 ];
		bounds->y2 = b[ 3 ];

		return kOfxStatOK;
	}
	
	int abort( OfxImageEffectHandle )
	{
		return 0;
	}
	
	OfxStatus image_memory_alloc( OfxImageEffectHandle, size_t, OfxImageMemoryHandle* )
	{
		return kOfxStatOK;
	}
	
	OfxStatus image_memory_free( OfxImageMemoryHandle )
	{
		return kOfxStatOK;
	}
	
	OfxStatus image_memory_lock( OfxImageMemoryHandle, void** )
	{
		return kOfxStatOK;
	}
	
	OfxStatus image_memory_unlock( OfxImageMemoryHandle )
	{
		return kOfxStatOK;
	}
	
	// Message Suite
	OfxStatus message( void*, const char*, const char*, const char*, ... )
	{
		return kOfxStatOK;
	}

	void set_property_suite( OfxPropertySuiteV1& property_suite )
	{
		property_suite.propSetPointer = prop_set_pointer;
		property_suite.propSetString = prop_set_string;
		property_suite.propSetDouble = prop_set_double;
		property_suite.propSetInt = prop_set_int;
		property_suite.propSetStringN = prop_set_stringN;
		property_suite.propSetDoubleN = prop_set_doubleN;
		property_suite.propSetIntN = prop_set_intN;
		property_suite.propGetPointer = prop_get_pointer;
		property_suite.propGetString = prop_get_string;
		property_suite.propGetDouble = prop_get_double;
		property_suite.propGetInt = prop_get_int;
		property_suite.propGetStringN = prop_get_stringN;
		property_suite.propGetDoubleN = prop_get_doubleN;
		property_suite.propGetIntN = prop_get_intN;
		property_suite.propReset = prop_reset;
		property_suite.propGetDimension = prop_get_dimension;
	}
	
	void set_image_effect_suite( OfxImageEffectSuiteV1& image_effect_suite )
	{
		image_effect_suite.getPropertySet = get_property_set;
		image_effect_suite.getParamSet = get_param_set;
		image_effect_suite.clipDefine = clip_define;
		image_effect_suite.clipGetHandle = clip_get_handle;
		image_effect_suite.clipGetPropertySet = clip_get_property_set;
		image_effect_suite.clipGetImage = clip_get_image;
		image_effect_suite.clipReleaseImage = clip_release_image;
		image_effect_suite.clipGetRegionOfDefinition = clip_get_region_of_definition;
		image_effect_suite.abort = abort;
		image_effect_suite.imageMemoryAlloc = image_memory_alloc;
		image_effect_suite.imageMemoryFree = image_memory_free;
		image_effect_suite.imageMemoryLock = image_memory_lock;
		image_effect_suite.imageMemoryUnlock = image_memory_unlock;
	}
	
	void set_parameter_suite( OfxParameterSuiteV1& parameter_suite )
	{
		parameter_suite.paramDefine = param_define;
		parameter_suite.paramGetHandle = param_get_handle;
		parameter_suite.paramSetGetPropertySet = param_set_get_property_set;
		parameter_suite.paramGetPropertySet = param_get_property_set;
		parameter_suite.paramGetValue = param_get_value;
		parameter_suite.paramGetValueAtTime = param_get_value_at_time;
		parameter_suite.paramGetDerivative = param_get_derivative;
		parameter_suite.paramGetIntegral = param_get_integral;
		parameter_suite.paramSetValue = param_set_value;
		parameter_suite.paramSetValueAtTime = param_set_value_at_time;
		parameter_suite.paramGetNumKeys = param_get_num_keys;
		parameter_suite.paramGetKeyTime = param_get_key_time;
		parameter_suite.paramGetKeyIndex = param_get_key_index;
		parameter_suite.paramDeleteKey = param_delete_key;
		parameter_suite.paramDeleteAllKeys = param_delete_all_keys;
		parameter_suite.paramCopy = param_copy;
		parameter_suite.paramEditBegin = param_edit_begin;
		parameter_suite.paramEditEnd = param_edit_end;
	}
	
	void set_memory_suite( OfxMemorySuiteV1& memory_suite )
	{
		memory_suite.memoryAlloc = memory_alloc;
		memory_suite.memoryFree = memory_free;
	}
	
	void set_multithread_suite( OfxMultiThreadSuiteV1& multithread_suite )
	{
		multithread_suite.multiThread = multithread;
		multithread_suite.multiThreadNumCPUs = multithread_num_cpus;
		multithread_suite.multiThreadIndex = multithread_index;
		multithread_suite.multiThreadIsSpawnedThread = multithread_is_spawned_thread;
		multithread_suite.mutexCreate = mutex_create;
		multithread_suite.mutexDestroy = mutex_destroy;
		multithread_suite.mutexLock = mutex_lock;
		multithread_suite.mutexUnLock = mutex_unlock;
		multithread_suite.mutexTryLock = mutex_trylock;
	}

	void set_interact_suite( OfxInteractSuiteV1& interact_suite )
	{
		interact_suite.interactSwapBuffers = interact_swap_buffers;
		interact_suite.interactRedraw = interact_redraw;
		interact_suite.interactGetPropertySet = interact_get_property_set;
	}
	
	void set_message_suite( OfxMessageSuiteV1& message_suite )
	{
		message_suite.message = message;
	}
	
	// Query structure.
	struct ofx_query_traits : public pl::default_query_traits
	{
		ofx_query_traits( const pl::wstring& request )
			: request_( request )
		{ }
		
		pl::wstring to_match( ) const
		{ return request_; }
		
		pl::wstring libname( ) const
		{ return pl::wstring( L"openmedialib" ); }
		
		pl::wstring type( ) const
		{ return pl::wstring( L"ofx" ); }
	
		const pl::wstring request_;
	};
}

class ML_PLUGIN_DECLSPEC ofx_plugin : public openmedialib_plugin
{
public:
	virtual input_type_ptr  input(  const pl::wstring& request ) { return discover( request, GENERATOR ); }
	virtual filter_type_ptr filter( const pl::wstring& request ) { return discover( request, FILTER ); }

private:
	filter_type_ptr discover( const pl::wstring& request, ofx_context context )
	{
		typedef pl::discovery<ofx_query_traits> discovery;
		ofx_query_traits query( request );
		
		discovery plugins( query );
		if( plugins.size( ) == 0 ) return filter_type_ptr( );

		discovery::const_iterator i = plugins.begin( );

		return filter_type_ptr( new ofx_filter( ( OfxPlugin* ) i->context( ), context ) );
	}
};

} } }

extern "C"
{
	ML_PLUGIN_DECLSPEC bool openplugin_init( void )
	{
		return true;
	}

	ML_PLUGIN_DECLSPEC bool openplugin_uninit( void )
	{
		return true;
	}
	
	ML_PLUGIN_DECLSPEC bool openplugin_create_plugin( const char* options, pl::openplugin** plug )
	{
		*plug = new ml::ofx_plugin;
		
		return true;
	}

	ML_PLUGIN_DECLSPEC void openplugin_destroy_plugin( pl::openplugin* plug )
	{ 
		delete static_cast<ml::ofx_plugin*>( plug ); 
	}
}

#endif
