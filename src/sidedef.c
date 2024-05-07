#include "../include/sidedef.h"
#include <stdio.h>

bool test = false;
sidedef read_sidedef(FILE *f, int offset, sector *sectors,
                     texture_map *textures, int len_textures) {
  sidedef s;
  s.x_offset = read_i16(f, offset);
  s.y_offset = read_i16(f, offset + 2);
  char *upper_texture = read_string(f, offset + 4, 8);
  char *lower_texture = read_string(f, offset + 12, 8);
  char *middle_texture = read_string(f, offset + 20, 8);
  char* uppercased_upper_texture = malloc(9 * sizeof(char));
  char* uppercased_lower_texture = malloc(9 * sizeof(char));
  char* uppercased_middle_texture = malloc(9 * sizeof(char));
  strtoupper(uppercased_upper_texture, upper_texture);
  strtoupper(uppercased_lower_texture, lower_texture);
  strtoupper(uppercased_middle_texture, middle_texture);
  free(upper_texture);
  free(lower_texture);
  free(middle_texture);
  s.hash_lower = ElfHash(uppercased_lower_texture);
  s.hash_upper = ElfHash(uppercased_upper_texture);
  s.hash_middle = ElfHash(uppercased_middle_texture);
  s.upper_texture =
      s.hash_upper == NO_TEXTURE_HASH
          ? NULL
          : get_texture_from_name(textures, len_textures, uppercased_upper_texture);
  s.lower_texture =
      s.hash_lower == NO_TEXTURE_HASH
          ? NULL
          : get_texture_from_name(textures, len_textures, uppercased_lower_texture);
  s.middle_texture =
      s.hash_middle == NO_TEXTURE_HASH
          ? NULL
          : get_texture_from_name(textures, len_textures, uppercased_middle_texture);
  s.sector_id = read_i16(f, offset + 28);
  s.sector = &sectors[s.sector_id];
  free(uppercased_upper_texture);
  free(uppercased_lower_texture);
  free(uppercased_middle_texture);
  return s;
}

sidedef *get_sidedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_sidedefs, sector *sectors,
                                texture_map *textures, int len_textures) {
  int offset = 0;
  lump lump_info = directory[lump_index];
  sidedef *sidedefs = malloc(sizeof(sidedef) * len_sidedefs);
  for (int i = 0; i < len_sidedefs; i++) {
    offset = lump_info.lump_offset + i * num_bytes + header_length;
    sidedefs[i] = read_sidedef(f, offset, sectors, textures, len_textures);
  }
  return sidedefs;
}

void sidedefs_free(sidedef *sidedefs) {
  free(sidedefs);
}