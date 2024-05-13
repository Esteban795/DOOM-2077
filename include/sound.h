#ifndef SOUND_H
#define SOUND_H

#include "byte_reader.h"
#include "lump.h"

#define DOOR_OPEN_SOUND "DSDOROPN"
#define DOOR_CLOSE_SOUND "DSDORCLS"
#define LIFT_START_SOUND "DSPSTART "
#define LIFT_STOP_SOUND "DSPSTOP"
#define SWITCH_USE "DSSWTCHN"

// https://doomwiki.org/wiki/Sound

struct Sound {
    char* name;
    u16 format_number;
    u16 sample_rate;
    u16 sample_count;
    u8* samples;
};

typedef struct Sound sound;

struct SoundEntry {
    char* sound;
    float angle;
    float volume;
};

typedef struct SoundEntry sound_entry;

extern sound_entry SOUNDS_TO_PLAY[16];
extern int SOUNDS_INDEX;

void sounds_free(sound* sounds, int sounds_count);

sound* get_sounds(FILE* f,lump* directory,int directory_size, int* sounds_count);

sound* get_sound_by_name(sound* sounds, int sounds_count, char* name);
#endif
