#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

#include "keybindings.h"
#include "wad_data.h"

struct Player;
struct BSP;
struct Engine;
struct MapRenderer;
struct Weapon;

struct Weapon{
    /*Identification de l'arme*/
    int id; /*Identifie précisement l'arme*/
    char* weapon_name; /*Nom de l'arme du coup*/
    char* sprite; /*Fichier contenant le/les sprite de l'arme*/

    /*Spécification de l'arme*/
    int magsize; /*Taille du chargeur*/
    int max_damage; /*Dégats max possible par balle*/
    int min_damage; /*Dégats min possible*/
    double fire_rate; /*Nombre de balle tirées/s*/
    double spray; /*Potentiel rayon de dispersion*/
    int ammo_bounce; /*Nombre de rebond sur les murs*/
    int ammo_id; /*ID des (types de) munitions utilisées */
    int type; /*Eventuellement si on veut rajouter/classifier les armes (melee vs range, hitscan vs projectile....)*/
};
struct Player {
  struct Engine *engine;
  thing thing;
  double x;
  double y;
  double angle;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
  WeaponInventory weapons;
};

struct Engine {
  const char *wadPath;
  bool running;
  wad_data *wData;
  struct Player *p;
  struct BSP *bsp;
  struct MapRenderer *map_renderer;
  int numkeys;
  const uint8_t *keys;
};

struct BSP {
  struct Engine *engine;
  struct Player *player;
  node *nodes;
  subsector *subsectors;
  segment *segments;
  size_t root_node_id;
};

struct MapRenderer {
  struct Engine *engine;
  SDL_Renderer *renderer;
  wad_data *wData;
  vertex *vertexes;
  linedef *linedefs;
  bbox map_bounds;
};


typedef struct Player player;
typedef struct Engine engine;
typedef struct BSP bsp;
typedef struct MapRenderer map_renderer;
typedef struct Weapon weapon;
typedef weapon** WeaponInventory;

struct Color {
  int r;
  int g;
  int b;
};

typedef struct Color color;

#endif
