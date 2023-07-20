#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpulru.h"

Instruction* generateRandomInstructions(int);
Instruction* readInstructions(char*, int*);

#endif // !GENERATOR_H