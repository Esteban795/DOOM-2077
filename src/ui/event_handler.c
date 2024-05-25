#include "../../include/ui/event_handler.h"
#include "../../include/game_states.h"

#define GET_KEYBIND_LEFT SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[6]->element)->text)
#define GET_KEYBIND_RIGHT SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[8]->element)->text)
#define GET_KEYBIND_UP SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[10]->element)->text)
#define GET_KEYBIND_DOWN SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[12]->element)->text)
#define GET_KEYBIND_RELOAD SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[14]->element)->text)
#define GET_KEYBIND_INTERACT SDL_GetScancodeFromName(((UITextBox*)e->uimodules[2]->elements[16]->element)->text)

#define GET_PLAYER_NAME (((UITextBox*)e->uimodules[3]->elements[6]->element)->text)
#define GET_SERVER_IP (((UITextBox*)e->uimodules[3]->elements[8]->element)->text)
#define GET_SETTING_SENS (((UITextBox*)e->uimodules[2]->elements[18]->element)->text)
#define GET_SETTING_FOV (((UITextBox*)e->uimodules[2]->elements[20]->element)->text)
#define GET_TEXTBOX ((UITextBox*)e->uimodules[3]->elements[3]->element)

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
    case UIEC_SaveSettings:
      UIECSaveSettings(e);
      break;
    }
    e->uinextevent = UIEC_None;
  }
}

void UIECJoinServer(engine *e){
  printf("Joining server %s...\n", GET_SERVER_IP);
  printf("Player name: %s\n", GET_PLAYER_NAME);
  switch_scene(e, STATE_INGAME);
}

void UIECSendChat(engine *e){
  printf("%s\n",GET_TEXTBOX->text);
  GET_TEXTBOX->text[0] = '\0';
  GET_TEXTBOX->text_index = 0;
}

void UIECSaveSettings(engine *e){

}

