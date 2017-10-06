#ifndef GRAPHVIZ_LIBRARY_H
#define GRAPHVIZ_LIBRARY_H

struct adj_list {
    int cod;
    struct adj_list *prox;
};

void add_color(int, char*);

struct col *find_color(int);

void fill_colors();

void gera_graphviz_matriz(int, short int[][], char*);

void gera_graphviz_lista(int, struct adj_list[], char*);

#endif