#ifndef SOUND_H
#define SOUND_H

#include "byte_reader.h"
#include "lump.h"

// https://doomwiki.org/wiki/Sound

struct Sound {
    char* name;
    u16 format_number;
    u16 sample_rate;
    u16 sample_count;
    u8* samples;
};

typedef struct Sound sound;

void sounds_free(sound* sounds, int sounds_count);

sound* get_sounds(FILE* f,lump* directory,int directory_size, int* sounds_count);
#endif
