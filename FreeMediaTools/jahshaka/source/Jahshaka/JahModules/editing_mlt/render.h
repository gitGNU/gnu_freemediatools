/*******************************************************************************
**
** The header file for the Jahshaka/MLT editing module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#ifndef JAHNLE_RENDER_H
#define JAHNLE_RENDER_H

//TODO remove #include <mlt++/Mlt.h>
#include <qstring.h>

class QImage;
class QStringList;

namespace jahnle
{
    class Render
    {
        private:
      //            Mlt::PushConsumer *m_consumer;
            QString m_file_name;
        public:
            Render( );
            ~Render( );
            bool fetchProfile( const char *path, bool pal, double ar );
	    //            bool executeProfile( Mlt::Properties& properties, bool pal, double ar );
	    //bool createFormatsList(Mlt::Properties& properties, QStringList& label_list, QString& path);
	    //bool selectFormat( Mlt::Properties &properties, QString path );
	    //void createProfilesList(Mlt::Properties& properties, QStringList& label_list);
	    //bool selectProfile( Mlt::Properties& properties );
	    //            bool push( Mlt::Frame * );
            bool push( QImage & );

	    //Mlt::Consumer *getConsumer( ) { return m_consumer; }

            QString getFileName( );
    };
}

#endif
