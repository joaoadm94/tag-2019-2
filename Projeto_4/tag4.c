// Teoria e aplicação de grafos - Turma A - 2019/2
// João Antonio Desiderio de Moraes (16/0126975)
// Hiago dos Santos Rabelo (16/0124492)
// ALOCACAO DE MATERIAS DO FLUXO DE CIC - UNB
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a, b) ((a)>(b)?(a):(b))

// ----------------- Definições de tipos -------------------
typedef struct vertice vertice;
typedef struct aresta{
    int terminal;       // índice do vértice terminal
    int peso;           // peso ou valor da aresta, 0 = peso padrão
    int visitado;
    vertice *atalho;    //
    struct aresta *prox; // referencia para a prox aresta
} tAresta;

typedef struct vertice {
    int id;             // id do vértice
    int grau;           // grau do vértice
    int grauEntrada;    // grau de entrada do vertices
    double coeficiente; // coeficiente de aglomeracao do vertice
    int peso;           // peso associado as arestas que saem do vertice
    char nome[10];      // referencia p/ o nome do vértice
    int visitado;       //
    tAresta *grafoAresta;    // referencia p/ a lista de arestas (grafo)
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

tGrafo *grafo;                         // referencia para um grafo
tNo *listaAdjacencias;                 // referencia para a lista de adjacencias

// --------------------- Funções ---------------------------

void imprimirVertices(tVertice *vertice);

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
                auxVertice->grafoAresta = NULL;
                // auxVertice->nome;
                auxVertice->grau = 0;
                auxVertice->grauEntrada = 0;
                auxVertice->visitado = 0;
                grafo->vertice = auxVertice;
            } else {
                printf("Não foi possível alocar memória para o vértice. Encerrando...");
                return -1;
            }
            for (int i = 2; i <= qtdVertices; i++) {
                vertice = (tVertice*)malloc(sizeof(tVertice));
                if (vertice) {
                    vertice->id = i;
                    auxVertice->grafoAresta = NULL;
                    // vertice->nome = NULL;
                    vertice->grau = 0;
                    vertice->grauEntrada = 0;
                    vertice->visitado = 0;
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

tVertice* inserirVerticeRotulado(tVertice *anterior, int id, char rotulo[], int peso) {
    tVertice *vertice;
    vertice = (tVertice*)malloc(sizeof(tVertice));
    if (vertice) {
        vertice->id = id;
        vertice->grafoAresta = NULL;
        vertice->grau = 0;
        vertice->grauEntrada = 0;
        strcpy(vertice->nome, rotulo);
        vertice->prox = NULL;
        vertice->peso = peso;
        vertice->visitado = 0;
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

    aresta = origem->grafoAresta;
    if(aresta == NULL) {
        origem->grafoAresta = novaAresta;
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
    novaAresta->atalho = destino;
    return 0;
}

int inserirAresta(int origem, int destino, int direcionado) {
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

    criarAresta(verticeOrigem, verticeDestino, verticeOrigem->peso);
    if (direcionado == 0) {
        criarAresta(verticeDestino, verticeOrigem, verticeDestino->peso);
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
        sscanf(linha, "%d %s %d", &id, rotulo, &peso);
        
        vertice = inserirVerticeRotulado(auxVertice, id, rotulo, peso);
        grafo->vertice = vertice;
        auxVertice = vertice;
        qtdVertices--;
        while (qtdVertices > 0) {
            fgets(linha, 100, arquivo);
            sscanf(linha, "%d %s %d", &id, rotulo, &peso);
            vertice = inserirVerticeRotulado(auxVertice, id, rotulo, peso);
            auxVertice = vertice;
            qtdVertices--;
        }
    }
    imprimirVertices(grafo->vertice);

    while (fgets(linha, 100, arquivo)) {
        sscanf(linha, "%d %d", &origem, &destino);
        // conferir peso
        inserirAresta(origem, destino, direcionado);
    }

    free(linha);
    return 0;
}

void liberarGrafo() {
    tVertice *vertice, *auxVertice;
    tAresta *aresta, *auxAresta;

    vertice = grafo->vertice;
    while(vertice != NULL) {
        aresta = vertice->grafoAresta;
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
        aresta = vertice->grafoAresta;
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
            if (vertice->grafoAresta != NULL) {
                aresta = vertice->grafoAresta;
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
            printf("Vértice %d - Grau %d - Entrada %d - Peso %d\n", vertice->id, 
                                                                    vertice->grau, 
                                                                    vertice->grauEntrada,
                                                                    vertice->peso);
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

int depthFirstSearch(tVertice *vert) {
    tAresta *aresta;
    int result = 0;
    
    if(vert->visitado == 1) {
        return -1;
    }
    if (vert->visitado == 0) {
        vert->visitado = 1; 
        aresta = vert->grafoAresta;
        if (aresta != NULL) {
            while (aresta != NULL && result == 0) {
                result = depthFirstSearch(aresta->atalho);
                aresta = aresta->prox;
            }
        }
        if (result == -1) {
            return -1;
        }
        vert->visitado = 2;
    }
    return 0;
}

int converteGrafoParaArquivoVisual(){
    char *arquivoFonteDot = "grafo.dot";
    char *formatoGrafo = "digraph Disciplinas {\n";
    char *descricao = " c [color=lightblue, label=\"Disciplinas\nCurso: Ciência da Computação\"];\n";
    char comando[200];

    FILE *pArq; 
    tAresta *arestaAux;
    tVertice *verticeAux = grafo->vertice;
    int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoGrafo);
    fprintf(pArq, descricao);

    for(i = 0; i < grafo->qtdVertices; i++){
        arestaAux = verticeAux->grafoAresta;

        if (verticeAux->grauEntrada == 0 && arestaAux == NULL) {
            fprintf(pArq, "%s;\n", verticeAux->nome);
        }
        while(arestaAux){
            fprintf(pArq, "%s -> %s;\n", verticeAux->nome, arestaAux->atalho->nome);
            arestaAux = arestaAux->prox;
        }
        verticeAux = verticeAux->prox;
    }

    fprintf(pArq,"\n}");
    fclose(pArq);

    sprintf(comando, "dot -Tpng -O '%s'", arquivoFonteDot);
    return system(comando);
}

// Funcao principal do programa
int main() {
    printf("------ Teoria e Aplicação de Grafos - Projeto 4 ------\n");
    printf("---- João Antonio Desiderio de Moraes (16/0126975) ----\n");
    printf("---- Hiago dos Santos Rabelo (16/0124492) ----\n\n");
    lerArquivo();
    imprimirVertices(grafo->vertice);
    if(converteGrafoParaArquivoVisual()){
        printf("Não foi possível passar o grafo para o arquivo visual\n");
        return EXIT_FAILURE;
    }
    // achaVerticesComMaiorCustoDeChegadaEInsereNaStruct();
    // printf(">> Caminhos críticos: \n");
    // printf("Maior: %s\n", maiorESegundoMaiorCaminho.Maior->nome);
    // printf("Segundo Maior: %s\n", maiorESegundoMaiorCaminho.segundoMaior->nome);
    // printf("\nQuestão 2: imprimir todos os cliques maximais\n\n");
    // imprimirCliques();
    // printf("\nQuestão 3: imprimir o coeficiente de aglomeração de cada vértice\n\n");
    // obterCoeficienteAglomeracaoGrafo();
    printf("\nLiberando a memória alocada para o grafo...\n");
    liberarGrafo();
    printf("\nEncerrando...\n\n");
    return 0;
}