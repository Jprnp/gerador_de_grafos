#define CHEIO 71 //71
#define VAZIO 31

#include "uthash/uthash.h"
#include "graphviz/graphviz.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifdef linux

#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>

#endif // linux

//*****************************************************************************
//                          DECLARA합ES GLOBAIS
//*****************************************************************************

int num_vertices = 0;

typedef struct verificados {
    int qtd;
    int lista[0];
}VERF;

int ask_tipo() {
    short int is_correct = 0;
    char tp_grafo_s[5], *tp_grafo_p = "";
    int tp_grafo = 0;

    printf("\n TIPO DE GRAFO\n");
    printf("\n 1 - Grafo cheio;\n");
    printf(" 2 - Grafo vazio;\n");

    while (is_correct == 0) {
        scanf("%s", tp_grafo_s);
        tp_grafo = strtol(tp_grafo_s, &tp_grafo_p, 10);

        if (tp_grafo == 1 || tp_grafo == 2) {
            is_correct = 1;
        } else {
            printf("\nTipo Grafo %d\n", tp_grafo);
            printf("\nSelecionar um valor valido, tente novamente\n");
        }
    }
    printf("\nTipo Grafo %d\n", tp_grafo);
    return tp_grafo;
}

//*****************************************************************************
//                          FUN합ES P/ MATRIZ
//*****************************************************************************

void preenche_matriz(short int m[num_vertices][num_vertices], int p) {
    short int i, u;
    short int is_aresta = 0;

    for (i = 0; i < num_vertices; i++) {
        for (u = i; u < num_vertices; u++) {
            if (u != i)                       //EVITA LIGAÇÃO RECURSIVA
            {
                is_aresta = (rand() % 100) < p;
                m[i][u] = is_aresta;
            } else {
                m[i][u] = 0;
            }
        }
    }

    for (i = 1; i < num_vertices; i++) {
        for (u = 0; u < i; u++) {
            m[i][u] = m[u][i];
        }
    }
}

void printa_matriz(short int m[num_vertices][num_vertices]) {
    short int i, u;
    printf("\n");
    for (i = 0; i < num_vertices; i++) {
        for (u = 0; u < num_vertices; u++) {
            printf("%hd ", m[i][u]);
        }
        printf("\n");
    }
}

int verifica_conexoes(int lin, int cont, short int m[num_vertices][num_vertices]) {
    int col, indices[num_vertices], i = 0, u;
    if (cont == num_vertices) {
        return 1;
    } else {
        for (col = 0; col < num_vertices; col++) {
            if (m[lin][col]) {
                indices[i] = col;
                i++;
            }
        }

        for (u = 0; u < i; u++) {
            if (verifica_conexoes(indices[u], cont + 1, m)) {
                return 1;
            }
        }

        return 0;
    }
}

int is_conexo_matriz( short int m[num_vertices][num_vertices] ) {
    int i, con = 0;

    for (i = 0; i < num_vertices; i++) {
        con = verifica_conexoes(i, 1, m);

        if (con != 1) {
            return 0;
        }
    }

    return 1;
}

//*****************************************************************************
//                          FUN합ES P/ LISTA
//*****************************************************************************

void cria_listas_vazias(struct adj_list l[num_vertices]) {
    int i;
    struct adj_list *e;

    for (i = 0; i < num_vertices; i++) {
        e = &l[i];
        e->cod = i;
        e->prox = NULL;
    }
}

void gera_ligacao(struct adj_list *e, int cod) {
    if (e->cod == cod)   //EVITA LIGAÇÃO RECURSIVA
    {
        return;
    }
    struct adj_list *e1;
    if (e->prox == NULL) {
        e->prox = (struct adj_list *) malloc(sizeof(struct adj_list));
        e1 = e->prox;
        e1->cod = cod;
        e1->prox = NULL;
    } else {
        e1 = e->prox;
        gera_ligacao(e1, cod);
    }
}

int confere_reflexo(struct adj_list *e, int cod) {
    if (e->cod == cod) {
        return 1;
    } else {
        if (e->prox == NULL) {
            return 0;
        } else {
            return confere_reflexo(e->prox, cod);
        }
    }
}

void gera_ligacoes(struct adj_list l[num_vertices], int p) {
    int i, u;
    int is_aresta = 0;
    struct adj_list *e, *e1;

    for (i = 0; i < num_vertices; i++) {
        e = &l[i];
        for (u = 0; u < num_vertices; u++) {
            if (i <= u) {
                is_aresta = (rand() % 100) < p;
                if (is_aresta == 1) {
                    gera_ligacao(e, u);
                }
            } else {
                e1 = &l[u];
                if (confere_reflexo(e1, i) == 1) {
                    gera_ligacao(e, u);
                }
            }
        }
    }
}

