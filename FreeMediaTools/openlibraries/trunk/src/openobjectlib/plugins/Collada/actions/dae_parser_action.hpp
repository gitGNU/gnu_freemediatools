
// sg - A scene graph representation.

// Copyright (C) 2005-2006 VM Inc.
// Released under the LGPL.
// For more information, see http://www.openlibraries.org.

#ifndef DAE_PARSER_ACTION_INC_
#define DAE_PARSER_ACTION_INC_

#include <map>
#include <stack>

#include <boost/filesystem/path.hpp>
#include <boost/shared_ptr.hpp>

#include <openobjectlib/sg/scene.hpp>
#include <openobjectlib/sg/appearance.hpp>
#include <openobjectlib/sg/meshes.hpp>
#include <openobjectlib/sg/shaders.hpp>

//------------------------- forward declarations -----------------------------

#ifdef WIN32

struct MSXML2::ISAXAttributes;
#define XML_ATTR_TYPE   MSXML2::ISAXAttributes __RPC_FAR

#else                                       // #ifdef WIN32

#include <libxml/parser.h>
#define XML_ATTR_TYPE   xmlChar *

#endif                                      // #ifdef WIN32

namespace opl = olib::openpluginlib;
namespace olib { namespace openobjectlib { namespace sg { namespace actions {

typedef openpluginlib::wstring                  opl_string;
typedef std::vector<float>                      vector_flt;
typedef boost::shared_ptr<vector_flt>           vector_flt_ptr;
typedef std::map<opl_string, vector_flt_ptr>    float_arrays;
typedef int                                     ACTION_RETURN;

const ACTION_RETURN ACTION_RETURN_CONTINUE  = 0;
const ACTION_RETURN ACTION_RETURN_ERROR     = 1;
const ACTION_RETURN ACTION_RETURN_IGNORE    = 2;

enum e_dae_node                             //! DAE plugin known XML nodes
{
    E_DAE_NODE_UNKNOWN,                     //!< Unknown node

