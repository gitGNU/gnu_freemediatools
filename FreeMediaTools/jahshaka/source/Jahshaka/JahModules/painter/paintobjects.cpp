
 /*******************************************************************************
 **
 ** The source file for the Jahshaka animation m_bSelections module
 ** The Jahshaka Project
 ** Copyright (C) 2000-2006 VM Inc.
 ** Released under the GNU General Public License
 **
 *******************************************************************************/
 
 
 
 #include "paint.h"
 #include <glcore.h>
 #include <sstream>
 #include <qpushbutton.h>
 
 
 void            
 GLPaint::executePaintEffect(PAINT_EFFECT_TYPE effect_type)
 {
     ( *this.*getPaintEffectFunction(effect_type) )();
 }
 
 /*
 | Apply the given convolution matrix to the pixel at the given location, and
 | set the rgb color to the color for the pixel at that location.
 */
 void GLPaint::convolute(const convolutionType type) 
 {
     color3f255 rgb;
     unsigned int pix;
 
     int buffer_size = getFrameWidth() * getFrameHeight();
     unsigned int* buffer = new unsigned int[buffer_size];
     unsigned int* working_buffer = new unsigned int[buffer_size];
 
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     jahstd::glGetTextureSubImage(0, 0, getFrameWidth(), getFrameHeight(), 
                                  getTextureWidth(), getTextureHeight(), (unsigned char*)buffer );
 
     for (int y = 0; y < getFrameHeight(); y++)
     {
         int pixel_row_offset = y * getFrameWidth();
 
         for (int x = 0; x < getFrameWidth(); x++) 
         {
             rgb[0] = rgb[1] = rgb[2] = 0.0;
 
             for( int row = -1;  row <= 1;  row++ ) 
             {
                 for( int col = -1;  col <= 1;  col++ ) 
                 {
                     int current_column = x + row;
                     int current_row = y + col;
                     int row_offset = current_row * getFrameWidth();
                     int buffer_offset = row_offset + current_column;
 
                     if (buffer_offset >= 0 && buffer_offset < buffer_size)
                     {
                         pix = buffer[buffer_offset];
                     }
                     else
                     {
                         pix = buffer[pixel_row_offset + x];
                     }
 
                     rgb[0] += (double)qRed(pix)   * convolutionMatrix[type][col+1][row+1];
                     rgb[1] += (double)qGreen(pix) * convolutionMatrix[type][col+1][row+1];
                     rgb[2] += (double)qBlue(pix)  * convolutionMatrix[type][col+1][row+1];
                 }
             }
 
             unsigned int convoluted_pixel = qRgb(limit0_255((int)rgb[0]),
                                                  limit0_255((int)rgb[1]),
                                                  limit0_255((int)rgb[2]) );
 
             working_buffer[pixel_row_offset + x] = convoluted_pixel;
         }
     }
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, working_buffer );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] buffer;
     delete [] working_buffer;
 }
 
 void GLPaint::invertEffect() 
 {
     int buffer_size = getFrameWidth() * getFrameHeight() * 4;
     unsigned char* buffer = new unsigned char[buffer_size];
 
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     jahstd::glGetTextureSubImage(0, 0, getFrameWidth(), getFrameHeight(), 
                                  getTextureWidth(), getTextureHeight(), buffer);
 
     for (int i = 0; i < buffer_size; i++)
     {
         buffer[i] = ~buffer[i];
     }
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, buffer );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] buffer;
 }
 
 void 
 GLPaint::blurEffect()
 {
     convolute(blur);
 }
 
 void 
 GLPaint::sharpenEffect()
 {
     convolute(sharpen);
 }
 
 void 
 GLPaint::edgeDetectXEffect()
 {
     convolute(edgeDetectX);
 }
 
 void 
 GLPaint::edgeDetectYEffect()
 {
     convolute(edgeDetectY);
 }
 
 void
 GLPaint::aOverBCompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslAOverBComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void
 GLPaint::bOverACompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslBOverAComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void
 GLPaint::aInBCompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslAInBComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void
 GLPaint::bInACompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslBInAComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void GLPaint::aOutBCompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslAOutBComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void GLPaint::bOutACompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslBOutAComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void GLPaint::aAtopBCompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslAAtopBComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void GLPaint::bAtopACompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslBAtopAComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 void GLPaint::aXorBCompositeEffect()
 {
     int a_frame_number = getCurrentHistoryFrame();
     int b_frame_number = (a_frame_number > 1) ? a_frame_number - 1 : a_frame_number;
     glslAXorBComposite( getTextureId(a_frame_number), getTextureId(b_frame_number) );
 }
 
 
 void 
 GLPaint::sobelEffect()
 {
     convolute(sobel);
 }
 
 void 
 GLPaint::laplacianEffect()
 {
     convolute(laplacian);
 }
     
 void 
 GLPaint::laplacian2Effect()
 {
     convolute(laplacian2);
 }
     
 void GLPaint::lapOfGaussEffect()
 {
     convolute(lapOfGauss);
 }
 
 
 
 /*
 | Fade the colors in the image towards white.
 | Approximately the opposite of intensify().
 */
 void GLPaint::fadeEffect() 
 {
     unsigned int pix;
 
     int buffer_size = getFrameWidth() * getFrameHeight();
     unsigned int* buffer = new unsigned int[buffer_size];
 
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     jahstd::glGetTextureSubImage(0, 0, getFrameWidth(), getFrameHeight(), getTextureWidth(), getTextureHeight(), 
                                  (unsigned char*)buffer );
 
     QImage qimage(getFrameWidth(), getFrameHeight(), 32);
     updateQimageFromRgbaBuffer(&qimage, buffer, getFrameWidth(), getFrameHeight());
 
     for (int y = 0; y < qimage.height(); y++)
     {
         for (int x = 0; x < qimage.width(); x++) 
         {
             pix = qimage.pixel(x,y);
             qimage.setPixel(x, y,
                 qRgb(limit0_255( (qRed(pix) * m_fade_degree) / 255 ),
                 limit0_255( (qGreen(pix) * m_fade_degree) / 255 ),
                 limit0_255( (qBlue(pix) * m_fade_degree) / 255 ) ) );
         }
     }
 
     qimage = qimage.mirror(false, true);
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, qimage.bits() );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] buffer;
 }
 
 
 /*
 | Intensify the colors in the image towards black.
 | Approximately the opposite of fade().
 */
 void GLPaint::intensifyEffect() 
 {
     unsigned int pix;
 
     int buffer_size = getFrameWidth() * getFrameHeight();
     unsigned int* buffer = new unsigned int[buffer_size];
 
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     jahstd::glGetTextureSubImage(0, 0, getFrameWidth(), getFrameHeight(), getTextureWidth(), getTextureHeight(), 
                                  (unsigned char*)buffer );
 
     QImage qimage(getFrameWidth(), getFrameHeight(), 32);
     updateQimageFromRgbaBuffer(&qimage, buffer, getFrameWidth(), getFrameHeight());
 
     for (int y = 0; y < qimage.height(); y++)
     {
         for (int x = 0; x < qimage.width(); x++) 
         {
             pix = qimage.pixel(x,y);
             qimage.setPixel(x, y,
                 qRgb(limit0_255( (qRed(pix) * 255) / m_fade_degree ),
                 limit0_255( (qGreen(pix) * 255) / m_fade_degree ),
                 limit0_255( (qBlue(pix) * 255) / m_fade_degree ) ) );
         }
     }
 
     qimage = qimage.mirror(false, true);
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, qimage.bits() );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] buffer;
 }
 
 void 
 GLPaint::embossEffect()
 {
     int buffer_size = getFrameWidth() * getFrameHeight();
     unsigned int* buffer = new unsigned int[buffer_size];
 
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     jahstd::glGetTextureSubImage(0, 0, getFrameWidth(), getFrameHeight(), getTextureWidth(), getTextureHeight(), 
                                  (unsigned char*)buffer );
 
     QImage qimage(getFrameWidth(), getFrameHeight(), 32);
     updateQimageFromRgbaBuffer(&qimage, buffer, getFrameWidth(), getFrameHeight());
 
     int red;
     int green;
     int blue;
     int alpha = 0;
 
     const int emboss_w = 3;
     const int emboss_h = 3;
 
     int sumr = 0;
     int sumg = 0;
     int sumb = 0;
     int emboss_filter[emboss_w][emboss_h]={{2,0,0},{0,-1,0},{0,0,-1}};
     //int emboss_sum = (int)(getCurrentEffectLayer()->layernodes->m_node->rx+1);
     int emboss_sum = 1;
 
     if (emboss_sum < 1) 
     {
         emboss_sum = 1;
     }
 
     QImage* qimage_buffer = new QImage( getFrameWidth(), getFrameHeight(), 32 );
 
     for ( int y = 1; y < getFrameHeight() - 1; y++ )
     {
         for ( int x = 1; x < getFrameWidth() - 1; x++ )
         {
             sumr = 0;
 
             //added these
             sumg = 0;
             sumb = 0;
 
             for(int k = 0; k < emboss_w; k++)
             {
                 for(int l = 0; l < emboss_h; l++)
                 {
                     int xval = x - ( (emboss_w-1) >> 1 ) + k;
                     int yval = y - ( (emboss_h-1) >> 1 ) + l;
 
                     QRgb *sp = (QRgb *)qimage.scanLine(yval) + xval;
 
                     red = qRed(*sp);
                     green = qGreen(*sp);
                     blue = qBlue(*sp);
                     alpha = qAlpha(*sp);
 
                     sumr += red * emboss_filter[k][l];
                     sumg += green * emboss_filter[k][l];
                     sumb += blue * emboss_filter[k][l];
                 }
             }
 
             sumr /= emboss_sum; 
             sumr += 128;
             sumr = jahstd::Min<int>(sumr, 255);
             sumr = jahstd::Max<int>(sumr, 0);
 
             //added these
             sumg /= emboss_sum; 
             sumg += 128;
             sumg = jahstd::Min<int>(sumg, 255);
             sumg = jahstd::Max<int>(sumg, 0);
 
             sumb /= emboss_sum; 
             sumb += 128;
             sumb = jahstd::Min<int>(sumb, 255);
             sumb = jahstd::Max<int>(sumb, 0);
 
             QRgb *p = (QRgb *)qimage_buffer->scanLine(y) + x;
             *p = qRgba(sumr, sumg, sumb, alpha);
         }
     }
 
     //now we must copy buffer into pix
     qimage = qimage_buffer->copy();
 	delete qimage_buffer;
 
     qimage = qimage.mirror(false, true);
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, qimage.bits() );
     updateGL();
     delete [] buffer;
     updateTimeSlider( getCurrentHistoryFrame() );
 }
 
 
 void GLPaint::drawToolPen( color4& pen_color )
 {
     // A point on point1 and point2, with a line between, without any gradient.
 
     //glPointSize(brush_size);
 
 
 	std::vector< PaintPixelCoords >::iterator iter;
 
     int point_brush_size = int( jahstd::Max<int>(int( m_point_brush_size ), 1) );
     int line_brush_size = int( jahstd::Max<int>(int( m_line_brush_size ), 1) );
 
 	for ( iter = m_pixel_coords.begin( ); iter != m_pixel_coords.end( ); iter ++ )
 	{
 		PaintPixelCoords &coords = *iter;
 
     	glColor4fv(&pen_color.x);
 
     	if (getPenAlpha() > 0.99f)
     	{
         	glDisable(GL_BLEND);
     	}
 
     	glPointSize(point_brush_size);
         glEnable(GL_POINT_SMOOTH);
     	glBegin(GL_POINTS);
     	{
         	glVertex2f(coords.x1,coords.y1); 
         	glVertex2f(coords.x2,coords.y2);
     	}
     	glEnd();
 
 		glLineWidth(line_brush_size);
 		glEnable(GL_LINE_SMOOTH);
     	glBegin(GL_LINES);
     	{
         	glVertex2f(coords.x1,coords.y1);   
         	glVertex2f(coords.x2,coords.y2);
     	}
     	glEnd();
 	}
 }
 
 void GLPaint::drawToolLine( int, color4& pen_color_light, color4& pen_color_dark, GLint,
                            int x1, int y1, int x2, int y2  ) 
 {
     float line_range[2];
     glGetFloatv(GL_LINE_WIDTH_RANGE, line_range);
 
     float point_size = jahstd::Min<float>(m_point_brush_size, line_range[1]);
     point_size = jahstd::Max(point_size, 1.0f);
     float line_size = point_size;
 
     // A point on point1 and point2, with a line between.
     glPointSize(point_size);
 	glEnable(GL_POINT_SMOOTH);
     glBegin(GL_POINTS);
     glColor4fv(&pen_color_light.x);
     //glVertex2f(x1, y1);
     //glVertex2f(x2, y2);
     glEnd();
 
     glLineWidth(line_size);
 	glEnable(GL_LINE_SMOOTH);
     glBegin(GL_LINES);
     {
         glColor4fv(&pen_color_light.x);
         glVertex2f(x1,y1);
         glColor4fv(&pen_color_dark.x);
         glVertex2f(x2,y2);
     }
     glEnd();
 }
 
 void GLPaint::drawToolRect( int,
                            color4& pen_color, color4& pen_color_light, color4& pen_color_dark, GLint,
                            int x1, int y1, int x2, int y2  ) 
 {
     float line_brush_size = jahstd::Even(int( m_line_brush_size ) + 1);
     line_brush_size = jahstd::Max<float>(line_brush_size, 2.0f);
 
     float y_displacement = line_brush_size / 2.0f;
 	y_displacement = (y1 > y2) ? y_displacement : -y_displacement;
     float x_displacement = line_brush_size / 2.0f;
 	x_displacement = (x1 < x2) ? x_displacement : -x_displacement;
 
     glLineWidth(line_brush_size);
 	glDisable(GL_LINE_SMOOTH);
     glBegin(GL_LINES);
 	{
 		glColor4fv(&pen_color_light.x); 
 		glVertex2f(x1 + x_displacement, y1 - y_displacement); 
 		glColor4fv(&pen_color.x);      
 		glVertex2f(x2 - x_displacement, y1 - y_displacement);
 
 		glVertex2f(x2, y1); 
 		glColor4fv(&pen_color_dark.x);  
 		glVertex2f(x2, y2);
 
 		glVertex2f(x2 - x_displacement, y2 + y_displacement); 
 		glColor4fv(&pen_color.x);      
 		glVertex2f(x1 + x_displacement, y2 + y_displacement);
 
 		glVertex2f(x1, y2); 
 		glColor4fv(&pen_color_light.x); 
 		glVertex2f(x1, y1);
 	}
     glEnd();
 }
 
 void GLPaint::drawToolRectFilled( int,  GLint,
                                  color4& pen_color, color4& pen_color_light, color4& pen_color_dark,
                                  color4& fill_color, color4& fill_color_light, color4& fill_color_dark,
                                  int x1, int y1, int x2, int y2  ) 
 {
     float line_brush_size = jahstd::Even(int( m_line_brush_size ) + 1);
 
     float y_displacement = line_brush_size / 2.0f;
 	y_displacement = (y1 > y2) ? y_displacement : -y_displacement;
     float x_displacement = line_brush_size / 2.0f;
 	x_displacement = (x1 < x2) ? x_displacement : -x_displacement;
 
     if (line_brush_size > 0)
     {
         glLineWidth(line_brush_size);
         glDisable(GL_LINE_SMOOTH);
         glBegin(GL_LINES);
         {
             glColor4fv(&pen_color_light.x); 
             glVertex2f(x1 + x_displacement, y1 - y_displacement); 
             glColor4fv(&pen_color.x);      
             glVertex2f(x2 - x_displacement, y1 - y_displacement);
 
             glVertex2f(x2, y1); 
             glColor4fv(&pen_color_dark.x);  
             glVertex2f(x2, y2);
 
             glVertex2f(x2 - x_displacement, y2 + y_displacement); 
             glColor4fv(&pen_color.x);      
             glVertex2f(x1 + x_displacement, y2 + y_displacement);
 
             glVertex2f(x1, y2); 
             glColor4fv(&pen_color_light.x); 
             glVertex2f(x1, y1);
         }
         glEnd();
     }
 
     y_displacement = (y1 < y2) ? line_brush_size : -line_brush_size;
 
     glDisable(GL_POLYGON_SMOOTH);
     glBegin(GL_POLYGON);
     {
         glColor4fv(&fill_color_dark.x);
         glVertex3f(x1 + x_displacement, y1 + y_displacement, 0.0f);
         glColor4fv(&fill_color.x);
         glVertex3f(x2 - x_displacement, y1 + y_displacement, 0.0f);
         glColor4fv(&fill_color_light.x);
         glVertex3f(x2 - x_displacement, y2 - y_displacement, 0.0f);
         glColor4fv(&fill_color.x);
         glVertex3f(x1 + x_displacement, y2 - y_displacement, 0.0f);
     }
     glEnd();
 
     glColor4fv(&pen_color.x);
 }
 
 
 
 void GLPaint::drawToolCircle(  color4& pen_color, GLfloat gradientDegree,
                              int x1, int y1, int x2, int y2  ) 
 {
     // A circle with its centre on point1, and its radius out to point2
     float xLen = fabs( fabs( double(x1) ) - fabs( double(x2) ) );
     float yLen = fabs( fabs( double(y1) ) - fabs( double(y2) ) );
 
     float radius = sqrt( xLen * xLen + yLen * yLen );
 	xLen = jahstd::Max<float>(xLen, 0.0001f);
 	
     float drawingAngle = atan( yLen / xLen );  // angle from point1 to point2
 
     if ( y2 > y1 ) 
     {
         if ( x2 < x1 ) 
         {
             drawingAngle  = PI - drawingAngle;    //flip   into quad2
         }
     } 
     else 
     {
         if ( x2 > x1 ) 
         {
             drawingAngle  = twoPI - drawingAngle; //flip   into quad4
         }
         else           
         {
             drawingAngle += float(PI);                   //rotate into quad3
         }
     }
 
     float line_brush_size = (m_line_brush_size == 0) ? 1 : m_line_brush_size;
 
 	glLineWidth(line_brush_size);
 	glEnable(GL_LINE_SMOOTH);
 
     float step = float(PI) / float(toolCirclePointsPerPI);
     float offset = 0.0f;
 
 
     glBegin(GL_LINE_STRIP);
     {
         for (int i = 0; i < 5; i++)
         {
             for ( float angle  = offset ; angle  < float(twoPI) + step + offset; angle += step)
             {
                 glColor4f( pen_color.red() - gradientDegree * cos(angle),
                     pen_color.green() - gradientDegree * cos(angle),
                     pen_color.blue() - gradientDegree * cos(angle), pen_color.alpha() );
                 glVertex2f( x1 + radius * cos(angle+drawingAngle),
                     y1 + radius * sin(angle+drawingAngle));
             }
 
             offset += step / 5.0f;
         }
     }
     glEnd();
 
 
 }
 
 void GLPaint::drawToolCircleFilled(  color4& pen_color, color4& fill_color, GLfloat gradientDegree,
                                    int x1, int y1, int x2, int y2  ) 
 {
     GLfloat angle,drawingAngle,xLen,yLen,radius;
 
     // A filled circle with its centre on point1, and its radius out to point2,
     // and a pen circle with its centre on point1 and its radius out to point2.
     xLen = fabs(fabs((double)x1)-fabs((double)x2));
     yLen = fabs(fabs((double)y1)-fabs((double)y2));
     radius = sqrt( xLen*xLen + yLen*yLen );
     float fill_radius = radius - (m_line_brush_size / 2.0f) + 0.5f;
 
     if (xLen == 0) 
     {
         xLen = 0.0001f;          // avoid dividing by zero
     }
 
     drawingAngle = atan ( yLen / xLen );  // angle from point1 to point2
 
     if ( y2 > y1 ) 
     {
         if ( x2 < x1 ) 
         {
             drawingAngle  = PI - drawingAngle;    //flip   into quad2
         }
     } 
     else 
     {
         if ( x2 > x1 ) 
         {
             drawingAngle  = twoPI - drawingAngle; //flip   into quad4
         }
         else           
         {
             drawingAngle += float(PI);                   //rotate into quad3
         }
     }
 
 	glDisable(GL_POLYGON_SMOOTH);
     glBegin(GL_POLYGON);
     {
         for ( angle  = 0.0f; angle  < float(twoPI) + float(PI) / float(toolCirclePointsPerPI);
             angle += float(PI) / float(toolCirclePointsPerPI) ) 
         {
             glColor4f( fill_color.red() + gradientDegree * cos(angle),
                 fill_color.green() + gradientDegree * cos(angle),
                 fill_color.blue() + gradientDegree * cos(angle), fill_color.alpha() );
             glVertex2f( x1 + fill_radius * cos(angle+drawingAngle),
                 y1 + fill_radius * sin(angle+drawingAngle));
         }
     }
     glEnd();
 
     if (m_line_brush_size > 0)
     {
         glLineWidth(m_line_brush_size);
         glEnable(GL_LINE_SMOOTH);
 
         float step = float(PI) / float(toolCirclePointsPerPI);
         float offset = 0.0f;
 
         glBegin(GL_LINE_STRIP);
         {
             for (int i = 0; i < 5; i++)
             {
                 for ( angle  = offset ; angle  < float(twoPI) + step + offset; angle += step)
                 {
                     glColor4f( pen_color.red() - gradientDegree * cos(angle),
                         pen_color.green() - gradientDegree * cos(angle),
                         pen_color.blue() - gradientDegree * cos(angle), pen_color.alpha() );
                     glVertex2f( x1 + radius * cos(angle+drawingAngle),
                         y1 + radius * sin(angle+drawingAngle));
                 }
 
                 offset += step / 5.0f;
             }
         }
         glEnd();
     }
 }
 
 void GLPaint::drawToolTri ( color4& pen_color, color4& pen_color_light, color4& pen_color_dark,
                            int x1, int y1, int x2, int y2  ) 
 {
     GLfloat angle,xLen,yLen,radius;
 
     // A pen triangle from point1 to point2, attempting to be equilateral,
     // with a point on each of its corners.
     xLen   = fabs(fabs((double)x2)-fabs((double)x1));
     yLen   = fabs(fabs((double)y1)-fabs((double)y2));
     radius = sqrt( xLen*xLen + yLen*yLen ) * atanPI;
 
     if (radius != 0) 
     {                  // be careful not to divide by zero
         angle  = asin( yLen / radius );
         xLen   = radius * sin(angle) * 0.77;
         yLen   = radius * cos(angle) * 0.77;
 		glColor4fv(&pen_color.x);
 
         float line_brush_size = (m_line_brush_size == 0) ? 1 : m_line_brush_size;
 
 		glLineWidth(line_brush_size);
 		glDisable(GL_LINE_SMOOTH);
 
 		glBegin(GL_LINE_STRIP);
         {
             glColor4fv(&pen_color_light.x);
             glVertex2f( x2, y2 );
             glColor4fv(&pen_color_dark.x);
 
             if (y2<=y1 && x2>=x1) 
             {
                 glVertex2f( x1-xLen, y1-yLen ); glVertex2f( x1+xLen, y1+yLen );
             } 
             else if (y2<=y1 && x2< x1) 
             {
                 glVertex2f( x1-xLen, y1+yLen ); glVertex2f( x1+xLen, y1-yLen );
             } 
             else if (y2> y1 && x2>=x1) 
             {
                 glVertex2f( x1+xLen, y1-yLen ); glVertex2f( x1-xLen, y1+yLen );
             } 
             else                       
             {
                 glVertex2f( x1+xLen, y1+yLen ); glVertex2f( x1-xLen, y1-yLen );
             }
             glColor4fv(&pen_color_light.x);
             glVertex2f( x2, y2 );
         }
         glEnd();
     }
                            }
 
 void GLPaint::drawToolTriFilled( color4& pen_color, color4& pen_color_light,color4& pen_color_dark,
                                 color4& fill_color, color4& fill_color_light,color4& fill_color_dark,
                                 int x1, int y1, int x2, int y2  ) 
 {
     GLfloat angle,xLen,yLen,radius;
 
 
     // A filled triangle from point1 to point2, attempting to be equilateral,
     // a pen triangle from point1 to point2, attempting to be equilateral,
     // and a point on each of the corners.
     xLen   = fabs(fabs((double)x2)-fabs((double)x1));
     yLen   = fabs(fabs((double)y1)-fabs((double)y2));
     radius = sqrt( xLen*xLen + yLen*yLen ) * atanPI;
 
     if (radius != 0) 
     {                  // be careful not to divide by zero
         angle  = asin( yLen / radius );
         xLen   = radius * sin(angle) * 0.77;
         yLen   = radius * cos(angle) * 0.77;
         glColor4fv(&fill_color.x);
 
 		glDisable(GL_POLYGON_SMOOTH);
         glBegin(GL_POLYGON);
         {
             glColor4fv(&fill_color_light.x);
             glVertex2f( x2, y2 );
             glColor4fv(&fill_color_dark.x);
 
             if (y2<=y1 && x2>=x1) 
             {        // quadrant 4
                 glVertex2f( x1-xLen, y1-yLen ); glVertex2f( x1+xLen, y1+yLen );
             } 
             else if (y2<=y1 && x2< x1) 
             {        // quadrant 3
                 glVertex2f( x1-xLen, y1+yLen ); glVertex2f( x1+xLen, y1-yLen );
             } 
             else if (y2> y1 && x2>=x1) 
             {        // quadrant 1
                 glVertex2f( x1+xLen, y1-yLen ); glVertex2f( x1-xLen, y1+yLen );
             } 
             else                       
             {        // quadrant 2
                 glVertex2f( x1+xLen, y1+yLen ); glVertex2f( x1-xLen, y1-yLen );
             }
         }
         glEnd();
 
         glColor4fv(&pen_color.x);
 
         if (m_line_brush_size > 0)
         {
             glLineWidth(m_line_brush_size);
             glDisable(GL_LINE_SMOOTH);
             glBegin(GL_LINE_STRIP);
             {
                 glColor4fv(&pen_color_dark.x);
                 glVertex2f( x2, y2 );
                 glColor4fv(&pen_color_light.x);
 
                 if        (y2<=y1 && x2>=x1) 
                 {        // quadrant 4
                     glVertex2f( x1-xLen, y1-yLen ); glVertex2f( x1+xLen, y1+yLen );
                 } 
                 else if (y2<=y1 && x2< x1) 
                 {        // quadrant 3
                     glVertex2f( x1-xLen, y1+yLen ); glVertex2f( x1+xLen, y1-yLen );
                 } 
                 else if (y2> y1 && x2>=x1) 
                 {        // quadrant 1
                     glVertex2f( x1+xLen, y1-yLen ); glVertex2f( x1-xLen, y1+yLen );
                 } 
                 else                       
                 {        // quadrant 2
                     glVertex2f( x1+xLen, y1+yLen ); glVertex2f( x1-xLen, y1-yLen );
                 }
                 glColor4fv(&pen_color_dark.x);
                 glVertex2f( x2, y2 );
             }
             glEnd();
         }
     }
 }
 
 void 
 GLPaint::drawToolForegroundColorPicker( color4& pen_color, int x_position, int y_position )
 {
     unsigned int m_bSelect_color;
     glReadPixels(x_position, y_position, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &m_bSelect_color);
 
     // QT uses BGRA so reverse red and blue
     unsigned char m_bSelect_red = qBlue(m_bSelect_color);
     unsigned char m_bSelect_green = qGreen(m_bSelect_color);
     unsigned char m_bSelect_blue = qRed(m_bSelect_color);
     unsigned char m_bSelect_alpha = qAlpha(m_bSelect_color);
 
     color4 m_bSelect_color4( float(m_bSelect_red) / 255.0f, float(m_bSelect_green) / 255.0f, 
                           float(m_bSelect_blue) / 255.0f, float(m_bSelect_alpha) / 255.0f ); 
 
     pen_color = m_bSelect_color4;
 
     QRgb new_color = qRgba(m_bSelect_red, m_bSelect_green, m_bSelect_blue, m_bSelect_alpha);
 
 	setPenColor( new_color );
 	m_pen_color_button->setPaletteBackgroundColor( new_color ); 
 
     std::ostringstream color_string_stream;
     color_string_stream << (int)m_bSelect_red << ", " << (int)m_bSelect_green << ", ";
     color_string_stream << (int)m_bSelect_blue << ", " << (int)m_bSelect_alpha;
 
     getCurrentColorLabel()->setText( color_string_stream.str().data() );
     getCurrentColorLabel()->setBackgroundColor( new_color );
 
     QColor foreground_color = qRgba(~m_bSelect_red, ~m_bSelect_green, ~m_bSelect_blue, 255); 
     getCurrentColorLabel()->setPaletteForegroundColor( foreground_color );
 }
 
 void 
 GLPaint::drawToolBackgroundColorPicker( color4& fill_color, int x_position, int y_position )
 {
     unsigned int m_bSelect_color;
     glReadPixels(x_position, y_position, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &m_bSelect_color);
 
     // QT uses BGRA so reverse red and blue
     unsigned char m_bSelect_red = qBlue(m_bSelect_color);
     unsigned char m_bSelect_green = qGreen(m_bSelect_color);
     unsigned char m_bSelect_blue = qRed(m_bSelect_color);
     unsigned char m_bSelect_alpha = qAlpha(m_bSelect_color);
 
     color4 m_bSelect_color4( float(m_bSelect_red) / 255.0f, float(m_bSelect_green) / 255.0f, 
                           float(m_bSelect_blue) / 255.0f, float(m_bSelect_alpha) / 255.0f ); 
 
     fill_color = m_bSelect_color4;
 
     QRgb new_color = qRgba(m_bSelect_red, m_bSelect_green, m_bSelect_blue, m_bSelect_alpha);
 
 	setFillColor( new_color );
 	m_fill_color_button->setPaletteBackgroundColor( new_color ); 
 
     std::ostringstream color_string_stream;
     color_string_stream << (int)m_bSelect_red << ", " << (int)m_bSelect_green << ", ";
     color_string_stream << (int)m_bSelect_blue << ", " << (int)m_bSelect_alpha;
 
     getCurrentColorLabel()->setText( color_string_stream.str().data() );
     getCurrentColorLabel()->setBackgroundColor( new_color );
 
     QColor foreground_color = qRgba(~m_bSelect_red, ~m_bSelect_green, ~m_bSelect_blue, 255); 
     getCurrentColorLabel()->setPaletteForegroundColor( foreground_color );
 }
 
 void            
 GLPaint::buildEraserCircleLookupTable()
 {
     if ( getUseGpuEraser() )
     {
         return;
     }
 
     if (m_eraser_circle_lookup_table)
     {
         delete m_eraser_circle_lookup_table;
     }
 
     int eraser_diameter = int( m_point_brush_size );
     int eraser_radius = eraser_diameter / 2;
     eraser_radius = jahstd::Max<int>(eraser_radius, 2);
 
     m_eraser_circle_lookup_table = new bool [(eraser_diameter + 1) * (eraser_diameter + 1)];
 
     for (int x = 0; x <= eraser_diameter; x++)
     {
         float float_x_minus_eraser_radius =  float(x - eraser_radius);
 
         for (int y = 0; y <= eraser_diameter; y++)
         {
             float float_y_minus_eraser_radius = float(y - eraser_radius);
 
             float radius = sqrtf( ( float_x_minus_eraser_radius * float_x_minus_eraser_radius ) + 
                                   ( float_y_minus_eraser_radius * float_y_minus_eraser_radius ) );
 
             if (radius < eraser_radius)
             {
                 // Inside the circle
                 m_eraser_circle_lookup_table[ (y * eraser_diameter) + x ] = true;
             }
             else
             {
                 // Outside the circle
                 m_eraser_circle_lookup_table[ (y * eraser_diameter) + x ] = false;
             }
         }
     }
 }
 
 void            
 GLPaint::captureEraserBuffer()
 {
     if ( getUseGpuEraser() )
     {
         return;
     }
 
     int buffer_size = getFrameWidth() * getFrameHeight();
     m_eraser_buffer = new unsigned int[buffer_size];
     m_eraser_buffer_color4 = new color4[buffer_size];
 
     GLuint texture_id;
 
     if ( getCopyListPtr()->first() && getCopyListPtr()->first()->getTextureId() )
     {
         texture_id = getCopyListPtr()->first()->getTextureId();
     }
     else
     {
          texture_id = getImageLayer()->getTextureData().getTexId();
     }
 
     glBindTexture(GL_TEXTURE_2D, texture_id);
     jahstd::glGetTextureSubImage( 1, 0, getFrameWidth(), getFrameHeight(), 
                                   getTextureWidth(), getTextureHeight(), (unsigned char*)getEraserBuffer() );
 
     unsigned int* eraser_buffer = getEraserBuffer();
 
     for (int i = 0; i < buffer_size; i++)
     {
         unsigned int pixel;
         pixel = eraser_buffer[i];
         // QT uses BGRA so reverse red and blue
         float red = qBlue(pixel) / 255.0f;
         float green = qGreen(pixel) / 255.0f;
         float blue = qRed(pixel) / 255.0f;
         m_eraser_buffer_color4[i].x = red;
         m_eraser_buffer_color4[i].y = green;
         m_eraser_buffer_color4[i].z = blue;
     }
 }
 
 void
 GLPaint::deleteEraserBuffer()
 {
     if ( getUseGpuEraser() )
     {
         return;
     }
 
     delete [] getEraserBuffer();
     delete [] m_eraser_buffer_color4;
 }
 
 void 
 GLPaint::drawEraser( int x_position, int y_position )
 {
 
     if (x_position< 0)
     {
         return;
     }
 
     if ( getUseGpuEraser() )
     {
         glslDrawEraser();
         return;
     }
 
     std::vector< PaintPixelCoords >::iterator iter;
 
     int lower_left_x = getLowerLeftX();
     int lower_left_y = getLowerLeftY();
     int frame_width = getFrameWidth();
     int frame_height = getFrameHeight();
     color4* eraser_buffer = getEraserBufferColor4();
     // Fudge factor because points tend to get written multiple times as the mouse is moved
     float pen_alpha = getPenAlpha() * getPenAlpha();
 
     int eraser_diameter = m_point_brush_size_int;
     int eraser_radius = eraser_diameter / 2;
     eraser_radius = jahstd::Max<int>(eraser_radius, 2);
     glDisable(GL_POINT_SMOOTH);
     glPointSize(1.0f);
 
     if (pen_alpha > 0.99f)
     {
         glDisable(GL_BLEND);
     }
 
     x_position = x1;
     y_position = y1;
 
     for (int x = -eraser_radius; x <= eraser_radius; x++)
     {
         int x_plus_eraser_radius = x + eraser_radius;
         int pixel_x_position = x_position + x;
         int pixel_x_offset = pixel_x_position - lower_left_x;
 
         for (int y = -eraser_radius; y <= eraser_radius; y++)
         {
             color4& pixel = eraser_buffer[0];
 
             if ( isInsideTheEraserCircle(x_plus_eraser_radius, y + eraser_radius) )
             {
                 int pixel_y_position = y_position + y;
                 int pixel_y_offset = pixel_y_position - lower_left_y;
 
                 if (   pixel_x_offset < 0 || pixel_x_offset >= frame_width
                     || pixel_y_offset < 0 || pixel_y_offset >= frame_height 
                     )
                 {
                     continue;
                 }
 
                 if (getImageLayer()->layerStatus)
                 {
                     int buffer_offset = (frame_width * pixel_y_offset) + pixel_x_offset;
                     pixel = eraser_buffer[buffer_offset];
                 }
                 else
                 {
                     pixel.x = 0.0f;
                     pixel.y = 0.0f;
                     pixel.z = 0.0f;
                     pixel.w = 0.0f;
                 }
 
                 glBegin(GL_POINTS);
                 {
                     glColor4f( pixel.x, pixel.y, pixel.z, pen_alpha );
                     glVertex2f(pixel_x_position + 1, pixel_y_position);
                 }
                 glEnd(); // GL_POINTS
             }
         }
     }
 }
 
 void 
 GLPaint::drawToolPaintBucketFillAll()
 {
     color3 fillColor;
     fillColor[0] = m_fill_color->red() / 255.0f;
     fillColor[1] = m_fill_color->green() / 255.0f;
     fillColor[2] = m_fill_color->blue() / 255.0f;
     color4 fill_color( fillColor[0], fillColor[1], fillColor[2], getFillAlpha() );
 
     int x_position = x1;
     int y_position = y1;
 
     unsigned int m_bSelect_color;
 
     int buffer_size = getFrameWidth() * getFrameHeight();
 
     unsigned int* image_buffer = new unsigned int[buffer_size];
 
     glReadPixels(getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
 
     int x_offset = x_position - getLowerLeftX();
     int y_offset = y_position - getLowerLeftY();
 
     if (x_offset > getFrameWidth() || y_offset > getFrameHeight() || x_offset < 0 || y_offset < 0)
     {
         return;
     }
 
     m_bSelect_color = *(image_buffer + ( y_offset * getFrameWidth() ) + x_offset);
 
     // QT uses BGRA so reverse red and blue
     unsigned char m_bSelect_red = qBlue(m_bSelect_color);
     unsigned char m_bSelect_green = qGreen(m_bSelect_color);
     unsigned char m_bSelect_blue = qRed(m_bSelect_color);
 
     color4 m_bSelect_color4( float(m_bSelect_red) / 255.0f, float(m_bSelect_green) / 255.0f, float(m_bSelect_blue) / 255.0f, 1.0f ); 
 
     if (m_bSelect_color4 == fill_color)
     {
         return;
     }
 
     // This fudge factor seems to necessary to deal with inaccuracy in drawn colors
     int color_tolerance = getFillBucketRange() + 4;
 
     unsigned char m_bSelect_red_low = (color_tolerance > (int)m_bSelect_red) ? 0 : m_bSelect_red - color_tolerance;
     unsigned char m_bSelect_green_low = (color_tolerance > (int)m_bSelect_green) ? 0 : m_bSelect_green - color_tolerance;
     unsigned char m_bSelect_blue_low = (color_tolerance > (int)m_bSelect_blue) ? 0 : m_bSelect_blue - color_tolerance;
 
     unsigned char m_bSelect_red_high = ( (m_bSelect_red + color_tolerance ) > 255 ) ? 255 : m_bSelect_red + color_tolerance;
     unsigned char m_bSelect_green_high = ( (m_bSelect_green + color_tolerance ) > 255 ) ? 255 : m_bSelect_green + color_tolerance;
     unsigned char m_bSelect_blue_high = ( (m_bSelect_blue + color_tolerance ) > 255 ) ? 255 : m_bSelect_blue + color_tolerance;
 
     color4 m_bSelect_color_low( float(m_bSelect_red_low) / 255.0f, float(m_bSelect_green_low) / 255.0f, float(m_bSelect_blue_low) / 255.0f, 1.0f);
     color4 m_bSelect_color_high( float(m_bSelect_red_high) / 255.0f, float(m_bSelect_green_high) / 255.0f, float(m_bSelect_blue_high) / 255.0f, 1.0f);
 
     unsigned char new_red = (unsigned char)(fill_color.red() * 255.0f);
     unsigned char new_green = (unsigned char)(fill_color.green() * 255.0f);
     unsigned char new_blue = (unsigned char)(fill_color.blue() * 255.0f);
     unsigned char new_alpha = (unsigned char)(fill_color.alpha() * 255.0f);
 
     // QT uses BGRA so reverse red and blue
     QRgb new_color = qRgba(new_blue, new_green, new_red, new_alpha); 
 
     int x;
     int y;
     int buffer_offset;
 
     for (x = 0; x < getFrameWidth(); x++)
     {
         for (y = 0; y < getFrameHeight(); y++)
         {
             buffer_offset = ( y * getFrameWidth() ) + x;
 
             if ( checkColorRangeAndPosition(x, y, image_buffer, new_color, m_bSelect_color_low, m_bSelect_color_high) )
             {
                 image_buffer[buffer_offset] = new_color;
             }
         }
     }
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image_buffer );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] image_buffer;
 }
 
 void 
 GLPaint::drawToolPaintBucket()
 {
     color3 fillColor;
     fillColor[0] = m_fill_color->red() / 255.0f;
     fillColor[1] = m_fill_color->green() / 255.0f;
     fillColor[2] = m_fill_color->blue() / 255.0f;
     color4 fill_color( fillColor[0], fillColor[1], fillColor[2], getFillAlpha() );
 
     int x_position = x1;
     int y_position = y1;
 
     unsigned int m_bSelect_color;
 
     int buffer_size = getFrameWidth() * getFrameHeight();
 
     unsigned int* image_buffer = new unsigned int[buffer_size];
 
     glReadPixels(getLowerLeftX(), getLowerLeftY(), getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image_buffer);
 
     int x_offset = x_position - getLowerLeftX();
     int y_offset = y_position - getLowerLeftY();
 
     if (x_offset > getFrameWidth() || y_offset > getFrameHeight() || x_offset < 0 || y_offset < 0)
     {
         return;
     }
 
     m_bSelect_color = *(image_buffer + ( y_offset * getFrameWidth() ) + x_offset);
 
     // QT uses BGRA so reverse red and blue
     unsigned char m_bSelect_red = qBlue(m_bSelect_color);
     unsigned char m_bSelect_green = qGreen(m_bSelect_color);
     unsigned char m_bSelect_blue = qRed(m_bSelect_color);
 
     color4 m_bSelect_color4( float(m_bSelect_red) / 255.0f, float(m_bSelect_green) / 255.0f, float(m_bSelect_blue) / 255.0f, 1.0f ); 
 
     if (m_bSelect_color4 == fill_color)
     {
         return;
     }
 
     // This fudge factor seems to necessary to deal with inaccuracy in drawn colors
     int color_tolerance = getFillBucketRange() + 4;
 
     unsigned char m_bSelect_red_low = (color_tolerance > (int)m_bSelect_red) ? 0 : m_bSelect_red - color_tolerance;
     unsigned char m_bSelect_green_low = (color_tolerance > (int)m_bSelect_green) ? 0 : m_bSelect_green - color_tolerance;
     unsigned char m_bSelect_blue_low = (color_tolerance > (int)m_bSelect_blue) ? 0 : m_bSelect_blue - color_tolerance;
 
     unsigned char m_bSelect_red_high = ( (m_bSelect_red + color_tolerance ) > 255 ) ? 255 : m_bSelect_red + color_tolerance;
     unsigned char m_bSelect_green_high = ( (m_bSelect_green + color_tolerance ) > 255 ) ? 255 : m_bSelect_green + color_tolerance;
     unsigned char m_bSelect_blue_high = ( (m_bSelect_blue + color_tolerance ) > 255 ) ? 255 : m_bSelect_blue + color_tolerance;
 
     color4 m_bSelect_color_low( float(m_bSelect_red_low) / 255.0f, float(m_bSelect_green_low) / 255.0f, float(m_bSelect_blue_low) / 255.0f, 1.0f);
     color4 m_bSelect_color_high( float(m_bSelect_red_high) / 255.0f, float(m_bSelect_green_high) / 255.0f, float(m_bSelect_blue_high) / 255.0f, 1.0f);
 
     unsigned char new_red = (unsigned char)(fill_color.red() * 255.0f);
     unsigned char new_green = (unsigned char)(fill_color.green() * 255.0f);
     unsigned char new_blue = (unsigned char)(fill_color.blue() * 255.0f);
     unsigned char new_alpha = (unsigned char)(fill_color.alpha() * 255.0f);
 
     // QT uses BGRA so reverse red and blue
     QRgb new_color = qRgba(new_blue, new_green, new_red, new_alpha); 
 
     //printf("x_position = %d   y_position = %d  color = %08x\n", x_position, y_position, m_bSelect_color);
 
     fillPixelColorRange(x_offset, y_offset, m_bSelect_color_low, m_bSelect_color_high, new_color, image_buffer);
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     refreshCurrentTexture();
     glBindTexture(GL_TEXTURE_2D, getCurrentTextureId() );
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getFrameWidth(), getFrameHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image_buffer );
     updateGL();
     updateTimeSlider( getCurrentHistoryFrame() );
     delete [] image_buffer;
 }
 
 bool 
 GLPaint::checkColorRangeAndPosition(int x_position, int y_position, unsigned int* pixel_buffer, 
                                     QRgb, color4& color_range_low, color4& color_range_high)
 {
     if ( x_position < 0 || x_position >= getFrameWidth() || y_position < 0 || y_position >= getFrameHeight() )
     {
         return false;
     }
 
     int buffer_offset = ( y_position * getFrameWidth() ) + x_position;
     unsigned int* pixel_pointer = pixel_buffer + buffer_offset;
     unsigned int current_color = *pixel_pointer;
 
         // QT uses BGRA so reverse red and blue
     float current_red = qBlue(current_color) / 255.0f;
     float current_green = qGreen(current_color) / 255.0f;
     float current_blue = qRed(current_color) / 255.0f;
 
     //if (current_color == (unsigned int)new_color)
     //{
     //    return true;
     //}
 
     if (  current_red < color_range_low.red() || current_red > color_range_high.red() )
     {
         return false;
     }
 
     if (  current_green < color_range_low.green() || current_green > color_range_high.green() )
     {
         return false;
     }
 
     if (  current_blue < color_range_low.blue() || current_blue > color_range_high.blue() )
     {
         return false;
     }
 
     return true;
 }
 
 bool 
 GLPaint::adjacentPixelSet(bool* image_map, int x_position, int y_position)
 {
     if ( x_position < 0 || x_position >= getFrameWidth() || y_position < 0 || y_position >= getFrameHeight() )
     {
         return false;
     }
 
     int buffer_offset;
     int x;
     int y;
 
     for (x = x_position - 1; x <= x_position + 1; x++)
     {
         for (y = y_position - 1; y <= y_position + 1; y++)
         {
             if (x == x_position && y == y_position)
             {
                 continue;
             }
 
             if ( x < 0 || x > getFrameWidth() || y < 0 || y > getFrameHeight() )
             {
             }
             else
             {
                 buffer_offset = ( y * getFrameWidth() ) + x;
 
                 if (image_map[buffer_offset])
                 {
                     return true;
                 }
             }
         }
     }
 
     return false;
 }
 
 void
 GLPaint::evaluateAndSetPixelColor(int x, int y, unsigned int* pixel_buffer, bool* image_map,
                                   QRgb new_color, color4& color_range_low, color4& color_range_high,
                                   bool& point_found)
 {
     if ( checkColorRangeAndPosition(x, y, pixel_buffer, new_color, color_range_low, color_range_high) )
     {
         if ( adjacentPixelSet(image_map, x, y) )
         {
             int buffer_offset = ( y * getFrameWidth() ) + x;
             pixel_buffer[buffer_offset] = new_color;
             image_map[buffer_offset] = true;
             point_found = true;
         }
     }
 }
 
 void 
 GLPaint::fillPixelColorRange(int x_position, int y_position, 
                              color4& color_range_low, color4& color_range_high, QRgb new_color, 
                              unsigned int* pixel_buffer)
 {
     int map_size = getFrameWidth() * getFrameHeight();
     bool* image_map = new bool[map_size];
 
     int x;
     int y;
     int buffer_offset;
 
     for (int i = 0; i < map_size; i++)
     {
         image_map[i] = false;
     }
 
     int max_radius = jahstd::Max<int>( getFrameWidth(), getFrameHeight() );
     buffer_offset = ( y_position * getFrameWidth() ) + x_position;
     image_map[buffer_offset] = true;
     pixel_buffer[buffer_offset] = new_color;
 
     bool point_found = true;
     int radius;
 
     for (radius = 1; radius < max_radius && point_found; radius++)
     {
         point_found = false;
         
         y = y_position - radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position - radius;
         evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
             color_range_low, color_range_high, point_found);
 
         x = x_position - radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         y = y_position - radius;
         evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
             color_range_low, color_range_high, point_found);
 
         y = y_position + radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position - radius;
         evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
             color_range_low, color_range_high, point_found);
 
         x = x_position + radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         y = y_position - radius;
         evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
             color_range_low, color_range_high, point_found);
     }
 
     max_radius = radius;
 
     for (radius = max_radius; radius >= 0; radius--)
     {
         y = y_position - radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position - radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         y = y_position + radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position + radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
     }
 
     for (radius = 0; radius < max_radius; radius++)
     {
         y = y_position - radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position - radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         y = y_position + radius;
 
         for (x = x_position - radius; x <= x_position + radius; x++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
 
         x = x_position + radius;
 
         for (y = y_position - radius; y <= y_position + radius; y++)
         {
             evaluateAndSetPixelColor(x, y, pixel_buffer, image_map, new_color, 
                                      color_range_low, color_range_high, point_found);
         }
     }
 
     delete [] image_map;
 }
 
 
 void GLPaint::addLayer(void)
 {
     //add a new layer
 
 
 }
 
 
 void GLPaint::changeParticle() {
 
 }
 
 void GLPaint::changeFontDraw() {
 
 
 }
 
 void GLPaint::changeObjectDraw() {
 
 
 }
 
 
 void GLPaint::keyCliplayer() {
 
 }
 
 void GLPaint::keyClipkey() 
 {
 
 }
 
 void 
 GLPaint::copyTextureToQimage(QImage& qimage, unsigned int texture_id)
 {
     int pixel_count = getTextureWidth() * getTextureHeight();
     unsigned int* buffer = new unsigned int[pixel_count];
     glBindTexture(GL_TEXTURE_2D, texture_id);
     glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, buffer);
     qimage.create(getTextureWidth(), getTextureHeight(), 32);
     qimage.loadFromData( (const unsigned char*)buffer, pixel_count * 4 );
     qimage = m_pCore->convertToGLFormat(qimage);
     delete [] buffer;
 }
 
 void 
 GLPaint::copyQimageToTexture(unsigned int texture_id, QImage& qimage)
 {
     glBindTexture(GL_TEXTURE_2D, texture_id);
     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, getTextureWidth(), getTextureHeight(), GL_BGRA, GL_UNSIGNED_BYTE, (void*)qimage.bits() );
 }
 
 bool            
 GLPaint::isInsideTheEraserCircle(int x, int y)
 {
     
     return *(getEraserCircleLookupTable() + (m_point_brush_size_int * y) + x);
 }
 
 static const char* a_over_b_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = a_color + ( b_color * (1.0 - a_color.w) );                   \n"
         "   }                                                                               \n"
 };
 
 static const char* b_over_a_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = b_color + ( a_color * (1.0 - b_color.w) );                   \n"
         "   }                                                                               \n"
 };
 
 static const char* a_in_b_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = a_color * b_color.w;                                         \n"
         "   }                                                                               \n"
 };
 
 
 static const char* b_in_a_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = b_color * a_color.w;                                         \n"
         "   }                                                                               \n"
 };
 
 static const char* a_out_b_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = a_color * (1.0 - b_color.w);                                 \n"
         "   }                                                                               \n"
 };
 
 static const char* b_out_a_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = b_color * (1.0 - a_color.w);                                 \n"
         "   }                                                                               \n"
 };
 
 static const char* a_atop_b_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = (a_color * b_color.w) + ( b_color * (1.0 - a_color.w) );     \n"
         "   }                                                                               \n"
 };
 
 static const char* b_atop_a_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                               \n"
         "   uniform sampler2D      a_texture;                                               \n"
         "                                                                                   \n"
         "   void main()                                                                     \n"
         "   {                                                                               \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                     \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                     \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                         \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                         \n"
         "                                                                                   \n"
         "       gl_FragColor = (b_color * a_color.w) + ( a_color * (1.0 - b_color.w) );     \n"
         "   }                                                                               \n"
 };
 
 static const char* a_xor_b_fragment_shader =
 {
         "   uniform sampler2D      b_texture;                                                       \n"
         "   uniform sampler2D      a_texture;                                                       \n"
         "                                                                                           \n"
         "   void main()                                                                             \n"
         "   {                                                                                       \n"
         "       vec2    a_position = gl_TexCoord[0].st;                                             \n"
         "       vec2    b_position = gl_TexCoord[1].st;                                             \n"
         "       vec4    a_color = texture2D(a_texture, a_position);                                 \n"
         "       vec4    b_color = texture2D(b_texture, b_position);                                 \n"
         "                                                                                           \n"
         "       gl_FragColor = ( a_color * (1.0 - b_color.w) ) + ( b_color * (1.0 - a_color.w) );   \n"
         "   }                                                                                       \n"
 };
 
 void        
 GLPaint::glslAOverBComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, a_over_b_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 void        
 GLPaint::glslBOverAComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, b_over_a_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void        
 GLPaint::glslAInBComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, a_in_b_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void        
 GLPaint::glslBInAComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, b_in_a_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void 
 GLPaint::glslAOutBComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, a_out_b_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void 
 GLPaint::glslBOutAComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, b_out_a_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void 
 GLPaint::glslAAtopBComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, a_atop_b_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void 
 GLPaint::glslBAtopAComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, b_atop_a_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 void 
 GLPaint::glslAXorBComposite(GLuint a_texture_id, GLuint b_texture_id)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
 
     if (shader_program == NULL)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, a_xor_b_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     executeComposite(a_texture_id, b_texture_id, shader_program_handle, shader_program);
 }
 
 
 void
 GLPaint::executeComposite(GLuint a_texture_id, GLuint b_texture_id, 
                           GLhandleARB shader_program_handle, GlslProgram* shader_program)
 {
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glBindTexture(GL_TEXTURE_2D, a_texture_id);
     glEnable(GL_TEXTURE_2D);
 
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glBindTexture(GL_TEXTURE_2D, b_texture_id);
     glEnable(GL_TEXTURE_2D);
 
     glUseProgramObjectARB(shader_program_handle);
 
     shader_program->setTextureHandle("a_texture", 0);
     shader_program->setTextureHandle("b_texture", 1);
 
     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 
     glBegin(GL_QUADS);
     {
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
         glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() -  ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f );
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, m_texture_ratio.x, 0.0f);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, m_texture_ratio.x, 0.0f);
         glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() - ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, m_texture_ratio.x, m_texture_ratio.y);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, m_texture_ratio.x, m_texture_ratio.y);
         glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, m_texture_ratio.y);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, m_texture_ratio.y);
         glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
     }
     glEnd(); // GL_QUADS
 
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glDisable(GL_TEXTURE_2D);
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glUseProgramObjectARB(0);
 
     if ( getClipIsStillImage() && !getApplyEffectToAllFrames() && !getAdvanceFrameAfterEffectApplication() )
     {
 	    incrementHistoryCount();
     }
 
     captureDrawingArea();
     updateTimeSlider( getCurrentHistoryFrame() );
     advanceVideoFrameIfRequested();
 }
 
 static const char* eraser_fragment_shader =
 {
         "   uniform sampler2D       eraser_texture;                                                 \n"
         "   uniform sampler2D       base_texture;                                                   \n"
         "   uniform vec2            eraser_position;                                                \n"
         "   uniform int             eraser_src_blend_mode;                                          \n"
         "   uniform int             eraser_dst_blend_mode;                                          \n"
         "   uniform float           eraser_radius;                                                  \n"
         "   uniform float           eraser_transparency;                                            \n"
         "   uniform int             JAH_GL_ZERO;                                                    \n"
         "   uniform int             JAH_GL_ONE;                                                     \n"
         "   uniform int             JAH_GL_SRC_COLOR;                                               \n"
         "   uniform int             JAH_GL_ONE_MINUS_SRC_COLOR;                                     \n"
         "   uniform int             JAH_GL_SRC_ALPHA;                                               \n"
         "   uniform int             JAH_GL_ONE_MINUS_SRC_ALPHA;                                     \n"
         "   uniform int             JAH_GL_DST_ALPHA;                                               \n"
         "   uniform int             JAH_GL_ONE_MINUS_DST_ALPHA;                                     \n"
         "   uniform int             JAH_GL_DST_COLOR;                                               \n"
         "   uniform int             JAH_GL_ONE_MINUS_DST_COLOR;                                     \n"
         "                                                                                           \n"
         "   void main()                                                                             \n"
         "   {                                                                                       \n"
         "       vec2    texture_coords = gl_TexCoord[0].st;                                         \n"
         "       vec4    eraser_color = texture2D(eraser_texture, texture_coords);                   \n"
         "       vec4    base_color = texture2D(base_texture, texture_coords);                       \n"
         "       vec2    position = gl_FragCoord.xy;                                                 \n"
         "       vec2    delta = eraser_position - position;                                         \n"
         "       float   radius = length(delta);                                                     \n"
         "                                                                                           \n"
         "       if (radius < eraser_radius)                                                         \n"
         "       {                                                                                   \n"
         "           if (eraser_src_blend_mode == JAH_GL_ZERO)                                       \n"
         "           {                                                                               \n"
         "               eraser_color = vec4(0.0, 0.0, 0.0, 0.0);                                    \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_ONE)                                   \n"
         "           {                                                                               \n"
         "               //eraser_color = eraser_color;                                              \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_SRC_COLOR)                             \n"
         "           {                                                                               \n"
         "               eraser_color *= eraser_color;                                               \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_ONE_MINUS_SRC_COLOR)                   \n"
         "           {                                                                               \n"
         "               eraser_color *= (1.0 - eraser_color);                                       \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_SRC_ALPHA)                             \n"
         "           {                                                                               \n"
         "               eraser_color *= eraser_transparency;                                        \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_ONE_MINUS_SRC_ALPHA)                   \n"
         "           {                                                                               \n"
         "               eraser_color *= (1.0 - eraser_transparency);                                \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_DST_ALPHA)                             \n"
         "           {                                                                               \n"
         "               eraser_color *= base_color.w;                                               \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_ONE_MINUS_DST_ALPHA)                   \n"
         "           {                                                                               \n"
         "               eraser_color *= (1.0 - base_color.w);                                       \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_DST_COLOR)                             \n"
         "           {                                                                               \n"
         "               eraser_color *= base_color;                                                 \n"
         "           }                                                                               \n"
         "           else if (eraser_src_blend_mode == JAH_GL_ONE_MINUS_DST_COLOR)                   \n"
         "           {                                                                               \n"
         "               eraser_color *= (1.0 - base_color);                                         \n"
         "           }                                                                               \n"
         "                                                                                           \n"
         "           if (eraser_dst_blend_mode == JAH_GL_ZERO)                                       \n"
         "           {                                                                               \n"
         "               base_color = vec4(0.0, 0.0, 0.0, 0.0);                                      \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_ONE)                                   \n"
         "           {                                                                               \n"
         "               //base_color = base_color;                                                  \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_SRC_COLOR)                             \n"
         "           {                                                                               \n"
         "               base_color *= eraser_color;                                                 \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_ONE_MINUS_SRC_COLOR)                   \n"
         "           {                                                                               \n"
         "               base_color *= (1.0 - eraser_color);                                         \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_SRC_ALPHA)                             \n"
         "           {                                                                               \n"
         "               base_color *= eraser_transparency;                                          \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_ONE_MINUS_SRC_ALPHA)                   \n"
         "           {                                                                               \n"
         "               base_color *= (1.0 - eraser_transparency);                                  \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_DST_ALPHA)                             \n"
         "           {                                                                               \n"
         "               base_color *= base_color.w;                                                 \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_ONE_MINUS_DST_ALPHA)                   \n"
         "           {                                                                               \n"
         "               base_color *= (1.0 - base_color.w);                                         \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_DST_COLOR)                             \n"
         "           {                                                                               \n"
         "               base_color *= base_color;                                                   \n"
         "           }                                                                               \n"
         "           else if (eraser_dst_blend_mode == JAH_GL_ONE_MINUS_DST_COLOR)                   \n"
         "           {                                                                               \n"
         "               base_color *= (1.0 - base_color);                                           \n"
         "           }                                                                               \n"
         "                                                                                           \n"
         "           gl_FragColor = eraser_color + base_color;                                       \n"
         "       }                                                                                   \n"
         "       else                                                                                \n"
         "       {                                                                                   \n"
         "           gl_FragColor = base_color;                                                      \n"
         "       }                                                                                   \n"
         "   }                                                                                       \n"
 };
 
 void
 GLPaint::glslDrawEraser(bool initialize)
 {
     GLsizei                             message_length;
     char                                message[1000];
 
     static GLhandleARB                  shader_program_handle = 0;
     static std::vector<GlslShader*>     shader_vector;
     static GlslShader*                  fragment_shader;
     static GlslProgram*                 shader_program = NULL;
     static GLint                        eraser_position_handle = 0;
     static GLint                        eraser_radius_handle = 0;
     static GLint                        eraser_transparency_handle = 0;
     static GLint                        eraser_src_blend_mode_handle = 0;
     static GLint                        eraser_dst_blend_mode_handle = 0;
     static GLint                        JAH_GL_ZERO_handle = 0;
     static GLint                        JAH_GL_ONE_handle = 0;
     static GLint                        JAH_GL_SRC_COLOR_handle = 0;
     static GLint                        JAH_GL_ONE_MINUS_SRC_COLOR_handle = 0;
     static GLint                        JAH_GL_SRC_ALPHA_handle = 0;
     static GLint                        JAH_GL_ONE_MINUS_SRC_ALPHA_handle = 0;
     static GLint                        JAH_GL_DST_ALPHA_handle = 0;
     static GLint                        JAH_GL_ONE_MINUS_DST_ALPHA_handle = 0;
     static GLint                        JAH_GL_DST_COLOR_handle = 0;
     static GLint                        JAH_GL_ONE_MINUS_DST_COLOR_handle = 0;
 
     if (shader_program == NULL || initialize)
     {
         fragment_shader = new GlslShader(GL_FRAGMENT_SHADER_ARB, eraser_fragment_shader);
         shader_vector.push_back(fragment_shader);
         shader_program = new GlslProgram(shader_vector);
         shader_program_handle = *shader_program->getGlHandle();
         eraser_position_handle = glGetUniformLocationARB(shader_program_handle, "eraser_position");
         eraser_radius_handle = glGetUniformLocationARB(shader_program_handle, "eraser_radius");
         eraser_transparency_handle = glGetUniformLocationARB(shader_program_handle, "eraser_transparency");
         eraser_src_blend_mode_handle = glGetUniformLocationARB(shader_program_handle, "eraser_src_blend_mode");
         eraser_dst_blend_mode_handle = glGetUniformLocationARB(shader_program_handle, "eraser_dst_blend_mode");
         JAH_GL_ZERO_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ZERO");
         JAH_GL_ONE_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ONE");
         JAH_GL_SRC_COLOR_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_SRC_COLOR");
         JAH_GL_ONE_MINUS_SRC_COLOR_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ONE_MINUS_SRC_COLOR");
         JAH_GL_SRC_ALPHA_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_SRC_ALPHA");
         JAH_GL_ONE_MINUS_SRC_ALPHA_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ONE_MINUS_SRC_ALPHA");
         JAH_GL_DST_ALPHA_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_DST_ALPHA");
         JAH_GL_ONE_MINUS_DST_ALPHA_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ONE_MINUS_DST_ALPHA");
         JAH_GL_DST_COLOR_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_DST_COLOR");
         JAH_GL_ONE_MINUS_DST_COLOR_handle = glGetUniformLocationARB(shader_program_handle, "JAH_GL_ONE_MINUS_DST_COLOR");
         glGetInfoLogARB(shader_program_handle, 1000, &message_length, message);
     }
 
     if (initialize)
     {
         return;
     }
 
     GLuint eraser_texture_id;
     GLuint base_texture_id = getCurrentTextureId();
 
     int eraser_diameter = m_point_brush_size_int;
     int eraser_radius = eraser_diameter / 2;
     float eraser_transparency = getPenAlpha() * getPenAlpha();
 
     if ( getCopyListPtr()->first() && getCopyListPtr()->first()->getTextureId() )
     {
         eraser_texture_id = getCopyListPtr()->first()->getTextureId();
     }
     else
     {
         eraser_texture_id = getImageLayer()->getTextureData().getTexId();
     }
 
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glBindTexture(GL_TEXTURE_2D, eraser_texture_id);
     glEnable(GL_TEXTURE_2D);
 
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glBindTexture(GL_TEXTURE_2D, base_texture_id);
     glEnable(GL_TEXTURE_2D);
 
     glUseProgramObjectARB(shader_program_handle);
 
     shader_program->setTextureHandle("eraser_texture", 0);
     shader_program->setTextureHandle("base_texture", 1);
 
     glUniform2fARB( eraser_position_handle, float(x1), float(y1) );
     glUniform1fARB( eraser_radius_handle, float(eraser_radius) );
     glUniform1fARB( eraser_transparency_handle, eraser_transparency );
     glUniform1iARB( eraser_src_blend_mode_handle, (int)getSrcBlendFactor() );
     glUniform1iARB( eraser_dst_blend_mode_handle, (int)getDstBlendFactor() );
     glUniform1iARB( JAH_GL_ZERO_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_ZERO) ) );
     glUniform1iARB( JAH_GL_ONE_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_ONE) ) );
     glUniform1iARB( JAH_GL_SRC_COLOR_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_SRC_COLOR) ) );
     glUniform1iARB( JAH_GL_ONE_MINUS_SRC_COLOR_handle, GLint(JAH_GL_ONE_MINUS_SRC_COLOR) );
     glUniform1iARB( JAH_GL_SRC_ALPHA_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_SRC_ALPHA) ) );
     glUniform1iARB( JAH_GL_ONE_MINUS_SRC_ALPHA_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_ONE_MINUS_SRC_ALPHA) ) );
     glUniform1iARB( JAH_GL_DST_ALPHA_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_DST_ALPHA) ) );
     glUniform1iARB( JAH_GL_ONE_MINUS_DST_ALPHA_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_ONE_MINUS_DST_ALPHA) ) );
     glUniform1iARB( JAH_GL_DST_COLOR_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_DST_COLOR) ) );
     glUniform1iARB( JAH_GL_ONE_MINUS_DST_COLOR_handle, GLint( getJahGlBlendModeValueVector(JAH_GL_ONE_MINUS_DST_COLOR) ) );
 
     glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
 
     // This is necessary becauseof some bizarre nvidia driver bug
     glDisable(GL_BLEND);
 
     glBegin(GL_QUADS);
     {
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, 0.0f);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, 0.0f);
         glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() -  ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f );
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, m_texture_ratio.x, 0.0f);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, m_texture_ratio.x, 0.0f);
         glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() - ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, m_texture_ratio.x, m_texture_ratio.y);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, m_texture_ratio.x, m_texture_ratio.y);
         glVertex3f( getScreenCenterX() + ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
 
         glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0.0f, m_texture_ratio.y);
         glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0.0f, m_texture_ratio.y);
         glVertex3f( getScreenCenterX() - ( getImageWidthDiv2() * m_pCore->zoomVal ), 
                     getScreenCenterY() + ( getImageHeightDiv2() * m_pCore->zoomVal ), 
                     0.0f);
     }
     glEnd(); // GL_QUADS
 
     glActiveTextureARB(GL_TEXTURE1_ARB);
     glDisable(GL_TEXTURE_2D);
     glActiveTextureARB(GL_TEXTURE0_ARB);
     glUseProgramObjectARB(0);
 
 }
