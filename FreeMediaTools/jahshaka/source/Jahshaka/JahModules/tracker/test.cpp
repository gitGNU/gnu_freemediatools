/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "tracker.h"
#include <glworldlists.h>
#include <glcore.h>
#include <sstream>
#include <InputLCD.h>
#include <supergrangecontrol.h>
#include <jahrender.h>
#include <keyframer.h>
#include <qlistview.h>
#include <jahpreferences.h>
#include <qslider.h>
#include <qlabel.h>

static const char* point_colors[MAX_TRACKING_POINTS] = { "Red", "Green", "Grey", "Yellow" };

static const int BASE_CHECKSUM_TOLERANCE = 160;

void GLTracker::start( )
{
	buildInterface(m_pControls);
	buildInterfaceMenus(m_pLeftController,m_pRightController);
	initializeObjects();

	// Update sliders to reflected m_bSelected fps
	setResolution( JahPrefs::getInstance().getJahResolution() );
}

void GLTracker::initializeObjects()
{    
    m_pJTrace->info( "Initializing GLTracker Class");

    //initialize the keyframe editor
    //we can always move this into the effectinterface.cpp obj

    m_pKeyFramer = new JahKeyframer( this, moduleOptionsUI[3], "keyframer" );
    connectKeyframer();
    m_bHasKeyFramer = true;

    //make the current opengl context current so the textures can be set up properly
    makeCurrent();
        
    ///////////////////////////////////////////////
    //this creates the default world layer using the linked list

    LayerListEntry* new_layer_list_entry = new LayerListEntry("World");
    getLayerList()->append(new_layer_list_entry);
    JahLayer* new_jah_layer = new_layer_list_entry->thelayer;
    new_jah_layer->layerStatus = false; //set the status to false since we dont useit
    new_jah_layer->blend = TRUE;
    new_jah_layer->depth = FALSE;
    new_jah_layer->layerAssetStatus = TRUE;
    new_jah_layer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer";
    new_jah_layer->asset.extension = ".bmp";
    buildLayer(new_jah_layer, "World", LayerCategory::LAYER, ObjectCategory::BACKDROP );

    new_layer_list_entry = new LayerListEntry("Image");
    getLayerList()->append(new_layer_list_entry);
    new_jah_layer = new_layer_list_entry->thelayer;
    new_jah_layer->blend = TRUE;
    new_jah_layer->depth = TRUE;
    new_jah_layer->layerAssetStatus = false;
    new_jah_layer->layerStatus = true;
    new_jah_layer->layernodes->m_node->tz = 0.0;
    new_jah_layer->asset.filename = m_qsJahBasePath+"Pixmaps/jahlayer"; 
    new_jah_layer->asset.extension = ".bmp";
    buildLayer(new_jah_layer, "Image Layer", LayerCategory::LAYER, ObjectCategory::JAHLAYER ); 
    setBaseLayer(new_jah_layer);

    QCheckListItem* new_q_list_view_item = new QCheckListItem(m_pLayerListView, "Image Layer", QCheckListItem::CheckBoxController);
    new_jah_layer->setCheckListItem( new_q_list_view_item );
    new_q_list_view_item->setOn(true);

    int point_texture_width;
    int max_image_width = TRACKER_POINT_TEXTURE_WIDTH;

    getPowerOfTwoTextureSize(point_texture_width, point_texture_width, max_image_width, max_image_width);
    setPointTextureWidth(point_texture_width);

    createTrackerPointLayers();

	changeSelection(1);
    updateSliders( getActiveJahLayer()->layernodes->m_node );

    /////////////////////////////////////////////////////
    //initialize the buffer used for effect outputimage buffering
    //fxbufferexists = false;
    //useBuffering = true;

    //initBuffer(&getLayerList()->at(1)->thelayer->textureData.objectImage);
   // forceupdatefxbuffer = true;

    //turn m_bForcePlay on here
    setForcePlay(true);

    //////////////////////////////////////////////////////
    //set the ui tool
    noTool();

	//some variables
    m_bTexturesInitialized = false;
    m_bUseTexturesForReadback = false;
    m_bGPUSelect = true;
    m_stabilize = false;
    m_nCurrentFrame = 1;
}

