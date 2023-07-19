#include "lista.h"

Lista* iniciaLista(Lista *pLista){
    pLista->pCabeca = NULL;
    pLista->pUltimo = pLista->pCabeca;
    pLista->tam = 0;
    return pLista;
}

int listaVazia(Lista *pLista){
    return (pLista->pCabeca == pLista->pUltimo);
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

int retiraLista(Lista *pLista,Line *x){
    if(listaVazia(pLista))
        return 0;
    Celula *aux = pLista->pCabeca;
    while(aux->prox != NULL){
        pLista->pUltimo = aux;
        aux = pLista->pUltimo->prox;
    }

    return 1;
}