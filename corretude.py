# Disciplina: Computação Concorrete
# Professora: Silvana Rosseto
# Implemantação: Algoritmo em python para verificar a corretude dos 
#                programas fw-conc.c e fw-seq.c usando a bilioteca subprocess.
#
# Desenvolvido por: Ellen Almeida de Souza e Kevin Sena de Andrade

import subprocess

# Conjunto de tamanho para as matrizes entrada
tamanhoMatrizes = [250, 500, 1000]

# Arquivos de entrada e saída
entrada   = "entrada_saida/input.txt"
saidaSeq  = "entrada_saida/output-seq.txt"
saidaConc = "entrada_saida/output-conc.txt"

for tamMatriz in tamanhoMatrizes:

    # Para cada tamanho gera 4 matrizes diferentes
    for i in range(4):
        
        # Gerando a matriz
        geraMatriz = subprocess.run(['./g', entrada, str(tamMatriz)], capture_output=True, text=True)
        
        # Para cada matriz roda 3 vezes para verificar corretude
        for j in range(3):

            # Executar Floyd-Warshall sequencial
            seq =  subprocess.run(['./fws', entrada], capture_output=True, text=True)

            # Executar Floyd-Warshall concorrente
            conc = subprocess.run(['./fwc', entrada, '2'], capture_output=True, text=True)

            # Tirar a diferença e verifica se os arquivos são iguais
            diff = subprocess.run(['diff', saidaSeq, saidaConc], capture_output=True, text=True)
            if(diff.stdout != ""):
                print("Arquivos diferentes!")
                exit()

print("Resultados estão corretos")
