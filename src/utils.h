#ifndef _UTILS_H_
#define _UTILS_H_


#include <stdio.h>
#include <stdlib.h>

void debug (unsigned int dbg);
void * wr_calloc (size_t nmemb, size_t size);

void *
wr_calloc (size_t nmemb, size_t size) {
	void * alloc = calloc (nmemb, size);
	if (!alloc) {
		perror ("in calloc()");
		return NULL;
	}
	return alloc;
}

void
debug(unsigned int dbg) {
	fprintf (stderr, "[DEBUG] %d\n", dbg);
}


#endif /* _UTILS_H_ */

