#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include "../WAD/sound.h"
#include "../settings.h"
#include "../util/geometry.h"
#include "emitter.h"

#include "SDL2/SDL_mixer.h"

/// Auxio mixer
///
/// ┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓
/// ┃ HOW TO USE THE AUDIO MIXER:                             ┃
/// ┃ - fetch the Sound object you want through wad data      ┃
/// ┃ (hardcode the index of the sound you want)              ┃
/// ┃ - run `audiomixer_play(e->mixer, <pointer to Sound>,    ┃
/// ┃ angle, volume)`                                         ┃
/// ┃ = angle is a float between 0 and 2*pi, OR -1            ┃
/// ┃ > 0 is the front                                        ┃
/// ┃ > -1 means non postional                                ┃
/// ┃ = volume is between 0 and 1 and is used for priority    ┃
/// ┃ when there are many                                     ┃
/// ┃ sounds being played                                     ┃
/// ┃ - it returns an uid for you to kill the sound if        ┃
/// ┃ needed, wth                                             ┃
/// ┃ `audiomixer_kill`                                       ┃
/// ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛
typedef struct _AudioMixer {
  AudioEmitter *channels[AUDIO_MIXER_CHANNELS];
} AudioMixer;

AudioMixer *audiomixer_init();
void audiomixer_free(AudioMixer *am);
void audiomixer_update(AudioMixer *am, int dt);
/// \remark audiomixer_play returns an integer corresponding the uid of the
/// sound you can pass it on to audiomixer_kill to stop it
int audiomixer_play(AudioMixer *am, sound *sound, float angle, float volume);
void audiomixer_kill(AudioMixer *am, int uid);

double get_audio_gain(double distance);

#endif
