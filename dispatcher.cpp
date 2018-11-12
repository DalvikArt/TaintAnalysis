#include "dispatcher.h"

#include <iostream>
#include <asm/unistd.h>

#include "readsyscall.h"
#include "memop.h"

using namespace std;

VOID Syscall_Dispatcher(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v)
{
    if(PIN_GetSyscallNumber(ctx, std) == __NR_read) // syscall(SYS_read, fd, buf, count)
    {
        ReadSyscallHandler(ctx, std);
    }
}

VOID Instruction(INS ins, VOID *v)
{
    if (INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)ReadMemToRegHandler, IARG_ADDRINT, INS_Address(ins), 
            IARG_PTR, new string(INS_Disassemble(ins)), IARG_MEMORYOP_EA, 0, IARG_END);
    }
    else if (INS_MemoryOperandIsWritten(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WriteMemHandler,IARG_ADDRINT, INS_Address(ins),
            IARG_PTR, new string(INS_Disassemble(ins)), IARG_MEMORYOP_EA, 0, IARG_END);
    }
}