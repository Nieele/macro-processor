#include "memory_utils.h"

void* safe_malloc(size_t size) {
	void* ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "failed to allocate memory");
		exit(FAILED_TO_ALLOCATE_MEMORY);
	}
	return ptr;
}

void* safe_realloc(void* ptr, size_t size) {
	void* temp = realloc(ptr, size);
	if (temp == NULL) {
		fprintf(stderr, "failed to rellocate memory");
		exit(FAILED_TO_ALLOCATE_MEMORY);
	}
	return temp;
}