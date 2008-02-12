/****************************************************************************
** LevelsWidget meta object code from reading C++ file 'levelswidget.h'
**
** Created: Sat Jan 26 08:34:41 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "levelswidget.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *LevelsWidget::className() const
{
    return "LevelsWidget";
}

QMetaObject *LevelsWidget::metaObj = 0;
static QMetaObjectCleanUp cleanUp_LevelsWidget( "LevelsWidget", &LevelsWidget::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString LevelsWidget::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LevelsWidget", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString LevelsWidget::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "LevelsWidget", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* LevelsWidget::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "levelstring", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"levelChooser", 1, param_slot_0 };
    static const QUParameter param_slot_1[] = {
	{ "state", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_1 = {"setRealTime", 1, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "state", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"realTimeToggled", 1, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "image", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod slot_3 = {"setTheImage", 1, param_slot_3 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "levelChooser(const QString&)", &slot_0, QMetaData::Public },
	{ "setRealTime(bool)", &slot_1, QMetaData::Public },
	{ "realTimeToggled(bool)", &slot_2, QMetaData::Public },
	{ "setTheImage(QImage*)", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"realTimeChanged", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "realTimeChanged(bool)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"LevelsWidget", parentObject,
	slot_tbl, 5,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_LevelsWidget.setMetaObject( metaObj );
    return metaObj;
}

void* LevelsWidget::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "LevelsWidget" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL realTimeChanged
void LevelsWidget::realTimeChanged( bool t0 )
{
    activate_signal_bool( staticMetaObject()->signalOffset() + 0, t0 );
}

bool LevelsWidget::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: levelChooser((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: setRealTime((bool)static_QUType_bool.get(_o+1)); break;
    case 2: realTimeToggled((bool)static_QUType_bool.get(_o+1)); break;
    case 3: setTheImage((QImage*)static_QUType_varptr.get(_o+1)); break;
    case 4: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool LevelsWidget::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: realTimeChanged((bool)static_QUType_bool.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool LevelsWidget::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool LevelsWidget::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
