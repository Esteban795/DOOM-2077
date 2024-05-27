#ifndef AUDIO_EMITTER_H
#define AUDIO_EMITTER_H

#include "../sound.h"

#include "SDL2/SDL_mixer.h"
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct _AudioEmitter {
  int uid;
  sound *current_sound;
  float angle;  // if == -1, becomes non positional. between 0 and 2*pi rad,
                // plays positionally
  float volume; // used as priority when there are too many sounds playing.
  Mix_Chunk *chunk;
  u32 sample_position;
  int channel;
} AudioEmitter;

AudioEmitter *audioemitter_create(sound *sound, float angle, float volume,
                                  int channel);
void audioemitter_free(AudioEmitter **ae);

// returns true if the sound is done playing
bool audioemitter_update(AudioEmitter *ae, int dt);

#endif
