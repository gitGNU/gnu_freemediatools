#ifndef JAHLOADXML_H_INCLUDED
#define JAHLOADXML_H_INCLUDED

#include <string>
#include <map>
#include <list>
#include <vector>

//for layer support
#include "coreobject.h"

//for effect support
#include "coreeffects.h"

static const int NUM_OPENGL_BLENDMODES = 8;


class SceneFileXml
{
public:
    enum KEYWORD
    {
        ASSET,
        BACKDROP,
        BUILTIN,
        CAMERA,
        COLOR,
        EFFECT,
        EXPORT,
        FRAMES,
        KEY,
        KEY_ASSET,
        KEYFRAME,
        KEYFRAMES,
        JAHSHAKA,
        LAYER,
        LIGHT,
        MODULE,
        OBJECT,
        PARTICLE,
        PLUGIN,
        PROPERTIES,
        SCENE_FILE,
        TYPE,
        TEXT,

        NO_KEYWORD
    };

    struct SCENE_FILE_STRUCTURE
    {
        std::string     name;
    };

    struct JAHSHAKA_STRUCTURE
    {
        std::string     jah_version;
        std::string     xml_file_version;
    };

    struct MODULE_STRUCTURE
    {
        std::string     name;
    };

    struct FRAMES_STRUCTURE
    {
        std::string     resolution;
        int             code;
        int             number;
    };

    struct OBJECT_STRUCTURE
    {
        std::string     filename;
        std::string     extension;
    };

    struct PROPERTIES_STRUCTURE
    {
        bool            blend;
        bool            depth;
        bool            cull;
        bool            lighting;
        bool            translate_first;
        bool            foreground;
        bool            smooth;
        bool            reflect;
    };

    struct PARTICLE_STRUCTURE
    {
        std::string     type;
        std::string     draw;
        bool            color;
    };

    struct KEY_STRUCTURE
    {
        bool            invert;
        bool            draw;
        bool            status;
        int             keyinframe;
        int             keyoutframe;
        bool            use_open_gl_keyer;
    };

    struct TYPE_STRUCTURE
    {
        std::string             layertype;
        std::string             objtype;
        std::string             composite_type;
    };

    struct BACKDROP_STRUCTURE
    {
        bool            grid_setting;
        bool            aliasing;
        bool            text_safe;
    };

    struct KEYFRAME_STRUCTURE
    {
        int             TC;
        float           SX;
        float           SY;
        float           SZ;
        float           TX;
        float           TY;
        float           TZ;
        float           RX;
        float           RY;
        float           RZ;
        float           AL;
        float           extrude;
        bool            layer_enabled;
        bool            found_lighting_settings;
        float           ambient_red;
        float           ambient_green;
        float           ambient_blue;
        float           diffuse_red;
        float           diffuse_green;
        float           diffuse_blue;
        float           specular_red;
        float           specular_green;
        float           specular_blue;
    };

    struct KEYFRAMES_STRUCTURE
    {
        int                                 count;
        std::vector<KEYFRAME_STRUCTURE>     keyframe_vector;
    };

    struct LAYER_STRUCTURE
    {
        std::string                     name;
        bool                            activation;
        KEYFRAMES_STRUCTURE             keyframes;
    };

    struct LIGHT_STRUCTURE
    {
        bool                            enabled;
        KEYFRAMES_STRUCTURE             keyframes;
    };

    struct CAMERA_STRUCTURE
    {
        std::vector<KEYFRAME_STRUCTURE>     keyframe_vector;
    };

    struct BUILTIN_STRUCTURE
    {
        std::string     name;
        std::string     type;
    };

    struct PLUGIN_STRUCTURE
    {
        std::string     name;
        std::string     guid;
    };

    struct EFFECT_STRUCTURE
    {
        EffectInfo::EFFECT_CATEGORY     category;
//        EffectInfo::EFFECT_CATEGORY     category;
        std::string                     name;
        BUILTIN_STRUCTURE               builtin;
        PLUGIN_STRUCTURE                plugin;
        KEYFRAMES_STRUCTURE             keyframes;
    };

