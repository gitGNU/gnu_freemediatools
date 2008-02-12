#include "jahhttpd.h"
#include "jhclient.h"

JahHttpd::JahHttpd(JahDesktop *desktop, Q_UINT16 port, int backlog, QObject *parent, const char *name)
    : QServerSocket(port, backlog, parent, name)
	, m_desktop( desktop )
{
    ;
}

JahHttpd::~JahHttpd( )
{
	JHClient::freeResults( );
}

void JahHttpd::newConnection(int socketid)
{
    JHClient *c = new JHClient(m_desktop, this);
    c->setSocket(socketid);
}

