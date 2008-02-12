/****************************************************************************
** DirectoryView meta object code from reading C++ file 'dirview.h'
**
** Created: Sat Jan 26 08:34:14 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.7   edited Oct 19 16:22 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "dirview.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.7. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *DirectoryView::className() const
{
    return "DirectoryView";
}

QMetaObject *DirectoryView::metaObj = 0;
static QMetaObjectCleanUp cleanUp_DirectoryView( "DirectoryView", &DirectoryView::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString DirectoryView::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DirectoryView", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString DirectoryView::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "DirectoryView", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* DirectoryView::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QListView::staticMetaObject();
    static const QUParameter param_slot_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_0 = {"setDir", 1, param_slot_0 };
    static const QUMethod slot_1 = {"newCollection", 0, 0 };
    static const QUMethod slot_2 = {"renameCollection", 0, 0 };
    static const QUParameter param_slot_3[] = {
	{ "dir", &static_QUType_ptr, "Directory", QUParameter::In }
    };
    static const QUMethod slot_3 = {"deleteCollection", 1, param_slot_3 };
    static const QUMethod slot_4 = {"deleteSelectedCollection", 0, 0 };
    static const QUMethod slot_5 = {"collapseCurrentBranch", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_6 = {"slotFolderSelected", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "button", &static_QUType_int, 0, QUParameter::In },
	{ "item", &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ "pos", &static_QUType_varptr, "\x0e", QUParameter::In },
	{ "c", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"slotFolderSelectedViaPress", 4, param_slot_7 };
    static const QUMethod slot_8 = {"delayedSlotFolderSelection", 0, 0 };
    static const QUMethod slot_9 = {"selectionChangedHelperSlot", 0, 0 };
    static const QUMethod slot_10 = {"openFolder", 0, 0 };
    static const QUParameter param_slot_11[] = {
	{ 0, &static_QUType_ptr, "QListViewItem", QUParameter::In },
	{ 0, &static_QUType_varptr, "\x0e", QUParameter::In },
	{ 0, &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_11 = {"contextMenuCreator", 3, param_slot_11 };
    static const QMetaData slot_tbl[] = {
	{ "setDir(const QString&)", &slot_0, QMetaData::Public },
	{ "newCollection()", &slot_1, QMetaData::Public },
	{ "renameCollection()", &slot_2, QMetaData::Public },
	{ "deleteCollection(Directory*)", &slot_3, QMetaData::Public },
	{ "deleteSelectedCollection()", &slot_4, QMetaData::Public },
	{ "collapseCurrentBranch()", &slot_5, QMetaData::Public },
	{ "slotFolderSelected(QListViewItem*)", &slot_6, QMetaData::Protected },
	{ "slotFolderSelectedViaPress(int,QListViewItem*,const QPoint&,int)", &slot_7, QMetaData::Protected },
	{ "delayedSlotFolderSelection()", &slot_8, QMetaData::Protected },
	{ "selectionChangedHelperSlot()", &slot_9, QMetaData::Protected },
	{ "openFolder()", &slot_10, QMetaData::Protected },
	{ "contextMenuCreator(QListViewItem*,const QPoint&,int)", &slot_11, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"folderSelected", 1, param_signal_0 };
    static const QUMethod signal_1 = {"collectionDeleted", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "folderSelected(const QString&)", &signal_0, QMetaData::Public },
	{ "collectionDeleted()", &signal_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"DirectoryView", parentObject,
	slot_tbl, 12,
	signal_tbl, 2,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_DirectoryView.setMetaObject( metaObj );
    return metaObj;
}

void* DirectoryView::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "DirectoryView" ) )
	return this;
    return QListView::qt_cast( clname );
}

// SIGNAL folderSelected
void DirectoryView::folderSelected( const QString& t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

// SIGNAL collectionDeleted
void DirectoryView::collectionDeleted()
{
    activate_signal( staticMetaObject()->signalOffset() + 1 );
}

bool DirectoryView::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: setDir((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: newCollection(); break;
    case 2: renameCollection(); break;
    case 3: deleteCollection((Directory*)static_QUType_ptr.get(_o+1)); break;
    case 4: deleteSelectedCollection(); break;
    case 5: collapseCurrentBranch(); break;
    case 6: slotFolderSelected((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 7: slotFolderSelectedViaPress((int)static_QUType_int.get(_o+1),(QListViewItem*)static_QUType_ptr.get(_o+2),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+3)),(int)static_QUType_int.get(_o+4)); break;
    case 8: delayedSlotFolderSelection(); break;
    case 9: selectionChangedHelperSlot(); break;
    case 10: openFolder(); break;
    case 11: contextMenuCreator((QListViewItem*)static_QUType_ptr.get(_o+1),(const QPoint&)*((const QPoint*)static_QUType_ptr.get(_o+2)),(int)static_QUType_int.get(_o+3)); break;
    default:
	return QListView::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool DirectoryView::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: folderSelected((const QString&)static_QUType_QString.get(_o+1)); break;
    case 1: collectionDeleted(); break;
    default:
	return QListView::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool DirectoryView::qt_property( int id, int f, QVariant* v)
{
    return QListView::qt_property( id, f, v);
}

bool DirectoryView::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
