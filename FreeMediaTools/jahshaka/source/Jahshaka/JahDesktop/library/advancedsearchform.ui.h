/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you want to add, delete, or rename functions or slots, use
** Qt Designer to update this file, preserving your code.
**
** You should not define a constructor or destructor in this file.
** Instead, write your code in functions called init() and destroy().
** These will automatically be called by the form's constructor and
** destructor.
*****************************************************************************/

#include <qpushbutton.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qtable.h>
#include <qstringlist.h>
#include <qfontmetrics.h>
#include <qmessagebox.h>
#include <qheader.h>
#include "embedded-images.h" // from qembed script

#include "dirview.h"
#include "AssetManager.h" // from openassetlib
#include "DataSource.h"
using namespace olib::openassetlib;
class olib::openassetlib::DataSource;

void advancedSearchForm::init()
{
    // (if we want to enable row moving, mustnt hide the left hand header...)
    table->setShowGrid( true );
    table->horizontalHeader()->hide();
    //    table->verticalHeader()->hide();
    //   table->setLeftMargin(0);
    table->setTopMargin(0);

    //table->setFocusStyle( QTable::FollowStyle );
    //table->setSelectionMode( QTable::SingleRow );
    QFontMetrics fm( table->font() );
    table->setColumnWidth( 3,  2* fm.width( " AND " )); // not great, but the up/down arrows have width.
    table->erase();
    
    // NB we dont necessarily have a data source at init time... TODO resolve this.
    //addCondition();
    datasource = 0;

}

void advancedSearchForm::addCondition()
{
    if ( !datasource ) // TODO improve this... confusing for user :(
    {     QMessageBox::information( this, "" , "You must choose a data source\n before constructing a search.", "OK"  );
          return;
    }
    
    QFontMetrics fm( table->font() );
    
    // Populate searchable fields etc.
    QStringList strl;
    strl.clear();
    std::list<std::string> fields = datasource->searchableFields();
    std::list<std::string>::iterator it = fields.begin();
    while ( it != fields.end() )
    {   strl.push_back( QString( (*it).c_str() ) );
        ++it;
    }

    QComboTableItem *fieldscom = new QComboTableItem( table, strl );
    fieldscom->setCurrentItem( "Filename" );

    strl.clear();
    strl.push_back("Contains");
    strl.push_back("Equals");
    strl.push_back("Not equal to");
    strl.push_back("Less than");
    strl.push_back("Greater than");

    QComboTableItem *contcom = new QComboTableItem( table, strl );
    strl.clear();
    strl.push_back("AND");
    strl.push_back("OR");
    QComboTableItem *logiccom = new QComboTableItem( table, strl );
    logiccom->setStringList( strl );

    int row = table->numRows();
    table->setNumRows( table->numRows() +1 );
    table->setItem( row, 0, fieldscom );
    table->setItem( row, 1, contcom );

    QString conv;
    // Could set number on left, but they get messed up if we delete rows.
    //conv.setNum( row +1 );
    table->verticalHeader()->setLabel( row, QIconSet( qembed_findImage("tick-mini")), "");//, conv );

    table->setText(row, 2, "*");
    if ( row > 0 )
        table->setItem( row -1, 3, logiccom );
    else 
        delete logiccom;

    // table->setRowHeight( row,  fm.height() + 15 ); 

}


void advancedSearchForm::deleteSelectedCondition()
{   //int currow = table->currentRow();
    if ( table->currentRow() >= 0 )
    {   table->removeRow(  table->currentRow() );
        // For clarity, we never leave a AND/OR logical combo on the last item.
        if ( table->currentRow() == table->numRows() -1 && table->item( table->currentRow() ,3) )
        {  
            QTableItem *item = table->item( table->currentRow() ,3);
            table->takeItem( item );
            delete item;
        }
    }

}

void advancedSearchForm::listViewSelectionChangedSlot( QListViewItem* i )
{
    if ( !i )
        return;
    Directory *dir;
    dir = dynamic_cast<Directory*>(i);
    if ( !dir )
        return;
    
    if ( dir->dataSource() && datasource != dir->dataSource() )
    {   datasource = dir->dataSource();
        table->erase();
        addCondition();
    }
}


void advancedSearchForm::executeSearch()
{
    if ( table->numRows() == 0 || !datasource )
        return;

    // We must make sure table cells are not in editable mode, else we dont get update for 
    // that cell text. This small hack de-selects the current cell, forcing its editor to flush.
    table->setCurrentCell( table->currentRow(), 0 );
    
    std::list<DataSource::Query::Condition*> conditions;
    DataSource::Query::Condition *pcondition = 0;
    DataSource::Query::Condition  *condition = 0;

    for ( int row = 0; row < table->numRows(); row++ )
    {   
        
        DataSource::Query::Condition::Comparitor compar = DataSource::Query::Condition::Contains;
        QString compstr = table->text(row, 1 );
        if ( compstr.contains("Contains") )
            compar = DataSource::Query::Condition::Contains;
        else if ( compstr.contains("Equals") )
            compar = DataSource::Query::Condition::EqualTo;
        else if ( compstr.contains("Not equal to") )
            compar = DataSource::Query::Condition::NotEqualTo;
        else if ( compstr.contains("Greater than" ) )
            compar = DataSource::Query::Condition::GreaterThan;
        else if ( compstr.contains("Less than") )
            compar = DataSource::Query::Condition::LessThan;
    
        QString searchstr = table->text( row, 2 ).stripWhiteSpace();

        bool strok = true;
        if ( searchstr.length() == 0 )
            strok = false; // ignore blank searches.
        // We dont actually use wildcards yet...
        searchstr = searchstr.replace( "*", "" );

        condition = new DataSource::Query::Condition( table->text(row, 0).latin1(), searchstr.latin1(), compar ); 
        conditions.push_back( condition );
        if ( !pcondition )
            pcondition = condition;
        else // combine with previous condition
        {   QString logicstr = table->text( row-1, 3 );
            DataSource::Query::Condition::Logic logic = DataSource::Query::Condition::AND;
            if ( logicstr.contains( "OR" ) )
                logic = DataSource::Query::Condition::OR;
            DataSource::Query::Condition  *combcondition = new DataSource::Query::Condition( pcondition, condition, logic ); 
            conditions.push_back( combcondition );
            pcondition = combcondition;
        }      
    }

    DataSource::Query query( pcondition );
    query.maxHits( hitLimitSpinBox->value() );
    
    // Delete assets from our old hit list.
    std::list<Asset*>::iterator hitit = hits.begin();
    while( hitit != hits.end() )
    {   delete *hitit;
        ++hitit;
    }
    hits.clear();

    hits = datasource->search( query );
    printf("Hits found = %i \n", hits.size() );

    emit hitsToShow( hits ); // NB asset pointers are valid until next search.
    
    // Tidy up conditions we used
    std::list<DataSource::Query::Condition*>::iterator it = conditions.begin();
    while ( it != conditions.end() )
    {   delete *it;
        ++it;
    }

}
