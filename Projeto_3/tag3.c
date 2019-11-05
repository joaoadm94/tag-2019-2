// Teoria e aplicação de grafos - Turma A - 2019/2
// João Antonio Desiderio de Moraes (16/0126975)
// Hiago dos Santos Rabelo (16/0124492)
 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX(a, b) ((a)>(b)?(a):(b))

// ----------------- Definições de tipos -------------------
typedef struct verticeEscola verticeEscola;
typedef struct verticeProfessor verticeProfessor;
// typedef struct arestaEscola arestaEscola;
// typedef struct arestaProfessor verticeProfessor;

typedef struct arestaEscola{
    int terminal;       // índice do vértice terminal
    int prioridade;           // prioridade (1 = máx e assim por diante)
    int visitado;       // 1 se já foi visitado e 0 caso contrário
    verticeProfessor *atalho;    //
    struct arestaEscola *prox; // referencia para a prox aresta
} tArestaEscola;

typedef struct arestaProfessor{
    int terminal;                       // índice do vértice terminal
    int prioridade;                     // prioridade (1 = máx e assim por diante)
    verticeEscola *atalho;    
    struct arestaProfessor *prox; // referencia para a prox aresta
} tArestaProfessor;

typedef struct verticeEscola {
    int id;             // id do vértice
    int grau;           // grau do vértice
    int emparelhado;    // 0 se não foi emparelhado ainda
    int grauEntrada;    // grau de entrada do vertices
    int habilitacoesMinimas[2]; //habilitações mínimas, se não tiver a segunda fica como zero
    int custoFinalizar; // maior custo para se chegar e finalizar esse vértice (dag)
    verticeEscola *VerticeAntMaiorCusto; // de qual vértice tem que partir para se chegar no de maior custo
    double coeficiente; // coeficiente de aglomeracao do vertice
    int peso;           // peso associado as arestas que saem do vertice
    char nome[10];         // referencia p/ o nome do vértice
    verticeProfessor *ProfessorRequisitado1;
    verticeProfessor *ProfessorRequisitado2;
    tArestaEscola *ArestaParaProfessor1;  // referencia p/ a lista de arestas das opções de professores
    tArestaEscola *ArestaParaProfessor2;  // referencia p/ a lista de arestas das opções de professores
    struct verticeEscola *prox;    // referencia p/ o proximo vertice
} tVerticeEscola;

typedef struct verticeProfessor {
    int id;             // id do vértice
    int grau;           // grau do vértice
    int grauEntrada;    // grau de entrada do vertices
    int habilitacao;    // habilitação que o professor tem 
    int custoFinalizar; // maior custo para se chegar e finalizar esse vértice (dag)
    verticeProfessor *VerticeAntMaiorCusto; // de qual vértice tem que partir para se chegar no de maior custo
    double coeficiente; // coeficiente de aglomeracao do vertice
    int peso;           // peso associado as arestas que saem do vertice
    char nome[10];         // referencia p/ o nome do vértice
    verticeEscola *escolaQueORecebeu;
    tArestaProfessor *atualEscolaASerAnalizada;
    tArestaProfessor *ArestaParaEscola;    // referencia p/ a lista de arestas (grafo)
    tArestaProfessor *CriticoAresta;  // referencia p/ lista de arestas (caminho crítico)
    struct verticeProfessor *prox;    // referencia p/ o proximo vertice
} tVerticeProfessor;

typedef struct {
    int qtdEscolas;    // numero de Vértices Escolas no grafo
    int qtdProfessores;     // numero de vertices Professores no grafo
    // int direcionado;    // 1 = grafo direcionado, 0 = nao direcionado
    // double coeficiente; // coeficiente de aglomeracao do grafo
    tVerticeProfessor *verticeProfessor;  // referencia para lista de vertices
    tVerticeEscola *verticeEscola;  // referencia para lista de vertices
} tGrafo;

typedef struct no {
    int id;                 // id do vértice
    struct no *adjacente;   // lista de ids dos vertices adjacentes
    tVerticeEscola *vertice;      // endereço do vertice em network
    struct no *prox;        // referencia para proximo no
} tNo;                      // tnode = nó da estrutura da lista de adjacencias

typedef struct NoOrdTop{
    tVerticeEscola *vertice;
    struct NoOrdTop *prox;
} tNoOrdTop;

typedef struct OrdenacaoTop{
    tNoOrdTop *inicio;
    tNoOrdTop *fim;
    int ciclo;
} listaOrdenacaoTop;

typedef struct tCaminhoCritico{
    tVerticeEscola *Maior;        // Vértice com maior custo para se chegar e finalizar
    tVerticeEscola *segundoMaior; // Segundo vértice com maior custo para se chegar e finalizar
}tCaminhoCritico;

// ------------------ Variáveis globais --------------------

tGrafo *grafo;                         // referencia para um grafo
tNo *listaAdjacencias;                 // referencia para a lista de adjacencias
listaOrdenacaoTop listaOrdTop; // referencia para a lista de adjacencias 
                                       // ordenada topologicamente
tCaminhoCritico maiorESegundoMaiorCaminho;

// --------------------- Funções ---------------------------

/*
void zeraListaOrdTop(){
    listaOrdTop.inicio = NULL;
    listaOrdTop.fim = NULL;
    listaOrdTop.ciclo = 0;
}
*/


