#include "generator.h"
#include <stdio.h>
#include <stdlib.h>

Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => this is an instruction
    // 02|33|12|01 => this is another instruction
            
    // 0 => opcode => move
    // 1 => opcode => sum
    // 2 => opcode => subtract
    //-1 => halt
    
    int n = 10;

    Instruction* instructions = (Instruction*) malloc((n + 1) * sizeof(Instruction));

    for (int i=0; i<n; i++) {
        instructions[i].add1.block = rand() % ramSize;
        instructions[i].add1.word = rand() % WORDS_SIZE;
        instructions[i].add2.block = rand() % ramSize;
        instructions[i].add2.word = rand() % WORDS_SIZE;
        instructions[i].add3.block = rand() % ramSize;
        instructions[i].add3.word = rand() % WORDS_SIZE;
        instructions[i].opcode = rand() % 3;
    }
    instructions[n].opcode = -1;
    
    return instructions;
}


Instruction* generateMultiplicationInstructions(int num1, int num2){

    Instruction *instructions = (Instruction*) malloc((num1+4) * sizeof(Instruction));

    instructions[0].opcode = 0;
    instructions[0].info1 = num1;//mandar o valor de num1 para ir para memoria
    instructions[0].info2 = 0;//aponta para qual endereço da memoria vai ficar o valor
    instructions[0].info3 = 0;

    instructions[1].opcode = 0;
    instructions[1].info1 = num2;//manda o valor de num2 para memoria
    instructions[1].info2 = 1;//aponta para endereço da memoria o valor de num2 vai ficar
    instructions[1].info3 = 0;

    instructions[2].opcode = 0;
    instructions[2].info1 = 0;//adiciona 0 na memoria
    instructions[2].info2 = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
    instructions[2].info3 = 0;

    multiplica(num1, instructions, 3);
  
    instructions[num1+3].opcode = -1;
    instructions[num1+3].info1 = -1;
    instructions[num1+3].info2 = -1;
    instructions[num1+3].info3 = -1;
    


    return instructions;
}

Instruction* generateDivisionInstructions(int num1, int num2) {
    Instruction* instructions = (Instruction*) malloc((num1+num1+6)* sizeof(Instruction));
    
    int c=1;
    instructions[0].opcode = 0;   // Salvar num1
    instructions[0].info1 = num1;
    instructions[0].info2 = 0;
    

    instructions[1].opcode = 0;   // Salvar num2
    instructions[1].info1 = num2;
    instructions[1].info2 = 1;


    instructions[2].opcode = 3; //copia num1 para a RAM[2]
    instructions[2].info1 = 0;
    instructions[2].info2 = 2;

    int i;
    for ( i = 3; i < num1 + 4; i += num2){
        instructions[i].opcode = 2;   
        instructions[i].info1 = 2;
        instructions[i].info2 = 1;
        instructions[i].info3 = 2;
        c=c+1;
    }
    
    instructions[i].opcode = 0;   
    instructions[i].info1 = c-2;
    instructions[i].info2 = 2;
    instructions[i].info3 = 0;
    
    instructions[i + 1].opcode = -1;   // Encerrar programa
    instructions[i + 1].info1 = -1;
    instructions[i + 1].info2 = -1;
    instructions[i + 1].info3 = -1;

    return instructions;
}

