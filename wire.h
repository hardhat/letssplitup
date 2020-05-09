#ifndef WIRE_H
#define WIRE_H
#include "tile.h"

//The wire class.
//When previous wire in an array is powered, the current wire will become powered.
class Wire
{
    public:
        Wire();
        //Tile tile;
        bool powered;
        virtual ~Wire();
        bool isOn(){return powered;}//a simple check to see if the metioned wire is on.
        void setPowered(bool x){powered = x;}//a method to power the wire.



};

#endif // WIRE_H
