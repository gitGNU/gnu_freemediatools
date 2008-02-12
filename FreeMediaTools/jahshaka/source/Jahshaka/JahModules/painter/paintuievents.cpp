
/*******************************************************************************
**
** The source file for the Jahshaka animation module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "paint.h"
#include <glcore.h>
#include <timer.h>
#include <qcursor.h>

void GLPaint::mousePressEvent( QMouseEvent* mouse_position ) 
{
    setClearState();
    mousePressed = true;


    if ( Globals::getGlRenderer().find("RADEON Xpress 200 Series") == std::string::npos )
    {
        // Software cursors screw up all kinds of things on RADEON Xpress 200 Series
        // There are probably more hardware/driver combinations of this bug, but this is the '
        // only one I know about for sure.
        getMainworldQframe()->setCursor( Qt::CrossCursor ); 
    }

    x1 = mouse_position->x();
    y1 = m_pCore->height() - mouse_position->y();
    x2 = x1;
    y2 = y1;

	if (m_active_tool == pen)
	{
		m_pixel_coords.clear( );
		PaintPixelCoords pixel( x1, y1, x2, y2 );
		m_pixel_coords.push_back( pixel );
	}
    else if (m_active_tool == eraser)
    {
        if ( getClipIsStillImage() && !getOverwriteStillFrames() )
        {
            incrementHistoryCount();
        }

        if ( !getUseGpuEraser() )
        {
            captureDrawingArea();
            captureEraserBuffer();
            buildEraserCircleLookupTable();
        }
   }
    else if (m_active_tool == paintBucketFillAll)
    {
        drawToolPaintBucketFillAll();
        return;
    }
    else if (m_active_tool == paintBucket)
    {
        drawToolPaintBucket();
        return;
    }
    else if (m_active_tool == foregroundColorPicker || m_active_tool == backgroundColorPicker)
    {
        updateGL( );

        if (m_active_tool == paintBucket || m_active_tool == paintBucketFillAll)
        {
            advanceVideoFrameIfRequested();
        }
    }
}

void GLPaint::mouseReleaseEvent( QMouseEvent * ) 
{
    getMainworldQframe()->setCursor( Qt::ArrowCursor ); 

    if (!mousePressed)
    {
        return;
    }

    mousePressed = false;

    if (m_active_tool == paintBucket || m_active_tool == paintBucketFillAll)
    {
        advanceVideoFrameIfRequested();
        return;
    }
    else if (m_active_tool == foregroundColorPicker || m_active_tool == backgroundColorPicker)
    {
        return;
    }

	// Draw with the currently active tool
    updateGL();

    if ( getClipIsStillImage() && !getOverwriteStillFrames() && m_active_tool != eraser )
    {
	    incrementHistoryCount();
    }

	// Capture the image
    captureDrawingArea();
    updateTimeSlider( getCurrentHistoryFrame() );

    if (m_active_tool == eraser)
    {
        deleteEraserBuffer();
    }

	// Clear the state so we don't draw again (result should be cached now)
	m_pixel_coords.clear( );
	x1 = x2 = y1 = y2 = -1;

	// Redraw the captured texture to clip everything
    updateGL();

	// And move to the next position
    advanceVideoFrameIfRequested();
}

void GLPaint::mouseMoveEvent( QMouseEvent* mouse_position ) 
{
    if (   m_active_tool == paintBucket || m_active_tool == paintBucketFillAll
        || m_active_tool == foregroundColorPicker || m_active_tool == backgroundColorPicker)
    {
        return;
    }

    if ( mousePressed ) 
    {  // drawing is currently occurring
        if ( m_active_tool == pen ) 
        {
            x1 = x2;
            y1 = y2;
            x2 = mouse_position->x();
            y2 = m_pCore->height() - mouse_position->y();

			PaintPixelCoords pixel( x1, y1, x2, y2 );
			if ( m_pixel_coords.empty( ) || pixel != *( m_pixel_coords.end( ) - 1 ) )
				m_pixel_coords.push_back( pixel );

			m_pCore->update( );
        } 
        else if (m_active_tool == eraser)
        {
            x1 = x2;
            y1 = y2;
            x2 = mouse_position->x();
            y2 = m_pCore->height() - mouse_position->y();
            captureDrawingArea();
            updateGL();
       }
        else 
        {
            x2 = mouse_position->x();
            y2 = m_pCore->height() - mouse_position->y();
            m_pCore->update();
        }
    }
    else
    {
        setClearState();
    }
}
