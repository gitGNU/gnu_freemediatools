#ifndef SIMPLEOBSERVER_H
#define SIMPLEOBSERVER_H

class SimpleSubject;
    
class SimpleObserver {
    public:
        virtual ~SimpleObserver() {};
        virtual void updateNotified(SimpleSubject* theChangedSubject) = 0;
    protected:
        SimpleObserver() {};
};

#endif
