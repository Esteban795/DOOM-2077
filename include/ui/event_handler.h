#ifndef UI_EVENT_HANDLER_H
#define UI_EVENT_HANDLER_H

#include "../structs.h"

typedef enum _UIEVENTCallback {
  UIEC_None = 0,
  UIEC_SetSubstate0,
  UIEC_SetSubstate1,
  UIEC_SetSubstate2,
  UIEC_SetSubstate3
} UIEVENTCallback;

void ui_handle_events(engine *e);

#endif
