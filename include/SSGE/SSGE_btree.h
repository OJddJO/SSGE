#ifndef __SSGE_B_TREE_H__
#define __SSGE_B_TREE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdbool.h>

typedef struct _SSGE_BTree {
    int keys[2];
    void *values[2];
    struct _SSGE_BTree *parent;
    struct _SSGE_BTree *children[3];
    int nbKeys;
    bool isLeaf;
} SSGE_BTree;

SSGE_BTree *SSGE_BTree_Create();
void SSGE_BTree_Insert(SSGE_BTree *, int, void *);
void *SSGE_BTree_Get(SSGE_BTree *, int);

#ifdef __cplusplus
}
#endif

#endif // __SSGE_B_TREE_H__