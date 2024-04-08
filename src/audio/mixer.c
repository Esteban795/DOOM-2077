#include "../../include/audio/mixer.h"
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>

AudioMixer *audiomixer_init() {
  Mix_AllocateChannels(AUDIO_MIXER_CHANNELS);

  AudioMixer *am = malloc(sizeof(AudioMixer));

  for (int i = 0; i < AUDIO_MIXER_CHANNELS; i++) {
    am->channels[i] = 0;
  }

  return am;
}

void audiomixer_free(AudioMixer *am) {
  for (int i = 0; i < AUDIO_MIXER_CHANNELS; i++) {
    if (am->channels[i] != 0) {
      audioemitter_free(am->channels[i]);
    }
  }
  free(am);
  Mix_CloseAudio();
}
