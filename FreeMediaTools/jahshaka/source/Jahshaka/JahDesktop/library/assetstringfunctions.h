#ifndef ASSETSTRINGFUNCTIONS_H
#define ASSETSTRINGFUNCTIONS_H

#include <qstring.h>
#include <qdatetime.h>
#include <qfileinfo.h>

// Various string functions used by widgets to present asset data in human readable QStrings etc.
// Prob this should be somewhere else...
class AssetStringFunctions
{
public:

    AssetStringFunctions() {};
    ~AssetStringFunctions() {};

    static QString formattedDateString( QFileInfo& fi )
    {
        QDate datecreated = fi.created().date();
        QString qsdatecreated = datecreated.toString("d MMM yyyy");
        return qsdatecreated;
    };

    static QString frameSizeString( QImage& theimage )
    {
        QString conv;
        conv.setNum( theimage.width() ); 
        QString framesizelabel = conv;
        conv.setNum( theimage.height() );
        framesizelabel += " x " + conv;
        return framesizelabel;
    };

    static QString fileSizeString( QFileInfo& fi )
    {
        int size = int(fi.size()); // size in bytes.
        QString qssize, conv;
        if ( size < 1000 ) 
        {   conv.setNum ( size );
        qssize = conv + " bytes";
        }
        else if ( size < 1000000 )
        {   conv.setNum( int(size/1000));
        qssize = conv + " k";
        }
        else if ( size < 1000000000 )
        {   conv.setNum( int(size/1000000));
        double msize = double(size/1000000);
        conv = QString::number( msize, 'f', 1 );    
        qssize = conv  + " MB";
        }
        else 
        {   conv.setNum( int(size/1000000000) );
        double msize = double(size/10000000);
        conv = QString::number( msize, 'f', 1 );    
        qssize = conv + " GB";
        }
        return qssize;

    };
};

#endif
