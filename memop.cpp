#include "memop.h"

#include <iostream>
#include <list>

#include "global.h"
#include "regop.h"

using namespace std;

VOID removeMemTainted(UINT64 addr)
{
    bytesTainted.remove(addr);
    #ifdef _VERBOSE
    cout << hex << "\t\t\t" << addr << " is now freed" << endl;
    #endif
}

VOID addMemTainted(UINT64 addr)
{
    bytesTainted.push_back(addr);
    #ifdef _VERBOSE
    cout << hex << "\t\t\t" << addr << " is now tainted" << endl;
    #endif
}

VOID ReadMemToRegHandler(UINT64 insAddr, string insDis, UINT32 opCount, REG reg_r, UINT64 memOp)
{
    UINT64 addr = memOp;
    
    if(opCount != 2)
        return;

    for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        if (addr >= i->base && addr < (i->base + i->size) && i->status == FREE)
        {
            cout << hex << "[UAF in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            return;
        }
    }

    for(list<UINT64>::iterator i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            #ifdef _VERBOSE
            cout << hex << "[READ in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            #endif
            taintReg(reg_r);
            return;
        }
    } 

    if (checkAlreadyRegTainted(reg_r))
    {
        #ifdef _VERBOSE
        cout << hex << "[READ in " << addr << "]\t" << insAddr << ": " << insDis << endl;
        #endif
        removeRegTainted(reg_r);
    }
}

VOID WriteMemHandler(UINT64 insAddr, string insDis, UINT32 opCount, REG reg_r, UINT64 memOp)
{
    UINT64 addr = memOp;

    if(opCount != 2)
        return;

    for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        if (addr >= i->base && addr < (i->base + i->size) && i->status == FREE)
        {
            cout << hex << "[UAF in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            return;
        }
    }

    for(list<UINT64>::iterator i = bytesTainted.begin(); i != bytesTainted.end(); i++)
    {
        if (addr == *i)
        {
            #ifdef _VERBOSE
            cout << hex << "[WRITE in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            #endif
            if (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r))
                removeMemTainted(addr);
            return ;
        }
    }
    if (checkAlreadyRegTainted(reg_r))
    {
        #ifdef _VERBOSE
        cout << hex << "[WRITE in " << addr << "]\t" << insAddr << ": " << insDis << endl;
        #endif
        addMemTainted(addr);
    }
}