    struct ASSET_STRUCTURE
    {
        bool                    layer_status;
        VideoCategory::TYPE     category;
        VideoType::TYPE         type;
        std::string             clipname;
        int                     clipnumber;
        std::string             location;
        std::string             filename;
        std::string             extension;
        int                     startframe;
        int                     endframe;
        int                     clipinframe;
        int                     clipoutframe;
        std::string             videoext;
        bool                    videotex;
        int                     slip;
        int                     inframe_number;
        int                     outframe_number;
        bool                    showedges;
        bool                    loop;
        bool                    ping;
        int                     xpos;
        int                     ypos;
    };

    struct KEY_ASSET_STRUCTURE
    {   
        bool                    layer_status;
        VideoCategory::TYPE     category;
        VideoType::TYPE         type;
        std::string             clipname;
        int                     clipnumber;
        std::string             location;
        std::string             filename;
        std::string             extension;
        int                     startframe;
        int                     endframe;
        int                     clipinframe;
        int                     clipoutframe;
        std::string             videoext;
        bool                    videotex;
        std::string             blendmode;
        int                     slip;
        int                     inframe_number;
        int                     outframe_number;
        int                     xpos;
        int                     ypos;
    };

    struct COLOR_STRUCTURE
    {
        float4                  value;
    };

    struct TEXT_STRUCTURE
    {
        std::string             text;
        std::string             font;
        FontDrawstyle::TYPE     style;
        float                   extrude_depth;
    };


    int             getCurrentStringPosition() { return m_current_string_position; }
    void            setCurrentStringPosition(int position) { m_current_string_position = position; }
    JahLayer*       getCurrentJahLayer() { return m_current_jah_layer; }
    void            setCurrentJahLayer(JahLayer* layer) { m_current_jah_layer = layer; }

    int             goToNextKnownKeywordPosition(KEYWORD known_keyword);
    int             goToNextUnknownKeywordPosition(KEYWORD& unknown_keyword);
    int             goToNextKeywordPosition(KEYWORD& keyword);
    int             findNextKnownKeywordPosition(KEYWORD known_keyword);
    int             findNextUnknownKeywordPosition(KEYWORD& unknown_keyword);
    int             findNextKeywordPosition(KEYWORD& keyword, bool go_to = false);
    int             findNextKeywordPosition(std::string& keyword_string);
    int             findNextKeywordPosition();

    bool            parseJahshakaStructure(JAHSHAKA_STRUCTURE& jahshaka_structure);
    bool            parseSceneFileStructure(SCENE_FILE_STRUCTURE& scene_file_structure);
    bool            parseModuleStructure(MODULE_STRUCTURE& module_structure);
    bool            parseFramesStructure(FRAMES_STRUCTURE& frames_structure);
    bool            parseLayerStructure(LAYER_STRUCTURE& layer_structure);
    bool            parseLightStructure(LIGHT_STRUCTURE& light_structure);
    bool            parseObjectStructure(OBJECT_STRUCTURE& object_structure);
    bool            parsePropertiesStructure(PROPERTIES_STRUCTURE& properties_structure);
    bool            parseKeyStructure(KEY_STRUCTURE& key_structure);
    bool            parseTypeStructure(TYPE_STRUCTURE& type_structure);
    bool            parseBackdropStructure(BACKDROP_STRUCTURE& backdrop_structure);
    bool            parseKeyframeStructure(KEYFRAME_STRUCTURE& keyframe_structure);
    bool            parseCameraStructure(CAMERA_STRUCTURE& camera_structure);
    bool            parseKeyframesStructure(KEYFRAMES_STRUCTURE& keyframes_structure);
    bool            parseBuiltinStructure(BUILTIN_STRUCTURE& builtin_structure);
    bool            parsePluginStructure(PLUGIN_STRUCTURE& plugin_structure);
    bool            parseEffectStructure(EFFECT_STRUCTURE& effect_structure);
    bool            parseAssetStructure(ASSET_STRUCTURE& asset_structure);
    bool            parseKeyAssetStructure(KEY_ASSET_STRUCTURE& key_asset_structure);
    bool            parseColorStructure(COLOR_STRUCTURE& color_structure);
    bool            parseTextStructure(TEXT_STRUCTURE& text_structure);
    bool            parseParticleStructure(PARTICLE_STRUCTURE& particle_structure);

    bool            parseToken(std::string& name, int& begin, int& end);
    bool            parseStringToken(std::string& value, std::string& token);
    bool            parseBoolToken(bool& value, std::string& token);
    bool            parseIntToken(int& value, std::string& token);
    bool            parseFloatToken(float& value, std::string& token);

    bool            convertCarriageReturns(std::string& text_string);