int inicializarGrafo(int qtdProfessores, int qtdEscolas, int qtdEscolasEscolhidas, int qtdProfEscolhidos) {
    tVerticeEscola *verticeEscola, *auxVerticeEscola;
    tVerticeProfessor *verticeProfessor, *auxVerticeProfessor;

    grafo = (tGrafo*) malloc(sizeof(tGrafo));
    if (grafo) {
        grafo->qtdEscolas = qtdEscolas;
        grafo->qtdProfessores = qtdProfessores;
    } else {
        printf("Não foi possível alocar memória para o grafo. Encerrando...");
        return -1;
    }
    
    if (qtdEscolas > 0 && qtdProfessores > 0) {
        auxVerticeEscola = (tVerticeEscola*)malloc(sizeof(tVerticeEscola));
        if (auxVerticeEscola) {
            auxVerticeEscola->id = 1;
            auxVerticeEscola->ArestaParaProfessor1 = NULL;
            auxVerticeEscola->ArestaParaProfessor2 = NULL;
            // auxVerticeEscola->nome;
            auxVerticeEscola->grau = 0;
            auxVerticeEscola->grauEntrada = 0;
            auxVerticeEscola->prox = NULL;
            grafo->verticeEscola = auxVerticeEscola;
        } else {
            printf("Não foi possível alocar memória para o vértice. Encerrando...");
            return -1;
        }
        for (int i = 2; i <= qtdEscolas; i++) {
            verticeEscola = (tVerticeEscola*)malloc(sizeof(tVerticeEscola));
            if (verticeEscola) {
                verticeEscola->id = i;
                auxVerticeEscola->ArestaParaProfessor1 = NULL;
                auxVerticeEscola->ArestaParaProfessor2 = NULL;
                // vertice->nome = NULL;
                verticeEscola->grau = 0;
                verticeEscola->grauEntrada = 0;
                verticeEscola->prox = NULL;
                auxVerticeEscola->prox = verticeEscola;
                auxVerticeEscola = verticeEscola;    
            } else {
                printf("Não foi possível alocar memória para o vértice. Encerrando...");
                return -1;
            }
        }

        auxVerticeProfessor = (tVerticeProfessor*)malloc(sizeof(tVerticeProfessor));
        if (auxVerticeProfessor) {
            auxVerticeProfessor->id = 1;
            auxVerticeProfessor->ArestaParaEscola = NULL;
            // auxVerticeProfessor->nome;
            auxVerticeProfessor->grau = 0;
            auxVerticeProfessor->grauEntrada = 0;
            auxVerticeProfessor->prox = NULL;
            grafo->verticeProfessor = auxVerticeProfessor;
        } else {
            printf("Não foi possível alocar memória para o vértice de escolas. Encerrando...");
            return -1;
        }
        for (int i = 2; i <= qtdProfessores; i++) {
            verticeProfessor = (tVerticeProfessor*)malloc(sizeof(tVerticeProfessor));
            if (verticeProfessor) {
                verticeProfessor->id = i;
                auxVerticeProfessor->ArestaParaEscola = NULL;        
                // vertice->nome = NULL;
                verticeProfessor->grau = 0;
                verticeProfessor->grauEntrada = 0;
                verticeProfessor->prox = NULL;
                auxVerticeProfessor->prox = verticeProfessor;
                auxVerticeProfessor = verticeProfessor;    
            } else {
                printf("Não foi possível alocar memória para o vértice de professores. Encerrando...");
                return -1;
            }
        }
    }
    
    return 0;
}

/*
tVerticeEscola* inserirVerticeRotulado(tVerticeEscola *anterior, int id, char rotulo[], int peso) {
    tVerticeEscola *vertice;
    vertice = (tVerticeEscola*)malloc(sizeof(tVerticeEscola));
    if (vertice) {
        vertice->id = id;
        vertice->ArestaParaProfessor1 = NULL;
        vertice->ArestaParaProfessor2 = NULL;
        vertice->grau = 0;
        vertice->grauEntrada = 0;
        strcpy(vertice->nome, rotulo);
        vertice->prox = NULL;
        vertice->prioridade = peso;
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
*/

int criarArestaEscolaParaProfessor(tVerticeEscola *verticeEscola, tVerticeProfessor *verticeProfessor, 
                                    int indiceHabilitacaoMin){
    
    tArestaEscola *arestaEscola = (tArestaEscola *) malloc(sizeof(tArestaEscola));
    tArestaEscola *auxArestaEscola;

    if(indiceHabilitacaoMin == 0){
        auxArestaEscola = verticeEscola->ArestaParaProfessor1;
    }
    else{
        auxArestaEscola = verticeEscola->ArestaParaProfessor2;
    }
    // tVerticeEscola *auxVerticeEscola = verticeEscola;

    if(auxArestaEscola == NULL){
        if(indiceHabilitacaoMin == 0)
            verticeEscola->ArestaParaProfessor1 = arestaEscola;
        else
            verticeEscola->ArestaParaProfessor2 = arestaEscola;
    }
    else{
        while(auxArestaEscola->prox!= NULL){
            auxArestaEscola = auxArestaEscola->prox;
        }
        auxArestaEscola->prox = arestaEscola;
    }
    // auxVerticeEscola = auxVerticeEscola->prox;
    arestaEscola->atalho = verticeProfessor;
    arestaEscola->prioridade = verticeProfessor->habilitacao - verticeEscola->habilitacoesMinimas[indiceHabilitacaoMin] +1;
    arestaEscola->prox = NULL;
    arestaEscola->terminal = 0;
    arestaEscola->visitado = 0;

    // if(verticeEscola->ArestaParaEscola == NULL) {
    //     verticeEscola->ArestaParaEscola = arestaEscola;
    // } else {
    //     auxArestaEscola = verticeEscola->ArestaParaEscola;
    //     while(auxArestaEscola->prox != NULL) {
    //         auxArestaEscola = auxArestaEscola->prox;
    //     }
    //     auxArestaEscola->prox = arestaEscola;
    // }
    // printf(">>>> %d", arestaProfessor->);
    
    return 0;
}



