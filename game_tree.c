struct Node {
    int remaining;
    int minimax;
    int currentPlayer;
    struct Node *children[K];
};

int computeMinimax(struct Node *n){
    if (n->remaining == 0) {
        // quem jogou antes de zerar ganha
        return n->currentPlayer * -1;
    }
    int minimaxSum = 0;
    for(int i=0; i<K && i<n->remaining;i++) {
        minimaxSum += computeMinimax(n->children[i]);
    }
    return minimaxSum;
}

struct Node *buildGameTree(int npalitos, int currentPlayer){
    struct Node *node = calloc(1, sizeof(struct Node));
    node->remaining = npalitos;
    node->currentPlayer = currentPlayer;
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
