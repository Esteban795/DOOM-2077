#ifndef _LIB_DOOM_COMPONENT_STATISTIC_H
#define _LIB_DOOM_COMPONENT_STATISTIC_H

#include "../ecs/component.h"

/// The tag for the statistic component.
extern const int COMPONENT_TAG_STATISTIC;

/**
 * statistic_ct is a component that represents the statistic of a player.
 * It contains all the informations that are displayed on the scoreboard.
 */
typedef struct {
  /// The tag of the component
  /// This should be COMPONENT_TAG_STATISTIC
  int tag;
  /// The recorded number of kills
  int kills;
  /// The recorded number of deaths
  int deaths;
} statistic_ct;

/// Create a new statistic component
component_t *statistic_create(int kills, int deaths);

/// Get the number of kills
inline int statistic_get_kills(statistic_ct *component) {
  return component->kills;
}

/// Get the number of deaths
inline int statistic_get_deaths(statistic_ct *component) {
  return component->deaths;
}

/// Increment the number of kills
inline int statistic_increment_kills(statistic_ct *component, int increment) {
  component->kills += increment;
  return component->kills;
}

/// Increment the number of deaths
inline int statistic_increment_deaths(statistic_ct *component, int increment) {
  component->deaths += increment;
  return component->deaths;
}

/// Reset the statistics to 0
inline void statistic_reset(statistic_ct *component) {
  component->kills = 0;
  component->deaths = 0;
}

#endif