#include "ctlflow.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

VOID CallHandler(INS ins, CONTEXT *ctx)
{
    ADDRINT rsp = PIN_GetContextReg(ctx, REG_RSP);

    // remove rsp - 8 (the address return address will be pushed into) from tainted list
    bytesTainted.remove(rsp - 8);
}

VOID RetHandler(INS ins, ADDRINT insAddr, string disAsm, UINT64 stackPtr)
{
    // terminate process if the return address is in the tainted list
    for(list<UINT64>::iterator i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if(stackPtr == *i)
        {
            cout << "\033[1;31mOverflow occured!!!\033[0m" << " Process will be terminated." << endl;
            cout << hex << "Position: 0x" << insAddr << "\nInstruction: " << disAsm << "\nRSP: 0x" << stackPtr << endl;
            exit(0);
        }
    }
}