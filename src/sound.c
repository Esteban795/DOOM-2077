#include "../include/sound.h"


sound read_sound(FILE *f,char* name, int offset) {
  sound s;
  s.name = name;
  s.format_number = 3;
  s.sample_rate = read_u16(f, offset + 2);
  s.sample_count = read_u32(f, offset + 4);
  s.samples = malloc(sizeof(u8) * s.sample_count);
  for (int i = 0; i < s.sample_count; i++) {
    s.samples[i] = read_u8(f, offset + 8 + i);
  }
  return s;
}

sound* get_sounds_from_lump(FILE* f,lump l){
  return NULL;
}