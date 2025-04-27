#include "SSGE/SSGE_btree.h"

/**
 * Creates a B-Tree
 * \return The new B-Tree
 */
SSGE_BTree *SSGE_BTree_Create() {
    SSGE_BTree *tree = (SSGE_BTree *)malloc(sizeof(SSGE_BTree));
    tree->keys[0] = 0;
    tree->keys[1] = 0;
    tree->values[0] = NULL;
    tree->values[1] = NULL;
    tree->children[0] = NULL;
    tree->children[1] = NULL;
    tree->children[2] = NULL;
    tree->nbKeys = 0;
    tree->isLeaf = true;
    return tree;
}

static void _split_child(SSGE_BTree *parent, int index) {
    SSGE_BTree *child = parent->children[index];
    SSGE_BTree *newChild = SSGE_BTree_Init();
    
    newChild->isLeaf = child->isLeaf;
    newChild->nbKeys = 1;
    
    newChild->keys[0] = child->keys[1];
    newChild->values[0] = child->values[1];
    
    if (!child->isLeaf) {
        newChild->children[0] = child->children[1];
        newChild->children[1] = child->children[2];
    }
    
    child->nbKeys = 1;

    for (int i = parent->nbKeys; i >= index + 1; i--) {
        parent->children[i + 1] = parent->children[i];
    }
    parent->children[index + 1] = newChild;

    for (int i = parent->nbKeys - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
    }
    parent->keys[index] = child->keys[1];
    parent->nbKeys++;
}

static void _insert_non_full_node(SSGE_BTree *node, int key, void *value) {
    int i = node->nbKeys - 1;

    if (node->isLeaf) {
        // Find the location to insert the new key
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
            i--;
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->nbKeys++;
    } else {
        // Find the child to recurse into
        while (i >= 0 && key < node->keys[i]) {
            i--;
        }
        i++;

        // Split the child if it is full
        if (node->children[i]->nbKeys == 2) {
            _split_child(node, i);
            if (key > node->keys[i]) {
                i++;
            }
        }
        _insert_non_full_node(node->children[i], key, value);
    }
}

/**
 * Inserts a key-value pair into a B-Tree
 * \param tree The B-Tree to insert into
 * \param key The key to insert
 * \param value The pointer of the value to insert
 */
void SSGE_BTree_Insert(SSGE_BTree *tree, int key, void *value) {
    SSGE_BTree *root = tree;

    if (root->nbKeys == 2) {
        SSGE_BTree *new_root = SSGE_BTree_Init();
        new_root->isLeaf = false;
        new_root->children[0] = root;

        _split_child(new_root, 0);

        int i = (key > new_root->keys[0]) ? 1 : 0;
        _insert_non_full_node(new_root->children[i], key, value);

        *tree = *new_root;
    } else {
        _insert_non_full_node(root, key, value);
    }
}

/**
 * Gets the value associated with a key in the B-Tree
 * \param tree The B-Tree to search in
 * \param key The key to search for
 */
void *SSGE_BTree_Get(SSGE_BTree *tree, int key) {
    int i = 0;
    while (i < tree->nbKeys && key > tree->keys[i]) {
        i++;
    }

    if (i < tree->nbKeys && key == tree->keys[i]) {
        return tree->values[i];
    } else if (tree->isLeaf) {
        return NULL;
    } else {
        return SSGE_BTree_Get(tree->children[i], key);
    }
}