/*******************************************************************************
**
** The source file for the Jahshaka editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include <iostream>
using namespace std;

#include "render.h"
using namespace Mlt;
using namespace jahnle;

// QT Widgets used
#include <qimage.h>
#include <qfiledialog.h>
#include <qinputdialog.h>
#include <qerrormessage.h>

#include <sys/types.h>
#include <sys/stat.h>

// Local non-class functions

static void profileCleanup( void *arg )
{
    Properties *properties = ( Properties * )arg;
    delete properties;
}

bool 
Render::createFormatsList(Mlt::Properties& properties, QStringList& label_list, QString& path)
{
    // Obtain the directory and verify that it exists
    QDir directory( path );

    std::string path_string = path.data();

    if ( directory.exists( ) )
    {
        // Iterate through the contents of the directory
        const QFileInfoList *list = directory.entryInfoList();
        QFileInfoListIterator it( *list );

        // Now we scan all the files in this directory
        while ( it.current( ) != 0 )
        {
            // Get the file name
            QString name = it.current( )->fileName( );
            std::string name_string = name.data();

            // If it's not a directory and it has the right extension, then process it
            if ( !it.current( )->isDir( ) && name.endsWith( ".jpl" ) )
            {
                // Create a new properties object and load it
                Properties *contents = new Properties( );
                contents->load( path + name );

                // Get the label
                const char *label = contents->get( "label" );

                // Make sure there is a label and it's unique
                if ( label != NULL && properties.get_data( label ) == NULL )
                {
                    properties.set( label, contents, 0, profileCleanup );
                    label_list << label;
                }
                else
                {
                    cerr << "Invalid jpl - " << name << " invalid label." << endl;
                    delete contents;
                }
            }

            ++ it;
        }

        return true;
    }

    return false;
}

bool Render::selectFormat( Properties &properties, QString path )
{
    bool ret = false;
    QStringList label_list;

    if ( createFormatsList(properties, label_list, path) )
    {
        // Only continue if we have formats to offer
        if ( label_list.count( ) > 0 )
        {
            QString res = QInputDialog::getItem( "Render", "Select a format:", label_list, 0, false, &ret );
            printf("selectFormat() selected %s\n", res.data() );

            if ( ret )
            {
                properties.set( "format", res );
            }
        }
    }

    return ret;
}

void Render::createProfilesList(Properties& properties, QStringList& label_list)
{
    QString format = properties.get( "format" );
    std::string format_string = format.data();
    Properties *available = ( Properties * )properties.get_data( format );
    Properties *profile_list = new Properties( );

    // Iterate through each profile and convert each to a properties object
    for ( int i = 0; i < available->count( ); i ++ )
    {
        if ( strcmp( available->get_name( i ), "label" ) )
        {
            char *values = available->get( i );
            Tokeniser tokens( values, "," );
            Properties *data = new Properties( );
            for ( int j = 0; j < tokens.count( ); j ++ )
                data->parse( tokens.get( j ) );
            profile_list->set( data->get( "label" ), data, 0, profileCleanup );
            label_list << data->get( "label" );
        }
    }

    // Keep the list safe
    properties.set( "profile_list", profile_list, 0, profileCleanup );
}

bool Render::selectProfile( Properties &properties )
{
    bool ret = false;
    QStringList label_list;

    createProfilesList(properties, label_list);

    // Only continue if we have formats to offer
    if ( label_list.count( ) > 0 )
    {
        QString res = QInputDialog::getItem( "Render", "Select a profile:", label_list, 0, false, &ret );

        printf("selectProfile() selected %s\n", res.data() );
        if ( ret )
            properties.set( "profile", res );
    }

    return ret;
}

static bool selectFile( Properties &properties )
{
    Properties *list = ( Properties * )properties.get_data( "profile_list" );
    Properties *profile = ( Properties * )list->get_data( properties.get( "profile" ) );
    QString extension = profile->get( "extension" );

    // Obtain file to write to
    QFileDialog *fd = new QFileDialog( NULL, "Select output file name", TRUE );
    fd->setMode( QFileDialog::AnyFile );
    fd->setViewMode( QFileDialog::Detail );
    if ( fd->exec() != QDialog::Accepted )
        return false;

    // Get the file name
    QString file_name = fd->selectedFile();

    // Make sure the file name ends with the correct extension
    if ( !file_name.endsWith( extension ) )
        file_name += extension;

    properties.set( "file_name", file_name );

    return true;
}

// The render class

Render::Render( ) :
    m_consumer( NULL ),
    m_file_name( "" )
{
}

Render::~Render( )
{
    delete m_consumer;
}

bool Render::executeProfile(Properties& properties, bool pal, double ar)
{
	Factory::init( );

    bool ret = false;
    Properties *list = ( Properties * )properties.get_data( "profile_list" );
    Properties *profile = ( Properties * )list->get_data( properties.get( "profile" ) );
    char *consumer = profile->get( "consumer" );
    char *file_name = properties.get( "file_name" );
    char command[ 4096 ] = "";

	if ( file_name == NULL )
	{
		if ( !selectFile( properties ) )
			return false;
    	file_name = properties.get( "file_name" );
	}

    // Create the consumer requested
    if ( profile->get( "dvpipe" ) == NULL )
    {
        m_consumer = new PushConsumer( consumer, file_name );
    }
#if !defined( WIN32 )
    else
    {
        char pipe[ 4096 ];
        sprintf( pipe, "%s/.jahshaka/pipe.dv", getenv( "HOME" ) );
        mkfifo( pipe, 0777 );
        sprintf( command, profile->get( "dvpipe" ), pipe, file_name );
        m_consumer = new PushConsumer( consumer, pipe );
    }
#endif

    // Only continue if it's valid
    if ( m_consumer->is_valid( ) )
    {
        // Set the file name in case the user of the class wants it during processing
        m_file_name = file_name;

        // Some profiles will turn rendering off
        if ( profile->get_int( "render_off" ) )
            m_consumer->set_render( 0, 0, 0.0 );
        else
            m_consumer->set_render( m_consumer->get_int( "width" ), m_consumer->get_int( "height" ), ar );

        // Turn on the best scaling and deinterlace methods
        m_consumer->set( "rescale", "hyper" );
        m_consumer->set( "deinterlace", 1 );
        m_consumer->set( "deinterlace_method", "weave" );

        // Pass all propertes from the profile to the consumer
        m_consumer->inherit( *profile );

        // Allow ntsc/pal variance
        m_consumer->pass_values( *profile, pal ? "pal_" : "ntsc_" );

        // Attempt to start the consumer
        ret = m_consumer->start( ) == 0;

        // Check if there's a command to run and run it
        if ( strcmp( command, "" ) )
            system( command );
    }

    return ret;
}

bool Render::fetchProfile( const char *path, bool pal, double ar )
{
    bool ret = false;
    Properties properties;

    if ( selectFormat( properties, QString( path ) + "plugins/encoders/" ) )
    {
        if ( selectProfile( properties ) )
        {
            if ( selectFile( properties ) )
            {
                ret = executeProfile(properties, pal, ar);

                // Report the bad news if need be
                if ( !ret )
                {
                    QErrorMessage error( NULL );
                    error.message( "Unable to create selected profile - aborting." );
                    error.exec( );
                }
            }
        }
    }

    return ret;
}

bool Render::push( Mlt::Frame *frame )
{
    return m_consumer->push( frame ) == 0;
}

bool Render::push( QImage &image )
{
    Frame *frame = m_consumer->construct( image.numBytes( ) );
    uint8_t *buffer = ( uint8_t * )frame->get_data( "image" );
    memcpy( buffer, image.bits( ), image.numBytes( ) );
    frame->set( "format", mlt_image_rgb24a );
    frame->set( "width", image.width( ) );
    frame->set( "height", image.height( ) );
    frame->set( "progressive", 1 );
    frame->set( "aspect_ratio", 1 );
    if ( image.width( ) == 720 && image.height( ) == 576 )
        frame->set( "aspect_ratio", 59.0 / 54.0 );
    else if ( image.width( ) == 720 && image.height( ) == 480 )
        frame->set( "aspect_ratio", 10.0 / 11.0 );
    bool ret = push( frame );
    delete frame;
    return ret;
}

QString Render::getFileName( )
{
    return m_file_name;
}

