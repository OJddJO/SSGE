#include "SSGE_btree.h"
#include <stdio.h>

int main() {
    SSGE_BTree *tree = SSGE_BTree_Create();
    SSGE_BTreeNode *node = *tree;

    SSGE_BTree_Insert(tree, 1, NULL);
    SSGE_BTree_Insert(tree, 2, NULL);
    SSGE_BTree_Insert(tree, 3, NULL);
    SSGE_BTree_Insert(tree, 4, NULL);
    SSGE_BTree_Insert(tree, 5, NULL);
    SSGE_BTree_Insert(tree, 6, NULL);
    SSGE_BTree_Insert(tree, 7, NULL);
    SSGE_BTree_Insert(tree, 8, NULL);
    SSGE_BTree_Insert(tree, 9, NULL);
    SSGE_BTree_Insert(tree, 10, NULL);
    SSGE_BTree_Insert(tree, 11, NULL);

    node = *tree;

    printf("%d\n", node->keys[1]);

    return 0;
}