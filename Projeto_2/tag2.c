// Teoria e aplicação de grafos - Turma A - 2019/2
// João Antonio Desiderio de Moraes (16/0126975)
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// ----------------- Definições de tipos -------------------
typedef struct aresta{
    int terminal;       // índice do vértice terminal
    int peso;           // peso ou valor da aresta, 0 = peso padrão
    struct aresta *prox; // referencia para a prox aresta
} tAresta;

typedef struct vertice {
    int id;             // id do vértice
    int grau;           // grau do vértice
    int grauEntrada;    // grau de entrada do vertices
    double coeficiente; // coeficiente de aglomeracao do vertice
    char *nome;         // referencia p/ o nome do vértice
    tAresta *aresta;    // referencia p/ a lista de arestas
    struct vertice *prox;     // referencia p/ o proximo vertice
} tVertice;

typedef struct {
    int qtdVertices;    // numero de vertices no grafo
    int qtdArestas;     // numero de vertices no grafo
    int direcionado;    // 1 = grafo direcionado, 0 = nao direcionado
    double coeficiente; // coeficiente de aglomeracao do grafo
    tVertice *vertice;  // referencia para lista de vertices
} tGrafo;

typedef struct no {
    int id;                 // id do vértice
    struct no *adjacente;   // lista de ids dos vertices adjacentes
    tVertice *vertice;      // endereço do vertice em network
    struct no *prox;        // referencia para proximo no
} tNo;                      // tnode = nó da estrutura da lista de adjacencias

// ------------------ Variáveis globais --------------------

tGrafo *grafo;             // referencia para um grafo
tNo *listaAdjacencias;    // referencia para a lista de adjacencias


// --------------------- Funções ---------------------------

int inicializarGrafo(int qtdVertices, int qtdArestas, int direcionado, int rotulado) {
    tVertice *vertice, *auxVertice;

    grafo = (tGrafo*) malloc(sizeof(tGrafo));
    if (grafo) {
        grafo->qtdVertices = qtdVertices;
        grafo->qtdArestas = qtdArestas;
        grafo->direcionado = direcionado;
        grafo->vertice = NULL;
    } else {
        printf("Não foi possível alocar memória para o grafo. Encerrando...");
        return -1;
    }

    if (!rotulado) {
        printf("n rotulado");
        if (qtdVertices > 0) {
            auxVertice = (tVertice*)malloc(sizeof(tVertice));
            if (auxVertice) {
                auxVertice->id = 1;
                auxVertice->aresta = NULL;
                auxVertice->nome = NULL;
                auxVertice->grau = 0;
                auxVertice->grauEntrada = 0;
                grafo->vertice = auxVertice;
            } else {
                printf("Não foi possível alocar memória para o vértice. Encerrando...");
                return -1;
            }
            for (int i = 2; i <= qtdVertices; i++) {
                vertice = (tVertice*)malloc(sizeof(tVertice));
                if (vertice) {
                    vertice->id = i;
                    auxVertice->aresta = NULL;
                    vertice->nome = NULL;
                    vertice->grau = 0;
                    vertice->grauEntrada = 0;
                    auxVertice->prox = vertice;
                    auxVertice = vertice;    
                } else {
                    printf("Não foi possível alocar memória para o vértice. Encerrando...");
                    return -1;
                }
            }
        }
    }

    return 0;
}

tVertice* inserirVerticeRotulado(tVertice *anterior, int id, char rotulo[]) {
    tVertice *vertice;

    vertice = (tVertice*)malloc(sizeof(tVertice));
    if (vertice) {
        vertice->id = id;
        vertice->aresta = NULL;
        vertice->grau = 0;
        vertice->grauEntrada = 0;
        vertice->nome = rotulo;
        vertice->prox = NULL;
    } else {
        printf("Não foi possível alocar memória para o vértice. Encerrando...");
        return NULL;
    }
    if (anterior != NULL) {
        anterior->prox = vertice;
    }

    return vertice;
}

