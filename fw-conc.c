/*
 * Disciplina: Computação Concorrente
 * Professora: Silvana Rosseto
 * Trabalho de implementação: Problema do caminho mínimo para todos os pares de vértices de um grafo.
 *                            Solução concorrente usando o algoritmo de Floyd-Warshall.
 *
 * Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade
 */


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define INFINITO 99999999  //para representar o infito na matriz
#define TAM_MAX_STRING 10  //para alocar o tamanho da string usada na leitura

//variáveis de escopo global
int * mat_dist;           // matriz distância advinda do grafo
int nthreads;             //quantidade de threads
pthread_mutex_t mutex;    //variável de exclusão mútua
pthread_cond_t  cond;     //variável de condição
int bloqueadas = 0;       //para a implementação da barreira


//estrutura de dados para passar para as threads
typedef struct {
	int n;
	int id;
}T_ARGS;

//função para printar a matriz
void print_mat(int *mat_dist, int n){
        for(int i = 0; i < n; i++){
                for(int j = 0; j < n; j++)
                        printf("%*d%c", 4, mat_dist[i * n + j], j != n - 1?'\t':'\n');
        }
}

//função da barreira
void barreira(int nthreads){
	pthread_mutex_lock(&mutex); //inicio da seção crítica
	if(bloqueadas == (nthreads-1)){
		//última thread a chegar na barreira
		pthread_cond_broadcast(&cond);
		bloqueadas=0;
	}
	else{
		bloqueadas++;
		pthread_cond_wait(&cond, &mutex);
	}
	pthread_mutex_unlock(&mutex);
}


//função que as threads irão executar
/*
 * Algoritmo de Floyd-Warshall para calcular o caminho mínimo
 * para todo par de vértices de um grafo dirigido e ponderado.
 *
 * Parâmetros (arg)
 *      inteiro com o Tamanho da matriz
 *      pthread_t com o id da thread
 */
void* fw (void * arg){
	T_ARGS * dados = (T_ARGS *) arg;
	int tam = dados->n;       //pega o tamanho da matriz
	int id = dados->id;       //pega o id da thread
	
	printf("thread %d atuando\n", id);

	for(int k = 0; k < tam; k++){
		for(int i = id; i < tam; i+=nthreads){
			for(int j = 0; j < tam; j++){
				if( mat_dist[i * tam + j] > mat_dist[i * tam + k] + mat_dist[k * tam + j] )
					mat_dist[i * tam + j] = mat_dist[i * tam + k] + mat_dist[k * tam + j];
			}

			printf("thread %d executou a linha %d no passo %d\n",id,i,k);
		}
		barreira(nthreads); //aguarda as outras threads terminarem suas linhas

	}
	pthread_exit(NULL);
}


//fluxo principal
int main(int argc, char * argv[]){
	int n;            //Tamanho da maztriz quadrada 
	FILE *arq;        //Ponteiro para arquivo
	char str[TAM_MAX_STRING];    //string auxiliar para leitura da matriz

	pthread_t *tid;   //id das threads no sistema
	T_ARGS *dados;    //estruturas das threads

	// Verificação inicial
	if(argc < 3){
		fprintf(stderr, "Digite: %s <nome do arquivo> <quantidade de threads>\n", argv[0]);
		return 1;
	}
	nthreads=atoi(argv[2]);        


	//inicializa o mutex e a variável de condição
	pthread_mutex_init (&mutex,NULL);
	pthread_cond_init (&cond, NULL);

	// Abre o arquivo
	arq = fopen(argv[1], "r");
	if(arq == NULL){fprintf(stderr, "ERRO -- fopen()\n");return 2;}

	//leitura do arquivo
	fscanf(arq, "%d", &n);         // Tamanho da matriz
	
	// Aloca memória para a matriz distância
	mat_dist = malloc(sizeof(int) * n * n);
	if(mat_dist == NULL){fprintf(stderr, "ERRO -- malloc()\n"); return 3;}
	
	//leitura da matriz
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			fscanf(arq, "%s", str);
			mat_dist[i * n + j] = strcmp(str, "N") == 0? INFINITO : atoi(str);
		}
	}

	//alocação das estruturas
	tid= (pthread_t *) malloc(sizeof(pthread_t)* nthreads);
	if(tid==NULL){puts("ERRO--malloc()");	return 2;}
	
	dados= (T_ARGS *)malloc (sizeof(T_ARGS) * nthreads);
	if(dados==NULL){puts("ERRO--malloc()");	return 2;}

	//cria as threads e chama o algoritmo de Floyd Marshall
	for(int i=0;i<nthreads;i++){
		(dados+i)->id=i;
		(dados+i)->n=n;
		
		if(pthread_create((tid+i), NULL, fw,(void *)(dados+i))){
		       puts("ERRO-- pthread_create()\n");	return 3;
		}
	}		

	// Es
	for(int i=0;i<nthreads;i++){
		if(pthread_join(*(tid+i), NULL)){
		       puts("ERRO-- pthread_join()\n");	return 4;
		}
	}	

	// Printa resultado
	print_mat(mat_dist, n);

	// Fecha o arquivo
	fclose(arq);

	// Libera memórias alocadas
	free(mat_dist);
	free(tid);
	free(dados);

	//destrói o mutex e a variável de condição 
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&cond);

	return 0;

}