void
GLTracker::createTrackerPointLayers()
{
    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        std::ostringstream name;
        name << point_colors[i] << " point";
        std::string name_string = name.str();
        char* layer_name = (char*)( name_string.c_str() );
        LayerListEntry* new_layer_list_entry = new LayerListEntry(layer_name);
        getLayerList()->append(new_layer_list_entry);
        JahLayer* new_jah_layer = new_layer_list_entry->getJahLayer();
        new_jah_layer->layerStatus = true;   
        new_jah_layer->layertype = LayerCategory::TRACKER_POINT;
        new_jah_layer->objtype = ObjectCategory::TRACKER_POINT;
        new_jah_layer->layernodes->m_node->layer_enabled = false;
        QCheckListItem* new_q_check_list_item = addCheckListItem(m_pLayerListView, false); 
        new_jah_layer->setCheckListItem(new_q_check_list_item);
        setPointLayer(i, new_jah_layer);
    
        // Create a texture to store the pattern being tracked
        createEmpty2DTexture(getPointTextureIdPtr(i), GL_RGBA, 
                             getPointTextureWidth(), getPointTextureWidth() );

        m_point_previous_position[i].x = -1.0f;
        m_point_previous_position[i].y = -1.0f;
        m_point_pixel_buffer[i] = 
            new unsigned char[ getPointTextureWidth() * getPointTextureWidth() * 4 ];
        m_point_enabled[i] = false;
    }
}


void GLTracker::objectMenu(int menu)
{
    changeSelection(menu);
}

//////////////////////////////////////////
// need to fix missing command below makeTexture
//////////////////////////////////////////
///////////////////////////////////////////
///////////////////////////////////////////

void GLTracker::addClip(assetData clip)
{
    JahLayer* current_layer = getBaseLayer();

    int endframe = clip.endframe - 1;

    if (endframe > m_nMaxFrame)
    {
        m_nMaxFrame = endframe;
        updatesliderEndframe(m_nMaxFrame);
        m_pEndFrameControl->setValue(m_nMaxFrame);
    }

    if (clip.startframe <= m_nMinFrame)
    {
        // This is a hack.  Should startframe ever be zero?
        if (clip.startframe < 1)
        {
            m_nMinFrame = 1;
        }
        else
        {
            m_nMinFrame = clip.startframe;
        }

        updatesliderStartframe(m_nMinFrame);
        m_pStartFrameControl->setValue(m_nMinFrame);
    }

    current_layer->assignAsset(clip, VideoCategory::IMAGE);
    current_layer->LoadLayerAsset(VideoCategory::IMAGE);
    current_layer->makeTexture();

    //set renderspace imagesize here...
    m_pCore->imagewidth  = current_layer->getImageWidth();
    m_pCore->imageheight = current_layer->getImageHeight();

    //forceupdatefxbuffer = true;

    //initialize the buffer
    //if (useBuffering)
    //{
   //     initBuffer(&current_layer->textureData.objectImage);
   // }

    float2 screen_size;
    screen_size.x = m_pCore->getRenderWidth();
    screen_size.y = m_pCore->getRenderHeight();
    float2 screen_size_div_2 = screen_size / 2.0f;

    float2 image_size;
    image_size.x = static_cast<float>( getBaseLayer()->getTextureData().getImageWidth() );
    image_size.y = static_cast<float>( getBaseLayer()->getTextureData().getImageHeight() );
    float2 image_size_div_2 = image_size / 2.0f;

    m_image_lower_left_corner = screen_size_div_2 - image_size_div_2;
    float2 zero_delta(0.0f);

    // Reset all the tracker points to center position to avoid going problems from a new image size
    for (int point_number = 0; point_number < MAX_TRACKING_POINTS; point_number++)
    {
        m_point_screen_position[point_number] = screen_size_div_2;
        m_point_lower_left_corner_screen_coords[point_number] = getPointScreenPosition(point_number) - MAX_TRACKER_CIRCLE_RADIUS;
        // Subtract one more because max circle diameter is two less than the buffer width
        m_point_lower_left_corner_screen_coords[point_number] -= 1.0f;
        m_point_lower_left_corner_image_coords[point_number] = m_point_lower_left_corner_screen_coords[point_number] - getImageLowerLeftCorner();
        m_point_image_position[point_number] = getPointScreenPosition(point_number) - getImageLowerLeftCorner();
        setPointPreviousPosition( point_number, getPointScreenPosition(point_number) );
        setPointPreviousDelta( point_number, zero_delta );
    }

    updateGL();
    updateSliders( getActiveJahLayer()->layernodes->m_node );
}

