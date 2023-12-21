#include <stdbool.h>
#include <Windows.h>

#include "heap.h"

size_t _CalcBlockSize(_In_ HeapNode* Node) {
	void* finish = Node->Next;
	void* start = Node;

	HeapStruct* heap = Node->Heap;

	if (finish == NULL) {
		finish = (char*)heap + heap->Size;
	}

	size_t size = ((size_t)finish - (size_t)start) - sizeof(HeapNode);

	return size;
}

HeapNode* _InitNode(
	_In_ HeapNode* Next,
	_In_ HeapNode* Last,
	_In_ HeapStruct* Heap,
	_Out_ HeapNode* Addr) {

	Addr->Heap = Heap;
	Addr->Next = Next;
	Addr->Last = Last;
	Addr->Free = true;

	return Addr;
}

int UHeapInit(
	_In_ size_t Size,
	_Out_ HeapStruct** Heap) {
	
	int res = 0;

	do {
		HeapStruct* heap = (HeapStruct*)VirtualAlloc(
			NULL, 
			Size, 
			MEM_COMMIT | MEM_RESERVE, 
			PAGE_READWRITE
		);
		if (heap == NULL) {
			res = -1;
			break;
		}
#ifdef _DEBUG
		memset((char*)heap, 0x69, Size);
#endif

		heap->Size = Size;
		heap->First = NULL;
		

		heap->First = _InitNode(
			NULL,
			NULL, 
			heap,
			((char*)heap + sizeof(HeapStruct))
		);

		*Heap = heap;


	} while (false);


	return res;
}

void* UHeapAlloc(
	_In_ HeapStruct* Heap,
	_In_ size_t Size) {

	Size += sizeof(HeapNode);

	HeapNode* currentNode = Heap->First;
	bool found = false;

	while (currentNode != NULL) {
		if (currentNode->Free && _CalcBlockSize(currentNode) >= Size) {
			found = true;
			break;
		}
		currentNode = currentNode->Next;
		continue;
	}
	if (!found) {
		return NULL;
	}

	HeapNode* next = currentNode->Next;
	currentNode->Next = _InitNode(
		next,
		currentNode,
		Heap,
		((char*)currentNode + Size + sizeof(HeapNode))
	);

	currentNode->Free = false;
	return currentNode->Data;
}
int UHeapFree(
	_In_ HeapStruct* Heap,
	_In_ void* Addr) {

	HeapNode* currentNode = Heap->First;
	bool found = false;

	while (currentNode != NULL) {
		if (currentNode->Data == Addr) {
			found = true;
			break;
		}
		currentNode = currentNode->Next;
		continue;
	}
	if (!found || currentNode->Free) {
		return -1;
	}

	/*
	* Skip the next node
	*/
	HeapNode* next = currentNode->Next;
	HeapNode* affterNext = next->Next;
	if (next != NULL && next->Free) {
		currentNode->Next = next->Next;
		
		if (affterNext != NULL) {
			affterNext->Last = currentNode;
		}
	}

	/*
	* Skip this node
	*/
	HeapNode* last = currentNode->Last;
	if (last != NULL && last->Free) {
		last->Next = currentNode->Next;

		if (next != NULL) {
			next->Last = last;
		}
	}

	currentNode->Free = true;

	return 0;
}

#ifdef _DEBUG

#include <stdio.h>

void LogInfo(_In_ HeapStruct* Heap) {
	HeapNode* node = Heap->First;

	int i = 0;
	while (node != NULL) {
		if (!node->Free) {
			printf("Node %d: size: %d\n", i, _CalcBlockSize(node));
			i++;

		}


		node = node->Next;
	}

	printf("Num of nodes: %d\n", i);
}
#endif 