    bool            setKeyframes(JahLayer* jah_layer, KEYFRAMES_STRUCTURE& keyframes_structure);

    std::string*    getSceneFileString() { return &m_scene_file_string; }
    std::string&    getKeywordString(KEYWORD keyword);

    static std::vector<std::string>*    getKeywordStringVector() { return &m_keyword_string_vector; }
    static std::string&                 getBlendmodeString(int index) { return m_blendmode_string_map[index]; }
    static unsigned int                 getBlendmodeValue(std::string& blendmode);

    SceneFileXml(std::string scene_file_string);

    SCENE_FILE_STRUCTURE&       getSceneFileStructure() { return m_scene_file_structure; }
    JAHSHAKA_STRUCTURE&         getJahshakaStructure() { return m_jahshaka_structure; }
    MODULE_STRUCTURE&           getModuleStructure() { return m_module_structure; }
    FRAMES_STRUCTURE&           getFramesStructure() { return m_frames_structure; }
    LAYER_STRUCTURE&            getLayerStructure() { return m_layer_structure; }
    OBJECT_STRUCTURE&           getObjectStructure() { return m_object_structure; }
    PROPERTIES_STRUCTURE&       getPropertiesStructure() { return m_properties_structure; }
    KEY_STRUCTURE&              getKeyStructure() { return m_key_structure; }
    TYPE_STRUCTURE&             getTypeStructure() { return m_type_structure; }
    BACKDROP_STRUCTURE&         getBackdropStructure() { return m_backdrop_structure; }
    KEYFRAME_STRUCTURE&         getKeyframeStructure() { return m_keyframe_structure; }
    CAMERA_STRUCTURE&           getCameraStructure() { return m_camera_structure; }
    KEYFRAMES_STRUCTURE&        getKeyframesStructure() { return m_keyframes_structure; }
    BUILTIN_STRUCTURE&          getBuiltinStructure() { return m_builtin_structure; }
    PLUGIN_STRUCTURE&           getPluginStructure() { return m_plugin_structure; }
    EFFECT_STRUCTURE&           getEffectStructure() { return m_effect_structure; }
    ASSET_STRUCTURE&            getAssetStructure() { return m_asset_structure; }
    KEY_ASSET_STRUCTURE&        getKeyAssetStructure() { return m_key_asset_structure; }
    COLOR_STRUCTURE&            getColorStructure() { return m_color_structure; }
    TEXT_STRUCTURE&             getTextStructure() { return m_text_structure; }
    PARTICLE_STRUCTURE&         getParticleStructure() { return m_particle_structure; }
    LIGHT_STRUCTURE&            getLightStructure() { return m_light_structure; }

protected:
    int             m_current_string_position;       
    JahLayer*       m_current_jah_layer;
    std::string     m_scene_file_string;

    SCENE_FILE_STRUCTURE        m_scene_file_structure;
    JAHSHAKA_STRUCTURE          m_jahshaka_structure;
    MODULE_STRUCTURE            m_module_structure;
    FRAMES_STRUCTURE            m_frames_structure;
    LAYER_STRUCTURE             m_layer_structure;
    LIGHT_STRUCTURE             m_light_structure;
    PROPERTIES_STRUCTURE        m_properties_structure;
    OBJECT_STRUCTURE            m_object_structure;
    KEY_STRUCTURE               m_key_structure;
    TYPE_STRUCTURE              m_type_structure;
    BACKDROP_STRUCTURE          m_backdrop_structure;
    KEYFRAME_STRUCTURE          m_keyframe_structure;
    CAMERA_STRUCTURE            m_camera_structure;
    KEYFRAMES_STRUCTURE         m_keyframes_structure;
    BUILTIN_STRUCTURE           m_builtin_structure;
    PLUGIN_STRUCTURE            m_plugin_structure;
    EFFECT_STRUCTURE            m_effect_structure;
    ASSET_STRUCTURE             m_asset_structure;
    KEY_ASSET_STRUCTURE         m_key_asset_structure;
    COLOR_STRUCTURE             m_color_structure;
    TEXT_STRUCTURE              m_text_structure;
    PARTICLE_STRUCTURE          m_particle_structure;

    static std::vector<std::string>             m_keyword_string_vector;
    static std::map<unsigned int,std::string>   m_blendmode_string_map;
};

#endif // JAHLOADXML_H_INCLUDED
