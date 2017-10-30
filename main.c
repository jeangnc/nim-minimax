#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define M 13
#define K 4

char player[] = "MAX";

struct Node{
    int cubesRemaining;
    struct Node *left;
    struct Node *right;
}; 

int minimum(int a, int b){
    if (a < b){
        return a;
    }
    else{
        return b;
    }
}

int maximum(int a, int b){
    if (a > b){
        return a;
    }
    else{
        return b;
    }
}

char switchPlayer(){
    if (strcmp(player, "MAX") == 0){
        strcpy(player, "MIN");
    }
    else{
        strcpy(player, "MAX");
    }
}

struct Node *buildGameTree(int ncubes){
    struct Node *cube = calloc(1, sizeof(struct Node));
    cube->cubesRemaining = ncubes;

    if (cube->cubesRemaining >= 1){
        cube->left = buildGameTree(ncubes - 1);
        switchPlayer();
    }
    if (cube->cubesRemaining >= M){
        cube->right = buildGameTree(ncubes - K);
        switchPlayer();
    }

    return (cube);
}

int computeMinimax(struct Node *n){
    int value;

    if (n->cubesRemaining == 0){
        if (strcmp(player, "MIN") == 0){
            return 1;
        }
        else{
            return -1;
        }
    }
    else{
        if (strcmp(player, "MIN") == 0){
            value = minimum(-1, computeMinimax(n->left));
            if (n->right != NULL){
                value = minimum(value, computeMinimax(n->right));
            }
        }
        else{
            value = maximum(1, computeMinimax(n->left));
            if (n->right != NULL){
                value = maximum(value, computeMinimax(n->right));
            }
        }
    }

    return value;

} 

int main(){
    struct Node *root = buildGameTree(M);

    printf("WELCOME TO THE GAME\n");

    while (true){
        int takenCubes = 0;
        int v1,v2;

        v1 = computeMinimax(root->left);
        if (root->right != NULL){
            v2 = computeMinimax(root->right);
        }
        else{
            v2 = 2;
        }

        if (v1 < v2){
            takenCubes = 1;
        }
        else{
            takenCubes = K;
        }


        if (takenCubes == 1){
            root = root->left;
        }
        if (takenCubes == K){
            root = root->right;
        }

        printf("MAX player(PC) takes %d cubes, remaining %d cubes\n", takenCubes, root->cubesRemaining);

        if (root->cubesRemaining == 0){
            printf("MAX player(PC) wins the game!");
            break;
        }


        do{
            printf("How many cubes do you want? ");

            takenCubes = scanf("%d", &takenCubes);

            if (takenCubes >= 1 && takenCubes <= K && root->cubesRemaining - takenCubes >= 0){
                break;
            }

            printf("That's an illegal move. Choose 1 or %d matches.", K);

        } while (true);

        if (takenCubes == 1){
            root = root->left;
        }
        if (takenCubes == K){
            root = root->right;
        }

        printf("MIN player(human) takes %d cubes, leaving %d\n", takenCubes, root->cubesRemaining);

        if (root->cubesRemaining == 0){
            printf("MIN player(human) wins the game!");
            break;
        }
    }
}