int criarAresta(tVertice *origem, tVertice *destino, int peso) {
    tAresta *aresta, *novaAresta = (tAresta *) malloc(sizeof(tAresta));
    int flagAresta = 1;
    if (novaAresta == NULL) {
       printf("Não foi possível alocar memória para a aresta. Encerrando...");
       return -1;       
    }

    aresta = origem->aresta;
    if(aresta == NULL) {
        origem->aresta = novaAresta;
    } else {
        while(flagAresta) {
            if (aresta->prox == NULL) {
                flagAresta = 0;
            } else {
                aresta = aresta->prox;
            }
        }
        aresta->prox = novaAresta;
    }
    novaAresta->terminal = destino->id;
    novaAresta->peso = peso;
    return 0;
}

int inserirAresta(int origem, int destino, int peso, int direcionado) {
    tVertice *vertice, *verticeOrigem = NULL, *verticeDestino = NULL;
    int flagOrigem = 1, flagDestino = 1;

    vertice = grafo->vertice;
    while (flagOrigem || flagDestino) {
        if (vertice->id == origem) {
            verticeOrigem = vertice;
            flagOrigem = 0;
        }
        if (vertice->id == destino) {
            verticeDestino = vertice;
            flagDestino = 0;
        }
        vertice = vertice->prox;
    }

    criarAresta(verticeOrigem, verticeDestino, peso);
    if (direcionado == 0) {
        criarAresta(verticeDestino, verticeOrigem, peso);
    } else {
        verticeDestino->grauEntrada++;
    }

    return 0;
}

int lerGrafo(FILE *arquivo) {
    char *linha = malloc(100);
    char *index;
    int qtdVertices, qtdArestas, direcionado, rotulado, id, flag;
    int origem, destino, peso;
    char rotulo[20] = ""; 
    tVertice *vertice, *auxVertice;

    flag = 1;

    while (flag && fgets(linha, 100, arquivo)) {
        index = linha;
        if (*index != '%') {
            flag = 0;
            sscanf(linha, "%d %d %d %d", &qtdVertices, &qtdArestas, &direcionado, &rotulado);
            if(inicializarGrafo(qtdVertices, qtdArestas, direcionado, rotulado) == -1) {
                return -1;
            }
        }
    }

    if (rotulado) {
        auxVertice = NULL;
        fgets(linha, 20, arquivo);
        sscanf(linha, "%d %s", &id, rotulo);
        vertice = inserirVerticeRotulado(auxVertice, id, rotulo);
        grafo->vertice = vertice;
        auxVertice = vertice;
        qtdVertices--;
        while (qtdVertices > 0) {
            fgets(linha, 100, arquivo);
            sscanf(linha, "%d %s", &id, rotulo);
            vertice = inserirVerticeRotulado(auxVertice, id, rotulo);
            auxVertice = vertice;
            qtdVertices--;
        }
    }

    while (fgets(linha, 100, arquivo)) {
        sscanf(linha, "%d %d %d", &origem, &destino, &peso);
        //conferir peso
        inserirAresta(origem, destino, peso, direcionado);
    }

    free(linha);
    return 0;
}

void liberarGrafo() {
    tVertice *vertice, *auxVertice;
    tAresta *aresta, *auxAresta;

    vertice = grafo->vertice;
    while(vertice != NULL) {
        aresta = vertice->aresta;
        while(aresta != NULL) {
            auxAresta = aresta;
            aresta = aresta->prox;
            free(auxAresta);
        }
        auxVertice = vertice;
        vertice = vertice->prox;
        free(auxVertice);
    }
    free(grafo);
}

