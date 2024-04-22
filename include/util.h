#ifndef UTIL_H
#define UTIL_H

#include "byte_reader.h"

unsigned long ElfHash(char *s);

#define NO_TEXTURE_HASH ElfHash("-")
#define SKY_TEXTURE_HASH ElfHash("F_SKY1")

void fast_srand(int seed);

int fast_rand(void);
#endif