GLuint          
GLTracker::getBaseLayerTextureId()
{
    return getBaseLayer()->getTextureData().getTexId();
}

void
GLTracker::setTrackerPoint(int point_number)
{
    float2 screen_size;
    screen_size.x = m_pCore->getRenderWidth();
    screen_size.y = m_pCore->getRenderHeight();
    float2 screen_size_div_2 = screen_size / 2.0f;

    motionNode* sliders = getPointLayer(point_number)->layernodes->m_node;
    m_point_screen_position[point_number].x = jahstd::Roundf(sliders->rx + screen_size_div_2.x);
    m_point_screen_position[point_number].y = jahstd::Roundf(sliders->ry + screen_size_div_2.y);

    m_point_lower_left_corner_screen_coords[point_number] = getPointScreenPosition(point_number) - MAX_TRACKER_CIRCLE_RADIUS;
    // Subtract one more because max circle diameter is two less than the buffer width
    m_point_lower_left_corner_screen_coords[point_number] -= 1.0f;

    m_point_lower_left_corner_image_coords[point_number] = m_point_lower_left_corner_screen_coords[point_number] - getImageLowerLeftCorner();
    m_point_image_position[point_number] = getPointScreenPosition(point_number) - getImageLowerLeftCorner();

    float point_radius = ( (sliders->rz / 200.0f) * MAX_TRACKER_CIRCLE_RADIUS ) + 5.0f;
    point_radius = jahstd::Min<float>(point_radius, MAX_TRACKER_CIRCLE_RADIUS);
    setPointRadius(point_number, point_radius);

    float search_radius = ( (sliders->tx / 500.0f) * MAX_TRACKER_TRACKING_RADIUS ) + 3.0f;
    search_radius = jahstd::Min<float>(search_radius, MAX_TRACKER_TRACKING_RADIUS);
    setPointSearchRadius(point_number, search_radius);

    float sensitivity = (sliders->ty / 20.0f) + 10.0f;
    setPointSensitivity(point_number, sensitivity);

    int checksum_tolerance = int(sliders->tz / 5.0f) + BASE_CHECKSUM_TOLERANCE;
    setPointChecksumTolerance(point_number, checksum_tolerance);

    capturePointPattern(point_number);

    glBindTexture(GL_TEXTURE_2D, getPointTextureId(point_number) ); 

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getPointTextureWidth(), getPointTextureWidth(),
        GL_RGBA, GL_UNSIGNED_BYTE, getPointPixelBuffer(point_number) );
}

void
GLTracker::capturePointPattern(int point_number)
{
    if ( jahstd::glslSupport() && m_bNV_GPUSupport && getUseGpu() )
    {
        getBaseLayer()->setTextureCompositeDirty(false);
        m_pRenderSpace->drawLayer( getBaseLayer(), true );

        int lower_left_x = static_cast<int>( getPointLowerLeftCornerScreenCoords(point_number).x );
        int lower_left_y = static_cast<int>( getPointLowerLeftCornerScreenCoords(point_number).y );

        glReadPixels(
            lower_left_x,
            lower_left_y,
            getPointTextureWidth(),
            getPointTextureWidth(),
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            getPointPixelBuffer(point_number) );
    }
    else
    {
        QImage qimage_flipped = getBaseLayer()->textureData.objectImage.mirror(false, true);

        float2 rectangle_lower_left_corner = getPointLowerLeftCornerImageCoords(point_number);
        int lower_left_x = static_cast<int>(rectangle_lower_left_corner.x);
        int lower_left_y = static_cast<int>(rectangle_lower_left_corner.y);

        int i = 0;

        if (   lower_left_x >= qimage_flipped.width()
            || lower_left_x < 0
            || lower_left_y >= qimage_flipped.height()
            || lower_left_y < 0
           )
        {
            return;
        }

        for (int row = lower_left_y; row < lower_left_y + getPointTextureWidth(); row++)
        {
            for (int column = lower_left_x; column < lower_left_x + getPointTextureWidth(); column++)
            {
                if ( row < qimage_flipped.height() && column < qimage_flipped.width() )
                {
                    QRgb* pixel_pointer = (QRgb*)( qimage_flipped.scanLine(row) ) + column;
                    getPointPixelBuffer(point_number)[i + 0] = qRed(*pixel_pointer);
                    getPointPixelBuffer(point_number)[i + 1] = qGreen(*pixel_pointer);
                    getPointPixelBuffer(point_number)[i + 2] = qBlue(*pixel_pointer);
                    getPointPixelBuffer(point_number)[i + 3] = qAlpha(*pixel_pointer);
                    i += 4;
                    //int red = *(pixel_pointer + 2);
                    //int green = *(pixel_pointer + 1);
                    //int blue = *(pixel_pointer + 0);
                }
            }
        }
    }
}