int criarLista() {
    int i, j;
    tVertice *vertice = grafo->vertice;
    tAresta *aresta;
    tNo *no, *adj;
    listaAdjacencias = malloc(sizeof(tNo));
    if (listaAdjacencias==NULL) {
        return -1;
    }
    no = listaAdjacencias;

    for(i = 0; i < grafo->qtdVertices; i++) {
        aresta = vertice->aresta;
        no->id = vertice->id;
        no->vertice = vertice;
        no->adjacente = malloc(sizeof(tNo));
        no->prox = malloc(sizeof(tNo));
        if (no -> adjacente == NULL) {
            return -1;
        }
        adj = no->adjacente;
        for(j = 0; j < no->vertice->grau; j++) {
            adj->id = aresta->terminal + 1;
            adj->prox = malloc(sizeof(tNo));
            adj = adj->prox;
            aresta++;
        }
        adj->prox = NULL;
        vertice++;
        no = no->prox;
    }
    return 0;
}

// Função para leitura do arquivo
// Parâmetros: n/a
// Retornos: -1 para falha e 0 para sucesso
int lerArquivo() {
    FILE *arquivo = fopen("curriculo.mtx", "r");
    int result;

    if (arquivo == NULL) {
        printf("Não foi possível ler o arquivo \n");
        return -1;
    }

    lerGrafo(arquivo);
    fclose(arquivo);
    
    return 0;
}

// Função para imprimir na tela os vértices e seus
// respectivos graus. (Questão 1)
void imprimirVertices(tVertice *vertice) {
    tAresta *aresta;
    int grau, flagVertice = 1, flagAresta = 1;
    
    if (vertice == NULL) {
        printf("Grafo vazio!\n");
    } else {
        // laço para percorrer os vértices
        while (flagVertice) {
            grau = 0;
            if (vertice->aresta != NULL) {
                aresta = vertice->aresta;
                // laço para percorrer as arestas do vértice
                flagAresta = 1;
                while (flagAresta) {
                    grau++;
                    if (aresta->prox == NULL) {
                        flagAresta = 0;
                    } else {
                        aresta = aresta->prox;
                    }
                }
            }
            vertice->grau = grau;
            printf("Vértice %d - Grau %d\n", vertice->id, vertice->grau);
            if (vertice->prox == NULL) {
                flagVertice = 0;
            } else {
                vertice = vertice->prox;
            }
        }
    }
}

tVertice* copiarListaVertices(tVertice *lista) {
    tVertice *indiceLista, *novo, *inicio, *aux;

    if (lista != NULL) {
        novo = (tVertice*)malloc(sizeof(tVertice));
        indiceLista = lista;
        *novo = *indiceLista;
        indiceLista = indiceLista->prox; 
        inicio = novo;
        aux = novo;
        while(indiceLista != NULL) {
            novo = (tVertice*)malloc(sizeof(tVertice));
            *novo = *indiceLista;
            aux->prox = novo;
            aux = novo;
            indiceLista = indiceLista->prox;
             
        }
        aux->prox = NULL;
        return inicio;
    }

    return NULL;
}

tAresta* copiarListaArestas(tAresta *lista) {
    tAresta *aux, *novo, *inicio;

    novo = (tAresta*)malloc(sizeof(tAresta));
    *novo = *lista;
    inicio = novo;
    aux = novo;
    lista = lista->prox;
    while(lista != NULL) {
        novo = (tAresta*)malloc(sizeof(tAresta));
        *novo = *lista;
        aux->prox = novo;
        aux = novo; 
    }
    aux->prox = NULL;
    return inicio;
}

// Retorna uma lista de vertices com a interseccao dos vertices 
// da lista original e dos adjacentes a outro vertice selecionado.
// Parametros:
// tAresta *arestas: lista de arestas do vertice selecionado (adjacentes)
// tVertice *vertice: lista de vertices  
tVertice* interseccaoVertices(tAresta *arestas, tVertice* vertice) {
    tVertice *freeVertice, *auxVertice, *novoVertice, *interseccao = NULL;
    tAresta *auxAresta;

    if(vertice == NULL) {
        return NULL;
    }
    while (vertice != NULL) {
        auxAresta = arestas;
        while (auxAresta != NULL) {
            if(vertice->id == auxAresta->terminal) {
                novoVertice = (tVertice*) malloc(sizeof(tVertice));
                *novoVertice = *vertice;
                novoVertice->prox = NULL;
                if (interseccao == NULL) {
                    interseccao = novoVertice;
                } else {
                    auxVertice->prox = novoVertice;
                }
                auxVertice = novoVertice;
            }
            auxAresta = auxAresta->prox;
        }
        freeVertice = vertice;
        vertice = vertice->prox;
        free(freeVertice);
    }
    return interseccao;
}

