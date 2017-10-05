#define CHEIO 71
#define VAZIO 31

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

#include "uthash.h"

//*****************************************************************************
//                          DECLARA합ES GLOBAIS
//*****************************************************************************

int num_vertices = 0;
int is_windows = 0;

struct col {
    int id;            /* we'll use this field as the key */
    char cor[15];
    UT_hash_handle hh; /* makes this structure hashable */
};

struct col *colors = NULL;

struct adj_list {
    int cod;
    struct adj_list *prox;
};

//*****************************************************************************
//                          FUN합ES P/ GRAPHVIZ
//*****************************************************************************

void add_color(int color_id, char *cor) {
    struct col *s;
    HASH_FIND_INT(colors, &color_id, s);
    if (s == NULL) {
        s = (struct col *) malloc(sizeof(struct col));
        s->id = color_id;
        strcpy(s->cor, cor);
        HASH_ADD_INT(colors, id, s);
    }
}

struct col *find_color(int color_id) {
    struct col *s;

    HASH_FIND_INT(colors, &color_id, s);
    return s;
}

void fill_colors() {
    int i;
    char cor[25];
    for (i = 1; i <= 10; i++) {
        switch (i) {
            case 1:
                strcpy(cor, "black");
                break;
            case 2:
                strcpy(cor, "blue");
                break;
            case 3:
                strcpy(cor, "brown1");
                break;
            case 4:
                strcpy(cor, "cadetblue1");
                break;
            case 5:
                strcpy(cor, "chartreuse");
                break;
            case 6:
                strcpy(cor, "crimson");
                break;
            case 7:
                strcpy(cor, "darkolivegreen1");
                break;
            case 8:
                strcpy(cor, "darkorchid");
                break;
            case 9:
                strcpy(cor, "deeppink");
                break;
            case 10:
                strcpy(cor, "gold");
                break;
            default:
                break;
        }
        add_color(i, cor);
    }
}

