#include "../../include/audio/emitter.h"
#include <SDL2/SDL_mixer.h>

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SIGN(x) (int)(x > 0) ? 1 : ((x < 0) ? -1 : 0)

Uint8 right_ear_amp(float angle) {
  float r = (1 + MAX(cos(2 * angle), SIGN(sin(angle)))) / 2.0;
  r *= 255;
  return (Uint8)r;
}

Uint8 left_ear_amp(float angle) { return right_ear_amp(-angle); }

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

  Mix_VolumeChunk(ae->chunk, ae->volume * MIX_MAX_VOLUME);
  Mix_SetPanning(ae->channel, left_ear_amp(ae->angle),
                 right_ear_amp(ae->angle));

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
