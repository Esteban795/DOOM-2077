#ifndef SOUND_H
#define SOUND_H

#include "../util/byte_reader.h"
#include "../util/geometry.h"
#include "../util/vec2.h"
#include "lump.h"

#define MAX_SOUNDS_PLAYING 32

#define DOOR_OPEN_SOUND "DSDOROPN"
#define DOOR_CLOSE_SOUND "DSDORCLS"
#define LIFT_START_SOUND "DSPSTART"
#define LIFT_STOP_SOUND "DSPSTOP"
#define SWITCH_USE_SOUND "DSSWTCHN"

#define PISTOL_SOUND "DSPISTOL"
#define SHOTGUN_SOUND "DSSHOTGN"
#define PUNCH_SOUND "DSPUNCH"
#define CHAINGUN_SOUND "DCHAINGN"

#define FOOTSTEP_SOUND "DFTSTEP"

/// Ref: https://doomwiki.org/wiki/Sound

struct Sound {
  char *name;
  u16 format_number;
  u16 sample_rate;
  u32 sample_count;
  u8 *samples;
};

typedef struct Sound sound;

struct SoundEntry {
  char *sound;
  double x;
  double y;
};

typedef struct SoundEntry sound_entry;

extern sound_entry *SOUNDS_TO_PLAY[MAX_SOUNDS_PLAYING];
extern int SOUNDS_INDEX;

void sounds_free(sound *sounds, int sounds_count);

sound *get_sounds(FILE *f, lump *directory, int directory_size,
                  int *sounds_count);

sound *get_sound_by_name(sound *sounds, int sounds_count, char *name);

sound_entry *add_sound_to_play(char *sound, double x, double y);
#endif
