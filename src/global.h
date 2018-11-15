#include "pin.H"
#include <list>

#define _VERBOSE

#define ALLOCATED  1
#define FREED      !ALLOCATED

typedef struct
{
  UINT64  base;
  UINT64  size;
  BOOL    status;
} MallocArea;

extern list<UINT64> bytesTainted;
extern list<REG> regsTainted;

extern list<MallocArea> mallocAreaList;