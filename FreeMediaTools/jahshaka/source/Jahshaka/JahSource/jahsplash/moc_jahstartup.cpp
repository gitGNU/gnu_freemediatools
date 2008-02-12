/****************************************************************************
** startup meta object code from reading C++ file 'jahstartup.h'
**
** Created: Wed Jan 23 19:13:36 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahstartup.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *startup::className() const
{
    return "startup";
}

QMetaObject *startup::metaObj = 0;
static QMetaObjectCleanUp cleanUp_startup( "startup", &startup::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString startup::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "startup", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString startup::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "startup", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* startup::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"updateProgressValue", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"addProgressSteps", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setProgressText", 1, param_slot_2 };
    static const QUMethod slot_3 = {"introIsFinished", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "updateProgressValue()", &slot_0, QMetaData::Public },
	{ "addProgressSteps(int)", &slot_1, QMetaData::Public },
	{ "setProgressText(const QString&)", &slot_2, QMetaData::Public },
	{ "introIsFinished()", &slot_3, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"startup", parentObject,
	slot_tbl, 4,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_startup.setMetaObject( metaObj );
    return metaObj;
}

void* startup::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "startup" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool startup::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: updateProgressValue(); break;
    case 1: addProgressSteps((int)static_QUType_int.get(_o+1)); break;
    case 2: setProgressText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: introIsFinished(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool startup::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool startup::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool startup::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
