#include "pin.H"

VOID ReadMemToRegHandler(UINT64 insAddr, string insDis, UINT32 opCount, REG reg_r, UINT64 memOp);

VOID WriteMemHandler(UINT64 insAddr, string insDis, UINT32 opCount, REG reg_r, UINT64 memOp);

VOID removeMemTainted(UINT64 addr);