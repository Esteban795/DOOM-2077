#include "../../include/ui/feed.h"

UIFeed* uifeed_create(UILabel** uil, int fl, int delay){
  UIFeed* uif = malloc(sizeof(UIFeed));

  uif->feed_length = fl;
  uif->default_delay = delay;
  uif->associated_labels = uil;
  uif->associated_delays = malloc(uif->feed_length * sizeof(int));

  for (int i = 0; i < uif->feed_length; i++){
    uif->associated_delays[i] = -1;
  }

  return uif;
}

void uifeed_free(UIFeed* uif){
  free(uif->associated_labels);
  free(uif->associated_delays);
  free(uif);
}

void uifeed_append(UIFeed* f, char* message){

  for (int i = f->feed_length-2; i >= 0; i--)
  {
    uilabel_set_content(f->associated_labels[i+1], f->associated_labels[i]->string);
    f->associated_delays[i+1] = f->associated_delays[i];
  }
  uilabel_set_content(f->associated_labels[0], message);
  f->associated_delays[0] = f->default_delay;
}

void uifeed_update(UIFeed* f, int dt){
  for (int i = 0; i < f->feed_length; i++){
    if (f->associated_delays[i] >= 0){
      f->associated_delays[i] -= dt;
      if (f->associated_delays[i] < 0){
        uilabel_set_content(f->associated_labels[i], "");
      }
    }
  }
}