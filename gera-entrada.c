/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rossetto
 * Implementação: Código para gerar matrizes de distância automaticamente
 * 		  e servirem como dado de entrada para fw-seq.c e fw-conc.c
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */

#include <stdio.h>   // printf(), fprintf(), fopen(), fclose()
#include <stdlib.h> //  malloc(), atoi()
#include <time.h>  //   rand(), srand(), time()

/* 
 * Código para gerar uma matriz de distância com valores aleatórios e
 * escrever no arquivo passado na execução
 *
 */

int main (int argc, char * argv[]){
	int tam;         // Tamanho da matriz
	FILE *arq;       // Ponteiro para o arquivo
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
					aux = rand() % 19 + 1;
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
