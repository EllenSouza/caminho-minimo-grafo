/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rossetto
 * Trabalho de implementação: Problema do caminho mínimo para todos os pares de vértices de um grafo.
 * 			      Solução sequencial usando o algoritmo de Floyd-Warshall.
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */

#include <stdio.h>       // printf(), fprintf(), fscanf(), fopen(), fclose()
#include <stdlib.h>     //  malloc(), free(), atoi()
#include <string.h>    //   strcmp()
#include <pthread.h>  //    (Só para o timer.h funcionar)
#include "timer.h"   //	    GET_TIME()

#define INFINITO 99999999  // Para representar o infito na matriz
#define TAM_MAX_STRING 10 //  Para alocar o tamanho da string usada na leiturai

/*
 * Algoritmo de Floyd-Warshall para calcular o caminho mínimo
 * para todo par de vértices de um grafo dirigido e ponderado.
 *
 * Parâmetros
 * 	int *mat: Matriz distância inicial
 * 	int n: Tamanho da matriz
 */
void fw(int *mat, int n){
	for(int k = 0; k < n; k++){
		for(int i = 0; i < n; i++){
			if(i == k) continue;
			for(int j = 0; j < n; j++){
				if(j == k || i == j) continue;
				if( mat[i * n + j] > mat[i * n + k] + mat[k * n + j] )
					mat[i * n + j] = mat[i * n + k] + mat[k * n + j];
			}
		}
	}
}

int main(int argc, char* argv[]){
	int n;			     // Tamanho da matriz quadrada e um contador
	int *mat_dist;		    //  Matriz distância advinda do grafo
	FILE *arq; 		   //   Ponteiro para arquivo
	char str[TAM_MAX_STRING]; //    String auxiliar para leitura da matriz
	double ini, fim;	 //	Tomada de tempo

	// Verificação inicial
	if(argc < 2){
		fprintf(stderr, "Digite: %s <nome do arquivo>\n", argv[0]);
		return 1;
	}
	
	// Abre o arquivo
	arq = fopen(argv[1], "r");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n");return 2;}

	fscanf(arq, "%d", &n); // Tamanho da matriz
	
	// Aloca memória para a matriz distância
	mat_dist = malloc(sizeof(int) * n * n);
	if(mat_dist == NULL){fprintf(stderr, "ERRO -- malloc()\n"); return 3;}

	printf("Lendo o arquvio...\n");

	// Leitura da matriz de distância
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fscanf(arq, "%s", str);
			mat_dist[i * n + j] = strcmp(str, "N") == 0? INFINITO : atoi(str);
		}
	}
	
	// Fecha o arquivo
	fclose(arq);

	printf("Leitura concluída.\n\n");

	printf("Calculando Floyd-Warshal...\n");

	GET_TIME(ini);
	fw(mat_dist, n); // Algoritmo de Floyd-Warshall
	GET_TIME(fim);
	
	printf("Cálculo concluído.\n\n");

	printf("Tempo sequencial: %lf\n\n", fim - ini);	
	
	// Imprime resultado em output-seq.txt
	arq = fopen("entrada_saida/output-seq.txt", "w+");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n"); return 2;}
	
	// Escreve resultado no arquivo
	printf("Escrevendo resultado em output-seq.txt...\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)
			fprintf(arq,"%d%c", mat_dist[i * n + j], j == n - 1?'\n':' ');
	}
	printf("Escrita concluída.\n\n");
	printf("Fim da execução!\n");
	
	// Fecha o arquivo
	fclose(arq);
	
	// Libera memórias alocadas
	free(mat_dist);

	return 0;
}

