/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "tracker.h"
#include "pbuffer.h"
#include <jahrender.h>
#include <glworldlists.h>
#include <glcore.h>
#include <sstream>
#include <map>
#include <timer.h>


void GLTracker::paintGL()
{
    float2 screen_size;
    screen_size.x = m_pCore->getRenderWidth();
    screen_size.y = m_pCore->getRenderHeight();
    float2 screen_size_div_2 = screen_size / 2.0f;

    for (int point_number = 0; point_number < MAX_TRACKING_POINTS; point_number++)
    {
        bool point_enabled = getPointEnabled(point_number);
        int point_reference_frame = getPointReferenceFrame(point_number);

        if (  point_enabled && getCurrentFrame() == point_reference_frame )
        {
            setPointScreenPosition( point_number, getPointReferenceScreenPosition(point_number) );

            m_point_lower_left_corner_screen_coords[point_number] = getPointScreenPosition(point_number) - MAX_TRACKER_CIRCLE_RADIUS;
            m_point_lower_left_corner_image_coords[point_number] = m_point_lower_left_corner_screen_coords[point_number] - getImageLowerLeftCorner();
            m_point_image_position[point_number] = getPointScreenPosition(point_number) - getImageLowerLeftCorner();

            setPointPreviousPosition( point_number, getPointScreenPosition(point_number) );
            float2 zero_delta(0.0f);
            setPointPreviousDelta( point_number, zero_delta );
            capturePointPattern(point_number);
            glBindTexture(GL_TEXTURE_2D, getPointTextureId(point_number) ); 

            glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getPointTextureWidth(), getPointTextureWidth(),
                GL_RGBA, GL_UNSIGNED_BYTE, getPointPixelBuffer(point_number) );
        }
    }

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    if (!m_bSelect)
    { 
        if ( isLayerVisible( getJahLayer(0) ) )
        {
            m_pRenderSpace->drawLayer( getJahLayer(0), true );
        }
        else
        {
            m_pRenderSpace->drawLayer(m_pTitleSafe, true);
        }
    }
        
    //draw the grid if its visible
    if (m_bGridVal) 
    {  
        m_pRenderSpace->drawLayer(m_pGrid, true); 
    }
    
    //draw the grid if its visible
    if (m_bGridVal) 
    {  
        m_pRenderSpace->drawLayer(m_pGrid, true); 
    }

    static int buffer_size = int(MAX_TRACKER_TRACKING_RADIUS) * 4 * int(MAX_TRACKER_TRACKING_RADIUS) * 4 * 4;
    static unsigned char* pixel_buffer = new unsigned char[buffer_size];
    static bool point_was_reset[MAX_TRACKING_POINTS] = { false, false, false, false };

    if ( jahstd::glslSupport() && m_bNV_GPUSupport && getUseGpu() )
    {
        glslTracker( getBaseLayer() );

        for (int point_number = 0; point_number < MAX_TRACKING_POINTS; point_number++)
        {
            JahLayer* point_layer = getPointLayer(point_number);

            if ( point_layer->layerIsEnabled() )
            {
                float2 lower_left_corner = getPointScreenPosition(point_number) - getPointSearchRadius(point_number);
                int lower_left_x = static_cast<int>( jahstd::Roundf(lower_left_corner.x) );
                int lower_left_y = static_cast<int>( jahstd::Roundf(lower_left_corner.y) );

                int diameter = static_cast<int>( getPointSearchRadius(point_number) ) * 2;

                glReadPixels(
                    lower_left_x,
                    lower_left_y,
                    diameter,
                    diameter,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    pixel_buffer);

                int best_checksum = 255;
                int best_x = 0;
                int best_y = 0;
                float2 center;
                center.x = static_cast<float>(diameter) / 2.0f;
                center.y = static_cast<float>(diameter) / 2.0f;

                // Find the new center
                for (int y = 0; y < diameter; y++)
                {
                    for (int x = 0; x < diameter; x++)
                    {
                        float2 delta;
                        delta.x = static_cast<float>(x) - center.x;
                        delta.y = static_cast<float>(y) - center.y;
                        float radius = delta.length();
                        // Add an extra 1.0 for safety in rounding errors
                        radius += 1.0f;
                        
                        if ( radius < getPointSearchRadius(point_number) )
                        {
                            unsigned char checksum = pixel_buffer[(y * diameter * 4) + (x * 4)];

                            if (checksum < best_checksum)
                            {
                                best_checksum = checksum;
                                best_x = x;
                                best_y = y;
                            }
                        }
                    }
                }

                motionNode* sliders = getPointLayer(point_number)->layernodes->m_node;

                if ( !getDontTrack() )
                {
                    m_point_screen_position_unrounded[point_number].x = lower_left_corner.x + float(best_x);
                    m_point_screen_position_unrounded[point_number].y = lower_left_corner.y + float(best_y);
                    m_point_screen_position[point_number].x = jahstd::Roundf(m_point_screen_position_unrounded[point_number].x);
                    m_point_screen_position[point_number].y = jahstd::Roundf(m_point_screen_position_unrounded[point_number].y);

                    float2 delta = getPointScreenPosition(point_number) - getPointPreviousPosition(point_number);

                    if (point_was_reset[point_number])
                    {
                        // Recursion exit point
                        point_was_reset[point_number] = false;
                        return;
                    }
                    else if ( best_checksum == 255 )
                    {
                        float2 estimated_point_position = getPointPreviousPosition(point_number) + getPointPreviousDelta(point_number);
                        setPointScreenPosition(point_number, estimated_point_position);
                        sliders->rx = m_point_screen_position[point_number].x - screen_size_div_2.x;
                        sliders->ry = m_point_screen_position[point_number].y - screen_size_div_2.y;

                        setTrackerPoint(point_number);
                        point_was_reset[point_number] = true;
                        // Recursive call to keep display updated
                        updateGL();
                    }
                    else if ( best_checksum > getPointChecksumTolerance(point_number) )
                    {

                        //printf("Frame %d Resetting tracker point %d because checksum %d is out of tolerance range %d %f, %f\n",
                        //        getCurrentFrame(),
                        //        point_number,
                        //        best_checksum,
                        //        getPointChecksumTolerance(point_number),
                        //        m_point_screen_position[point_number].x,
                        //        m_point_screen_position[point_number].y);

                        sliders->rx = m_point_screen_position[point_number].x - screen_size_div_2.x;
                        sliders->ry = m_point_screen_position[point_number].y - screen_size_div_2.y;

                        setTrackerPoint(point_number);
                        point_was_reset[point_number] = true;
                        // Recursive call to keep display updated
                        updateGL();
                    }

                    m_point_image_position[point_number] = m_point_screen_position[point_number] - getImageLowerLeftCorner();
                    //m_keyframe_data[m_nCurrentFrame] = new TrackerData(m_nCurrentFrame, &m_point_image_position[point_number]);

                    delta = getPointScreenPosition(point_number) - getPointPreviousPosition(point_number);
                    setPointPreviousDelta(point_number, delta);
                    setPointPreviousPosition( point_number, getPointScreenPosition(point_number) );
                }
            }
        }
    }
    else
    {
        // CPU tracker
        QImage base_qimage = getBaseLayer()->getTextureData().objectImage.copy();

        for (int point_number = 0; point_number < MAX_TRACKING_POINTS; point_number++)
        {
            JahLayer* point_layer = getPointLayer(point_number);

            if ( point_layer->layerIsEnabled() )
            {
                // Do the CPU calculation in image coordinates rather than screen coordinates
                float2 pixel_position;
                float2 search_center_position;
                float2 rectangle_lower_left_corner;
                int rectangle_width = TRACKER_POINT_TEXTURE_WIDTH;
                int rectangle_height = rectangle_width;
                float2 delta;
                float radius;
                int best_checksum = 255;
                int best_x = 0;
                int best_y = 0;
                float checksum = 0.0f;
                int x_center;
                int y_center;
                int x;
                int y;

                static const int point_pattern_buffer_row_size = getPointTextureWidth() * 4;
                static const int bytes_per_pixel = 4;

                //int x_offset_in_pixel_buffer = (getPointTextureWidth() / 2) - static_cast<int>( getPointRadius(point_number) ) - 1; 
                //int y_offset_in_pixel_buffer = (getPointTextureWidth() / 2) - static_cast<int>( getPointRadius(point_number) ) - 1; 

                rectangle_lower_left_corner =  getPointImagePosition(point_number) - getPointRadius(point_number);
                rectangle_lower_left_corner.round();

                for (y = 0; y < rectangle_height; y++)
                {
                    pixel_position.y = rectangle_lower_left_corner.y + static_cast<float>(y);

                    // QImages are flipped vertically
                    int qimage_row = base_qimage.height() - 1 - static_cast<int>(pixel_position.y);

                    qimage_row = (qimage_row < 0) ? 0 
                        : ( qimage_row >= base_qimage.height() ) ? base_qimage.height() - 1
                        : qimage_row;

                    for (x = 0; x < rectangle_width; x++)
                    {
                        pixel_position.x = rectangle_lower_left_corner.x + static_cast<float>(x);
                        delta = pixel_position - getPointImagePosition(point_number);
                        radius = delta.length() + 1.0f;

                        if ( radius <= getPointRadius(point_number) )
                        {
                            int qimage_column = static_cast<int>(pixel_position.x);
                            qimage_column = (qimage_column < 0) ? 0 
                                : ( qimage_column >= base_qimage.width() ) ? base_qimage.width() - 1
                                : qimage_column;

                            QRgb* image_pixel = (QRgb*)base_qimage.scanLine(qimage_row) + qimage_column;

                            switch (point_number)
                            {
                                case 0  : { *image_pixel = qRgba(255, 0, 0, 255); break; }
                                case 1  : { *image_pixel = qRgba(0, 255, 0, 255); break; }
                                case 2  : { *image_pixel = qRgba(128, 128, 128, 255); break; }
                                default : { *image_pixel = qRgba(255, 255, 0, 255); break; }
                            }
                        }
                    }
                }

                float2 search_lower_left_corner = getPointImagePosition(point_number) - getPointSearchRadius(point_number);
                search_lower_left_corner.round();
                int search_diameter = static_cast<int>( jahstd::Roundf( getPointSearchRadius(point_number) ) * 2.0f );

                for (x_center = 0; x_center < search_diameter; x_center++)
                {
                    search_center_position.x = search_lower_left_corner.x + static_cast<float>(x_center);
                    int qimage_search_center_column = static_cast<int>(search_center_position.x);
                    qimage_search_center_column = (qimage_search_center_column < 0) ? 0 
                        : ( qimage_search_center_column >= base_qimage.width() ) ? base_qimage.width() - 1
                        : qimage_search_center_column;

                    for (y_center = 0; y_center < search_diameter; y_center++)
                    {
                        search_center_position.y = search_lower_left_corner.y + static_cast<float>(y_center);
                        // Qimages are upside down
                        int qimage_search_center_row = base_qimage.height() - static_cast<int>(search_center_position.y);
                        qimage_search_center_row = (qimage_search_center_row < 0) ? 0 
                            : ( qimage_search_center_row >= base_qimage.height() ) ? base_qimage.height() - 1
                            : qimage_search_center_row;
                        QRgb* point_pixel = (QRgb*)base_qimage.scanLine(qimage_search_center_row) + qimage_search_center_column;

                        float2 search_delta = search_center_position - getPointImagePosition(point_number);
                        float search_radius = search_delta.length() + 1.0f;
            
                        if ( search_center_position == getPointImagePosition(point_number) )
                        {
                            //bool break_here = true;
                        }

                        if ( search_radius <= getPointSearchRadius(point_number) )
                        {
                            rectangle_lower_left_corner = search_center_position - MAX_TRACKER_CIRCLE_RADIUS;
                            rectangle_lower_left_corner -= 1.0f;
                            checksum = 0.0f;

                            for (y = 0; y < rectangle_height; y++)
                            {
                                pixel_position.y = rectangle_lower_left_corner.y + static_cast<float>(y);
                                // QImages are upside down
                                int qimage_row = base_qimage.height() - 1 - static_cast<int>(pixel_position.y);
                                qimage_row = (qimage_row < 0) ? 0 
                                             : ( qimage_row >= base_qimage.height() ) ? base_qimage.height() - 1
                                             : qimage_row;

                                for (x = 0; x < rectangle_width; x++)
                                {
                                    pixel_position.x = rectangle_lower_left_corner.x + static_cast<float>(x);

                                    int qimage_column = static_cast<int>(pixel_position.x);
                                    qimage_column = (qimage_column < 0) ? 0 
                                        : ( qimage_column >= base_qimage.width() ) ? base_qimage.width() - 1
                                        : qimage_column;

                                    delta = pixel_position - search_center_position;
                                    radius = delta.length() + 1.0f;
                                    int column_offset = x * bytes_per_pixel;

                                    if ( radius <= getPointRadius(point_number) )
                                    {
                                        float4 actual_color;
                                        float4 expected_color;

                                        QRgb* image_pixel = (QRgb*)getBaseLayer()->getTextureData().objectImage.scanLine(qimage_row) + qimage_column;
                                        actual_color.x = static_cast<float>( qRed(*image_pixel) ) / 255.0f;
                                        actual_color.y = static_cast<float>( qGreen(*image_pixel) ) / 255.0f;
                                        actual_color.z = static_cast<float>( qBlue(*image_pixel) ) / 255.0f;

                                        int pixel_buffer_offset = (y * point_pattern_buffer_row_size) + column_offset;
                                        expected_color.x = static_cast<float>( *(getPointPixelBuffer(point_number) + pixel_buffer_offset + 0) ) / 255.0f;
                                        expected_color.y = static_cast<float>( *(getPointPixelBuffer(point_number) + pixel_buffer_offset + 1) ) / 255.0f;
                                        expected_color.z = static_cast<float>( *(getPointPixelBuffer(point_number) + pixel_buffer_offset + 2) ) / 255.0f;

                                        float4 difference = expected_color - actual_color;
                                        float dot_product = float4::dot(difference, difference);
                                        checksum += dot_product;
                                    }
                                }
                            }

                            checksum /= getPointSensitivity(point_number);
                            checksum *= 255.0f;

                            if (checksum < best_checksum)
                            {
                                best_x = x_center;
                                best_y = y_center;
                                best_checksum = int( checksum );
                            }

                            float4 checksum_color;
                            checksum_color.x = checksum;
                            checksum_color.y = checksum;
                            checksum_color.z = checksum;
                            checksum_color.w = 255.0f;
                            checksum_color.clamp();

                            *point_pixel = qRgba(
                                static_cast<int>(checksum_color.x),
                                static_cast<int>(checksum_color.y),
                                static_cast<int>(checksum_color.z),
                                static_cast<int>(checksum_color.w) );
                        }
                    }
                }

                motionNode* sliders = getPointLayer(point_number)->layernodes->m_node;

                if ( !getDontTrack() )
                {
                    m_point_image_position[point_number].x = jahstd::Roundf( search_lower_left_corner.x + float(best_x) );
                    m_point_image_position[point_number].y = jahstd::Roundf( search_lower_left_corner.y + float(best_y) );
                    m_point_screen_position[point_number] = m_point_image_position[point_number] + getImageLowerLeftCorner();

                    m_point_screen_position_unrounded[point_number].x = search_lower_left_corner.x + float(best_x);
                    m_point_screen_position_unrounded[point_number].y = search_lower_left_corner.y + float(best_y);
                    m_point_screen_position_unrounded[point_number] += getImageLowerLeftCorner();

                    float2 delta = getPointScreenPosition(point_number) - getPointPreviousPosition(point_number);

                    if (point_was_reset[point_number])
                    {
                        // Recursion exit point
                        point_was_reset[point_number] = false;
                        return;
                    }
                    else if ( best_checksum == 255 )
                    {
                        //printf("Resetting tracker point using motion estimation %9.3f  %9.3f\n",
                        //        getPointPreviousDelta(i).x, getPointPreviousDelta(i).y );
                        float2 estimated_point_position = getPointPreviousPosition(point_number) + getPointPreviousDelta(point_number);
                        setPointScreenPosition(point_number, estimated_point_position);
                        sliders->rx = m_point_screen_position[point_number].x - screen_size_div_2.x;
                        sliders->ry = m_point_screen_position[point_number].y - screen_size_div_2.y;
                        setTrackerPoint(point_number);
                        point_was_reset[point_number] = true;
                        // Recursive call to keep display updated
                        updateGL();
                    }
                    else if ( best_checksum > getPointChecksumTolerance(point_number) )
                    {
                        //printf("Resetting tracker point best_checksum = %d\n", best_checksum);
                        sliders->rx = m_point_screen_position[point_number].x - screen_size_div_2.x;
                        sliders->ry = m_point_screen_position[point_number].y - screen_size_div_2.y;
                        setTrackerPoint(point_number);
                        point_was_reset[point_number] = true;
                        // Recursive call to keep display updated
                        updateGL();
                    }

                    delta = getPointScreenPosition(point_number) - getPointPreviousPosition(point_number);
                    setPointPreviousDelta(point_number, delta);
                    setPointPreviousPosition( point_number, getPointScreenPosition(point_number) );
                }
            }
        }

        updateTextureFromQImage( getBaseLayer()->getTextureComposite(), &base_qimage, getBaseLayer() );
        getBaseLayer()->setTextureCompositeDirty(true);
        m_pRenderSpace->drawLayer(getBaseLayer(), true);
    }

    if ( getStabilize() )
    {
        if ( getNumberOfEnabledPoints() == 1)
        {
            // Single point stabilization
            int enabled_point = -1;

            for (int point_number = 0; point_number < MAX_TRACKING_POINTS; point_number++)
            {
                if ( getPointEnabled(point_number) )
                {
                    enabled_point = point_number;
                    break;
                }
            }

            int image_width = getBaseLayer()->getImageWidth();
            int image_height = getBaseLayer()->getImageHeight();
            float2 image_size( (float)image_width, (float)image_height );
            int search_radius = int( jahstd::Roundf( getPointSearchRadius(enabled_point) ) );
            search_radius = jahstd::Max<int>(0, search_radius);
            int x_offset = search_radius;
            //int y_offset = search_radius;
            float2 lower_left;
            float2 upper_right;
            lower_left.x = float(x_offset) / float(image_width);
            lower_left.y = lower_left.x;
            upper_right = 1.0f - lower_left;
            float2 delta = getPointScreenPositionUnrounded(enabled_point) - getPointInitialScreenPosition(enabled_point);
            //printf("%d: %f  %f\n", m_nCurrentFrame, delta.x, delta.y);
            delta /= image_size;
            lower_left += delta;
            upper_right += delta;
            float2 vertex_coordinates;
            float2 texture_ratio;

            texture_ratio.x = getBaseLayer()->textureData.texwidthratio;
            texture_ratio.y = getBaseLayer()->textureData.texheightratio;
            vertex_coordinates.x = getBaseLayer()->textureData.mapwidth;
            vertex_coordinates.y = getBaseLayer()->textureData.mapheight;

            lower_left *= texture_ratio;
            upper_right *= texture_ratio;

            //printf("lower_left.x = %f  lower_left.y = %f\n", lower_left.x, lower_left.y);
            //printf("upper_right.x = %f  upper_right.y = %f\n", upper_right.x, upper_right.y);

            getBaseLayer()->setTextureCompositeDirty(false);
            getBaseLayer()->bindBaseOrCompositeTexture();

            glBegin(GL_QUADS);	
            {
                glTexCoord2f(lower_left.x, lower_left.y);           
                glVertex3f(-vertex_coordinates.x, -vertex_coordinates.y,  0.0f);

                glTexCoord2f(upper_right.x,  lower_left.y);   
                glVertex3f( vertex_coordinates.x, -vertex_coordinates.y,  0.0f);

                glTexCoord2f(upper_right.x,  upper_right.y);                    
                glVertex3f( vertex_coordinates.x,  vertex_coordinates.y,  0.0f);

                glTexCoord2f(lower_left.x, upper_right.y);  
                glVertex3f(-vertex_coordinates.x,  vertex_coordinates.y,  0.0f);
            }
            glEnd();

        }
    }

    if ( m_keyframe_data.find(m_nCurrentFrame) == m_keyframe_data.end() )
    {
        TrackerData* new_tracker_data = new TrackerData(m_nCurrentFrame, 
            &m_point_image_position[0],
            &m_point_enabled[0] );
        m_keyframe_data[m_nCurrentFrame] = new_tracker_data;
    }
    else
    {
        std::map<int,TrackerData*>::iterator current_tracker_data;
        TrackerData* tracker_data = m_keyframe_data.find(m_nCurrentFrame)->second;
        tracker_data->setFrameNumber(m_nCurrentFrame);
        tracker_data->setPointImagePosition(&m_point_image_position[0]);
        tracker_data->setPointEnabled(&m_point_enabled[0]);
    }

    glPopMatrix();
    
    glFlush();
        
    m_bSlidersHaveChanged = false;
}


