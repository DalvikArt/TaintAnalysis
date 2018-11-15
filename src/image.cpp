#include "image.h"

#include <iostream>

#include "global.h"

using namespace std;

static size_t lastSize;

VOID callbackBeforeMalloc(ADDRINT size)
{
    // save the malloc size
    lastSize = size;
}

VOID callbackBeforeFree(ADDRINT addr)
{ 
    #ifdef _VERBOSE
    cout << "[INFO] free(" << hex << addr << ")" << endl;
    #endif

    // mark the malloc area as FREED
    for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
    {
        if (addr == i->base)
        {
            i->status = FREED;
            break;
        }
    }
}

VOID callbackAfterMalloc(ADDRINT ret)
{
    #ifdef _VERBOSE
    cout << "[INFO] malloc(" << lastSize << ") = " << hex << ret << endl;
    #endif

    if (ret)
    {
        // mark the area as ALLOCATED if the malloc area is in malloc list
        for(list<MallocArea>::iterator i = mallocAreaList.begin(); i != mallocAreaList.end(); i++)
        {
            if (ret == i->base)
            {
                i->status = ALLOCATED;
                i->size = lastSize;
                return;
            }
        }

        // add area to malloc list if the area is not in malloc list
        MallocArea elem;
        elem.base = ret;
        elem.size = lastSize;
        elem.status = ALLOCATED;

        mallocAreaList.push_front(elem);
    }
}

VOID Image(IMG img, VOID *v)
{
    // get malloc and free rtn
    RTN mallocRtn = RTN_FindByName(img, "malloc");
    RTN freeRtn = RTN_FindByName(img, "free");

    // insert malloc before and after callback
    if (RTN_Valid(mallocRtn))
    {
        RTN_Open(mallocRtn);

        RTN_InsertCall(mallocRtn, IPOINT_BEFORE, (AFUNPTR)callbackBeforeMalloc, IARG_FUNCARG_ENTRYPOINT_VALUE, 0, IARG_END);

        RTN_InsertCall(mallocRtn, IPOINT_AFTER, (AFUNPTR)callbackAfterMalloc, IARG_FUNCRET_EXITPOINT_VALUE, IARG_END);

        RTN_Close(mallocRtn);
    }

    // insert free before callback
    if (RTN_Valid(freeRtn))
    {
        RTN_Open(freeRtn);
        RTN_InsertCall(freeRtn, IPOINT_BEFORE, (AFUNPTR)callbackBeforeFree, IARG_FUNCARG_ENTRYPOINT_VALUE, 0,IARG_END);
        RTN_Close(freeRtn);
    }
}