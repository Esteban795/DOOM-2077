#ifndef UI_EVENT_HANDLER_H
#define UI_EVENT_HANDLER_H

#include "../structs.h"

#define GET_UIFEAD_CHAT(engine__) ((UIFeed*)(engine__)->uimodules[3]->elements[5]->element)
#define GET_UIFEAD_KILLFEED(engine__) ((UIFeed*)(engine__)->uimodules[2]->elements[0]->element)

typedef enum _UIEVENTCallback {
  UIEC_None = 0,
  UIEC_SetSubstate0,
  UIEC_SetSubstate1,
  UIEC_SetSubstate2,
  UIEC_SetSubstate3,
  UIEC_JoinServer,
  UIEC_SendChat,
  UIEC_SaveSettings,
} UIEVENTCallback;

void ui_handle_events(engine *e);
void UIECJoinServer(engine *e);
void UIECSendChat(engine *e);
void UIECSaveSettings(engine *e);

#endif
