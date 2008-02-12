/****************************************************************************
** AssetManMainWidget meta object code from reading C++ file 'assetmanmainwidget.h'
**
** Created: Wed Jan 23 19:11:09 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "assetmanmainwidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AssetManMainWidget::className() const
{
    return "AssetManMainWidget";
}

QMetaObject *AssetManMainWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AssetManMainWidget( "AssetManMainWidget", &AssetManMainWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AssetManMainWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AssetManMainWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AssetManMainWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AssetManMainWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AssetManMainWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"populateDrivesCombo", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "drive", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"changeWinDir", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "path", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"changePath", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "thedir", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"comboTextWasChanged", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "path", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"setPathCombo", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "dir", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"directoryChanged", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ "dirs", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotStartReadDir", 1, param_slot_6 };
    static const QUMethod slot_7 = {"slotReadNextDir", 0, 0 };
    static const QUMethod slot_8 = {"slotReadDirDone", 0, 0 };
    static const QUParameter param_slot_9[] = {
	{ "filename", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_9 = {"loadThisFile", 1, param_slot_9 };
    static const QUParameter param_slot_10[] = {
	{ "filename", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"setTheFileName", 1, param_slot_10 };
    static const QUMethod slot_11 = {"advandedSearchClicked", 0, 0 };
    static const QUMethod slot_12 = {"advancedSearchCancelPressed", 0, 0 };
    static const QUMethod slot_13 = {"executeSimpleSearch", 0, 0 };
    static const QUMethod slot_14 = {"cdUp", 0, 0 };
    static const QUMethod slot_15 = {"enableUp", 0, 0 };
    static const QUMethod slot_16 = {"disableUp", 0, 0 };
    static const QUMethod slot_17 = {"connectToServerDialog", 0, 0 };
    static const QUMethod slot_18 = {"enableBack", 0, 0 };
    static const QUMethod slot_19 = {"disableBack", 0, 0 };
    static const QUMethod slot_20 = {"cdBack", 0, 0 };
    static const QUMethod slot_21 = {"locateAssetDatabase", 0, 0 };
    static const QUMethod slot_22 = {"returnFile", 0, 0 };
    static const QUMethod slot_23 = {"languageChange", 0, 0 };
    static const QUMethod slot_24 = {"populateDirView", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "populateDrivesCombo()", &slot_0, QMetaData::Public },
	{ "changeWinDir(int)", &slot_1, QMetaData::Public },
	{ "changePath(const QString&)", &slot_2, QMetaData::Public },
	{ "comboTextWasChanged(const QString&)", &slot_3, QMetaData::Public },
	{ "setPathCombo(QString)", &slot_4, QMetaData::Public },
	{ "directoryChanged(const QString&)", &slot_5, QMetaData::Public },
	{ "slotStartReadDir(int)", &slot_6, QMetaData::Public },
	{ "slotReadNextDir()", &slot_7, QMetaData::Public },
	{ "slotReadDirDone()", &slot_8, QMetaData::Public },
	{ "loadThisFile(QString)", &slot_9, QMetaData::Public },
	{ "setTheFileName(QString)", &slot_10, QMetaData::Public },
	{ "advandedSearchClicked()", &slot_11, QMetaData::Public },
	{ "advancedSearchCancelPressed()", &slot_12, QMetaData::Public },
	{ "executeSimpleSearch()", &slot_13, QMetaData::Public },
	{ "cdUp()", &slot_14, QMetaData::Public },
	{ "enableUp()", &slot_15, QMetaData::Public },
	{ "disableUp()", &slot_16, QMetaData::Public },
	{ "connectToServerDialog()", &slot_17, QMetaData::Public },
	{ "enableBack()", &slot_18, QMetaData::Public },
	{ "disableBack()", &slot_19, QMetaData::Public },
	{ "cdBack()", &slot_20, QMetaData::Public },
	{ "locateAssetDatabase()", &slot_21, QMetaData::Public },
	{ "returnFile()", &slot_22, QMetaData::Public },
	{ "languageChange()", &slot_23, QMetaData::Protected },
	{ "populateDirView()", &slot_24, QMetaData::Protected }
    };
    static const QUMethod signal_0 = {"loadButton", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "loadButton()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"AssetManMainWidget", parentObject,
	slot_tbl, 25,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AssetManMainWidget.setMetaObject( metaObj );
    return metaObj;
}

void* AssetManMainWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AssetManMainWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL loadButton
void AssetManMainWidget::loadButton()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool AssetManMainWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: populateDrivesCombo(); break;
    case 1: changeWinDir((int)static_QUType_int.get(_o+1)); break;
    case 2: changePath((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: comboTextWasChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: setPathCombo((QString)static_QUType_QString.get(_o+1)); break;
    case 5: directoryChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 6: slotStartReadDir((int)static_QUType_int.get(_o+1)); break;
    case 7: slotReadNextDir(); break;
    case 8: slotReadDirDone(); break;
    case 9: loadThisFile((QString)static_QUType_QString.get(_o+1)); break;
    case 10: setTheFileName((QString)static_QUType_QString.get(_o+1)); break;
    case 11: advandedSearchClicked(); break;
    case 12: advancedSearchCancelPressed(); break;
    case 13: executeSimpleSearch(); break;
    case 14: cdUp(); break;
    case 15: enableUp(); break;
    case 16: disableUp(); break;
    case 17: connectToServerDialog(); break;
    case 18: enableBack(); break;
    case 19: disableBack(); break;
    case 20: cdBack(); break;
    case 21: locateAssetDatabase(); break;
    case 22: returnFile(); break;
    case 23: languageChange(); break;
    case 24: populateDirView(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AssetManMainWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: loadButton(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool AssetManMainWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool AssetManMainWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
