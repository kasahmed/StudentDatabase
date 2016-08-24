#ifndef BSTADT_H_INCLUDED
#define BSTADT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*	Header file for Binary Search Tree (BST). Contains
	structural definitions and prototypes for BST.
	   Written by:
	   Date:
*/
//#include <stdbool.h>
/*#include "stackADT.h"
#include "queueADT.h"*/

typedef enum { false, true} bool;


//	BST type definitions
typedef struct node
{
	 void*        dataPtr;
	 struct node* left;
	 struct node* right;
} NODE;

typedef struct
{
	 int   count;
	 int  (*compare) (void* argu1, void* argu2);
	 NODE*  root;
} BST_TREE;

//	BST: Prototype Declarations for public functions
	BST_TREE* BST_Create
	         (int (*compare) (void* argu1, void* argu2));
	BST_TREE* BST_Destroy (BST_TREE* tree, void (*process)(void* dataPtr));

	bool  BST_Insert   (BST_TREE* tree, void* dataPtr);
	bool  BST_Delete   (BST_TREE* tree, void* dltKey);
	void* BST_Retrieve (BST_TREE* tree, void* keyPtr);
    bool BST_RetrieveMultiple (BST_TREE* tree, void* keyPtr, NODE* root, void (*process)(void* dataPtr));
	void  BST_Traverse (BST_TREE* tree,
	                    void (*process)(void* dataPtr));

	bool BST_Empty (BST_TREE* tree);
	bool BST_Full  (BST_TREE* tree);
	int  BST_Count (BST_TREE* tree);
    void BST_BFS(NODE* root, void (*process)(void* dataPtr));

	void printBST   (void* num1);
	void BST_Graphic(BST_TREE* tree, void (*process)(void* dataPtr, int indent) );





#endif // BSTADT_H_INCLUDED
