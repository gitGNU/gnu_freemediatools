#include "windowsiconprovider.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <qpainter.h>
#include <qimage.h>
#include <qbitmap.h>

WindowsIconProvider::WindowsIconProvider( QWidget *parent, const char *name )
: QFileIconProvider( parent, name ), smallicons(false)
{
#ifdef WIN32
    printf(" WindowsIconProvider constr \n");

    pixw = GetSystemMetrics( SM_CXICON );  // for large icons
    pixh = GetSystemMetrics( SM_CYICON );
    pixsmw = GetSystemMetrics( SM_CXSMICON ); // for small icons
    pixsmh = GetSystemMetrics( SM_CYSMICON );

    HKEY k;
    HICON si;
    int r;
    QString s;
    UINT res;

     BYTE value1[255]; 
   DWORD dwcount=255;

   /*
   QImage img; 
   img.setAlphaBuffer(true );
  // defaultFolder.convertFromImage( img );
           int pr, pg, pb, pa;
   for ( int y=0; y<img.height(); y++ )
        {
            for ( int x=0; x<img.width(); x++ )
            {
                QRgb *p = (QRgb *)img.scanLine(y) + x;
                pr=qRed(*p);
                pg=qGreen(*p);
                pb=qBlue(*p);
                pa = qAlpha(*p);

                *p = qRgba( pr,pg,pb, 200 );
               
            }
        }
           defaultFolder.convertFromImage( img );
           */
   

    // ---------- get default folder pixmap
    r = RegOpenKeyExA( HKEY_CLASSES_ROOT,
        "folder\\DefaultIcon",
        0, KEY_READ, &k );
    if ( r == ERROR_SUCCESS ) {

        s = RegQueryValueEx(k ,  NULL, NULL, 0, value1, &dwcount  );
       
        RegCloseKey( k );

        QStringList lst = QStringList::split( ",", s );

      //ORIG  res = ExtractIconExA( (const char*)lst[ 0 ].simplifyWhiteSpace().latin1(),
        //    lst[ 1 ].simplifyWhiteSpace().toInt(),
        //    0, &si, 1 );

            res = ExtractIconExA( (char*)"shell32.dll", 3, &si, 0, 1 ); // swap &si with 0 for small icons.

        if ( res != -1 ) {
            defaultFolder.resize( pixw, pixh );
           //? initPixmap( defaultFolder );
            QPainter p( &defaultFolder );
            DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
            p.end();
            if ( !defaultFolder.isNull() )
                defaultFolder.setMask( defaultFolder.createHeuristicMask() );
            defaultFolderSmall.resize( pixsmw, pixsmh );
           //? initPixmap( defaultFolder );
            QPainter ps( &defaultFolderSmall );
            DrawIconEx( ps.handle(), 0, 0, si, pixsmw, pixsmh, 0, NULL,  DI_NORMAL );
            ps.end();
            if ( !defaultFolderSmall.isNull() )
                defaultFolderSmall.setMask( defaultFolderSmall.createHeuristicMask() ); 

            DestroyIcon( si );
        } else {
            printf("res = ExtractIconExA FAIL defaultfolder\n");
        }
    } else {
           printf("res = ExtractIconExA FAIL key failed to open\n");
        RegCloseKey( k );
    }

    //------------------------------- get default file pixmap
    res = ExtractIconExA( (char*)"shell32.dll",
        70, &si, 0, 1 ); // could use 0 for generic file.

    if ( res != -1 ) {
        defaultFile.resize( pixw, pixh );
        QPainter p( &defaultFile );
        DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
        p.end();
        if ( !defaultFile.isNull() )
            defaultFile.setMask( defaultFile.createHeuristicMask() );
         
        defaultFileSmall.resize( pixsmw, pixsmh );
        QPainter ps( &defaultFileSmall );
        DrawIconEx( ps.handle(), 0, 0, si, pixsmw, pixsmh, 0, NULL,  DI_NORMAL );
        ps.end();
        if ( !defaultFileSmall.isNull() )
            defaultFileSmall.setMask( defaultFileSmall.createHeuristicMask() );
    
        DestroyIcon( si );
    } else {
        printf("res = ExtractIconExA FAIL file icon extract\n");
    }

    //------------------------------- get default exe pixmap
    res = ExtractIconExA( (char*) "shell32.dll",
        2, 0, &si, 1 );

    if ( res != -1 ) {
        defaultExe.resize( pixw, pixh );
        QPainter p( &defaultExe );
        DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
        p.end();
        if ( !defaultExe.isNull() )
            defaultExe.setMask( defaultExe.createHeuristicMask() );
        
        defaultExeSmall.resize( pixsmw, pixsmh );
        QPainter ps( &defaultExeSmall );
        DrawIconEx( ps.handle(), 0, 0, si, pixsmw, pixsmh, 0, NULL,  DI_NORMAL );
        ps.end();
        if ( !defaultExeSmall.isNull() )
            defaultExeSmall.setMask( defaultExeSmall.createHeuristicMask() );
        DestroyIcon( si );
    } else {
        printf("res = ExtractIconExA FAIL exe icon extract\n");
    }

    // -------------------------------- make a hard drive icon
res = ExtractIconExA( (char*) "shell32.dll",
        8, 0, &si, 1 );

    if ( res != -1 ) {
        hardDrive.resize( pixw, pixh );
        QPainter p( &hardDrive );
        DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
        p.end();
        if ( !hardDrive.isNull() )
          hardDrive.setMask( hardDrive.createHeuristicMask() );
        
        hardDriveSmall.resize( pixsmw, pixsmh );
        QPainter ps( &hardDriveSmall );
        DrawIconEx( ps.handle(), 0, 0, si, pixsmw, pixsmh, 0, NULL,  DI_NORMAL );
        ps.end();
        if ( !hardDriveSmall.isNull() )
            hardDriveSmall.setMask( hardDriveSmall.createHeuristicMask() );
        DestroyIcon( si );
    } else {
        hardDriveSmall = defaultFolderSmall;
        hardDrive = defaultFolder;
    }
    #endif 
}


