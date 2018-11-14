#include "pin.H"

VOID RegisterHandler(INS ins);

bool checkAlreadyRegTainted(REG reg);

bool taintReg(REG reg);
bool removeRegTainted(REG reg);