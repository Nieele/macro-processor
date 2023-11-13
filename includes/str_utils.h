#ifndef STR_UTILS
#define STR_UTILS

#include <stdio.h>
#include <string.h>

void skipdelimitersiters(const char* delimiters, const char** word_start);
ssize_t scanWord(const char* line, const char* delimiters, size_t sizeBuff, char* buffer);

#endif // !STR_UTILS