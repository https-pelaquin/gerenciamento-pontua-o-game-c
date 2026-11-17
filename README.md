# Bruno Henrique Pelaquin Ferreira RA: 2209748

</br>

## SameGame: Ranking com Lista Encadeada

Atividade de Estrutura de Dados em C. Continua o **SameGame** da Atividade
Avaliativa 1 e acrescenta uma estrutura dinâmica que guarda a pontuação dos
jogadores ao longo de várias partidas e mostra o resultado em forma de ranking.

---

## 1. Objetivo

Guardar, a cada partida, o **nome do jogador** e a **pontuação obtida**, sem
usar vetor de tamanho fixo, e mostrar todos os resultados **ordenados** da
maior para a menor pontuação.

---

## 2. Estrutura escolhida: lista simplesmente encadeada ordenada

Das três opções permitidas (lista, pilha e fila), a lista é a única que deixa
inserir um elemento **no meio**. Pilha e fila só inserem e removem nas pontas,
então para mostrar um ranking com elas seria preciso copiar tudo para outro
lugar e ordenar depois.

Com a lista, o novo jogador **já entra na posição certa**. A lista fica sempre
ordenada, e mostrar o ranking é só percorrer do início ao fim.

### O nó

```c
typedef struct Jogador {
    char nome[TAM_NOME];       /* nome do jogador          */
    int pontos;                /* pontuação da partida     */
    struct Jogador *proximo;   /* próximo nó, ou NULL      */
} Jogador;
```

Cada nó é criado com `malloc` no momento em que a partida termina. Não existe
limite de partidas: a lista cresce enquanto houver memória.

### Como a inserção ordenada funciona

Exemplo: a lista tem `Beto 120 → Ana 50 → Caio 10` e entra `Duda 80`.

```
inicio
  │
  ▼
[Beto 120] ──► [Ana 50] ──► [Caio 10] ──► NULL
     ▲
   atual  (o próximo, Ana 50, é menor que 80: para aqui)

novo->proximo = atual->proximo;   Duda aponta para Ana
atual->proximo = novo;            Beto aponta para Duda

[Beto 120] ──► [Duda 80] ──► [Ana 50] ──► [Caio 10] ──► NULL
```

Há dois casos:

| Caso | O que acontece |
|---|---|
| Lista vazia, ou o novo é maior que o primeiro | O novo vira o início da lista |
| Qualquer outro | Anda até o último nó com pontos `>=` aos do novo e encaixa depois dele |

Por usar `>=`, em caso de **empate** quem fez a pontuação primeiro continua na
frente.

---

## 3. Arquitetura

```
samegame.c   o jogo (Atividade 1) + o laço de várias partidas
ranking.h    o struct Jogador e as declarações das funções da lista
ranking.c    a implementação da lista encadeada
```

A explicação linha a linha de todo o código está em
[EXPLICACAO.md](EXPLICACAO.md).

A lista ficou em arquivos separados porque ela não depende do jogo: as três
funções só recebem nome e pontos. O `samegame.c` usa a lista sem precisar saber
como ela funciona por dentro.

### Funções da lista ([ranking.c](ranking.c))

| Função | Linha | O que faz |
|---|---|---|
| `inserir_ordenado()` | 10 | Cria o nó com `malloc` e o encaixa na posição certa. Devolve o novo início |
| `mostrar_ranking()` | 39 | Percorre a lista e imprime posição, nome e pontos |
| `liberar_ranking()` | 57 | Percorre a lista dando `free` em cada nó |

`inserir_ordenado` **devolve** o início da lista em vez de receber um ponteiro
para ponteiro (`Jogador **`). As duas formas funcionam. A escolhida é mais
fácil de ler: `ranking = inserir_ordenado(ranking, nome, pontos);`.

### Mudanças no jogo ([samegame.c](samegame.c))

| Função | Linha | Mudança |
|---|---|---|
| `jogar_partida()` | 146 | É o antigo `main`: joga uma partida e **devolve** a pontuação |
| `testar_ranking()` | 202 | Teste com `assert` da lista (nova) |
| `main()` | 225 | Pede o nome, chama `jogar_partida`, insere na lista, mostra o ranking e pergunta se quer jogar de novo |

O fluxo do `main`:

