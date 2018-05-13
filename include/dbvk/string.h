
#include <string.h>

#define NEW_STRING(n) {.value = (char*)malloc(n), .size = n, length = 0}

typedef struct {
	char *const value;
	const size_t size;
	size_t length;
} string;
