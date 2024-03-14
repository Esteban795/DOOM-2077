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

enum param_type { PlayerKeybind, PlayerSetting };

void write_keybinds(const char *fp, player_keybind *settings);

void modify_keybind(player_keybind *keybinds, char *name, char *key);

player_keybind *get_player_keybinds(const char *fp);

void free_keybinds(player_keybind *keybinds);

SDL_Keycode get_key_from_action(player_keybind *keybinds, char *action);

void write_settings(const char *fp, player_setting *settings);

void modify_setting(player_setting *settings, char *name, char *key);

player_setting *get_player_settings(const char *fp);

void free_settings(player_setting *settings);

double get_setting_from_name(player_setting *settings, char *name);
#endif