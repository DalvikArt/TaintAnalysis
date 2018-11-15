#include "global.h"

// memory tainted list
list<UINT64> bytesTainted;
// register tainted list
list<REG> regsTainted;

// malloc list
list<MallocArea> mallocAreaList;