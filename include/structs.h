#ifndef STRUCTS_H
#define STRUCTS_H

#include <SDL2/SDL.h>

#include "audio/mixer.h"
#include "keybindings.h"
#include "settings.h"
#include "vec2.h"
#include "wad_data.h"

#define STATE_COUNT 2

typedef enum {
  STATE_MENU,
  STATE_INGAME,
} GameState;

struct Player;
struct BSP;
struct Engine;
struct SegmentHandler;
struct Weapon;
struct WeaponsArray;

struct Weapon {
  /*Identification de l'arme*/
  int id;            /*Identifie précisement l'arme*/
  char *weapon_name; /*Nom de l'arme du coup*/
  char *sprite;      /*Fichier contenant le/les sprite de l'arme*/

  /*Spécification de l'arme*/
  int magsize;      /*Taille du chargeur*/
  int max_damage;   /*Dégats max possible par balle*/
  int min_damage;   /*Dégats min possible*/
  double fire_rate; /*Nombre de balle tirées/s*/
  double spray;     /*Potentiel rayon de dispersion*/
  int ammo_bounce;  /*Nombre de rebond sur les murs*/
  int ammo_id;      /*ID des (types de) munitions utilisées */
  int type; /*Eventuellement si on veut rajouter/classifier les armes (melee vs
               range, hitscan vs projectile....)*/
};

struct WeaponsArray {
  int weapons_number;
  struct Weapon **weapons;
};

struct Player {
  struct Engine *engine;
  thing thing; // thing associated with the player. It is wadData->things[0] by default
  vec2 pos;
  double angle;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
  double height;
  int *ammo; /*Array of size weapon_number that indicates the number of ammo by
                weapon (id)*/
  int active_weapon;
  int life;
  int cooldown;
};

struct Engine {
  SDL_Renderer *renderer;
  const char *wadPath;
  bool running;
  int DT; // time used to render last frame

  struct WADData *wData;
  struct Player *p;
  struct BSP *bsp;
  struct SegmentHandler *seg_handler;
  struct Player** players;
  AudioMixer* mixer;
  Uint32 pixels[WIDTH * HEIGHT];
  SDL_Texture *texture;
  GameState state;
  
  
  door** doors;
  int num_doors;
  
  lift** lifts;
  int len_lifts;
};

struct BSP {
  struct Engine *engine;
  struct Player *player;
  node *nodes;
  subsector *subsectors;
  segment *segments;
  size_t root_node_id;
};

struct SegmentHandler {
  struct Engine *engine;
  struct Player *player;
  segment *seg;
  double raw_angle_1;
  int screen_range[WIDTH + 1];
  double upper_clip[WIDTH + 1];
  double lower_clip[WIDTH + 1];
  size_t screen_range_count;
}; 


typedef struct Player player;
typedef struct Engine engine;
typedef struct BSP bsp;
typedef struct SegmentHandler segment_handler;
typedef struct Weapon weapon;
typedef struct WeaponsArray weapons_array;
typedef weapon **WeaponInventory;

#endif