void liberarListaVertices(tVertice *lista) {
    tVertice *aux = lista;
    while(lista != NULL) {
        lista = lista->prox;
        free(aux);
        aux = lista;
    }
}

int liberarListaArestas(tAresta *lista) {
    tAresta *aux = lista;
    while(lista != NULL) {
        lista = lista->prox;
        free(aux);
        aux = lista;
    }
    return 0;
}

// Implementacao do algoritmo BronKerbosch
int bronKerbosch(tVertice *r, tVertice *p, tVertice *x) {
    tVertice *indiceR = r, *indiceP = p, *indiceX = x, *novo, *novoR, *novoP, *novoX, *aux;
    tAresta *indiceAresta;
    int flag;

    if (p == NULL) {
        if(x == NULL) {
            printf("Clique maximal: { ");
            while(indiceR != NULL) {
                printf("%d ", indiceR->id);
                indiceR = indiceR->prox;
            }
            printf("}\n");
        }       
    } else {
        flag = 1;
        while (p != NULL) {
            // Selecionar primeiro de P para adicionar em R
            novo = (tVertice*) malloc(sizeof(tVertice));
            *novo = *p;
            indiceP = p;
            novo->prox = NULL;
            indiceAresta = p->aresta;
            p = p->prox;
            free(indiceP);
            indiceP = p;
            if (r == NULL) {
                r = novo;
                indiceR = novo;
            } else {
                while(indiceR->prox != NULL) {
                    indiceR = indiceR->prox;
                }
                indiceR->prox = novo;
            }

            // Gerar listas que possam ser alteradas sem interferencia
            // nas da iteracao atual
            novoR = copiarListaVertices(r);
            novoP = copiarListaVertices(p);
            novoX = copiarListaVertices(x);
            // Limitar P e X aos adjacentes do vértice adicionado em R
            novoP = interseccaoVertices(indiceAresta, novoP);
            novoX = interseccaoVertices(indiceAresta, novoX);

            // Chamada recursiva da função
            bronKerbosch(novoR, novoP, novoX);

            // Transferir o último vértice adicionado em R para X
            aux = indiceR;
            indiceR = indiceR->prox;    
            if (indiceR != NULL) {
                novo = (tVertice*) malloc(sizeof(tVertice));
                *novo = *indiceR;
                novo->prox = NULL;
                free(indiceR);
                aux->prox = NULL;
            } else {
                r = NULL;
                flag = 0;
            }
            
            indiceR = r;
            indiceX = x;
            if (x == NULL) {
                x = novo;
            } else {
                while(indiceX->prox != NULL) {
                    indiceX = indiceX->prox;
                }
                indiceX->prox = novo;
                indiceX = x;
            }
        }
    }
    // Libera da memoria as listas de vertices que foram utilizadas
    // nesta chamada da funcao
    liberarListaVertices(r);
    liberarListaVertices(p);
    liberarListaVertices(x);

    return 0;
}

// Função para imprimir todos os cliques maximais
// por meio de um algoritmo Bron-Kerbosch
int imprimirCliques() {
    tVertice *r, *p, *x, *indice;
    
    // Inicializando as listas p, r, x
    r = NULL;
    p = NULL;
    x = NULL;
    indice = grafo->vertice;
    if (indice == NULL) {
        printf("Grafo vazio, nenhum clique será encontrado.\n");
    } else {
        p = copiarListaVertices(indice);
    }
    bronKerbosch(r, p, x);
    return 0;
}

