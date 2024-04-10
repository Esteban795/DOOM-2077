#include "../../include/audio/mixer.h"
#include <SDL2/SDL_audio.h>
#include <SDL2/SDL_mixer.h>

AudioMixer *audiomixer_init() {
  Mix_OpenAudio(AUDIO_SAMPLE_RATE, AUDIO_U8, 1, 256);
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

void audiomixer_update(AudioMixer *am, int dt) {
  for (int i = 0; i < AUDIO_MIXER_CHANNELS; i++) {
    if (am->channels[i] != 0) {
      audioemitter_update(&(am->channels[i]), dt);
    }
  }
}

int audiomixer_play(AudioMixer *am, sound *sound, float angle, float volume) {
  for (int i = 0; i < AUDIO_MIXER_CHANNELS; i++) {
    if (am->channels[i] == 0) {
      am->channels[i] = audioemitter_create(sound, angle, volume, i);
      return am->channels[i]->uid;
    }
  }

  // this starts the procedure for sound replacement

  float volume_min = am->channels[0]->volume;
  int volume_min_index = 0;
  for (int i = 1; i < AUDIO_MIXER_CHANNELS; i++) {
    if (am->channels[i]->volume < volume_min) {
      volume_min = am->channels[i]->volume;
      volume_min_index = i;
    }
  }
  audioemitter_free(am->channels[volume_min_index]);
  am->channels[volume_min_index] =
      audioemitter_create(sound, angle, volume, volume_min_index);
  return am->channels[volume_min_index]->uid;
}
