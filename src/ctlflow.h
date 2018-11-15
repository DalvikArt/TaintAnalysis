#include "pin.H"

VOID CallHandler(INS ins, CONTEXT *ctx);

VOID RetHandler(INS ins, ADDRINT insAddr, string disAsm, UINT64 stackPtr);