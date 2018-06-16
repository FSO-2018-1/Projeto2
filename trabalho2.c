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
signed char valor;
int memoriaFisica[TOTAL_QUADROS][TAM_QUADROS];
int tabelaPag[TAM], deslocamento, numPag, erroPag = 0, enderecoFisico;

void lerBackingStore(int numeroPag){

    fseek(fileBackingStore, numeroPag * TAM, SEEK_SET);
    fread(disco, sizeof(signed char), TAM, fileBackingStore);

    for(int i = 0; i < TAM; i++){
        memoriaFisica[primeiroQuadroLivre][i] = disco[i];
    }

    tabelaPag[numeroPag] = primeiroQuadroLivre;

    primeiroQuadroLivre++;
}

void lerNumPag(int endereco){
    deslocamento = endereco & 0x000000FF;
    numPag = (endereco & 0x0000FF00) >> 8;

    if(tabelaPag[numPag] == -1){
        erroPag++;
        lerBackingStore(numPag);
    }

    enderecoFisico = (tabelaPag[numPag] * 256) + deslocamento;
    valor = memoriaFisica[tabelaPag[numPag]][deslocamento];

    printf("----------------------------------------------------------\n");
	  printf("| Processo | Quadro | Offset |  Endereco Fisico  | Valor |\n");
	  printf("----------------------------------------------------------\n");
    printf("   %d      %d        %d           %d           %c  \n",endereco, tabelaPag[numPag], deslocamento, enderecoFisico, valor);
    printf("\n\n");
}

int main(int argc, char *argv[]){
    FILE *fileAddress;
    char *endereco;
    int enderecoLogico[10000];
    int i,j, numeroEnderecosLidos;
    double taxaErros;

    endereco = argv[1];
    fileAddress = fopen(endereco, "r");
    fileBackingStore = fopen("BACKING_STORE.bin", "rb");

    for(i = 0; i < TAM; i++){
        tabelaPag[i] = -1;
    }

    for( i = 0; !feof(fileAddress); i++){
        fscanf(fileAddress, "%d", &enderecoLogico[i]);
        numeroEnderecosLidos = i + 1;
    }

    for( i = 0; i < numeroEnderecosLidos; i++){
        lerNumPag(enderecoLogico[i]);
    }

    taxaErros = erroPag / (double)numeroEnderecosLidos;

    printf("--------Estatisticas--------\n");
    printf("Numero de erros de pagina: %d\n",erroPag);
    printf("Taxa de erro de pagina: %.3f\n\n", taxaErros);

}
