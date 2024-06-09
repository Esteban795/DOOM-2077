#include "../../include/ui/event_handler.h"
#include "../../include/core/game_states.h"
#include "../../include/core/engine.h"
#include "../../include/remote.h"
#include <string.h>

#define GET_KEYBIND_LEFT ((UITextBox*)e->uimodules[2]->elements[6]->element)->text
#define GET_KEYBIND_RIGHT ((UITextBox*)e->uimodules[2]->elements[8]->element)->text
#define GET_KEYBIND_UP ((UITextBox*)e->uimodules[2]->elements[10]->element)->text
#define GET_KEYBIND_DOWN ((UITextBox*)e->uimodules[2]->elements[12]->element)->text
#define GET_KEYBIND_RELOAD ((UITextBox*)e->uimodules[2]->elements[14]->element)->text
#define GET_KEYBIND_INTERACT ((UITextBox*)e->uimodules[2]->elements[16]->element)->text

#define GET_PLAYER_NAME (((UITextBox*)e->uimodules[3]->elements[6]->element)->text)
#define GET_SERVER_IP (((UITextBox*)e->uimodules[3]->elements[8]->element)->text)
#define GET_SETTING_SENS (((UITextBox*)e->uimodules[2]->elements[18]->element)->text)
#define GET_SETTING_FOV (((UITextBox*)e->uimodules[2]->elements[20]->element)->text)
#define GET_TEXTBOX ((UITextBox*)e->uimodules[3]->elements[3]->element)

#ifndef SERVER_PORT
#define SERVER_PORT 9999
#endif

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
  if (strlen(GET_PLAYER_NAME) == 0 || strlen(GET_SERVER_IP) == 0) {
    return;
  }

  printf("Joining server %s as %s...\n", GET_SERVER_IP, GET_PLAYER_NAME);
  strncpy(e->player_name, GET_PLAYER_NAME, 127);
  
  // Try to connect to the server
  // but before a very wrongful way to reset the engine (while it's running)
  engine_reset(e);
  remote_init(e->remote, GET_SERVER_IP, SERVER_PORT, (char*)e->player_name);
  e->state = STATE_INGAME;
  game_states_free[STATE_MENU](e);

  // Wait for the connection to be established
  uint64_t old = SDL_GetTicks();
  while(SDL_GetTicks() - old < 5000) {
    int status = remote_update(e, e->remote);
    if (e->remote->connected == 1) {
      e->remote->connected = 2;
      break; // Connection established
    } else if (e->remote->connected == -2) {
      printf("Error at remote connection\n");
      break;
    } else if (e->remote->connected == -1) {
      break; // Solo mode
    }
    if (status < 0) {
      printf("Error while initializing the remote sync...");
    }
  }
  if (e->remote->connected < 2) {
    printf("Connection to server failed! Pursuing in solo...\n");
    e->remote->connected = -1;
    e->remote->player_id = 0;
    read_map(e, "E1M3");
  } else {
    printf("Connection to server successful!\n");
  }
}

void UIECSendChat(engine *e){
  if (strlen(GET_TEXTBOX->text) == 0) {
    return;
  }

  remote_send_chat(e, GET_TEXTBOX->text);
  
  GET_TEXTBOX->text[0] = '\0';
  GET_TEXTBOX->text_index = 0;
}

void UIECSaveSettings(engine *e){
  keybind_modify(e->p->keybinds,"MOVE_LEFT",GET_KEYBIND_LEFT);
  keybind_modify(e->p->keybinds,"MOVE_RIGHT",GET_KEYBIND_RIGHT);
  keybind_modify(e->p->keybinds,"MOVE_FORWARD",GET_KEYBIND_UP);
  keybind_modify(e->p->keybinds,"MOVE_BACKWARD",GET_KEYBIND_DOWN);
  keybind_modify(e->p->keybinds,"RELOAD",GET_KEYBIND_RELOAD);
  keybind_modify(e->p->keybinds,"INTERACT",GET_KEYBIND_INTERACT);
  keybinds_write(KEYBINDS_FILE,e->p->keybinds);
}

