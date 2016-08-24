/*
    This file contains the definitons of the functions to maintain
	and process a BST:

    Public Functions:
		BST_Create
		BST_Destroy
		BST_Insert
		BST_Delete
		BST_Retrieve
		BST_Traverse
		BST_Empty
		BST_Full
		BST_Count
		BST_RetrieveMultiple
		void BST_BFS(NODE* root, void (*process)(void* dataPtr));

    Private Functions:
		_insert
		_delete
		_retrieve
		_traverse
		_destroy
*/


#include "bstADT.h"
#include "queueADT.h"



//	BST: Prototype Declarations for private functions
static NODE* _insert (BST_TREE* tree,
					  NODE* root,
					  NODE* newPtr);
static NODE* _delete (BST_TREE* tree,
					  NODE* root,
					  void* dataPtr,
					  bool* success);
static void*  _retrieve(BST_TREE* tree,
					   void* dataPtr,
					   NODE* root);

static void _traverse (NODE* root,
					   void (*process) (void* dataPtr));
static void _destroy (NODE* root,  void (*process)(void* dataPtr));

/*	================= BST_Create ================
	Allocates dynamic memory for an BST tree head
	node and returns its address to caller
	   Pre    compare is address of compare function
	          used when two nodes need to be compared
	   Post   head allocated or error returned
	   Return head node pointer; null if overflow
*/
BST_TREE* BST_Create
        (int  (*compare) (void* argu1, void* argu2))
{
	BST_TREE* tree;

	tree = (BST_TREE*) malloc (sizeof (BST_TREE));
	if (tree){
	    tree->root    = NULL;
	    tree->count   = 0;
	    tree->compare = compare;
	}

	return tree;
}// BST_Create

/*	================= BST_Insert ===================
	This function inserts new data into the tree.
	   Pre    tree is pointer to BST tree structure
	   Post   data inserted or memory overflow
	   Return Success (true) or Overflow (false)
*/
bool BST_Insert (BST_TREE* tree, void* dataPtr)
{
	NODE* newPtr;

	newPtr = (NODE*)malloc(sizeof(NODE));
	if (!newPtr)
	   return false;

	newPtr->right   = NULL;
	newPtr->left    = NULL;
	newPtr->dataPtr = dataPtr;

	if (tree->count == 0)
	    tree->root  =  newPtr;
	else
	    _insert(tree, tree->root, newPtr);

	(tree->count)++;
	return true;
}// BST_Insert

/*	==================== _insert ====================
	This function uses recursion to insert the new data
	into a leaf node in the BST tree.
	   Pre    Application has called BST_Insert, which
	          passes root and data pointer
	   Post   Data have been inserted
	   Return pointer to [potentially] new root
*/
static NODE* _insert (BST_TREE* tree, NODE* root, NODE* newPtr)
{
	if (!root) // if NULL tree
	   return newPtr;

	// Locate null subtree for insertion
        if (tree->compare(newPtr->dataPtr, root->dataPtr) < 0)
        {
            root->left = _insert(tree, root->left, newPtr);
            return root;
        }     // new < node
        else{ // new data >= root data
            root->right = _insert(tree, root->right, newPtr);
            return root;
        } // else new data >= root data

}// _insert

/* ================== BST_Delete ==================
	This function deletes a node from the tree and
	rebalances it if necessary.
	   Pre    tree initialized--null tree is OK
	          dltKey is pointer to data structure
	                 containing key to be deleted
	   Post   node deleted and its space recycled
	          -or- An error code is returned
	   Return Success (true) or Not found (false)
*/
bool BST_Delete (BST_TREE* tree, void* dltKey)
{
	bool  success;
	NODE* newRoot;

	newRoot = _delete (tree, tree->root, dltKey, &success);
	if (success){
	    tree->root = newRoot;
	    (tree->count)--;
	    if (tree->count == 0) // Tree now empty
	        tree->root = NULL;
	}
	return success;
}// BST_Delete

