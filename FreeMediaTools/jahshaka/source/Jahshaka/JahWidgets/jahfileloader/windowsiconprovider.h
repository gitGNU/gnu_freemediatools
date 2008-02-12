#ifndef WINDOWSICONPROVIDER_H
#define WINDOWSICONPROVIDER_H

#include <qpixmap.h>
#include <qmap.h>

class JahFileIconView;

class WindowsIconProvider : public QFileIconProvider
{
public:
    WindowsIconProvider( QWidget *parent=0, const char *name=0 );
    ~WindowsIconProvider();

    QPixmap * pixmap( QFileInfo* fi, bool overidesmallicon = false);

    // Defaults to large icons.
    void setSmallIcons( bool state );

    // Convenience fn to see if we are a root drive (could be static)
    bool isADrive( QFileInfo *fi );

    QPixmap *hardDrivePixmap() { return &hardDrive; };
    QPixmap *hardDriveSmallPixmap() { return &hardDriveSmall; };
   

private:

     QPixmap defaultFolder;
    QPixmap defaultFile;
    QPixmap defaultExe;

    QPixmap hardDrive;

    QPixmap defaultFolderSmall;
    QPixmap defaultFileSmall;
    QPixmap defaultExeSmall;

    QPixmap hardDriveSmall;

    QPixmap pix;
    int pixw, pixh;
    int pixsmw, pixsmh;
    QMap< QString, QPixmap > cache;
    QMap< QString, QPixmap > cachesm; // small icon cache TODO

    bool smallicons; 

};

#endif
