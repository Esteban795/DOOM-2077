#ifndef UTIL_H
#define UTIL_H

#include "byte_reader.h"
#include "string.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_stdinc.h>
#include <ctype.h>
#include <stdint.h>

unsigned long ElfHash(char *s);
extern SDL_PixelFormat *fmt;

#define NO_TEXTURE_HASH ElfHash("-")
#define SKY_TEXTURE_HASH ElfHash("F_SKY1")

void fast_srand(int seed);

int fast_rand(void);

char *strtoupper(char *dest, const char *src);

void swap(void **arr, int i, int j);

Uint32 AdjustHSL(Uint8 r, Uint8 g, Uint8 b, Uint8 a, double hOffset);

#endif