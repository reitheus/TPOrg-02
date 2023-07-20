#ifndef MMU_H
#define MMU_H

#include "memory.h"
#include "instruction.h"
#include "cpu.h"

typedef enum {
    L1Hit = 1,
    L2Hit,
    L3Hit,
    RAMHit
} WhereWasHit;

char* convertToString(WhereWasHit);

Line* MMUSearchOnMemorys(Address, Machine*, WhereWasHit*);

Line* MMUSearchOnMemorysLfu(Address add, Machine* machine, WhereWasHit* whereWasHit);

Line* MMUSearchOnMemorysLru(Address add, Machine* machine, WhereWasHit* whereWasHit);

#endif // !MMU_H