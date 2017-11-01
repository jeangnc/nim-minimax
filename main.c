#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#define M 13
#define K 4

const int JOGO_FINALIZADO = 1;
const int JOGADA_BEM_SUCEDIDA = 2;
const int JOGADA_INVALIDA = 3;

char player[] = "MAX";
struct Node * arvore;

struct Node{
    int palitosRestantes;
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

struct Node *buildGameTree(int npalitos){
    struct Node *node = calloc(1, sizeof(struct Node));
    node->palitosRestantes = npalitos;

    if (node->palitosRestantes >= 1){
        node->left = buildGameTree(npalitos - 1);
    }
    if (node->palitosRestantes >= K){
        node->right = buildGameTree(npalitos - K);
    }

    return node;
}

int computeMinimax(struct Node *n){
    int value;

    if (n->palitosRestantes == 0){
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

void podaArvore(int ultimaJogada) {
    if (ultimaJogada == 1){
        arvore = arvore->left;
    }
    if (ultimaJogada == K && arvore->right != NULL){
        arvore = arvore->right;
    }
}

int jogadaHumana() {
    int palitosRemovidos;

    do{
        printf("Quandos palitos deseja tirar? Número: ");
        scanf("%d", &palitosRemovidos);

        // valida se a jogada está dentro da faixa permitida
        if ((palitosRemovidos != 1 && palitosRemovidos != K) || arvore->palitosRestantes - palitosRemovidos < 0){
            printf("Jogada inválida. Escolha 1 ou %d palito(s)\n", K);
            continue;
        }

        break;
    } while (true);

    return palitosRemovidos;
}

int melhorJogada() {
    int palitosRemovidos = 0;
    int v1,v2;

    v1 = computeMinimax(arvore->left);
    if (arvore->right != NULL){
        v2 = computeMinimax(arvore->right);
    }

    palitosRemovidos = maximum(v1, v2);
    return palitosRemovidos;
}

int registraJogada(int jogada) {
    podaArvore(jogada);
    printf("%s tira %d palitos, restam %d\n", player, jogada, arvore->palitosRestantes);
    if (arvore->palitosRestantes == 0){
        printf("O jogador %s ganhou o jogo!", player);
        return JOGO_FINALIZADO;
    }
    switchPlayer();
    return JOGADA_BEM_SUCEDIDA;
}


int main(){
    printf("Configuração do jogo: jogada máxima %d e número de palitos %d\n", K, M);

    arvore = buildGameTree(M);

    while (true){
        if (registraJogada(melhorJogada()) == JOGO_FINALIZADO) {
            break;
        }

        if (registraJogada(jogadaHumana()) == JOGO_FINALIZADO) {
            break;
        }
    }
}
