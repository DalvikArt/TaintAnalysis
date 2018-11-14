#include "dispatcher.h"

#include <iostream>
#include <asm/unistd.h>

#include "readsyscall.h"
#include "memop.h"
#include "regop.h"
#include "ctlflow.h"

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
    if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)ReadMemToRegHandler, IARG_PTR, ins, IARG_MEMORYOP_EA, 0, IARG_END);
    }
    else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WriteMemHandler, IARG_PTR, ins, IARG_MEMORYOP_EA, 0, IARG_END);
    }
    else if (INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegisterHandler, IARG_PTR, ins, IARG_END);
    }
    else //if (INS_IsRet(ins))
    {
        string disasm(INS_Disassemble(ins));

        if((int)disasm.find("ret") != -1)
        {
            INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RetHandler, IARG_PTR, ins, IARG_CONTEXT, IARG_END);
        }
    }
}