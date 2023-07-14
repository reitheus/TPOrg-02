#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction* generateMultiplicationInstructions(int, int);
Instruction* readInstructions(char*, int*);
Instruction* multiplica(int, Instructions*, int);
Instruction* generateDivisionInstructions(int num1, int num2);
Instruction* generatePowerInstructions(int num1, int num2);

#endif // !GENERATOR_H
