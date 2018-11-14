#include "pin.H"

// syscall dispatcher
VOID Syscall_Dispatcher(THREADID thread_id, CONTEXT *ctx, SYSCALL_STANDARD std, void *v);

// instruction dispatcher
VOID Instruction(INS ins, VOID *v);