Teoria e aplicação de grafos - Turma A - 2019/2
João Antonio Desiderio de Moraes (16/0126975)
Hiago dos Santos Rabelo (16/0124492)

Esta é a entrega do segundo projeto de Teoria e Aplicação de Grafos 2019.2. 

AVISO
    O programa contido no arquivo "tag2.c" foi desenvolvido em Linux (Mint 19.1), compilado e testado com o gcc mais atual. O código utiliza bibliotecas padrão, além do pacote graphviz para gerar a visualização do grafo.

COMO COMPILAR E EXECUTAR
    É necessário instalar o pacote graphviz e possuir o comando dot habilitado para que o programa gere a visualização. Caso as imagens não sejam geradas, os arquivos .dot gerados descrevem as estruturas encontradas.
    No mais, não é necessário incluir nenhum parâmetro especial durante a compilação e execução do programa. Os comandos a seguir foram suficientes em uma máquina Linux com o gcc instalado.
    
    gcc -o tag2 tag2.c
    ./tag2

    

SOBRE O PROGRAMA
    O arquivo "tag2.c" contém o código de um programa capaz de abrir grafos descritos em arquivos .mtx, utilizando o mesmo formato do arquivo "curriculo.mtx". O programa pode ser adaptado para ter maior versatilidade na abertura de arquivos, mas foi limitado a este arquivo para garantir uma execução sem imprevistos.

O grafo é formado a partir de uma lista ligada de vértices, e cada nó dessa lista guarda o endereço para uma lista de arestas que informa os vértices adjacentes. O programa aceita também digrafos, pesos e rótulos para os vértices. Estes novos atributos são utilizados para tentar encontrar uma ordenação topológica possível e caminhos críticos no grafo descrito.

Nesta versão do programa, o grafo é lido do arquivo "curriculo.mtx", que contém as disciplinas do currículo do curso Ciência da Computação na Universidade de Brasília. Os vértices representam as máterias; as arestas estabelecem as relações de requisito entre elas, ou seja, a matéria no vértice de origem é o pré-requisito da matéria no vértice terminal.

Em seguida, o programa realiza uma tentativa de ordenação topológica por busca em profundidade no grafo. Para o caso de um vértice já visitado nessa busca ser novamente alcançado em uma das recursões mais internas, o programa acusa um ciclo e não pode continuar. 

Baseado nesta ordenação topológica, o programa define o menor valor (soma de pesos) possível para alcançar cada vértice e extrai daí o caminho crítico encontrado. Tanto o grafo quanto a ordenação e o caminho crítico são extraídos em arquivos .dot, que são depois processados pelo graphviz para renderização gráfica.