    E_DAE_NODE_ACCESSOR,
    E_DAE_NODE_AMBIENT,
    E_DAE_NODE_ANIMATION,
    E_DAE_NODE_ANIMATION_CLIP,
    E_DAE_NODE_ASSET,
    E_DAE_NODE_ASPECT_RATIO,
    E_DAE_NODE_BOOL_ARRAY,
    E_DAE_NODE_CAMERA,
    E_DAE_NODE_CHANNEL,
    E_DAE_NODE_CODE,
    E_DAE_NODE_COLLADA,
    E_DAE_NODE_COLOR,
    E_DAE_NODE_CONSTANT_ATTENUATION,
    E_DAE_NODE_CONTRIBUTOR,
    E_DAE_NODE_CONTROLLER,
    E_DAE_NODE_DIFFUSE,
    E_DAE_NODE_DIRECTIONAL,
    E_DAE_NODE_EFFECT,
    E_DAE_NODE_EMISSION,
    E_DAE_NODE_EXTRA,
    E_DAE_NODE_FLOAT,
    E_DAE_NODE_FLOAT2,
    E_DAE_NODE_FLOAT3,
    E_DAE_NODE_FLOAT4,
    E_DAE_NODE_FLOAT2X2,
    E_DAE_NODE_FLOAT3X3,
    E_DAE_NODE_FLOAT4X4,
    E_DAE_NODE_FLOAT_ARRAY,
    E_DAE_NODE_GEOMETRY,
    E_DAE_NODE_IDREF_ARRAY,
    E_DAE_NODE_IMAGE,
    E_DAE_NODE_IMAGER,
    E_DAE_NODE_INIT_FROM,
    E_DAE_NODE_INPUT,
    E_DAE_NODE_INSTANCE_ANIMATION,
    E_DAE_NODE_INSTANCE_CAMERA,
    E_DAE_NODE_INSTANCE_CONTROLLER,
    E_DAE_NODE_INSTANCE_EFFECT,
    E_DAE_NODE_INSTANCE_GEOMETRY,
    E_DAE_NODE_INSTANCE_LIGHT,
    E_DAE_NODE_INSTANCE_NODE,
    E_DAE_NODE_INSTANCE_VISUAL_SCENE,
    E_DAE_NODE_INT,
    E_DAE_NODE_INT2,
    E_DAE_NODE_INT3,
    E_DAE_NODE_INT4,
    E_DAE_NODE_INT_ARRAY,
    E_DAE_NODE_JOINTS,
    E_DAE_NODE_LIBRARY_ANIMATIONS,
    E_DAE_NODE_LIBRARY_ANIMATION_CLIPS,
    E_DAE_NODE_LIBRARY_CAMERAS,
    E_DAE_NODE_LIBRARY_CONTROLLERS,
    E_DAE_NODE_LIBRARY_EFFECTS,
    E_DAE_NODE_LIBRARY_FORCE_FIELDS,
    E_DAE_NODE_LIBRARY_GEOMETRIES,
    E_DAE_NODE_LIBRARY_IMAGES,
    E_DAE_NODE_LIBRARY_LIGHTS,
    E_DAE_NODE_LIBRARY_MATERIALS,
    E_DAE_NODE_LIBRARY_NODES,
    E_DAE_NODE_LIBRARY_PHYSICS_MODELS,
    E_DAE_NODE_LIBRARY_PHYSICS_SCENES,
    E_DAE_NODE_LIBRARY_VISUAL_SCENES,
    E_DAE_NODE_LIGHT,
    E_DAE_NODE_LINEAR_ATTENUATION,
    E_DAE_NODE_LINES,
    E_DAE_NODE_LINESTRIPS,
    E_DAE_NODE_LOOKAT,
    E_DAE_NODE_MATERIAL,
    E_DAE_NODE_MATRIX,
    E_DAE_NODE_MESH,
    E_DAE_NODE_MORPH,
    E_DAE_NODE_NAME_ARRAY,
    E_DAE_NODE_NEWPARAM,
    E_DAE_NODE_NODE,
    E_DAE_NODE_OPTICS,
    E_DAE_NODE_ORTHOGRAPHIC,
    E_DAE_NODE_P,
    E_DAE_NODE_PARAM,
    E_DAE_NODE_PERSPECTIVE,
    E_DAE_NODE_POINT,
    E_DAE_NODE_POLYGONS,
    E_DAE_NODE_POLYLIST,
    E_DAE_NODE_PROFILE_GLSL,
    E_DAE_NODE_QUADRATIC_ATTENUATION,
    E_DAE_NODE_ROTATE,
    E_DAE_NODE_SAMPLER,
    E_DAE_NODE_SCALE,
    E_DAE_NODE_SCENE,
    E_DAE_NODE_SHININESS,
    E_DAE_NODE_SKELETON,
    E_DAE_NODE_SKEW,
    E_DAE_NODE_SKIN,
    E_DAE_NODE_SOURCE,
    E_DAE_NODE_SPECULAR,
    E_DAE_NODE_SPLINE,
    E_DAE_NODE_SPOT,
    E_DAE_NODE_TARGETS,
    E_DAE_NODE_TECHNIQUE,
    E_DAE_NODE_TECHNIQUE_COMMON,
    E_DAE_NODE_TEXTURE,
    E_DAE_NODE_TRANSLATE,
    E_DAE_NODE_TRANSPARENCY,
    E_DAE_NODE_TRIANGLES,
    E_DAE_NODE_TRIFANS,
    E_DAE_NODE_TRISTRIPS,
    E_DAE_NODE_VERTEX_WEIGHTS,
    E_DAE_NODE_VERTICES,
    E_DAE_NODE_VISUAL_SCENE,
    E_DAE_NODE_YFOV,
    E_DAE_NODE_ZFAR,
    E_DAE_NODE_ZNEAR,
};

//
///////////////////////////////////////////////////////////////////////////////
//                          --- struct dae_node ---                          //
///////////////////////////////////////////////////////////////////////////////
struct dae_node
{
    dae_node(e_dae_node e, const openpluginlib::wstring &s = L"");

    e_dae_node      type_;                  //!< Node type
    opl_string      name_;                  //!< Object name (map keyword)
    opl::matrixf    node_matrix_;           //!< Node matrix

    vector_flt      lookat_;                //!< Look At node if not empty
};

inline dae_node::dae_node(e_dae_node e, const openpluginlib::wstring &s)
    : type_(e)
    , name_(s)
    , node_matrix_( opl::make_identity<float>( ) )
{
}

//
///////////////////////////////////////////////////////////////////////////////
//                         --- Class shape_cont ---                          //
///////////////////////////////////////////////////////////////////////////////
class shape_cont
{
public:
    // Construction

    shape_cont();

    // Member variables

