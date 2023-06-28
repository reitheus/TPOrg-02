#include "cpu.h"
#include "generator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main(int argc, char**argv) {

    srand(time(NULL));   // Inicializacao da semente para os numeros aleatorios.

    if (argc != 5) {
        printf("Numero de argumentos invalidos! Sao 3.\n");
        printf("Linha de execucao: ./exe TIPO_INSTRUCAO [TAMANHO_RAM|ARQUIVO_DE_INSTRUCOES] [NUMERO1] [NUMERO2] \n");
        printf("\tExemplo 1 de execucao: ./exe random 10 0 0\n");
        printf("\tExemplo 2 de execucao: ./exe multi 10 02 03\n");
        printf("\tExemplo 3 de execucao: ./exe division 10 08 02\n");
        printf("\tExemplo 4 de execucao: ./exe power 10 03 03\n");
        printf("\tExemplo 5 de execucao: ./exe file arquivo_de_instrucoes.txt 0 0\n");
        return 0;
    }

    int ramSize,num1,num2;
    Machine machine;
    Instruction *instructions;

    if (strcmp(argv[1], "random") == 0) {
        ramSize = atoi(argv[2]);
        instructions = generateRandomInstructions(ramSize);
    } else if (strcmp(argv[1], "file") == 0) {
        instructions = readInstructions(argv[2], &ramSize);
    }else if(strcmp(argv[1], "multi")==0){
        ramSize = atoi(argv[2]);
        num1 = atoi(argv[3]);
        num2 = atoi(argv[4]);
        instructions = generateMultiplicationInstructions(num1,num2);
    }else if(strcmp(argv[1], "div")==0){
        ramSize = atoi(argv[2]);
        num1 = atoi(argv[3]);
        num2 = atoi(argv[4]);
        instructions = generateDivisionInstructions(num1,num2);
    }else if(strcmp(argv[1], "pow")==0){
        ramSize = atoi(argv[2]);
        num1 = atoi(argv[3]);
        num2 = atoi(argv[4]);
        instructions = generatePowerInstructions(num1,num2);
    }else{
        printf("Opcao invalida.\n");
        return 0;
    }
    
    printf("Iniciando a maquina...\n");
    start(&machine, instructions, ramSize);
    printRAM(&machine);
    run(&machine);
    printRAM(&machine);
    stop(&machine);
    printf("Finalizando a maquina...\n");
    return 0;
}
