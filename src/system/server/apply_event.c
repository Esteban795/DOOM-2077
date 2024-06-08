#include "../../../include/system/server/apply_event.h"
#include "../../../include/component/health.h"
#include "../../../include/component/position.h"
#include "../../../include/component/statistic.h"
#include "../../../include/core/spawnpoints.h"
#include "../../../include/ecs/world.h"
#include "../../../include/event/all.h"
#include "../../../include/net/packet/server.h"
#include "../../../include/net/tracked_connection.h"
#include "../../../include/server/state.h"
#include "../../../include/server/task.h"

const system_t APPLY_EVENT_SYSTEM = {
    .fn = apply_event,
};

/// Respawn task
///
/// Respawn a player after some time by TPing them elsewhere and reseting their
/// health.
///
/// @param _entity_id the id of the player in the ECS
void respawn_task(void *_entity_id) {
  uint64_t entity_id = *(uint64_t *)_entity_id;
  entity_t pid = ENTITY_BY_ID(entity_id);
  health_ct *health = (health_ct *)world_get_component(
      &SERVER_STATE->world, &pid, COMPONENT_TAG_HEALTH);
  if (health == NULL)
    return; // If the player does not have a position or health, cancel.
  if (health_get(health) > 0)
    return; // Cancel too if the player is alive.
  spawnpoint sp = get_random_spawnpoint();
  player_move_event_t *move_ev =
      ServerPlayerMoveEvent_new(entity_id, sp.x, sp.y, sp.z, sp.angle);
  player_health_event_t *health_ev =
      ServerPlayerHealthEvent_new(entity_id, 100.0, 100.0);
  world_queue_event(&SERVER_STATE->world, (event_t *)health_ev);
  world_queue_event(&SERVER_STATE->world, (event_t *)move_ev);

  // Send the move event to the player before it can spam us the death coords.
  uint8_t buf[256] = {0};
  int len = server_player_move(buf, entity_id, sp.x, sp.y, sp.z, sp.angle);
  broadcast(&SERVER_STATE->sock, SERVER_STATE->conns, SERVER_STATE->conn_count,
            buf, len);
}

/*
 * The first system in the chain, it applies the modifications to the world
 * based on the event. For example, if the event is a player moving, this system
 * will update the player's position in the world. This system is intented to be
 * used on the server side, and ensure the world and events are consistent.
 */
