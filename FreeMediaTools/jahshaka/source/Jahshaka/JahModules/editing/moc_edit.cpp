/****************************************************************************
** GLEdit meta object code from reading C++ file 'edit.h'
**
** Created: Fri Feb 1 09:25:54 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "edit.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *GLEdit::className() const
{
    return "GLEdit";
}

QMetaObject *GLEdit::metaObj = 0;
static QMetaObjectCleanUp cleanUp_GLEdit( "GLEdit", &GLEdit::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString GLEdit::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLEdit", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString GLEdit::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "GLEdit", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* GLEdit::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = GLWorld::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ "parentbox", &static_QUType_ptr, "QHBox", QUParameter::In }
    };
    static const QUMethod slot_0 = {"initializeMediaTable", 1, param_slot_0 };
    static const QUMethod slot_1 = {"addLayer", 0, 0 };
    static const QUMethod slot_2 = {"changeParticle", 0, 0 };
    static const QUMethod slot_3 = {"changeFontDraw", 0, 0 };
    static const QUMethod slot_4 = {"changeObjectDraw", 0, 0 };
    static const QUMethod slot_5 = {"keyCliplayer", 0, 0 };
    static const QUMethod slot_6 = {"keyClipkey", 0, 0 };
    static const QUMethod slot_7 = {"updateUiSettings", 0, 0 };
    static const QUMethod slot_8 = {"grabDesktop", 0, 0 };
    static const QUMethod slot_9 = {"addDesktop", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ "newclip", &static_QUType_ptr, "assetData", QUParameter::In }
    };
    static const QUMethod slot_10 = {"addClipEdit", 1, param_slot_10 };
    static const QUParameter param_slot_11[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"updateanimframe", 1, param_slot_11 };
    static const QUMethod slot_12 = {"glworldUpdate", 0, 0 };
    static const QUParameter param_slot_13[] = {
	{ 0, &static_QUType_bool, 0, QUParameter::In },
	{ 0, &static_QUType_bool, 0, QUParameter::In }
    };
    static const QUMethod slot_13 = {"addListViewItem", 2, param_slot_13 };
    static const QUParameter param_slot_14[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_14 = {"updateEditanimframe", 1, param_slot_14 };
    static const QUParameter param_slot_15[] = {
	{ "frame", &static_QUType_int, 0, QUParameter::In },
	{ "layer", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_15 = {"updateSetinFrames", 2, param_slot_15 };
    static const QMetaData slot_tbl[] = {
	{ "initializeMediaTable(QHBox*)", &slot_0, QMetaData::Public },
	{ "addLayer()", &slot_1, QMetaData::Public },
	{ "changeParticle()", &slot_2, QMetaData::Public },
	{ "changeFontDraw()", &slot_3, QMetaData::Public },
	{ "changeObjectDraw()", &slot_4, QMetaData::Public },
	{ "keyCliplayer()", &slot_5, QMetaData::Public },
	{ "keyClipkey()", &slot_6, QMetaData::Public },
	{ "updateUiSettings()", &slot_7, QMetaData::Public },
	{ "grabDesktop()", &slot_8, QMetaData::Public },
	{ "addDesktop()", &slot_9, QMetaData::Public },
	{ "addClipEdit(assetData)", &slot_10, QMetaData::Public },
	{ "updateanimframe(int)", &slot_11, QMetaData::Public },
	{ "glworldUpdate()", &slot_12, QMetaData::Public },
	{ "addListViewItem(bool,bool)", &slot_13, QMetaData::Public },
	{ "updateEditanimframe(int)", &slot_14, QMetaData::Public },
	{ "updateSetinFrames(int,int)", &slot_15, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"GrabClip", 0, 0 };
    static const QUMethod signal_1 = {"AddClip", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "GrabClip()", &signal_0, QMetaData::Public },
	{ "AddClip()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"GLEdit", parentObject,
	slot_tbl, 16,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_GLEdit.setMetaObject( metaObj );
    return metaObj;
}

void* GLEdit::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "GLEdit" ) )
	return this;
    return GLWorld::qt_cast( clname );
}

// SIGNAL GrabClip
void GLEdit::GrabClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

// SIGNAL AddClip
void GLEdit::AddClip()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool GLEdit::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: initializeMediaTable((QHBox*)static_QUType_ptr.get(_o+1)); break;
    case 1: addLayer(); break;
    case 2: changeParticle(); break;
    case 3: changeFontDraw(); break;
    case 4: changeObjectDraw(); break;
    case 5: keyCliplayer(); break;
    case 6: keyClipkey(); break;
    case 7: updateUiSettings(); break;
    case 8: grabDesktop(); break;
    case 9: addDesktop(); break;
    case 10: addClipEdit((assetData)(*((assetData*)static_QUType_ptr.get(_o+1)))); break;
    case 11: updateanimframe((int)static_QUType_int.get(_o+1)); break;
    case 12: glworldUpdate(); break;
    case 13: addListViewItem((bool)static_QUType_bool.get(_o+1),(bool)static_QUType_bool.get(_o+2)); break;
    case 14: updateEditanimframe((int)static_QUType_int.get(_o+1)); break;
    case 15: updateSetinFrames((int)static_QUType_int.get(_o+1),(int)static_QUType_int.get(_o+2)); break;
    default:
	return GLWorld::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool GLEdit::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: GrabClip(); break;
    case 1: AddClip(); break;
    default:
	return GLWorld::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool GLEdit::qt_property( int id, int f, QVariant* v)
{
    return GLWorld::qt_property( id, f, v);
}

bool GLEdit::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
