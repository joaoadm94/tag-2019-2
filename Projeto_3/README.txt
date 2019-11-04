Teoria e aplicação de grafos - Turma A - 2019/2
João Antonio Desiderio de Moraes (16/0126975)
Hiago dos Santos Rabelo (16/0124492)

Esta é a entrega do terceiro projeto de Teoria e Aplicação de Grafos 2019.2. 

AVISO
    O programa contido no arquivo "tag3.c" foi desenvolvido em Linux (Mint 19.1), compilado e testado com o gcc mais atual. O código utiliza bibliotecas padrão.

DEFINIÇÃO DE PRIORIDADES
    Os professores já possuem definidas as prioridades das escolas que desejam trabalhar
    As escolas podem receber tanto algum professor com x de habilitação ou mais, porém
    se a habilitação for maior do que a que ele precisa não será prioridade 1, uma vez
    que quanto maior a habilitação mais difícil é de arrumar esse professor. Logo têm 
    escolas que precisam mais deste professor.
    Conta:
        se habilitação professor >= habilitação pedida:
            prioridade = habilitacaoProfessor - habilitacaoPedida + 1

COMO COMPILAR E EXECUTAR
    Não é necessário incluir nenhum parâmetro especial durante a compilação e execução do programa. Os comandos a seguir foram suficientes em uma máquina Linux com o gcc instalado.
    
    gcc -o tag3 tag3.c
    ./tag3

SOBRE O PROGRAMA
    O arquivo "tag1.c" contém o código de um programa capaz de abrir o arquivo "soc-dolphins.mtx" e montar um grafo a partir de seu conteúdo. O programa pode ser adaptado para ter maior versatilidade na abertura de arquivos, mas foi limitado a este arquivo para garantir uma execução sem imprevistos.

O grafo é formado a partir de uma lista ligada de vértices, e cada nó dessa lista guarda o endereço para uma lista de arestas que informa os vértices adjacentes. O uso de listas de adjacências é uma exigência do projeto e foi cumprida.

O programa possui a função "imprimirVertices" que percorre a lista de vértices. Em cada vértice, percorre a lista de arestas para determinar o grau do vértice. Todas essas informações são impressas na tela.

As funções "imprimirCliques" e "bronKerbosch" são responsáveis por imprimir na tela todos os cliques maximais do grafo. O algoritmo Bron-Kerbosch é iniciado com as listas R e X vazias e a lista P contendo todos os vértices do grafo. Não foi implementada uma estratégia para escolha dos itens da lista P, portanto, são selecionados na ordem que estiverem.

As funções "coeficienteAglomeracaoGrafo" e "coeficienteAglomeracaoVertice" trabalham em conjunto para encontrar esses coeficientes e imprimi-los na tela.

RESULTADO DA EXECUÇÃO ESPERADA
Incluo aqui um log da execução esperada para conferência.

------ Teoria e Aplicação de Grafos - Projeto 1. ------
---- João Antonio Desiderio de Moraes (16/0126975) ----
---- Hiago dos Santos Rabelo (16/0124492) ----

Questão 1: imprimir os vértices e seus respectivos graus

