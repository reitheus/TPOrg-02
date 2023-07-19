#include "instruction.h"

Address getAdd1(Instruction* instruction) { return instruction->add1; }
void setAdd1(Instruction* instruction, Address add) { instruction->add1 = add; }

Address getAdd2(Instruction* instruction) { return instruction->add2; }
void setAdd2(Instruction* instruction, Address add) { instruction->add2 = add; }

Address getAdd3(Instruction* instruction) { return instruction->add3; }
void setAdd3(Instruction* instruction, Address add) { instruction->add3 = add; }

int getOpcode(Instruction* instruction) { return instruction->opcode; }
void setOpcode(Instruction* instruction, int opcode) { instruction->opcode = opcode; }


Instruction* multiplica(int num1, Instruction *instructions,int cont){

    for (int i = cont; i < num1+cont; i++){
        instructions[i].opcode = 1;//opcode de soma
        instructions[i].add1.word = 2;
        instructions[i].add2.word = 1;
        instructions[i].add3.word = 2;
        
    }
  
  return 0;
}
//instrução para multiplicação
Instruction* generateMultiplicationInstructions(int num1, int num2){

    Instruction *instructions = (Instruction*) malloc((num1+4) * sizeof(Instruction));

    //instrução para colocar um valor na ram
    instructions[0].opcode = 0;
    instructions[0].add1.block = num1;//mandar o valor de num1 para ir para memoria
    instructions[0].add2.word = 0;//aponta para qual endereço da memoria vai ficar o valor
    instructions[0].add3.word = 0;

    instructions[1].opcode = 0;
    instructions[1].add1.block = num2;//manda o valor de num2 para memoria
    instructions[1].add2.word = 1;//aponta para endereço da memoria o valor de num2 vai ficar
    instructions[1].add3.word = 0;

    instructions[2].opcode = 0;
    instructions[2].add1.block = 0;//adiciona 0 na memoria
    instructions[2].add2.word = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
    instructions[2].add3.word = 0;

    multiplica(num1, instructions, 3);
  
    instructions[num1+3].opcode = -1;
    instructions[num1+3].add1.word = -1;
    instructions[num1+3].add2.word = -1;
    instructions[num1+3].add3.word = -1;
    


    return instructions;
}




//Instrução para divisão
Instruction* generateDivisionInstructions(int num1, int num2) {
    Instruction* instructions = (Instruction*) malloc((num1+num1+6)* sizeof(Instruction));
    
    int c=1;
    instructions[0].opcode = 0;   // Salvar num1
    instructions[0].add1.block = num1;
    instructions[0].add2.word  = 0;
    

    instructions[1].opcode = 0;   // Salvar num2
    instructions[1].add1.block = num2;
    instructions[1].add2.word  = 1;


    instructions[2].opcode = 3; //copia num1 para a RAM[2]
    instructions[2].add1.word = 0;
    instructions[2].add2.word  = 2;

    int i;
    for ( i = 3; i < num1 + 4; i += num2){
        instructions[i].opcode = 2;   
        instructions[i].add1.word = 2;
        instructions[i].add2.word  = 1;
        instructions[i].add3.word = 2;
        c=c+1;
    }
    
    instructions[i].opcode = 0;   
    instructions[i].add1.block = c-2;
    instructions[i].add2.word  = 2;
    instructions[i].add3.word = 0;
    
    instructions[i + 1].opcode = -1;   // Encerrar programa
    instructions[i + 1].add1.word = -1;
    instructions[i + 1].add2.word  = -1;
    instructions[i + 1].add3.word = -1;

    return instructions;
}


//num1 = base, num2 = expoente
Instruction* generatePowerInstructions(int num1, int num2){
  //quantidade de soma + instruções no loop + instruções basicas
  int tamInst =  (num1 * (num2 - 1)) + (num2 * 2) + 6;
  int aux;
  Instruction *instructions = (Instruction*) malloc(  tamInst * sizeof(Instruction));

  instructions[0].opcode = 0;
  instructions[0].add1.block = num1;//mandar o valor de num1 para ir para memoria
  instructions[0].add2.word = 0;//aponta para qual endereço da memoria vai ficar o valor
  instructions[0].add3.word = 0;

  instructions[1].opcode = 0;
  instructions[1].add1.block = num2;//manda o valor de num2 para memoria
  instructions[1].add2.word = 1;//aponta para endereço da memoria o valor de num2 vai ficar
  instructions[1].add3.word = 0;

  instructions[2].opcode = 0;
  instructions[2].add1.block = 0;//adiciona 0 na memoria
  instructions[2].add2.word = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
  instructions[2].add3.word = 0;

  //loop para fazer a potencia, multiplicação sucessiva
  instructions[3].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
  instructions[3].add1.word = 0;//copia o valor do endereço 0
  instructions[3].add2.word = 1;//para o endereço 1
  instructions[3].add3.word = 0;
  
  if(num1 == 2){
    aux = num2;
  }else{
    aux = num2 - 1;
  
  }
  
  for(int i = 4; i <  (num1 + 2) * aux ;i += num1 + 2){

       multiplica(num1, instructions, i);//percorre num1 instructions
       instructions[num1+i].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
       instructions[num1+i].add1.word = 2;//copia o valor do endereço 2
       instructions[num1+i].add2.word = 1;//para o endereço 1
       instructions[num1+i].add3.word = 0;

       instructions[num1+i + 1].opcode = 0;
       instructions[num1+i + 1].add1.block = 0;
       instructions[num1+i + 1].add2.word = 2;
       instructions[num1+i + 1].add3.word = 0;

    }
    
    instructions[tamInst - 2].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
    instructions[tamInst - 2].add1.word = 1;//copia o valor do endereço 2
    instructions[tamInst - 2].add2.word = 2;//para o endereço 1
    instructions[tamInst - 2].add3.word = 0;
    
    instructions[tamInst - 1].opcode = -1;
    instructions[tamInst - 1].add1.word = -1;
    instructions[tamInst - 1].add2.word = -1;
    instructions[tamInst - 1].add3.word = -1;
  
    return instructions;
}

