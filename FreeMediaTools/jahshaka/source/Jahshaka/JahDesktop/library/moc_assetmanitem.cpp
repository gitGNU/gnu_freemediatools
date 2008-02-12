/****************************************************************************
** AssetManItem meta object code from reading C++ file 'assetmanitem.h'
**
** Created: Wed Jan 23 19:11:10 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "assetmanitem.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *AssetManItem::className() const
{
    return "AssetManItem";
}

QMetaObject *AssetManItem::metaObj = 0;
static QMetaObjectCleanUp cleanUp_AssetManItem( "AssetManItem", &AssetManItem::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString AssetManItem::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AssetManItem", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString AssetManItem::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "AssetManItem", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* AssetManItem::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"expand", 0, 0 };
    static const QUParameter param_slot_1[] = {
	{ "groupnum", &static_QUType_int, 0, QUParameter::In },
	{ "col", &static_QUType_varptr, "\x0a", QUParameter::In }
    };
    static const QUMethod slot_1 = {"setGroupBackgroundColor", 2, param_slot_1 };
    static const QUParameter param_slot_2[] = {
	{ "filename", &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_2 = {"setInfoFromFile", 1, param_slot_2 };
    static const QUMethod slot_3 = {"resetAllFields", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "item", &static_QUType_ptr, "QIconViewItem", QUParameter::In }
    };
    static const QUMethod slot_4 = {"setInfoFromIconViewItem", 1, param_slot_4 };
    static const QUMethod slot_5 = {"newSlot", 0, 0 };
    static const QUMethod slot_6 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "expand()", &slot_0, QMetaData::Public },
	{ "setGroupBackgroundColor(int,QColor)", &slot_1, QMetaData::Public },
	{ "setInfoFromFile(const QString&)", &slot_2, QMetaData::Public },
	{ "resetAllFields()", &slot_3, QMetaData::Public },
	{ "setInfoFromIconViewItem(QIconViewItem*)", &slot_4, QMetaData::Public },
	{ "newSlot()", &slot_5, QMetaData::Public },
	{ "languageChange()", &slot_6, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"assetExpanded", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "assetExpanded(int)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"AssetManItem", parentObject,
	slot_tbl, 7,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_AssetManItem.setMetaObject( metaObj );
    return metaObj;
}

void* AssetManItem::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "AssetManItem" ) )
	return this;
    return QWidget::qt_cast( clname );
}

// SIGNAL assetExpanded
void AssetManItem::assetExpanded( int t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool AssetManItem::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: expand(); break;
    case 1: setGroupBackgroundColor((int)static_QUType_int.get(_o+1),(QColor)(*((QColor*)static_QUType_ptr.get(_o+2)))); break;
    case 2: setInfoFromFile((const QString&)static_QUType_QString.get(_o+1)); break;
    case 3: resetAllFields(); break;
    case 4: setInfoFromIconViewItem((QIconViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 5: newSlot(); break;
    case 6: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool AssetManItem::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: assetExpanded((int)static_QUType_int.get(_o+1)); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool AssetManItem::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool AssetManItem::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