//num1 = base, num2 = expoente
Instruction* generatePowerInstructions(int num1, int num2){
  //quantidade de soma + instruções no loop + instruções basicas
  int tamInst =  (num1 * (num2 - 1)) + (num2 * 2) + 6;
  int aux;
  Instruction *instructions = (Instruction*) malloc(  tamInst * sizeof(Instruction));

  instructions[0].opcode = 0;
  instructions[0].info1 = num1;//mandar o valor de num1 para ir para memoria
  instructions[0].info2 = 0;//aponta para qual endereço da memoria vai ficar o valor
  instructions[0].info3 = 0;

  instructions[1].opcode = 0;
  instructions[1].info1 = num2;//manda o valor de num2 para memoria
  instructions[1].info2 = 1;//aponta para endereço da memoria o valor de num2 vai ficar
  instructions[1].info3 = 0;

  instructions[2].opcode = 0;
  instructions[2].info1 = 0;//adiciona 0 na memoria
  instructions[2].info2 = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
  instructions[2].info3 = 0;

  //loop para fazer a potencia, multiplicação sucessiva
  instructions[3].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
  instructions[3].info1 = 0;//copia o valor do endereço 0
  instructions[3].info2 = 1;//para o endereço 1
  instructions[3].info3 = 0;
  
  if(num1 == 2){
    aux = num2;
  }else{
    aux = num2 - 1;
  
  }
  
  for(int i = 4; i <  (num1 + 2) * aux ;i += num1 + 2){

       multiplica(num1, instructions, i);//percorre num1 instructions
       instructions[num1+i].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
       instructions[num1+i].info1 = 2;//copia o valor do endereço 2
       instructions[num1+i].info2 = 1;//para o endereço 1
       instructions[num1+i].info3 = 0;

       instructions[num1+i + 1].opcode = 0;
       instructions[num1+i + 1].info1 = 0;
       instructions[num1+i + 1].info2 = 2;
       instructions[num1+i + 1].info3 = 0;

      }

      instructions[tamInst - 2].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
      instructions[tamInst - 2].info1 = 1;//copia o valor do endereço 2
      instructions[tamInst - 2].info2 = 2;//para o endereço 1
      instructions[tamInst - 2].info3 = 0;

      instructions[tamInst - 1].opcode = -1;
      instructions[tamInst - 1].info1 = -1;
      instructions[tamInst - 1].info2 = -1;
      instructions[tamInst - 1].info3 = -1;

      return instructions;
    }

Instruction* generateFactorInstructions(int num1){
   int aux = 0;
   int num2 = num1;
   int j = num1 - 1;
  
    //calcula a quandtidade de somas necessaria para a fatoração
    
    while(j > 1){
      aux = aux + j;
      j--;
      }

    //quantidade de somas + instruções no loop + instruções externas
    int tamInst =  aux + ((num1 - 1) * 2) + 4;
    if(num1 == 1 || num1 == 2){
      tamInst = 5;
    }
    Instruction *instructions = (Instruction*) malloc(  tamInst * sizeof(Instruction));

    instructions[0].opcode = 0;
    instructions[0].info1 = num1 - 1;//mandar o valor de num1 para ir para memoria
    instructions[0].info2 = 0;//aponta para qual endereço da memoria vai ficar o valor
    instructions[0].info3 = 0;

    instructions[1].opcode = 0;
    instructions[1].info1 = num1;//manda o valor de num2 para memoria
    instructions[1].info2 = 1;//aponta para endereço da memoria o valor de num2 vai ficar
    instructions[1].info3 = 0;

    instructions[2].opcode = 0;
    instructions[2].info1 = 0;//adiciona 0 na memoria
    instructions[2].info2 = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
    instructions[2].info3 = 0;
    if(num1 == 1 || num1 == 2){
      
        instructions[3].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
        instructions[3].info1 = 1;//copia o valor do endereço 1
        instructions[3].info2 = 2;//para o endereço 2
        instructions[3].info3 = 0;
      
        instructions[tamInst - 1].opcode = -1;
        instructions[tamInst - 1].info1 = -1;
        instructions[tamInst - 1].info2 = -1;
        instructions[tamInst - 1].info3 = -1;
        return instructions;
    }  
    num1--;
    int i  = 3;
    while( i < (num2 - 1) * 2 + aux ){
       
        multiplica(num1, instructions, i);//percorre num1 instructions
    
        instructions[num1+i].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
        instructions[num1+i].info1 = 2;//copia o valor do endereço 2
        instructions[num1+i].info2 = 1;//para o endereço 1
        instructions[num1+i].info3 = 0;

        instructions[num1+ i + 1].opcode = 0;
        instructions[num1+ i + 1].info1 = 0;//adiciona 0 na memoria
        instructions[num1+ i + 1].info2 = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
        instructions[num1+ i + 1].info3 = 0;
         i += num1 + 2;
        num1--;

        }


  instructions[tamInst - 1].opcode = -1;
  instructions[tamInst - 1].info1 = -1;
  instructions[tamInst - 1].info2 = -1;
  instructions[tamInst - 1].info3 = -1;
  return instructions;
}


