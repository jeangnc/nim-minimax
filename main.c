#define K 4
#define M 20
#define FIRST_PLAYER -1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game_tree.c"

const int JOGO_FINALIZADO = 1;
const int JOGADA_BEM_SUCEDIDA = 2;
const int JOGADA_INVALIDA = 3;

struct Node * tree;
int currentPlayer = FIRST_PLAYER;

char* playerName(int playerNumber) {
    return playerNumber == -1 ? "PC" : "Humano";
}

void switchPlayer() {
    currentPlayer *= -1;
}

int storeMove(int removed) {
    tree = tree->children[removed - 1];
    printf("%s tira %d palitos, restam %d\n", playerName(currentPlayer), removed, tree->remaining);
    if (tree->remaining == 0){
        printf("O jogador %s ganhou o jogo!", playerName(currentPlayer));
        return JOGO_FINALIZADO;
    }
    switchPlayer();
    return JOGADA_BEM_SUCEDIDA;
}

int seekBestPlay() {
    int minKey, maxKey;
    int currentMin = 500, currentMax = -500;
    for(int i=0; i<K && i < tree->remaining;i++) {
        struct Node *childNode = tree->children[i];
        printf("Opção %d com minimax %d\n", i+1, childNode->minimax, playerName(childNode->currentPlayer));
        if(childNode->minimax < currentMin) {
            currentMin = childNode->minimax;
            minKey = i;
        }
        if(childNode->minimax > currentMax) {
            currentMax = childNode->minimax;
            maxKey = i;
        }
    }
    return (currentPlayer == -1 ? minKey : maxKey) + 1;
}

int humanInput() {
    int removed;
    do{
        seekBestPlay();
        printf("Quandos palitos deseja tirar? Número: ");
        scanf("%d", &removed);
        if (removed > K || removed < 0){
            printf("Jogada inválida. Escolha entre 1 e %d palito(s)\n", K);
            continue;
        }
        if (tree->remaining - removed < 0){
            printf("Jogada inválida. Restam apenas %d palito(s)\n", tree->remaining);
            continue;
        }
        break;
    } while (true);
    return removed;
}

int main() {
    printf("Configuração do jogo: jogada máxima %d e número de palitos %d\n\n", K, M);

    tree = buildGameTree(M, FIRST_PLAYER);
    while (true){
        if (storeMove(seekBestPlay()) == JOGO_FINALIZADO) break;
        printf("\n");
        if (storeMove(humanInput()) == JOGO_FINALIZADO) break;
        printf("\n\n");
    }
}