int inserirArestaEscola(tVerticeEscola* verticeEscola, int indiceHabilitacaoMin){

    tVerticeProfessor* auxVerticeProfessor = grafo->verticeProfessor;
    int i;

    for(i = 0; i < grafo->qtdProfessores; i++){
        if(auxVerticeProfessor->habilitacao >= verticeEscola->habilitacoesMinimas[indiceHabilitacaoMin]){
            criarArestaEscolaParaProfessor(verticeEscola, auxVerticeProfessor, indiceHabilitacaoMin);
        }
        auxVerticeProfessor = auxVerticeProfessor->prox;
    }

    return 0;
}

int criarArestaProfessorParaEscola(tVerticeProfessor *verticeProfessor, 
                                    int escolaEscolhida, int prioridade) {
    tArestaProfessor *arestaProfessor = (tArestaProfessor *) malloc(sizeof(tArestaProfessor));
    tArestaProfessor *auxArestaProfessor;
    tVerticeEscola *auxVerticeEscola = grafo->verticeEscola;

    while(escolaEscolhida != auxVerticeEscola->id){
        auxVerticeEscola = auxVerticeEscola->prox;
    }

    arestaProfessor->atalho = auxVerticeEscola;
    arestaProfessor->prioridade = prioridade;
    arestaProfessor->prox = NULL;
    arestaProfessor->terminal = 0;

    if(verticeProfessor->ArestaParaEscola == NULL) {
        verticeProfessor->ArestaParaEscola = arestaProfessor;
    } else {
        auxArestaProfessor = verticeProfessor->ArestaParaEscola;
        while(auxArestaProfessor->prox != NULL) {
            auxArestaProfessor = auxArestaProfessor->prox;
        }
        auxArestaProfessor->prox = arestaProfessor;
    }
    // printf(">>>> %d", arestaProfessor->);

    return 0;
}

int inserirArestaProfessor(tVerticeProfessor* verticeProfessor, 
                           int escolasEscolhidas[], 
                           int qtdeEscolasEscolhidas) {

    tArestaProfessor* auxArestaProfessor;
    int i;
    // int flagOrigem = 1, flagDestino = 1;

    // vertice = grafo->verticeProfessor;
    for(i = 0; i < qtdeEscolasEscolhidas; i++){
        criarArestaProfessorParaEscola(verticeProfessor, escolasEscolhidas[i], i+1);
    }

    return 0;
}


int lerGrafo(FILE *arquivo) {
    const int VALORMALLOC = 140;
    char *linha = malloc(VALORMALLOC);
    char *index;
    int id, habilitacao, x, ocorrenciasDaLetra;
    char caractere = '(';
    int escolaEscolhida[4];
    int qtdProfessores, qtdEscolas, qtdEscolasEscolhidas, qtdProfEscolhidos, flag;
    int habilitacaoMinimaExigida[2];
    //int origem, destino, peso;
    char rotulo[20] = ""; 
    tVerticeEscola *verticeEscola, *auxVerticeEscola;
    tVerticeProfessor *auxVerticeProfessor;

    // Arestas de professores para escolas

    flag = 1;

    while (flag && fgets(linha, VALORMALLOC, arquivo)) {
        index = linha;
        if (*index != '/' && *index != '\n') {
            flag = 0;
            sscanf(linha, "%d %d %d %d", &qtdProfessores, &qtdEscolas, &qtdEscolasEscolhidas, &qtdProfEscolhidos);
            if(inicializarGrafo(qtdProfessores, qtdEscolas, qtdEscolasEscolhidas, qtdProfEscolhidos) == -1) {
                return -1;
            }
        }
    }
    
    flag = 1;

    while (flag && fgets(linha, VALORMALLOC, arquivo)) {
        index = linha;
        if (*index != '/' && *index != '\n') {
            flag = 0;
            sscanf(linha, "(P%d, %d): (E%d, E%d, E%d, E%d)\n", &id, &habilitacao, &escolaEscolhida[0], 
                &escolaEscolhida[1], &escolaEscolhida[2], &escolaEscolhida[3]);
                grafo->verticeProfessor->habilitacao = habilitacao;
        }
    }

    inserirArestaProfessor(grafo->verticeProfessor, escolaEscolhida, qtdEscolasEscolhidas);

    auxVerticeProfessor = grafo->verticeProfessor->prox;
    qtdProfessores--;
    while (qtdProfessores > 0) {
        fgets(linha, VALORMALLOC, arquivo);
        sscanf(linha, "(P%d, %d): (E%d, E%d, E%d, E%d)\n", &id, &habilitacao, &escolaEscolhida[0], 
                &escolaEscolhida[1], &escolaEscolhida[2], &escolaEscolhida[3]);
                auxVerticeProfessor->habilitacao = habilitacao;
        
        inserirArestaProfessor(auxVerticeProfessor, escolaEscolhida, qtdEscolasEscolhidas);
        auxVerticeProfessor = auxVerticeProfessor->prox;
        qtdProfessores--;
    }

    // Arestas de escolas para professores
    
    
    flag = 1;

    while (flag && fgets(linha, VALORMALLOC, arquivo)) {
        index = linha;
        if (*index != '/' && *index != '\n') {
            flag = 0;
            sscanf(linha, "(E%d):(%d):(%d)\n", &id, &habilitacaoMinimaExigida[0],
                                                    &habilitacaoMinimaExigida[1]);
                grafo->verticeEscola->habilitacoesMinimas[0] = habilitacaoMinimaExigida[0];
                grafo->verticeEscola->habilitacoesMinimas[1] = habilitacaoMinimaExigida[1];
        }
    }

    inserirArestaEscola(grafo->verticeEscola, 0);
    if(habilitacaoMinimaExigida[1]){
        inserirArestaEscola(grafo->verticeEscola, 1);
    }
    
    auxVerticeEscola = grafo->verticeEscola->prox;
    qtdEscolas--;
    while (qtdEscolas > 0) {
        fgets(linha, VALORMALLOC, arquivo);
        for(x=0, ocorrenciasDaLetra=0; x < strlen(linha); ++x)
            if (linha[x] == caractere) ++ocorrenciasDaLetra;

        if(ocorrenciasDaLetra ==3){
        sscanf(linha, "(E%d):(%d):(%d)\n", &id, &habilitacaoMinimaExigida[0],
                                                    &habilitacaoMinimaExigida[1]);
                auxVerticeEscola->habilitacoesMinimas[0] = habilitacaoMinimaExigida[0];
                auxVerticeEscola->habilitacoesMinimas[1] = habilitacaoMinimaExigida[1];

        }
        else{
            if(ocorrenciasDaLetra ==2){
                sscanf(linha, "(E%d):(%d)\n", &id, &habilitacaoMinimaExigida[0]);
                auxVerticeEscola->habilitacoesMinimas[0] = habilitacaoMinimaExigida[0];
            }
        }

        inserirArestaEscola(auxVerticeEscola, 0);
        if(habilitacaoMinimaExigida[1]){
            inserirArestaEscola(auxVerticeEscola, 1);
        }

        auxVerticeEscola = auxVerticeEscola->prox;
        qtdEscolas--;
    }
    
    free(linha);
    return 0;
}