/**
* Função para gerar o .png do grafo gerado quando o tipo de grafo for
* CHEIO.
**/
void gera_graphviz_matriz(short int m[num_vertices][num_vertices]) {
    FILE *fp;
    char *homepath = "";
    short int i, u;
    int id;
    struct col *color;
    char buf[200];

    //fp = fopen("../gerados/grafo1.gv", "w");
    if (is_windows == 1) {
        homepath = getenv("homepath");
        if (homepath == NULL) {
            return;
        }
    }
    char teste[10];
    strcpy(teste, "teste");
#ifdef linux
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    strcpy(homepath, homedir);
#endif // linux

    char sep[2];
    if (is_windows == 1) {
        strcpy(sep, "\\");
    } else {
        strcpy(sep, "/");
    }
    int suffix = 0, fd;
    sprintf(buf, "mkdir %s%sgrafosgerados", homepath, sep);
    system(buf);
    do {
        suffix++;
        sprintf(buf, "%s%sgrafosgerados%sgrafo%d.gv", homepath, sep, sep, suffix);
        fd = open(buf, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    } while (fd < 0);
    fp = fopen(buf, "w");
    printf("graph G1 {\n");
    fputs("graph G1 {\n", fp);
    for (i = 0; i < num_vertices; i++) {
        id = (i + 1) % 10;
        if ((i % 10) == 9) {
            id = 10;
        }
        color = find_color(id);
        printf("\tedge [color = ""%s"", fontcolor = ""%s""];\n",
               color->cor, color->cor);
        sprintf(buf, "\tedge [color = ""%s"", fontcolor = ""%s""];\n",
                color->cor, color->cor);
        fputs(buf, fp);

        printf("\t%hd [color = ""%s"", fontcolor = ""%s""];\n",
               i, color->cor, color->cor);
        sprintf(buf, "\t%hd [color = ""%s"", fontcolor = ""%s""];\n",
                i, color->cor, color->cor);
        fputs(buf, fp);
        for (u = i; u < num_vertices; u++) {
            if (m[i][u] == 1) {
                printf("\t%hd -- %hd;\n", i, u);
                sprintf(buf, "\t%hd -- %hd;\n", i, u);
                fputs(buf, fp);
            }
        }
    }
    printf("}\n");
    fputs("}\n", fp);
    fclose(fp);
    sprintf(buf,
            "dot -Tpng %s%sgrafosgerados%sgrafo%d.gv -o %s%sgrafosgerados%sgrafo%d.png",
            homepath, sep, sep, suffix, homepath, sep, sep, suffix);
    system(buf);
    if (is_windows == 1) {
        sprintf(buf, "%s%sgrafosgerados%sgrafo%d.png", homepath, sep, sep, suffix);
    } else {
        sprintf(buf, "eog %s%sgrafosgerados%sgrafo%d.png", homepath, sep, sep, suffix);
    }
    system(buf);
}

void graphviz_item_lista(FILE *fp, struct adj_list *e) {
    struct adj_list *e1;
    char buf[200];

    if (e->prox == NULL) {
        printf("\t%d;\n", e->cod);
        sprintf(buf, "\t%d;\n", e->cod);
        fputs(buf, fp);
        return;
    }

    e1 = e->prox;

    while (1) {
        printf("\t%d -- %d;\n", e->cod, e1->cod);
        sprintf(buf, "\t%d -- %d;\n", e->cod, e1->cod);
        fputs(buf, fp);

        if (e1->prox == NULL) {
            break;
        } else {
            e1 = e1->prox;
        }
    }
}

/**
* Função para gerar o .png do grafo gerado quando o tipo de grafo for
* VAZIO.
**/
void gera_graphviz_lista(struct adj_list l[num_vertices]) {
    FILE *fp;
    char *homepath = "";
    int i, id;
    struct col *color;
    char buf[200];

    if (is_windows == 1) {
        homepath = getenv("homepath");
        if (homepath == NULL) {
            return;
        }
    }
    char teste[10];
    strcpy(teste, "teste");
#ifdef linux
    struct passwd *pw = getpwuid(getuid());
    const char *homedir = pw->pw_dir;
    strcpy(homepath, homedir);
#endif // linux

    char sep[2];
    if (is_windows == 1) {
        strcpy(sep, "\\");
    } else {
        strcpy(sep, "/");
    }

    sprintf(buf, "mkdir %s%sgrafosgerados", homepath, sep);
    system(buf);
    int suffix = 0, fd;
    do {
        suffix++;
        sprintf(buf, "%s%sgrafosgerados%sgrafo%d.gv", homepath, sep, sep, suffix);
        fd = open(buf, O_CREAT | O_WRONLY | O_EXCL, S_IRUSR | S_IWUSR);
    } while (fd < 0);
    fp = fopen(buf, "w");
    printf("strict graph G1 {\n");
    fputs("strict graph G1 {\n", fp);

    for (i = 0; i < num_vertices; i++) {
        id = (i + 1) % 10;
        if ((i % 10) == 9) {
            id = 10;
        }
        color = find_color(id);
        printf("\tedge [color = ""%s"", fontcolor = ""%s""];\n",
               color->cor, color->cor);
        sprintf(buf, "\tedge [color = ""%s"", fontcolor = ""%s""];\n",
                color->cor, color->cor);
        fputs(buf, fp);

        printf("\t%hd [color = ""%s"", fontcolor = ""%s""];\n",
               i, color->cor, color->cor);
        sprintf(buf, "\t%hd [color = ""%s"", fontcolor = ""%s""];\n",
                i, color->cor, color->cor);
        fputs(buf, fp);
        graphviz_item_lista(fp, &l[i]);
    }

    printf("}\n");
    fputs("}\n", fp);
    fclose(fp);
    sprintf(buf,
            "dot -Tpng %s%sgrafosgerados%sgrafo%d.gv -o %s%sgrafosgerados%sgrafo%d.png",
            homepath, sep, sep, suffix, homepath, sep, sep, suffix);
    system(buf);
    if (is_windows == 1) {
        sprintf(buf, "%s%sgrafosgerados%sgrafo%d.png", homepath, sep, sep, suffix);
    } else {
        sprintf(buf, "eog %s%sgrafosgerados%sgrafo%d.png", homepath, sep, sep, suffix);
    }
    system(buf);
}

//*****************************************************************************
//                          FUN합ES GERAIS
//*****************************************************************************

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

typedef struct vazios {
    int * lin;
    int * col;
    int num_lin;
    int num_col;
} VAZ;

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

VAZ get_vazios( short int m[num_vertices][num_vertices] ) {
    VAZ vazios;
    vazios.lin = malloc(num_vertices * sizeof *vazios.lin);
    vazios.col = malloc(num_vertices * sizeof *vazios.col);
    short int i, u, is_vazio;
    int num_lin = 0, num_col = 0;
    int lin[num_vertices], col[num_vertices];

    for (i = 0; i < num_vertices - 1; i++) {
        is_vazio = 1;
        for (u = i + 1; u < num_vertices; u++) {
            if (m[i][i] == 1) {
                is_vazio = 0;
                break;
            }
        }
        if (is_vazio == 1) {
            lin[num_lin] = i;
            num_lin++;
        }
    }

    vazios.lin = lin;
    vazios.num_lin = num_lin;

    return vazios;
}

int is_conexo_matriz( short int m[num_vertices][num_vertices] ) {
    VAZ vazios = get_vazios( m );
    int i;

    for (i = 0; i < vazios.num_lin; i++) {
        printf("\n%d\n", vazios.lin[i]);
    }

    printf("\n------------------------------------------\n");

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
    is_windows = 1;
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
        is_conexo_matriz( matriz );
        if (answ == 's' || rc == 0) // Só gera o .png se o graphviz tiver instalado
            gera_graphviz_matriz(matriz);
    } else {
        struct adj_list adjacencias[num_vertices];
        perc = VAZIO;
        cria_listas_vazias(adjacencias);
        gera_ligacoes(adjacencias, perc);
        printa_lista(adjacencias);
        if (answ == 's' || rc == 0) // Só gera o .png se o graphviz tiver instalado
            gera_graphviz_lista(adjacencias);
    }

    return (0);
}
