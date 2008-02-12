#ifndef SIMPLESUBJECT_H
#define SIMPLESUBJECT_H

class SimpleObserver;
#include <list>

class SimpleSubject {
    public:
        virtual ~SimpleSubject();

   //? enum { InterestAll } Interest;
    
    virtual void Attach(SimpleObserver* obs);
    virtual void Detach(SimpleObserver* o);
    virtual void Notify(); // Notify( Interest a ) ?
    protected:
        SimpleSubject();
    private:
        std::list<SimpleObserver*> _observers;
    };
    


#endif
