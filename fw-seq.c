/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Trabalho de implementação: Problema do caminho mínimo para todos os pares de vértices de um grafo.
 * 			      Solução sequencial usando o algoritmo de Floyd-Warshall.
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INFINITO 99999999
#define TAM_MAX_STRING 10

void print_mat(int *mat_dist, int n){
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)
			printf("%*d%c", 4, mat_dist[i * n + j], j != n - 1?'\t':'\n');
	}
}

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


	// Verificação inicial
	if(argc < 2){
		fprintf(stderr, "Digite: %s <nome do arquivo>\n", argv[0]);
		return 1;
	}
	
	// Abre o arquivo
	arq = fopen(argv[1], "r");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n");return 2;}

	// Leitura da matriz de distância	
	fscanf(arq, "%d", &n); // Tamanho da matriz
	
	// Aloca memória para a matriz distância
	mat_dist = malloc(sizeof(int) * n * n);
	if(mat_dist == NULL){fprintf(stderr, "ERRO -- malloc()\n"); return 3;}
	
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fscanf(arq, "%s", str);
			mat_dist[i * n + j] = strcmp(str, "N") == 0? INFINITO : atoi(str);
		}
	}
	
	// Algoritmo de Floyd-Warshall
	fw(mat_dist, n);
	
	// Printa resultado
	print_mat(mat_dist, n);
	
	// Fecha o arquivo
	fclose(arq);

	// Libera memórias alocadas
	free(mat_dist);

	return 0;
}

