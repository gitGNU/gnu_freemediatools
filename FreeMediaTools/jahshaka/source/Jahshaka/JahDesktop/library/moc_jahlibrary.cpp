/****************************************************************************
** JahLibrary meta object code from reading C++ file 'jahlibrary.h'
**
** Created: Mon Jan 28 20:59:47 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahlibrary.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahLibrary::className() const
{
    return "JahLibrary";
}

QMetaObject *JahLibrary::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahLibrary( "JahLibrary", &JahLibrary::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahLibrary::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahLibrary", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahLibrary::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahLibrary", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahLibrary::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"directoryChanged", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "dirs", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"slotStartReadDir", 1, param_slot_2 };
    static const QUMethod slot_3 = {"slotReadNextDir", 0, 0 };
    static const QUMethod slot_4 = {"slotReadDirDone", 0, 0 };
    static const QUMethod slot_5 = {"cdUp", 0, 0 };
    static const QUMethod slot_6 = {"newFolder", 0, 0 };
    static const QUParameter param_slot_7[] = {
	{ "path", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"changePath", 1, param_slot_7 };
    static const QUMethod slot_8 = {"enableUp", 0, 0 };
    static const QUMethod slot_9 = {"disableUp", 0, 0 };
    static const QUMethod slot_10 = {"enableMkdir", 0, 0 };
    static const QUMethod slot_11 = {"disableMkdir", 0, 0 };
    static const QUParameter param_slot_12[] = {
	{ "drive", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_12 = {"changeWinDir", 1, param_slot_12 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"textWasChanged", 1, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"loadthisfile", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"setthefilename", 1, param_slot_15 };
    static const QUMethod slot_16 = {"returnFile", 0, 0 };
    static const QUMethod slot_17 = {"returnNull", 0, 0 };
    static const QUMethod slot_18 = {"clickedImportButton", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected },
	{ "directoryChanged(const QString&)", &slot_1, QMetaData::Protected },
	{ "slotStartReadDir(int)", &slot_2, QMetaData::Protected },
	{ "slotReadNextDir()", &slot_3, QMetaData::Protected },
	{ "slotReadDirDone()", &slot_4, QMetaData::Protected },
	{ "cdUp()", &slot_5, QMetaData::Protected },
	{ "newFolder()", &slot_6, QMetaData::Protected },
	{ "changePath(const QString&)", &slot_7, QMetaData::Protected },
	{ "enableUp()", &slot_8, QMetaData::Protected },
	{ "disableUp()", &slot_9, QMetaData::Protected },
	{ "enableMkdir()", &slot_10, QMetaData::Protected },
	{ "disableMkdir()", &slot_11, QMetaData::Protected },
	{ "changeWinDir(int)", &slot_12, QMetaData::Protected },
	{ "textWasChanged(const QString&)", &slot_13, QMetaData::Protected },
	{ "loadthisfile(QString)", &slot_14, QMetaData::Protected },
	{ "setthefilename(QString)", &slot_15, QMetaData::Protected },
	{ "returnFile()", &slot_16, QMetaData::Protected },
	{ "returnNull()", &slot_17, QMetaData::Protected },
	{ "clickedImportButton()", &slot_18, QMetaData::Protected }
    };
    static const QUMethod signal_0 = {"loadButton", 0, 0 };
    static const QUMethod signal_1 = {"importButton", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "loadButton()", &signal_0, QMetaData::Protected },
	{ "importButton()", &signal_1, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahLibrary", parentObject,
	slot_tbl, 19,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahLibrary.setMetaObject( metaObj );
    return metaObj;
}

void* JahLibrary::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahLibrary" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL loadButton
void JahLibrary::loadButton()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL importButton
void JahLibrary::importButton()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool JahLibrary::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    case 1: directoryChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: slotStartReadDir((int)static_QUType_int.get(_o+1)); break;
    case 3: slotReadNextDir(); break;
    case 4: slotReadDirDone(); break;
    case 5: cdUp(); break;
    case 6: newFolder(); break;
    case 7: changePath((const QString&)static_QUType_QString.get(_o+1)); break;
    case 8: enableUp(); break;
    case 9: disableUp(); break;
    case 10: enableMkdir(); break;
    case 11: disableMkdir(); break;
    case 12: changeWinDir((int)static_QUType_int.get(_o+1)); break;
    case 13: textWasChanged((const QString&)static_QUType_QString.get(_o+1)); break;
    case 14: loadthisfile((QString)static_QUType_QString.get(_o+1)); break;
    case 15: setthefilename((QString)static_QUType_QString.get(_o+1)); break;
    case 16: returnFile(); break;
    case 17: returnNull(); break;
    case 18: clickedImportButton(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahLibrary::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: loadButton(); break;
    case 1: importButton(); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahLibrary::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool JahLibrary::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
