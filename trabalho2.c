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
#define TAM_TLB 16

FILE *fileBackingStore;

int primeiroQuadroLivre = 0;
int primeiraTabelaPagLivre = 0;
int numEntradaTLB = 0, acertoTLB = 0;
unsigned char disco[TAM];
unsigned char valor;
int memoriaFisica[TOTAL_QUADROS][TAM_QUADROS];
int tabelaPag[TAM], deslocamento, numPag, erroPag = 0, enderecoFisico;
int TLB[TAM_TLB][2]; 

void lerBackingStore(int numeroPag){

    fseek(fileBackingStore, numeroPag * TAM, SEEK_SET);
    fread(disco, sizeof(signed char), TAM, fileBackingStore);

    for(int i = 0; i < TAM; i++){
        memoriaFisica[primeiroQuadroLivre][i] = disco[i];
    }

    tabelaPag[numeroPag] = primeiroQuadroLivre;

    primeiroQuadroLivre++;
}

void inserirTLB(int numeroPag){
     int i, j; 
    for(i = 0; i < numEntradaTLB; i++){
        if(TLB[i][0] == numeroPag){
            tabelaPag[numeroPag] = TLB[i][1];  
            acertoTLB++;                
            break;
        }
    }
    
    if(i == numEntradaTLB){
        if(numEntradaTLB < TAM_TLB){  
            TLB[numEntradaTLB][0] = numeroPag;    
            TLB[numEntradaTLB][1] = tabelaPag[numeroPag];
        }
        else{                                            
            for(i = 0; i < TAM_TLB - 1; i++){
                TLB[i][0] = TLB[i + 1][0];
                TLB[i][1] = TLB[i + 1][1];
            }
            TLB[numEntradaTLB-1][0] = numeroPag;  
            TLB[numEntradaTLB-1][1] = tabelaPag[numeroPag];
        }        
    }

    if(numEntradaTLB < TAM_TLB){  
        numEntradaTLB++;
    }    
}

void lerNumPag(int endereco){
    int i;

    deslocamento = endereco & 0x000000FF;
    numPag = (endereco & 0x0000FF00) >> 8;

    if(tabelaPag[numPag] == -1){
        erroPag++;
        lerBackingStore(numPag);
    }

    inserirTLB(numPag);
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
    double taxaErros, taxaAcerto;

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
    taxaAcerto = acertoTLB / (double)numeroEnderecosLidos;

    printf("--------Estatisticas--------\n");
    printf("Numero de enderecos logicos: %d\n", numeroEnderecosLidos);
    printf("Numero de erros de pagina: %d\n",erroPag);
    printf("Taxa de erro de pagina: %.3f\n", taxaErros);
    printf("Acerto TLB: %d\n", acertoTLB);
    printf("Taxa de acerto na TLB: %.3f\n\n", taxaAcerto);
}