    int                 mesh_type_;         //!< Type of mesh (TRIANGLES, POLYGONS, etc.)
    opl_string          material_name_;     //!< material name
    opl::m_int32        indices_;           //!< Indices for polygons
    opl::m_int32        count_;             //!< Count of indices in each poligon
    vector_flt_ptr      vertices_;          //!< Vertices
    vector_flt_ptr      normals_;           //!< Normals
    vector_flt_ptr      texcoords_;         //!< UV map coordinates
    int                 vertex_idx_;        //!< Offset of vertex in indicises
    int                 normal_idx_;        //!< Offset of normal in indicises
    int                 texture_idx_;       //!< Offset of texture in indiceses
    int                 indices_stride_;    //!< Indices stride
};

inline shape_cont::shape_cont()
  : vertex_idx_(-1)
  , normal_idx_(-1)
  , texture_idx_(-1)
  , indices_stride_(0)
  , mesh_type_(sg::mesh::UNKNOWN)
  { }

typedef boost::shared_ptr<shape_cont>            shape_cont_ptr;
typedef std::map<opl_string, shape_cont_ptr>     map_shapes;

//
///////////////////////////////////////////////////////////////////////////////
//                       --- Class geom_container ---                        //
///////////////////////////////////////////////////////////////////////////////
class geom_container
{
public:
    // API

    void           add                  ( shape_cont_ptr    & node,
                                          const opl_string  & key);

    // Variables

    float_arrays        flt_arrs;           //!< Named float arrays
    map_shapes          shapes_;            //!< Container with polygons indices
                                            //!< for specified material
};

inline void geom_container::add( shape_cont_ptr & n, const opl_string & k)
    { shapes_.insert(map_shapes::value_type(k, n)); }

typedef boost::shared_ptr<geom_container>       geom_container_ptr;

//
///////////////////////////////////////////////////////////////////////////////
//                        --- Class camera_cont ---                          //
///////////////////////////////////////////////////////////////////////////////
class camera_cont
{
public:
    // Construction

            camera_cont();

    // Member variables

    float           yfov_;
    float           aspect_ratio_;          //!< Aspect ratio
    float           znear_;
    float           zfar_;
};

inline camera_cont::camera_cont() : aspect_ratio_(1) { }

typedef boost::shared_ptr<camera_cont>       camera_cont_ptr;

//
///////////////////////////////////////////////////////////////////////////////
//                        --- struct light_cont ---                          //
///////////////////////////////////////////////////////////////////////////////
struct light_cont
{
    light_cont();

    // Member variables

    opl::color<float> color;                //!< Light color
    float             constant_attenuation; //!< Constant attenuation
    float             linear_attenuation;   //!< Linear attenuation
    float             quadratic_attenuation;//!< Quadratic attenuation
};

inline light_cont::light_cont()
                 : color(1, 1, 1)
                 , constant_attenuation(1)
                 , linear_attenuation(0)
                 , quadratic_attenuation(0)
{
}

typedef boost::shared_ptr<light_cont>       light_cont_ptr;

//
///////////////////////////////////////////////////////////////////////////////
//                     --- Class dae_parser_action ---                       //
///////////////////////////////////////////////////////////////////////////////
class dae_parser_action
{
private:
    dae_parser_action( const dae_parser_action& other );
    dae_parser_action& operator=( const dae_parser_action& other );

public:
    typedef openpluginlib::wstring                  key_type;
    typedef std::map<key_type, e_dae_node>          container;
    typedef boost::filesystem::path                 path;
    typedef std::map<key_type, geom_container_ptr>  map_geometries;
    typedef std::map<key_type, camera_cont_ptr>     map_cameras;
    typedef std::map<key_type, sg::appearance_ptr>  map_effects;
    typedef std::map<key_type, light_cont_ptr>      map_lights;
    typedef std::map<opl_string, opl_string>        map_strstr;

    explicit       dae_parser_action    ( );
    virtual       ~dae_parser_action    ( );

    ACTION_RETURN  dispatch             ( const key_type& node );
    void           characters           (       key_type& chars );

    virtual void   start                ( const sg::node_ptr& node );
    virtual void   finish               ( const sg::node_ptr& node );

    void           push                 ( e_dae_node          node_type,
                                          const key_type    & name = L"");
    void           pop                  ( );
    sg::scene_ptr  get_scene            ( );

    path           get_base_url         ( ) const;
    void           set_base_url         ( const path & base_url );