```
ranking = NULL
repete:
    lê o nome do jogador
    pontos  = jogar_partida()
    ranking = inserir_ordenado(ranking, nome, pontos)
    mostrar_ranking(ranking)
    jogar de novo? se não, sai
liberar_ranking(ranking)
```

Toda a lógica do jogo (flood fill recursivo, gravidade, compactação de
colunas) continua igual à da Atividade 1.

> **Correção feita no jogo:** a versão final da Atividade 1 não compilava,
> porque o `main` chamava a função `autoteste()`, que tinha sido removida. A
> chamada foi trocada por `testar_ranking()`, e `limpar_grade()`, que só o
> autoteste usava, foi removida.

---

## 4. Requisitos da atividade

| Requisito | Onde é atendido |
|---|---|
| Estrutura dinâmica (lista, pilha ou fila) | Lista simplesmente encadeada, `ranking.c` |
| Nome e pontuação do jogador | Campos `nome` e `pontos` do `struct Jogador` |
| Cadastrar ao longo das partidas | `main`: um nó novo ao fim de cada partida |
| Percorrer e apresentar | `mostrar_ranking()` |
| Pontuações ordenadas | `inserir_ordenado()` mantém a lista em ordem decrescente |
| Alocação dinâmica | `malloc` em `inserir_ordenado`, `free` em `liberar_ranking` |
| Sem vetor de tamanho fixo no lugar da estrutura | Os jogadores ficam só nos nós da lista |
| Somente C | Só a biblioteca padrão (`stdio.h`, `stdlib.h`, `string.h`, `time.h`, `assert.h`) |

---

## 5. Compilação e execução

Requisito: GCC (`sudo apt install build-essential`).

```bash
gcc -Wall -Wextra -std=c11 samegame.c ranking.c -o samegame
```

Agora são dois arquivos `.c`, e os dois precisam ir no comando. O código compila
com **zero avisos**.

```bash
./samegame
```

Teste automático da lista:

```bash
./samegame --teste
```

Saída esperada:

```
===== RANKING =====
 1. Beto                    120 pontos
 2. Duda                     80 pontos
 3. Ana                      50 pontos
 4. Eva                      50 pontos
 5. Caio                     10 pontos
===================
Todos os testes passaram.
```

O teste cobre os cinco casos de inserção: lista vazia, novo primeiro lugar,
último lugar, meio da lista e empate.

---

## 6. Demonstração

Três partidas seguidas (Bruno, Maria e João). Ao fim de cada uma o ranking é
mostrado, e o novo jogador já aparece na posição correta:

```
Nome do jogador: Bruno
   ... partida ...
Fim de jogo!
Pontuacao final: 212

===== RANKING =====
 1. Bruno                   212 pontos
===================
Jogar outra partida? (s/n): s

Nome do jogador: Maria
   ... partida ...
Pontuacao final: 32

===== RANKING =====
 1. Bruno                   212 pontos
 2. Maria                    32 pontos
===================
Jogar outra partida? (s/n): s

Nome do jogador: Joao
   ... partida ...
Pontuacao final: 58

===== RANKING =====
 1. Bruno                   212 pontos
 2. Joao                     58 pontos
 3. Maria                    32 pontos
===================
Jogar outra partida? (s/n): n
Ate a proxima!
```

O João fez 58 pontos e entrou **no meio** da lista, entre Bruno e Maria. Esse é
o caso 2 da inserção.

<!-- Espaço para prints da execução -->

### Verificação de memória

O programa foi compilado com o AddressSanitizer do GCC e executado com as três
partidas acima. Não houve vazamento nem acesso inválido de memória:

```bash
gcc -std=c11 -g -fsanitize=address,undefined samegame.c ranking.c -o samegame_asan
./samegame_asan
```

---

## 7. Conclusão

A lista simplesmente encadeada resolve o problema com três funções curtas. Como
a ordenação acontece **na inserção**, o ranking nunca precisa ser reordenado:
cada novo resultado custa no máximo uma passada pela lista (O(n)), e mostrar o
ranking também é O(n).

Limitações conhecidas:

- O ranking existe só enquanto o programa está aberto. Para guardá-lo entre
  execuções seria preciso gravar a lista em arquivo, o que ficou fora do
  escopo da atividade.
- Cada partida gera uma entrada nova, então o mesmo jogador pode aparecer
  várias vezes, como em um placar de fliperama.
