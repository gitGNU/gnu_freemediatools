/****************************************************************************
** JahDesktop meta object code from reading C++ file 'desktop.h'
**
** Created: Mon Jan 28 20:58:30 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "desktop.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahDesktop::className() const
{
    return "JahDesktop";
}

QMetaObject *JahDesktop::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahDesktop( "JahDesktop", &JahDesktop::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahDesktop::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktop", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahDesktop::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahDesktop", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahDesktop::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"RebuildDesktop", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_ptr, "ImageItem", QUParameter::Out },
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_1 = {"addImage2", 3, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_ptr, "ImageItem", QUParameter::Out },
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In },
	{ "xsize", &static_QUType_double, 0, QUParameter::InOut },
	{ "ysize", &static_QUType_double, 0, QUParameter::InOut },
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_2 = {"addImage2", 5, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ 0, &static_QUType_ptr, "ImageItem", QUParameter::Out },
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_3 = {"addImage", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "index", &static_QUType_int, 0, QUParameter::In },
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_4 = {"addItemToDesktopTable", 3, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "imagenumber", &static_QUType_int, 0, QUParameter::In },
	{ "newname", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"renameImageindatabase", 2, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "thedata", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_6 = {"addAssetTodatabase", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_7 = {"updateClipPosition", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_8 = {"updateClipSize", 1, param_slot_8 };
    static const QUParameter param_slot_9[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_9 = {"updateLockStatus", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_10 = {"updateFilmstripStatus", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "data", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_11 = {"slotUpdateFrameOffset", 1, param_slot_11 };
    static const QUParameter param_slot_12[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::Out }
    };
    static const QUMethod slot_12 = {"getModuleName", 1, param_slot_12 };
    static const QUMethod slot_13 = {"slotExport", 0, 0 };
    static const QUMethod slot_14 = {"slotImport", 0, 0 };
    static const QUMethod slot_15 = {"slotLoadIconview", 0, 0 };
    static const QUMethod slot_16 = {"clearItem", 0, 0 };
    static const QUMethod slot_17 = {"clearAll", 0, 0 };
    static const QUMethod slot_18 = {"tidyDesktop", 0, 0 };
    static const QUMethod slot_19 = {"slotExpand", 0, 0 };
    static const QUMethod slot_20 = {"toggleDesktopImages", 0, 0 };
    static const QUMethod slot_21 = {"toggleDesktopTable", 0, 0 };
    static const QUMethod slot_22 = {"changeModePlayer", 0, 0 };
    static const QUParameter param_slot_23[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out }
    };
    static const QUMethod slot_23 = {"clearTheDesktop", 1, param_slot_23 };
    static const QUParameter param_slot_24[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "erase", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_24 = {"eraseTheAsset", 2, param_slot_24 };
    static const QUParameter param_slot_25[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "exportto", &static_QUType_QString, 0, QUParameter::In },
	{ "theasset", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_25 = {"exportTheAsset", 3, param_slot_25 };
    static const QUMethod slot_26 = {"addRectangle", 0, 0 };
    static const QUMethod slot_27 = {"slotSave", 0, 0 };
    static const QUMethod slot_28 = {"slotLoad", 0, 0 };
    static const QUParameter param_slot_29[] = {
	{ "row", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_29 = {"selectTableRow", 2, param_slot_29 };
    static const QUParameter param_slot_30[] = {
	{ 0, &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_30 = {"assetSelected", 1, param_slot_30 };
    static const QMetaData slot_tbl[] = {
	{ "RebuildDesktop()", &slot_0, QMetaData::Public },
	{ "addImage2(assetData,QImage)", &slot_1, QMetaData::Public },
	{ "addImage2(assetData,double&,double&,QImage)", &slot_2, QMetaData::Public },
	{ "addImage(assetData,QImage)", &slot_3, QMetaData::Public },
	{ "addItemToDesktopTable(int,assetData,QImage)", &slot_4, QMetaData::Public },
	{ "renameImageindatabase(int,QString)", &slot_5, QMetaData::Public },
	{ "addAssetTodatabase(assetData)", &slot_6, QMetaData::Public },
	{ "updateClipPosition(assetData)", &slot_7, QMetaData::Public },
	{ "updateClipSize(assetData)", &slot_8, QMetaData::Public },
	{ "updateLockStatus(assetData)", &slot_9, QMetaData::Public },
	{ "updateFilmstripStatus(assetData)", &slot_10, QMetaData::Public },
	{ "slotUpdateFrameOffset(assetData)", &slot_11, QMetaData::Public },
	{ "getModuleName()", &slot_12, QMetaData::Public },
	{ "slotExport()", &slot_13, QMetaData::Public },
	{ "slotImport()", &slot_14, QMetaData::Public },
	{ "slotLoadIconview()", &slot_15, QMetaData::Public },
	{ "clearItem()", &slot_16, QMetaData::Public },
	{ "clearAll()", &slot_17, QMetaData::Public },
	{ "tidyDesktop()", &slot_18, QMetaData::Public },
	{ "slotExpand()", &slot_19, QMetaData::Public },
	{ "toggleDesktopImages()", &slot_20, QMetaData::Public },
	{ "toggleDesktopTable()", &slot_21, QMetaData::Public },
	{ "changeModePlayer()", &slot_22, QMetaData::Public },
	{ "clearTheDesktop()", &slot_23, QMetaData::Private },
	{ "eraseTheAsset(assetData)", &slot_24, QMetaData::Private },
	{ "exportTheAsset(QString,assetData)", &slot_25, QMetaData::Private },
	{ "addRectangle()", &slot_26, QMetaData::Private },
	{ "slotSave()", &slot_27, QMetaData::Private },
	{ "slotLoad()", &slot_28, QMetaData::Private },
	{ "selectTableRow(int,int)", &slot_29, QMetaData::Private },
	{ "assetSelected(assetData)", &slot_30, QMetaData::Private }
    };
    static const QUMethod signal_0 = {"resetGlobalClipNumber", 0, 0 };
    static const QUMethod signal_1 = {"notifyPlayerOfChange", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_2 = {"switchMainModule", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_3 = {"loadMainModuleFile", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_4 = {"loadMainModelFile", 1, param_signal_4 };
    static const QUParameter param_signal_5[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_5 = {"addProgressSteps", 1, param_signal_5 };
    static const QUMethod signal_6 = {"updateProgress", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "resetGlobalClipNumber()", &signal_0, QMetaData::Public },
	{ "notifyPlayerOfChange()", &signal_1, QMetaData::Public },
	{ "switchMainModule(QString)", &signal_2, QMetaData::Public },
	{ "loadMainModuleFile(QString)", &signal_3, QMetaData::Public },
	{ "loadMainModelFile(QString)", &signal_4, QMetaData::Public },
	{ "addProgressSteps(int)", &signal_5, QMetaData::Public },
	{ "updateProgress()", &signal_6, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahDesktop", parentObject,
	slot_tbl, 31,
	signal_tbl, 7,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahDesktop.setMetaObject( metaObj );
    return metaObj;
}

void* JahDesktop::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahDesktop" ) )
	return this;
    if ( !qstrcmp( clname, "AssetTable" ) )
	return (AssetTable*)this;
    return QWidget::qt_cast( clname );
}

// SIGNAL resetGlobalClipNumber
void JahDesktop::resetGlobalClipNumber()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL notifyPlayerOfChange
void JahDesktop::notifyPlayerOfChange()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL switchMainModule
void JahDesktop::switchMainModule( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 2, t0 );
}

// SIGNAL loadMainModuleFile
void JahDesktop::loadMainModuleFile( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 3, t0 );
}

// SIGNAL loadMainModelFile
void JahDesktop::loadMainModelFile( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 4, t0 );
}

// SIGNAL addProgressSteps
void JahDesktop::addProgressSteps( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 5, t0 );
}

// SIGNAL updateProgress
void JahDesktop::updateProgress()
{
    activate_signal( staticMetaObject()->signalOffset() + 6 );
}

bool JahDesktop::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: RebuildDesktop(); break;
    case 1: static_QUType_ptr.set(_o,addImage2((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))),(QImage)(*((QImage*)static_QUType_ptr.get(_o+2))))); break;
    case 2: static_QUType_ptr.set(_o,addImage2((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))),(double&)static_QUType_double.get(_o+2),(double&)static_QUType_double.get(_o+3),(QImage)(*((QImage*)static_QUType_ptr.get(_o+4))))); break;
    case 3: static_QUType_ptr.set(_o,addImage((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))),(QImage)(*((QImage*)static_QUType_ptr.get(_o+2))))); break;
    case 4: addItemToDesktopTable((int)static_QUType_int.get(_o+1),(assetData)(*((assetData*)static_QUType_ptr.get(_o+2))),(QImage)(*((QImage*)static_QUType_ptr.get(_o+3)))); break;
    case 5: renameImageindatabase((int)static_QUType_int.get(_o+1),(QString)static_QUType_QString.get(_o+2)); break;
    case 6: addAssetTodatabase((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 7: updateClipPosition((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 8: updateClipSize((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 9: updateLockStatus((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 10: updateFilmstripStatus((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 11: slotUpdateFrameOffset((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 12: static_QUType_QString.set(_o,getModuleName()); break;
    case 13: slotExport(); break;
    case 14: slotImport(); break;
    case 15: slotLoadIconview(); break;
    case 16: clearItem(); break;
    case 17: clearAll(); break;
    case 18: tidyDesktop(); break;
    case 19: slotExpand(); break;
    case 20: toggleDesktopImages(); break;
    case 21: toggleDesktopTable(); break;
    case 22: changeModePlayer(); break;
    case 23: static_QUType_bool.set(_o,clearTheDesktop()); break;
    case 24: static_QUType_bool.set(_o,eraseTheAsset((assetData)(*((assetData*)static_QUType_ptr.get(_o+1))))); break;
    case 25: static_QUType_bool.set(_o,exportTheAsset((QString)static_QUType_QString.get(_o+1),(assetData)(*((assetData*)static_QUType_ptr.get(_o+2))))); break;
    case 26: addRectangle(); break;
    case 27: slotSave(); break;
    case 28: slotLoad(); break;
    case 29: selectTableRow((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 30: assetSelected((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahDesktop::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: resetGlobalClipNumber(); break;
    case 1: notifyPlayerOfChange(); break;
    case 2: switchMainModule((QString)static_QUType_QString.get(_o+1)); break;
    case 3: loadMainModuleFile((QString)static_QUType_QString.get(_o+1)); break;
    case 4: loadMainModelFile((QString)static_QUType_QString.get(_o+1)); break;
    case 5: addProgressSteps((int)static_QUType_int.get(_o+1)); break;
    case 6: updateProgress(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahDesktop::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahDesktop::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
