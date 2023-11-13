#include "str_utils.h"

void skipdelimitersiters(const char* delimiters, const char** word_start) {
	while (strchr(delimiters, *(*word_start)) != NULL) {
		++(*word_start);
	}
}

ssize_t scanWord(const char* line, const char* delimiters, size_t sizeBuff, char* buffer)
{
	static const char* lastPos;

	const char* word_start = NULL;
	const char* word_end = NULL;

	if (line != NULL) {
		word_start = line;
	}
	else {
		if (lastPos == NULL) {
			buffer[0] = '\0';
			return -1;
		}
		word_start = lastPos;
	}

	skipdelimitersiters(delimiters, &word_start);

	// after skipping the delimitersiters, word_start is the end-line character
	if (*word_start == '\0' || *word_start == '\n') {
		lastPos = NULL;
		buffer[0] = '\0';
		return -1;
	}

	// find another delimitersiter
	word_end = strpbrk(word_start, delimiters);

	// delimitersiter not found => one word
	if (word_end == NULL) {
		word_end = word_start + strlen(word_start) - 1;
	}

	size_t word_length = word_end - word_start;
	word_length = (word_length >= sizeBuff ? (sizeBuff - 1) : word_length);

	strncpy(buffer, word_start, word_length);
	buffer[word_length] = '\0';
	lastPos = word_end;

	return word_length;
}