#include "lista.h"

void iniciaLista(Lista *pLista){
    pLista->pCabeca = NULL;
    pLista->pUltimo = NULL;
}

int listaVazia(Lista *pLista){
    return (pLista->pCabeca == NULL);
}

void insereLista(Lista *pLista,Line x){
    Celula *aux = (Celula*)malloc(sizeof(Celula));
    aux->item = x;
    aux->prox = NULL;
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
    Celula *aux;
    aux = pLista->pCabeca->prox;
    *x = aux->item;
    pLista->pCabeca->prox = aux->prox;
    free(aux);
    return 1;
}