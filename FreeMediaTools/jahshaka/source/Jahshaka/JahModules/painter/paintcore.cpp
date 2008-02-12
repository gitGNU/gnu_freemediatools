/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "paint.h"
#include <sstream>
#include <qpushbutton.h>
#include <InputLCD.h>
#include <qlistview.h>

/*
| Draw with the active tool using this canvas's two points, x1,y1 and x2,y2.
*/
void GLPaint::drawWithActiveTool() 
{
	frameEdited = true;

	GLint   minPointSize = 1;
	color3 penColor,  penColorDark,  penColorLight,
		fillColor, fillColorDark, fillColorLight;

	//switch from QT colors to openGL colors, 0...255  to  0.0...1.0
	GLfloat gradientDegree = m_gradient_degree / 255.0;
	penColor[0] = m_pen_color->red()   / 255.0;
	penColor[1] = m_pen_color->green() / 255.0;
	penColor[2] = m_pen_color->blue()  / 255.0;

	penColorLight[0] = penColor[0];
	penColorLight[1] = penColor[1];
	penColorLight[2] = penColor[2];

	//penColorDark  = penColor;     need to write a copy constructor to do this for me
	penColorDark[0]  = penColor[0];
	penColorDark[1]  = penColor[1];
	penColorDark[2]  = penColor[2];

	for (int i = 0; i < 3; i++) 
	{
		if  (penColorLight[i]  + gradientDegree > 1) penColorLight[i] = 1;
		else penColorLight[i] += gradientDegree;

		if  (penColorDark[i]   - gradientDegree < 0) penColorDark[i]  = 0;
		else penColorDark[i]  -= gradientDegree;
	}

	fillColor[0] = m_fill_color->red()   / 255.0;
	fillColor[1] = m_fill_color->green() / 255.0;
	fillColor[2] = m_fill_color->blue()  / 255.0;

	fillColorLight[0] = fillColor[0];
	fillColorLight[1] = fillColor[1];
	fillColorLight[2] = fillColor[2];

	fillColorDark[0]  = fillColor[0];
	fillColorDark[1]  = fillColor[1];
	fillColorDark[2]  = fillColor[2];

	for (int ii = 0; ii < 3; ii++) 
	{
		if  (fillColorLight[ii]  + gradientDegree > 1) fillColorLight[ii] = 1;
		else fillColorLight[ii] += gradientDegree;

		if  (fillColorDark[ii]   - gradientDegree < 0) fillColorDark[ii]  = 0;
		else fillColorDark[ii]  -= gradientDegree;
	}

    color4 pen_color( penColor[0], penColor[1], penColor[2], getPenAlpha() );
    color4 pen_color_light( penColorLight[0], penColorLight[1], penColorLight[2], getPenAlpha() );
    color4 pen_color_dark( penColorDark[0], penColorDark[1], penColorDark[2], getPenAlpha() );

    color4 fill_color( fillColor[0], fillColor[1], fillColor[2], getFillAlpha() );
    color4 fill_color_light( fillColorLight[0], fillColorLight[1], fillColorLight[2], getFillAlpha() );
    color4 fill_color_dark( fillColorDark[0], fillColorDark[1], fillColorDark[2], getFillAlpha() );


	// use my brush size for points and lines
	glLineWidth(m_brush_size);

    glEnable(GL_BLEND);
    glBlendFunc( getSrcBlendFactor(), getDstBlendFactor() );

    switch(m_active_tool) 
    {
    case none    :
        break;

    case pen     :

        drawToolPen( pen_color );
        break;

    case line      :

        drawToolLine( m_brush_size, pen_color_light, pen_color_dark, minPointSize,
            x1, y1, x2, y2 );
        break;

    case rectangle :

        drawToolRect( m_brush_size, pen_color, pen_color_light, pen_color_dark, minPointSize,
            x1, y1, x2, y2  );
        break;

    case rectangleFilled :

        drawToolRectFilled( m_brush_size, minPointSize,
            pen_color,  pen_color_light,  pen_color_dark, fill_color, fill_color_light, fill_color_dark,
            x1,  y1,  x2,  y2  );
        break;

    case circle :

        drawToolCircle( pen_color, gradientDegree, x1, y1, x2, y2 );
        break;

    case circleFilled :

        drawToolCircleFilled( pen_color, fill_color, gradientDegree, x1, y1, x2, y2  );
        break;

    case triangle :

        drawToolTri ( pen_color, pen_color_light, pen_color_dark,
            x1, y1, x2, y2  );
        break;

    case triangleFilled :

        drawToolTriFilled( pen_color,  pen_color_light,  pen_color_dark,
            fill_color, fill_color_light, fill_color_dark,
            x1, y1, x2, y2  );
        break;

    case eraser :

        drawEraser(x1, y1);
        break;

    case foregroundColorPicker :

        drawToolForegroundColorPicker(pen_color, x1, y1);
        break;

    case backgroundColorPicker :

        drawToolBackgroundColorPicker(fill_color, x1, y1);
        break;

    default :
        break;
    }

    glDisable(GL_BLEND);
}

