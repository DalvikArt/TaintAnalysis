#include <iostream>
#include "pin.H"

#include "readsyscall.h"
#include "instruction.h"
#include "image.h"

using namespace std;

int main(int argc, char *argv[])
{
    // init symbols to add instrument in libs
    PIN_InitSymbols();

    // exit if pin init failed
    if(PIN_Init(argc,argv))
    {
        cerr << "load file " << argv[0] << "failed!" << endl;
        return -1;
    }

    // hook syscall
    PIN_AddSyscallEntryFunction(SyscallEntryHandler, NULL);
    PIN_AddSyscallExitFunction(SyscallExitHandler,NULL);

    // add image instrument
    IMG_AddInstrumentFunction(Image, NULL);

    // add instruction instrument
    INS_AddInstrumentFunction(Instruction, NULL);

    // start program
    PIN_StartProgram();

    return 0;
}