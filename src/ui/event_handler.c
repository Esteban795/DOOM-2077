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
    case UIEC_JoinServer:
      UIECJoinServer(e);
      break;
    case UIEC_SendChat:
      UIECSendChat(e);
      break;
    }
    e->uinextevent = UIEC_None;
  }
}

void UIECJoinServer(engine *e){

}

#define GET_TEXTBOX ((UITextBox*)e->uimodules[3]->elements[3]->element)

void UIECSendChat(engine *e){
  printf("%s\n",GET_TEXTBOX->text);
  GET_TEXTBOX->text[0] = '\0';
}

