#ifndef HASHADT_H_INCLUDED
#define HASHADT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

typedef struct HASHTABLE
{
    void **HashArray;
    int (*encript) (void* argu, int maxSize);
    int (*compare) (void* argu1, void *argu2);
    int size;
    int maxSize;

    int collisions;
    int lcp;
}HASH;

HASH* hashCreate (int maxSize,int (*encript) (void* argu, int maxSize), int (*compare) (void* argu1, void*argu2));

bool hashInsert(HASH* hash, void* dataPtr);
bool hashDelete(HASH* hash, void* dataPtr);
bool RetriveHashData(HASH *hash, void* dataPtr, void (*process)(void* dataPtr));
int hashSize(HASH *hash);
int hashMaxSize(HASH *hash);
int linearProbe(HASH *hash, int key);
bool hashEmpty(HASH *hash);
bool hashTraverse(HASH *hash, void (*process)(void* dataPtr));
void* hashSearch(HASH* hash, void* dataPtr, int (*compare)(void*, void*));
bool hashDestroy(HASH* hash, void (*delete)(void*));

#endif // HASHADT_H_INCLUDED


