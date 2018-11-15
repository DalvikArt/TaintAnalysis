#include "instruction.h"

#include <iostream>
#include <asm/unistd.h>

#include "readsyscall.h"
#include "memop.h"
#include "regop.h"
#include "ctlflow.h"

using namespace std;

VOID Instruction(INS ins, VOID *v)
{
    // insert READ handler if the memory operand is read and the first operand is register
    // like "move rax, [rbp + 0x20]"
    if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsRead(ins, 0) && INS_OperandIsReg(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)ReadMemToRegHandler, IARG_ADDRINT, INS_Address(ins), IARG_PTR, new string(INS_Disassemble(ins)), IARG_UINT32, 
            INS_OperandCount(ins), IARG_UINT32, INS_OperandReg(ins, 0), IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    }
    // insert WRITE handler if the memory operand is write, like "move word ptr [rdi], dx"
    else if (INS_OperandCount(ins) > 1 && INS_MemoryOperandIsWritten(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)WriteMemHandler, IARG_ADDRINT, INS_Address(ins), IARG_PTR, new string(INS_Disassemble(ins)), IARG_UINT32, 
            INS_OperandCount(ins), IARG_UINT32, INS_OperandReg(ins, 1), IARG_MEMORYOP_EA, 0, IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    }
    // insert REGISTER spread handler if both of the operands are registers
    // like "move rax, rdx"
    else if(INS_OperandCount(ins) > 1 && INS_OperandIsReg(ins, 0))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RegisterHandler, IARG_PTR, ins, IARG_END);
    }
    // insert RETURN handler if the instruction is return
    else if(INS_IsRet(ins))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)RetHandler, IARG_PTR, ins, IARG_ADDRINT, INS_Address(ins), IARG_PTR, new string(INS_Disassemble(ins)), IARG_REG_VALUE, REG_STACK_PTR, IARG_END);
    }
    // insert CALL handler if the instruction is call
    else if(INS_IsCall(ins))
    {
        INS_InsertCall(ins, IPOINT_BEFORE, (AFUNPTR)CallHandler, IARG_PTR, ins, IARG_CONTEXT, IARG_END);
    }
}