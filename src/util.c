#include "../include/util.h"
#include <SDL2/SDL_stdinc.h>

SDL_PixelFormat* fmt = NULL;
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
  while ((*dest++ = toupper(*src++)))
    ;
  return result;
}

void swap(void **arr, int i, int j) {
  void *tmp = arr[i];
  arr[i] = arr[j];
  arr[j] = tmp;
}


#ifndef MIN
#define MIN(a,b) (((a)<(b))?(a):(b))
#endif

#ifndef MAX
#define MAX(a,b) (((a)>(b))?(a):(b))
#endif

#ifndef MAX3
#define MAX3(a,b,c) MAX(MAX(a,b),c)
#endif

#ifndef MIN3
#define MIN3(a,b,c) MIN(MIN(a,b),c)
#endif

void convertHSVtoRGB(const float h, const float s, const float v, unsigned char * r, unsigned char * g, unsigned char * b)
{
    /* Convert hue back to 0-6 space, floor */
    const float hex = h / 60.0;
    const unsigned char primary = (int) hex;
    const float secondary = hex - primary;

    /* a, b, c */
    const float x = (1.0 - s) * v;
    const float y = (1.0 - (s * secondary)) * v;
    const float z = (1.0 - (s * (1.0 - secondary))) * v;

    if(primary == 0)
    {
        /* 0: R = v, G = c, B = a */
        *r = (v * 255.0) + 0.5;
        *g = (z * 255.0) + 0.5;
        *b = (x * 255.0) + 0.5;
    }
    else if(primary == 1)
    {
        /* 1: R = b, G = v, B = a */
        *r = (y * 255.0) + 0.5;
        *g = (v * 255.0) + 0.5;
        *b = (x * 255.0) + 0.5;
    }
    else if(primary == 2)
    {
        /* 2: R = a, G = v, B = c */
        *r = (x * 255.0) + 0.5;
        *g = (v * 255.0) + 0.5;
        *b = (z * 255.0) + 0.5;
    }
    else if(primary == 3)
    {
        /* 3: R = a, G = b, B = v */
        *r = (x * 255.0) + 0.5;
        *g = (y * 255.0) + 0.5;
        *b = (v * 255.0) + 0.5;
    }
    else if(primary == 4)
    {
        /* 4: R = c, G = a, B = v */
        *r = (z * 255.0) + 0.5;
        *g = (x * 255.0) + 0.5;
        *b = (v * 255.0) + 0.5;
    }
    else if(primary == 5)
    {
        /* 5: R = v, G = a, B = b */
        *r = (v * 255.0) + 0.5;
        *g = (x * 255.0) + 0.5;
        *b = (y * 255.0) + 0.5;
    }
}

void convertRGBtoHSV(const unsigned char r, const unsigned char g, const unsigned char b, float * h, float * s, float * v)
{
    const unsigned char max = MAX3(r, g, b);
    const float max2 = max / 255.0;
    const unsigned char min = MIN3(r, g, b);
    const float min2 = min / 255.0;

    *s = (max2 < 0.0001) ? 0 : (max2 - min2) / max2;
    *v = max2;

    /* Saturation is 0 */
    if((*s * 100.0) < 0.1)
    {
        /* Hue is undefined, monochrome */
        *h = 0;
        return;
    }
    else if(r == max)
    {
        if(g == min)
        {
            /* H = 5 + B' */
            *h = 5 + ((max2 - (b / 255.0)) / (max2 - min2));
        }
        else
        {
            /* H = 1 - G' */
            *h = 1 - ((max2 - (g / 255.0)) / (max2 - min2));
        }
    }
    else if(g == max)
    {
        if(b == min)
        {
            /* H = R' + 1 */
            *h = ((max2 - (r / 255.0)) / (max2 - min2)) + 1;
        }
        else
        {
            /* H = 3 - B' */
            *h = 3 - ((max2 - (b / 255.0)) / (max2 - min2));
        }
    }
    /* This is actually a problem with the original paper, I've fixed it here, should email them... */
    else if(b == max && r == min)
    {
        /* H = 3 + G' */
        *h = 3 + ((max2 - (g / 255.0)) / (max2 - min2));
    }
    else
    {
        /* H = 5 - R' */
        *h = 5 - ((max2 - (r / 255.0)) / (max2 - min2));
    }

    /* Hue is then converted to degrees by multiplying by 60 */
    *h = MIN(*h * 60, 360);
}

// Function to adjust HSL values
Uint32 AdjustHSL(Uint8 r,Uint8 g, Uint8 b, Uint8 a, double hOffset) {
  float h, s, l;
  convertRGBtoHSV(r, g, b,&h, &s, &l);
  h = fmod((h + hOffset), 360.0);
  convertHSVtoRGB(h, s, l, &r, &g, &b);
  Uint32 pixel = SDL_MapRGBA(fmt, r, g, b, a);
  return pixel;
}



