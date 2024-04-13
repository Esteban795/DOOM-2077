#include <time.h>

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
#include "vec2.h"
#include "wad_data.h"
#include "ecs/world.h"

struct Player;
struct BSP;
struct Engine;
struct MapRenderer;
struct SegmentHandler;
struct Weapon;
struct WeaponsArray;

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

struct WeaponsArray{
    int weapons_number;
    struct Weapon** weapons;
};
struct Player {
  struct Engine *engine;
  thing thing;
  vec2 pos;
  double angle;
  struct PlayerSetting *settings;
  struct PlayerKeybind *keybinds;
  double height;
  int* ammo; /*Array of size weapon_number that indicates the number of ammo by weapon (id)*/
  int active_weapon;
  int life;
  int cooldown;
};

struct RemoteServer {
    IPaddress addr;
    UDPsocket socket;
    UDPpacket* packet;
    struct timespec next_tick;
};

struct Engine {
  const char *wadPath;
  bool running;
  wad_data *wData;
  struct Player *p;
  struct BSP *bsp;
  struct MapRenderer *map_renderer;
  struct SegmentHandler *seg_handler;
  struct RemoteServer *remote;
  int numkeys;
  const uint8_t *keys;
  int DT;
  struct Player **players;
  AudioMixer *mixer;
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
typedef struct RemoteServer remote_server_t;
typedef struct Weapon weapon;
typedef struct WeaponsArray weapons_array;
typedef weapon** WeaponInventory;

struct SegmentHandler {
  struct Engine *engine;
  player *player;
  segment *seg;
  double raw_angle_1;
  int screen_range[WIDTH + 1];
  double upper_clip[WIDTH + 1];
  double lower_clip[WIDTH + 1];
  size_t screen_range_count;
};

typedef struct SegmentHandler segment_handler;
#endif
