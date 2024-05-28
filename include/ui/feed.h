#ifndef UI_FEED_H
#define UI_FEED_H

#include "label.h"

#define UIFEED_LIMIT 128

typedef struct _UIFeed {
  int feed_length;
  UILabel** associated_labels;
  int *associated_delays;
  int default_delay;
} UIFeed;

UIFeed* uifeed_create(UILabel** uil, int fl, int delay);
void uifeed_free(UIFeed* f);
void uifeed_update(UIFeed* f, int dt);

void uifeed_append(UIFeed* f, char* message);

#endif