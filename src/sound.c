#include "../include/sound.h"
#include <stdio.h>

sound_entry* SOUNDS_TO_PLAY[MAX_SOUNDS_PLAYING];
int SOUNDS_INDEX = 0;

sound read_sound(FILE *f, char *name, int offset) {
  sound s;
  s.name = name;
  s.format_number = 3;
  s.sample_rate = read_u16(f, offset + 2);
  s.sample_count = read_u32(f, offset + 4);
  s.samples = malloc(sizeof(u8) * s.sample_count);
  offset += 18;
  for (int i = 0; i < s.sample_count; i++) {
    s.samples[i] = read_u8(f, offset + i);
  }
  return s;
}

// we are reading the Sound (Doom Format) version
int count_sounds(FILE *f, lump *directory, int directory_size) {
  int count = 0;
  for (int i = 0; i < directory_size; i++) {
    i16 type = read_i16(f, directory[i].lump_offset);
    if (type == 3 && directory[i].lump_name[0] == 'D') { // idfk how to check if it's a sound better than that lmao
      count++;
    }
  }
  return count;
}

void sounds_free(sound *sounds, int sounds_count) {
  for (int i = 0; i < sounds_count; i++) {
    free(sounds[i].samples);
  }
  free(sounds);
}

sound *get_sounds(FILE *f, lump *directory, int directory_size,
                  int *sounds_count) {
  *sounds_count = count_sounds(f, directory, directory_size);
  sound *sounds = malloc(sizeof(sound) * *sounds_count);
  if (sounds == NULL) {
    printf("Error allocating memory for sounds\n");
    exit(1);
  }
  int index = 0;
  for (int i = 0; i < directory_size; i++) {
    i16 type = read_i16(f, directory[i].lump_offset);
    if (type == 3 && directory[i].lump_name[0] == 'D') {
      sounds[index] =
          read_sound(f, directory[i].lump_name, directory[i].lump_offset);
      index++;
    }
  }
  return sounds;
}


sound* get_sound_by_name(sound* sounds, int sounds_count, char* name) {
  for (int i = 0; i < sounds_count; i++) {
    if (strcmp(sounds[i].name, name) == 0) {
      return &sounds[i];
    }
  }
  return NULL;
}

double get_audio_gain(double distance) {
  distance = min(AL_MAX_DISTANCE, max(AL_REFERENCE_DISTANCE, distance));
  double gain = (1 - AL_ROLLOFF_FACTOR * (distance - AL_REFERENCE_DISTANCE) /
                         (AL_MAX_DISTANCE - AL_REFERENCE_DISTANCE));
  return gain;
}

sound_entry* add_sound_to_play(char* sound,double x, double y) {
  if (SOUNDS_INDEX >= MAX_SOUNDS_PLAYING) {
    return NULL;
  }
  sound_entry* se = malloc(sizeof(sound_entry));
  se->sound = sound;
  se->x = x;
  se->y = y;
  SOUNDS_TO_PLAY[SOUNDS_INDEX] = se;
  SOUNDS_INDEX++;
  return se;
}