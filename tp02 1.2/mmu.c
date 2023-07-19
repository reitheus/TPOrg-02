#include "mmu.h"
#include "lista.h"
#include <stdio.h>


bool canOnlyReplaceBlock(Line);

//verifica se pode trocar na cache
bool canReplaceBlock(Cache*,Line);

int memoryCacheMapping(int, Cache*);

//verifica todo cache até achar a liha que contem o bloco da ram ou uma linha vazia, retorna -1 se não achar
int memoryCacheMappingAssociative(int address, Cache* cache); 

int lineWhichWillLeave(int, Cache*);

//verifica quem vai sair da cache e retorna um inteiro de pos
int lineWhichWillLeaveLfu( Cache* cache);

//verifica quem vai sair da cache e retorna um inteiro de pos
int lineWhichWillLeaveLru(Lista *pLista, Cache* cache);

void updateMachineInfos(Machine*, WhereWasHit*, int);

void moveLine();

char* convertToString(WhereWasHit whereWasHit) {
    switch (whereWasHit) {
        case L1Hit:
            return "CL1";
        case L2Hit:
            return "CL2";
        case L3Hit:
            return "CL3";
        case RAMHit:
            return "RAM";
    }
    return NULL;
}

Line* MMUSearchOnMemorys(Address add, Machine* machine, WhereWasHit* whereWasHit) {
    // Strategy => write back
    
    // Direct memory map
    int l1pos = memoryCacheMapping(add.block, &machine->l1);
    int l2pos = memoryCacheMapping(add.block, &machine->l2);
    int l3pos = memoryCacheMapping(add.block, &machine->l3);

    Line* cache1 = machine->l1.lines;
    Line* cache2 = machine->l2.lines;
    Line* cache3 = machine->l3.lines;
    MemoryBlock* RAM = machine->ram.blocks;
    int cost = 0;

    if (cache1[l1pos].tag == add.block) { //se o conteudo na memoria cache é igual a que está sendo procurada da hit
        /* Block is in memory cache L1 */
        cost = COST_ACCESS_L1;
        *whereWasHit = L1Hit;
    } else if (cache2[l2pos].tag == add.block) { 
        /* Block is in memory cache L2 */
        cache2[l2pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2;
        *whereWasHit = L2Hit;
        // Just works for Direct Mapping
        {
            Line tmp = cache1[l1pos];
            cache1[l1pos] = cache2[l2pos];
            int newL2pos = lineWhichWillLeave(tmp.tag, &machine->l2); /* Need to check the position of the block that will leave the L1 */
            if (!canOnlyReplaceBlock(cache2[newL2pos])) { 
                RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
            }
            cache2[newL2pos] = tmp;
        }
    } else if (cache3[l3pos].tag == add.block) { 
        /* Block is in memory cache L3 */
        cache3[l3pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
        *whereWasHit = L3Hit;
        // Just works for Direct Mapping
        {
            Line tmp = cache1[l1pos];
            cache1[l1pos] = cache3[l3pos];
            int newL3pos = lineWhichWillLeave(tmp.tag, &machine->l3); /* Need to check the position of the block that will leave the L1 */
            if (!canOnlyReplaceBlock(cache3[newL3pos])) { 
                RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
            }
            cache3[newL3pos] = tmp;
        }
    } else { 
        /* Block only in memory RAM, need to bring it to cache and manipulate the blocks */
        int newL2pos = lineWhichWillLeave(cache1[l1pos].tag, &machine->l2); /* Need to check the position of the block that will leave the L1 */
        int newL3pos = lineWhichWillLeave(cache1[l1pos].tag, &machine->l3);
        if (!canOnlyReplaceBlock(cache1[l1pos])) { 
            /* The block on cache L1 cannot only be replaced, the memories must be updated */
            if (!canOnlyReplaceBlock(cache2[newL2pos])){ 
                /* The block on cache L2 cannot only be replaced, the memories must be updated */
                if (!canOnlyReplaceBlock(cache3[newL3pos])){ 
                    /* The block on cache L2 cannot only be replaced, the memories must be updated */
                    
                    RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                }
                cache3[newL3pos] = cache2[l2pos];
                //RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
            }
            cache2[newL2pos] = cache1[l1pos];
        }
        cache1[l1pos].block = RAM[add.block];
        cache1[l1pos].tag = add.block;
        cache1[l1pos].updated = false;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
        *whereWasHit = RAMHit;
    }
    updateMachineInfos(machine, whereWasHit, cost);
    return &(cache1[l1pos]);
}

Line* MMUSearchOnMemorysLfu(Address add, Machine* machine, WhereWasHit* whereWasHit) {
    // Strategy => write back
    
    //  associative memory map -> Mapeamento por associação

    
    int l1pos = memoryCacheMappingAssociative(add.block, &machine->l1);
    int l2pos = memoryCacheMappingAssociative(add.block, &machine->l2);
    int l3pos = memoryCacheMappingAssociative(add.block, &machine->l3);

    Line* cache1 = machine->l1.lines;
    Line* cache2 = machine->l2.lines;
    Line* cache3 = machine->l3.lines;
    MemoryBlock* RAM = machine->ram.blocks;
    int cost = 0;

    if (cache1[l1pos].tag == add.block) { //se o conteudo na memoria cache é igual a que está sendo procurada da hit
        /* Block is in memory cache L1 */
        cache1[l1pos].cont++;
        cost = COST_ACCESS_L1;
        *whereWasHit = L1Hit;
    } else if (cache2[l2pos].tag == add.block) { 
        /* Block is in memory cache L2 */
        cache2[l2pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2;
        *whereWasHit = L2Hit;
        // Just works for Direct Mapping
        {
           l1pos = lineWhichWillLeaveLfu(&machine->l1);
            
            Line tmp1 = cache1[l1pos];
            
            if (!canOnlyReplaceBlock(cache1[l1pos])) { //se nao puder colocar na 1 atualiza na cache2

                int newL2pos = lineWhichWillLeaveLfu(&machine->l2);
                Line tmp2 = cache2[newL2pos];

                if (!canOnlyReplaceBlock(cache2[newL2pos])) { 

                    int newL3pos = lineWhichWillLeaveLfu(&machine->l3);
                    //Line tmp3 = cache3[newL3pos];
                        
                    if (!canOnlyReplaceBlock(cache3[newL3pos])) {

                        RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;

                    }

                    cache3[newL3pos] = tmp2;
                    cache3[newL3pos].cont = 0;

                }

                cache2[newL2pos] = tmp1;
                cache2[newL2pos].cont = 0;
            }
            cache2[l2pos].cont++;
            
            cache1[l1pos] = cache2[l2pos];
            cache1[l1pos].cont = 0;

             /* Need to check the position of the block that will leave the L1 */
            // if (!canOnlyReplaceBlock(cache2[newL2pos])) { 
            //     RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
            // }
            // cache2[newL2pos] = tmp;
        }
    } else if (cache3[l3pos].tag == add.block) { 
        /* Block is in memory cache L3 */
        cache3[l3pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
        *whereWasHit = L3Hit;
        // Just works for Direct Mapping
        {
  
            l1pos = lineWhichWillLeaveLfu(&machine->l1);
            
            Line tmp1 = cache1[l1pos];
            
            if (!canOnlyReplaceBlock(cache1[l1pos])) { //se nao puder colocar na 1 atualiza na cache2

                int newL2pos = lineWhichWillLeaveLfu(&machine->l2);
                Line tmp2 = cache2[newL2pos];

                if (!canOnlyReplaceBlock(cache2[newL2pos])) { 

                    int newL3pos = lineWhichWillLeaveLfu(&machine->l3);
                    //Line tmp3 = cache3[newL3pos];
                        
                    if (!canOnlyReplaceBlock(cache3[newL3pos])) {

                        RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                    }

                    cache3[newL3pos] = tmp2;
                    cache3[newL3pos].cont = 0;
                }

                cache2[newL2pos] = tmp1;
                cache2[newL2pos].cont = 0;
            }
            cache3[l3pos].cont++;
            cache1[l1pos] = cache3[l3pos];
            cache1[l1pos].cont = 0;
        }
    } else { 
        /* Block only in memory RAM, need to bring it to cache and manipulate the blocks */
        //int newL1pos = lineWhichWillLeaveLfu( &machine->l1);
        int newL2pos = memoryCacheMappingAssociative(add.block, &machine->l2); /* Need to check the position of the block that will leave the L1 */
        int newL3pos = memoryCacheMappingAssociative(add.block, &machine->l3); 
        
        if (!canOnlyReplaceBlock(cache1[l1pos])) { 
            /* The block on cache L1 cannot only be replaced, the memories must be updated */
            if (!canOnlyReplaceBlock(cache2[newL2pos])){ 
                /* The block on cache L2 cannot only be replaced, the memories must be updated */
                if (!canOnlyReplaceBlock(cache3[newL3pos])){ 
                    /* The block on cache L3 cannot only be replaced, the memories must be updated */
                    
                    RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                }

                cache3[newL3pos] = cache2[newL2pos];
                cache3[newL3pos].cont = 0;
                //RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
            }
            cache2[newL2pos] = cache1[l1pos];
            cache2[newL2pos].cont = 0;
        }
        cache1[l1pos].block = RAM[add.block];
        cache1[l1pos].tag = add.block;
        cache1[l1pos].cont = 0;
        cache1[l1pos].updated = false;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
        *whereWasHit = RAMHit;
    }
    updateMachineInfos(machine, whereWasHit, cost);
    return &(cache1[l1pos]);
}

Line* MMUSearchOnMemorysLru(Address add, Machine* machine, WhereWasHit* whereWasHit) {
    // Strategy => write back
    
    //associative memory map -> Mapeamento por associação

    int l1pos = memoryCacheMappingAssociative(add.block, &machine->l1);
    int l2pos = memoryCacheMappingAssociative(add.block, &machine->l2);
    int l3pos = memoryCacheMappingAssociative(add.block, &machine->l3);

    Line* cache1 = machine->l1.lines;
    Line* cache2 = machine->l2.lines;
    Line* cache3 = machine->l3.lines;
    MemoryBlock* RAM = machine->ram.blocks;
    Lista *listaL1 = iniciaLista(listaL1);
    Lista *listaL2 = iniciaLista(listaL2);
    Lista *listaL3 = iniciaLista(listaL3);
    int cost = 0;

    if (cache1[l1pos].tag == add.block) { //se o conteudo na memoria cache é igual a que está sendo procurada da hit
        /* bloco está na cache1 */
        trocaLista(listaL1,&cache1[l1pos]);
        cost = COST_ACCESS_L1;
        *whereWasHit = L1Hit;
    } else if (cache2[l2pos].tag == add.block) { 
        /* contéudo está na cache 2 */
        cache2[l2pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2;
        *whereWasHit = L2Hit;

        {

            int newl1pos = lineWhichWillLeaveLru(listaL1,&machine->l1);//pos de quem está saindo da cache 1           
            Line tmp = cache1[newl1pos];//armazena o valor de quem ta saindo da cache 1

            if (!canReplaceBlock(cache2,tmp)){ //se nao puder colocar na 2 atualiza na cache 3

                if (!canReplaceBlock(cache3,tmp)){ //se não puder colocar na 3 atualiza na RAM

                    retiraLista(listaL1);
                    RAM[cache1[newL3pos].tag] = cache1[newL3pos].block;
                }
                insereLista(listaL3,tmp);
                cache3[newl1pos] = tmp;
            }
            retiraLista(listaL1);
            insereLista(listaL1,cache2[l2pos]);
            cache1[l1pos] = cache2[l2pos];

        }
    } else if (cache3[l3pos].tag == add.block) { 
        /* Block is in memory cache L3 */
        cache3[l3pos].tag = add.block;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3;
        *whereWasHit = L3Hit;
        // Just works for Direct Mapping
        {
  
            l1pos = lineWhichWillLeaveLru(listaL1,&machine->l1);
            Line tmp1 = cache1[l1pos];
            
            if (!canOnlyReplaceBlock(cache1[l1pos])) { //se nao puder colocar na 1 atualiza na cache2

                int newL2pos = lineWhichWillLeaveLfu(&machine->l2);
                Line tmp2 = cache2[newL2pos];

                if (!canOnlyReplaceBlock(cache2[newL2pos])) { 

                    int newL3pos = lineWhichWillLeaveLfu(&machine->l3);
                    //Line tmp3 = cache3[newL3pos];   
                    if (!canOnlyReplaceBlock(cache3[newL3pos])) {

                        RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                    }
                    cache3[newL3pos] = tmp2;
                    cache3[newL3pos].cont = 0;
                }
                cache2[newL2pos] = tmp1;
                cache2[newL2pos].cont = 0;
            }
            cache3[l3pos].cont++;
            cache1[l1pos] = cache3[l3pos];
            cache1[l1pos].cont = 0;
        }
    } else { 
        /* Block only in memory RAM, need to bring it to cache and manipulate the blocks */
        //int newL1pos = lineWhichWillLeaveLfu( &machine->l1);
        int newL2pos = lineWhichWillLeaveLfu( &machine->l2); /* Need to check the position of the block that will leave the L1 */
        int newL3pos = lineWhichWillLeaveLfu( &machine->l3);
        
        if (!canOnlyReplaceBlock(cache1[l1pos])) { 
            /* The block on cache L1 cannot only be replaced, the memories must be updated */
            if (!canOnlyReplaceBlock(cache2[newL2pos])){ 
                /* The block on cache L2 cannot only be replaced, the memories must be updated */
                if (!canOnlyReplaceBlock(cache3[newL3pos])){ 
                    /* The block on cache L2 cannot only be replaced, the memories must be updated */
                    
                    RAM[cache3[newL3pos].tag] = cache3[newL3pos].block;
                }
                cache3[newL3pos] = cache2[l2pos];
                cache3[newL3pos].cont = 0;
                //RAM[cache2[newL2pos].tag] = cache2[newL2pos].block;
            }
            cache2[newL2pos] = cache1[l1pos];
            cache2[newL2pos].cont = 0;
        }
        cache1[l1pos].block = RAM[add.block];
        cache1[l1pos].tag = add.block;
        cache1[l1pos].cont = 0;
        cache1[l1pos].updated = false;
        cost = COST_ACCESS_L1 + COST_ACCESS_L2 + COST_ACCESS_L3 + COST_ACCESS_RAM;
        *whereWasHit = RAMHit;
    }
    updateMachineInfos(machine, whereWasHit, cost);
    return &(cache1[l1pos]);
}

bool canOnlyReplaceBlock(Line line) {
    // Or the block is empty or
    // the block is equal to the one in memory RAM and can be replaced
    if (line.tag == INVALID_ADD || (line.tag != INVALID_ADD && !line.updated))
        return true;
    return false;
}

bool canReplaceBlock(Cache *pCache,Line line) {
    for(int i = 0; i < pCache->size; i++){

        if((pCache->lines[i].tag != line.tag) || pCache->lines[i].tag == INVALID_ADD || (line.tag != INVALID_ADD && !line.updated)){

            return true;
        }
    }
    return false;
}

int memoryCacheMapping(int address, Cache* cache) {
    return address % cache->size;
}

int memoryCacheMappingAssociative(int address, Cache* cache) {
    
    for(int i = 0; i < cache->size; i++){//verifica todo cache até achar a liha que contem o bloco da ram

        if(address == cache->lines[i].tag ){

            return i;//retorna o endereço do cache que contem o bloco que está sendo procurado
        }
    } 
    return -1;
}

int lineWhichWillLeaveLfu( Cache* cache){
    int cont = 999999;
    int indice;
    for(int i = 0; i < cache->size; i++){//verifica todo cache procurando o que menos deu hit

        if(cache->lines[i].cont < cont ){
            //printf("\ntrue\n");
            indice = i;
        }
    }
    return indice ;//retorna o endereço do cache que menos deu hit
}

int lineWhichWillLeaveLru(Lista *pLista, Cache *pCache){

    Celula *aux = pLista->pCabeca;
    Line *sai;
    int index;

    while (aux->prox != NULL){
        if (aux->prox == NULL){
            sai = aux->prox;
        }
        aux = aux->prox;
    }
    free(aux);

    for (int i = 0; i < pCache->size; i++){
        if(pCache->lines[i].tag == sai->tag){
            index = i;
        }
    }
    return index;
}

int lineWhichWillLeave(int address, Cache* cache) {
    return address % cache->size;
}

void updateMachineInfos(Machine* machine, WhereWasHit* whereWasHit, int cost) {
    switch (*whereWasHit) {
        case L1Hit:
            machine->hitL1 += 1;
            break;

        case L2Hit:
            machine->hitL2 += 1;
            machine->missL1 += 1;
            break;
        
        case L3Hit:
            machine->hitL3 += 1;
            machine->missL2 += 1;
            machine->missL1 += 1;
            break;
        
        
        case RAMHit:
            machine->hitRAM += 1;
            machine->missL1 += 1;
            machine->missL2 += 1;
            machine->missL3 += 1;
            break;
    }
    machine->totalCost += cost;
}