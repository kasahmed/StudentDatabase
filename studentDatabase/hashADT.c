
#include "hashADT.h"

HASH* hashCreate (int maxSize,int  (*encript) (void* argu, int maxSize), int (*compare) (void* argu1, void* argu2))
{
//	Local Definitions
	HASH* hash;

//	Statements
	hash = (HASH*)malloc(sizeof (HASH));
	if (!hash)
	   return NULL;

	hash->encript = encript;
	hash->compare = compare;
	hash->size = 0;
	hash->collisions = 0;
	hash->lcp = 0;

	// Force heap size to power of 2 -1
	hash->maxSize = maxSize;
	hash->HashArray = (void*)
	                 calloc(hash->maxSize, sizeof(void*));
	return hash;
}	// createHash

bool hashInsert(HASH* hash, void* dataPtr)  //Need to check for colisions, need to use a flag?
{
    int key;
    key = 0;

    key = hash->encript(dataPtr, hash->maxSize);

    if(key > hash->maxSize)
        exit(6);

    if(hash->HashArray[key] != NULL)
    {
        if((key = linearProbe(hash,key)) == -1)
            printf("The table is filled please realloc!\n\n"), exit(7);
        (hash->collisions)++;//printf("colision found!\n\n");
    }

    hash->HashArray[key] = dataPtr;

   // printf("\nData inserted at %d\n", key);  //Data inserted at this index!
    (hash->size)++;
    return true;
}

int linearProbe(HASH *table, int key)
{
    int i;
    int j;

    for(i = 0; i < table->maxSize; i++)
    {
        j = i;

        if(i%2)
            j *= -1;

        if(table->HashArray[key+j] == NULL){
         if(i > table->lcp)
            table->lcp = i;
        return (key+j);
        }

    }

    return -1;
}


bool RetriveHashData(HASH* hash, void* dataPtr, void (*process)(void *dataPtr))
{
    int key;
    int i;
    int j;

    key = hash->encript(dataPtr, hash->maxSize);

    if(hash->HashArray[key] != NULL)
    {
        if(hash->compare(dataPtr, hash->HashArray[key]) == 0)
        {

            process(hash->HashArray[key]);
            return true;
        }
    }

        for(i = 1; i < hash->maxSize; i++)
        {
            j = i;

            if(i%2)
                j *= -1;

            if((key + j) >= 0 && (key + j) < hash->maxSize && hash->HashArray[key+j] != NULL)
            {
                if(hash->compare(dataPtr, hash->HashArray[key + j]) == 0)
                {
                    process(hash->HashArray[key+j]);
                    return true;
                }
            }

        }

    return false;
}

// Returns true if delete work, returns false if it failed
bool hashDelete(HASH* hash, void* dataPtr)
{
    int key;
    int i;
    int j;

    key = hash->encript(dataPtr, hash->maxSize);

    for(i = 0; i < hash->maxSize; i++)
    {
        j = i;

        if(i%2)
            j *= -1;
        if(key + j > 0 && key + j <= hash->maxSize)
        {
            if(hash->compare(dataPtr, hash->HashArray[key + j])== 0)
            {
                hash->HashArray[key+j] = NULL;
                (hash->size)--;
                return true;
            }
        }

    }

    return false;
}

bool hashDestroy(HASH* hash, void (*delete)(void*))
{
	int i;

	if (hashEmpty(hash))
		return true;

	if(delete)
		for (i = 0; i < hash->maxSize; i++)
			if ((hash->HashArray[i]))
				delete(hash->HashArray[i]);
	free(hash->HashArray);
	free(hash);
	return true;
}


bool hashTraverse(HASH *hash, void (*process)(void* dataPtr))
{
    int i;

    if(hashEmpty(hash))
        return true;

    for(i = 0; i < hash->maxSize;i++)
        if((hash->HashArray[i]))
            process(hash->HashArray[i]);
    return true;
}

bool hashEmpty(HASH* hash)
{
    if(hash->size == 0)
        return true;
    return false;
}

int hashSize(HASH *hash)
{
    return(hash->size);
}

int hashMaxSize(HASH *hash)
{
    return (hash->maxSize);
}
