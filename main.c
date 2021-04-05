#define K 3
#define M 10
#define FIRST_PLAYER -1

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "game_tree.c"

const int FINISHED = 1;
const int VALID_MOVE = 2;

struct Node * tree;
int currentPlayer = FIRST_PLAYER;

char* playerName(int playerNumber) {
    return playerNumber == -1 ? "PC" : "Human";
}

void switchPlayer() {
    currentPlayer *= -1;
}

int storeMove(int removed) {
    tree = tree->children[removed - 1];
    printf("%s takes %d sticks, %d left\n", playerName(currentPlayer), removed, tree->remaining);
    if (tree->remaining == 0){
        printf("%s won!", playerName(currentPlayer));
        return FINISHED;
    }
    switchPlayer();
    return VALID_MOVE;
}

int seekBestPlay() {
    int minKey, maxKey;
    int currentMin = 500, currentMax = -500;
    for(int i=0; i<K && i < tree->remaining;i++) {
        struct Node *childNode = tree->children[i];
        printf("%d option has score %d\n", i+1, childNode->minimax);
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
        printf("How many sticks do you want to take? Number (max %d): ", K);
        scanf("%d", &removed);
        if (removed > K || removed < 0){
            printf("Invalid play.\n");
            continue;
        }
        if (tree->remaining - removed < 0){
            printf("Inlvaid play. Only %d sticks remaining.\n", tree->remaining);
            continue;
        }
        break;
    } while (true);
    return removed;
}

int main() {
    printf("Game configuration: max %d and %d sticks.\n\n", K, M);
    tree = parallel_game_tree();
    while (true){
        if (storeMove(seekBestPlay()) == FINISHED) break;
        if (storeMove(humanInput()) == FINISHED) break;
    }
}
