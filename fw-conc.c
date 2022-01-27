/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Trabalho de implementação: Problema do caminho mínimo para todos os pares de vértices de um grafo.
 *                            Solução concorrente usando o algoritmo de Floyd-Warshall.
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */


#include <stdio.h>     // printf(), fprintf(), fscanf(), fopen(), fclose()
#include <stdlib.h>   //  malloc(), free(), atoi()
#include <pthread.h> //   wait(), broadcast(), create(), join(), init(), destroy(), lock(), unlock()
#include <string.h> //    strcmp()
#include "timer.h" //	  GET_TIME()

#define INFINITO 99999999   // Para representar o infito na matriz
#define TAM_MAX_STRING 10  //  Para alocar o tamanho da string usada na leitura

// Variáveis de escopo global
int *mat_dist;           // Matriz distância advinda do grafo
int nthreads;           //  Quantidade de threads

// Variáveis de sincronização
pthread_mutex_t mutex;    // Variável de exclusão mútua
pthread_cond_t  cond;    //  Variável de condição
int bloqueadas = 0;     //   Para a implementação da barreira


// Estrutura de dados para passar para as threads
typedef struct {
	int n;
	int id;
}T_ARGS;

/*
 * Função que implementa a sincronização coletiva
 * usando barreiras.
 */
void barreira(int nthreads){
	pthread_mutex_lock(&mutex); //inicio da seção crítica
	if(bloqueadas == (nthreads - 1)){
		// Última thread a chegar na barreira
		pthread_cond_broadcast(&cond);
		bloqueadas = 0;
	}
	else{
		bloqueadas++;
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);
}


/*
 * Função que as threads irão executar.
 * Algoritmo de Floyd-Warshall para calcular o caminho mínimo
 * para todo par de vértices de um grafo dirigido e ponderado.
 *
 * Parâmetros (arg)
 *      inteiro com o Tamanho da matriz
 *      pthread_t com o id da thread
 */
void* fw (void * arg){
	int tam = ((T_ARGS *) arg)->n;       // Tamanho da matriz
	int id  = ((T_ARGS *) arg)->id;     //  Id da thread
	
	for(int k = 0; k < tam; k++){
		for(int i = id; i < tam; i += nthreads){
			if(i == k) continue;
			for(int j = 0; j < tam; j++){
				if(j == k || i == j) continue;
				if( mat_dist[i * tam + j] > mat_dist[i * tam + k] + mat_dist[k * tam + j] )
					mat_dist[i * tam + j] = mat_dist[i * tam + k] + mat_dist[k * tam + j];
			}
		}
		barreira(nthreads); // Aguarda as outras threads terminarem suas linhas
	}
	pthread_exit(NULL);
}


// Fluxo principal
int main(int argc, char * argv[]){
	int n;            	    // Tamanho da maztriz quadrada 
	FILE *arq;		   //  Ponteiro para arquivo
	char str[TAM_MAX_STRING]; //   String auxiliar para leitura da matriz
	double ini, fim;	 //    Tomada de tempo

	pthread_t *tid;   // Id das threads no sistema
	T_ARGS *dados;   //  Estruturas das threads


	// Verificação inicial
	if(argc < 3){
		fprintf(stderr, "Digite: %s <nome do arquivo> <quantidade de threads>\n", argv[0]);
		return 1;
	}
	nthreads = atoi(argv[2]);

	// Inicializa as variáveis de sincronização
	pthread_mutex_init (&mutex,NULL);
	pthread_cond_init (&cond, NULL);

	// Abre o arquivo
	arq = fopen(argv[1], "r");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n"); return 2;}

	//leitura do arquivo
	fscanf(arq, "%d", &n);         // Tamanho da matriz
	
	// Aloca memória para a matriz distância
	mat_dist = malloc(sizeof(int) * n * n);
	if(mat_dist == NULL){fprintf(stderr, "ERRO -- malloc()\n"); return 3;}
	
	printf("Lendo o arquivo...\n");

	// Leitura da matriz
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fscanf(arq, "%s", str);
			mat_dist[i * n + j] = strcmp(str, "N") == 0? INFINITO : atoi(str);
		}
	}

	printf("Leitura concluída.\n\n");

	// Alocação de memória para as estruturas
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid == NULL){fprintf(stderr, "ERRO--malloc()");	return 2;}
	
	dados = (T_ARGS *) malloc(sizeof(T_ARGS) * nthreads);
	if(dados == NULL){fprintf(stderr, "ERRO--malloc()"); return 2;}

	printf("Calculando Floyd-Warshal...\n");

	GET_TIME(ini);
	// Cria as threads e chama o algoritmo de Floyd Marshall
	for(int i = 0; i < nthreads; i++){
		(dados + i)->id = i;
		(dados + i)->n = n;
		
		if(pthread_create((tid + i), NULL, fw, (void *)(dados + i))){
		       fprintf(stderr, "ERRO-- pthread_create()\n");	return 3;
		}
	}	

	// Espera pelo término das threads
	for(int i = 0; i < nthreads; i++){
		if(pthread_join(*(tid + i), NULL)){
		       fprintf(stderr, "ERRO-- pthread_join()\n");	return 4;
		}
	}	
	
	GET_TIME(fim);
	printf("Cálculo concluído.\n\n");

	printf("Tempo concorrente: %lf\n\n",fim - ini);

	// Fecha o arquivo
	fclose(arq);

	// Imprime resultado em output-conc.txt
	arq = fopen("entrada_saida/output-conc.txt", "w+");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n"); return 2;}

	// Escreve resultado no arquivo
	printf("Escrevendo resultado em output-conc.txt...\n");
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++)
			fprintf(arq,"%d%c", mat_dist[i * n + j], j == n - 1?'\n':' ');
	}
	printf("Escrita concluída.\n\n");

	printf("Fim da execução!\n");

	// Fecha o arquivo
	fclose(arq);

	// Libera memórias alocadas
	free(mat_dist); free(tid); free(dados);

	// Libera as variáveis de sincronização 
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
