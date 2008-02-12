/****************************************************************************
** JahTextBrowser meta object code from reading C++ file 'jahtextbrowser.h'
**
** Created: Wed Jan 23 19:09:24 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahtextbrowser.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahTextBrowser::className() const
{
    return "JahTextBrowser";
}

QMetaObject *JahTextBrowser::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahTextBrowser( "JahTextBrowser", &JahTextBrowser::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahTextBrowser::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahTextBrowser", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahTextBrowser::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahTextBrowser", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahTextBrowser::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QTextBrowser::staticMetaObject();
    static const QUMethod slot_0 = {"switchLine", 0, 0 };
    static const QUMethod slot_1 = {"home", 0, 0 };
    static const QUMethod slot_2 = {"stop", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "txt", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"setText", 1, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"fetch", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"done", 1, param_slot_5 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"imgdone", 1, param_slot_6 };
    static const QMetaData slot_tbl[] = {
	{ "switchLine()", &slot_0, QMetaData::Public },
	{ "home()", &slot_1, QMetaData::Public },
	{ "stop()", &slot_2, QMetaData::Public },
	{ "setText(const QString&)", &slot_3, QMetaData::Public },
	{ "fetch(const QString&)", &slot_4, QMetaData::Private },
	{ "done(bool)", &slot_5, QMetaData::Private },
	{ "imgdone(bool)", &slot_6, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"modeChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "modeChanged(int)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"JahTextBrowser", parentObject,
	slot_tbl, 7,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahTextBrowser.setMetaObject( metaObj );
    return metaObj;
}

void* JahTextBrowser::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahTextBrowser" ) )
	return this;
    return QTextBrowser::qt_cast( clname );
}

// SIGNAL modeChanged
void JahTextBrowser::modeChanged( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool JahTextBrowser::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: switchLine(); break;
    case 1: home(); break;
    case 2: stop(); break;
    case 3: setText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 4: fetch((const QString&)static_QUType_QString.get(_o+1)); break;
    case 5: done((bool)static_QUType_bool.get(_o+1)); break;
    case 6: imgdone((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QTextBrowser::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool JahTextBrowser::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: modeChanged((int)static_QUType_int.get(_o+1)); break;
    default:
	return QTextBrowser::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool JahTextBrowser::qt_property( int id, int f, QVariant* v)
{
    return QTextBrowser::qt_property( id, f, v);
}

bool JahTextBrowser::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
