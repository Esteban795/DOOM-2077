#include "../include/util.h"

unsigned int g_seed;

// Used to seed the generator.
void fast_srand(int seed) { g_seed = seed; }

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
int fast_rand(void) {
  g_seed = (214013 * g_seed + 2531011);
  return (g_seed >> 16) & 0x7FFF;
}

unsigned long ElfHash(char *s) {
  unsigned long h = 0, high;
  while (*s) {
    h = (h << 4) + *s++;
    if ((high = (h & 0xF0000000)))
      h ^= high >> 24;
    h &= ~high;
  }
  return h;
}

char *strtoupper(char *dest, const char *src) {
  char *result = dest;
  while ((*dest++ = toupper(*src++)));
  return result;
}

void swap(void** arr,int i,int j){
  void* tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}