Instruction* generateFibonacciInstructions(int num1){
  //quantidade de soma + instruções no loop + instruções basicas
  int tamInst =  (num1 * 4) + 5;
  
  Instruction *instructions = (Instruction*) malloc(  tamInst * sizeof(Instruction));
  
  instructions[0].opcode = 0;
  instructions[0].info1 = 0;//mandar o valor de 0 para ir para memoria
  instructions[0].info2 = 0;//aponta para qual endereço da memoria vai ficar o valor
  instructions[0].info3 = 0;

  instructions[1].opcode = 0;
  instructions[1].info1 = 1;//manda o valor de 1 para memoria
  instructions[1].info2 = 1;//aponta para endereço da memoria o valor de num2 vai ficar
  instructions[1].info3 = 0;

  instructions[2].opcode = 0;
  instructions[2].info1 = 0;//adiciona 0 na memoria
  instructions[2].info2 = 2;//adiciona 0 na posição 2 da memoria, que será utilizada para armazenar o resultado depois
  instructions[2].info3 = 0;
  
  instructions[3].opcode = 0;
  instructions[3].info1 = 1;//adiciona 1 na memoria
  instructions[3].info2 = 3;//adiciona 0 na posição 3 da memoria, que será utilizada para armazenar o resultado depois
  instructions[3].info3 = 0;
  
  for(int i = 4; i < (num1 * 4); i+=4){
      instructions[i].opcode = 1;
      instructions[i].info1 = 0;
      instructions[i].info2 = 1;
      instructions[i].info3 = 2;
    
      instructions[i+1].opcode = 1;
      instructions[i+1].info1 = 2;
      instructions[i+1].info2 = 3;
      instructions[i+1].info3 = 3;
    
      instructions[i+2].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
      instructions[i+2].info1 = 1;//copia o valor do endereço 1
      instructions[i+2].info2 = 0;//para o endereço 0
      instructions[i+2].info3 = 0;
      
    
      instructions[i+3].opcode = 3;//função que copia valor dentro da ram de uma variavel para outra
      instructions[i+3].info1 = 2;//copia o valor do endereço 2
      instructions[i+3].info2 = 1;//para o endereço 1
      instructions[i+3].info3 = 0;
    
     
  
  }
  
  
  
  instructions[tamInst - 1].opcode = -1;
  instructions[tamInst - 1].info1 = -1;
  instructions[tamInst - 1].info2 = -1;
  instructions[tamInst - 1].info3 = -1;
  return instructions;
  

}



Instruction* readInstructions(char* fileName, int* memoriesSize) {
    Instruction* instructions = NULL;

    printf("FILE -> %s\n", fileName);
    FILE* file = fopen(fileName, "r"); // Open file in read mode
    
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto.\n");
        exit(1);
    }

    int n, i = 0;
    fscanf(file, "%d %d", &n, &memoriesSize[0]);
    instructions = (Instruction*) malloc(n * sizeof(Instruction));
    while (i < n) {
        fscanf(file, "%d %d %d %d %d %d %d", 
            &instructions[i].opcode, 
            &instructions[i].add1.block, &instructions[i].add1.word,
            &instructions[i].add2.block, &instructions[i].add2.word,
            &instructions[i].add3.block, &instructions[i].add3.word);
        i++;
    }
    fclose(file); // Close file

    return instructions;
}
