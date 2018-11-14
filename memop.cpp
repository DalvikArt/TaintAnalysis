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

VOID ReadMemToRegHandler(INS ins, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;
    REG reg_r;
    
    if(INS_OperandCount(ins) != 2)
    return;

    reg_r = INS_OperandReg(ins, 0);

    for(i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if (addr == *i)
        {
            #ifdef _VERBOSE
            cout << hex << "[READ in " << addr << "]\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
            #endif
            taintReg(reg_r);
            return ;
        }
    } 

    if (checkAlreadyRegTainted(reg_r))
    {
        #ifdef _VERBOSE
        cout << hex << "[READ in " << addr << "]\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
        #endif
        removeRegTainted(reg_r);
    }
}

VOID WriteMemHandler(INS ins, UINT64 memOp)
{
    list<UINT64>::iterator i;
    UINT64 addr = memOp;
    REG reg_r;

    if(INS_OperandCount(ins) != 2)
        return;
    
    reg_r = INS_OperandReg(ins,0);

    for(i = bytesTainted.begin(); i != bytesTainted.end(); i++)
    {
        if (addr == *i)
        {
            #ifdef _VERBOSE
            cout << hex << "[WRITE in " << addr << "]\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
            #endif
            if (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r))
                removeMemTainted(addr);
            return ;
        }
    }
    if (checkAlreadyRegTainted(reg_r))
    {
        #ifdef _VERBOSE
        cout << hex << "[WRITE in " << addr << "]\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
        #endif
        addMemTainted(addr);
    }
}