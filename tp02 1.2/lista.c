#include "lista.h"

void iniciaLista(Lista *pLista){
    pLista->pCabeca = (Celula*)malloc(sizeof(Celula));
    pLista->pUltimo = pLista->pCabeca;
    pLista->pCabeca->prox = NULL;
    pLista->tam = 0;
}

int listaVazia(Lista *pLista){
    return (pLista->pCabeca == pLista->pUltimo);
}

void liberaLista(Lista *pLista){
    while(!listaVazia(pLista)){
        retiraLista(pLista);
    }
    free(pLista);
}

void insereLista(Lista *pLista,Line x){
    Celula *aux = (Celula*)malloc(sizeof(Celula));
    aux->item = x;
    aux->prox = NULL;
    pLista->tam++;
    if(listaVazia(pLista)){
        pLista->pCabeca = aux;
        pLista->pUltimo = aux;
    }else{
        pLista->pUltimo->prox = aux;
        pLista->pUltimo = aux;
    }
}

int retiraLista(Lista *pLista){
    if(listaVazia(pLista))
        return 0;
    Celula *ant = pLista->pCabeca;
    while(ant->prox != NULL){
        if(ant->prox == NULL){
            pLista->pUltimo = ant;
            ant = pLista->pUltimo->prox;
            pLista->pUltimo->prox = NULL;

        }
        ant = ant->prox;
    }
    free(ant);
    return 1;
}

void trocaLista(Lista *pLista,Line *x){
    Celula *aux = pLista->pCabeca;
    while(aux->prox != NULL){
        if(aux->prox->item.tag == x->tag){
            aux->prox = aux->prox->prox;
            aux->prox->prox = pLista->pCabeca->prox;
            pLista->pCabeca->prox = aux->prox;
            break;
        }
        aux = aux->prox;
    }
}