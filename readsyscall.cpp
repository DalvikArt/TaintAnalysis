#include "readsyscall.h"

#include <iostream>
#include <asm/unistd.h>

#include "global.h"

using namespace std;

ADDRINT syscallNum = 0xffffffffffffffff;
UINT64 start = 0;
UINT fd = 0;

VOID SyscallEntryHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
    syscallNum = PIN_GetSyscallNumber(ctx, std);

    if(syscallNum == __NR_read)
    {
        start = (UINT64)PIN_GetSyscallArgument(ctx,std,1);
        fd = (UINT)PIN_GetSyscallArgument(ctx,std,0);
    }
}

VOID SyscallExitHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
    if(syscallNum == __NR_read)
    {
        if(fd == 3)
            return;

        UINT64 size = PIN_GetSyscallReturn(ctx,std);
        for(UINT64 i = 0; i < size; ++i)
        {
            bytesTainted.push_back(start + i);
        }
        cout << hex << "[Taint] In 0x" << start << " Size: 0x" << size << " fd:" << fd <<" (via read)" << endl;
    }

    syscallNum = 0xffffffffffffffff;

    // get the start address of read buffer
    //UINT64 start = (UINT64)PIN_GetSyscallArgument(ctx,std,1);
    // get the buf size
    //UINT64 size = (UINT64)PIN_GetSyscallArgument(ctx,std,2);

    //for(UINT64 i = 0; i < size; ++i)
    //    bytesTainted.push_back(start + i);

    #ifdef _VERBOSE
    // get the end addr of read buf
    //UINT64 end = start + size;
    // get fd of read syscall
    //UINT fd = (UINT)PIN_GetSyscallArgument(ctx,std,0);
    
    //cout << hex << "[Taint] From 0x" << start << " to 0x" << end << " Size: 0x" << size << " fd:" << fd <<" (via read)" << endl;
    #endif
}