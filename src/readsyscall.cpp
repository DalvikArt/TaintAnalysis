#include "readsyscall.h"

#include <iostream>
#include <asm/unistd.h>

#include "global.h"

using namespace std;

// store value got from entry handler
// will be used in exit handler
ADDRINT syscallNum = 0xffffffffffffffff;
UINT64 start = 0;
UINT fd = 0;

VOID SyscallEntryHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
    // get syscall number
    syscallNum = PIN_GetSyscallNumber(ctx, std);

    // get buffer addr and fd if the syscall num is read(0)
    if(syscallNum == __NR_read)
    {
        start = (UINT64)PIN_GetSyscallArgument(ctx,std,1);
        fd = (UINT)PIN_GetSyscallArgument(ctx,std,0);
    }
}

VOID SyscallExitHandler(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
    // syscall num is read
    if(syscallNum == __NR_read)
    {
        if(fd == 3)
            return;

        // get the syscall return as read num (bytes)
        UINT64 size = PIN_GetSyscallReturn(ctx,std);
        for(UINT64 i = 0; i < size; ++i)
        {
            // add every byte in tainted list
            bytesTainted.push_back(start + i);
        }

        #ifdef _VERBOSE
        cout << hex << "[Taint] In 0x" << start << " Size: 0x" << size << " fd:" << fd <<" (via read)" << endl;
        #endif
    }

    // set syscall num to -1 everytime it was used
    syscallNum = 0xffffffffffffffff;
}