#include "heaps.h"



/*	==================== reheapUp ==================== */
/*	Reestablishes heap by moving data in child up to
	correct location heap array.
	   Pre  heap is array containing an invalid heap
	        newNode is index location to new data in heap
	   Post newNode has been inserted into heap
*/
void reheapUp (int *heap, int newNode)
{
//	Local Declarations
	int parent;
	int hold;

//	Statements
	// if not at root of heap
	if (newNode)
	   {
	   	parent = (newNode - 1)/ 2;
	    if ( heap[newNode] > heap[parent] )
	       {
	        // child is greater than parent
	        hold          = heap[parent];
	        heap[parent]  = heap[newNode];
	        heap[newNode] = hold;
	        reheapUp (heap, parent);
	       } // if heap[]
	   } // if newNode
	return;
}	// reheapUp

/*	==================== reheapDown ==================== */
/*	Reestablishes heap by moving data in root down to its
	correct location in the heap.
	   Pre  heap is an array of data
	        root is root of heap or subheap
	        last is an index to the last element in heap
	   Post heap has been restored
*/
void reheapDown   (int *heap, int root, int last)
{
//	Local Declarations
	int hold;
	int leftKey;
	int rightKey;
	int largeChildKey;
	int largeChildIndex;

//	Statements
	if ((root * 2 + 1) <= last)
	    // There is at least one child
	   {
	    leftKey   = heap[root * 2 + 1];
	    if ((root * 2 + 2) <= last)
	       rightKey  = heap[root * 2 + 2];
	    else
	       rightKey  = -1;

	    // Determine which child is larger
	    if (leftKey > rightKey)
	       {
	        largeChildKey   = leftKey;
	        largeChildIndex = root * 2 + 1;
	       } // if leftKey
	    else
	       {
	        largeChildKey   = rightKey;
	        largeChildIndex = root * 2 + 2;
	       } // else
	    // Test if root > larger subtree
	    if (heap[root] < heap[largeChildIndex])
	        {
	         // parent < children
	         hold                  = heap[root];
	         heap[root]            = heap[largeChildIndex];
	         heap[largeChildIndex] = hold;
	         reheapDown (heap, largeChildIndex, last);
	        } // if root <
	} // if root
	return;
}	// reheapDown

/*	================== insertHeap =================== */
/*	Inserts data into heap.
	   Pre    Heap is a valid heap structure.
	          last is pointer to index for last element
	          data is data to be inserted
			  size is the maximum size of the heap
	   Post   data have been inserted into heap.
	   Return true if successful; false if array full
*/
int insertHeap (int *heap, int *last, int  data, int size)
{
//	Local Declarations


//	Statements
	if (*last == size - 1)
	  return 0;
	++(*last);
	heap[*last] = data;
	reheapUp (heap, *last);
	return 1;
}	// insertHeap

/*	 ================== deleteHeap =================== */
/*	Deletes root of heap and passes data back to caller.
	   Pre    heap is a valid heap structure
	          last is index to last node in heap
	   Post   element removed and heap rebuilt
	          deleted data passed back to user
	   Return true if successful; false if array empty
*/
int deleteHeap (int *heap, int *last, int *dataOut)
{
//	Local Declarations

//	Statements
	if (*last < 0)
	    return 0;
	*dataOut = heap[0];
	heap[0]  = heap[*last];
	(*last)--;
	reheapDown (heap, 0, *last);
	return 1;
}// deleteHeap
