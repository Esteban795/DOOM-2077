#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "../settings.h"
#include "../sound.h"
#include "emitter.h"

#include "SDL2/SDL_mixer.h"

typedef struct _AudioMixer {
  AudioEmitter *channels[AUDIO_MIXER_CHANNELS];
} AudioMixer;

AudioMixer *audiomixer_init();
void audiomixer_free(AudioMixer *am);
void audiomixer_update(AudioMixer *am, int dt);
// INFO: audiomixer_play returns an integer corresponding the uid of the sound
// you can pass it on to audiomixer_kill to stop it
int audiomixer_play(AudioMixer *am, sound *sound, float angle, float volume);
void audiomixer_kill(AudioMixer *am, int uid);

#endif