WindowsIconProvider::~WindowsIconProvider()
{
}

void WindowsIconProvider::setSmallIcons( bool state )
{ 
    smallicons = state; 
}

QPixmap * WindowsIconProvider::pixmap( QFileInfo *fi, bool overidesmallicon )
{
    #ifdef WIN32
    // NOTE default iconprovider can be used like this, but only gives small folders etc.
    /* const QPixmap* ppp = QFileIconProvider::pixmap( *fi );
    QPixmap *npp = const_cast<QPixmap*>(ppp);
    return ( npp );
    */


    bool smallic = smallicons;
    if ( overidesmallicon )
        smallic = overidesmallicon;
        
    QString ext = fi->extension().upper();
    QString key = ext;
    ext.prepend( "." );
    QMap< QString, QPixmap >::Iterator it;

    if ( fi->isDir() ) 
    {
        return ( smallic ? &defaultFolderSmall : &defaultFolder );
    }
    else if ( ext.lower() == ".exe" )
    {
        return ( smallic ? &defaultExeSmall : &defaultExe );
    }
    else 
    {
        return ( smallic ? &defaultFileSmall : &defaultFile );
    }

// ***********************************************************


    // ***TO FIX ***********
    if ( fi->isDir() ) {
        printf("providing pixmap for folder %s\n", fi->filePath().latin1() );

        return &defaultFolder;
    } else if ( ext.lower() != ".exe" ) {
        it = cache.find( key );
        if ( it != cache.end() )
            return &( *it );

        HKEY k, k2;
        int r = RegOpenKeyExA( HKEY_CLASSES_ROOT,
            ext.latin1(),
            0, KEY_READ, &k );
        QString s;
        if ( r == ERROR_SUCCESS ) {
            s = RegQueryValueEx(k ,  NULL, NULL, NULL, NULL, NULL  );

        } else {
            cache[ key ] = defaultFile;
            RegCloseKey( k );
            return &defaultFile;
        }
        RegCloseKey( k );

        r = RegOpenKeyExA( HKEY_CLASSES_ROOT,
            QString( s + "\\DefaultIcon" ).latin1() ,
            0, KEY_READ, &k2 );
        if ( r == ERROR_SUCCESS ) {
         s = RegQueryValueEx(k2 ,  NULL, NULL, NULL, NULL, NULL  );

        } else {
            cache[ key ] = defaultFile;
            RegCloseKey( k2 );
            return &defaultFile;
        }
        RegCloseKey( k2 );

        QStringList lst = QStringList::split( ",", s );

        HICON si;
        UINT res;
        res = ExtractIconExA( (const char*)lst[ 0 ].simplifyWhiteSpace().latin1(),
            lst[ 1 ].simplifyWhiteSpace().toInt(),
            0, &si, 1 );

        if ( res != -1 ) {
            pix.resize( pixw, pixh );

            QPainter p( &pix );
            DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
            p.end();
            pix.setMask( pix.createHeuristicMask() );
            DestroyIcon( si );
        } else {
            pix = defaultFile;
        }

        cache[ key ] = pix;
        return &pix;
    } else {

        printf("providing pixmap for ext %s\n", ext.latin1() );

        HICON si;
        UINT res;
        res = ExtractIconExA( (const char*)fi->absFilePath().latin1(),
            -1,
            0, 0, 1 );

        if ( res == 0 ) {
            return &defaultExe;
        } else {
            res = ExtractIconExA( (char*)fi->absFilePath().latin1(),
                res - 1,
                0, &si, 1 );
        }

        if ( res != -1 ) {
            pix.resize( pixw, pixh );

            QPainter p( &pix );
            DrawIconEx( p.handle(), 0, 0, si, pixw, pixh, 0, NULL,  DI_NORMAL );
            p.end();
            pix.setMask( pix.createHeuristicMask() );
            DestroyIcon( si );
        } else {
            pix = defaultExe;
        }

        return &pix;
    }

    // can't happen!
    return 0;
    
    #endif
}

bool WindowsIconProvider::isADrive( QFileInfo *fi )
{
    if ( !fi->isDir() )
        return false;
    const QFileInfoList* roots = QDir::drives();
    QPtrListIterator<QFileInfo> i(*roots);
    QFileInfo* fid;
    bool result = false;
    while ( (fid = *i) ) 
    {
        printf("drive %s\n", fid->absFilePath().latin1() );
        if (fid->absFilePath() == fi->absFilePath() )
        {printf("in its a drive: abs file path %s %s\n", fi->absFilePath().latin1(), fi->baseName(true).latin1() );
           return true;
        }
        ++i;
    }
    return result;
}
