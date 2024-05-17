#include "../include/sound.h"

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
