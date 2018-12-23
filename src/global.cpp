#include "global.h"

// memory tainted list
hash_set<UINT64> bytesTainted;
// register tainted list
list<REG> regsTainted;

// malloc list
list<MallocArea> mallocAreaList;