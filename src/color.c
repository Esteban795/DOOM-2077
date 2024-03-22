#include "../include/color.h"

color get_color(unsigned long text_hash, i16 light_level) {
  fast_srand(text_hash + light_level);
  return (color){
      .r = fast_rand() % 255, .g = fast_rand() % 255, .b = fast_rand() % 255, .a = 255};
}