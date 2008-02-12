
// sg - A scene graph representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifdef HAVE_CONFIG_H
#include <openlibraries_global_config.hpp>
#endif

#ifdef WIN32
#import <msxml4.dll> raw_interfaces_only
using namespace MSXML2;
#endif // WIN32

#if !defined( GCC_VERSION ) && defined( __GNUC__ )
#define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#endif

#define _USE_MATH_DEFINES

#include <math.h>
#include <cassert>

#ifdef _MSC_VER
#pragma warning ( push )
#   pragma warning ( disable: 4251 4512 )
#endif

#include <openobjectlib/sg/spatial.hpp>

#include <openobjectlib/plugins/Collada/actions/dae_parser_action.hpp>
#include <openobjectlib/plugins/Collada/xml_value_tokenizer.hpp>

#ifdef _MSC_VER
#   pragma warning ( default: 4251 4512 )
#pragma warning ( pop )
#endif

namespace olib { namespace openobjectlib { namespace sg { namespace actions {

//
template<class T, bool shader = false>
struct assign_field
{ };

template<class T>
void tokenize_arr(std::vector<T> &values, opl::wstring& val_str)
{
    boost::char_separator<wchar_t> sep( L" \n" );
    wtokenizer                     wtok( val_str.begin( ), val_str.end( ), sep );

    for( wtokenizer::const_iterator I = wtok.begin( ); I != wtok.end( ); ++I )
        values.push_back( boost::lexical_cast<T>( I->c_str( ) ) );
}

template<class T>
struct assign_field<opl::value_property<T> >
{
    typedef opl::value_property<T> field_type;

