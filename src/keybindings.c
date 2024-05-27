#include "../include/keybindings.h"

#define BUFFER_SIZE 65536

// faster than to fgets("\n")
int count_lines(FILE *file) {
  char buf[BUFFER_SIZE];
  int counter = 0;
  while (true) {
    size_t res = fread(buf, 1, BUFFER_SIZE, file);
    if (ferror(file))
      return -1;
    size_t i;
    for (i = 0; i < res; i++) {
      if (buf[i] == '\n')
        counter++;
    }
    if (feof(file))
      break;
  }
  return counter;
}

// copy a string until a certain character is found and return the new string
// equivalent of doing str[:i] in python
char *copy_until_char(char *str, char c) {
  size_t i = 0;
  while (str[i] != c) {
    i++;
  }
  char *new_str = malloc(sizeof(char) * (i + 1));
  strncpy(new_str, str, i);
  new_str[i] = '\0';
  return new_str;
}

///// Keybinds

// Keybinds are stored as linked list with the name of the action and the key
// that is bound to it
static player_keybind *keybind_add(player_keybind *keybinds, char *name,
                                   char *key) {
  if (keybinds == NULL) {
    player_keybind *new_keybind = malloc(sizeof(player_keybind));
    new_keybind->name = name;
    new_keybind->key = SDL_GetKeyFromName(key);
    new_keybind->next = NULL;
    return new_keybind;
  } else {
    player_keybind *new_keybind = malloc(sizeof(player_keybind));
    new_keybind->name = name;
    new_keybind->key = SDL_GetKeyFromName(key);
    new_keybind->next = keybinds;
    return new_keybind;
  }
}

// Reads keybinds from the config file, line by line.
// The format is "action=key"
player_keybind *keybinds_read(FILE *f, size_t nlines) {
  size_t line_len = 0;
  player_keybind *keybinds = NULL;
  char *lineptr = NULL;
  for (size_t i = 0; i < nlines; i++) {
    ssize_t char_read = getline(&lineptr, &line_len, f);
    if (char_read == -1) {
      // If the last line is empty, we don't want to exit, just ignore it.
      if (i == nlines - 1) break;
      printf("Error reading line in the keybinds config file.\n");
      exit(1);
    }
    size_t i = 0;
    while (lineptr[i] != '=') {
      i++;
    }
    char *action = copy_until_char(lineptr, '=');
    char *key = copy_until_char(lineptr + i + 1, '\n');
    if (!strcmp(key,"")) { // example : MOVE_FORWARD=        (nothing is present)
      printf("Error reading keybinds config file : missing keybinds.\n");
      exit(1);
    }
    keybinds = keybind_add(keybinds, action, key);
    free(key);
  }
  free(lineptr);
  return keybinds;
}

// Write the current keybinds in memory to the config file
void keybinds_write(const char *fp, player_keybind *settings) {
  FILE *f = fopen(fp, "w");
  if (f == NULL) {
    printf("Error opening keybinds config file\n");
    exit(1);
  }
  player_keybind *current = settings;
  while (current != NULL) {
    fprintf(f, "%s=%s\n", current->name, SDL_GetKeyName(current->key));
    current = current->next;
  }
  fclose(f);
}

// Modify a keybind in memory
void keybind_modify(player_keybind *keybinds, char *name, char *key) {
  if (strcmp(key, "") == 0) return;
  player_keybind *current = keybinds;
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      current->key = SDL_GetKeyFromName(key);
      return;
    }
    current = current->next;
  }
}

// Given an action, return the key that is bound to it
SDL_Keycode get_key_from_action(player_keybind *keybinds, char *action) {
  player_keybind *current = keybinds;
  while (current != NULL) {
    if (strcmp(current->name, action) == 0) {
      return SDL_GetScancodeFromKey(current->key);
    }
    current = current->next;
  }
  return -1; //action doesn't exist, do nothing.
}

player_keybind *get_player_keybinds(const char *fp) {
  FILE *f = fopen(fp, "r");
  if (f == NULL) {
    printf("Error opening keybinds config file\n");
    exit(1);
  }
  int nlines = 1 + count_lines(f);
  rewind(f);
  player_keybind *keybinds = keybinds_read(f, nlines);
  fclose(f);
  return keybinds;
}

void keybinds_free(player_keybind *keybinds) {
  player_keybind *current = keybinds;
  while (current != NULL) {
    player_keybind *next = current->next;
    free(current->name);
    free(current);
    current = next;
  }
}

///// Settings

player_setting *add_setting(player_setting *settings, char *name, char *key) {
  if (settings == NULL) {
    player_setting *new_setting = malloc(sizeof(player_setting));
    new_setting->name = name;
    new_setting->value = strtod(key, NULL);
    new_setting->next = NULL;
    return new_setting;
  } else {
    player_setting *new_setting = malloc(sizeof(player_setting));
    new_setting->name = name;
    new_setting->value = strtod(key, NULL);
    new_setting->next = settings;
    return new_setting;
  }
}

player_setting *read_settings(FILE *f, size_t nlines) {
  size_t line_len = 0;
  player_setting *settings = malloc(sizeof(player_setting));
  settings = NULL;
  char *lineptr = NULL;
  for (size_t i = 0; i < nlines; i++) {
    ssize_t char_read = getline(&lineptr, &line_len, f);
    if (char_read == -1) {
      printf("Error reading line in the settings config file.\n");
      exit(1);
    }
    size_t i = 0;
    while (lineptr[i] != '=') {
      i++;
    }
    char *name = copy_until_char(lineptr, '=');
    char *value = copy_until_char(lineptr + i + 1, '\n');
    if (!strcmp(value,"")) { // example : FOV=        (nothing is present)
      printf("Error reading settings config file : missing settings.\n");
      exit(1);
    }
    settings = add_setting(settings, name, value);
    free(value);
  }
  free(lineptr);
  return settings;
}

void settings_write(const char *fp, player_setting *settings) {
  FILE *f = fopen(fp, "w");
  if (f == NULL) {
    printf("Error opening settings config file\n");
    exit(1);
  }
  player_setting *current = settings;
  while (current != NULL) {
    fprintf(f, "%s=%s\n", current->name, SDL_GetKeyName(current->value));
    current = current->next;
  }
  fclose(f);
}

void setting_modify(player_setting *settings, char *name, char *key) {
  player_setting *current = settings;
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      current->value = strtod(key, NULL);
      return;
    }
    current = current->next;
  }
}

player_setting *get_player_settings(const char *fp) {
  FILE *f = fopen(fp, "r");
  if (f == NULL) {
    printf("Error opening settings config file\n");
    exit(1);
  }
  int nlines = 1 + count_lines(f);
  rewind(f);
  player_setting *settings = read_settings(f, nlines);
  fclose(f);
  return settings;
}

double get_setting_from_name(player_setting *settings, char *name) {
  player_setting *current = settings;
  while (current != NULL) {
    if (strcmp(current->name, name) == 0) {
      return current->value;
    }
    current = current->next;
  }
  return -1;
}

void settings_free(player_setting *settings) {
  player_setting *current = settings;
  while (current != NULL) {
    player_setting *next = current->next;
    free(current->name);
    free(current);
    current = next;
  }
}


///// DEBUG FUNCTIONS
void print_keybinds(player_keybind *keybinds) {
  player_keybind *current = keybinds;
  while (current != NULL) {
    printf("%s=%s\n", current->name, SDL_GetKeyName(current->key));
    current = current->next;
  }
}

void print_settings(player_setting *settings) {
  player_setting *current = settings;
  while (current != NULL) {
    printf("%s=%f\n", current->name, current->value);
    current = current->next;
  }
}