#include "image.h"

#include <iostream>

#include "global.h"

using namespace std;

static size_t lastSize;

VOID callbackBeforeMalloc(ADDRINT size)
{
    lastSize = size;
}

VOID callbackBeforeFree(ADDRINT addr)
{ 
    list<MallocArea>::iterator i;
  
    cout << "[INFO] free(" << hex << addr << ")" << endl;
    for(i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        if (addr == i->base)
        {
            i->status = FREE;
            break;
        }
    }
}

VOID callbackAfterMalloc(ADDRINT ret)
{
    list<MallocArea>::iterator i;
    MallocArea elem;

    cout << "[INFO] malloc(" << lastSize << ") = " << hex << ret << endl;
    if (ret)
    {
        for(i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
        {
            if (ret == i->base)
            {
                i->status = ALLOCATE;
                i->size = lastSize;
                return;
            }
        }
        elem.base = ret;
        elem.size = lastSize;
        elem.status = ALLOCATE;
        mallocAreaList.push_front(elem);
    }
}

VOID Image(IMG img, VOID *v)
{
    RTN mallocRtn = RTN_FindByName(img, "malloc");
    RTN freeRtn = RTN_FindByName(img, "free");

    if (RTN_Valid(mallocRtn))
    {
        RTN_Open(mallocRtn);

        RTN_InsertCall(mallocRtn, IPOINT_BEFORE, (AFUNPTR)callbackBeforeMalloc, IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);

        RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR)callbackAfterMalloc, IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);

        RTN_Close(mallocRtn);
    }

    if (RTN_Valid(freeRtn))
    {
        RTN_Open(freeRtn);
        RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)callbackBeforeFree, IARG_FUNCARG_ENTRYPOINT_VALUE, 0,IARG_END);
        RTN_Close(freeRtn);
    }
}