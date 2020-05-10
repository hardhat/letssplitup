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
        void and(Wire x, Wire y){if (x.isOn() && y.isOn() == true){powered = true;}}
        void or(Wire x; Wire y){if (x.isOn() || y.isOn() == true){powered = true;}}
        void nand(Wire x; Wire y){if (x.isOn() && y.isOn() != true){powered = true;}}
        void nor(Wire x; Wire y){if (x.isOn() || y.isOn() != true){powered = true;}}

    protected:

    private:
};

#endif // GATE_H
