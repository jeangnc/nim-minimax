#include <pthread.h>

struct Node {
    int remaining;
    int minimax;
    int currentPlayer;
    struct Node *children[K];
};

struct ThreadInfo {
    int remaining;
    int currentPlayer;
    struct Node *node;
};


int minimax(struct Node *n){
    if (n->remaining == 0) {
        return n->currentPlayer * -1; // quem jogou antes de zerar ganha
    }
    int minimaxSum = 0;
    for(int i=0; i<K && i<n->remaining;i++) {
        minimaxSum += minimax(n->children[i]);
    }
    return minimaxSum;
}

struct Node *build_tree(int npalitos, int currentPlayer){
    struct Node *node = malloc(sizeof(struct Node));
    node->remaining = npalitos;
    node->currentPlayer = currentPlayer;
    int nextPlayer = currentPlayer * -1;
    for (int i=0; i<K; i++) {
        int remaining =  npalitos - (i + 1);
        if (remaining >= 0) {
            node->children[i] = build_tree(remaining, nextPlayer);
        }
    }
    node->minimax = minimax(node);
    return node;
}

void *build_thread(void *data) {
    struct ThreadInfo *info = data;
    info->node = build_tree(info->remaining, info->currentPlayer);
}

struct Node *parallel_game_tree() {
    pthread_t tid[K];
    struct ThreadInfo *threads_info[K];

    struct Node *gameTree = malloc(sizeof(struct Node));
    gameTree->currentPlayer = FIRST_PLAYER;
    gameTree->remaining = M;

    for(int i=0;i<K;i++) {
        struct ThreadInfo *info = malloc(sizeof(struct ThreadInfo));
        info->remaining = gameTree->remaining-(i+1);
        info->currentPlayer = gameTree->currentPlayer * -1;
        pthread_create(&tid[i], NULL, build_thread, info);
        threads_info[i] = info;
    }

    for(int i=0;i<K;i++) {
        pthread_join(tid[i], NULL);
        gameTree->children[i] = threads_info[i]->node;
    }

    return gameTree;
}
