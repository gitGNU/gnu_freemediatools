#ifndef JAHHTTPD_H
#define JAHHTTPD_H

#include <qserversocket.h>

class JahDesktop;

class JahHttpd : public QServerSocket {
    Q_OBJECT
public:
    JahHttpd(JahDesktop *desktop, Q_UINT16 =8080, int =1, QObject * =0, const char * =0);
	~JahHttpd( );
    void newConnection(int);
// TODO
// add rebind, in order to take advantage of new NICs
// add unbind and bind calls after the object is instantiated
// add feedback about connection status with signals
private:
	JahDesktop *m_desktop;
};

#endif // JAHHTTPD_H

