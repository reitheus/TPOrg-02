#ifndef LISTA_H
#define LISTA_H

#include "memory.h"

typedef struct celula{
    Line item;
    Celula *prox;
}Celula;

typedef struct lista{
    Celula *pCabeca, *pUltimo;
}Lista;

void iniciaLista(Lista* ,int );
int listaVazia(Lista* );
void insereLista(Lista* ,Line );
int retiraLista(Lista* ,Line* );

#endif//lista.h