/*	==================== _delete ====================
	Deletes node from the tree (key must be unique!)
	   Pre    tree initialized--null tree is OK.
	          dataPtr contains key of node to be deleted
	   Post   node is deleted and its space recycled
	          -or- if key not found, tree is unchanged
	   Return success is true if deleted; false if not found
	          pointer to root
*/
static NODE*  _delete (BST_TREE* tree,    NODE* root,
                void*     dataPtr, bool* success)
{
	NODE* dltPtr;
	NODE* exchPtr;
	NODE* newRoot;
	void* holdPtr;

	if (!root){
	    *success = false;
	    return NULL;
	}

	if (tree->compare(dataPtr, root->dataPtr) < 0)
	    root->left  = _delete (tree,    root->left,
	                           dataPtr, success);
	else if (tree->compare(dataPtr, root->dataPtr) > 0)
	    root->right = _delete (tree,    root->right,
	                            dataPtr, success);
	else{ // Delete node found--test for leaf node
	    dltPtr = root;
		if (!root->left){         // No left subtree
            //free (root->dataPtr);       // data memory
	        newRoot = root->right;
	        free (dltPtr);              // BST Node
	        *success = true;
	        return newRoot;             // base case
	     }
	     else
			 if (!root->right){   // Only left subtree
	             newRoot = root->left;
	             free (dltPtr);
	             *success = true;
	             return newRoot;         // base case
	         }
			 else{ // Delete Node has two subtrees
                 exchPtr = root->left;
	             // Find largest node on left subtree
	             while (exchPtr->right)
	                 exchPtr = exchPtr->right;

	              // Exchange Data
	              holdPtr          = root->dataPtr;
	              root->dataPtr    = exchPtr->dataPtr;
	              exchPtr->dataPtr = holdPtr;
	              root->left       =
	                 _delete (tree,   root->left,
	                          exchPtr->dataPtr, success);
			 }// else
	}// node found
	return root;
}// _delete

/*	==================== BST_Retrieve ===================
	Retrieve node searches tree for the node containing
	the requested key and returns pointer to its data.
	   Pre     Tree has been created (may be null)
	           data is pointer to data structure
	                containing key to be located
	   Post    Tree searched and data pointer returned
	   Return  Address of matching node returned
	           If not found, NULL returned
*/
void* BST_Retrieve  (BST_TREE* tree, void* dataPtr)
{
    NODE *found;
	if (tree->root)
	{
	    found = _retrieve (tree, dataPtr, tree->root);
	    if (found)
	        return found->dataPtr;
	    return NULL;
	}
    return NULL;
}// BST_Retrieve

/*	===================== _retrieve =====================
	Searches tree for node containing requested key
	and returns its data to the calling function.
	   Pre     _retrieve passes tree, dataPtr, root
	           dataPtr is pointer to data structure
	              containing key to be located
	   Post    tree searched; data pointer returned
	   Return  Address of data in matching node
	           If not found, NULL returned
*/
static void* _retrieve (BST_TREE* tree,
	              void* dataPtr, NODE* root)
{
	if (root){
	     if (tree->compare(dataPtr, root->dataPtr) < 0)
	         return _retrieve(tree, dataPtr, root->left);
	     else if (tree->compare(dataPtr, root->dataPtr) > 0)
	         return _retrieve(tree, dataPtr, root->right);
	     else
	         // Found equal key
	         return root;
	}  // if root
	    // Data not in tree
    return NULL;
}// _retrieve

/*	===================== BST_RetrieveMultiple =====================
	Searches tree for node containing ununique keys
	and returns a bool to the calling function.
	   Pre
	   Post    tree searched and goes either left or right of the tree
                depending if the key is less then or greater then the key.
                If it is equal than it will print the data and then
                continue through the tree.
	   Return  returns true if at least 1 entry is found
               returns false if no entrys were found
*/
bool BST_RetrieveMultiple  (BST_TREE* tree, void* dataPtr, NODE* root, void (*process)(void* dataPtr))
{
    int found;
    bool foundit;
    foundit = false;

    if (root)
    {
        if((found = tree->compare(dataPtr, root->dataPtr)) < 0)
             foundit = BST_RetrieveMultiple(tree, dataPtr, root->left, process);
        else
            foundit = BST_RetrieveMultiple(tree, dataPtr, root->right, process);

       if(tree->compare(dataPtr, root->dataPtr) == 0)
       {
            process(root->dataPtr);
            foundit = true;
       }
    }

    return foundit;
}// BST_Retrieve

/*	=================== BST_Traverse ===================
	Process tree using inorder traversal.
	   Pre   Tree has been created (may be null)
	         process �visits� nodes during traversal
	   Post  Nodes processed in LNR (inorder) sequence
*/
void BST_Traverse (BST_TREE* tree,
                   void (*process) (void* dataPtr))
{
	_traverse (tree->root, process);
	return;
} // end BST_Traverse

