#ifndef HEAPMINQUEUE_H
#define HEAPMINQUEUE_H
#include "Astar.h"

int heap_size;  //open表的大小

int left(int i)
{
	return (2*i+1);
}

int right(int i)
{
	return (2*i+2);
}

void minHeapify(OPEN A[],int i)
{
	int l = left(i);
	int r = right(i);
	int largest;
	OPEN temp;
	if(l<heap_size && A[l].f<A[i].f)
	{
		largest = l;
	}
	else
		largest = i;
	if(r<heap_size && A[r].f<A[largest].f)
	{
		largest = r;
	}
	if(largest != i)
	{
		temp=A[i];
		A[i]=A[largest];
		A[largest]=temp;
		minHeapify(A,largest);
	}
}

void BuildMinHeap(OPEN A[])
{
	int i;
	int t = heap_size/2-1;
	for(i = t;i >= 0; i--)
	{
		minHeapify(A,i);
	}
}

OPEN addMinHeap(OPEN A[],OPEN f)
{
	A[heap_size++] = f;
	BuildMinHeap(A);
	return f;
}

OPEN HeapMiniMum(OPEN A[])
{
	return A[0];
}

OPEN HeapExtractMin(OPEN A[])
{
	if(heap_size<1)
	{
		printf("heap underflow!\n");
		exit(0);
	}
	OPEN min = A[0];
	A[0] = A[heap_size-1];
	heap_size --;
	minHeapify(A,0);
	return min;
}

#endif