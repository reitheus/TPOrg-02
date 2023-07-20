#include "lista.h"

#include "memory.h"

void iniciaLista(Lista *pLista){
    pLista->pCabeca = (Celula*)malloc(sizeof(Celula));
    pLista->pUltimo = pLista->pCabeca;
    pLista->pCabeca->prox = NULL;
    
}

void liberaLista(Lista *pLista){
    Celula* aux = pLista->pCabeca->prox;
    while (aux != NULL) {
        Celula* temp = aux;
        aux = aux->prox;
        free(temp);
    }
    free(pLista->pCabeca);
}

int listaVazia(Lista *pLista){
    return(pLista->pCabeca == pLista->pUltimo);
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

void retiraLista(Lista *pLista){
    
    Celula *aux = pLista->pCabeca;
    while(aux->prox != NULL){        
        aux = aux->prox;
    }
    pLista->pUltimo = aux;
    aux = pLista->pUltimo->prox;
    pLista->pUltimo->prox = NULL;
    free(aux);

}

void trocaLista(Lista *pLista,int x){
    Celula *aux = pLista->pCabeca;
    Celula *ant = NULL;
    while (aux->prox != NULL) {
        if (aux->item.tag == x) {
            
            ant->prox = aux->prox; // Remove o elemento da sua posição atual
            aux->prox = pLista->pCabeca->prox; // Coloca o elemento no início da lista
            pLista->pCabeca->prox = aux;
            break;
            
        }
        ant = aux;
        aux = aux->prox;
    }
}