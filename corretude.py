import subprocess

# O objetivo desse programa é calcular a corretude e o desempenho
# dos programas fw-conc.c e fw-seq.c. 

tamanhoMatrizes = [250, 500, 1000] # Conjunto de tamanho para as matrizes distância

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

            # Verifica se os arquivos são iguais
            diff = subprocess.run(['diff', saidaSeq, saidaConc], capture_output=True, text=True)
            if(diff.stdout != ""):
                print("Arquivos diferentes!")
                exit()



print("Resultados estão corretos")
