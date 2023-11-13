#ifndef MEMORY_UTILS_H
#define MEMORY_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

void* safe_malloc(size_t size);
void* safe_realloc(void* ptr, size_t size);

#endif // !MEMORY_UTILS_H