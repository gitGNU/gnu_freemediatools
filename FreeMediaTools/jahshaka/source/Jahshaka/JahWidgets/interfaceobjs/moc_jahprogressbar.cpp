/****************************************************************************
** JahProgressBar meta object code from reading C++ file 'jahprogressbar.h'
**
** Created: Sat Jan 26 08:34:01 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahprogressbar.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *JahProgressBar::className() const
{
    return "JahProgressBar";
}

QMetaObject *JahProgressBar::metaObj = 0;
static QMetaObjectCleanUp cleanUp_JahProgressBar( "JahProgressBar", &JahProgressBar::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString JahProgressBar::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahProgressBar", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString JahProgressBar::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "JahProgressBar", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* JahProgressBar::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QProgressBar::staticMetaObject();
    metaObj = QMetaObject::new_metaobject(
	"JahProgressBar", parentObject,
	0, 0,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_JahProgressBar.setMetaObject( metaObj );
    return metaObj;
}

void* JahProgressBar::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "JahProgressBar" ) )
	return this;
    return QProgressBar::qt_cast( clname );
}

bool JahProgressBar::qt_invoke( int _id, QUObject* _o )
{
    return QProgressBar::qt_invoke(_id,_o);
}

bool JahProgressBar::qt_emit( int _id, QUObject* _o )
{
    return QProgressBar::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool JahProgressBar::qt_property( int id, int f, QVariant* v)
{
    return QProgressBar::qt_property( id, f, v);
}

bool JahProgressBar::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
