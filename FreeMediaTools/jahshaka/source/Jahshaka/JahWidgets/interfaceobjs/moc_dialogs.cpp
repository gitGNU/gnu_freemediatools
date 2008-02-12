/****************************************************************************
** FancyPopup meta object code from reading C++ file 'dialogs.h'
**
** Created: Sat Jan 26 08:33:59 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "dialogs.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *FancyPopup::className() const
{
    return "FancyPopup";
}

QMetaObject *FancyPopup::metaObj = 0;
static QMetaObjectCleanUp cleanUp_FancyPopup( "FancyPopup", &FancyPopup::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString FancyPopup::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FancyPopup", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString FancyPopup::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "FancyPopup", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* FancyPopup::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QLabel::staticMetaObject();
    static const QUParameter param_signal_0[] = {
	{ "textval", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"returnText", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "returnText(QString)", &signal_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"FancyPopup", parentObject,
	0, 0,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_FancyPopup.setMetaObject( metaObj );
    return metaObj;
}

void* FancyPopup::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "FancyPopup" ) )
	return this;
    return QLabel::qt_cast( clname );
}

// SIGNAL returnText
void FancyPopup::returnText( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool FancyPopup::qt_invoke( int _id, QUObject* _o )
{
    return QLabel::qt_invoke(_id,_o);
}

bool FancyPopup::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: returnText((QString)static_QUType_QString.get(_o+1)); break;
    default:
	return QLabel::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool FancyPopup::qt_property( int id, int f, QVariant* v)
{
    return QLabel::qt_property( id, f, v);
}

bool FancyPopup::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
