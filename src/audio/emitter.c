#include "../../include/audio/emitter.h"
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_stdinc.h>

AudioEmitter *audioemitter_create(sound *sound, float angle, float volume,
                                  int channel) {
  AudioEmitter *ae = malloc(sizeof(AudioEmitter));

  static int uid_gen = 0;
  ae->uid = uid_gen;
  uid_gen++;

  ae->current_sound = sound;
  ae->angle = angle;
  ae->volume = volume;

  ae->chunk = Mix_QuickLoad_RAW(ae->current_sound->samples,
                                (Uint32)ae->current_sound->sample_count);
  ae->channel = channel;

  Mix_PlayChannel(channel, ae->chunk, 0);

  return ae;
}

void audioemitter_free(AudioEmitter **ae) {
  Mix_FreeChunk((*ae)->chunk);
  free(*ae);
  *ae = 0;
}

void audioemitter_update(AudioEmitter **ae, int dt) {
  int dt_samples = (*ae)->current_sound->sample_rate * (float)dt / 1000.0;
  (*ae)->sample_position += dt_samples;
  if ((*ae)->sample_position > (*ae)->current_sound->sample_count) {
    audioemitter_free(ae);
    *ae = 0;
  }
}
