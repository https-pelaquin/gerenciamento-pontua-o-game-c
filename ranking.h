#ifndef RANKING_H
#define RANKING_H

#define TAM_NOME 50

typedef struct Jogador {
    char nome[TAM_NOME];
    int pontos;
    struct Jogador *proximo;
} Jogador;

Jogador *inserir_ordenado(Jogador *inicio, const char *nome, int pontos);
void mostrar_ranking(Jogador *inicio);
void liberar_ranking(Jogador *inicio);

#endif
