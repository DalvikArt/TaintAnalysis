#include "readsyscall.h"

#include <iostream>

#include "global.h"

using namespace std;

void ReadSyscallHandler(CONTEXT *ctx, SYSCALL_STANDARD std)
{
    // get the start address of read buffer
    UINT64 start = (UINT64)PIN_GetSyscallArgument(ctx,std,1);
    // get the buf size
    UINT64 size = (UINT64)PIN_GetSyscallArgument(ctx,std,2);

    for(UINT64 i = 0; i < size; ++i)
        bytesTainted.push_back(start + i);

    #ifdef _VERBOSE
    // get the end addr of read buf
    UINT64 end = start + size;
    // get fd of read syscall
    UINT fd = (UINT)PIN_GetSyscallArgument(ctx,std,0);
    
    cout << hex << "[Taint] From 0x" << start << " to 0x" << end << " Size: 0x" << size << " fd:" << fd <<" (via read)" << endl;
    #endif
}