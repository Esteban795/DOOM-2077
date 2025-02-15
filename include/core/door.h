#ifndef DOOR_H
#define DOOR_H

#include "../WAD/sector.h"
#include "../WAD/sidedef.h"
#include "../WAD/sound.h"
#include "../audio/mixer.h"
#include "../ecs/entity.h"
#include "../util/byte_reader.h"
#include "../util/geometry.h"

#define MINIMUM_FLOOR_HEIGHT -512

/// WAD Door Types
/// By default, P1 / PR and W1 / WR will still be activated in deathmatch
///
/// What the abbreviations mean:
/// OWC = open wait close
/// OSO = open stay open
/// CSC = close stay closed
/// CWO = close wait open
/// S = slow
/// F = fast
/// SR = switchable, retriggerable
/// S1 = switchable, non-retriggerable
/// WR = walkover, retriggerable
/// W1 = walkover, non-retriggerable
/// GR = gun, retriggerable
/// G1 = gun, non-retriggerable
/// PR = pushed, retriggerable
/// P1 = pushed, non-retriggerable
/// DR = door with key
enum DoorTypes {
  CSC_SR_S = 42,
  CSC_SR_F = 116,
  CSC_S1_S = 50,
  CSC_S1_F = 113,
  CSC_WR_S = 75,
  CSC_WR_F = 107,
  CSC_W1_S = 3,
  CSC_W1_F = 110,
  CWO_SR_S = 196,
  CWO_S1_S = 175,
  CWO_WR_S = 76,
  CWO_W1_S = 16,
  OSO_P1_S = 31,
  OSO_P1_F = 118,
  OSO_SR_S = 61,
  OSO_SR_F = 115,
  OSO_S1_S = 103,
  OSO_S1_F = 112,
  OSO_WR_S = 86,
  OSO_WR_F = 106,
  OSO_W1_S = 2,
  OSO_W1_F = 109,
  OSO_GR_S = 46,
  OWC_PR_S = 1,
  OWC_PR_F = 117,
  OWC_SR_S = 63,
  OWC_SR_F = 114,
  OWC_S1_S = 29,
  OWC_S1_F = 111,
  OWC_WR_S = 90,
  OWC_WR_F = 105,
  OWC_W1_S = 4,
  OWC_W1_F = 108,
  DR_RED_KEY = 28,
  DR_YELLOW_KEY = 27,
  DR_BLUE_KEY = 26,
};

enum DoorTransitionSpeed { // percentage of total height achieved per second
  NO_SPEED = 0,
  SLOW = 1,
  FAST = 2,
  TURBO = 3,
};

/// Describes how the door behaves
enum DoorFunction {
  OPEN_WAIT_CLOSE,
  OPEN_STAY_OPEN,
  CLOSE_STAY_CLOSED,
  CLOSE_WAIT_OPEN
};

/// Describes the state of an in-game door
struct Door {
  /// The corresponding entity_id of the door in the ECS.
  /// This is only used by the server.
  uint64_t id;
  enum DoorTransitionSpeed speed;
  enum DoorFunction function;
  /// wait time until the door switches to the next state
  int wait_time;
  /// time elapsed since the door switched to this state
  int time_elapsed;
  /// 0 = closed, 1 = open
  bool state;
  /// 0 = closed, 1 = open
  bool init_state;
  bool is_switching;
  int high_height;
  int low_height;
  sector *sector;
  /// ONLY USED BY LINEDEFS TO STORE SEVERAL DOORS IF THEY CAN ACTIVATE MULTIPLE
  /// DOORS
  struct Door *next_door;
};

typedef struct Door door;

void door_timeout(door *d, int DT);

void door_update(door *d, int DT);

void door_update_height(door *d, int high_height);

bool door_trigger_switch(door *d);

void doors_free(door **doors, int len_doors);

door *door_create(uint64_t id, enum DoorTransitionSpeed speed,
                  enum DoorFunction function, int wait_time, sector *sector,
                  bool init_state);

door *add_door(door *head, door *new_door);

bool is_a_door(u16 l);

bool is_a_direct_door(u16 l, u16 sector_tag);
#endif