#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <qfiledialog.h>
#include <qpixmap.h>
#include <qmap.h>

class JahFileIconView;

// Singleton for supplying file/folder and other common icons.
// TODO: store large/small icons for file types that are requested (see map cache).
class IconProvider : public QFileIconProvider
{
public:
    static IconProvider& instance();

    ~IconProvider();

    // Platform specific stuff. Windows init does registry trickery. 
    void windowsInit();
    void unixInit();

    QPixmap * pixmap( QFileInfo* fi, bool overidesmallicon = false);

    // We default to large icons.
    void setSmallIcons( bool state );

    // Convenience fn to see if we are a root drive (could be static)
    bool isADrive( QFileInfo *fi );

    QPixmap *hardDrivePixmap() { return &hardDrive; };
    QPixmap *hardDriveSmallPixmap() { return &hardDriveSmall; };
   
protected:

    // Windows specific lookup of icons from registry.
    QPixmap *windowsRegistryIcon( QFileInfo* fi, bool overidesmallicon );

private:

    IconProvider( QWidget *parent=0, const char *name=0 );

    static IconProvider* _instance;

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