/*
void liberarGrafo() {
    tVerticeEscola *vertice, *auxVertice;
    tArestaEscola *aresta, *auxAresta;

    vertice = grafo->vertice;
    while(vertice != NULL) {
        aresta = vertice->ArestaParaProfessor;
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
*/

/*
int criarLista() {
    int i, j;
    tVerticeEscola *vertice = grafo->vertice;
    tArestaEscola *aresta;
    tNo *no, *adj;
    listaAdjacencias = malloc(sizeof(tNo));
    if (listaAdjacencias==NULL) {
        return -1;
    }
    no = listaAdjacencias;

    for(i = 0; i < grafo->qtdVertices; i++) {
        aresta = vertice->ArestaParaProfessor;
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
*/

/*
int InserirNaListaOrdenacaoTopologica(tVerticeEscola* vertice) {
    
    tNoOrdTop *noOrdTopologica = malloc(sizeof(tNoOrdTop));
    noOrdTopologica->vertice = vertice;
    noOrdTopologica->prox = NULL;

    if(listaOrdTop.inicio == NULL){
        listaOrdTop.inicio = noOrdTopologica;
        listaOrdTop.fim = noOrdTopologica;
    }
    else{
        noOrdTopologica->prox = listaOrdTop.inicio;
        listaOrdTop.inicio = noOrdTopologica;
    }
    
    return 0;
}
*/

// Função para leitura do arquivo
// Parâmetros: n/a
// Retornos: -1 para falha e 0 para sucesso
int lerArquivo() {
    FILE *arquivo = fopen("entradaProj3TAG.txt", "r");
    int result;

    if (arquivo == NULL) {
        printf("Não foi possível ler o arquivo \n");
        return -1;
    }

    lerGrafo(arquivo);
    fclose(arquivo);
    
    return 0;
}



// Função para imprimir na tela os vértices das Escolas
void imprimirVerticesEscola(tVerticeEscola *verticeEscola) {
    
    tArestaEscola *auxArestaEscola; 
    int flagVertice;

    flagVertice = 1;
                                                                    
    
    if (verticeEscola == NULL) {
        printf("Grafo vazio!\n");
    } else {
        // laço para percorrer os vértices
        while (flagVertice) {
            
            
            // if (verticeEscola->ArestaParaProfessor != NULL) {
            //     aresta1 = verticeEscola->ArestaParaProfessor1;
            //     aresta2 = verticeEscola->ArestaParaProfessor2;
            //     // laço para percorrer as arestas do vértice
            //     flagAresta = 1;
            //     while (flagAresta) {
            //         grau++;
            //         if (aresta1->prox == NULL) {
            //             flagAresta = 0;
            //         } else {
            //             aresta = aresta->prox;
            //         }
            //     }
            // }
            
            // verticeEscola->grau = grau;
            if(verticeEscola->ProfessorRequisitado1)
                printf("Escola %d Professor %d-", verticeEscola->id,
                                          verticeEscola->ProfessorRequisitado1->id);
            printf("Escola %d - Habilitações Mín[0]. \n", verticeEscola->id);
            
            auxArestaEscola = verticeEscola->ArestaParaProfessor1;            
            while(auxArestaEscola != NULL){ 
                printf("%d prior: %d -", auxArestaEscola->atalho->id, auxArestaEscola->prioridade);
                auxArestaEscola = auxArestaEscola->prox;
            }
                printf("\n");
            if(verticeEscola->habilitacoesMinimas[1] > 0){
                printf("Mín[1]: ");    
                auxArestaEscola = verticeEscola->ArestaParaProfessor2;            
                while(auxArestaEscola != NULL){                      
                    printf("%d prior: %d -", auxArestaEscola->atalho->id, auxArestaEscola->prioridade);
                    auxArestaEscola = auxArestaEscola->prox;
                }
                printf("\n");
            }          
                        printf("\n");
            if (verticeEscola->prox == NULL) {
                flagVertice = 0;
            } else {
                verticeEscola = verticeEscola->prox;
            }
        }
    }
    
}


