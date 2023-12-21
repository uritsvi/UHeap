#include <stdio.h>
#include <memory.h>

#include "heap.h"

#define HEAP_SIZE (1 << 11)
#define ALLOCATION_SIZE (1 << 8)

#define MAX_BUFFERS 288

int g_BuffersIndex;
char* g_Buffers[MAX_BUFFERS];

int main() {
	HeapStruct* heap;
	int res = UHeapInit(
		HEAP_SIZE, 
		&heap
	);
	if (res != 0) {
		printf("Failed to init heap\n");
		return res;
	}

	int i = 0;
	while (true) {
		void* buffer = UHeapAlloc(heap, ALLOCATION_SIZE);
		if (buffer == NULL) {
			break;
		}

		memset(buffer, 69, ALLOCATION_SIZE);
		i++;

	

		g_Buffers[g_BuffersIndex++] = buffer;
	}

	printf("\033[1;32mFinish allocs\n");
	printf("\033[1;0m");

#ifdef _DEBUG
	LogInfo(heap);
#endif

	for (int i = 0; i < 5; i++) {
		res = UHeapFree(
			heap,
			g_Buffers[i]
		);
		if (res != 0) {
			printf("Failed to heap free index %d\n", i);
			return -1;
		}
	}

	printf("\033[1;32mFinish free\n");
	printf("\033[1;0m");

#ifdef _DEBUG
	LogInfo(heap);
#endif

	for (int i = 0; i < 5; i++) {
		void* buffer = UHeapAlloc(heap, ALLOCATION_SIZE * 2);
		if (buffer == NULL) {
			break;
		}
	

	}

	printf("\033[1;32mFinish allocs\n");
	printf("\033[1;0m");

#ifdef _DEBUG
	LogInfo(heap);
#endif



	return 0;
}