/*	=================== _traverse ===================
	Inorder tree traversal. To process a node, we use
	the function passed when traversal was called.
	   Pre   Tree has been created (may be null)
	   Post  All nodes processed
*/
static void _traverse (NODE* root,
                void (*process) (void* dataPtr))
{
    if (root)
    {
        _traverse (root->left, process);
        process   (root->dataPtr);
        _traverse (root->right, process);
    }
    return;
}// _traverse

/*	=================== BST_BFS ===================
	Breadth first traversal prints each row of the tree
	from top to bottom and will process each row.
	   Pre   creates a queue were it will store each
             row
             sets currentRoot = root to set up the loop
             and determine to current root.
	   Post  Process each row at a time by putting
            data into a queue and when the left and right
            of the root are in the queue it will dequeue
            and process.
*/

void BST_BFS (NODE* root, void (*process) (void* dataPtr))
{
    QUEUE* q;
    NODE* currentRoot;

    currentRoot = root;

    q = createQueue();
    while(currentRoot)
    {
        process(currentRoot->dataPtr);
        if(currentRoot->left)
            enqueue(q, currentRoot->left);
        if(currentRoot->right)
            enqueue(q, currentRoot->right);
        if(!(emptyQueue(q)))
            dequeue(q, &currentRoot);
        else
            currentRoot = NULL;
    }

}
/*	=================== BST_Empty ==================
	Returns true if tree is empty; false if any data.
	   Pre      Tree has been created. (May be null)
	   Returns  True if tree empty, false if any data
*/
bool BST_Empty (BST_TREE* tree)
{
	return (tree->count == 0);
}// BST_Empty

/*	===================== BST_Full ====================
	If there is no room for another node, returns true.
	   Pre      tree has been created
	   Returns  true if no room for another insert
	            false if room
*/
bool BST_Full (BST_TREE* tree)
{
	NODE* newPtr;

	newPtr = (NODE*)malloc(sizeof (*(tree->root)));
	if (newPtr){
	    free (newPtr);
	    return false;
	}
	return true;
}// BST_Full

/*	=================== BST_Count ==================
	Returns number of nodes in tree.
	   Pre     tree has been created
	   Returns tree count
*/
int BST_Count (BST_TREE* tree)
{
	return (tree->count);
}// BST_Count

/*	=============== BST_Destroy ==============
	Deletes all data in tree and recycles memory.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      tree is a pointer to a valid tree
	   Post     All data and head structure deleted
	   Return   null head pointer
*/
BST_TREE* BST_Destroy (BST_TREE* tree, void (*process)(void* dataPtr))
{
    printf("Start destroy!\n");
	if (tree)
		_destroy (tree->root, process);

	// All nodes deleted. Free structure
	free (tree);
	printf("Tree deleted!\n\n");
	return NULL;
}// BST_Destroy


/*	=============== _destroy ==============
	Deletes all data in tree and recycles memory.
	It also recycles memory for the key and data nodes.
	The nodes are deleted by calling a recursive
	function to traverse the tree in inorder sequence.
	   Pre      root is pointer to valid tree/subtree
	   Post     All data and head structure deleted
	   Return   null head pointer
*/
static void _destroy(NODE* root, void (*process)(void* dataPtr))
{
	if (root)
	{
		if (root->left)
		{
			_destroy(root->left, process);
		}
		if (root->right)
		{
			_destroy(root->right, process);
		}
	}
	if(process)
	{
	    printf("freeing data: %d\n", root->dataPtr);
        process(root->dataPtr);
        free(root->dataPtr);
        printf("dataPtr freed! %d\n", root->dataPtr);
	}


    printf("Freeing root! %d\n", root);
	free(root);

	return;
} // _destroy

void _print(NODE *p,int start, void (*process)(void* dataPtr, int indent))
{
        start++;
        if (p->right != NULL)
        {
            _print(p->right,start, process);
        }
        process(p->dataPtr, start);

        process(p->dataPtr, -1);
        if (p->left != NULL)
        {
            _print(p->left, start, process);
        }
}

void BST_Graphic(BST_TREE* tree, void (*process)(void* dataPtr, int indent) )
{
	NODE*p = tree->root;

	_print(p, 0, process);
}


