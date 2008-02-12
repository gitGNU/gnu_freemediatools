/*******************************************************************************
**
** The source file for the Jahshaka OpenMediaLib MLT class
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "mediaobject_mlt.h"
#include <mlt++/Mlt.h>
using namespace Mlt;

// We only need to initialise the library once
static int initialised = 0;

mediaObjectMlt::mediaObjectMlt(void) :
	producer( NULL )
{
	if ( initialised == 0 )
	{
		Factory::init( );
		initialised = 1;
	}
}

mediaObjectMlt::~mediaObjectMlt(void)
{
	delete producer;
}

void mediaObjectMlt::printinfo(void)
{
}

bool mediaObjectMlt::openMedia(QString mediafile)
{
	// Remove the current producer if it exists
	delete producer;

	// Temporary work around for a missing const (mlt fix broke ABI)
	char temp[ 4096 ];
	strcpy( temp, mediafile.latin1( ) );

	// Open the file specified
	producer = new Producer( temp );

	// If it's valid, we need to ensure that a colour space conversion is applied
	if ( producer != NULL && producer->is_valid( ) )
	{
		// Create the filter
		Filter filter( "avcolour_space" );

		// Special case to allow loading of westley/non-trivial composite producers
		// This enforces processing in yuv422, while guaranteeing the correct output
		// NB: avcolour_space must be present
		filter.set( "forced", mlt_image_yuv422 );

		// Attach the filter to the producer
		producer->attach( filter );
	}

	// Return true if valid
	return producer != NULL && producer->is_valid( );
}

bool mediaObjectMlt::closeMedia(void)
{
	delete producer;
	producer = NULL;
	return true;
}

QImage mediaObjectMlt::getVideoFrame(int frame)
{
	// This is the returned image
	QImage copy;

	// Seek to the position required
	producer->seek( frame );

	// Get the MLT frame from the producer
	Frame *f = producer->get_frame( );

	// Initialise arguments
	mlt_image_format format = mlt_image_rgb24a;
	int w = f->get_int( "width" );
	int h = f->get_int( "height" );

	// Fallback on real width/height when available
	// NB: This is required for cached image sequences (see aspect ratio handling)
	if ( f->get_int( "real_width" ) )
		w = f->get_int( "real_width" );
	if ( f->get_int( "real_height" ) )
		h = f->get_int( "real_height" );

	// Force deinterlacing
	f->set( "consumer_deinterlace", 1 );

	// Typically, we want the correct aspect ratio, but in the modules, we don't since
	// the modules resolutions don't take sample aspect ratio into account - we only make
	// a couple of exceptions for projects and dv (so that 16:9 is displayed sort of correctly)
	if ( getCorrectAspectRatio( ) )
	{
		w = int( h * ( f->get_double( "aspect_ratio" ) * ( double( w ) / double( h ) ) ) );
	}
	else
	{
		// Special case to recover options
		if ( producer->get_int( "jah_project" ) )
		{
			f->pass_values( *producer, "jah_" );
			w = f->get_int( "display_width" );
			h = f->get_int( "display_height" );
		}

		if ( f->get_data( "dvdata" ) != NULL && producer->get( "_westley" ) == NULL )
			f->set( "rescale.interp", "none" );
	}

	// Get the image
	uint8_t *image = f->get_image( format, w, h );

	// Sanity checks for invalid loads
	if ( image != NULL && w > 0 && h > 0 )
	{
		// Create a QImage using the MLT image
		QImage temp( image, w, h, 32, NULL, 0, QImage::IgnoreEndian );
		temp.setAlphaBuffer( true );

		// Do a deep copy
		copy = temp.copy( );
	}

	// Delete the MLT Frame
	delete f;

	// Return copy
	return copy;
}

int mediaObjectMlt::getVideoLength()
{
	return ( int )producer->get_playtime( );
}

void    mediaObjectMlt::getAudioFrame(int)
{
}

bool mediaObjectMlt::isMediaOpen()
{
	return producer != NULL && producer->is_valid( );
}

QString mediaObjectMlt::getFileName()
{
	return producer->get( "resource" );
}

int mediaObjectMlt::getNumberFrames()
{
	return ( int )producer->get_playtime( );
}


