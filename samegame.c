#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "ranking.h"

#define LINHAS 10
#define COLUNAS 7
#define NUM_CORES 5
#define VAZIO 0

int grade[LINHAS][COLUNAS];

char *cores[] = {"", "\033[31m", "\033[32m", "\033[33m", "\033[34m", "\033[35m"};

void inicializar() {
    for (int l = 0; l < LINHAS; l++)
        for (int c = 0; c < COLUNAS; c++)
            grade[l][c] = rand() % NUM_CORES + 1;
}

void desenhar() {
    printf("\n   ");
    for (int c = 0; c < COLUNAS; c++)
        printf("%2d ", c);
    printf("\n");

    for (int l = 0; l < LINHAS; l++) {
        printf("%2d ", l);
        for (int c = 0; c < COLUNAS; c++) {
            if (grade[l][c] == VAZIO)
                printf(" . ");
            else
                printf(" %s%c\033[0m ", cores[grade[l][c]], 'A' + grade[l][c] - 1);
        }
        printf("\n");
    }
    printf("\n");
}

int flood(int lin, int col, int cor, int visitado[LINHAS][COLUNAS]) {
    if (lin < 0 || lin >= LINHAS || col < 0 || col >= COLUNAS) return 0;
    if (visitado[lin][col]) return 0;
    if (cor == VAZIO) return 0;
    if (grade[lin][col] != cor) return 0;

    visitado[lin][col] = 1;

    return 1 + flood(lin - 1, col, cor, visitado)
             + flood(lin + 1, col, cor, visitado)
             + flood(lin, col - 1, cor, visitado)
             + flood(lin, col + 1, cor, visitado);
}

int remover_grupo(int lin, int col) {
    int visitado[LINHAS][COLUNAS] = {0};

    if (lin < 0 || lin >= LINHAS || col < 0 || col >= COLUNAS) return 0;
    if (grade[lin][col] == VAZIO) return 0;

    int tamanho = flood(lin, col, grade[lin][col], visitado);
    if (tamanho < 2) return 0;

    for (int l = 0; l < LINHAS; l++)
        for (int c = 0; c < COLUNAS; c++)
            if (visitado[l][c])
                grade[l][c] = VAZIO;

    return tamanho;
}

void aplicar_gravidade() {
    for (int c = 0; c < COLUNAS; c++) {
        int destino = LINHAS - 1;

        for (int l = LINHAS - 1; l >= 0; l--) {
            if (grade[l][c] != VAZIO) {
                grade[destino][c] = grade[l][c];
                destino--;
            }
        }
        while (destino >= 0) {
            grade[destino][c] = VAZIO;
            destino--;
        }
    }
}

void compactar_colunas() {
    int destino = 0;

    for (int c = 0; c < COLUNAS; c++) {
        int vazia = 1;
        for (int l = 0; l < LINHAS; l++)
            if (grade[l][c] != VAZIO) vazia = 0;

        if (vazia) continue;

        if (destino != c) {
            for (int l = 0; l < LINHAS; l++) {
                grade[l][destino] = grade[l][c];
                grade[l][c] = VAZIO;
            }
        }
        destino++;
    }
}

int tem_jogada() {
    int visitado[LINHAS][COLUNAS] = {0};

    for (int l = 0; l < LINHAS; l++) {
        for (int c = 0; c < COLUNAS; c++) {
            if (grade[l][c] == VAZIO) continue;
            if (visitado[l][c]) continue;
            if (flood(l, c, grade[l][c], visitado) >= 2) return 1;
        }
    }
    return 0;
}

int grade_vazia() {
    for (int l = 0; l < LINHAS; l++)
        for (int c = 0; c < COLUNAS; c++)
            if (grade[l][c] != VAZIO) return 0;
    return 1;
}

