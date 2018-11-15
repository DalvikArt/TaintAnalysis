#include "memop.h"

#include <iostream>
#include <list>
#include <hash_set>

#include "global.h"
#include "regop.h"

using namespace std;

// remove mem from tainted list
VOID removeMemTainted(UINT64 addr)
{
    hash_set<UINT64>::iterator itr = bytesTainted.find(addr);

    if(itr != bytesTainted.end())
    {
        bytesTainted.erase(itr);
        #ifdef _VERBOSE
        cout << hex << "\t\t\t" << addr << " is now freed" << endl;
        #endif
    }

}

// remove mem into tainted list
VOID addMemTainted(UINT64 addr)
{
    bytesTainted.insert(addr);
    #ifdef _VERBOSE
    cout << hex << "\t\t\t" << addr << " is now tainted" << endl;
    #endif
}

VOID ReadMemToRegHandler(UINT64 insAddr, string insDis, UINT32 opCount, REG reg_r, UINT64 memOp)
{
    UINT64 addr = memOp;
    
    if(opCount != 2)
        return;

    // check the malloc list
    for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        // print UAF information if the operand is in allocated area and this area has been freed
        if (addr >= i->base && addr < (i->base + i->size) && i->status == FREED)
        {
            cout << hex << "[UAF in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            return;
        }
    }

    // taint the register if mem read is tained mem
    
    hash_set<UINT64>::iterator itr = bytesTainted.find(addr);
    if(itr != bytesTainted.end())
    {
        #ifdef _VERBOSE
        cout << hex << "[READ in " << addr << "]\t" << insAddr << ": " << insDis << endl;
        #endif
        taintReg(reg_r);
        return;
    }
    

    // remove reg from tainted list if the mem is not a tainted mem and the reg is a tained reg
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

    // UAF check (same as read handler)
    for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        if (addr >= i->base && addr < (i->base + i->size) && i->status == FREED)
        {
            cout << hex << "[UAF in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            return;
        }
    }

    // remove mem from tainted list if the reg is not a tainted reg
    hash_set<UINT64>::iterator itr = bytesTainted.find(addr);
    if(itr != bytesTainted.end())
    {
            #ifdef _VERBOSE
            cout << hex << "[WRITE in " << addr << "]\t" << insAddr << ": " << insDis << endl;
            #endif
            if (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r))
                removeMemTainted(addr);
            return ;
    }

    // add mem into tainted list if the reg is a tainted reg and the mem is not a tained mem
    if (checkAlreadyRegTainted(reg_r))
    {
        #ifdef _VERBOSE
        cout << hex << "[WRITE in " << addr << "]\t" << insAddr << ": " << insDis << endl;
        #endif
        addMemTainted(addr);
    }
}