#pragma once

#include <stdint.h>
#include <stdbool.h>

typedef struct {
	bool Free;

	void* Heap;

	struct HeapNode* Next;
	struct HeapNode* Last;

	char Data[0];
}HeapNode;

/*
* The base addr of the memory block
*/
typedef struct {
	size_t Size;
	HeapNode* First;

}HeapStruct;

int UHeapInit(
	_In_ size_t Size,
	_Out_ HeapStruct** Heap
);

/*
*	returns the address of the mem block or null on failure
*/
void* UHeapAlloc(
	_In_ HeapStruct* Heap, 
	_In_ size_t Size
);

/*
*	returns 0 on sucess or a negative error code
*/
int UHeapFree(
	_In_ HeapStruct* Heap, 
	_In_ void* Addr
);

#ifdef _DEBUG

void LogInfo(_In_ HeapStruct* Heap);

#endif