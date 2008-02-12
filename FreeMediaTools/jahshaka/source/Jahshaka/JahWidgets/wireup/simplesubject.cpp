
#include "simplesubject.h"

void SimpleSubject::Attach (SimpleObserver* obs) 
{
        _observers.push_back(obs);
}
 
void SimpleSubject::Detach (SimpleObserver* o) 
{
        _observers.remove(o);
}
    
void SimpleSubject::Notify () 
{
        std::list<Observer*>::iterator it(_observers);
    
        for ( it = _observers.begin(); it != _observers.end(); it++) {
            it.CurrentItem()->update(this);
        }
}     

    