void 
GLTracker::setStabilize(void)
{
    
    m_stabilize = m_stabilize_qcheckbox->isChecked();
    
    updateGL();
}

void 
GLTracker::setGpuSelect(void)
{
    
    m_bGPUSelect = m_use_gpu_qcheckbox->isChecked();
    
    updateGL();
}

void            
GLTracker::glslTracker(JahLayer* image_layer)
{
    GLsizei message_length;
    char message[1000];

    static GLint point_radius_handle[MAX_TRACKING_POINTS];
    static GLint point_tracking_radius_handle[MAX_TRACKING_POINTS];
    static GLint point_position_handle[MAX_TRACKING_POINTS];
    static GLint point_enabled_handle[MAX_TRACKING_POINTS];
    static GLint point_sensitivity_handle[MAX_TRACKING_POINTS];

    static GLint image_lower_left_corner_handle;
    static GLint point_texture_step_handle;
    static GLint image_texture_step_handle;
    static GLint image_texture_ratio_handle;
    static GLint point_texture_ratio_handle;
    static GLint point_texture_size_handle;

    //glClear(GL_COLOR_BUFFER_BIT);

    if (m_pShaderProgram == NULL)
    {
        m_pFragmentShader = new GlslShader(GL_FRAGMENT_SHADER_ARB, getTrackerFragmentShader() );
        m_aShaderVector.push_back(m_pFragmentShader);
        
        m_pShaderProgram = new GlslProgram(m_aShaderVector);
        m_hShaderProgramHandle = *m_pShaderProgram->getGlHandle();

        image_lower_left_corner_handle 
	  = glGetUniformLocationARB(
				    m_hShaderProgramHandle, "image_lower_left_corner");
        image_texture_step_handle 
	  = glGetUniformLocationARB(m_hShaderProgramHandle, "image_texture_step");
        point_texture_step_handle = glGetUniformLocationARB(m_hShaderProgramHandle, "point_texture_step");
        image_texture_ratio_handle = glGetUniformLocationARB(m_hShaderProgramHandle, "image_texture_ratio");
        point_texture_ratio_handle = glGetUniformLocationARB(m_hShaderProgramHandle, "point_texture_ratio");
        point_texture_size_handle = glGetUniformLocationARB(m_hShaderProgramHandle, "point_texture_size");
        
        for (int i = 0; i < MAX_TRACKING_POINTS; i++)
        {
            std::ostringstream declaration;
            declaration << "point" << i << "_radius";
            std::string declaration_string = declaration.str();
            char* declaration_char_string = (char*)declaration_string.c_str();

            point_radius_handle[i] = 
                glGetUniformLocationARB( m_hShaderProgramHandle, (const char*)declaration_char_string );

            declaration.str("");
            declaration << "point" << i << "_tracking_radius";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            point_tracking_radius_handle[i] = 
                glGetUniformLocationARB( m_hShaderProgramHandle, (const char*)declaration_char_string );

            declaration.str("");
            declaration << "point" << i << "_position";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            point_position_handle[i] = 
                glGetUniformLocationARB( m_hShaderProgramHandle, (const char*)declaration_char_string );

            declaration.str("");
            declaration << "point" << i << "_enabled";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            point_enabled_handle[i] = 
                glGetUniformLocationARB( m_hShaderProgramHandle, (const char*)declaration_char_string );

            declaration.str("");
            declaration << "point" << i << "_sensitivity";
            declaration_string = declaration.str();
            declaration_char_string = (char*)declaration_string.c_str();

            point_sensitivity_handle[i] = 
                glGetUniformLocationARB( m_hShaderProgramHandle, (const char*)declaration_char_string );
        }
        
        glGetInfoLogARB(m_hShaderProgramHandle, 1000, &message_length, message);
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D, getBaseLayerTextureId() );
    glEnable(GL_TEXTURE_2D);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

    TextureImage* image_texture = &image_layer->textureData;

    float2 image_size;
    image_size.x = image_texture->getImageWidth();
    image_size.y = image_texture->getImageHeight();

    int image_layer_width_div_2 = int(image_size.x / 2.0f);
    int image_layer_height_div_2 = int(image_size.y / 2.0f);

    float2 image_texture_ratio;
    getBaseLayer()->getTextureData().getTextureRatios(image_texture_ratio);

    float2 image_texture_step = image_texture_ratio / image_size;

    float2 point_texture_ratio;
    point_texture_ratio.x = 1.0;
    point_texture_ratio.y = 1.0;

    float2 point_texture_size;
    point_texture_size.x = float( getPointTextureWidth() );
    point_texture_size.y = float( getPointTextureWidth() );

    float2 point_texture_step = point_texture_ratio / point_texture_size;

    float2 screen_size;
    screen_size.x = m_pCore->getRenderWidth();
    screen_size.y = m_pCore->getRenderHeight();

    float2 image_lower_left_corner = screen_size - image_size;
    image_lower_left_corner = image_lower_left_corner / 2.0f;
 
    bool point_enabled[MAX_TRACKING_POINTS];
    JahLayer* point_layer[MAX_TRACKING_POINTS];

    int i;

    glUseProgramObjectARB(m_hShaderProgramHandle);

    m_pShaderProgram->setTextureHandle("image_texture", 0);
    glUniform2fARB(image_lower_left_corner_handle, image_lower_left_corner.x, image_lower_left_corner.y);
    glUniform2fARB(point_texture_step_handle, point_texture_step.x, point_texture_step.y);
    glUniform2fARB(image_texture_step_handle, image_texture_step.x, image_texture_step.y);
    glUniform2fARB(image_texture_ratio_handle, image_texture_ratio.x, image_texture_ratio.y);
    glUniform2fARB(point_texture_ratio_handle, point_texture_ratio.x, point_texture_ratio.y);
    glUniform2fARB(point_texture_size_handle, point_texture_size.x, point_texture_size.y); 

    for (i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        GLuint texture_id = getPointTextureId(i) ;

        glActiveTextureARB(GL_TEXTURE1_ARB + i);
        glBindTexture(GL_TEXTURE_2D, texture_id);
        glEnable(GL_TEXTURE_2D);

        point_layer[i] = getPointLayer(i);

        glUniform2fARB( point_position_handle[i], getPointScreenPosition(i).x, getPointScreenPosition(i).y );
        glUniform1fARB( point_radius_handle[i], getPointRadius(i) );
        glUniform1fARB( point_tracking_radius_handle[i], getPointSearchRadius(i) );
        glUniform1fARB( point_sensitivity_handle[i], getPointSensitivity(i) ); 

        point_enabled[i] = point_layer[i]->layerStatus;
        glUniform1fARB(point_enabled_handle[i], point_enabled[i]);

        std::ostringstream point_name;
        point_name << "point" << i << "_texture";
        std::string point_name_string = point_name.str();
        char* point_name_cstring = (char*)( point_name_string.c_str() );
        m_pShaderProgram->setTextureHandle(point_name_cstring, i + 1);
    }

    //m_pCore->bindPbuffer();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_QUADS);
    {
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f); 
        glVertex3f(-image_layer_width_div_2, -image_layer_height_div_2, 0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, image_texture_ratio.x, 0.0f);           
        glVertex3f(image_layer_width_div_2, -image_layer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, image_texture_ratio.x, image_texture_ratio.y);           
        glVertex3f(image_layer_width_div_2, image_layer_height_div_2,  0.0f);
        
        glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, image_texture_ratio.y);           
        glVertex3f(-image_layer_width_div_2, image_layer_height_div_2,  0.0f);
    }
    glEnd(); // GL_QUADS

    //m_pCore->unbindPbuffer();

    glUseProgramObjectARB(0);

    for (i = 0; i < MAX_TRACKING_POINTS; i++)
    {
        glActiveTextureARB(GL_TEXTURE1_ARB + i);
        glDisable(GL_TEXTURE_2D);
    }

    glActiveTextureARB(GL_TEXTURE0_ARB);
}