void
GLTracker::captureTrackerPoint(int)
{
    int point_number = getSelectedTrackerPointNumber();

    if (point_number >= 0)
    {
        setTrackerPoint(point_number);
        float2 point_screen_position = getPointScreenPosition(point_number);
        setPointInitialScreenPosition(point_number, point_screen_position);
        setPointReferenceFrame( point_number, getCurrentFrame() );
        setPointReferenceScreenPosition( point_number, getPointScreenPosition(point_number) );

        setDontTrack(true);
        updateGL();
        setDontTrack(false);
    }
}

int             
GLTracker::getSelectedTrackerPointNumber()
{
    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        if ( getPointLayer(i) == getActiveJahLayer() )
        {
            return i;
        }
    }

    return -1;
}

void            
GLTracker::showSliders(bool flag)
{
    if (flag)
    {
        m_x_position_slider->show();
        m_y_position_slider->show();
        m_point_radius_slider->show();
        m_point_search_radius_slider->show();
        m_point_sensitivity_slider->show();
        m_point_checksum_tolerance_slider->show();

        m_x_position_lcd->show();
        m_y_position_lcd->show();
        m_point_radius_lcd->show();
        m_point_search_radius_lcd->show();
        m_point_sensitivity_lcd->show();
        m_point_checksum_tolerance_lcd->show();

        m_x_position_label->show();
        m_y_position_label->show();
        m_point_radius_label->show();
        m_point_search_radius_label->show();
        m_point_sensitivity_label->show();
        m_point_checksum_tolerance_label->show();
    }
    else
    {
        m_x_position_slider->hide();;
        m_y_position_slider->hide();;
        m_point_radius_slider->hide();;
        m_point_search_radius_slider->hide();;
        m_point_sensitivity_slider->hide();;
        m_point_checksum_tolerance_slider->hide();;

        m_x_position_lcd->hide();;
        m_y_position_lcd->hide();;
        m_point_radius_lcd->hide();;
        m_point_search_radius_lcd->hide();;
        m_point_sensitivity_lcd->hide();;
        m_point_checksum_tolerance_lcd->hide();;

        m_x_position_label->hide();;
        m_y_position_label->hide();;
        m_point_radius_label->hide();;
        m_point_search_radius_label->hide();;
        m_point_sensitivity_label->hide();;
        m_point_checksum_tolerance_label->hide();;
    }
}

void 
GLTracker::layerClicked( QListViewItem* item )
{
    if (!item) 
    {
        return;
    }

    LayerListEntry* layer_list_entry = findLayerListEntry(item);

    if (!layer_list_entry)
    {
        return;
    }

	JahLayer* jah_layer = layer_list_entry->thelayer;
    setActiveJahLayer(jah_layer);
    QCheckListItem* check_list_item = dynamic_cast<QCheckListItem*>(item);

    if ( jah_layer == getBaseLayer() )
    {
        if (check_list_item)
        {
            // Shouldn't ever happen
            check_list_item->setOn(true);
        }

        jah_layer->layernodes->m_node->layer_enabled = true;
        jah_layer->layerStatus = true;
        showSliders(false);
    }
    else
    {
        if (check_list_item)
        {
            jah_layer->layernodes->m_node->layer_enabled = check_list_item->isOn();
            jah_layer->layerStatus = check_list_item->isOn();
        }
        else
        {
            // Shouldn't ever happen
            jah_layer->layernodes->m_node->layer_enabled = true;
            jah_layer->layerStatus = true;
        }

        showSliders(true);
    }

    setDontTrack(true);
    int tracker_point = getSelectedTrackerPointNumber();

    if (tracker_point >= 0 && tracker_point < MAX_TRACKING_POINTS)
    {
        setTrackerPoint(tracker_point);
    }

    updateGL();
    setDontTrack(false);

    //clicked on layer, update keyframes
    //should only do this if the layer has changed...
    updateKeyframeDisplay();

    //now update the sliders
    updateSliders( getActiveJahLayer()->layernodes->m_node );
}

