#include <iostream>
#include "pin.H"

#include "dispatcher.h"

using namespace std;

int main(int argc, char *argv[])
{
    if(PIN_Init(argc,argv))
    {
        cerr << "load file " << argv[0] << "failed!" << endl;
        return -1;
    }

    PIN_AddSyscallEntryFunction(Syscall_Dispatcher, NULL);

    INS_AddInstrumentFunction(Instruction, NULL);

    PIN_StartProgram();

    return 0;
}