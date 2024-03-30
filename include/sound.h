#ifndef SOUND_H
#define SOUND_H

#include "byte_reader.h"
#include "lump.h"

struct Sound {
    char* name;
    u16 format_number;
    u16 sample_rate;
    u32 sample_count;
    u8* samples;
};

typedef struct Sound sound;

#endif
