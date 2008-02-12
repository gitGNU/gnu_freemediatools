/****************************************************************************
** LevelsPlot meta object code from reading C++ file 'levelsplot.h'
**
** Created: Sat Jan 26 08:34:41 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "levelsplot.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *LevelsPlot::className() const
{
    return "LevelsPlot";
}

QMetaObject *LevelsPlot::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LevelsPlot( "LevelsPlot", &LevelsPlot::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LevelsPlot::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LevelsPlot", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LevelsPlot::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LevelsPlot", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LevelsPlot::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QFrame::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::Out },
	{ "image", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_0 = {"setTheImage", 2, param_slot_0 };
    static const QUMethod slot_1 = {"rescanImage", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "setTheImage(QImage*)", &slot_0, QMetaData::Public },
	{ "rescanImage()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"LevelsPlot", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LevelsPlot.setMetaObject( metaObj );
    return metaObj;
}

void* LevelsPlot::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LevelsPlot" ) )
	return this;
    return QFrame::qt_cast( clname );
}

bool LevelsPlot::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: static_QUType_bool.set(_o,setTheImage((QImage*)static_QUType_varptr.get(_o+1))); break;
    case 1: rescanImage(); break;
    default:
	return QFrame::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool LevelsPlot::qt_emit( int _id, QUObject* _o )
{
    return QFrame::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool LevelsPlot::qt_property( int id, int f, QVariant* v)
{
    return QFrame::qt_property( id, f, v);
}

bool LevelsPlot::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
