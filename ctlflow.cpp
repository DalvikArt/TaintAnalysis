#include "ctlflow.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

VOID RetHandler(INS ins, CONTEXT *ctx)
{
    ADDRINT sp = PIN_GetContextReg(ctx, REG_RSP);

    ADDRINT retAddr = 0;

    PIN_SafeCopy(&retAddr, (VOID *)sp, sizeof(retAddr));

    cout << hex << "Ret addr: 0x"<< retAddr << endl;

    for(list<UINT64>::iterator i = bytesTainted.begin(); i != bytesTainted.end(); ++i)
    {
        if(retAddr == *i)
        {
            cout << "Overflow occured!"<<endl;
            exit(0);
        }
    }
}