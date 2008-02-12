/*******************************************************************************
**
** The source file for the Jahshaka editing interface module
** The Jahshaka Project
** Copyright (C) 2000-2006 VM Inc.
** Released under the GNU General Public License
**
*******************************************************************************/

#include "networking.h"
#include <qwidgetstack.h>
#include <qtabwidget.h>

//////////////////////////////////////////////////
//UI switching routines

void JahNet::cominterface(void)
{
    emit    switchToNetCom();

    NetInterface->raiseWidget(0);
}

void JahNet::p2pinterface(void)
{
    emit    switchToNetP2P();

    NetInterface->raiseWidget(1);

    //theSharedNetLayout->raiseWidget(1);
}

void JahNet::netinterface(void)
{
    emit    switchToNetNet();

    NetInterface->raiseWidget(2);
    
    theSharedNetLayout->raiseWidget(0);
}

void JahNet::httpdinterface(void)
{
    emit    switchToNetHttpd();

    NetInterface->raiseWidget(3);

    //theSharedNetLayout->raiseWidget(0);
}

void JahNet::togglep2plocalview(QWidget * widget)
{
    QString rere;

    int index = jahp2p->indexOf(widget);
    
    qDebug("we de ya switching:"+rere.setNum(index));

    if (index<=0) theSharedNetLayout->raiseWidget(1);
    if (index>=1) theSharedNetLayout->raiseWidget(2);
}






























