/****************************************************************************
** GLCore meta object code from reading C++ file 'glcore.h'
**
** Created: Wed Jan 23 19:11:33 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "glcore.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLCore::className() const
{
    return "GLCore";
}

QMetaObject *GLCore::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLCore( "GLCore", &GLCore::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLCore::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLCore", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLCore::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLCore", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLCore::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"initializeGL", 0, 0 };
    static const QUMethod slot_1 = {"paintGL", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"resizeGLInternals", 2, param_slot_2 };
    static const QUParameter param_slot_3[] = {
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In },
	{ "dont_zoom", &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_3 = {"resizeGLInternals", 3, param_slot_3 };
    static const QUParameter param_slot_4[] = {
	{ "w", &static_QUType_int, 0, QUParameter::In },
	{ "h", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"resizeGL", 2, param_slot_4 };
    static const QUMethod slot_5 = {"notifyDrop", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "event", &static_QUType_ptr, "QDragEnterEvent", QUParameter::In }
    };
    static const QUMethod slot_6 = {"customDragEnterEvent", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "event", &static_QUType_ptr, "QDropEvent", QUParameter::In }
    };
    static const QUMethod slot_7 = {"customDropEvent", 1, param_slot_7 };
    static const QMetaData slot_tbl[] = {
	{ "initializeGL()", &slot_0, QMetaData::Public },
	{ "paintGL()", &slot_1, QMetaData::Public },
	{ "resizeGLInternals(int,int)", &slot_2, QMetaData::Public },
	{ "resizeGLInternals(int,int,bool)", &slot_3, QMetaData::Public },
	{ "resizeGL(int,int)", &slot_4, QMetaData::Public },
	{ "notifyDrop()", &slot_5, QMetaData::Public },
	{ "customDragEnterEvent(QDragEnterEvent*)", &slot_6, QMetaData::Public },
	{ "customDropEvent(QDropEvent*)", &slot_7, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"paintit", 0, 0 };
    static const QUMethod signal_1 = {"resizeit", 0, 0 };
    static const QUParameter param_signal_2[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_2 = {"mousePress", 1, param_signal_2 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_3 = {"mouseRelease", 1, param_signal_3 };
    static const QUParameter param_signal_4[] = {
	{ 0, &static_QUType_ptr, "QMouseEvent", QUParameter::In }
    };
    static const QUMethod signal_4 = {"mouseMove", 1, param_signal_4 };
    static const QUMethod signal_5 = {"draggedAImage", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "paintit()", &signal_0, QMetaData::Public },
	{ "resizeit()", &signal_1, QMetaData::Public },
	{ "mousePress(QMouseEvent*)", &signal_2, QMetaData::Public },
	{ "mouseRelease(QMouseEvent*)", &signal_3, QMetaData::Public },
	{ "mouseMove(QMouseEvent*)", &signal_4, QMetaData::Public },
	{ "draggedAImage()", &signal_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLCore", parentObject,
	slot_tbl, 8,
	signal_tbl, 6,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLCore.setMetaObject( metaObj );
    return metaObj;
}

void* GLCore::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLCore" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

// SIGNAL paintit
void GLCore::paintit()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL resizeit
void GLCore::resizeit()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL mousePress
void GLCore::mousePress( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 2 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL mouseRelease
void GLCore::mouseRelease( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 3 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL mouseMove
void GLCore::mouseMove( QMouseEvent* t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 4 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,t0);
    activate_signal( clist, o );
}

// SIGNAL draggedAImage
void GLCore::draggedAImage()
{
    activate_signal( staticMetaObject()->signalOffset() + 5 );
}

bool GLCore::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: initializeGL(); break;
    case 1: paintGL(); break;
    case 2: resizeGLInternals((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 3: resizeGLInternals((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2),(bool)static_QUType_bool.get(_o+3)); break;
    case 4: resizeGL((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    case 5: notifyDrop(); break;
    case 6: customDragEnterEvent((QDragEnterEvent*)static_QUType_ptr.get(_o+1)); break;
    case 7: customDropEvent((QDropEvent*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLCore::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: paintit(); break;
    case 1: resizeit(); break;
    case 2: mousePress((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 3: mouseRelease((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 4: mouseMove((QMouseEvent*)static_QUType_ptr.get(_o+1)); break;
    case 5: draggedAImage(); break;
    default:
	return QGLWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLCore::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool GLCore::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
