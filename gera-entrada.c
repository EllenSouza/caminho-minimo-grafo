#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/*
 * Código para gerar uma matriz de distância com valores aleatórios e
 * escrever no arquivo passado na execução
 *
 */

int main (int argc, char * argv[]){
	int tam;         //tamanho da matriz
	FILE *arq;       //ponteiro para o arquivo
	int aux;

	//verificação inicial
	if (argc < 3){
		printf("Insira: %s <nome do arquivo> <tamanho da matriz>\n",argv[0]);
		return 1;
	}
	tam=atoi(argv[2]);

	//abre o arquivo
	arq=fopen (argv[1], "w");
	if(arq == NULL) {fprintf(stderr, "ERRO -- fopen()\n");return 2;}

	srand(time(NULL));
	//escrita no arquivo
	for(int i=0; i<tam; i++){
		//escreve o tamanho da matriz
		if( i==0)	fprintf(arq, "%d\n", tam);

		for(int j=0; j<tam; j++){
			aux=rand() % 2; //se 0 imprime infinito, senão sorteia um int

			if(j==i) fprintf(arq, "0 ");

			else {
				
				if(aux==0)	fprintf(arq, "N ");
				else {
					aux = rand() %101 ;
					fprintf(arq, "%d ",aux);
				}
			}
		}
		fprintf(arq, "\n");

	}
	
	//fecha o arquivo
	fclose(arq);
	

	return 0;
}