int apply_event(world_t *world, event_t *event) {
  switch (event->tag) {
  case SERVER_PLAYER_JOIN_EVENT_TAG: {
    // On join, we want to set their initial position and health.
    server_player_join_event_t *server_player_join_event =
        (server_player_join_event_t *)event;
    entity_t pid = ENTITY_BY_ID(server_player_join_event->entity_id);
    position_ct *pos =
        (position_ct *)world_get_component(world, &pid, COMPONENT_TAG_POSITION);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (pos == NULL || health == NULL)
      return -1; // If the player does not have a position or health, we cannot
                 // apply the event, cancel it.
    health_set(health, 100.0);
    health_set_max(health, 100.0);
    break;
  }
  case SERVER_PLAYER_MOVE_EVENT_TAG: {
    player_move_event_t *player_move_event = (player_move_event_t *)event;
    entity_t pid = ENTITY_BY_ID(player_move_event->entity_id);
    position_ct *pos =
        (position_ct *)world_get_component(world, &pid, COMPONENT_TAG_POSITION);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (pos == NULL)
      return -1; // If the player does not have a position, we cannot apply the
                 // event, cancel it.
    if (health != NULL && health_get(health) <= 0) {
      // Cancel the event if the player is dead.
      // player_move_event_t* new_move_ev =
      // ServerPlayerMoveEvent_new(player_move_event->entity_id, pos->x, pos->y,
      // pos->z, pos->angle); world_queue_event(world, (event_t*) new_move_ev);
      return -1;
    }
    pos->x = player_move_event->x;
    pos->y = player_move_event->y;
    pos->z = player_move_event->z;
    pos->angle = player_move_event->angle;
    break;
  }
  case SERVER_PLAYER_DAMAGE_EVENT_TAG: {
    player_damage_event_t *player_damage_event = (player_damage_event_t *)event;
    if (player_damage_event->source_entity_id != 0) {
      entity_t src_id = ENTITY_BY_ID(player_damage_event->source_entity_id);
      health_ct *src_hth = (health_ct *)world_get_component(
          world, &src_id, COMPONENT_TAG_HEALTH);
      if (src_hth != NULL && health_get(src_hth) <= 0) {
        // Cancel the event if the player is dead.
        return -1;
      }
    }
    entity_t pid = ENTITY_BY_ID(player_damage_event->entity_id);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (health == NULL)
      return -1; // If the player does not have health, we cannot apply the
                 // event, cancel it.
    if (health_get(health) <= 0)
      return -1; // If the player is already dead, don't trigger the death
                 // event.
    health_sub(health, player_damage_event->damage);
    if (health_get(health) <= 0) {
      event_t *kill_event = (event_t *)ServerPlayerKillEvent_new(
          player_damage_event->entity_id, player_damage_event->source_entity_id,
          player_damage_event->weapon_id);
      world_queue_event(world, kill_event);
      return -1; // If the player is dead, it is a kill event that we need to
                 // send, so we cancel this event.
    }
    break;
  }
  case SERVER_PLAYER_HEAL_EVENT_TAG: {
    player_heal_event_t *player_heal_event = (player_heal_event_t *)event;
    entity_t pid = ENTITY_BY_ID(player_heal_event->entity_id);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (health == NULL)
      return -1; // If the player does not have health, we cannot apply the
                 // event, cancel it.
    health_add(health, player_heal_event->gain);
    break;
  }
  case SERVER_PLAYER_HEALTH_EVENT_TAG: {
    player_health_event_t *player_health_event = (player_health_event_t *)event;
    entity_t pid = ENTITY_BY_ID(player_health_event->entity_id);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (health == NULL)
      return -1; // If the player does not have health, we cannot apply the
                 // event, cancel it.
    health_set(health, player_health_event->hp);
    health_set_max(health, player_health_event->max_hp);
    break;
  }
  case SERVER_PLAYER_KILL_EVENT_TAG: {
    player_kill_event_t *player_kill_event = (player_kill_event_t *)event;
    entity_t pid = ENTITY_BY_ID(player_kill_event->entity_id);
    health_ct *health =
        (health_ct *)world_get_component(world, &pid, COMPONENT_TAG_HEALTH);
    if (health == NULL)
      return -1; // If the player does not have health, we cannot apply the
                 // event, cancel it.
    health_set(health, 0);

    // Increment the deaths of the player.
    statistic_ct *stats = (statistic_ct *)world_get_component(
        world, &pid, COMPONENT_TAG_STATISTIC);
    if (stats == NULL)
      return -1; // If the player does not have stats, we cannot apply the
                 // event, cancel it.
    statistic_increment_deaths(stats, 1);

    // Increment the kills of the source player.
    if (player_kill_event->source_entity_id == 0)
      return 0; // If the source entity is 0 (NONE), then just return, no need
                // to increment kills.
    pid = ENTITY_BY_ID(player_kill_event->source_entity_id);
    stats = (statistic_ct *)world_get_component(world, &pid,
                                                COMPONENT_TAG_STATISTIC);
    if (stats == NULL)
      return -1; // If the player does not have stats, we cannot apply the
                 // event, cancel it.
    statistic_increment_kills(stats, 1);

    // Trigger a scoreboard update event.
    char *entries[10] = {0};
    uint16_t deaths[10] = {0};
    uint16_t kills[10] = {0};
    int count = scoreboard_generate(world, entries, deaths, kills);
    event_t *scoreboard_event = (event_t *)ServerScoreboardUpdateEvent_new(
        count, entries, deaths, kills);
    world_queue_event(world, scoreboard_event);

    // Schedule the respawn task
    uint64_t *eid = malloc(sizeof(uint64_t));
    *eid = player_kill_event->entity_id;
    Instant execute_at;
    INSTANT_NOW(&execute_at);
    if (player_kill_event->weapon_id == 3) {
      // Extreme death : 400ms
      long ms = 1000000000 - execute_at.tv_nsec;
      if (ms < 400000000) {
        execute_at.tv_nsec += 4000000000;
      } else {
        execute_at.tv_sec += 1;
        execute_at.tv_nsec = 4000000000 - ms;
      }
    } else {
      // Normal death : 1s
      execute_at.tv_sec += 1;
    }
    task_t respawn = task_new(respawn_task, (void *)eid, execute_at);
    task_executor_add(&SERVER_STATE->task_executor, respawn);
    break;
  }
  default:
    break;
  }
  return 0;
}
