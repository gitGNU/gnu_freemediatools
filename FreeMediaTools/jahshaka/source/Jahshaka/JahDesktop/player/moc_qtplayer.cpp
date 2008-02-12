/****************************************************************************
** QtPlayer meta object code from reading C++ file 'qtplayer.h'
**
** Created: Tue Jan 29 23:20:02 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "qtplayer.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *QtPlayer::className() const
{
    return "QtPlayer";
}

QMetaObject *QtPlayer::metaObj = 0;
static QMetaObjectCleanUp cleanUp_QtPlayer( "QtPlayer", &QtPlayer::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString QtPlayer::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtPlayer", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString QtPlayer::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "QtPlayer", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* QtPlayer::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_ptr, "QContextMenuEvent", QUParameter::In }
    };
    static const QUMethod slot_0 = {"contextMenuEvent", 1, param_slot_0 };
    static const QUMethod slot_1 = {"timeOutSlot", 0, 0 };
    static const QUMethod slot_2 = {"mouseSlot", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "contextMenuEvent(QContextMenuEvent*)", &slot_0, QMetaData::Protected },
	{ "timeOutSlot()", &slot_1, QMetaData::Protected },
	{ "mouseSlot()", &slot_2, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ "position", &static_QUType_int, 0, QUParameter::In },
	{ "image", &static_QUType_varptr, "\x0f", QUParameter::In }
    };
    static const QUMethod signal_0 = {"showImage", 2, param_signal_0 };
    static const QUMethod signal_1 = {"draggedAImage", 0, 0 };
    static const QUMethod signal_2 = {"requestNextItem", 0, 0 };
    static const QUParameter param_signal_3[] = {
	{ 0, &static_QUType_ptr, "JahHeadable", QUParameter::In }
    };
    static const QUMethod signal_3 = {"showInHead", 1, param_signal_3 };
    static const QMetaData signal_tbl[] = {
	{ "showImage(int,QImage*)", &signal_0, QMetaData::Private },
	{ "draggedAImage()", &signal_1, QMetaData::Private },
	{ "requestNextItem()", &signal_2, QMetaData::Private },
	{ "showInHead(JahHeadable*)", &signal_3, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"QtPlayer", parentObject,
	slot_tbl, 3,
	signal_tbl, 4,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_QtPlayer.setMetaObject( metaObj );
    return metaObj;
}

void* QtPlayer::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "QtPlayer" ) )
	return this;
    if ( !qstrcmp( clname, "JahHeadable" ) )
	return (JahHeadable*)this;
    return QGLWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL showImage
void QtPlayer::showImage( int t0, QImage* t1 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[3];
    static_QUType_int.set(o+1,t0);
    static_QUType_varptr.set(o+2,t1);
    activate_signal( clist, o );
}

// SIGNAL draggedAImage
void QtPlayer::draggedAImage()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

// SIGNAL requestNextItem
void QtPlayer::requestNextItem()
{
    activate_signal( staticMetaObject()->signalOffset() + 2 );
}

// SIGNAL showInHead
void QtPlayer::showInHead( JahHeadable* t0 )
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

bool QtPlayer::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: contextMenuEvent((QContextMenuEvent*)static_QUType_ptr.get(_o+1)); break;
    case 1: timeOutSlot(); break;
    case 2: mouseSlot(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool QtPlayer::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: showImage((int)static_QUType_int.get(_o+1),(QImage*)static_QUType_varptr.get(_o+2)); break;
    case 1: draggedAImage(); break;
    case 2: requestNextItem(); break;
    case 3: showInHead((JahHeadable*)static_QUType_ptr.get(_o+1)); break;
    default:
	return QGLWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool QtPlayer::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool QtPlayer::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
