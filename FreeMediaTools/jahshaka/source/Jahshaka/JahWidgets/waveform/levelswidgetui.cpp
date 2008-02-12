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

#include "levelswidget.h"
#include <qcombobox.h>


void LevelsWidget::init()
{
    levelChooser ( "Luminosity" );
    setFocusPolicy(QWidget::NoFocus  );
    levelComboBox->setFocusPolicy(QWidget::NoFocus  );
}


void LevelsWidget::levelChooser( const QString &levelstring )
{
    if ( levelstring == "Luminosity" )
        levelsPlot->setLevel ( LevelsPlot::Luminosity );
   else if ( levelstring == "Reds" )
       levelsPlot->setLevel( LevelsPlot::Reds );
   else if ( levelstring == "Greens" )
        levelsPlot->setLevel ( LevelsPlot::Greens );
   else if ( levelstring == "Blues" )
        levelsPlot->setLevel ( LevelsPlot::Blues );
   else if ( levelstring == "RGB Stack" )
        levelsPlot->setLevel ( LevelsPlot::RGBStack );
   else if ( levelstring == "Split Horizontal" )
       levelsPlot->setLevel ( LevelsPlot::SplitH );
   else if ( levelstring == "Split Vertical" )
       levelsPlot->setLevel( LevelsPlot::SplitV );

   if ( levelsPlot != 0 )
       levelsPlot->rescanImage();
    
}
