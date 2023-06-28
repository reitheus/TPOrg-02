#ifndef GENERATOR_H
#define GENERATOR_H

#include "cpu.h"

Instruction* generateRandomInstructions(int);
Instruction* generateMultiplicationInstructions(int, int);
Instruction* readInstructions(char*, int*);
Instruction* generateDivisionInstructions(int num1, int num2);
Instruction* generatePowerInstructions(int num1, int num2);
Instruction* generateFactorInstructions(int num1);
Instruction* generateFibonacciInstructions(int num1);

#endif // !GENERATOR_H
