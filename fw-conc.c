/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Trabalho de implementação: Problema do caminho mínimo para todos os pares de vértices de um grafo.
 *                            Solução concorrente usando o algoritmo de Floyd-Warshall.
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */


#include <stdio.h>	/* printf() */
#include <stdlib.h>	/* malloc() */
#include <pthread.h>	/* wait(), signal(), pthread_init, pthread_destroy, pthread_t, mutex_t, cond_t*/
#include <string.h>	/* strcmp(), */
#include <time.h>	/* clock_t, CLOCKS_PER_SEC*/

#define INFINITO 99999999  //para representar o infito na matriz
#define TAM_MAX_STRING 10  //para alocar o tamanho da string usada na leitura

// Variáveis de escopo global
int * mat_dist;           // matriz distância advinda do grafo
int nthreads;             //quantidade de threads

// Variáveis de sincronização
pthread_mutex_t mutex;    //variável de exclusão mútua
pthread_cond_t  cond;     //variável de condição
int bloqueadas = 0;       //para a implementação da barreira


// Estrutura de dados para passar para as threads
typedef struct {
	int n;
	int id;
}T_ARGS;

// Função para printar a matriz
void print_mat(int *mat_dist, int n){
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++)
                        printf("%*d%c", 4, mat_dist[i * n + j], j != n - 1?'\t':'\n');
        }
}

// Função da barreira
void barreira(int nthreads){
	pthread_mutex_lock(&mutex); //inicio da seção crítica
	if(bloqueadas == (nthreads-1)){
		//última thread a chegar na barreira
		pthread_cond_broadcast(&cond);
		bloqueadas = 0;
	}
	else{
		bloqueadas++;
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);
}


// Função que as threads irão executar
/*
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
		barreira(nthreads); //aguarda as outras threads terminarem suas linhas
	}
	pthread_exit(NULL);
}


// Fluxo principal
int main(int argc, char * argv[]){
	int n;            	    // Tamanho da maztriz quadrada 
	FILE *arq;		   //  Ponteiro para arquivo
	char str[TAM_MAX_STRING]; //   String auxiliar para leitura da matriz
	clock_t t;	   	 //    Tomada de tempo
	
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
	if(tid == NULL){puts("ERRO--malloc()");	return 2;}
	
	dados = (T_ARGS *) malloc(sizeof(T_ARGS) * nthreads);
	if(dados == NULL){puts("ERRO--malloc()"); return 2;}

	printf("Calculando Floyd-Warshal...\n");

	t = clock();
	// Cria as threads e chama o algoritmo de Floyd Marshall
	for(int i = 0; i < nthreads; i++){
		(dados + i)->id = i;
		(dados + i)->n = n;
		
		if(pthread_create((tid + i), NULL, fw, (void *)(dados + i))){
		       puts("ERRO-- pthread_create()\n");	return 3;
		}
	}		

	// Espera pelo término das threads
	for(int i = 0; i < nthreads; i++){
		if(pthread_join(*(tid + i), NULL)){
		       puts("ERRO-- pthread_join()\n");	return 4;
		}
	}	
	
	printf("Cálculo concluído.\n\n");

	t = clock() - t;

	// Printa resultado
	//print_mat(mat_dist, n);
	printf("Tempo concorrente: %f\n", ((float)t)/CLOCKS_PER_SEC);

	// Fecha o arquivo
	fclose(arq);

	// Libera memórias alocadas
	free(mat_dist); free(tid); free(dados);

	// Libera as variáveis de sincronização 
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;
}