void 
TrackerData::setPointImagePosition(float2* position)
{
    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        m_point_image_position[i] = position[i]; 
    }
}

void 
TrackerData::setPointEnabled(bool* enabled)
{
    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        m_point_enabled[i] = enabled[i]; 
    }
}

TrackerData::TrackerData(int frame_number, float2* position, bool* enabled)
{
    m_frame_number = frame_number;

    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        m_point_image_position[i] = position[i]; 
        m_point_enabled[i] = enabled[i];
    }
}

int             
GLTracker::getNumberOfEnabledPoints()
{
    int count = 0;

    for (int i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        if ( getPointEnabled(i) )
        {
            count++;
        }
    }

    return count;
}



// 
void            GLTracker::setBaseLayer(JahLayer* layer)
 { 
   m_pBaseLayer = layer;
 }


    JahLayer*       GLTracker::getPointLayer(int index) { return m_aPointLayers[index]; }
    void			GLTracker::setPointLayer(int index, JahLayer* layer) { m_aPointLayers[index] = layer; }

    void            GLTracker::setPointEnabled(int index, bool value) { m_point_enabled[index] = value; }
    float2&         GLTracker::getPointLowerLeftCornerScreenCoords(int point_number) { return m_point_lower_left_corner_screen_coords[point_number]; }
    void            GLTracker::setPointLowerLeftCornerScreenCoords(int point_number, float2& position) { m_point_lower_left_corner_screen_coords[point_number] = position; }



    float2&         GLTracker::getPointLowerLeftCornerImageCoords(int point_number) { return m_point_lower_left_corner_image_coords[point_number]; }
    void            GLTracker::setPointLowerLeftCornerImageCoords(int point_number, float2& position) { m_point_lower_left_corner_image_coords[point_number] = position; }
    float2&         GLTracker::getPointScreenPosition(int point_number) { return m_point_screen_position[point_number]; }
    void            GLTracker::setPointScreenPosition(int point_number, float2& position) { m_point_screen_position[point_number] = position; }
    float2&         GLTracker::getPointScreenPositionUnrounded(int point_number) { return m_point_screen_position_unrounded[point_number]; }
    void            GLTracker::setPointScreenPositionUnrounded(int point_number, float2& position) { m_point_screen_position_unrounded[point_number] = position; }
    float2&         GLTracker::getPointInitialScreenPosition(int point_number) { return m_point_initial_screen_position[point_number]; }
    void            GLTracker::setPointInitialScreenPosition(int point_number, float2& position) { m_point_initial_screen_position[point_number] = position; }
    float2&         GLTracker::getPointImagePosition(int point_number) { return m_point_image_position[point_number]; }
    void            GLTracker::setPointImagePosition(int point_number, float2& position) { m_point_image_position[point_number] = position; }
    float2&         GLTracker::getPointPreviousPosition(int point_number) { return m_point_previous_position[point_number]; }
    void            GLTracker::setPointPreviousPosition(int point_number, float2& position) { m_point_previous_position[point_number] = position; }
    float2&         GLTracker::getPointPreviousDelta(int point_number) { return m_point_previous_delta[point_number]; }
    void            GLTracker::setPointPreviousDelta(int point_number, float2& delta) { m_point_previous_delta[point_number] = delta; }
    int             GLTracker::getPointTextureWidth() { return m_point_texture_width; }
    void            GLTracker::setPointTextureWidth(int width) { m_point_texture_width = width; }
    void            GLTracker::setPointSearchRadius(int point_number, float value) { m_point_search_radius[point_number] = value; }
    float           GLTracker::getPointSearchRadius(int point_number) { return m_point_search_radius[point_number]; }
    float           GLTracker::getPointSearchDiameter(int point_number) { return m_point_search_radius[point_number] * 2.0f; }
    void            GLTracker::setPointRadius(int point_number, float value) { m_point_radius[point_number] = value; }
    float           GLTracker::getPointRadius(int point_number) { return m_point_radius[point_number]; }
    void            GLTracker::setTrackerPoint(int point_number);
    float2&         GLTracker::getImageLowerLeftCorner() { return m_image_lower_left_corner; }
    void            GLTracker::setImageLowerLeftCorner(float2& position) { m_image_lower_left_corner = position; }
    float2&         GLTracker::getPointTimeZeroPosition(int point_number) { return m_point_time_zero_position[point_number]; }
    void            GLTracker::setPointTimeZeroPosition(int point_number, float2& position) { m_point_time_zero_position[point_number] = position; }
    float2&         GLTracker::getPointReferenceScreenPosition(int point_number) { return m_point_reference_position[point_number]; }
    void            GLTracker::setPointReferenceScreenPosition(int point_number, float2& position) { m_point_reference_position[point_number] = position; }
    int             GLTracker::getPointReferenceFrame(int point_number) { return m_point_reference_frame[point_number]; }
    void            GLTracker::setPointReferenceFrame(int point_number, int frame) { m_point_reference_frame[point_number] = frame; } 


    int             GLTracker::getNumberOfEnabledPoints();
    bool            GLTracker::getUseGpu() { return m_pGPUSelect; }

    void            GLTracker::glslTracker(JahLayer* base_layer)
{

}




    std::map<int,TrackerData*>*  GLTracker::getKeyframeData() { return &m_keyframe_data; }


    GLTracker::GLTracker( GLCore* m_pCore, const char* name, QHBox* controls,
        QHBox* controller,QHBox* leftcontroller, QHBox* rightcontroller,
        int* globalm_nClipnumber )
        :  GLWorld(m_pCore, name, controller, globalm_nClipnumber )
		, m_pControls( controls )
		, m_pLeftController( leftcontroller )
		, m_pRightController( rightcontroller )
    {
    }

    GLTracker::~GLTracker() {
    }

    JahLayer*               GLTracker::getChromaBaseLayer() { return m_pChromaBaseLayer; }
    JahLayer*               GLTracker::getChromaKeyLayer() { return m_pChromaKeyLayer; }
    JahLayer*               GLTracker::getChromaSettingsLayer() { return m_pChromaSettingsLayer; }
    bool                    GLTracker::getSelectColorsIndividually() { return m_bSelectColorsIndividually; }
    bool                    GLTracker::getGpuSelect() { return m_pGPUSelect; }
    GLuint*                 GLTracker::getBaseTextureIDPtr() { return &m_hBaseTextureID; }
    GLuint                  GLTracker::getBaseTextureID() { return m_hBaseTextureID; }
    GLuint*                 GLTracker::getKeyTextureIDPtr() { return &m_hKeyTextureID; }
    GLuint                  GLTracker::getKeyTextureID() { return m_hKeyTextureID; }
    bool                    GLTracker::getTexturesInitialized() { return m_bTexturesInitialized; }


    unsigned char*  GLTracker::getPointPixelBuffer(int index) { return m_point_pixel_buffer[index]; }
    int             GLTracker::getPointChecksumTolerance(int index) { return m_point_checksum_tolerance[index]; }
    void            GLTracker::setPointChecksumTolerance(int index, int value) { m_point_checksum_tolerance[index] = value; }
    float           GLTracker::getPointSensitivity(int index) { return m_point_sensitivity[index]; }
    void            GLTracker::setPointSensitivity(int index, float value) { m_point_sensitivity[index] = value; }
    GLuint          GLTracker::getPointTextureId(int index) { return m_point_texture_id[index]; }
    GLuint*         GLTracker::getPointTextureIdPtr(int index) { return &(m_point_texture_id[index]); }

    bool            GLTracker::getPointEnabled(int index) { return getPointLayer(index)->layerStatus; }

    bool            GLTracker::getStabilize() { return m_stabilize; }
    void            GLTracker::setStabilize(bool flag) { m_stabilize = flag; }



