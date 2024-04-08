#ifndef AUDIO_EMITTER_H
#define AUDIO_EMITTER_H

#include "../sound.h"

#include "SDL2/SDL_mixer.h"
#include <stdlib.h>

typedef struct _AudioEmitter {
  int uid;
  sound *current_sound;
  float angle;  // if == -1, becomes non positional. between 0 and 2*pi rad,
                // plays positionally
  float volume; // used as priority when there are too many sounds playing.
  Mix_Chunk *chunk;
} AudioEmitter;

AudioEmitter *audioemitter_create(sound *sound, float angle, float volume);
void audioemitter_free(AudioEmitter *ae);
// TODO:
void audioemitter_update(AudioEmitter *ae, int dt);

#endif
