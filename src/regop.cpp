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
    #ifdef _VERBOSE
    cout << "\t\t\t" << REG_StringShort(reg) << " is already tainted" << endl;
    #endif

    return false;
  }

  switch(reg){

    case REG_RAX:  regsTainted.push_front(REG_RAX);
    case REG_EAX:  regsTainted.push_front(REG_EAX); 
    case REG_AX:   regsTainted.push_front(REG_AX); 
    case REG_AH:   regsTainted.push_front(REG_AH); 
    case REG_AL:   regsTainted.push_front(REG_AL); 
         break;

    case REG_RBX:  regsTainted.push_front(REG_RBX);
    case REG_EBX:  regsTainted.push_front(REG_EBX);
    case REG_BX:   regsTainted.push_front(REG_BX);
    case REG_BH:   regsTainted.push_front(REG_BH);
    case REG_BL:   regsTainted.push_front(REG_BL);
         break;

    case REG_RCX:  regsTainted.push_front(REG_RCX); 
    case REG_ECX:  regsTainted.push_front(REG_ECX);
    case REG_CX:   regsTainted.push_front(REG_CX);
    case REG_CH:   regsTainted.push_front(REG_CH);
    case REG_CL:   regsTainted.push_front(REG_CL);
         break;

    case REG_RDX:  regsTainted.push_front(REG_RDX); 
    case REG_EDX:  regsTainted.push_front(REG_EDX); 
    case REG_DX:   regsTainted.push_front(REG_DX); 
    case REG_DH:   regsTainted.push_front(REG_DH); 
    case REG_DL:   regsTainted.push_front(REG_DL); 
         break;

    case REG_RDI:  regsTainted.push_front(REG_RDI); 
    case REG_EDI:  regsTainted.push_front(REG_EDI); 
    case REG_DI:   regsTainted.push_front(REG_DI); 
    case REG_DIL:  regsTainted.push_front(REG_DIL); 
         break;

    case REG_RSI:  regsTainted.push_front(REG_RSI); 
    case REG_ESI:  regsTainted.push_front(REG_ESI); 
    case REG_SI:   regsTainted.push_front(REG_SI); 
    case REG_SIL:  regsTainted.push_front(REG_SIL); 
         break;

    default:
        #ifdef _VERBOSE
        cout << "\t\t\t" << REG_StringShort(reg) << " can't be tainted" << endl;
        #endif
        return false;
  }
  #ifdef _VERBOSE
  cout << "\t\t\t" << REG_StringShort(reg) << " is now tainted" << endl;
  #endif
  return true;
}

bool removeRegTainted(REG reg)
{
  switch(reg){

    case REG_RAX:  regsTainted.remove(REG_RAX);
    case REG_EAX:  regsTainted.remove(REG_EAX);
    case REG_AX:   regsTainted.remove(REG_AX);
    case REG_AH:   regsTainted.remove(REG_AH);
    case REG_AL:   regsTainted.remove(REG_AL);
         break;

    case REG_RBX:  regsTainted.remove(REG_RBX);
    case REG_EBX:  regsTainted.remove(REG_EBX);
    case REG_BX:   regsTainted.remove(REG_BX);
    case REG_BH:   regsTainted.remove(REG_BH);
    case REG_BL:   regsTainted.remove(REG_BL);
         break;

    case REG_RCX:  regsTainted.remove(REG_RCX); 
    case REG_ECX:  regsTainted.remove(REG_ECX);
    case REG_CX:   regsTainted.remove(REG_CX);
    case REG_CH:   regsTainted.remove(REG_CH);
    case REG_CL:   regsTainted.remove(REG_CL);
         break;

    case REG_RDX:  regsTainted.remove(REG_RDX); 
    case REG_EDX:  regsTainted.remove(REG_EDX); 
    case REG_DX:   regsTainted.remove(REG_DX); 
    case REG_DH:   regsTainted.remove(REG_DH); 
    case REG_DL:   regsTainted.remove(REG_DL); 
         break;

    case REG_RDI:  regsTainted.remove(REG_RDI); 
    case REG_EDI:  regsTainted.remove(REG_EDI); 
    case REG_DI:   regsTainted.remove(REG_DI); 
    case REG_DIL:  regsTainted.remove(REG_DIL); 
         break;

    case REG_RSI:  regsTainted.remove(REG_RSI); 
    case REG_ESI:  regsTainted.remove(REG_ESI); 
    case REG_SI:   regsTainted.remove(REG_SI); 
    case REG_SIL:  regsTainted.remove(REG_SIL); 
         break;

    default:
      return false;
  }
  #ifdef _VERBOSE
  cout << "\t\t\t" << REG_StringShort(reg) << " is now freed" << endl;
  #endif
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
            #ifdef _VERBOSE
            cout << "[SPREAD]\t\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
            cout << "\t\t\toutput: "<< REG_StringShort(reg_w) << " | input: " << (REG_valid(reg_r) ? REG_StringShort(reg_r) : "constant") << endl;
            #endif
            removeRegTainted(reg_w);
        }
        else if (!checkAlreadyRegTainted(reg_w) && checkAlreadyRegTainted(reg_r))
        {
            #ifdef _VERBOSE
            cout << "[SPREAD]\t\t" << INS_Address(ins) << ": " << INS_Disassemble(ins) << endl;
            cout << "\t\t\toutput: " << REG_StringShort(reg_w) << " | input: "<< REG_StringShort(reg_r) << endl;
            #endif
            taintReg(reg_w);
        }
    }
}