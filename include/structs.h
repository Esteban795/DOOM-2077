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

struct AnimationsArray;
struct Player;
struct BSP;
struct Engine;
struct SegmentHandler;
struct Weapon;
struct WeaponsArray;

typedef struct AnimationSprite{
  patch animation_sprite; //sprite de l'animation
  int y_difference; //Pour ajuster le sprite et ses layers par rapport à la nouvelle position de l'animation
  vec2 wanim_origin; //origine de l'animation
  vec2 wanim_pos;
  vec2 wanim_speed;
  int *layers_index; //tableau des index des layers de l'animation
  int layer_index_len; //longueur du tableau des layers
  bool (*linked_function)(struct Engine *e); /*fonction liée à l'animation,
       par exemple si il faut vérifier que le joueur a encore des munitions avant de charger le super shotgun */
  int duration; //durée de l'animation
} animation_sprite;

typedef struct AnimationsArray{
  animation_sprite *animation_sprites_array; //tableau de sprites
  int animation_len; //longueur de l'animation
  int animation_duration; //durée de l'animation
} animations_array;

struct Weapon {
  /*Identification de l'arme*/
  int id;            /*Identifie précisement l'arme*/
  char *weapon_name; /*Nom de l'arme du coup*/
  char *abbreviation; /*Abbréviation utilisée dans les fichiers WAD*/
  
  /*Animation*/
  animations_array *sprites; /*Tableau avec en i tous les sprites pour l'animation x*/


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

typedef struct weaponsArray_Cooldown_spray {
  double * cs;
  struct Player * p;
}WACS; 

struct Player {
  struct Engine *engine;
  thing thing;
  entity_t* entity;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
  double height;
  int *ammo; /*Array of size weapon_number that indicates the number of ammo by
                weapon (id)*/
  int t_last_shot; 
  bool has_attacked;
  int active_weapon;
  /*Animation de l'arme*/
  int life;
  //int cooldown; //nombre d'unités de temps nécéssaires avant de tirer , 0 indique qu'on peut tirer
  //double spray; //nombre d'unités de temps nécéssaires avant de ne plus avoir de spray, 0 indique que le tir sera parfaitement droit
  i16 subsector_id;
  WACS* cooldowns_sprays;

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
