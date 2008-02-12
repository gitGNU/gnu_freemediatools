/****************************************************************************
** advancedSearchForm meta object code from reading C++ file 'advancedsearchform.h'
**
** Created: Wed Jan 23 19:11:11 2008
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.6   edited Mar 8 17:43 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "advancedsearchform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *advancedSearchForm::className() const
{
    return "advancedSearchForm";
}

QMetaObject *advancedSearchForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_advancedSearchForm( "advancedSearchForm", &advancedSearchForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString advancedSearchForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "advancedSearchForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString advancedSearchForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "advancedSearchForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* advancedSearchForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"addCondition", 0, 0 };
    static const QUMethod slot_1 = {"deleteSelectedCondition", 0, 0 };
    static const QUParameter param_slot_2[] = {
	{ "i", &static_QUType_ptr, "QListViewItem", QUParameter::In }
    };
    static const QUMethod slot_2 = {"listViewSelectionChangedSlot", 1, param_slot_2 };
    static const QUMethod slot_3 = {"executeSearch", 0, 0 };
    static const QUMethod slot_4 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "addCondition()", &slot_0, QMetaData::Public },
	{ "deleteSelectedCondition()", &slot_1, QMetaData::Public },
	{ "listViewSelectionChangedSlot(QListViewItem*)", &slot_2, QMetaData::Public },
	{ "executeSearch()", &slot_3, QMetaData::Public },
	{ "languageChange()", &slot_4, QMetaData::Protected }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_ptr, "std::list<olib::openassetlib::Asset*>", QUParameter::In }
    };
    static const QUMethod signal_0 = {"hitsToShow", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "hitsToShow(std::list<olib::openassetlib::Asset*>)", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"advancedSearchForm", parentObject,
	slot_tbl, 5,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_advancedSearchForm.setMetaObject( metaObj );
    return metaObj;
}

void* advancedSearchForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "advancedSearchForm" ) )
	return this;
    return QWidget::qt_cast( clname );
}

#include <qobjectdefs.h>
#include <qsignalslotimp.h>

// SIGNAL hitsToShow
void advancedSearchForm::hitsToShow( std::list<olib::openassetlib::Asset*> t0 )
{
    if ( signalsBlocked() )
	return;
    QConnectionList *clist = receivers( staticMetaObject()->signalOffset() + 0 );
    if ( !clist )
	return;
    QUObject o[2];
    static_QUType_ptr.set(o+1,&t0);
    activate_signal( clist, o );
}

bool advancedSearchForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: addCondition(); break;
    case 1: deleteSelectedCondition(); break;
    case 2: listViewSelectionChangedSlot((QListViewItem*)static_QUType_ptr.get(_o+1)); break;
    case 3: executeSearch(); break;
    case 4: languageChange(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool advancedSearchForm::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: hitsToShow((std::list<olib::openassetlib::Asset*>)(*((std::list<olib::openassetlib::Asset*>*)static_QUType_ptr.get(_o+1)))); break;
    default:
	return QWidget::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool advancedSearchForm::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool advancedSearchForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
