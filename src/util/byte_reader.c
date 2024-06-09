#include "../../include/util/byte_reader.h"
#include <stdio.h>
#include <stdlib.h>

byte read_1_byte(FILE *f, int offset) {
  fseek(f, offset, 0);
  byte b = getc(f);
  return b;
}

byte *read_bytes(FILE *f, int offset, int num_bytes) {
  byte *bytes = malloc(sizeof(byte) * num_bytes);
  fseek(f, offset, 0);
  int bytes_read = (int)fread(bytes, sizeof(byte), num_bytes, f);
  if (bytes_read < num_bytes) {
    printf("Error while reading bytes : got fewer bytes than expected");
    exit(1);
  }
  return bytes;
}

// WAD files use little endianness

i8 read_i8(FILE *f, int offset) {
  fseek(f, offset, 0);
  i8 b = (i8)getc(f);
  return b;
}

i16 read_i16(FILE *f, int offset) {
  byte *bytes = read_bytes(f, offset, 2);
  i16 temp = 0;
  temp = bytes[1] << 8;
  temp |= bytes[0];
  free(bytes);
  return temp;
}

u16 read_u16(FILE *f, int offset) {
  byte *bytes = read_bytes(f, offset, 2);
  u16 temp = 0;
  temp = bytes[1] << 8;
  temp |= bytes[0];
  free(bytes);
  return temp;
}

i32 read_i32(FILE *f, int offset) {
  byte *bytes = read_bytes(f, offset, 4);
  i32 temp = 0;
  temp = bytes[3] << 24;
  temp |= bytes[2] << 16;
  temp |= bytes[1] << 8;
  temp |= bytes[0];
  free(bytes);
  return temp;
}

char *read_string(FILE *f, int offset, int num_bytes) {
  char *str = malloc(sizeof(char) * (num_bytes + 1));
  str[num_bytes] = '\0';
  byte *bytes = read_bytes(f, offset, num_bytes);
  for (int i = 0; i < num_bytes; i++) {
    str[i] = bytes[i];
  }
  free(bytes);
  return str;
}

char *read_texture_name(FILE *f, int offset, int length) {
  fseek(f, offset, 0);
  char *texture_name = malloc(sizeof(char) * (length + 1));
  for (int i = 0; i < length; i++) {
    texture_name[i] = (char)read_i8(f, offset + i);
  }
  texture_name[length] = '\0';
  return texture_name;
}

u8 read_u8(FILE *f, int offset) {
  fseek(f, offset, 0);
  u8 b = getc(f);
  return b;
}

u32 read_u32(FILE *f, int offset) {
  byte *bytes = read_bytes(f, offset, 4);
  u32 temp = 0;
  temp = bytes[3] << 24;
  temp |= bytes[2] << 16;
  temp |= bytes[1] << 8;
  temp |= bytes[0];
  free(bytes);
  return temp;
}