Vértice 1 - Grau 6
Vértice 2 - Grau 8
Vértice 3 - Grau 4
Vértice 4 - Grau 3
Vértice 5 - Grau 1
Vértice 6 - Grau 4
Vértice 7 - Grau 6
Vértice 8 - Grau 5
Vértice 9 - Grau 6
Vértice 10 - Grau 7
Vértice 11 - Grau 5
Vértice 12 - Grau 1
Vértice 13 - Grau 1
Vértice 14 - Grau 8
Vértice 15 - Grau 12
Vértice 16 - Grau 7
Vértice 17 - Grau 6
Vértice 18 - Grau 9
Vértice 19 - Grau 7
Vértice 20 - Grau 4
Vértice 21 - Grau 9
Vértice 22 - Grau 6
Vértice 23 - Grau 1
Vértice 24 - Grau 3
Vértice 25 - Grau 6
Vértice 26 - Grau 3
Vértice 27 - Grau 3
Vértice 28 - Grau 5
Vértice 29 - Grau 5
Vértice 30 - Grau 9
Vértice 31 - Grau 5
Vértice 32 - Grau 1
Vértice 33 - Grau 3
Vértice 34 - Grau 10
Vértice 35 - Grau 5
Vértice 36 - Grau 1
Vértice 37 - Grau 7
Vértice 38 - Grau 11
Vértice 39 - Grau 8
Vértice 40 - Grau 2
Vértice 41 - Grau 8
Vértice 42 - Grau 5
Vértice 43 - Grau 6
Vértice 44 - Grau 7
Vértice 45 - Grau 4
Vértice 46 - Grau 11
Vértice 47 - Grau 2
Vértice 48 - Grau 6
Vértice 49 - Grau 1
Vértice 50 - Grau 2
Vértice 51 - Grau 7
Vértice 52 - Grau 10
Vértice 53 - Grau 4
Vértice 54 - Grau 2
Vértice 55 - Grau 7
Vértice 56 - Grau 2
Vértice 57 - Grau 2
Vértice 58 - Grau 9
Vértice 59 - Grau 1
Vértice 60 - Grau 5
Vértice 61 - Grau 1
Vértice 62 - Grau 3

Questão 2: imprimir todos os cliques maximais

Clique maximal: { 1 11 43 48 }
Clique maximal: { 1 15 41 }
Clique maximal: { 1 16 41 }
Clique maximal: { 2 18 28 }
Clique maximal: { 2 20 55 }
Clique maximal: { 2 27 28 }
Clique maximal: { 2 29 }
Clique maximal: { 2 37 }
Clique maximal: { 2 42 55 }
Clique maximal: { 3 11 43 }
Clique maximal: { 3 45 }
Clique maximal: { 3 62 }
Clique maximal: { 4 9 60 }
Clique maximal: { 4 15 }
Clique maximal: { 5 52 }
Clique maximal: { 6 10 14 58 }
Clique maximal: { 6 57 }
Clique maximal: { 7 10 14 18 58 }
Clique maximal: { 7 14 55 58 }
Clique maximal: { 7 57 }
Clique maximal: { 8 20 31 }
Clique maximal: { 8 20 55 }
Clique maximal: { 8 28 }
Clique maximal: { 8 41 }
Clique maximal: { 9 21 29 }
Clique maximal: { 9 38 46 }
Clique maximal: { 9 46 60 }
Clique maximal: { 10 14 33 }
Clique maximal: { 10 14 42 58 }
Clique maximal: { 11 30 }
Clique maximal: { 12 52 }
Clique maximal: { 13 34 }
Clique maximal: { 14 42 55 58 }
Clique maximal: { 15 17 34 38 }
Clique maximal: { 15 17 34 39 }
Clique maximal: { 15 17 34 51 }
Clique maximal: { 15 25 }
Clique maximal: { 15 34 35 38 }
Clique maximal: { 15 34 38 41 }
Clique maximal: { 15 34 38 44 }
Clique maximal: { 15 34 39 44 }
Clique maximal: { 15 39 53 }
Clique maximal: { 15 41 53 }
Clique maximal: { 16 19 25 46 }
Clique maximal: { 16 46 60 }
Clique maximal: { 16 56 }
Clique maximal: { 17 21 39 }
Clique maximal: { 17 21 51 }
Clique maximal: { 18 23 }
Clique maximal: { 18 26 28 }
Clique maximal: { 18 32 }
Clique maximal: { 19 21 }
Clique maximal: { 19 22 30 46 52 }
Clique maximal: { 19 25 30 46 52 }
Clique maximal: { 21 29 48 }
Clique maximal: { 21 37 }
Clique maximal: { 21 39 45 }
Clique maximal: { 22 34 38 }
Clique maximal: { 22 38 46 }
Clique maximal: { 24 37 }
Clique maximal: { 24 46 52 }
Clique maximal: { 26 27 28 }
Clique maximal: { 29 31 48 }
Clique maximal: { 30 36 }
Clique maximal: { 30 44 }
Clique maximal: { 30 53 }
Clique maximal: { 31 43 48 }
Clique maximal: { 33 61 }
Clique maximal: { 35 45 }
Clique maximal: { 35 50 }
Clique maximal: { 37 38 41 }
Clique maximal: { 37 40 }
Clique maximal: { 37 60 }
Clique maximal: { 38 62 }
Clique maximal: { 39 59 }
Clique maximal: { 40 58 }
Clique maximal: { 43 51 }
Clique maximal: { 44 47 }
Clique maximal: { 44 54 }
Clique maximal: { 46 51 52 }
Clique maximal: { 47 50 }
Clique maximal: { 49 58 }
Clique maximal: { 52 56 }
Clique maximal: { 54 62 }

