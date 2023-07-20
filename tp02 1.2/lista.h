#ifndef LISTA_H
#define LISTA_H

#include "memory.h"

typedef struct celula{
    Line item;
    struct celula *prox;
}Celula;

typedef struct lista{
    Celula* pCabeca;
    Celula* pUltimo;
}Lista;

void iniciaLista(Lista* );
int listaVazia(Lista* );
void insereLista(Lista* ,Line );
void retiraLista(Lista* );
void liberaLista(Lista* );

//Busca pela linha e troca ela com a primeira posição
void trocaLista(Lista* ,int );

#endif//lista.h