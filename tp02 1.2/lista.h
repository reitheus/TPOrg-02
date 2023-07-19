#ifndef LISTA_H
#define LISTA_H

#include "memory.h"

typedef struct celula{
    Line item;
    Celula *prox;
}Celula;

typedef struct lista{
    Celula *pCabeca, *pUltimo;
    int tam;
}Lista;


Lista* iniciaLista(Lista* );

int listaVazia(Lista* );
void insereLista(Lista* ,Line );
int retiraLista(Lista* );

//Busca pela linha e troca ela com a primeira posição
void trocaLista(Lista* ,Line* );

#endif//lista.h