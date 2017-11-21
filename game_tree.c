struct Node {
    int remaining;
    int playerValue;
    int minimax;
    struct Node *children[K];
};

int computeMinimax(struct Node *n){
    if (n->remaining == 0) {
        //ganha quem não estava jogando quando zerou
        return n->playerValue * -1;
    }
    int minimaxSum = 0;
    for(int i=0; i<K && i+1 <= n->remaining;i++) {
        minimaxSum += n->children[i]->minimax;
    }
    return minimaxSum;
}

struct Node *buildGameTree(int npalitos, int currentPlayer){
    struct Node *node = calloc(1, sizeof(struct Node));
    node->remaining = npalitos;
    node->playerValue = currentPlayer;
    int nextPlayer = currentPlayer * -1;
    for (int i=0; i<K; i++) {
        int remaining =  npalitos - (i + 1);
        if (remaining >= 0) {
            node->children[i] = buildGameTree(remaining, nextPlayer);
        }
    }
    node->minimax = computeMinimax(node);
    return node;
}