////////////////////////////////////////////////////
//Jah UI code to be implemented
void GLPaint::updateSliders() 
{   
}

void GLPaint::clicked( QListViewItem* item) 
{ 
    if (!item)
    {
        return;
    }

    getImageLayer()->layerStatus = ( (QCheckListItem*)item )->isOn();
    updateGL();
}


////////////////////////////////////////////////////
//history code

void GLPaint::slotHistoryPrevious() 
{ 
    previousframeanimation();
}

void GLPaint::slotHistoryNext()     
{ 
	nextframeanimation();
}

void GLPaint::slotDelete()     
{ 
	deleteFrames(getCurrentHistoryFrame(), 1);
}

void GLPaint::slotCopyOne()     
{ 
	copyFrames(getCurrentHistoryFrame(), 1);
}

void GLPaint::slotPaste()     
{ 
	pasteFrames( getCurrentHistoryFrame() );
}

bool            
GLPaint::getApplyEffectToAllFrames()
{
    return( m_apply_effect_to_all_frames_qcheckbox->isChecked() );
}

bool            
GLPaint::getAdvanceFrameAfterEffectApplied()
{
    return( m_advance_frame_qcheckbox->isChecked() );
}


void            
GLPaint::executeEffect(PAINT_EFFECT_TYPE effect_type)
{
    if ( getApplyEffectToAllFrames() )
    {
        setStopRequested(false);  
        //firstframeanimation();

        while ( m_nCurrentFrame <= m_nMaxFrame && getApplyEffectToAllFrames() && !getStopRequested() )
        {
            qApp->processEvents();

            executePaintEffect(effect_type);
            nextframeanimation();

            if (m_nCurrentFrame == m_nMaxFrame)
            {
                executePaintEffect(effect_type);
                break;
            }
        }
    }
    else
    {
        executePaintEffect(effect_type);
        advanceVideoFrameIfRequested();
    }

    if ( getStopRequested() )
    {
        setStopRequested(false);
    }
}


////////////////////////////////////////////////////
//paint slots

void GLPaint::slotInvert()       
{ 
    executeEffect(INVERT_EFFECT);
}

void GLPaint::slotFade()         
{ 
    executeEffect(FADE_EFFECT);
}

void GLPaint::slotIntensify()    
{ 
    executeEffect(INTENSIFY_EFFECT);
}

void GLPaint::slotEmboss()    
{ 
    executeEffect(EMBOSS_EFFECT);
}

void GLPaint::slotBlur()         
{ 
    executeEffect(BLUR_EFFECT);
}

void GLPaint::slotSharpen()      
{ 
    executeEffect(SHARPEN_EFFECT);
}

void GLPaint::slotEdgeDetectX()  
{ 
    executeEffect(EDGE_DETECT_X_EFFECT);
}

void GLPaint::slotEdgeDetectY()  
{ 
    executeEffect(EDGE_DETECT_Y_EFFECT);
}

