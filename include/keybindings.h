#ifndef KEYBINDINGS_H
#define KEYBINDINGS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdio.h>

#define KEYBINDS_FILE "keybinds.cfg"
#define SETTINGS_FILE "settings.cfg"

struct PlayerKeybind {
  char *name;
  SDL_Keycode key;
  struct PlayerKeybind *next;
};

typedef struct PlayerKeybind player_keybind;

struct PlayerSetting {
  char *name;
  double value;
  struct PlayerSetting *next;
};

typedef struct PlayerSetting player_setting;

void keybinds_write(const char *fp, player_keybind *settings);

void keybind_modify(player_keybind *keybinds, char *name, char *key);

void keybinds_free(player_keybind *keybinds);

player_keybind *get_player_keybinds(const char *fp);

SDL_Keycode get_key_from_action(player_keybind *keybinds, char *action);

void settings_write(const char *fp, player_setting *settings);

void setting_modify(player_setting *settings, char *name, char *key);

player_setting *get_player_settings(const char *fp);

void settings_free(player_setting *settings);

double get_setting_from_name(player_setting *settings, char *name);
#endif