#include "memop.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

VOID ReadMemToRegHandler(UINT64 insAddr, string disassamble, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;
    
    for(i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            cout << hex << "[READ\tin 0x" << addr << "] 0x" << insAddr << ": " << disassamble << endl;
        }
    } 
}

VOID WriteMemHandler(UINT64 insAddr, string disassamble, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;

    for(i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            cout << hex << "[WRITE\tin 0x" << addr << "] 0x" << insAddr << ": " << disassamble << endl;
        }
    } 
}