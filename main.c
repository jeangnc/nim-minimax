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

char* player_name(int playerNumber) {
    return playerNumber == -1 ? "PC" : "Human";
}

void switch_player() {
    currentPlayer *= -1;
}

int store_move(int removed) {
    tree = tree->children[removed - 1];
    printf("%s takes %d sticks, %d left\n", player_name(currentPlayer), removed, tree->remaining);
    if (tree->remaining == 0){
        printf("%s won!", player_name(currentPlayer));
        return FINISHED;
    }
    switch_player();
    return VALID_MOVE;
}

int seek_best_move() {
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

int human_input() {
    int removed;
    do{
        seek_best_move();
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
        if (store_move(seek_best_move()) == FINISHED) break;
        if (store_move(human_input()) == FINISHED) break;
    }
}
