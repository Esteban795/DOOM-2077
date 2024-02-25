#include "../include/keybindings.h"
#include <stdio.h>

#define BUFFER_SIZE 65536

int count_lines(FILE* file){
    char buf[BUFFER_SIZE];
    int counter = 0;
    while (true) {
        size_t res = fread(buf, 1, BUFFER_SIZE, file);
        if (ferror(file))
            return -1;
        size_t i;
        for(i = 0; i < res; i++) {
            if (buf[i] == '\n') counter++;
        }
        if (feof(file))
            break;
    }
    return counter;
}

char* copy_until_char(char* str, char c){
    size_t i = 0;
    while (str[i] != c){
        i++;
    }
    char* new_str = malloc(sizeof(char) * (i + 1));
    strncpy(new_str, str, i);
    new_str[i] = '\0';
    return new_str;
}

player_keybind* add_keybind(player_keybind* keybinds, char* name, char* key){
    if (keybinds == NULL){
        player_keybind* new_keybind = malloc(sizeof(player_keybind));
        new_keybind->name = name;
        new_keybind->key = SDL_GetKeyFromName(key);
        new_keybind->next = NULL;
        return new_keybind;
    } else {
        player_keybind* new_keybind = malloc(sizeof(player_keybind));
        new_keybind->name = name;
        new_keybind->key = SDL_GetKeyFromName(key);
        new_keybind->next = keybinds;
        return new_keybind;
    }
}

player_keybind* read_keybinds(FILE* f, size_t nlines){
    size_t line_len = 0;
    player_keybind* keybinds = malloc(sizeof(player_keybind));
    keybinds = NULL;
    char* lineptr = NULL;
    for (size_t i = 0; i < nlines; i++){
        ssize_t char_read = getline(&lineptr, &line_len, f);
        if (char_read == -1){
            printf("Error reading line in the config file.\n");
            exit(1);
        }
        size_t i = 0;
        while (lineptr[i] != '='){
            i++;
        }
        char* action = copy_until_char(lineptr, '=');
        char* key = copy_until_char(lineptr + i + 1, '\n');
        keybinds = add_keybind(keybinds, action, key);
        free(key);
    }
    free(lineptr);
    return keybinds;
}

void write_keybinds(const char* fp, player_keybind* settings){
    FILE* f = fopen(fp, "w");
    if (f == NULL){
        printf("Error opening config file\n");
        exit(1);
    }
    player_keybind* current = settings;
    while (current != NULL){
        fprintf(f, "%s=%s\n", current->name, SDL_GetKeyName(current->key));
        current = current->next;
    }
    fclose(f);
}

void modify_keybind(player_keybind* keybinds, char* name, char* key){
    player_keybind* current = keybinds;
    while (current != NULL){
        if (strcmp(current->name, name) == 0){
            current->key = SDL_GetKeyFromName(key);
            return;
        }
        current = current->next;
    }
}

player_keybind* get_player_keybinds(const char* fp){
    FILE* f = fopen(fp, "r");
    if (f == NULL){
        printf("Error opening config file\n");
        exit(1);
    }
    int nlines = 1 + count_lines(f);
    rewind(f);
    player_keybind* keybinds = read_keybinds(f,nlines);
    fclose(f);
    return keybinds;
}

void free_keybinds(player_keybind* keybinds){
    player_keybind* current = keybinds;
    while (current != NULL){
        player_keybind* next = current->next;
        free(current->name);
        free(current);
        current = next;
    }
}

void print_keybinds(player_keybind* keybinds){
    player_keybind* current = keybinds;
    while (current != NULL){
        printf("%s=%s\n", current->name, SDL_GetKeyName(current->key));
        current = current->next;
    }
}