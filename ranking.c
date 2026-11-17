#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ranking.h"

Jogador *inserir_ordenado(Jogador *inicio, const char *nome, int pontos) {
    Jogador *novo = malloc(sizeof(Jogador));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente.\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    novo->pontos = pontos;
    novo->proximo = NULL;

    if (inicio == NULL || pontos > inicio->pontos) {
        novo->proximo = inicio;
        return novo;
    }

    Jogador *atual = inicio;
    while (atual->proximo != NULL && atual->proximo->pontos >= pontos)
        atual = atual->proximo;

    novo->proximo = atual->proximo;
    atual->proximo = novo;
    return inicio;
}

void mostrar_ranking(Jogador *inicio) {
    printf("\n===== RANKING =====\n");
    if (inicio == NULL) {
        printf("Nenhuma partida registrada.\n");
        return;
    }

    int posicao = 1;
    for (Jogador *atual = inicio; atual != NULL; atual = atual->proximo) {
        printf("%2d. %-20s %6d pontos\n", posicao, atual->nome, atual->pontos);
        posicao++;
    }
    printf("===================\n");
}

void liberar_ranking(Jogador *inicio) {
    while (inicio != NULL) {
        Jogador *proximo = inicio->proximo;
        free(inicio);
        inicio = proximo;
    }
}