Questão 3: imprimir o coeficiente de aglomeração de cada vértice

Vértice 1 - Coeficiente 0.33
Vértice 2 - Coeficiente 0.14
Vértice 3 - Coeficiente 0.17
Vértice 4 - Coeficiente 0.33
Vértice 5 - Coeficiente 0.00
Vértice 6 - Coeficiente 0.50
Vértice 7 - Coeficiente 0.53
Vértice 8 - Coeficiente 0.20
Vértice 9 - Coeficiente 0.27
Vértice 10 - Coeficiente 0.52
Vértice 11 - Coeficiente 0.40
Vértice 12 - Coeficiente 0.00
Vértice 13 - Coeficiente 0.00
Vértice 14 - Coeficiente 0.50
Vértice 15 - Coeficiente 0.26
Vértice 16 - Coeficiente 0.24
Vértice 17 - Coeficiente 0.60
Vértice 18 - Coeficiente 0.22
Vértice 19 - Coeficiente 0.52
Vértice 20 - Coeficiente 0.50
Vértice 21 - Coeficiente 0.14
Vértice 22 - Coeficiente 0.53
Vértice 23 - Coeficiente 0.00
Vértice 24 - Coeficiente 0.33
Vértice 25 - Coeficiente 0.53
Vértice 26 - Coeficiente 0.67
Vértice 27 - Coeficiente 0.67
Vértice 28 - Coeficiente 0.40
Vértice 29 - Coeficiente 0.30
Vértice 30 - Coeficiente 0.25
Vértice 31 - Coeficiente 0.30
Vértice 32 - Coeficiente 0.00
Vértice 33 - Coeficiente 0.33
Vértice 34 - Coeficiente 0.33
Vértice 35 - Coeficiente 0.30
Vértice 36 - Coeficiente 0.00
Vértice 37 - Coeficiente 0.05
Vértice 38 - Coeficiente 0.24
Vértice 39 - Coeficiente 0.29
Vértice 40 - Coeficiente 0.00
Vértice 41 - Coeficiente 0.25
Vértice 42 - Coeficiente 0.60
Vértice 43 - Coeficiente 0.33
Vértice 44 - Coeficiente 0.24
Vértice 45 - Coeficiente 0.17
Vértice 46 - Coeficiente 0.31
Vértice 47 - Coeficiente 0.00
Vértice 48 - Coeficiente 0.40
Vértice 49 - Coeficiente 0.00
Vértice 50 - Coeficiente 0.00
Vértice 51 - Coeficiente 0.24
Vértice 52 - Coeficiente 0.24
Vértice 53 - Coeficiente 0.33
Vértice 54 - Coeficiente 0.00
Vértice 55 - Coeficiente 0.38
Vértice 56 - Coeficiente 0.00
Vértice 57 - Coeficiente 0.00
Vértice 58 - Coeficiente 0.36
Vértice 59 - Coeficiente 0.00
Vértice 60 - Coeficiente 0.30
Vértice 61 - Coeficiente 0.00
Vértice 62 - Coeficiente 0.00

Questão 4: imprimir o coeficiente de aglomeração do grafo.

O coeficiente de aglomeração do grafo é 0.259

Liberando a memória alocada para o grafo...

Encerrando...

  