void
GLPaint::slotSetCompositeType()
{
    QPopupMenu *menu = new QPopupMenu( 0 );

    menu->insertItem( "A over B", A_OVER_B_COMPOSITE_EFFECT);
    menu->insertItem( "B over A", B_OVER_A_COMPOSITE_EFFECT);
    menu->insertItem( "A in B", A_IN_B_COMPOSITE_EFFECT);
    menu->insertItem( "B in A", B_IN_A_COMPOSITE_EFFECT);
    menu->insertItem( "A out B", A_OUT_B_COMPOSITE_EFFECT);
    menu->insertItem( "B out A", B_OUT_A_COMPOSITE_EFFECT);
    menu->insertItem( "A atop B", A_ATOP_B_COMPOSITE_EFFECT);
    menu->insertItem( "B atop A", B_ATOP_A_COMPOSITE_EFFECT);
    menu->insertItem( "A xor B", A_XOR_B_COMPOSITE_EFFECT);

    menu->setMouseTracking(TRUE);

    PAINT_EFFECT_TYPE type = 
        (PAINT_EFFECT_TYPE)menu->exec( m_select_composite_button->mapToGlobal( QPoint( 0, m_select_composite_button->height() + 1 ) ) );

    if (type < A_OVER_B_COMPOSITE_EFFECT || type > A_XOR_B_COMPOSITE_EFFECT) 
    {
        type = A_OVER_B_COMPOSITE_EFFECT;
    }

    setCurrentCompositeType(type);

    switch (type)
    {
        case A_OVER_B_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("A over B");
            break;
        }
        case B_OVER_A_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("B over A");
            break;
        }
        case A_IN_B_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("B in A");
            break;
        }
        case B_IN_A_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("B in A");
            break;
        }
        case A_OUT_B_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("A out B");
            break;
        }
        case B_OUT_A_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("B out A");
            break;
        }
        case A_ATOP_B_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("A atop B");
            break;
        }
        case B_ATOP_A_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("B atop A");
            break;
        }
        case A_XOR_B_COMPOSITE_EFFECT :
        {
            m_composite_button->setText("A xor B");
            break;
        }
        default :
        {
            m_composite_button->setText("A Over B");
            break;
        }
    }

    delete menu;
}

void
GLPaint::slotComposite()
{
	if ( jahstd::glslSupport() )
    {
        executeEffect( getCurrentCompositeType() );
    }
}

void GLPaint::slotSobel()        
{ 
    executeEffect(SOBEL_EFFECT);
}

void GLPaint::slotLaplacian()    
{ 
    executeEffect(LAPLACIAN_EFFECT);
}

void GLPaint::slotLaplacian2()   
{ 
    executeEffect(LAPLACIAN2_EFFECT);
}

void GLPaint::slotLapOfGauss()   
{ 
    executeEffect(LAP_OF_GAUSS_EFFECT);
}

void GLPaint::slotClear()        
{ 
	clear(); 
}

void GLPaint::slotRefresh()        
{ 
	updateGL(); 
}

void
GLPaint::slotTurnOffApplyEffectsToAllFrames()
{
    if ( !getAdvanceFrameAfterEffectApplied() )
    {
        return;
    }

    if ( m_apply_effect_to_all_frames_qcheckbox->isChecked() )
    {
        m_apply_effect_to_all_frames_qcheckbox->toggle();
    }
}

void
GLPaint::slotTurnOffAdvanceFrame()
{
    if ( !getApplyEffectToAllFrames() )
    {
        return;
    }

    if ( m_advance_frame_qcheckbox->isChecked() )
    {
        m_advance_frame_qcheckbox->toggle();
    }
}

////////////////////////////////////////////
//brush and gradient sizing

void GLPaint::slotBrushSize(int value)  
{ 
	setBrushSize(value); 
}

void GLPaint::slotGradientDegree(int value) 
{  
	setGradientDegree(value); 
}

void GLPaint::slotFillBucketRange(int value) 
{  
	setFillBucketRange(value); 
}

void 
GLPaint::showPaintSlider(enum PaintSliders slider)
{
    switch(slider)
    {
        case PEN_ALPHA_SLIDER :
        {
            m_pen_alpha_slider->show();
            m_pen_alpha_lcd->show();
            m_pen_alpha_label->show();
            break;
        }
        case FILL_ALPHA_SLIDER :
        {
            m_fill_alpha_slider->show();
            m_fill_alpha_lcd->show();
            m_fill_alpha_label->show();
            break;
        }
        case BACKGROUND_ALPHA_SLIDER :
        {
            m_background_alpha_slider->show();
            m_background_alpha_lcd->show();
            m_background_alpha_label->show();
            break;
        }
        case BRUSH_SIZE_SLIDER :
        {
            m_brush_size_slider->show();
            m_brush_size_lcd->show();
            m_brush_size_label->show();
            break;
        }
        case GRADIENT_SLIDER :
        {
            m_gradient_slider->show();
            m_gradient_lcd->show();
            m_gradient_label->show();
            break;
        }
        case BUCKET_RANGE_SLIDER :
        {
            m_bucket_range_slider->show();
            m_bucket_range_lcd->show();
            m_bucket_range_label->show();
            break;
        }
        default :
        {
            break;
        }
    }
}

