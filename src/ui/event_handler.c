#include "../../include/ui/event_handler.h"

void ui_handle_events(engine *e) {
  if (e->uinextevent != UIEC_None) {
    switch (e->uinextevent) {
    case UIEC_SetSubstate0:
      e->substate = 0;
      break;
    case UIEC_SetSubstate1:
      e->substate = 1;
      break;
    case UIEC_SetSubstate2:
      e->substate = 2;
      break;
    case UIEC_SetSubstate3:
      e->substate = 3;
      break;
    }
    e->uinextevent = UIEC_None;
  }
}