void imprimirVerticesProfessores(tVerticeProfessor *verticeProfessor) {
    tArestaProfessor *aresta;
    int grau, flagVertice = 1, flagAresta = 1;
    
    if (verticeProfessor == NULL) {
        printf("Grafo vazio!\n");
    } else {
        // laço para percorrer os vértices
        printf(">>> PROFESSORES \n\n");
        while (verticeProfessor) {
            /*
            if (verticeProfessor->ArestaParaProfessor != NULL) {
                aresta1 = verticeProfessor->ArestaParaProfessor1;
                aresta2 = verticeProfessor->ArestaParaProfessor2;
                // laço para percorrer as arestas do vértice
                flagAresta = 1;
                while (flagAresta) {
                    grau++;
                    if (aresta1->prox == NULL) {
                        flagAresta = 0;
                    } else {
                        aresta = aresta->prox;
                    }
                }
            }
            */
            
            printf("Vértice %02d - Habilitação %d - Preferência de escola: %02d %02d %02d %02d - respecitiva prioridade %d %d %d %d \n", verticeProfessor->id, 
                                                                    verticeProfessor->habilitacao, 
                                                                    verticeProfessor->ArestaParaEscola->atalho->id,
                                                                    verticeProfessor->ArestaParaEscola->prox->atalho->id,
                                                                    verticeProfessor->ArestaParaEscola->prox->prox->atalho->id,
                                                                    verticeProfessor->ArestaParaEscola->prox->prox->prox->atalho->id,
                                                                    verticeProfessor->ArestaParaEscola->prioridade,
                                                                    verticeProfessor->ArestaParaEscola->prox->prioridade,
                                                                    verticeProfessor->ArestaParaEscola->prox->prox->prioridade,
                                                                    verticeProfessor->ArestaParaEscola->prox->prox->prox->prioridade);
            
            // printf("Prof %d Escola -", verticeProfessor->id,
                                        //   verticeProfessor->escolaQueORecebeu->id);

            verticeProfessor = verticeProfessor->prox;
        }
    }
}


/*
tVerticeEscola* copiarListaVertices(tVerticeEscola *lista) {
    tVerticeEscola *indiceLista, *novo, *inicio, *aux;

    if (lista != NULL) {
        novo = (tVerticeEscola*)malloc(sizeof(tVerticeEscola));
        indiceLista = lista;
        *novo = *indiceLista;
        indiceLista = indiceLista->prox; 
        inicio = novo;
        aux = novo;
        while(indiceLista != NULL) {
            novo = (tVerticeEscola*)malloc(sizeof(tVerticeEscola));
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
*/

/*
tArestaEscola* copiarListaArestas(tArestaEscola *lista) {
    tArestaEscola *aux, *novo, *inicio;

    novo = (tArestaEscola*)malloc(sizeof(tArestaEscola));
    *novo = *lista;
    inicio = novo;
    aux = novo;
    lista = lista->prox;
    while(lista != NULL) {
        novo = (tArestaEscola*)malloc(sizeof(tArestaEscola));
        *novo = *lista;
        aux->prox = novo;
        aux = novo; 
    }
    aux->prox = NULL;
    return inicio;
}
*/

/*
// Retorna uma lista de vertices com a interseccao dos vertices 
// da lista original e dos adjacentes a outro vertice selecionado.
// Parametros:
// tArestaEscola *arestas: lista de arestas do vertice selecionado (adjacentes)
// tVerticeEscola *vertice: lista de vertices  
tVerticeEscola* interseccaoVertices(tArestaEscola *arestas, tVerticeEscola* vertice) {
    tVerticeEscola *freeVertice, *auxVertice, *novoVertice, *interseccao = NULL;
    tArestaEscola *auxAresta;

    if(vertice == NULL) {
        return NULL;
    }
    while (vertice != NULL) {
        auxAresta = arestas;
        while (auxAresta != NULL) {
            if(vertice->id == auxAresta->terminal) {
                novoVertice = (tVerticeEscola*) malloc(sizeof(tVerticeEscola));
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
*/

/*
void liberarListaVertices(tVerticeEscola *lista) {
    tVerticeEscola *aux = lista;
    while(lista != NULL) {
        lista = lista->prox;
        free(aux);
        aux = lista;
    }
}
*/

/*
int liberarListaArestas(tArestaEscola *lista) {
    tArestaEscola *aux = lista;
    while(lista != NULL) {
        lista = lista->prox;
        free(aux);
        aux = lista;
    }
    return 0;
}
*/

/*
// Implementacao do algoritmo BronKerbosch
int bronKerbosch(tVerticeEscola *r, tVerticeEscola *p, tVerticeEscola *x) {
    tVerticeEscola *indiceR = r, *indiceP = p, *indiceX = x, *novo, *novoR, *novoP, *novoX, *aux;
    tArestaEscola *indiceAresta;
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
            novo = (tVerticeEscola*) malloc(sizeof(tVerticeEscola));
            *novo = *p;
            indiceP = p;
            novo->prox = NULL;
            indiceAresta = p->ArestaParaProfessor;
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
                novo = (tVerticeEscola*) malloc(sizeof(tVerticeEscola));
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
*/

/*
int depthFirstSearch(tVerticeEscola *vert) {
    tArestaEscola *aresta;
    int result = 0;
    
    if(vert->visitado == 1) {
        return -1;
    }
    if (vert->visitado == 0) {
        vert->visitado = 1; 
        aresta = vert->ArestaParaProfessor;
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
        InserirNaListaOrdenacaoTopologica(vert);
    }
    return 0;
}
*/

