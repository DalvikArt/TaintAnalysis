#include "regop.h"

#include <iostream>
#include <list>

#include "global.h"

using namespace std;

bool checkAlreadyRegTainted(REG reg)
{
  list<REG>::iterator i;

  for(i = regsTainted.begin(); i != regsTainted.end(); i++){
    if (*i == reg){
      return true;
    }
  }
  return false;
}

bool taintReg(REG reg)
{
    if (checkAlreadyRegTainted(reg) == true)
    {
        std::cout << "\t\t\t" << REG_StringShort(reg) << " is already tainted" << std::endl;
        return false;
    }

    regsTainted.push_front(reg);
    return true;
}

bool removeRegTainted(REG reg)
{
    regsTainted.remove(reg);
    return true;
}

VOID RegisterHandler(INS ins)
{
    REG reg_r, reg_w;

    if (INS_OperandCount(ins) != 2)
        return;

    reg_r = INS_RegR(ins, 0);
    reg_w = INS_RegW(ins, 0);
  
    if (REG_valid(reg_w))
    {
        if (checkAlreadyRegTainted(reg_w) && (!REG_valid(reg_r) || !checkAlreadyRegTainted(reg_r)))
        {
            std::cout << "[SPREAD]\t\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << std::endl;
            std::cout << "\t\t\toutput: "<< REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << std::endl;
            removeRegTainted(reg_w);
        }
        else if (!checkAlreadyRegTainted(reg_w) && checkAlreadyRegTainted(reg_r))
        {
            std::cout << "[SPREAD]\t\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << std::endl;
            std::cout << "\t\t\toutput: " << REG_StringShort(reg_w) << " | input: "<< REG_StringShort(reg_r) << std::endl;
            taintReg(reg_w);
        }
    }
}