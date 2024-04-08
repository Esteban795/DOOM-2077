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
// TODO: update: countdown sounds, free sounds that are no longer playing
void audiomixer_update(AudioMixer *am, int dt);
// TODO: plays a sound in the mixer
// returns an uid (used for stopping sounds properly, works like js intervals)
int audiomixer_play(AudioMixer *am, sound *sound, float angle, float volume);
// TODO: stop a sound from its id
void audiomixer_kill(AudioMixer *am, int uid);

#endif