/*
int ordenacaoTopologica() {
    tVerticeEscola *vertice = grafo->vertice;
    int result = 0;

    while (vertice != NULL && result == 0) {
        result = depthFirstSearch(vertice);
        vertice = vertice->prox;
    }
    if (result == -1) {
        printf("Ciclo encontrado. Ordenacao topologica impossivel. \n");
        listaOrdTop.ciclo = 1;
    }

    return 0;
}
*/

/*
// Analisa o Grafo e insere nos vértices o maior custo
// para se chegar naquele vértice e também o respectivo vértice anterior
// a ele que deve se ter como origem para se chegar naquele vértice, 
// apontando apenas para um vértice atrás 
int InsereNosVerticesOCustoDeChegada(){
    int i;
    int aux_custo;
    tArestaEscola* tmp_aresta;
    tNoOrdTop *tmpNoOrd = listaOrdTop.inicio;
    tVerticeEscola* tmp_vertice = grafo->vertice;

    for(i=0; i<grafo->qtdVertices; ++i){
        // tmp_vertice->CriticoAresta = NULL;
        tmp_vertice->custoFinalizar = tmp_vertice->prioridade;
        tmp_vertice->VerticeAntMaiorCusto = tmp_vertice;
        // printf("%s: %i\n", tmp_vertice->nome, tmp_vertice->custoFinalizar);

        tmp_vertice = tmp_vertice->prox;
    }
    tmp_aresta = tmpNoOrd->vertice->ArestaParaProfessor;

    for(i=0; i<grafo->qtdVertices; ++i){
        tmp_aresta = tmpNoOrd->vertice->ArestaParaProfessor;
        // printf("id vértice atual: %s \n", tmpNoOrd->vertice->nome);

        
        while(tmp_aresta){
            aux_custo = tmp_aresta->atalho->prioridade + tmpNoOrd->vertice->custoFinalizar;
            // printf("%d ", aux_custo);
            if(tmp_aresta->atalho->custoFinalizar < aux_custo){
                tmp_aresta->atalho->custoFinalizar = aux_custo;
                tmp_aresta->atalho->VerticeAntMaiorCusto = tmpNoOrd->vertice;
            }
            tmp_aresta = tmp_aresta->prox;
        }
        tmpNoOrd = tmpNoOrd->prox;
    }
    printf("\n");
    tmp_vertice = grafo->vertice;
    // for(i=0; i<grafo->qtdVertices;++i){
    //     printf("Maior custo para finalizar (%s): %i\n", tmp_vertice->nome,
    //                                         tmp_vertice->custoFinalizar);
    //     printf("Vértice anterior maior custo: %s\n\n", tmp_vertice->VerticeAntMaiorCusto->nome);                                    
    //     tmp_vertice = tmp_vertice->prox;
    // }

    return 0;
}
*/

/*
// Função para imprimir todos os cliques maximais
// por meio de um algoritmo Bron-Kerbosch
int imprimirCliques() {
    tVerticeEscola *r, *p, *x, *indice;
    
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
*/

/*
// Percorre as arestas do vertice.
// Para cada aresta:
//      Busca o vértice terminal e compara as arestas
//      desse vértice com as demais arestas do vértice original;
//      para cada match, reporta um triangulo possivel.
// Ao fim, divide os triangulos encontrados pelos triangulos
// possiveis.
void obterCoeficienteAglomeracaoVertice(tVerticeEscola *vertice) {
    tVerticeEscola *auxVertice;
    tArestaEscola *indiceAresta, *auxAresta, *auxArestaVertice;
    int encontrouVertice, encontrouAresta;
    double triangulosPossiveis, qtdTriangulos = 0;

    if (vertice->grau == 1) {
        vertice->coeficiente = 0;
    } else {
        triangulosPossiveis = (vertice->grau * (vertice->grau - 1)) / 2;
        indiceAresta = vertice->ArestaParaProfessor;
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
            auxArestaVertice = auxVertice->ArestaParaProfessor;
            while(auxAresta != NULL) {
                encontrouAresta = 1;
                while(auxArestaVertice != NULL && encontrouAresta) {
                    if (auxAresta->terminal == auxArestaVertice->terminal) {
                        qtdTriangulos++;
                        encontrouAresta = 0;
                    }
                    auxArestaVertice = auxArestaVertice->prox;
                }
                auxArestaVertice = auxVertice->ArestaParaProfessor;
                auxAresta = auxAresta->prox;
            }
            indiceAresta = indiceAresta->prox;
            auxAresta = indiceAresta->prox;
        }
        vertice->coeficiente = qtdTriangulos/triangulosPossiveis;
    }

    printf("Vértice %d - Coeficiente %.2lf\n", vertice->id, vertice->coeficiente);
}
*/

/*
// - Realiza o calculo do coeficiente para cada vertice,
// - Acumula o coeficiente obtido a cada iteracao,
// - Divide o acumulado pela quantidade de vertices no grafo
//   e imprime o resultado.
void obterCoeficienteAglomeracaoGrafo() {
    tVerticeEscola *vertice = grafo->vertice;
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
*/

/*
void mostrarListaOrdTop(){
    tNoOrdTop* tmp = listaOrdTop.inicio;

    printf("\n\n\n");
    while (tmp){
        printf("%s ", tmp->vertice->nome);
        // printf("%i ", tmp->vertice->custoFinalizar);
        tmp = tmp->prox;
    }
}
*/

