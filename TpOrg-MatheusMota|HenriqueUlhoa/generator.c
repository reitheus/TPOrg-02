#include "generator.h"
#include "cpu.h"
#include <stdio.h> 
#include <stdlib.h>
  
Instruction* generateRandomInstructions(int ramSize) {
    // 01|22|13|45 => isto é uma instrução
    // 02|33|12|01 => isto é outra instrução
            
    // 0 => salvar na memória
    // 1 => opcode => somar
    // 2 => opcode => subtrair
    //-1 => halt
    
    // 22 => significa um endereço da RAM (10 endereço) 
    // 13 => significa 2o endereço
    // 45 => significa 3o endereco
    //ramSize => ESTA FORA DO INTERVALO DE 0 A ramSize DA MEMÓRIA RAM

    Instruction* instructions = (Instruction*) malloc(10 * sizeof(Instruction));

    for (int i=0; i<9; i++){
        instructions[i].opcode = rand() % 4; //0, 1, 2, 3, 4
        instructions[i].info1 = rand() % ramSize; //0 ... RAM_SIZE
        do {
            instructions[i].info2 = rand() % ramSize; //0 ... RAM_SIZE
        } while (instructions[i].info1 == instructions[i].info2);
        instructions[i].info3 = rand() % ramSize; //0 ... RAM_SIZE
    }
    
    //inserindo a ultima instrucao do programa que nao faz nada que presta
    instructions[9].opcode =-1;
    instructions[9].info1 = -1;
    instructions[9].info2 = -1;
    instructions[9].info3 = -1;
    
    return instructions;
}

Instruction* multiplica(int num1, Instruction *instructions,int cont){

    for (int i = cont; i < num1+cont; i++){
        instructions[i].opcode = 1;//opcode de soma
        instructions[i].info1 = 2;
        instructions[i].info2 = 1;
        instructions[i].info3 = 2;
        
        
    }
  
    /*multiplicação que funciona
     for (int i = cont; i < num1+cont; i++){
        instructions[i].opcode = 1;//opcode de soma
        instructions[i].info1 = 2;
        instructions[i].info2 = 1;
        instructions[i].info3 = 2;
        
        
    }*/

  return 0;
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



Instruction* readInstructions(char* fileName, int* ramSize) {
    printf("FILE -> %s\n", fileName);
    FILE* file = fopen(fileName, "r"); // Abrindo arquivo no modo leitura
    
    if (file == NULL) {
        printf("Arquivo nao pode ser aberto.\n");
        exit(1);
    }

    int n, i = 0;
    fscanf(file, "%d %d", ramSize, &n);
    Instruction* instructions = (Instruction*) malloc(n * sizeof(Instruction));
    while (i < n) {
        fscanf(file, "%d %d %d %d", &instructions[i].opcode, &instructions[i].info1, &instructions[i].info2, &instructions[i].info3);
        i++;
    }
    fclose(file); // Fechando o arquivo

    return instructions;
}