void printa_elemento(struct adj_list e) {
    if (e.prox == NULL) {
        printf("%d\n", e.cod);
    } else {
        printf("%d -> ", e.cod);
        printa_elemento(*e.prox);
    }
}

void printa_lista(struct adj_list l[num_vertices]) {
    int i;

    for (i = 0; i < num_vertices; i++) {
        printa_elemento(l[i]);
    }
}

int verifica_conexoes_lista( VERF * verf, struct adj_list l, struct adj_list la[] ) {
    int indices[num_vertices], i = 0, u, u2;
    int contin = 0;
    struct adj_list l1 = l;
    verf->lista[verf->qtd] = l.cod;
    verf->qtd++;
    if (verf->qtd == num_vertices) {
        return 1;
    } else {
        while(l1.prox != NULL) {
            l1 = (*l1.prox);
            indices[i] = l1.cod;
            i++;
        }

        for (u = 0; u < i; u++) {
            l1 = la[indices[u]];
            for (u2 = 0; u2 < verf->qtd; u2++) {
                if (verf->lista[u2] == indices[u]) {
                    contin= 1;
                    break;
                }
            }
            if (contin) {
                contin = 0;
            } else {
                if (verifica_conexoes_lista(verf, l1, la)) {
                    return 1;
                }
            }
        }

        return 0;
    }
}

int is_conexo_lista( struct adj_list l[num_vertices] ) {
    int i;
    VERF * verf;
    verf = malloc((sizeof (*verf)) + num_vertices * sizeof(int));
    verf->qtd = 0;

    return verifica_conexoes_lista(verf, l[0], l);

    return 0;
}

//*****************************************************************************
//                             MAIN
//*****************************************************************************

int main() {
    char answ = 'n';
    int rc;
#ifdef _WIN32
    rc = system("dot -V"); // Comando para verificação da versão do Graphviz
    if (rc != 0) {
        printf("Não foi identificada uma instalação do graphviz.\n");
        printf("ou a variável path não foi configurada\n.");
    }
    printf("Windows\n\n");
#endif // _WIN32
#ifdef linux
    printf("Linux\n");
    rc = system("dot -V"); // Comando para verificação da versão do Graphviz
    if (rc != 0)
    {
        printf("Não foi identificada uma instalação do graphviz,\n");
        printf("deseja realizar a instalação?(s - Sim)\n");
        scanf("%c", &answ);
        if (answ == 's')
        {
            system("sudo apt-get install graphviz");
        }
        printf("\n\n");
    }
#endif // linux
    time_t t;
    srand((unsigned) time(&t));
    int perc;
    int tipo_grafo;
    int is_conexo;
    char title[20];

    fill_colors();

    printf("Insira o num de vertices:\n");
    scanf("%d", &num_vertices);
    num_vertices = abs(num_vertices);

    tipo_grafo = ask_tipo();
    if (tipo_grafo == 1) {
        short int matriz[num_vertices][num_vertices];
        perc = CHEIO;
        preenche_matriz(matriz, perc);
        printa_matriz(matriz);
        is_conexo = is_conexo_matriz( matriz );
        if ( is_conexo ) {
            printf("\nGRAFO CONEXO\n\n");
            strcpy(title, "GRAFO CONEXO");
        } else {
            printf("\nGRAFO DESCONEXO\n\n");
            strcpy(title, "GRAFO DESCONEXO");
        }
        if (answ == 's' || rc == 0) // Só gera o .png se o graphviz tiver instalado
            gera_graphviz_matriz(num_vertices, matriz, title);
    } else {
        struct adj_list adjacencias[num_vertices];
        perc = VAZIO;
        cria_listas_vazias(adjacencias);
        gera_ligacoes(adjacencias, perc);
        printa_lista(adjacencias);
        is_conexo = is_conexo_lista( adjacencias );
        if ( is_conexo ) {
            printf("\nGRAFO CONEXO\n\n");
            strcpy(title, "GRAFO CONEXO");
        } else {
            printf("\nGRAFO DESCONEXO\n\n");
            strcpy(title, "GRAFO DESCONEXO");
        }
        if (answ == 's' || rc == 0) // Só gera o .png se o graphviz tiver instalado
            gera_graphviz_lista(num_vertices, adjacencias, title);
    }

    return (0);
}