/*
void liberarListaOrdTop(){
    tNoOrdTop* tmp;
    if(!listaOrdTop.inicio)
        return;

    printf("Liberando memória alocada para lista de ordenação topológica\n");
    do{
        tmp = listaOrdTop.inicio;
        listaOrdTop.inicio = listaOrdTop.inicio->prox;
        
        free(tmp);
    }while(listaOrdTop.inicio);

    listaOrdTop.fim = NULL;
}
*/

/*
int converteGrafoParaArquivoVisual(){
    char *arquivoFonteDot = "grafo.dot";
    char *formatoGrafo = "digraph Disciplinas {\n";
    char *descricao = " c [color=lightblue, label=\"Disciplinas\nCurso: Ciência da Computação\"];\n";
    char comando[200];

    FILE *pArq; 
    tArestaEscola *arestaAux;
    tVerticeEscola *verticeAux = grafo->vertice;
    int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoGrafo);
    fprintf(pArq, descricao);

    for(i = 0; i < grafo->qtdVertices; i++){
        arestaAux = verticeAux->ArestaParaProfessor;

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
*/

/*
int converteOrdTopologicaParaArquivoVisual(){
    char *arquivoFonteDot = "ordenacaoTop.dot";
    char *formatoOrd = "digraph OrdenacaoTopol {\n";
    char *descricao = " c [color=lightblue, label=\"Ordenação Topológica\"]";
    char comando[200];

    FILE *pArq; 
    tNoOrdTop *noOrdTopAux = listaOrdTop.inicio;
    int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoOrd);
    fprintf(pArq, descricao);

    for(i = 0; i < grafo->qtdVertices; i++){

        fprintf(pArq, "%s ;", noOrdTopAux->vertice->nome);
        noOrdTopAux = noOrdTopAux->prox;
    }

    fprintf(pArq,"\n}");
    fclose(pArq);

    sprintf(comando, "dot -Tpng -O '%s'", arquivoFonteDot);
    return system(comando);
}
*/

/*
int converteCaminhoCriticoParaArquivoVisual(){
    char *arquivoFonteDot = "CaminhoCritico.dot";
    char *formatoOrd = "digraph CaminhoCritico {\n";
    char *curso = " c [color=lightblue, label=\"Curso: Ciência da Computação\"];\n";
    char comando[200];
    FILE *pArq; 
    tNoOrdTop *noOrdTopAux = listaOrdTop.inicio;
    tVerticeEscola *tmpVerticeCritico = maiorESegundoMaiorCaminho.Maior;
    // int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoOrd);
    fprintf(pArq, curso);

    do{
        fprintf(pArq, "%s -> %s ;\n",tmpVerticeCritico->VerticeAntMaiorCusto, 
                                   tmpVerticeCritico->nome);
        tmpVerticeCritico = tmpVerticeCritico->VerticeAntMaiorCusto;
    }while(tmpVerticeCritico != tmpVerticeCritico->VerticeAntMaiorCusto);

    fprintf(pArq,"\n}");
    fclose(pArq);

    sprintf(comando, "dot -Tpng -O '%s'", arquivoFonteDot);
    return system(comando);
}
*/

/*
void achaVerticesComMaiorCustoDeChegadaEInsereNaStruct(){
    int i;
    tVerticeEscola *tmpVertice = grafo->vertice;
    tVerticeEscola *verticeCaminhoMaior = grafo->vertice;
    tVerticeEscola *verticeSegundoCaminhoMaior = grafo->vertice;

    for(i=0; i<grafo->qtdVertices;++i){
        if(verticeCaminhoMaior->custoFinalizar <= tmpVertice->custoFinalizar){
            verticeSegundoCaminhoMaior = verticeCaminhoMaior;
            verticeCaminhoMaior = tmpVertice;
            // printf("maior: %s\n", verticeCaminhoMaior->nome);
            // printf("Segundo maior: %s\n", verticeSegundoCaminhoMaior->nome);
        }
        tmpVertice = tmpVertice->prox;
    }

    maiorESegundoMaiorCaminho.Maior = verticeCaminhoMaior;
    maiorESegundoMaiorCaminho.segundoMaior = verticeSegundoCaminhoMaior;
}
*/

/*
void passaCaminhoCriticoParaArquivoVisual(){
    char *arquivoFonteDot = "CaminhoCritico.dot";
    char *formatoOrd = "digraph CaminhoCritico {\n";
    char *descricao = " c [color=lightblue, label=\"Maior Caminho Crítico\"];\n";
    char comando[200];

    FILE *pArq; 
    tVerticeEscola *verticeCritico = maiorESegundoMaiorCaminho.Maior;
    // int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoOrd);
    fprintf(pArq, descricao);

    do{
        fprintf(pArq, "%s -> %s;\n", verticeCritico->VerticeAntMaiorCusto->nome, 
                                     verticeCritico->nome);

        verticeCritico = verticeCritico->VerticeAntMaiorCusto;
    }while(verticeCritico != verticeCritico->VerticeAntMaiorCusto);

    fprintf(pArq,"\n}");
    fclose(pArq);

    sprintf(comando, "dot -Tpng -O '%s'", arquivoFonteDot);
    return system(comando);
}
*/

