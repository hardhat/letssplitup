#ifndef GATE_H
#define GATE_H
#include "wire.h"
//A subclass of wire.
//When a gate's condition is met, it will become powered and it will power the next wire in the array.
class Gate: public Wire
{
    public:
        Gate();
        virtual ~Gate();

    protected:

    private:
};

#endif // GATE_H
