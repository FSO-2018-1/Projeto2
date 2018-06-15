/*
Trabalho 2 FSO
Maria Luiza Ferreira - 16/0014433
Lucas Arthur Lermen - 16/0012961
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <alloca.h>

#define TAM 256
#define TOTAL_QUADROS 256
#define TAM_QUADROS 256

FILE *fileBackingStore;

int primeiroQuadroLivre = 0;
int primeiraTabelaPagLivre = 0;
signed char disco[TAM];
int memoriaFisica[TOTAL_QUADROS][TAM_QUADROS];
int tabelaPag[TAM], deslocamento, numPag;

void lerBackingStore(int numeroPag){

    fseek(fileBackingStore, numeroPag * TAM, SEEK_SET);
    fread(disco, sizeof(signed char), TAM, fileBackingStore);

    for(int i = 0; i < TAM; i++){
        memoriaFisica[primeiroQuadroLivre][i] = disco[i];
    }
    
    tabelaPag[numeroPag] = primeiroQuadroLivre;

    printf("%d - TABELA DE PAGINAS - %d QUADRO - %d\n",i, numeroPag,tabelaPag[numeroPag]);

    primeiroQuadroLivre++;
}

void lerNumPag(int endereco){
    // deslocamento = endereco & 0x000000FF;
    numPag = (endereco & 0x0000FF00) >> 8;

    if(tabelaPag[numPag] == -1){
        lerBackingStore(numPag);
    }
}




int main(int argc, char *argv[]){
    FILE *fileAddress;
    char *endereco;
    int enderecoLogico[10000];
    int i,j;

    endereco = argv[1];
    fileAddress = fopen(endereco, "r");
    fileBackingStore = fopen("BACKING_STORE.bin", "rb");

    for(i = 0; i < TAM; i++){
        tabelaPag[i] = -1;
    }

    for( i = 0; !feof(fileAddress); i++){
        fscanf(fileAddress, "%d", &enderecoLogico[i]);
    }

    for( i = 0; i < 10000; i++){
        lerNumPag(enderecoLogico[i]);
    }  



}

