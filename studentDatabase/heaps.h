#ifndef HEAPS_H_INCLUDED
#define HEAPS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*	This library contains four heap functions:
	    reheapUp
	    reheapDown
	    insertHeap
	    deleteHeap

	Written by: G & F
	Date:       2/98

    Brooks/Cole Publishing Company
	An International Thomson Publishing Company
	Copyright 1998. All Rights Reserved
*/

void reheapUp   (int *heap, int newNode);
void reheapDown (int *heap, int root,  int  last);
int insertHeap  (int *heap, int *last, int  data, int size);
int deleteHeap  (int *heap, int *last, int *dataOut);

#endif // HEAPS_H_INCLUDED
