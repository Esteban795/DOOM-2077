#include "../../include/audio/emitter.h"

AudioEmitter *audioemitter_create(sound *sound, float angle, float volume) {
  AudioEmitter *ae = malloc(sizeof(AudioEmitter));

  static int uid_gen = 0;
  ae->uid = uid_gen;
  uid_gen++;

  ae->current_sound = sound;
  ae->angle = angle;
  ae->volume = volume;

  ae->chunk = Mix_QuickLoad_RAW(ae->current_sound->samples,
                                ae->current_sound->sample_count);

  return ae;
}

void audioemitter_free(AudioEmitter *ae) {
  Mix_FreeChunk(ae->chunk);
  free(ae);
}
