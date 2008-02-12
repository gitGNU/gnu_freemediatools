/****************************************************************************
** GLTexobj meta object code from reading C++ file 'jahstartupgl.h'
**
** Created: Wed Jan 23 19:13:37 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "jahstartupgl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLTexobj::className() const
{
    return "GLTexobj";
}

QMetaObject *GLTexobj::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLTexobj( "GLTexobj", &GLTexobj::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLTexobj::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLTexobj", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLTexobj::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLTexobj", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLTexobj::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"toggleAnimation", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "toggleAnimation()", &slot_0, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"introFinished", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "introFinished()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLTexobj", parentObject,
	slot_tbl, 1,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLTexobj.setMetaObject( metaObj );
    return metaObj;
}

void* GLTexobj::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLTexobj" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

// SIGNAL introFinished
void GLTexobj::introFinished()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool GLTexobj::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: toggleAnimation(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLTexobj::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: introFinished(); break;
    default:
	return QGLWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLTexobj::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool GLTexobj::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
