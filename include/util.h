#ifndef UTIL_H
#define UTIL_H

#include <ctype.h>
#include "string.h"
#include "byte_reader.h"
#include <ctype.h>

unsigned long ElfHash(char *s);

#define NO_TEXTURE_HASH ElfHash("-")
#define SKY_TEXTURE_HASH ElfHash("F_SKY1")

void fast_srand(int seed);

int fast_rand(void);

char *strtoupper(char *dest, const char *src);

void swap(void** arr,int i,int j);
#endif