void 
GLPaint::hidePaintSliders()
{
    m_pen_alpha_slider->hide();
    m_pen_alpha_lcd->hide();
    m_pen_alpha_label->hide();
    m_fill_alpha_slider->hide();
    m_fill_alpha_lcd->hide();
    m_fill_alpha_label->hide();
    m_background_alpha_slider->hide();
    m_background_alpha_lcd->hide();
    m_background_alpha_label->hide();
    m_brush_size_slider->hide();
    m_brush_size_lcd->hide();
    m_brush_size_label->hide();
    m_gradient_slider->hide();
    m_gradient_lcd->hide();
    m_gradient_label->hide();
    m_bucket_range_slider->hide();
    m_bucket_range_lcd->hide();
    m_bucket_range_label->hide();
}


////////////////////////////////////////////
//tool m_bSelection slots

void GLPaint::slotPen()          
{ 
	activateTool(pen); 
    m_pen_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Brush Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Brush Size");
}

void GLPaint::slotLine()         
{ 
	activateTool(line); 
    m_line_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotRectangle()    
{ 
	activateTool(rectangle); 
    m_rectangle_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotRectangleFilled()  
{ 
	activateTool(rectangleFilled); 
    m_rectangle_filled_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(FILL_ALPHA_SLIDER);
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotCircle()       
{ 
	activateTool(circle); 
    m_circle_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotCircleFilled()  
{ 
	activateTool(circleFilled); 
    m_circle_filled_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(FILL_ALPHA_SLIDER);
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotTriangle()     
{ 
	activateTool(triangle); 
    m_triangle_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotTriangleFilled()   
{ 
	activateTool(triangleFilled); 
    m_triangle_filled_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Pen Alpha");
    showPaintSlider(FILL_ALPHA_SLIDER);
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Line Width");
    showPaintSlider(GRADIENT_SLIDER);
}

void GLPaint::slotPaintBucket()   
{ 
	activateTool(paintBucket); 
    m_paintbucket_button->setFocus();

    hidePaintSliders();
    showPaintSlider(FILL_ALPHA_SLIDER);
    showPaintSlider(BUCKET_RANGE_SLIDER);
}

void GLPaint::slotPaintBucketFillAll()   
{ 
	activateTool(paintBucketFillAll); 
    m_paintbucket_fill_all_button->setFocus();

    hidePaintSliders();
    showPaintSlider(FILL_ALPHA_SLIDER);
    showPaintSlider(BUCKET_RANGE_SLIDER);
}

void GLPaint::slotEraser()   
{ 
	activateTool(eraser); 
    m_eraser_button->setFocus();

    hidePaintSliders();
    showPaintSlider(PEN_ALPHA_SLIDER);
    m_pen_alpha_label->setText("Eraser Alpha");
    showPaintSlider(BRUSH_SIZE_SLIDER);
    m_brush_size_label->setText("Eraser Size");
}

void GLPaint::slotForegroundColorPicker()   
{ 
	activateTool(foregroundColorPicker); 
    m_foreground_color_picker_button->setFocus();

    hidePaintSliders();
}

void GLPaint::slotBackgroundColorPicker()   
{ 
	activateTool(backgroundColorPicker); 
    m_background_color_picker_button->setFocus();

    hidePaintSliders();
}

////////////////////////////////////////////////
// color m_bSelection slots

void GLPaint::slotPenColor()  
{
    incrementHistoryCount(true);

    captureDrawingArea();

	QColor m_bSelect_color = QColorDialog::getColor( penColor(), this );

	if ( m_bSelect_color.isValid() ) 
	{
		setPenColor( m_bSelect_color );
		m_pen_color_button->setPaletteBackgroundColor( m_bSelect_color ); 
	}

    historyRemoveCurrent();
    updateGL();

    unsigned char m_bSelect_red = qRed( m_bSelect_color.rgb() );
    unsigned char m_bSelect_green = qGreen( m_bSelect_color.rgb() );
    unsigned char m_bSelect_blue = qBlue( m_bSelect_color.rgb() );
    unsigned char m_bSelect_alpha = qAlpha( m_bSelect_color.rgb() );

    std::ostringstream color_string_stream;
    color_string_stream << (int)m_bSelect_red << ", " << (int)m_bSelect_green << ", ";
    color_string_stream << (int)m_bSelect_blue << ", " << (int)m_bSelect_alpha;

    getCurrentColorLabel()->setText( color_string_stream.str().data() );
    getCurrentColorLabel()->setBackgroundColor( m_bSelect_color );

    QColor foreground_color = qRgba(~m_bSelect_red, ~m_bSelect_green, ~m_bSelect_blue, 255); 
    getCurrentColorLabel()->setPaletteForegroundColor( foreground_color );
}

void GLPaint::slotFillColor() 
{
    incrementHistoryCount(true);

    captureDrawingArea();

	QColor m_bSelect_color = QColorDialog::getColor( fillColor(), this );

	if ( m_bSelect_color.isValid() ) 
	{
		setFillColor( m_bSelect_color );
		m_fill_color_button->setPaletteBackgroundColor( m_bSelect_color ); 
	}

   	historyRemoveCurrent();

    updateGL();

    unsigned char m_bSelect_red = qRed( m_bSelect_color.rgb() );
    unsigned char m_bSelect_green = qGreen( m_bSelect_color.rgb() );
    unsigned char m_bSelect_blue = qBlue( m_bSelect_color.rgb() );
    unsigned char m_bSelect_alpha = qAlpha( m_bSelect_color.rgb() );

    std::ostringstream color_string_stream;
    color_string_stream << (int)m_bSelect_red << ", " << (int)m_bSelect_green << ", ";
    color_string_stream << (int)m_bSelect_blue << ", " << (int)m_bSelect_alpha;

    getCurrentColorLabel()->setText( color_string_stream.str().data() );
    getCurrentColorLabel()->setBackgroundColor( m_bSelect_color );

    QColor foreground_color = qRgba(~m_bSelect_red, ~m_bSelect_green, ~m_bSelect_blue, 255); 
    getCurrentColorLabel()->setPaletteForegroundColor( foreground_color );
}

void GLPaint::slotBackgroundColor() 
{
    if ( getClipIsStillImage() )
    {
	    incrementHistoryCount(true);
    }

    captureDrawingArea();

	QColor m_bSelect_color = QColorDialog::getColor( backgroundColor(), this );

	if ( m_bSelect_color.isValid() ) 
	{
		setBackgroundColor( m_bSelect_color );
		m_background_color_button->setPaletteBackgroundColor( m_bSelect_color ); 
	}

    if ( getClipIsStillImage() )
    {
	    historyRemoveCurrent();
    }

	clear();

    unsigned char m_bSelect_red = qRed( m_bSelect_color.rgb() );
    unsigned char m_bSelect_green = qGreen( m_bSelect_color.rgb() );
    unsigned char m_bSelect_blue = qBlue( m_bSelect_color.rgb() );
    unsigned char m_bSelect_alpha = qAlpha( m_bSelect_color.rgb() );

    std::ostringstream color_string_stream;
    color_string_stream << (int)m_bSelect_red << ", " << (int)m_bSelect_green << ", ";
    color_string_stream << (int)m_bSelect_blue << ", " << (int)m_bSelect_alpha;

    getCurrentColorLabel()->setText( color_string_stream.str().data() );
    getCurrentColorLabel()->setBackgroundColor( m_bSelect_color );

    QColor foreground_color = qRgba(~m_bSelect_red, ~m_bSelect_green, ~m_bSelect_blue, 255); 
    getCurrentColorLabel()->setPaletteForegroundColor( foreground_color );

}

void
GLPaint::setBackgroundColor( QColor newColor )
{
    *m_background_color = newColor;
    m_background_color_button->setPaletteBackgroundColor( newColor ); 
    QImage temp_qimage(getTextureWidth(), getTextureHeight(), 32);
    temp_qimage.fill( newColor.rgb() );
    //temp_qimage.fill( black );
    glBindTexture(GL_TEXTURE_2D, getImageLayerTextureId() );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, getTextureWidth(), getTextureHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexSubImage2D( GL_TEXTURE_2D, 0, 0, 0, getTextureWidth(), getTextureHeight(), GL_BGRA, GL_UNSIGNED_BYTE, temp_qimage.mirror(false, true).bits() );
    m_current_history_frame--;
    clear(true);
}

bool            
GLPaint::getAdvanceFrameAfterEffectApplication()
{
    if ( m_advance_frame_qcheckbox->isChecked() )
    {
        return true;
    }

    return false;
}

void            
GLPaint::advanceVideoFrameIfRequested()
{
    if ( getAdvanceFrameAfterEffectApplication() )
    {
        nextframeanimation();
    }
}

bool            
GLPaint::getOverwriteStillFrames()
{
    return m_overwrite_still_frames_qcheckbox->isChecked();
}

