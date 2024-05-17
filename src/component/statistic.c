#include <stdlib.h>

#include "../../include/ecs/component.h"
#include "../../include/component/statistic.h"

const int COMPONENT_TAG_STATISTIC = 5;

component_t* statistic_create(int kills, int deaths) {
    statistic_ct* statistic_component = malloc(sizeof(statistic_ct));
    statistic_component->kills = kills;
    statistic_component->deaths = deaths;
    statistic_component->tag = COMPONENT_TAG_STATISTIC;
    return (component_t*) statistic_component;
}

extern inline int statistic_get_kills(statistic_ct* component);
extern inline int statistic_get_deaths(statistic_ct* component);
extern inline int statistic_increment_kills(statistic_ct* component, int increment);
extern inline int statistic_increment_deaths(statistic_ct* component, int increment);
extern inline void statistic_reset(statistic_ct* component);