int jogar_partida() {
    int pontos = 0;
    int lin, col;
    char entrada[50];

    inicializar();

    printf("Remova grupos de 2 ou mais pecas da mesma cor ligadas na horizontal ou vertical.\n");
    printf("Um grupo de n pecas vale n*(n-1) pontos.\n");
    printf("Digite a linha e a coluna (exemplo: 3 5), ou q para encerrar a partida.\n");

    while (tem_jogada()) {
        desenhar();
        printf("Pontos: %d\n", pontos);
        printf("Sua jogada (linha coluna): ");

        if (fgets(entrada, sizeof(entrada), stdin) == NULL) break;
        if (entrada[0] == 'q' || entrada[0] == 'Q') break;

        if (sscanf(entrada, "%d %d", &lin, &col) != 2) {
            printf("Digite dois numeros: a linha e a coluna.\n");
            continue;
        }
        if (lin < 0 || lin >= LINHAS || col < 0 || col >= COLUNAS) {
            printf("Fora da grade. As linhas vao de 0 a %d e as colunas de 0 a %d.\n",
                   LINHAS - 1, COLUNAS - 1);
            continue;
        }
        if (grade[lin][col] == VAZIO) {
            printf("Essa posicao esta vazia.\n");
            continue;
        }

        int removidas = remover_grupo(lin, col);
        if (removidas == 0) {
            printf("Essa peca nao tem nenhum vizinho da mesma cor.\n");
            continue;
        }

        aplicar_gravidade();
        compactar_colunas();
        pontos = pontos + removidas * (removidas - 1);
        printf("Removeu %d pecas.\n", removidas);
    }

    desenhar();
    printf("Fim de jogo!\n");
    if (grade_vazia()) {
        pontos = pontos + 1000;
        printf("Voce limpou a grade inteira! Bonus de 1000 pontos.\n");
    }
    printf("Pontuacao final: %d\n", pontos);
    return pontos;
}

void testar_ranking() {
    Jogador *ranking = NULL;

    ranking = inserir_ordenado(ranking, "Ana", 50);   /* lista vazia      */
    ranking = inserir_ordenado(ranking, "Beto", 120); /* vira o primeiro  */
    ranking = inserir_ordenado(ranking, "Caio", 10);  /* vai para o fim   */
    ranking = inserir_ordenado(ranking, "Duda", 80);  /* fica no meio     */
    ranking = inserir_ordenado(ranking, "Eva", 50);   /* empata com a Ana */

    /* ordem esperada: Beto 120, Duda 80, Ana 50, Eva 50, Caio 10 */
    Jogador *p = ranking;
    assert(strcmp(p->nome, "Beto") == 0); p = p->proximo;
    assert(strcmp(p->nome, "Duda") == 0); p = p->proximo;
    assert(strcmp(p->nome, "Ana") == 0);  p = p->proximo;
    assert(strcmp(p->nome, "Eva") == 0);  p = p->proximo;
    assert(strcmp(p->nome, "Caio") == 0); p = p->proximo;
    assert(p == NULL);

    mostrar_ranking(ranking);
    liberar_ranking(ranking);
    printf("Todos os testes passaram.\n");
}

int main(int argc, char *argv[]) {
    Jogador *ranking = NULL;
    char nome[TAM_NOME];
    char resposta[10];

    if (argc > 1 && strcmp(argv[1], "--teste") == 0) {
        testar_ranking();
        return 0;
    }

    srand(time(NULL));
    printf("=== SameGame ===\n");

    while (1) {
        printf("\nNome do jogador: ");
        if (fgets(nome, sizeof(nome), stdin) == NULL) break;
        nome[strcspn(nome, "\n")] = '\0';  
        if (nome[0] == '\0') strcpy(nome, "Anonimo");

        int pontos = jogar_partida();
        ranking = inserir_ordenado(ranking, nome, pontos);
        mostrar_ranking(ranking);

        printf("Jogar outra partida? (s/n): ");
        if (fgets(resposta, sizeof(resposta), stdin) == NULL) break;
        if (resposta[0] != 's' && resposta[0] != 'S') break;
    }

    liberar_ranking(ranking);
    printf("Ate a proxima!\n");
    return 0;
}
