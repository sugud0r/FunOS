#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>

#include <stddef.h>

int memcmp(const void*, const void*, size_t);
void* memcpy(void* restrict, const void* restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);

#endif