    void assign( const dae_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
    {
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
        field_type* field = node->pointer<field_type>( name );

        std::vector<float> value;

        if( xml_value_tokenizer( ).tokenize<float>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
        {
            for( size_t i = 0; i < value.size( ); ++i )
                field->get( )[ i ] = value[ i ];
        }
#endif
    }

    void assign( const sg::node_ptr& node, const opl::wstring& name, opl::wstring& val_str )
    {
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
        field_type* field = node->pointer<field_type>( name );

        std::vector<float> values;

        tokenize_arr<float>(values, val_str);

        for( size_t i = 0; i < values.size( ); ++i )
            field->get( )[ i ] = values[ i ];
#endif
    }
};

template<class T>
struct assign_field<opl::value_property<T>, true>
{
    typedef opl::value_property<T> field_type;

    template<typename VEC_TYPE>
    void assign( const sg::shader_ptr& node, const opl::wstring& name, opl::wstring& val_str )
    {
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
        field_type* field = node->pointer_param<field_type>( name );

        std::vector<VEC_TYPE> values;
        tokenize_arr<VEC_TYPE>(values, val_str);

        for( size_t i = 0; i < values.size( ); ++i )
            field->get( )[ i ] = values[ i ];
#endif
    }
};

template<>
struct assign_field<opl::v_float>
{
    void assign( const sg::node_ptr& node, const opl::wstring& name, opl::wstring& val_str )
    {
        opl::v_float     * field = node->pointer<opl::v_float>( name );
        std::vector<float> values;

        tokenize_arr<float>(values, val_str);

        if ( values.empty( ) )
            return;

        field->get( ) = values[ 0 ];
    }
};

template<>
struct assign_field<opl::v_int32, true>
{
    template<typename VEC_TYPE>
    void assign( const sg::shader_ptr& node, const opl::wstring& name, opl::wstring& val_str )
    {
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
        opl::v_int32* field = node->pointer_param<opl::v_int32>( name );

        std::vector<VEC_TYPE> values;
        tokenize_arr<VEC_TYPE>(values, val_str);

        field->get( ) = values[ 0 ];
#endif
    }
};

template<>
struct assign_field<opl::v_float, true>
{
    template<typename VEC_TYPE>
    void assign( const sg::shader_ptr& node, const opl::wstring& name, opl::wstring& val_str )
    {
#if _MSC_VER >= 1310 || GCC_VERSION >= 30400
        opl::v_float* field = node->pointer_param<opl::v_float>( name );

        std::vector<VEC_TYPE> values;
        tokenize_arr<VEC_TYPE>(values, val_str);

        field->get( ) = values[ 0 ];
#endif
    }
};

template<>
struct assign_field<opl::v_string>
{
    void assign( const dae_parser_action& pa, const sg::node_ptr& node, const opl::wstring& name, const opl::wstring& xml_name = L"" )
    {
        opl::v_string* field = node->pointer<opl::v_string>( name );

        std::vector<opl::wstring> value;
        if( xml_value_tokenizer( ).tokenize<opl::wstring>( xml_name.empty( ) ? name : xml_name, pa.attributes_, value ) )
        {
            if( value.empty( ) )
                return;

            field->get( ).assign( value[ 0 ] );
        }
    }
};

//
//****************************************************************************
//              --- dae_parser_action::dae_parser_action ---
//
//!\brief   dae_parser_action object constructor
//!\author  Pavel A. Chuvanov 05.05.2006
//****************************************************************************
dae_parser_action::dae_parser_action()
                  : scene_( new sg::scene( ) )
                  , is_viewpnt_set_(false)
{
    dispatch_.insert( container::value_type( L"accessor",                E_DAE_NODE_ACCESSOR ) );
    dispatch_.insert( container::value_type( L"ambient",                 E_DAE_NODE_AMBIENT ) );
    dispatch_.insert( container::value_type( L"animation",               E_DAE_NODE_ANIMATION ) );
    dispatch_.insert( container::value_type( L"animation_clip",          E_DAE_NODE_ANIMATION_CLIP ) );
    dispatch_.insert( container::value_type( L"asset",                   E_DAE_NODE_ASSET ) );
    dispatch_.insert( container::value_type( L"aspect_ratio",            E_DAE_NODE_ASPECT_RATIO ) );
    dispatch_.insert( container::value_type( L"bool_array",              E_DAE_NODE_BOOL_ARRAY ) );
    dispatch_.insert( container::value_type( L"camera",                  E_DAE_NODE_CAMERA ) );
    dispatch_.insert( container::value_type( L"channel",                 E_DAE_NODE_CHANNEL ) );
    dispatch_.insert( container::value_type( L"code",                    E_DAE_NODE_CODE ) );
    dispatch_.insert( container::value_type( L"COLLADA",                 E_DAE_NODE_COLLADA ) );
    dispatch_.insert( container::value_type( L"color",                   E_DAE_NODE_COLOR ) );
    dispatch_.insert( container::value_type( L"constant_attenuation",    E_DAE_NODE_CONSTANT_ATTENUATION ) );
    dispatch_.insert( container::value_type( L"contributor",             E_DAE_NODE_CONTRIBUTOR ) );
    dispatch_.insert( container::value_type( L"controller",              E_DAE_NODE_CONTROLLER ) );
    dispatch_.insert( container::value_type( L"diffuse",                 E_DAE_NODE_DIFFUSE ) );
    dispatch_.insert( container::value_type( L"directional",             E_DAE_NODE_DIRECTIONAL ) );
    dispatch_.insert( container::value_type( L"effect",                  E_DAE_NODE_EFFECT ) );
    dispatch_.insert( container::value_type( L"emission",                E_DAE_NODE_EMISSION ) );
    dispatch_.insert( container::value_type( L"extra",                   E_DAE_NODE_EXTRA ) );
    dispatch_.insert( container::value_type( L"float",                   E_DAE_NODE_FLOAT ) );
    dispatch_.insert( container::value_type( L"float2",                  E_DAE_NODE_FLOAT2 ) );
    dispatch_.insert( container::value_type( L"float3",                  E_DAE_NODE_FLOAT3 ) );
    dispatch_.insert( container::value_type( L"float4",                  E_DAE_NODE_FLOAT4 ) );
    dispatch_.insert( container::value_type( L"float2x2",                E_DAE_NODE_FLOAT2X2 ) );
    dispatch_.insert( container::value_type( L"float3x3",                E_DAE_NODE_FLOAT3X3 ) );
    dispatch_.insert( container::value_type( L"float4x4",                E_DAE_NODE_FLOAT4X4 ) );
    dispatch_.insert( container::value_type( L"float_array",             E_DAE_NODE_FLOAT_ARRAY ) );
    dispatch_.insert( container::value_type( L"geometry",                E_DAE_NODE_GEOMETRY ) );
    dispatch_.insert( container::value_type( L"IDREF_array",             E_DAE_NODE_IDREF_ARRAY ) );
    dispatch_.insert( container::value_type( L"image",                   E_DAE_NODE_IMAGE ) );
    dispatch_.insert( container::value_type( L"imager",                  E_DAE_NODE_IMAGER ) );
    dispatch_.insert( container::value_type( L"init_from",               E_DAE_NODE_INIT_FROM ) );
    dispatch_.insert( container::value_type( L"input",                   E_DAE_NODE_INPUT ) );
    dispatch_.insert( container::value_type( L"instance_animation",      E_DAE_NODE_INSTANCE_ANIMATION ) );
    dispatch_.insert( container::value_type( L"instance_camera",         E_DAE_NODE_INSTANCE_CAMERA ) );
    dispatch_.insert( container::value_type( L"instance_controller",     E_DAE_NODE_INSTANCE_CONTROLLER ) );
    dispatch_.insert( container::value_type( L"instance_effect",         E_DAE_NODE_INSTANCE_EFFECT ) );
    dispatch_.insert( container::value_type( L"instance_geometry",       E_DAE_NODE_INSTANCE_GEOMETRY ) );
    dispatch_.insert( container::value_type( L"instance_light",          E_DAE_NODE_INSTANCE_LIGHT ) );
    dispatch_.insert( container::value_type( L"instance_node",           E_DAE_NODE_INSTANCE_NODE ) );
    dispatch_.insert( container::value_type( L"instance_visual_scene",   E_DAE_NODE_INSTANCE_VISUAL_SCENE ) );
    dispatch_.insert( container::value_type( L"int",                     E_DAE_NODE_INT ) );
    dispatch_.insert( container::value_type( L"int2",                    E_DAE_NODE_INT2 ) );
    dispatch_.insert( container::value_type( L"int3",                    E_DAE_NODE_INT3 ) );
    dispatch_.insert( container::value_type( L"int4",                    E_DAE_NODE_INT4 ) );
    dispatch_.insert( container::value_type( L"int_array",               E_DAE_NODE_INT_ARRAY ) );
    dispatch_.insert( container::value_type( L"joints",                  E_DAE_NODE_JOINTS ) );
    dispatch_.insert( container::value_type( L"library_animations",      E_DAE_NODE_LIBRARY_ANIMATIONS ) );
    dispatch_.insert( container::value_type( L"library_animation_clips", E_DAE_NODE_LIBRARY_ANIMATION_CLIPS ) );
    dispatch_.insert( container::value_type( L"library_cameras",         E_DAE_NODE_LIBRARY_CAMERAS ) );
    dispatch_.insert( container::value_type( L"library_controllers",     E_DAE_NODE_LIBRARY_CONTROLLERS ) );
    dispatch_.insert( container::value_type( L"library_effects",         E_DAE_NODE_LIBRARY_EFFECTS ) );
    dispatch_.insert( container::value_type( L"library_force_fields",    E_DAE_NODE_LIBRARY_FORCE_FIELDS ) );
    dispatch_.insert( container::value_type( L"library_geometries",      E_DAE_NODE_LIBRARY_GEOMETRIES ) );
    dispatch_.insert( container::value_type( L"library_images",          E_DAE_NODE_LIBRARY_IMAGES ) );
    dispatch_.insert( container::value_type( L"library_lights",          E_DAE_NODE_LIBRARY_LIGHTS ) );
    dispatch_.insert( container::value_type( L"library_materials",       E_DAE_NODE_LIBRARY_MATERIALS ) );
    dispatch_.insert( container::value_type( L"library_nodes",           E_DAE_NODE_LIBRARY_NODES ) );
    dispatch_.insert( container::value_type( L"library_physics_models",  E_DAE_NODE_LIBRARY_PHYSICS_MODELS ) );
    dispatch_.insert( container::value_type( L"library_physics_scenes",  E_DAE_NODE_LIBRARY_PHYSICS_SCENES ) );
    dispatch_.insert( container::value_type( L"library_visual_scenes",   E_DAE_NODE_LIBRARY_VISUAL_SCENES ) );
    dispatch_.insert( container::value_type( L"light",                   E_DAE_NODE_LIGHT ) );
    dispatch_.insert( container::value_type( L"linear_attenuation",      E_DAE_NODE_LINEAR_ATTENUATION ) );
    dispatch_.insert( container::value_type( L"lines",                   E_DAE_NODE_LINES ) );
    dispatch_.insert( container::value_type( L"linestrips",              E_DAE_NODE_LINESTRIPS ) );
    dispatch_.insert( container::value_type( L"lookat",                  E_DAE_NODE_LOOKAT ) );
    dispatch_.insert( container::value_type( L"material",                E_DAE_NODE_MATERIAL ) );
    dispatch_.insert( container::value_type( L"matrix",                  E_DAE_NODE_MATRIX ) );
    dispatch_.insert( container::value_type( L"mesh",                    E_DAE_NODE_MESH ) );
    dispatch_.insert( container::value_type( L"morph",                   E_DAE_NODE_MORPH ) );
    dispatch_.insert( container::value_type( L"Name_array",              E_DAE_NODE_NAME_ARRAY ) );
    dispatch_.insert( container::value_type( L"newparam",                E_DAE_NODE_NEWPARAM ) );
    dispatch_.insert( container::value_type( L"node",                    E_DAE_NODE_NODE ) );
    dispatch_.insert( container::value_type( L"optics",                  E_DAE_NODE_OPTICS ) );
    dispatch_.insert( container::value_type( L"orthographic",            E_DAE_NODE_ORTHOGRAPHIC ) );
    dispatch_.insert( container::value_type( L"p",                       E_DAE_NODE_P     ) );
    dispatch_.insert( container::value_type( L"param",                   E_DAE_NODE_PARAM ) );
    dispatch_.insert( container::value_type( L"perspective",             E_DAE_NODE_PERSPECTIVE ) );
    dispatch_.insert( container::value_type( L"point",                   E_DAE_NODE_POINT ) );
    dispatch_.insert( container::value_type( L"polygons",                E_DAE_NODE_POLYGONS ) );
    dispatch_.insert( container::value_type( L"polylist",                E_DAE_NODE_POLYLIST ) );
    dispatch_.insert( container::value_type( L"profile_GLSL",            E_DAE_NODE_PROFILE_GLSL ) );
    dispatch_.insert( container::value_type( L"quadratic_attenuation",   E_DAE_NODE_QUADRATIC_ATTENUATION ) );
    dispatch_.insert( container::value_type( L"rotate",                  E_DAE_NODE_ROTATE ) );
    dispatch_.insert( container::value_type( L"sampler",                 E_DAE_NODE_SAMPLER ) );
    dispatch_.insert( container::value_type( L"scale",                   E_DAE_NODE_SCALE ) );
    dispatch_.insert( container::value_type( L"scene",                   E_DAE_NODE_SCENE ) );
    dispatch_.insert( container::value_type( L"shininess",               E_DAE_NODE_SHININESS ) );
    dispatch_.insert( container::value_type( L"skeleton",                E_DAE_NODE_SKELETON ) );
    dispatch_.insert( container::value_type( L"skew",                    E_DAE_NODE_SKEW ) );
    dispatch_.insert( container::value_type( L"skin",                    E_DAE_NODE_SKIN ) );
    dispatch_.insert( container::value_type( L"source",                  E_DAE_NODE_SOURCE ) );
    dispatch_.insert( container::value_type( L"specular",                E_DAE_NODE_SPECULAR ) );
    dispatch_.insert( container::value_type( L"spline",                  E_DAE_NODE_SPLINE ) );
    dispatch_.insert( container::value_type( L"spot",                    E_DAE_NODE_SPOT ) );
    dispatch_.insert( container::value_type( L"targets",                 E_DAE_NODE_TARGETS ) );
    dispatch_.insert( container::value_type( L"technique",               E_DAE_NODE_TECHNIQUE ) );
    dispatch_.insert( container::value_type( L"technique_common",        E_DAE_NODE_TECHNIQUE_COMMON ) );
    dispatch_.insert( container::value_type( L"texture",                 E_DAE_NODE_TEXTURE ) );
    dispatch_.insert( container::value_type( L"translate",               E_DAE_NODE_TRANSLATE ) );
    dispatch_.insert( container::value_type( L"transparency",            E_DAE_NODE_TRANSPARENCY ) );
    dispatch_.insert( container::value_type( L"triangles",               E_DAE_NODE_TRIANGLES ) );
    dispatch_.insert( container::value_type( L"trifans",                 E_DAE_NODE_TRIFANS ) );
    dispatch_.insert( container::value_type( L"tristrips",               E_DAE_NODE_TRISTRIPS ) );
    dispatch_.insert( container::value_type( L"vertex_weights",          E_DAE_NODE_VERTEX_WEIGHTS ) );
    dispatch_.insert( container::value_type( L"vertices",                E_DAE_NODE_VERTICES ) );
    dispatch_.insert( container::value_type( L"visual_scene",            E_DAE_NODE_VISUAL_SCENE ) );
    dispatch_.insert( container::value_type( L"yfov",                    E_DAE_NODE_YFOV ) );
    dispatch_.insert( container::value_type( L"zfar",                    E_DAE_NODE_ZFAR ) );
    dispatch_.insert( container::value_type( L"znear",                   E_DAE_NODE_ZNEAR ) );

    sg::spatial_ptr grp( new sg::group( ) );
    scene_->set_root( grp );
}
//
//****************************************************************************
//              --- dae_parser_action::~dae_parser_action ---
//
//!\brief   dae_parser_action object destructor
//!\author  Pavel A. Chuvanov 05.05.2006
//****************************************************************************
dae_parser_action::~dae_parser_action( )
{
}
//
//****************************************************************************
//                   --- dae_parser_action::dispatch ---
//
//!\brief   Callback for process each node
//!\param   node - Node name
//!\retval       - ACTION_RETURN_CONTINUE
//!\author  Pavel A. Chuvanov 10.05.2006
//****************************************************************************
ACTION_RETURN dae_parser_action::dispatch( const key_type& node )
{
    container::const_iterator I = dispatch_.find( node );
    if ( I != dispatch_.end( ) )
    {
        opl_string name;

//        wprintf(L"%s %d\n", I->first.c_str(), I->second);

        switch (I->second)
        {
            case E_DAE_NODE_NEWPARAM:
                get_value_from_name( L"sid", attributes_, name );
                break;

            case E_DAE_NODE_PARAM:
                get_value_from_name( L"name", attributes_, name );
                break;

            case E_DAE_NODE_INPUT:
                get_value_from_name( L"source", attributes_, name );
                name = name.substr(1);      // Skip #
                break;;

            case E_DAE_NODE_POLYGONS:
                get_value_from_name( L"material", attributes_, name );
                break;

            case E_DAE_NODE_TEXTURE:
                get_value_from_name( L"texture", attributes_, name );
                break;

            case E_DAE_NODE_INSTANCE_GEOMETRY:
            case E_DAE_NODE_INSTANCE_CAMERA:
            case E_DAE_NODE_INSTANCE_EFFECT:
                get_value_from_name( L"url", attributes_, name );
                name = name.substr(1);      // Skip #
                break;

            default:
                get_value_from_name(L"id", attributes_, name);
                break;
        }                                   // switch (I->second)

        push(I->second, name);
        node_found();
    }
    else
    {
//        wprintf(L"Unknown %s\n", node.c_str());
        push( E_DAE_NODE_UNKNOWN );
    }

    return ACTION_RETURN_CONTINUE;
}
//
//****************************************************************************
//                  --- dae_parser_action::node_found ---
//
//!\brief   Node callback
//!\author  Pavel A. Chuvanov 22.05.2006
//****************************************************************************
void dae_parser_action::node_found()
{
    switch (node_back_type(0))
    {
        case E_DAE_NODE_TEXTURE:
            if (node_type(1) == E_DAE_NODE_LIBRARY_EFFECTS &&
                node_type(2) == E_DAE_NODE_EFFECT)
            {
                parse_effect(nodes_[2], *node_back(0), NULL, NULL);
            }
            break;

        case E_DAE_NODE_INSTANCE_GEOMETRY:
        case E_DAE_NODE_INSTANCE_CAMERA:
            if (node_back_type(1) == E_DAE_NODE_NODE)
                add_instance(*node_back(0));
            break;

        case E_DAE_NODE_INSTANCE_EFFECT:
            if (node_back_type(1) == E_DAE_NODE_MATERIAL &&
                node_back_type(2) == E_DAE_NODE_LIBRARY_MATERIALS)
            {
                mateff_.insert( map_strstr::value_type( node_back(1)->name_, node_back(0)->name_ ) );
            }
            break;

        case E_DAE_NODE_INPUT:
            if (node_type(5) == E_DAE_NODE_INPUT    &&
                node_type(4) == E_DAE_NODE_VERTICES &&
                node_type(3) == E_DAE_NODE_MESH     &&
                node_type(2) == E_DAE_NODE_GEOMETRY &&
                node_type(1) == E_DAE_NODE_LIBRARY_GEOMETRIES)
            {
                parse_geom_vert(nodes_[5], nodes_[4], nodes_[2]);
            }
            else if (node_type(5) == E_DAE_NODE_INPUT     &&
                     node_type(3) == E_DAE_NODE_MESH      &&
                     node_type(2) == E_DAE_NODE_GEOMETRY  &&
                     node_type(1) == E_DAE_NODE_LIBRARY_GEOMETRIES)
            {
                parse_poly_in(nodes_[5], nodes_[4], nodes_[2]);
            }
            break;
    }                                       // switch (node_back_type(0))
}
//
//****************************************************************************
//                    --- dae_parser_action::start ---
//
//!\brief
//!\param   node -
//!\author  Pavel A. Chuvanov 10.05.2006
//****************************************************************************
void dae_parser_action::start(const sg::node_ptr &node)
{
    while( !nodes_.empty( ) )
        nodes_.pop_back( );
}
//
//****************************************************************************
//                    --- dae_parser_action::finish ---
//
//!\brief
//!\param   node -
//!\author  Pavel A. Chuvanov 10.05.2006
//****************************************************************************
void dae_parser_action::finish( const sg::node_ptr &node)
{
}
//
//****************************************************************************
//                     --- dae_parser_action::push ---
//
//!\brief   Push node to stack
//!\param   node_type - Node type
//!\param   name      - Node name
//!\author  Pavel A. Chuvanov 10.05.2006
//****************************************************************************
void dae_parser_action::push( e_dae_node node_type, const key_type &name )
{
    nodes_.push_back( dae_node(node_type, name) );
}
//
//****************************************************************************
//                     --- dae_parser_action::pop ---
//
//!\brief   Pop node from stack
//!\author  Pavel A. Chuvanov 10.05.2006
//****************************************************************************
void dae_parser_action::pop( )
{
    nodes_.pop_back();
}
//
//****************************************************************************
//                  --- dae_parser_action::node_type ---
//
//!\brief   Get type of node with specified index
//!\param   node_idx - Node index
//!\retval           - Node type or E_DAE_NODE_UNKNOWN if out of range
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
e_dae_node dae_parser_action::node_type( int node_idx )
{
    if ( node_idx >= 0 && node_idx < (int) nodes_.size() )
        return nodes_[node_idx].type_;

    return E_DAE_NODE_UNKNOWN;
}
//
//****************************************************************************
//                --- dae_parser_action::node_back_type ---
//
//!\brief   Get type of node from tail
//!\param   node_back_idx - Node index from tile (0 - current node, 1 - previous, etc.)
//!\retval                - Type of node
//!\author  Pavel A. Chuvanov 21.05.2006
//****************************************************************************
e_dae_node dae_parser_action::node_back_type( int node_back_idx )
{
    if ( node_back_idx >= 0 && node_back_idx < (int) nodes_.size() )
        return nodes_[(int) nodes_.size() - 1 - node_back_idx].type_;

    return E_DAE_NODE_UNKNOWN;
}
//
//****************************************************************************
//                  --- dae_parser_action::node_back ---
//
//!\brief   Get pointer to node from tail
//!\param   node_back_idx - Node index from tile (0 - current node, 1 - previous, etc.)
//!\retval                - Pointer to node
//!\author  Pavel A. Chuvanov 21.05.2006
//****************************************************************************
dae_node *dae_parser_action::node_back( int node_back_idx )
{
    if ( node_back_idx >= 0 && node_back_idx < (int) nodes_.size() )
        return &nodes_[(int) nodes_.size() - 1 - node_back_idx];

    return NULL;
}
//
//****************************************************************************
//              --- dae_parser_action::build_node_matrix ---
//
//!\brief   Build node matrix from nodes stack
//!\param   matrix - Matrix buffer
//!\author  Pavel A. Chuvanov 21.05.2006
//****************************************************************************
void dae_parser_action::build_node_matrix( opl::matrixf & matrix )
{
    matrix = opl::make_identity<float>( );

    int n  = nodes_.size();

    for (int i = 0; i < n; i++)
    {
        if (nodes_[i].type_ == E_DAE_NODE_NODE)
            matrix *= nodes_[i].node_matrix_;
    }                                       // for (int i = 0; i < n; i++)
}
//
//****************************************************************************
//                  --- dae_parser_action::characters ---
//
//!\brief   Process characters between elements
//!\param   chars - Characters string
//!\author  Pavel A. Chuvanov 04.05.2006
//****************************************************************************
void dae_parser_action::characters( key_type& chars )
{
    if (node_type(1) == E_DAE_NODE_LIBRARY_EFFECTS &&
        node_type(2) == E_DAE_NODE_EFFECT)
    {
        e_dae_node e0 = node_back_type(0);  // Last node
        e_dae_node e1 = node_back_type(1);  // Node before last
        e_dae_node e2 = node_back_type(2);  // Node twice before last

        if ( e0 == E_DAE_NODE_COLOR                                  ||
            (e0 == E_DAE_NODE_FLOAT3   && e1 == E_DAE_NODE_NEWPARAM) ||
            (e0 == E_DAE_NODE_FLOAT4X4 && e1 == E_DAE_NODE_NEWPARAM)
            )
        {
            parse_effect(nodes_[2], *node_back(1), node_back(0), &chars);
        }

        if (e0 == E_DAE_NODE_CODE && e2 == E_DAE_NODE_PROFILE_GLSL)
        {
            parse_effect(nodes_[2], *node_back(2), node_back(0), &chars);
        }
    }
    else if (node_type(5) == E_DAE_NODE_FLOAT_ARRAY &&
             node_type(4) == E_DAE_NODE_SOURCE      &&
             node_type(3) == E_DAE_NODE_MESH        &&
             node_type(2) == E_DAE_NODE_GEOMETRY    &&
             node_type(1) == E_DAE_NODE_LIBRARY_GEOMETRIES)
    {
        parse_geom_fa(nodes_[5], nodes_[4], nodes_[2], chars);
    }
    else if (node_type(5) == E_DAE_NODE_P        &&
             node_type(4) == E_DAE_NODE_POLYGONS &&
             node_type(3) == E_DAE_NODE_MESH     &&
             node_type(2) == E_DAE_NODE_GEOMETRY &&
             node_type(1) == E_DAE_NODE_LIBRARY_GEOMETRIES)
    {
        add_polygon(nodes_[2], nodes_[4], chars);
    }
    else if (node_back_type(0) == E_DAE_NODE_TRANSLATE &&
             node_back_type(1) == E_DAE_NODE_NODE)
    {
        move(*node_back(1), chars);
    }
    else if (node_back_type(0) == E_DAE_NODE_ROTATE &&
             node_back_type(1) == E_DAE_NODE_NODE)
    {
        rotate(*node_back(1), chars);
    }
    else if (node_back_type(0) == E_DAE_NODE_SCALE &&
             node_back_type(1) == E_DAE_NODE_NODE)
    {
        scale(*node_back(1), chars);
    }
    else if (node_type(2) == E_DAE_NODE_CAMERA           &&
             node_type(3) == E_DAE_NODE_OPTICS           &&
             node_type(4) == E_DAE_NODE_TECHNIQUE_COMMON &&
             node_type(5) == E_DAE_NODE_PERSPECTIVE)
    {
        if (node_type(6) == E_DAE_NODE_YFOV         ||
            node_type(6) == E_DAE_NODE_ASPECT_RATIO ||
            node_type(6) == E_DAE_NODE_ZNEAR        ||
            node_type(6) == E_DAE_NODE_ZFAR)
        {
            add_camera_param(nodes_[2], nodes_[6], chars);
        }
    }
    else if (node_back_type(0) == E_DAE_NODE_LOOKAT &&
             node_back_type(1) == E_DAE_NODE_NODE)
    {
        dae_node *node = node_back(1);
        tokenize_arr<float>(node->lookat_, chars);
    }
    else if (node_back_type(0) == E_DAE_NODE_INIT_FROM &&
             node_back_type(1) == E_DAE_NODE_IMAGE     &&
             node_back_type(2) == E_DAE_NODE_LIBRARY_IMAGES)
    {
        dae_node *img = node_back(1);
        teximgs_.insert(map_strstr::value_type(img->name_, chars));
    }
    else if (node_back_type(4) == E_DAE_NODE_LIBRARY_LIGHTS &&
             node_back_type(3) == E_DAE_NODE_LIGHT          &&
             node_back_type(1) == E_DAE_NODE_POINT)
    {
        set_light(*node_back(3), *node_back(0), chars);
    }
}
//
//****************************************************************************
//                --- dae_parser_action::parse_geom_fa ---
//
//!\brief   Parse geometry->source->float_array sequence
//!\param   float_arr - float_array element
//!\param   source    - source element
//!\param   geom      - geometry element
//!\param   chars     - Characters inside float_array element
//!\author  Pavel A. Chuvanov 27.05.2006
//****************************************************************************
void dae_parser_action::parse_geom_fa(dae_node &float_arr, dae_node &source,
                                      dae_node &geom, key_type &chars)
{
//----------------------- Obtain geometry container --------------------------

    geom_container_ptr cont;

    map_geometries::const_iterator I = geometry_.find( geom.name_ );
    if ( I == geometry_.end( ) )
    {
        cont = geom_container_ptr( new geom_container( ) );

        add( cont, geom.name_ );
    }
    else
        cont = I->second;

//------------------------------- Add array ----------------------------------

    vector_flt_ptr vf( new vector_flt );

    tokenize_arr<float>(*vf, chars);

    cont->flt_arrs.insert(float_arrays::value_type(float_arr.name_, vf));
    cont->flt_arrs.insert(float_arrays::value_type(source   .name_, vf));
}
//
//****************************************************************************
//               --- dae_parser_action::parse_geom_vert ---
//
//!\brief   Parse geometry->vertices->input sequence
//!\param   input    - Input
//!\param   vertices - Vertices
//!\param   geom     - Geometry
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::parse_geom_vert(dae_node &input, dae_node &vertices,
                                        dae_node &geom)
{
    map_geometries::const_iterator I = geometry_.find( geom.name_ );
    if ( I != geometry_.end( ) )
    {
        geom_container_ptr cont = I->second;

        float_arrays::const_iterator J = cont->flt_arrs.find( input.name_ );

        if ( J != cont->flt_arrs.end( ) )
        {
            cont->flt_arrs.insert(float_arrays::value_type(vertices.name_, J->second));
        }
    }
}
//
//****************************************************************************
//                --- dae_parser_action::parse_poly_in ---
//
//!\brief   Parse geometry->polygons->input
//!\param   input     - Input (Current element)
//!\param   type_node - Polygons element, Triangles element
//!\param   geom      - Geometry element
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::parse_poly_in(dae_node &input, dae_node &type_node,
                                      dae_node &geom)
{
    map_geometries::const_iterator I = geometry_.find( geom.name_ );
    if ( I != geometry_.end( ) )
    {
        geom_container_ptr         cont = I->second;

        shape_cont_ptr             mc;
        map_shapes::const_iterator J    = cont->shapes_.find( type_node.name_ );

        if ( J != cont->shapes_.end( ) )
            mc = J->second;
        else
        {
            mc = shape_cont_ptr( new shape_cont() );

            if (type_node.type_ == E_DAE_NODE_POLYGONS)
                mc->mesh_type_ = sg::mesh::POLYGONS;
            else if (type_node.type_ == E_DAE_NODE_TRIANGLES)
                mc->mesh_type_ = sg::mesh::TRIANGLES;
            else if (type_node.type_ == E_DAE_NODE_TRISTRIPS)
                mc->mesh_type_ = sg::mesh::TRIANGLESTRIP;
//            else if (type_node.type_ == E_DAE_NODE_TRIFANS)
//                mc->mesh_type_ = sg::mesh::TRIFANS;

            mc->material_name_ = type_node.name_;
            cont->add(mc, type_node.name_);
        }

        opl_string        semantic;
        opl_string        idx;

        get_value_from_name( L"semantic", attributes_, semantic );
        get_value_from_name( L"offset",   attributes_, idx );

        float_arrays::const_iterator itFA = cont->flt_arrs.find( input.name_ );

        if ( itFA != cont->flt_arrs.end( ) )
        {
            if (semantic == L"VERTEX")
            {
                mc->vertex_idx_ = boost::lexical_cast<int>( idx.c_str( ) );
                mc->vertices_   = itFA->second;
            }
            else if (semantic == L"NORMAL")
            {
                mc->normal_idx_ = boost::lexical_cast<int>( idx.c_str( ) );
                mc->normals_    = itFA->second;
            }
            else if (semantic == L"TEXCOORD")
            {
                mc->texture_idx_ = boost::lexical_cast<int>( idx.c_str( ) );
                mc->texcoords_   = itFA->second;
            }

            mc->indices_stride_ = 0;

            if (mc->vertex_idx_ != -1)
                mc->indices_stride_++;
            if (mc->normal_idx_ != -1)
                mc->indices_stride_++;
            if (mc->texture_idx_ != -1)
                mc->indices_stride_++;
        }
    }
}
//
//****************************************************************************
//                 --- dae_parser_action::add_polygon ---
//
//!\brief   Parse p element
//!\param   geom     - Geometry element
//!\param   polygons - Polygons element
//!\param   chars    - Indices array
//!\author  Pavel A. Chuvanov 20.05.2006
//****************************************************************************
void dae_parser_action::add_polygon(dae_node &geom, dae_node &polygons, key_type &chars)
{
    map_geometries::const_iterator I = geometry_.find( geom.name_ );
    if ( I != geometry_.end( ) )
    {
        geom_container_ptr cont = I->second;

        shape_cont_ptr              matcont;
        map_shapes::const_iterator J = cont->shapes_.find( polygons.name_ );

        if ( J != cont->shapes_.end( ) )
            matcont = J->second;
        else
        {
            matcont = shape_cont_ptr( new shape_cont() );
            matcont->material_name_ = polygons.name_;
            cont->add(matcont, polygons.name_);
        }

        std::vector<int>   indices;

        tokenize_arr<int>(indices, chars);

        int                icnt = indices.size();

        matcont->count_.push_back(icnt);

        for (int i = 0; i < icnt; i++)
            matcont->indices_.push_back(indices[i]);
    }
}
//
//****************************************************************************
//               --- dae_parser_action::add_camera_param ---
//
//!\brief   Add camera parameter (create camera if it first parameter)
//!\param   camera - Camera node
//!\param   param  - Param node
//!\param   chars  - Characters
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::add_camera_param(dae_node &camera, dae_node &param,
                                         key_type &chars)
{
    camera_cont_ptr cam;

    map_cameras::const_iterator I = cameras_.find( camera.name_ );
    if ( I == cameras_.end( ) )
    {
        cam = camera_cont_ptr(new camera_cont( ));
        add( cam, camera.name_ );
    }
    else
        cam = I->second;

    while (chars[0] == L' ')
        chars.erase(0, 1);

    if (param.type_ == E_DAE_NODE_YFOV)
        cam->yfov_  = boost::lexical_cast<float>( chars.c_str( ) );
    else if (param.type_ == E_DAE_NODE_ZNEAR)
    {
        cam->znear_ = boost::lexical_cast<float>( chars.c_str( ) );

        // Note: Collada RT sets znear to 1.0 if znear less. The code has
        //       following comment
        //       !!!GAC force Znear to be >= 1.0 to avoid rendering problems

        if (cam->znear_ < 1.0f)
            cam->znear_ = 1.0f;
    }
    else if (param.type_ == E_DAE_NODE_ZFAR)
        cam->zfar_  = boost::lexical_cast<float>( chars.c_str( ) );
    else if (param.type_ == E_DAE_NODE_ASPECT_RATIO)
        cam->aspect_ratio_ = boost::lexical_cast<float>( chars.c_str( ) );
}
//
//****************************************************************************
//                     --- dae_parser_action::move ---
//
//!\brief   Handle scene->node->translate
//!\param   node  - Object node
//!\param   chars - float3 (x y z)
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::move(dae_node &node, key_type &chars)
{
    vector_flt vf;
    tokenize_arr<float>(vf, chars);

    if (vf.size() == 3)
        node.node_matrix_ *= opl::make_translation( opl::vector_3<float>(vf[0], vf[1], vf[2]) );
}
//
//****************************************************************************
//                    --- dae_parser_action::rotate ---
//
//!\brief   Handle scene->node->rotate
//!\param   node  - Object node
//!\param   chars - float4 (x y z angle)
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::rotate(dae_node &node, key_type &chars)
{
    vector_flt vf;
    tokenize_arr<float>(vf, chars);

    if (vf.size() == 4)
    {
        opl::axis_angle<float> aa(vf[0], vf[1], vf[2], vf[3]);
        node.node_matrix_ *= opl::from_axis_angle_rotation( aa );
    }
}
//
//****************************************************************************
//                    --- dae_parser_action::scale ---
//
//!\brief   Handle node->scale
//!\param   node  - Node for that scale will aplly
//!\param   chars - Characters with scale values
//!\author  Pavel A. Chuvanov 21.05.2006
//****************************************************************************
void dae_parser_action::scale(dae_node &node, key_type &chars)
{
    vector_flt vf;
    tokenize_arr<float>(vf, chars);

    if (vf.size() == 3)
    {
        opl::vector_3<float> v(vf[0], vf[1], vf[2]);
        node.node_matrix_ *= opl::make_scale( v );
    }
}
//
//****************************************************************************
//                 --- dae_parser_action::init_m_float ---
//
//!\brief   Create attributes array for specified mesh field
//!\param   mesh       - Mesh
//!\param   field      - Field name
//!\param   components - Count of values components
//!\retval             - Created array
//!\author  Pavel A. Chuvanov 12.05.2006
//****************************************************************************
opl::m_float *dae_parser_action::init_m_float(sg::mesh_ptr     &mesh,
                                              const opl_string &field,
                                              int              components)
{
    sg::v_attribute_array_ptr *p = mesh->pointer<sg::v_attribute_array_ptr>( field );
    p->get()                     = sg::attribute_array_ptr(new attribute_array( ));

    opl::m_float *pRet           = p->get()->pointer<opl::m_float>( L"value" );

    opl::v_int32 *comp           = p->get()->pointer<opl::v_int32>( L"components" );
    comp->get( ) = components;

    return pRet;
}
//
//****************************************************************************
//                --- dae_parser_action::parse_effect ---
//
//!\brief   Parse effect node
//!\param   effect_node - Effect node (Effect name)
//!\param   type_node   - Type of node (emission, ambient, diffuse, texture)
//!\param   chars_node  - Characters node type (color, float), can be NULL
//!\param   chars       - Characters, can be NULL
//!\author  Pavel A. Chuvanov 23.05.2006
//****************************************************************************
void dae_parser_action::parse_effect(dae_node     & effect_node,
                                     dae_node     & type_node,
                                     dae_node     * chars_node,
                                     opl::wstring * chars)
{
    sg::appearance_ptr app;

    map_effects::const_iterator I = effects_.find( effect_node.name_ );
    if ( I == effects_.end( ) )
    {
        app = sg::appearance_ptr( new sg::appearance( ) );
        effects_.insert( map_effects::value_type(effect_node.name_, app) );
    }
    else
        app = I->second;

    if (chars_node && chars)
    {
        if (type_node.type_ == E_DAE_NODE_NEWPARAM)
        {
            parse_shader(effect_node, app, type_node, *chars_node, *chars);
        }
        else if (type_node.type_ == E_DAE_NODE_PROFILE_GLSL)
        {
            parse_shader(effect_node, app, type_node, *chars_node, *chars);
        }
        else if (chars_node->type_ == E_DAE_NODE_COLOR ||
                 chars_node->type_ == E_DAE_NODE_FLOAT)
        {
            parse_effect(app, type_node, *chars);
        }
    }
    else if (type_node.type_ == E_DAE_NODE_TEXTURE)
    {
        map_strstr::const_iterator I = teximgs_.find( type_node.name_ );
        if ( I != teximgs_.end( ) )
        {
            sg::texture_ptr im = sg::texture_ptr( new sg::texture( ) );

            sg::mv_texture_ptr* texture = app->pointer<sg::mv_texture_ptr>( L"texture" );

            if (texture)
            {
                texture->push_back( sg::v_texture_ptr( im ) );
            }

            opl::m_url *url = im->pointer<opl::m_url>( L"url" );

            url->push_back( path( opl::to_string( I->second ).c_str() ) );

            im->update( );
        }
    }
}
//
//****************************************************************************
//                --- dae_parser_action::parse_effect ---
//
//!\brief   Add material node/material node parameter into appearance node
//!\param   app        - Appearance object reference
//!\param   type_node  - Material parameter type (emission, ambient, diffuse)
//!\param   chars      - Characters
//!\author  Pavel A. Chuvanov 23.05.2006
//****************************************************************************
void dae_parser_action::parse_effect(sg::appearance_ptr & app,
                                     dae_node           & type_node,
                                     opl::wstring       & chars)
{
    sg::v_material_ptr *v_mat = app->pointer<sg::v_material_ptr>( L"material" );
    sg::material_ptr    mat   = v_mat->get( );

    if (mat == NULL)
    {
        mat           = sg::material_ptr( new sg::material( ) );
        v_mat->get( ) = mat;
    }

    switch (type_node.type_)
    {
        case E_DAE_NODE_DIFFUSE:
            assign_field<opl::v_color>().assign( mat, L"diffuseColor", chars );
            break;

        case E_DAE_NODE_EMISSION:
            assign_field<opl::v_color>().assign( mat, L"emissiveColor", chars );
            break;

        case E_DAE_NODE_SPECULAR:
            assign_field<opl::v_color>().assign( mat, L"specularColor", chars );
            break;

        case E_DAE_NODE_SHININESS:
            assign_field<opl::v_float>().assign( mat, L"shininess", chars );
            break;

        case E_DAE_NODE_TRANSPARENCY:
            assign_field<opl::v_float>().assign( mat, L"transparency", chars );
            break;
    }                                       // switch (type_node.type_)
}
//
//****************************************************************************
//                 --- dae_parser_action::parse_shader ---
//
//!\brief   Parse shader elements (newparam, code, etc.)
//!\param   effect_node - Effect node
//!\param   app         - Appearance object
//!\param   type_node   - Type node (newparam). Param name
//!\param   chars_node  - Characters node
//!\param   chars       - Characters
//!\author  Pavel A. Chuvanov 07.06.2006
//****************************************************************************
void dae_parser_action::parse_shader(dae_node           & effect_node,
                                     sg::appearance_ptr & app,
                                     dae_node           & type_node,
                                     dae_node           & chars_node,
                                     opl::wstring       & chars)
{
    v_shader_ptr * v_shader = app->pointer<sg::v_shader_ptr>( L"shader" );
    sg::shader_ptr shader   = v_shader->get( );

    if (shader == NULL)
    {
        shader             = sg::composed_shader_ptr( new sg::composed_shader( ) );
        v_shader->get( )   = shader;

        opl::v_string *def = shader->pointer<opl::v_string>( L"DEF" );
        def->get( )        = effect_node.name_;
    }

    if (type_node.type_ == E_DAE_NODE_PROFILE_GLSL)
    {
        opl::v_string *l = shader->pointer<opl::v_string>( L"language" );
        l->get( )        = L"GLSL";
    }

    switch (chars_node.type_)
    {
        case E_DAE_NODE_INT:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_int32_typename, opl::v_int32( ) ) );
            assign_field<opl::v_int32, true>( ).assign<int>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_FLOAT:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_float_typename, opl::v_float( ) ) );
            assign_field<opl::v_float, true>( ).assign<float>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_FLOAT2:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_vec2f_typename, opl::v_vec2f( ) ) );
            assign_field<opl::v_vec2f, true>( ).assign<float>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_FLOAT3:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_vec3f_typename, opl::v_vec3f( ) ) );
            assign_field<opl::v_vec3f, true>( ).assign<float>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_FLOAT4:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_vec4f_typename, opl::v_vec4f( ) ) );
            assign_field<opl::v_vec4f, true>( ).assign<float>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_FLOAT2X2:
            break;

        case E_DAE_NODE_FLOAT3X3:
            break;

        case E_DAE_NODE_FLOAT4X4:
            shader->insert_param( type_node.name_, sg::shader::field_type( opl::v_matrix4f_typename, opl::v_matrix4f( ) ) );
            assign_field<opl::v_matrix4f, true>( ).assign<float>( shader, type_node.name_, chars );
            break;

        case E_DAE_NODE_CODE:
            parse_shader_code(shader, chars);
            break;
    }                                   // switch (chars_node->type_)
}
//
//****************************************************************************
//                 --- dae_parser_action::shader_part ---
//
//!\brief   Generate shader part
//!\param   shader - Shader text or shader file path name (depend on source)
//!\param   type   - Type of shader (one of: "VERTEX", "FRAGMENT")
//!\param   source - true - shader is shader text, otherwise shader is shader
//!                  file path name
//!\retval         - Shader part object
//!\author  Pavel A. Chuvanov 09.06.2006
//****************************************************************************
shader_part_ptr dae_parser_action::shader_part( const opl::wstring & shader,
                                                const opl::wstring & type,
                                                bool                 source )
{
    shader_part_ptr p( new sg::shader_part( ) );

    opl::m_url*     u = p->pointer<opl::m_url>( L"url" );
    opl::v_string*  t = p->pointer<opl::v_string>( L"type" );
    opl::v_string*  s = p->pointer<opl::v_string>( L"source" );
    opl::v_bool*    e = p->pointer<opl::v_bool>( L"embedded" );

    if( !source )
    {
        u->push_back( boost::filesystem::path( opl::to_string(shader.c_str( )).c_str(),
                      boost::filesystem::native ) );
        e->get( ) = false;
    }
    else
    {
        s->get( ) = shader;
        e->get( ) = true;
    }

    t->get( ) = type;

    return p;
}
//
//****************************************************************************
//              --- dae_parser_action::parse_shader_code ---
//
//!\brief   Parse and add shader code
//!\param   shader - Shader object
//!\param   code   - Shader code text
//!\author  Pavel A. Chuvanov 09.06.2006
//****************************************************************************
void dae_parser_action::parse_shader_code(sg::shader_ptr &shader, const opl::wstring &code)
{
    m_shader_part_ptr* p = shader->pointer<m_shader_part_ptr>( L"parts" );

    p->push_back( shader_part( code, L"VERTEX", true ) );
    p->push_back( shader_part( code, L"FRAGMENT", true ) );
}
//
//****************************************************************************
//                 --- dae_parser_action::add_instance ---
//
//!\brief   Add instance
//!\param   instance - Name of source
//!\author  Pavel A. Chuvanov 11.05.2006
//****************************************************************************
void dae_parser_action::add_instance(dae_node &instance)
{
    opl::matrixf mx;                        // Matrix from nodes stack
    build_node_matrix(mx);

    if (instance.type_ == E_DAE_NODE_INSTANCE_GEOMETRY)
    {
//---------------------------- Insert geometry -------------------------------

        map_geometries::const_iterator I = geometry_.find( instance.name_ );
        if ( I != geometry_.end( ) )
        {
            geom_container_ptr cont     = I->second;
            sg::m_spatial_ptr* children = scene_->get_root()->pointer<sg::m_spatial_ptr>( L"children" );

            add_geom(mx, cont, children);
        }
    }
    else if (instance.type_ == E_DAE_NODE_INSTANCE_CAMERA)
    {
//----------------------------- Insert camera --------------------------------

        if (!is_viewpnt_set_)
        {
            map_cameras::const_iterator I = cameras_.find( instance.name_ );
            if ( I != cameras_.end( ) )
            {
                camera_cont_ptr cam = I->second;
                is_viewpnt_set_     = true;

                opl::vec3f pos(mx[12], mx[13], mx[14]);
                opl::vec3f look;
                opl::vec4f up = mx * opl::vec4f( 0.0f, 1.0f, 0.0f, 0.0f );

                sg::camera_ptr c( new sg::camera( ) );

                c->perspective( cam->yfov_ * 180.0f / static_cast<float>( M_PI ),
                               cam->aspect_ratio_, cam->znear_, cam->zfar_ );

                if (node_back_type(1) == E_DAE_NODE_NODE && node_back(1)->lookat_.size() == 9)
                {
                    vector_flt &v = node_back(1)->lookat_;
                    pos  = opl::vec3f(v[0], v[1], v[2]);
                    look = opl::vec3f(v[3], v[4], v[5]);
                    up   = opl::vec4f(v[6], v[7], v[8], 0);
                }

                c->look_at( pos, look, opl::vec3f( up[ 0 ], up[ 1 ], up[ 2 ] ) );

                /*
                printf("Camera\n");
                printf("    YFOV   : %f\n", cam->yfov_);
                printf("    Aspect : %f\n", cam->aspect_ratio_);
                printf("    ZNEAR  : %f\n", cam->znear_);
                printf("    ZFAR   : %f\n", cam->zfar_);

                printf("    pos    : %f %f %f\n", pos[0], pos[1], pos[2]);
                printf("    look   : %f %f %f\n", look[0], look[1], look[2]);
                printf("    up     : %f %f %f\n", up[0], up[1], up[2]);
                */

                get_scene( )->add_camera( c );
            }
        }
    }
}
//
//****************************************************************************
//                   --- dae_parser_action::add_geom ---
//
//!\brief   Add geometry
//!\param   node_matrix - Node matrix
//!\param   geom        - Geometry container
//!\param   children    - Children object to add shapes
//!\author  Pavel A. Chuvanov 21.05.2006
//****************************************************************************
void dae_parser_action::add_geom(opl::matrixf &node_matrix, geom_container_ptr &geom,
                                 sg::m_spatial_ptr *children)
{
    sg::spatial_ptr  grp( new sg::transform( ) );
    opl::v_matrix4f* transform = grp->pointer<opl::v_matrix4f>( L"matrix" );
    transform->get( ) = node_matrix;
    children->push_back(grp);

    children = grp->pointer<sg::m_spatial_ptr>( L"children" );

    map_shapes::const_iterator I = geom->shapes_.begin( );

    while (I != geom->shapes_.end( ) )
    {
        sg::shape_ptr shape( new sg::shape( ) );
        children->push_back( shape );

        sg::v_mesh_ptr* geometry = shape->pointer<sg::v_mesh_ptr>( L"geometry" );
        geometry->get( )         = sg::mesh_ptr( new sg::mesh );

        shape_cont_ptr ptr = I->second;
        set_mesh    (ptr, geometry->get( ));
        set_material(ptr, shape);

        I++;
    }                                       // while (I != geom.shapes_.end(
}
//
//****************************************************************************
//                 --- dae_parser_action::set_material ---
//
//!\brief   Set material
//!\param   cont  - Shape container (plugin data)
//!\param   shape - Shape container (library data)
//!\author  Pavel A. Chuvanov 23.05.2006
//****************************************************************************
void dae_parser_action::set_material(shape_cont_ptr &cont, sg::shape_ptr &shape)
{
    map_strstr::const_iterator I = mateff_.find( cont->material_name_ );
    if ( I != mateff_.end( ) )
    {
        // Note: I->second - Contain effect name

        map_effects::const_iterator J = effects_.find(I->second);

        if ( J != effects_.end( ) )
        {
            sg::v_appearance_ptr *v_app = shape->pointer<sg::v_appearance_ptr>( L"appearance" );
            v_app->get( ) = J->second;
        }
    }
}
//
//****************************************************************************
//                   --- dae_parser_action::set_mesh ---
//
//!\brief   Setup mesh
//!\param   cont - Container. Contain data to set into mesh
//!\param   mesh - Mesh
//!\author  Pavel A. Chuvanov 12.05.2006
//****************************************************************************
void dae_parser_action::set_mesh(shape_cont_ptr &cont, sg::mesh_ptr &mesh)
{
    mesh->prim_type() = cont->mesh_type_;

//----------------------- Allocate attribute arrays --------------------------

    opl::m_float *arr[3]    = { NULL, NULL, NULL };
                                            // [vertices, normals, texture coords]
    int           vx_stride = 3;            // Count of floats in vertex
    int           nm_stride = 3;            // Count of floats in normal
    int           tx_stride = 2;            // Count of floats in text coord

    if (cont->vertices_ != NULL && cont->vertex_idx_ != -1)
        arr[0] = init_m_float(mesh, L"coord", 3);

    if (cont->normals_ != NULL && cont->normal_idx_ != -1)
        arr[1] = init_m_float(mesh, L"normal", 3);

    if (cont->texcoords_ != NULL && cont->texture_idx_ != -1)
    {
        sg::mv_uv_set_ptr *mv_uv  = mesh->pointer<sg::mv_uv_set_ptr>( L"uv" );
        sg::uv_set_ptr     uv_set = sg::uv_set_ptr( new sg::uv_set );

        mv_uv->push_back( sg::v_uv_set_ptr( uv_set ) );

        sg::v_attribute_array_ptr* uv = uv_set->pointer<sg::v_attribute_array_ptr>( L"uv" );
        sg::attribute_array_ptr    att( new attribute_array( ) );

        uv->get( ) = att;

        opl::v_int32 *comp = att->pointer<opl::v_int32>( L"components" );
        comp->get( ) = 2;

        arr[2] = att->pointer<opl::m_float>( L"value" );
    }

//----------------------- Fill index & count fields --------------------------

    opl::m_int32 *field_index = mesh->pointer<opl::m_int32>( L"index" );
    opl::m_int32 *field_count = mesh->pointer<opl::m_int32>( L"count" );

    size_t i;
    size_t sz = cont->count_.size();

    for ( i = 0; i < sz; ++i )
        field_count->push_back(cont->count_[i] / cont->indices_stride_);

    sz = cont->indices_.size() / cont->indices_stride_;

    for ( i = 0; i < sz; ++i )
        field_index->push_back(i);

//------------------------------ Fill arrays ---------------------------------

    if (arr[0])
        arr[0]->reserve(sz * vx_stride);
    if (arr[1])
        arr[1]->reserve(sz * nm_stride);
    if (arr[2])
        arr[2]->reserve(sz * tx_stride);

    opl::m_int32::const_pointer s = cont->indices_.data( );

    for ( i = 0; i < sz; ++i )
    {
        if (arr[0])
        {
            int index = s[cont->vertex_idx_] * vx_stride;
            for (int j = 0; j < vx_stride; j++)
                arr[0]->push_back((*cont->vertices_)[index + j]);
        }

        if (arr[1])
        {
            int index = s[cont->normal_idx_] * nm_stride;
            for (int j = 0; j < nm_stride; j++)
                arr[1]->push_back((*cont->normals_)[index + j]);
        }

        if (arr[2])
        {
            int index = s[cont->texture_idx_] * tx_stride;
            for (int j = 0; j < tx_stride; j++)
                arr[2]->push_back((*cont->texcoords_)[index + j]);
        }

        s += cont->indices_stride_;
    }                                   // for ( i = 0; i < sz; ++i )

    /*
    printf("normal : %d\n", arr[1]->size());

    {
        printf("vertex : %d\n", arr[0]->size());
        int n = arr[0]->size();
        for (int i = 0; i < n;)
        {
            printf("%3d: %3f %3f %3f\n", i / 3, (*arr[0])[i], (*arr[0])[i + 1], (*arr[0])[i + 2]);
            i += 3;
        }
    }

    {
        printf("indices\n");

        int n   = field_count->size();
        int idx = 0;

        for (int i = 0; i < n; i++)
        {
            printf("%d ", (*field_count)[i]);

            for (int j = 0; j < (*field_count)[i]; j++)
            {
                printf("%d ", (*field_index)[idx]);
                idx++;
            }

            printf("\n");
        }
    }*/
}
//
//****************************************************************************
//                  --- dae_parser_action::set_light ---
//
//!\brief   Set point light parameter
//!\param   light      - Light name
//!\param   param_type - Parameter type
//!\param   chars      - Characters
//!\author  Pavel A. Chuvanov 26.05.2006
//****************************************************************************
void dae_parser_action::set_light(dae_node &light, dae_node &param_type, key_type &chars)
{
    std::vector<float> v;
    tokenize_arr<float>(v, chars);

    switch (param_type.type_)
    {
        case E_DAE_NODE_COLOR:
            if (v.size() == 3)
                get_light(light.name_, true)->color = opl::color<float>(v[0], v[1], v[2]);
            break;

        case E_DAE_NODE_CONSTANT_ATTENUATION:
            if (v.size() == 1)
                get_light(light.name_, true)->constant_attenuation = v[0];
            break;

        case E_DAE_NODE_LINEAR_ATTENUATION:
            if (v.size() == 1)
                get_light(light.name_, true)->linear_attenuation = v[0];
            break;

        case E_DAE_NODE_QUADRATIC_ATTENUATION:
            if (v.size() == 1)
                get_light(light.name_, true)->quadratic_attenuation = v[0];
            break;
    }                                       // switch (param_type.type_)
}
//
//****************************************************************************
//                  --- dae_parser_action::get_light ---
//
//!\brief   Get light
//!\param   name         - Name of light
//!\param   allow_create - true - create container if not exist
//!\retval               - Container
//!\author  Pavel A. Chuvanov 26.05.2006
//****************************************************************************
light_cont_ptr dae_parser_action::get_light(key_type &name, bool allow_create)
{
    light_cont_ptr             p;
    map_lights::const_iterator I = lights_.find( name );
    if ( I != lights_.end( ) )
        p = I->second;
    else if (allow_create)
    {
        p = light_cont_ptr( new light_cont( ) );
        lights_.insert( map_lights::value_type( name, p ) );
    }

    return p;
}

} } } }
