#include "../../include/ui/feed.h"

UIFeed* uifeed_create(UILabel** uil, int fl, bool reversed){
  UIFeed* uif = malloc(sizeof(UIFeed));

  uif->feed_length = fl;
  uif->reversed = reversed;
  uif->associated_labels = uil;

  return uif;
}

void uifeed_free(UIFeed* uif){
  free(uif->associated_labels);
  free(uif);
}

void uifeed_append(UIFeed* f, char* message){
  for (int i = f->feed_length-2; i >= 0; i--)
  {
    f->associated_labels[i+1]->string = f->associated_labels[i]->string;
  }
  f->associated_labels[0]->string = message;
}
