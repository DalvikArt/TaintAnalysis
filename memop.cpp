#include "memop.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

VOID ReadMemToRegHandler(INS ins, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;
    REG reg;
    
    for(i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            cout << hex << "[READ\tin 0x" << addr << "] 0x" << insAddr << ": " << disassamble << endl;
        }
    } 
}

VOID WriteMemHandler(INS ins, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;
    UINT64 writeAddr = writeOp;

    for(i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            cout << hex << "[WRITE\tin 0x" << addr << "] 0x" << insAddr << " write op : 0x"<< writeAddr <<": " << disassamble << endl;
        }
    } 
}