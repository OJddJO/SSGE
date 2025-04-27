#include "SSGE_btree.h"

static SSGE_BTreeNode *SSGE_BTree_CreateNode() {
    SSGE_BTreeNode *tree = (SSGE_BTreeNode *)malloc(sizeof(SSGE_BTreeNode));
    for (int i = 0; i < 3; i++) {
        tree->keys[i] = 0;
        tree->values[i] = NULL;
        tree->children[i] = NULL;
    }
    tree->children[3] = NULL;
    tree->parent = NULL;
    tree->nbKeys = 0;
    tree->isLeaf = false;
    return tree;
}

/**
 * Creates a B-Tree
 * \return The pointer to the B-Tree
 */
SSGE_BTree *SSGE_BTree_Create() {
    SSGE_BTree *tree = (SSGE_BTree *)malloc(sizeof(SSGE_BTree));
    *tree = SSGE_BTree_CreateNode(); // tree is just a pointer to the root of the B-Tree
    ((SSGE_BTreeNode *)*tree)->isLeaf = true;
    return tree;
}

static void _split_node(SSGE_BTree *root, SSGE_BTreeNode *node) {
    if (node->parent == NULL) {
        // If the node to split is the root
        SSGE_BTreeNode *newRoot = SSGE_BTree_CreateNode(); // Create new root
        node->parent = newRoot; // Assign newRoot as the parent of the current root
        newRoot->children[0] = node; // Add current node to newRoot children
        *root = newRoot; // Change current root
    }

    // Add the middle key and value in the parent
    SSGE_BTreeNode *parent = node->parent;
    parent->keys[parent->nbKeys] = node->keys[1];
    parent->values[parent->nbKeys] = node->values[1];
    ++parent->nbKeys;

    // Create the new child
    SSGE_BTreeNode *newChild = SSGE_BTree_CreateNode();
    parent->children[parent->nbKeys] = newChild;
    newChild->parent = parent;
    newChild->isLeaf = node->isLeaf;
    newChild->keys[0] = node->keys[2];
    newChild->values[0] = node->values[2];
    newChild->children[0] = node->children[2];
    newChild->children[1] = node->children[3];
    if (newChild->children[0] != NULL) {
        newChild->children[0]->parent = newChild;
    }
    if (newChild->children[1] != NULL) {
        newChild->children[1]->parent = newChild;
    }
    newChild->nbKeys = 1;

    node->nbKeys = 1;
}

static void _btree_insert(SSGE_BTree *tree, SSGE_BTreeNode *node, int key, void *value) {
    if (node->isLeaf) {
        // Insert the key and value in the leaf node
        int i = 0;
        while (i < node->nbKeys && key > node->keys[i]) {
            ++i;
        }
        for (int j = node->nbKeys; j > i; --j) {
            node->keys[j] = node->keys[j - 1];
            node->values[j] = node->values[j - 1];
        }
        node->keys[i] = key;
        node->values[i] = value;
        ++node->nbKeys;
    } else {
        // Find the child to insert the key
        int i = 0;
        while (i < node->nbKeys && key > node->keys[i]) {
            ++i;
        }
        _btree_insert(tree, node->children[i], key, value);
    }
    if (node->nbKeys == 3) {
        _split_node(tree, node);
    }
}

/**
 * Insert a key-value pair in a B-Tree
 * \param tree The B-Tree to insert the pair
 * \param key The key to insert in the tree
 * \param value The pointer to the value to insert
 */
void SSGE_BTree_Insert(SSGE_BTree *tree, int key, void *value) {
    SSGE_BTreeNode *root = *tree;
    _btree_insert(tree, root, key, value);
}

/**
 * Remove a key from a B-Tree
 * \param tree The B-Tree to remove the key from
 * \param key The key to remove in the tree
 * \return The pointer to the value associated with the key
 */
void *SSGE_BTree_Remove(SSGE_BTree *tree, int key) {
    
}