/*
void passaSegundoCaminhoCriticoParaArquivoVisual(){
    char *arquivoFonteDot = "SegundoCaminhoCritico.dot";
    char *formatoOrd = "digraph SegundoCaminhoCritico {\n";
    char *descricao = " c [color=lightblue, label=\"Segundo Maior Caminho Crítico\"];\n";
    char comando[200];

    FILE *pArq; 
    tVerticeEscola *verticeCritico = maiorESegundoMaiorCaminho.segundoMaior;
    // int i;

    pArq = fopen(arquivoFonteDot, "w");
    fprintf(pArq, formatoOrd);
    fprintf(pArq, descricao);

    do{
        fprintf(pArq, "%s -> %s;\n", verticeCritico->VerticeAntMaiorCusto->nome, 
                                     verticeCritico->nome);

        verticeCritico = verticeCritico->VerticeAntMaiorCusto;
    }while(verticeCritico != verticeCritico->VerticeAntMaiorCusto);

    fprintf(pArq,"\n}");
    fclose(pArq);

    sprintf(comando, "dot -Tpng -O '%s'", arquivoFonteDot);
    return system(comando);
}
*/

void matchEstavel(){
    int i, x;
    verticeEscola *auxVerticeEscola = grafo->verticeEscola;
    verticeProfessor *auxVerticeProfessor = grafo->verticeProfessor;
    tArestaProfessor *auxArestaEscola;

    for(i=0; i<grafo->qtdEscolas; ++i){
        auxVerticeEscola->ProfessorRequisitado1 = NULL;
        auxVerticeEscola->ProfessorRequisitado2 = NULL;
        auxVerticeEscola = auxVerticeEscola->prox;
    }
    auxVerticeEscola = grafo->verticeEscola;

    for(i=0; i<grafo->qtdProfessores; ++i){
        auxVerticeProfessor->escolaQueORecebeu = NULL;
        auxVerticeProfessor->atualEscolaASerAnalizada = auxVerticeProfessor->ArestaParaEscola;
        auxVerticeProfessor = auxVerticeProfessor->prox;
    }
    
    auxVerticeProfessor = grafo->verticeProfessor;
    for(i=0; i < grafo->qtdProfessores; ++i){
        if(auxVerticeProfessor->escolaQueORecebeu != NULL || auxVerticeProfessor->atualEscolaASerAnalizada == NULL){
            auxVerticeProfessor = auxVerticeProfessor->prox;
            continue;
        }
        
        auxArestaEscola = auxVerticeProfessor->atualEscolaASerAnalizada;
        while((auxArestaEscola && auxArestaEscola->atalho->habilitacoesMinimas[0] > auxVerticeProfessor->habilitacao)){
            auxArestaEscola = auxArestaEscola->prox;
        }
        if(auxArestaEscola == NULL){
            auxVerticeProfessor->atualEscolaASerAnalizada = NULL;
            continue;
        }
        if(auxArestaEscola->atalho->ProfessorRequisitado1 == NULL){
            auxArestaEscola->atalho->ProfessorRequisitado1 = auxVerticeProfessor;
            auxVerticeProfessor->escolaQueORecebeu = auxArestaEscola->atalho;
            
            auxVerticeProfessor = auxVerticeProfessor->prox;
        }
        else{
           if((auxArestaEscola->atalho->ProfessorRequisitado1->habilitacao - 
           auxArestaEscola->atalho->habilitacoesMinimas[0] + 1) > 
           (auxVerticeProfessor->habilitacao - auxArestaEscola->atalho->habilitacoesMinimas[0] + 1)){
               auxArestaEscola->atalho->ProfessorRequisitado1->escolaQueORecebeu = NULL;
               auxArestaEscola->atalho->ProfessorRequisitado1 = auxVerticeProfessor;
               auxVerticeProfessor->escolaQueORecebeu = auxArestaEscola->atalho;
               i=0;
               auxVerticeProfessor = grafo->verticeProfessor;
           }
           else{
               auxVerticeProfessor = auxVerticeProfessor->prox;
           }
           
        
        }
            // printf("tem que fazer pra segunda vaga também");
        }

}

// Funcao principal do programa
int main() {
    printf("------ Teoria e Aplicação de Grafos - Projeto 2 ------\n");
    printf("---- João Antonio Desiderio de Moraes (16/0126975) ----\n");
    printf("---- Hiago dos Santos Rabelo (16/0124492) ----\n\n");
    lerArquivo();
    matchEstavel();
    imprimirVerticesProfessores(grafo->verticeProfessor);
    imprimirVerticesEscola(grafo->verticeEscola);
    // zeraListaOrdTop();
    // printf("Realizando ordenacao topologica...");
    // ordenacaoTopologica();
    
    /*
    if (listaOrdTop.ciclo == 0) {
        mostrarListaOrdTop();
        InsereNosVerticesOCustoDeChegada();
    if(converteOrdTopologicaParaArquivoVisual()){
        printf("Não foi possível criar aquivo visual da ordenação topológica\n");
        return EXIT_FAILURE;
    }
    }
    */

    /*
    if(converteGrafoParaArquivoVisual()){
        printf("Não foi possível passar o grafo para o arquivo visual\n");
        return EXIT_FAILURE;
    }
    */

    // achaVerticesComMaiorCustoDeChegadaEInsereNaStruct();
    // passaCaminhoCriticoParaArquivoVisual();
    // passaSegundoCaminhoCriticoParaArquivoVisual();
    // printf(">> Caminhos críticos: \n");
    // printf("Maior: %s\n", maiorESegundoMaiorCaminho.Maior->nome);
    // printf("Segundo Maior: %s\n", maiorESegundoMaiorCaminho.segundoMaior->nome);
    // printf("\nQuestão 2: imprimir todos os cliques maximais\n\n");
    // imprimirCliques();
    // printf("\nQuestão 3: imprimir o coeficiente de aglomeração de cada vértice\n\n");
    // obterCoeficienteAglomeracaoGrafo();
    // printf("\nLiberando a memória alocada para o grafo...\n");
    // liberarGrafo();
    // liberarListaOrdTop(); 
    printf("\nEncerrando...\n\n");
    return 0;
}