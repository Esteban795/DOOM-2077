#include <time.h>
#include <stdint.h>

#ifndef STRUCTS_H
#define STRUCTS_H

#ifndef _LIB_SDL_NET_H
#define _LIB_SDL_NET_H
#include <SDL2/SDL_net.h>
#endif

#include "audio/mixer.h"
#include "keybindings.h"
#include "settings.h"
#include "remote.h"
#include "ui/common.h"
#include "ui/module.h"
#include "vec2.h"
#include "wad_data.h"
#include "ecs/world.h"

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
  thing thing;
  entity_t* entity;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
};

struct RemoteServer {
    IPaddress addr;
    UDPsocket socket;
    UDPpacket* packet;
    struct timespec next_tick;
    int connected; // 0: not connected (waiting for connection), 1: handshake completed, 2: connected, -1: disconnected, -2: error
    uint64_t player_id;
};

struct Engine {
  SDL_Renderer *renderer;
  const char *wadPath;
  int DT; // time used to render last frame

  struct WADData *wData;
  struct Player *p;
  struct BSP *bsp;
  struct SegmentHandler *seg_handler;
  struct RemoteServer *remote;
  entity_t** players;
  AudioMixer* mixer;
  Uint32 pixels[WIDTH * HEIGHT];
  SDL_Texture *texture;
  GameState state;
  
  
  door** doors;
  int substate;
  // INFO: substate is used for UI stuff
  // substates are defined in ui/def.c for each state!
  int num_doors;
  UIModule **uimodules;
  int nuimodules;
  int uinextevent;
  
  lift** lifts;
  int len_lifts;
  world_t *world;
  char player_name[128];
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
typedef struct RemoteServer remote_server_t;

#endif
