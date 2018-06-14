/*
Trabalho 2 FSO
Maria Luiza Ferreira - 16/0014433
Lucas Arthur Lermen - 16/0012961
*/

#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <time.h>
#include <unistd.h>
#include <locale.h>
#include <string.h>

int enderecoLogico[10000];

#define swap( a, b )   do{ int tmp = a; a = b; b = tmp; }while(0)

const char * dec2bin( char * bin, int d ){
    int i = 0;
    int j = 0;

    for( i = 0; d > 0; d /= 2, i++ )
        bin[i] = (d % 2) ? '1' : '0';

    for( j = 0; j < (i / 2); j++ )
        swap( bin[j], bin[ i - j - 1 ] );

    bin[i] = '\0';

    return bin;
}

void leia_nome(char *nome){
      int i=0,c=getchar();
      setbuf(stdin,NULL);

      if(c == '\n'){
          c = getchar();
      }
      do{
        nome[i]=c;
        i++;
        c=getchar();
      }while(c!='\n');
      nome[i]='\0';
}

int main(int argc, char *argv[]){
    FILE *file;
    char *endereco;
    char bin[16];
    char matriz[10000][16], matrizTeste[10000][16];
    int i,j;

    endereco = argv[1];
    file = fopen(endereco, "r");
    
    for( i = 0; !feof(file); i++){
        fscanf(file, "%d", &enderecoLogico[i]);
    }

    for( i = 0;i < 10000; i++){
            strcpy(matriz[i], dec2bin(bin,enderecoLogico[i]));
            strcpy(matrizTeste[i], matriz[i]);
            // printf("%d - %s\n",i, matriz[i]);
    }
    
    printf("1 - %s", matriz[9970]>>16);

    for( int i = 0; i < 10000;i++ ){
        // printf("%d - BINARIO %s\n",i, matrizTeste[i]);
        // printf("INTEIRO %d\n", enderecoLogico[i]);
    }

    // for(i = 0; i < 10000; i++){
    //     for(j = 0; j < 16; j++){
    //         printf("%s ",matriz[1][j]);
    //     }
    // }

    // printf("BINARIO %s\n INTEIRO %d", dec2bin(bin,enderecoLogico[0]), enderecoLogico[0]);


}