    void           set_attributes       ( XML_ATTR_TYPE   * attributes );

protected:
    // Tools

    void           node_found           ( );

    void           add                  ( geom_container_ptr & node,
                                          const key_type     & name);
    void           add                  ( camera_cont_ptr    & node,
                                          const key_type     & name);

    e_dae_node     node_type            ( int                  node_idx );
    e_dae_node     node_back_type       ( int                  node_back_idx );
    dae_node     * node_back            ( int                  node_back_idx );
    void           build_node_matrix    ( opl::matrixf       & matrix );

    opl::m_float * init_m_float         (sg::mesh_ptr        & mesh,
                                         const opl_string    & field,
                                         int                   components);

    void           parse_geom_fa        (dae_node            & float_arr,
                                         dae_node            & source,
                                         dae_node            & geom,
                                         key_type            & chars);
    void           parse_geom_vert      (dae_node            & input,
                                         dae_node            & vertices,
                                         dae_node            & geom);
    void           parse_poly_in        (dae_node            & input,
                                         dae_node            & type_node,
                                         dae_node            & geom);
    void           add_polygon          (dae_node            & geom,
                                         dae_node            & polygons,
                                         key_type            & chars);
    void           move                 (dae_node            & node,
                                         key_type            & chars);
    void           rotate               (dae_node            & node,
                                         key_type            & chars);
    void           scale                (dae_node            & node,
                                         key_type            & chars);
    void           add_camera_param     (dae_node            & camera,
                                         dae_node            & param,
                                         key_type            & chars);
    void           add_instance         (dae_node            & instance);
    void           add_geom             (opl::matrixf        & node_matrix,
                                         geom_container_ptr  & geom,
                                         sg::m_spatial_ptr   * children);
    void           set_light            (dae_node            & light,
                                         dae_node            & param_type,
                                         key_type            & chars);
    light_cont_ptr get_light            (key_type            & name,
                                         bool                  allow_create = false);

    void           parse_effect         (dae_node            & effect_node,
                                         dae_node            & type_node,
                                         dae_node            * chars_node,
                                         opl::wstring        * chars);
    void           parse_effect         (sg::appearance_ptr  & app,
                                         dae_node            & type_node,
                                         opl::wstring        & chars);
    void           parse_shader         (dae_node            & effect_node,
                                         sg::appearance_ptr  & app,
                                         dae_node            & type_node,
                                         dae_node            & chars_node,
                                         opl::wstring        & chars);
    shader_part_ptr shader_part         (const opl::wstring  & shader,
                                         const opl::wstring  & type,
                                         bool                  source = false );
    void           parse_shader_code    (sg::shader_ptr      & shader,
                                         const opl::wstring  & code);
    void           set_mesh             (shape_cont_ptr      & cont,
                                         sg::mesh_ptr        & mesh);
    void           set_material         (shape_cont_ptr      & cont,
                                         sg::shape_ptr       & shape);
private:
    // Variables

    container               dispatch_;      //!< XML nodes dispatcher (map key to func)
    path                    base_url_;      //!< Base path to document
    sg::scene_ptr           scene_;         //!< Scene representation

    std::vector<dae_node>   nodes_;         //!< Nodes hierarchy
    map_geometries          geometry_;      //!< Library of geometry
    map_cameras             cameras_;       //!< Library of cameras
    map_strstr              teximgs_;       //!< Map texture name(key) -> texture url (value)
    map_strstr              mateff_;        //!< Map material ID(key) -> effect url (value)
    map_effects             effects_;       //!< Library of effects
    map_lights              lights_;        //!< Library of lights

    bool                    is_viewpnt_set_;//!< true if viewpoint already set

public:
    XML_ATTR_TYPE         * attributes_;
};

//
//-------------------------------- Inlines -----------------------------------

inline void dae_parser_action::add( geom_container_ptr &n, const key_type &name )
    { geometry_.insert(map_geometries::value_type(name, n)); }
inline void dae_parser_action::add( camera_cont_ptr    &n, const key_type &name )
    { cameras_.insert(map_cameras::value_type(name, n)); }
inline sg::scene_ptr dae_parser_action::get_scene(void)
    { return scene_;             }
inline void dae_parser_action::set_base_url( const dae_parser_action::path& base_url )
    { base_url_ = base_url;      }
inline void dae_parser_action::set_attributes( XML_ATTR_TYPE *attributes )
    { attributes_ = attributes;  }

} } } }

#endif
