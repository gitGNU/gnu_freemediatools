/*******************************************************************************
**
** The source file for the Jahshaka 
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/
#ifdef _MSC_VER
#pragma warning (disable : 4786)
#endif

#include <assert.h>
#include <iostream>

//#include "glworld.h"

#include "jahloadxml.h"



int
SceneFileXml::findNextKeywordPosition(std::string& keyword_string)
{
    KEYWORD keyword;

    for (int i = 0; i < NO_KEYWORD; i++)
    {
        if (m_keyword_string_vector[i] == keyword_string)
        {
            keyword = KEYWORD(i);
        }
    }

    return findNextKeywordPosition(keyword, false);
}

int
SceneFileXml::goToNextUnknownKeywordPosition(KEYWORD& unknown_keyword)
{
    unknown_keyword = NO_KEYWORD;
    int position = goToNextKeywordPosition(unknown_keyword);
    return position;
}

int
SceneFileXml::goToNextKnownKeywordPosition(KEYWORD known_keyword)
{
    KEYWORD keyword = known_keyword;
    int position = goToNextKeywordPosition(keyword);
    return position;
}

int
SceneFileXml::goToNextKeywordPosition(KEYWORD& keyword)
{
    int position = findNextKeywordPosition(keyword, true);
    return position;
}

int
SceneFileXml::findNextUnknownKeywordPosition(KEYWORD& unknown_keyword)
{
    unknown_keyword = NO_KEYWORD;
    int position = findNextKeywordPosition(unknown_keyword, false);
    return position;
}

int
SceneFileXml::findNextKnownKeywordPosition(KEYWORD known_keyword)
{
    KEYWORD keyword = known_keyword;
    int position = findNextKeywordPosition(keyword, false);
    return position;
}

int
SceneFileXml::findNextKeywordPosition(KEYWORD& keyword, bool go_to)
{
    int position = (int)getSceneFileString()->find("<", getCurrentStringPosition() );

    if (position == (int)std::string::npos)
    {
        return position;
    }

    if (keyword != NO_KEYWORD)
    {
        // The caller is looking for a specific keyword
        position = (int)getSceneFileString()->find( "<" + getKeywordString( KEYWORD(keyword) ), 
                                               getCurrentStringPosition() );

        if (go_to)
        {
            setCurrentStringPosition(position + 1);
        }

        return position;
    }

    // The caller is looking for any keyword
    int closest = INT_MAX;

    for ( int keyword_number = 0; keyword_number < NO_KEYWORD; keyword_number++)
    {
        KEYWORD keyword_type = KEYWORD(keyword_number);
        position = (int)getSceneFileString()->find( "<" + getKeywordString(keyword_type),  
                                               getCurrentStringPosition() );

        if (position != (int)std::string::npos && position <= closest)
        {
            closest = position;
            keyword = keyword_type;
        }
    }

    if (closest == INT_MAX)
    {
        return int(std::string::npos);
    }

    if (go_to)
    {
        setCurrentStringPosition(closest + 1);
    }

    return closest;
}

std::string&    
SceneFileXml::getKeywordString(KEYWORD keyword)
{
    if (keyword < NO_KEYWORD)
    {
        return m_keyword_string_vector[keyword];
    }

    return m_keyword_string_vector[NO_KEYWORD];
}

std::vector<std::string> SceneFileXml::m_keyword_string_vector(SceneFileXml::NO_KEYWORD + 1);
std::map<unsigned int,std::string> SceneFileXml::m_blendmode_string_map;

SceneFileXml::SceneFileXml(std::string scene_file_string)
{
    m_current_string_position = 0;
    m_scene_file_string = scene_file_string;
    m_effect_structure.builtin.type = "";

    m_keyword_string_vector[FREEMEDIATOOLS] = "FREEMEDIATOOLS";
    m_keyword_string_vector[SCENE_FILE] = "SCENE_FILE";
    m_keyword_string_vector[MODULE] = "MODULE";
    m_keyword_string_vector[EXPORT] = "EXPORT";
    m_keyword_string_vector[FRAMES] = "FRAMES";
    m_keyword_string_vector[LAYER] = "LAYER";
    m_keyword_string_vector[OBJECT] = "OBJECT";
    m_keyword_string_vector[PROPERTIES] = "PROPERTIES";
    m_keyword_string_vector[KEY] = "KEY";
    m_keyword_string_vector[LIGHT] = "LIGHT";
    m_keyword_string_vector[TYPE] = "TYPE";
    m_keyword_string_vector[ASSET] = "ASSET";
    m_keyword_string_vector[KEY_ASSET] = "KEYASSET";
    m_keyword_string_vector[BACKDROP] = "BACKDROP";
    m_keyword_string_vector[CAMERA] = "CAMERA";
    m_keyword_string_vector[KEYFRAMES] = "KEYFRAMES";
    m_keyword_string_vector[KEYFRAME] = "KEYFRAME";
    m_keyword_string_vector[EFFECT] = "EFFECT";
    m_keyword_string_vector[PLUGIN] = "PLUGIN";
    m_keyword_string_vector[BUILTIN] = "BUILTIN";
    m_keyword_string_vector[TEXT] = "TEXT";
    m_keyword_string_vector[COLOR] = "COLOR";
    m_keyword_string_vector[PARTICLE] = "PARTICLE";
    m_keyword_string_vector[NO_KEYWORD] = "NO_KEYWORD";

    m_blendmode_string_map[GL_ZERO] = "GL_ZERO";
    m_blendmode_string_map[GL_ONE] = "GL_ONE";
    m_blendmode_string_map[GL_SRC_COLOR] = "GL_SRC_COLOR";
    m_blendmode_string_map[GL_ONE_MINUS_SRC_COLOR] = "GL_ONE_MINUS_SRC_COLOR";
    m_blendmode_string_map[GL_SRC_ALPHA] = "GL_SRC_ALPHA";
    m_blendmode_string_map[GL_ONE_MINUS_SRC_ALPHA] = "GL_ONE_MINUS_SRC_ALPHA";
    m_blendmode_string_map[GL_DST_ALPHA] = "GL_DST_ALPHA";
    m_blendmode_string_map[GL_ONE_MINUS_DST_ALPHA] = "GL_ONE_MINUS_DST_ALPHA";
    m_blendmode_string_map[GL_DST_COLOR] = "GL_DST_COLOR";
    m_blendmode_string_map[GL_ONE_MINUS_DST_COLOR] = "GL_ONE_MINUS_DST_COLOR";
    m_blendmode_string_map[GL_SRC_ALPHA_SATURATE] = "GL_SRC_ALPHA_SATURATE";
}

bool
SceneFileXml::parseJahshakaStructure(JAHSHAKA_STRUCTURE& jahshaka_structure)
{
    std::string tmp;
    parseStringToken(jahshaka_structure.jah_version, tmp = "fmt_version" );
    parseStringToken(jahshaka_structure.xml_file_version, tmp = "xml_file_version" );
    return true;
}

bool
SceneFileXml::parseSceneFileStructure(SCENE_FILE_STRUCTURE& scene_file_structure)
{
    std::string tmp;
    parseStringToken(scene_file_structure.name, tmp = "name" );
    return true;
}

bool
SceneFileXml::parseModuleStructure(MODULE_STRUCTURE& module_structure)
{
    std::string tmp;
    parseStringToken(module_structure.name, tmp = "name" );
    return true;
}

bool
SceneFileXml::parseFramesStructure(FRAMES_STRUCTURE& frames_structure)
{
    std::string tmp;
    parseStringToken(frames_structure.resolution, tmp = "resolution" );
    parseIntToken(frames_structure.code, tmp = "code" );
    parseIntToken(frames_structure.number  , tmp = "number" );
    return true;
}

bool            
SceneFileXml::parseLayerStructure(LAYER_STRUCTURE& layer_structure)
{
    std::string tmp;
    parseStringToken(layer_structure.name, tmp = "name" );
    parseBoolToken(layer_structure.activation, tmp = "activation" );
    return true;
}

bool            
SceneFileXml::parsePropertiesStructure(PROPERTIES_STRUCTURE& properties_structure)
{
    std::string tmp;
    parseBoolToken(properties_structure.blend, tmp = "blend" );
    parseBoolToken(properties_structure.depth, tmp = "depth" );
    parseBoolToken(properties_structure.cull, tmp = "cull" );
    parseBoolToken(properties_structure.lighting, tmp = "lighting" );

    if ( !parseBoolToken(properties_structure.translate_first, tmp = "translate_first" ) )
    {
        properties_structure.translate_first = true;
    }

    parseBoolToken(properties_structure.foreground, tmp = "foreground" );
    parseBoolToken(properties_structure.smooth, tmp = "smooth" );
    parseBoolToken(properties_structure.reflect, tmp = "reflect" );
    return true;
}

bool
SceneFileXml::parseKeyStructure(KEY_STRUCTURE& key_structure)
{
    std::string tmp;
    parseBoolToken(key_structure.invert, tmp = "invert" );
    parseBoolToken(key_structure.draw, tmp = "draw" );
    parseBoolToken(key_structure.status, tmp = "status" );
    parseIntToken(key_structure.keyinframe, tmp = "keyinframe" );
    parseIntToken(key_structure.keyoutframe, tmp = "keyoutframe" );
    parseBoolToken(key_structure.use_open_gl_keyer, tmp = "use_open_gl_keyer" );
    
    return true;
}

bool
SceneFileXml::parseTypeStructure(TYPE_STRUCTURE& type_structure)
{
    std::string tmp;
    parseStringToken(type_structure.layertype, tmp = "layertype" );
    parseStringToken(type_structure.objtype, tmp = "objtype" );
    parseStringToken(type_structure.composite_type, tmp = "composite_type" );
    return true;
}

bool
SceneFileXml::parseBackdropStructure(BACKDROP_STRUCTURE& backdrop_structure)
{
    std::string tmp;
    parseBoolToken(backdrop_structure.grid_setting, tmp = "grid_setting" );
    parseBoolToken(backdrop_structure.aliasing, tmp = "aliasing" );
    parseBoolToken(backdrop_structure.text_safe, tmp = "text_safe" );
    return true;
}

bool
SceneFileXml::parseKeyframeStructure(KEYFRAME_STRUCTURE& keyframe_structure)
{
    std::string tmp;
    parseIntToken(keyframe_structure.TC, tmp = "TC" );
    parseFloatToken(keyframe_structure.SX, tmp = "SX" );
    parseFloatToken(keyframe_structure.SY, tmp = "SY" );
    parseFloatToken(keyframe_structure.SZ, tmp = "SZ" );
    parseFloatToken(keyframe_structure.TX, tmp = "TX" );
    parseFloatToken(keyframe_structure.TY, tmp = "TY" );
    parseFloatToken(keyframe_structure.TZ, tmp = "TZ" );
    parseFloatToken(keyframe_structure.RX, tmp = "RX" );
    parseFloatToken(keyframe_structure.RY, tmp = "RY" );
    parseFloatToken(keyframe_structure.RZ, tmp = "RZ" );
    parseFloatToken(keyframe_structure.AL, tmp = "AL" );
    parseFloatToken(keyframe_structure.extrude, tmp = "extrude" );

    if ( !parseBoolToken(keyframe_structure.layer_enabled, tmp = "layer_enabled" ) )
    {
        keyframe_structure.layer_enabled = true;
    }

    if ( parseFloatToken(keyframe_structure.ambient_red, tmp = "ambient_red" ) )
    {
        parseFloatToken(keyframe_structure.ambient_green, tmp = "ambient_green" );
        parseFloatToken(keyframe_structure.ambient_blue, tmp = "ambient_blue" );
        parseFloatToken(keyframe_structure.diffuse_red, tmp = "diffuse_red" );
        parseFloatToken(keyframe_structure.diffuse_green, tmp = "diffuse_green" );
        parseFloatToken(keyframe_structure.diffuse_blue, tmp = "diffuse_blue" );
        parseFloatToken(keyframe_structure.specular_red, tmp = "specular_red" );
        parseFloatToken(keyframe_structure.specular_green, tmp = "specular_green" );
        parseFloatToken(keyframe_structure.specular_blue, tmp = "specular_blue" );
        keyframe_structure.found_lighting_settings = true;
    }
    else
    {
        keyframe_structure.found_lighting_settings = false;
    }

    return true;
}

bool
SceneFileXml::parseCameraStructure(CAMERA_STRUCTURE& )
{
    // Needs to be filled in
    return true;
}

bool
SceneFileXml::parseKeyframesStructure(KEYFRAMES_STRUCTURE& keyframes_structure)
{
    int string_position = getCurrentStringPosition();
    std::string tmp;
    parseIntToken(keyframes_structure.count, tmp = "count" );
    int end = (int)getSceneFileString()->find( "</KEYFRAMES>", getCurrentStringPosition() );
    KEYWORD keyword = KEYFRAME;
    string_position = goToNextKnownKeywordPosition(KEYFRAME);

    while (string_position < end && keyword == KEYFRAME && string_position != (int)std::string::npos)
    {
        KEYFRAME_STRUCTURE* new_keyframe_structure = new KEYFRAME_STRUCTURE;
        parseKeyframeStructure( *new_keyframe_structure );
        keyframes_structure.keyframe_vector.push_back( *new_keyframe_structure );
        string_position = goToNextUnknownKeywordPosition(keyword);
    }

    setCurrentStringPosition(end);
    return true;
}

bool
SceneFileXml::parseBuiltinStructure(BUILTIN_STRUCTURE& builtin_structure)
{
    std::string tmp;
    parseStringToken(builtin_structure.name, tmp = "name" );
    parseStringToken(builtin_structure.type, tmp = "type" );
    return true;
}

bool
SceneFileXml::parsePluginStructure(PLUGIN_STRUCTURE& plugin_structure)
{
    std::string tmp;
    parseStringToken(plugin_structure.name, tmp = "name" );
    parseStringToken(plugin_structure.guid, tmp = "guid" );
    return true;
}

bool
SceneFileXml::parseLightStructure(LIGHT_STRUCTURE& light_structure)
{
    KEYWORD keyword;
    std::string tmp;
    parseBoolToken(light_structure.enabled, tmp = "enabled" );

    int string_position = goToNextUnknownKeywordPosition(keyword);

    if (keyword == KEYFRAMES)
    {
        setCurrentStringPosition(string_position + 1);
        parseKeyframesStructure(light_structure.keyframes);
    }

    return true;
}

bool
SceneFileXml::parseEffectStructure(EFFECT_STRUCTURE& effect_structure)
{
    std::string tmp;
    std::string string_value;
    parseStringToken(string_value, tmp = "category" );
    effect_structure.category = EffectInfo::getCategory(string_value);
    int end = (int)getSceneFileString()->find( "</EFFECT>", getCurrentStringPosition() );

    KEYWORD keyword;
    int string_position = goToNextUnknownKeywordPosition(keyword);

    effect_structure.builtin.type = "NOT_A_TYPE";
    effect_structure.plugin.guid = "";

    while (string_position < end)
    {
        switch (keyword)
        {
            case BUILTIN :
            {
                setCurrentStringPosition(string_position + 1);
                parseBuiltinStructure(effect_structure.builtin);
                effect_structure.name = effect_structure.builtin.name;
                break;
            }
            case PLUGIN :
            {
                setCurrentStringPosition(string_position + 1);
                parsePluginStructure(effect_structure.plugin);
                effect_structure.name = effect_structure.plugin.name;
                break;
            }
            case KEYFRAMES :
            {
                setCurrentStringPosition(string_position + 1);
                parseKeyframesStructure(effect_structure.keyframes);
                break;
            }
			default:
            {
				break;
            }
        }

        string_position = findNextUnknownKeywordPosition(keyword);
    }

    setCurrentStringPosition(end);
    return true;
}
    
bool
SceneFileXml::parseAssetStructure(ASSET_STRUCTURE& asset_structure)
{
    std::string tmp;
    std::string data_string;
    parseBoolToken(asset_structure.layer_status, tmp = "layer_status" );

    parseStringToken(data_string, tmp = "category" );
    asset_structure.category = VideoCategory::getType(data_string);

    parseStringToken(data_string, tmp = "type" );
    asset_structure.type = VideoType::getType(data_string);

    parseStringToken(asset_structure.clipname, tmp = "clipname" );
    parseIntToken(asset_structure.clipnumber, tmp = "clipnumber" );
    parseStringToken(asset_structure.location, tmp = "location" );
    parseStringToken(asset_structure.filename, tmp = "filename" );
    parseStringToken(asset_structure.extension, tmp = "extension" );
    parseIntToken(asset_structure.startframe, tmp = "startframe" );
    parseIntToken(asset_structure.endframe, tmp = "endframe" );
    parseIntToken(asset_structure.clipinframe, tmp = "clipinframe" );
    parseIntToken(asset_structure.clipoutframe, tmp = "clipoutframe" );
    parseStringToken(asset_structure.videoext, tmp = "videoext" );
    parseBoolToken(asset_structure.videotex, tmp = "videotex" );
    parseIntToken(asset_structure.slip, tmp = "slip" );
    parseIntToken(asset_structure.inframe_number, tmp = "inframe_number" );
    parseIntToken(asset_structure.outframe_number, tmp = "outframe_number" );
    parseBoolToken(asset_structure.showedges, tmp = "showedges" );
    parseBoolToken(asset_structure.loop, tmp = "loop" );
    parseBoolToken(asset_structure.ping, tmp = "ping" );
    parseIntToken(asset_structure.xpos, tmp = "xpos" );
    parseIntToken(asset_structure.ypos, tmp = "ypos" );
    return true;
}
bool
SceneFileXml::parseKeyAssetStructure(KEY_ASSET_STRUCTURE& key_asset_structure)
{
    std::string tmp;
    std::string data_string;

    parseStringToken(data_string, tmp = "category" );
    key_asset_structure.category = VideoCategory::getType(data_string);

    parseStringToken(data_string, tmp = "type" );
    key_asset_structure.type = VideoType::getType(data_string);

    parseStringToken(key_asset_structure.clipname, tmp = "clipname" );
    parseIntToken(key_asset_structure.clipnumber, tmp = "clipnumber" );
    parseStringToken(key_asset_structure.location, tmp = "location" );
    parseStringToken(key_asset_structure.filename, tmp = "filename" );
    parseStringToken(key_asset_structure.extension, tmp = "extension" );
    parseIntToken(key_asset_structure.startframe, tmp = "startframe" );
    parseIntToken(key_asset_structure.endframe, tmp = "endframe" );
    parseIntToken(key_asset_structure.clipinframe, tmp = "clipinframe" );
    parseIntToken(key_asset_structure.clipoutframe, tmp = "clipoutframe" );
    parseStringToken(key_asset_structure.videoext, tmp = "videoext" );
    parseBoolToken(key_asset_structure.videotex, tmp = "videotex" );
    parseStringToken(key_asset_structure.blendmode, tmp = "blendmode" );
    parseIntToken(key_asset_structure.slip, tmp = "slip" );
    parseIntToken(key_asset_structure.inframe_number, tmp = "inframe_number" );
    parseIntToken(key_asset_structure.outframe_number, tmp = "outframe_number" );
    parseIntToken(key_asset_structure.xpos, tmp = "xpos" );
    parseIntToken(key_asset_structure.ypos, tmp = "ypos" );
    return true;
}

bool            
SceneFileXml::parseColorStructure(COLOR_STRUCTURE& color_structure)
{
    std::string tmp;
    parseFloatToken(color_structure.value.x, tmp = "red");
    parseFloatToken(color_structure.value.y, tmp = "green");
    parseFloatToken(color_structure.value.z, tmp = "blue");
    return true;
}

bool
SceneFileXml::parseObjectStructure(OBJECT_STRUCTURE& object_structure)
{
    std::string tmp;
    parseStringToken(object_structure.filename, tmp = "filename" );
    parseStringToken(object_structure.extension, tmp = "extension" );
    return true;
}

bool            
SceneFileXml::parseTextStructure(TEXT_STRUCTURE& text_structure)
{
    std::string tmp;
    parseStringToken(text_structure.text, tmp = "text");
    parseStringToken(text_structure.font, tmp = "font");
    std::string style_string;
    parseStringToken(style_string, tmp = "style");
    text_structure.style = FontDrawstyle::getType(style_string);
    parseFloatToken(text_structure.extrude_depth, tmp = "extrude_depth");
    return true;
}
    
bool            
SceneFileXml::parseParticleStructure(PARTICLE_STRUCTURE& particle_structure)
{
    std::string tmp;
    parseStringToken(particle_structure.type, tmp = "type");
    parseStringToken(particle_structure.draw, tmp = "draw");
    parseBoolToken(particle_structure.color, tmp = "color");
    return true;
}
    
bool
SceneFileXml::parseToken(std::string& name, int& begin, int& end)
{
    name += " = ";
    int location = (int)getSceneFileString()->find(name, getCurrentStringPosition() );
    begin = (int)getSceneFileString()->find("'", location) + 1;
    end = (int)getSceneFileString()->find("'", begin);

    if (location != (int)std::string::npos)
    {
        return true;
    }

    return false;
}

bool            
SceneFileXml::convertCarriageReturns(std::string& text_string)
{
    std::string carriage_return = "\n";

    // Replace &#10 with carriage return
    size_t carriage_return_position = text_string.find("&#10", 0);

    while(carriage_return_position != std::string::npos)
    {
        text_string.replace(carriage_return_position, 4, carriage_return);
        carriage_return_position = text_string.find_first_of("&#10", carriage_return_position + 1);
    }

    return true;
}

bool
SceneFileXml::parseStringToken(std::string& value, std::string& token)
{
    int begin;
    int end;
    parseToken(token, begin, end);

    value = getSceneFileString()->substr(begin, end - begin);
    convertCarriageReturns(value);

    return true;
}

bool
SceneFileXml::parseBoolToken(bool& value, std::string& token)
{
    int begin;
    int end;
    bool token_found = parseToken(token, begin, end);
    std::string substring = getSceneFileString()->substr(begin, end - begin);
    value = (substring == "1");

    if (token_found)
    {
        return true;
    }

    return false;
}

bool
SceneFileXml::parseIntToken(int& value, std::string& token)
{
    int begin;
    int end;
    bool token_found = parseToken(token, begin, end);
    std::string substring = getSceneFileString()->substr(begin, end - begin);
    value = strtoul( (const char*)substring.c_str(), NULL, 10);

    if (token_found)
    {
        return true;
    }

    return false;
}

bool
SceneFileXml::parseFloatToken(float& value, std::string& token)
{
    int begin;
    int end;
    bool token_found = parseToken(token, begin, end);
    std::string substring = getSceneFileString()->substr(begin, end - begin);
    value = atof( (const char*)substring.c_str() ) ;

    if (token_found)
    {
        return true;
    }

    return false;
}

bool            
SceneFileXml::setKeyframes(JahLayer* jah_layer, KEYFRAMES_STRUCTURE& keyframes_structure )
{
    std::vector<KEYFRAME_STRUCTURE> keyframe_vector = keyframes_structure.keyframe_vector;

	motionNode* keyframe = jah_layer->layernodes->m_node;

	size_t vector_size = keyframe_vector.size();
    int count = keyframes_structure.count;

    for (size_t i = 0; i < vector_size; i++)
    {
        keyframe->sx = keyframe_vector[i].SX;
        keyframe->sy = keyframe_vector[i].SY;
        keyframe->sz = keyframe_vector[i].SZ;
        keyframe->rx = keyframe_vector[i].RX;
        keyframe->ry = keyframe_vector[i].RY;
        keyframe->rz = keyframe_vector[i].RZ;
        keyframe->tx = keyframe_vector[i].TX;
        keyframe->ty = keyframe_vector[i].TY;
        keyframe->tz = keyframe_vector[i].TZ;
        keyframe->Alpha = keyframe_vector[i].AL;
        keyframe->extrude = keyframe_vector[i].extrude;
        keyframe->layer_enabled = keyframe_vector[i].layer_enabled;

        if (keyframe_vector[i].found_lighting_settings)
        {
            keyframe->ambient_light_color.x = keyframe_vector[i].ambient_red / 255.0f;
            keyframe->ambient_light_color.y = keyframe_vector[i].ambient_green / 255.0f;
            keyframe->ambient_light_color.z = keyframe_vector[i].ambient_blue / 255.0f;
            keyframe->diffuse_light_color.x = keyframe_vector[i].diffuse_red / 255.0f;
            keyframe->diffuse_light_color.y = keyframe_vector[i].diffuse_green / 255.0f;
            keyframe->diffuse_light_color.z = keyframe_vector[i].diffuse_blue / 255.0f;
            keyframe->specular_light_color.x = keyframe_vector[i].specular_red / 255.0f;
            keyframe->specular_light_color.y = keyframe_vector[i].specular_green / 255.0f;
            keyframe->specular_light_color.z = keyframe_vector[i].specular_blue / 255.0f;
        }

        if (count > 0)
        {
            jah_layer->layernodes->addNodes(keyframe_vector[i].TC, keyframe_vector[i].layer_enabled);
        }
    }

    return true;
}

unsigned int                        
SceneFileXml::getBlendmodeValue(std::string& blendmode)
{
    std::map<unsigned int,std::string>::iterator entry = m_blendmode_string_map.begin();

    for (; entry != m_blendmode_string_map.end(); entry++)
    {
        if ( entry->second == blendmode )
        {
            return entry->first;
        }
    }

    return 0;
}