// Percorre as arestas do vertice.
// Para cada aresta:
//      Busca o vértice terminal e compara as arestas
//      desse vértice com as demais arestas do vértice original;
//      para cada match, reporta um triangulo possivel.
// Ao fim, divide os triangulos encontrados pelos triangulos
// possiveis.
void obterCoeficienteAglomeracaoVertice(tVertice *vertice) {
    tVertice *auxVertice;
    tAresta *indiceAresta, *auxAresta, *auxArestaVertice;
    int encontrouVertice, encontrouAresta;
    double triangulosPossiveis, qtdTriangulos = 0;

    if (vertice->grau == 1) {
        vertice->coeficiente = 0;
    } else {
        triangulosPossiveis = (vertice->grau * (vertice->grau - 1)) / 2;
        indiceAresta = vertice->aresta;
        auxAresta = indiceAresta->prox;
        for (int i = 1; i < vertice->grau; i++) {
            auxVertice = grafo->vertice;
            encontrouVertice = 0;
            while (!encontrouVertice) {
                if (auxVertice == NULL) {
                    printf("Ocorreu um erro ao procurar o vértice %d. Encerrando procedimento...\n", indiceAresta->terminal);
                    return;
                }
                if (auxVertice->id == indiceAresta->terminal) {
                    encontrouVertice = 1;
                } else {
                    auxVertice = auxVertice->prox;
                }
            }
            auxArestaVertice = auxVertice->aresta;
            while(auxAresta != NULL) {
                encontrouAresta = 1;
                while(auxArestaVertice != NULL && encontrouAresta) {
                    if (auxAresta->terminal == auxArestaVertice->terminal) {
                        qtdTriangulos++;
                        encontrouAresta = 0;
                    }
                    auxArestaVertice = auxArestaVertice->prox;
                }
                auxArestaVertice = auxVertice->aresta;
                auxAresta = auxAresta->prox;
            }
            indiceAresta = indiceAresta->prox;
            auxAresta = indiceAresta->prox;
        }
        vertice->coeficiente = qtdTriangulos/triangulosPossiveis;
    }

    printf("Vértice %d - Coeficiente %.2lf\n", vertice->id, vertice->coeficiente);
}

// - Realiza o calculo do coeficiente para cada vertice,
// - Acumula o coeficiente obtido a cada iteracao,
// - Divide o acumulado pela quantidade de vertices no grafo
//   e imprime o resultado.
void obterCoeficienteAglomeracaoGrafo() {
    tVertice *vertice = grafo->vertice;
    double coeficienteAglomeracao = 0;
    for(int i = 0; i < grafo->qtdVertices; i++) {
        obterCoeficienteAglomeracaoVertice(vertice);
        coeficienteAglomeracao = coeficienteAglomeracao + vertice->coeficiente;
        vertice = vertice->prox;
    }
    printf("\nQuestão 4: imprimir o coeficiente de aglomeração do grafo.\n");
    grafo->coeficiente = coeficienteAglomeracao/(double) grafo->qtdVertices;
    printf("\nO coeficiente de aglomeração do grafo é %.3lf\n", grafo->coeficiente);
}

// Funcao principal do programa
int main() {
    printf("------ Teoria e Aplicação de Grafos - Projeto 2 ------\n");
    printf("---- João Antonio Desiderio de Moraes (16/0126975) ----\n\n");
    lerArquivo();
    imprimirVertices(grafo->vertice);
    // printf("\nQuestão 2: imprimir todos os cliques maximais\n\n");
    // imprimirCliques();
    // printf("\nQuestão 3: imprimir o coeficiente de aglomeração de cada vértice\n\n");
    // obterCoeficienteAglomeracaoGrafo();

    printf("\nLiberando a memória alocada para o grafo...\n");
    liberarGrafo();
    printf("\nEncerrando...\n\n");
    return 0;
}