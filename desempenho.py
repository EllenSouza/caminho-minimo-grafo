
# Disciplina: Computação Concorrente
# Professora: Silvana Rosseto
# Implemantação: Algoritmo em python para verificar o desempenho dos 
#                programas fw-conc.c e fw-seq.c usando a bilioteca subprocess.
#
# Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade

import subprocess     # Para rodar os executáveis
from array import *   # Para lidar com array
import sys            # Para pegar parametros na linha de comando

if (len(sys.argv) < 2):
    print(f"Entre com: {sys.argv[0]}  <número de threads>")
    exit()

# Número de threads
nthreads = int(sys.argv[1])

# Conjunto de tamanho para as matrizes
tamanhoMatrizes = [250, 500, 1000]

# Arquivos de entrada e saída
entrada   = "entrada_saida/input.txt"
saidaSeq  = "entrada_saida/output-seq.txt"
saidaConc = "entrada_saida/output-conc.txt"

# Função auxiliar
def min(a, b):
    return a if a < b else b

# Vetor com os menores tempo para cada quantidade de threads
minConc = array('f',[-1.11,-1.11,-1.11,-1.11])

for tamMatriz in tamanhoMatrizes:
    print(f"Matriz {tamMatriz}x{tamMatriz}:")
    # Para cada tamanho gerar 4 matrizes diferentes

    for i in range(5):

        # Gerando a matriz
        geraMatriz = subprocess.run(
                ['./g', entrada, str(tamMatriz)], capture_output=True, text=True)
        
        # Executar Floyd-Marshall sequencial e armazenar o tempo retornado
        seq = subprocess.run(
                    ['./fws', entrada], capture_output=True, text= True)

        # O tempo está na linha 7 do log de execução
        text = seq.stdout.splitlines()
        text = (text[6].split(': ')[1])
        
        if (i == 0):
            minSeq = float(text)
        minSeq = min(minSeq, float(text))
        
        # Cada matriz roda a quantidade de threads (1, 2, 3 e 4) por ex
        for j in range (nthreads):
           
            # Executar Floyd-Marshall concorrente e armazenar o tempo retornado
            conc = subprocess.run(
                    ['./fwc', entrada, str(j + 1)], capture_output=True, text=True)

            # O tempo está na linha 7 do log de execução
            text = conc.stdout.splitlines()
            text = (text[6].split(': ')[1])

            if (i == 0):
                minConc[j] = float(text)
            minConc[j] = min(minConc[j], float(text))
            # minConc -> array com os menores tempo para cada quantidade de threads

    print("Desempenho: (sequencial/concorrente)")    
    for k in range(nthreads):
     
        print(f"\tCom {k + 1} thread(s):",end=" ")
        desempenho = minSeq / minConc[k]
        print(f"{minSeq:,.6f} / {minConc[k]:,.6f} = {desempenho:,.6f}")

        # Quebra de linha para organizar o log
        if(k == nthreads - 1): print("")

