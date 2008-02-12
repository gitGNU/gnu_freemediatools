/****************************************************************************
** Jahshaka meta object code from reading C++ file 'jahshaka.h'
**
** Created: Wed Jan 23 19:13:29 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahshaka.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Jahshaka::className() const
{
    return "Jahshaka";
}

QMetaObject *Jahshaka::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Jahshaka( "Jahshaka", &Jahshaka::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Jahshaka::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Jahshaka", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Jahshaka::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Jahshaka", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Jahshaka::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QMainWindow::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "value", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"shrinkUI", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"JahchangeModeText", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "steps", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"JahaddProgressSteps", 1, param_slot_2 };
    static const QUMethod slot_3 = {"JahsetProgressValue", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "pal", &static_QUType_varptr, "\x0b", QUParameter::In }
    };
    static const QUMethod slot_4 = {"setStyleColor", 1, param_slot_4 };
    static const QUMethod slot_5 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "shrinkUI(bool)", &slot_0, QMetaData::Public },
	{ "JahchangeModeText(const QString&)", &slot_1, QMetaData::Public },
	{ "JahaddProgressSteps(int)", &slot_2, QMetaData::Public },
	{ "JahsetProgressValue()", &slot_3, QMetaData::Public },
	{ "setStyleColor(QPalette)", &slot_4, QMetaData::Public },
	{ "languageChange()", &slot_5, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "text", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"sendchangeModeText", 1, param_signal_0 };
    static const QUParameter param_signal_1[] = {
	{ "progress", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_1 = {"sendsetProgressValue", 1, param_signal_1 };
    static const QMetaData signal_tbl[] = {
	{ "sendchangeModeText(const QString&)", &signal_0, QMetaData::Public },
	{ "sendsetProgressValue(int)", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"Jahshaka", parentObject,
	slot_tbl, 6,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Jahshaka.setMetaObject( metaObj );
    return metaObj;
}

void* Jahshaka::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Jahshaka" ) )
	return this;
    return QMainWindow::qt_cast( clname );
}

// SIGNAL sendchangeModeText
void Jahshaka::sendchangeModeText( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL sendsetProgressValue
void Jahshaka::sendsetProgressValue( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 1, t0 );
}

bool Jahshaka::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: shrinkUI((bool)static_QUType_bool.get(_o+1)); break;
    case 1: JahchangeModeText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 2: JahaddProgressSteps((int)static_QUType_int.get(_o+1)); break;
    case 3: JahsetProgressValue(); break;
    case 4: setStyleColor((QPalette)(*((QPalette*)static_QUType_ptr.get(_o+1)))); break;
    case 5: languageChange(); break;
    default:
	return QMainWindow::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Jahshaka::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: sendchangeModeText((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: sendsetProgressValue((int)static_QUType_int.get(_o+1)); break;
    default:
	return QMainWindow::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Jahshaka::qt_property( int id, int f, QVariant* v)
{
    return QMainWindow::qt_property( id, f, v);
}

bool Jahshaka::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
