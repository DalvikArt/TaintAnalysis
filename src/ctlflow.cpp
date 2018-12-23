#include "ctlflow.h"

#include <iostream>
#include <hash_set>

#include "global.h"
#include "memop.h"

using namespace std;

VOID CallHandler(INS ins, CONTEXT *ctx)
{
    ADDRINT rsp = PIN_GetContextReg(ctx, REG_RSP);

    // remove rsp - 8 (the address return address will be pushed into) from tainted list

    removeMemTainted(rsp - 8);
}

VOID RetHandler(INS ins, ADDRINT insAddr, string disAsm, UINT64 stackPtr)
{
    // terminate process if the return address is in the tainted list
    hash_set<UINT64>::iterator itr = bytesTainted.find(stackPtr);
    if(itr != bytesTainted.end())
    {
        cout << "\033[1;31mOverflow occured!!!\033[0m" << " Process will be terminated." << endl;
        cout << hex << "Position: 0x" << insAddr << "\nInstruction: " << disAsm << "\nRSP: 0x" << stackPtr << endl;
        exit(0);
    }
}