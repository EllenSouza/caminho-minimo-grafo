<!-- Título e subtítulo -->
<h1 align="center">
    Caminhos mínimos de um grafo<br>
    Algoritmo de Floyd-Warshall
</h1>

<!-- Breve introdução -->
<h1><br>Descrição do projeto</h1>
<p>A aplicação em questão é um trabalho da faculdade (UFRJ) para a disciplina de Computação Concorrente (MAB-117) - 2021/2 e que foi desenvolvida para ambiente Linux. </p>
<p>O objetivo do projeto é mostrar os benefícios da programação concorrente resolução do problema de encontrar todos os todos os caminhos mínimo para cada par de vértices de um grafo dirigido ponderado.</p>
<p>Utilizamos o algoritmo de Floyd-Warshall para realizar as comparações entre um algoritmo sequencial e um concorrente. Toda a aplicação foi desenvolvida em linguagem C utilizando a biblioteca Pthread.</p>
<p>Para mais detalhes sobre o projeto <a href="">clique aqui</a>.</p>

<!-- Acesso ao projeto -->
<h1><br>Como executar o projeto</h1>
<p>Antes de começar, é necessário que tenha o compilador GCC instalado na sua máquina. Caso não tenha, o baixe utilizando o comando abaixo:</p>

<h2>Pré-requisitos</h2>

```bash
   $ sudo apt install gcc
```

<h2>Rodar a aplicação</h2>
<p>Clone o projeto na sua máquina:</p>

```bash
    $ git clone https://github.com/EllenSouza/caminho-minimo-grafo
```

<p>Como C é uma linguagem complidada, primeiro compile os arquivos sequencial e concorrente.</p><br>

```bash
    # Sequencial

    $ gcc -o fws fw-seq.c -Wall

    # Concorrente

    $ gcc -o fwc fw-conc.c -Wall -lpthread
```

<p>Agora execute-os passando os parâmetros solicitados.</p>


```bash
    # Sequencial

    $ ./fws entrada_saida/input.txt

    # Concorrente

    $ ./fwc entrada_saida/input.txt <números de threads>
```
<!-- Autores -->
<br><h1>Autores</h1>
<div id = "autores">
    <div>
        <a href="https://github.com/EllenSouza">
            <img src="https://github.com/EllenSouza.png">
            <br><sub>Ellen Almeida</sub>
        </a>   
    </div>
    <div>
        <a href="https://github.com/kevinsena01">
            <img src="https://github.com/kevinsena01.png">
            <br><sub>Kevin Sena</sub>
        </a>
    </div>
    
</div>

<style>
    p{
        text-align: justify
    }
    img {
        width: 50%;
        border-radius: 8rem;
    }
    sub{        
        font-size: 1.5rem;
    }
    #autores{
        display: flex;
    }
</style>