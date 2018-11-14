#include "ctlflow.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

VOID CallHandler(INS ins, CONTEXT *ctx)
{
    ADDRINT rsp = PIN_GetContextReg(ctx, REG_RSP);

    bytesTainted.remove(rsp);
}

VOID RetHandler(INS ins, CONTEXT *ctx)
{
    ADDRINT rsp = PIN_GetContextReg(ctx, REG_RSP);

    for(list<UINT64>::iterator i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if(rsp == *i)
        {
            cout << "\033[1;31mOverflow occured!!!\033[0m" << " Process will be terminated." << endl;
            exit(0);
        }
    }
}