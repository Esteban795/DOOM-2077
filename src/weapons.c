#include "../include/weapons.h"
#include "../include/game_states.h"

#include <assert.h>

#define ANIMATION_WIDTH 100
#define ANIMATION_HEIGTH 50

animations_array *init_animations_array(map_renderer *mr,char *abbreviation){
    animations_array *ar = malloc(3*sizeof(animations_array)); // A priori pas besoin de plus vu qu'on a que 2 animations 
                                                            //pour les armes : IDLE et animation + la partie qui se rajoute sur l'animation
    int m = strlen(abbreviation);
    int indexes[10]; // J'ai supposé qu'on aurait pas plus de 10 sprites pour l'animation
    for (int i = 0; i < 10; i++) {
        indexes[i] = -1;
    }
    int animation_len = 0;
    int all_sprites_len = 0;
    int i = 0;
    patch *sprites = mr->wData->sprites;
    char* to_cmp = malloc(10*sizeof(char));
    strcpy(to_cmp,sprites[i].patchname);
    to_cmp[m]='\0';
    while(!(strcmp(abbreviation,to_cmp)==0) && i<mr->wData->len_sprites){
        i++;
        strcpy(to_cmp,sprites[i].patchname);
        to_cmp[m]='\0';
    };
    
    int k = 0;
    while(strcmp(abbreviation,to_cmp)==0 && i<mr->wData->len_sprites){
        indexes[k] = i;
        k++;
        i++;
        strcpy(to_cmp,sprites[i].patchname);
        if (!(to_cmp[m] == 'F')){
            animation_len++;
        }
        all_sprites_len++;
        to_cmp[m]='\0';
    }
    if(i==mr->wData->len_sprites){
        printf("Erreur lors du chargement des sprites \n");
        return NULL;
    }
    patch *idle_anim_sprite = malloc(sizeof(patch));
    *idle_anim_sprite = sprites[indexes[0]];
    animations_array idle_anim = {.animation_sprites = idle_anim_sprite,.animation_len = 1};
    ar[IDLE] = idle_anim;

    patch *fire_anim_sprites = malloc(sizeof(patch)*animation_len-1);
    for(int j = 1; j<animation_len;j++){
        fire_anim_sprites[j-1] = sprites[indexes[j]];
    }
    animations_array fire_anim = {.animation_sprites = fire_anim_sprites,.animation_len = animation_len-1};
    ar[ANIMATION] = fire_anim;
    
    patch *layer_sprites = malloc(sizeof(patch)*(all_sprites_len - animation_len));
    for(int j = animation_len; j<all_sprites_len;j++){
        layer_sprites[all_sprites_len - j -1] = sprites[indexes[j]];
    }
    animations_array fire_layer = {.animation_sprites = layer_sprites,.animation_len = all_sprites_len - animation_len};
    ar[FIRE] = fire_layer;

    free(to_cmp);
    return ar;
}

weapon* init_one_weapon(map_renderer *mr,int id, char* weapon_name, char* abbreviation, int magsize, int max_damage, 
                            int min_damage, double fire_rate, double spray, int ammo_bounce, int ammo_id, int type){
    weapon* w = malloc(sizeof(weapon));
    w->id = id;
    w->weapon_name = strdup(weapon_name);
    w->abbreviation = strdup(abbreviation);
    w->sprites = init_animations_array(mr,abbreviation);
    w->magsize = magsize;
    w->max_damage = max_damage;
    w->min_damage = min_damage;
    w->fire_rate = fire_rate;
    w->spray = spray;
    w->ammo_bounce = ammo_bounce;
    w->ammo_id = ammo_id;
    w->type = type;
    return w;
}

void print_animations_patches(weapon *w){
    animations_array *aa = w->sprites;
    printf("Animation d'idle de base :%s \n",aa[0].animation_sprites->patchname);
    for(int j = 1; j<3;j++){
        for(int i = 0; i<aa[j].animation_len; i++){
            if(j>1){
                printf("Case %d \n", j);
            }
            printf("Animation/Layer %d de %s : %s \n", i, w->weapon_name,aa[j].animation_sprites[i].patchname);
        }
    }
}

weapons_array* init_weapons_array(map_renderer *mr){
    weapons_array* wa = malloc(sizeof(weapons_array));

    FILE* f = fopen("weapons.csv","r");
    if(f==NULL){
        perror("The file couldn't be opened \n");
        free(wa);
        return NULL;
    }

    int weapons_number;
    fscanf(f,"%d\n",&weapons_number);
    wa->weapons_number = weapons_number;

    // Skip the header line
    char header[2048];
    fgets(header, sizeof(header), f);
    printf("header: %s\n",header);

    WeaponInventory winv = malloc(weapons_number*sizeof(weapon*));
    for(int i = 0; i<weapons_number;i++){
        char line[1024];
        int id;
        char weapon_name[1024];
        char abbreviation[5];
        int magsize; 
        int max_damage; 
        int min_damage; 
        double fire_rate; 
        double spray; 
        int ammo_bounce; 
        int ammo_id; 
        int type; 

        fgets(line,sizeof(line),f);
        sscanf(line, "%d,%1023[^,],%4[^,],%d,%d,%d,%lf,%lf,%d,%d,%d", &id, weapon_name, abbreviation, &magsize, &max_damage, &min_damage, &fire_rate, &spray, &ammo_bounce, &ammo_id, &type);
        printf("id: %d, weapon_name: %s, abbreviation: %s, magsize: %d, max_damage: %d, min_damage: %d, fire_rate: %lf, spray: %lf, ammo_bounce: %d, ammo_id: %d, type: %d\n",id,weapon_name,abbreviation,magsize,max_damage,min_damage,fire_rate,spray,ammo_bounce,ammo_id,type);
        weapon* new_weapon = init_one_weapon(mr,id, weapon_name, abbreviation, magsize, max_damage, min_damage, fire_rate, spray, ammo_bounce, ammo_id, type);
        winv[i] = new_weapon;
    }
    wa->weapons = winv;

    if(fclose(f) !=0){
        perror("Error while closing the file");
        free(wa);
        return NULL;
    }
    return wa;
}

//Affiche le sprite a la bonne taille a l'endroit donné
void draw_weapon(map_renderer *mr, patch sprite, int x, int y) {
    
    
    SDL_Rect dest_rect;
    dest_rect.x = x;
    dest_rect.y = y;
    //printf("x: %d, y: %d\n", dest_rect.x, dest_rect.y);
    dest_rect.w = sprite.header.width * X_SCALE;
    dest_rect.h = sprite.header.height * Y_SCALE;

    SDL_RenderCopy(mr->renderer, sprite.tex, NULL, &dest_rect);
}

void update_animation(map_renderer *mr){
    player *p = mr->engine->p;
    weapon *w = wa->weapons[p->active_weapon];
    player_keybind *kb = p->keybinds;
    bool forward = keys[get_key_from_action(kb, "MOVE_FORWARD")];
    bool left = keys[get_key_from_action(kb, "MOVE_LEFT")];
    bool backward = keys[get_key_from_action(kb, "MOVE_BACKWARD")];
    bool right_d = keys[get_key_from_action(kb, "MOVE_RIGHT")];
    bool attack = keys[get_key_from_action(kb, "ATTACK")];

    bool move = forward || left || backward || right_d;
    if(attack || (time_elapsed_in_game - p->t_last_shot <= p->cooldown && time_elapsed_in_game > p->cooldown)){
        if(time_elapsed_in_game - p->t_last_shot > p->cooldown){
            p->t_last_shot = time_elapsed_in_game;
        }
        fire_weapon_animation(mr,w);
    } else {
        idle_weapon_animation(mr,w,move);
    }  
}

void idle_weapon_animation(map_renderer *mr,weapon *w, bool is_moving){
    patch idle_sprite = w->sprites->animation_sprites[0];
    player *p = mr->engine->p;
    
    set_origin(p,idle_sprite);
    
    if(is_moving){
        if(p->wanim_speed.x == 0){
            p->wanim_speed.x = -2;
        }
        if(p->wanim_speed.y == 0){
            p->wanim_speed.y = -2;
        }
    } else {
        p->wanim_speed.x = 0;
        p->wanim_speed.y = 0;
        p->wanim_pos.x = p->wanim_origin.x;
        p->wanim_pos.y = p->wanim_origin.y;
    }

    int origin_x = p->wanim_origin.x;
    int origin_y = p->wanim_origin.y;
    int init_x = p->wanim_pos.x;
    int init_y = p->wanim_pos.y;
    int dx = p->wanim_speed.x;
    int dy = p->wanim_speed.y;

    if(init_x + dx > origin_x + ANIMATION_WIDTH/2 || init_x + dx < origin_x - ANIMATION_WIDTH/2){
        dx = -dx;
        p->wanim_speed.x = dx;
    }

    if(init_y + dy > origin_y + ANIMATION_HEIGTH/2 || init_y + dy < origin_y - ANIMATION_HEIGTH/2){
        dy = -dy;
        p->wanim_speed.y = dy;
    }
    p->wanim_pos.x += dx;
    p->wanim_pos.y += dy;

    draw_weapon(mr,idle_sprite,p->wanim_pos.x,p->wanim_pos.y);

}

void fire_weapon_animation(map_renderer *mr,weapon *w){
    player *p = mr->engine->p;
    animations_array *aa = w->sprites;
    patch *fire_anim = aa[ANIMATION].animation_sprites;
    int animation_len = aa[ANIMATION].animation_len;
    int time_elapsed = time_elapsed_in_game - p->t_last_shot;
    float one_frame_time = p->cooldown/(2*animation_len-2);
    int x = p->wanim_origin.x;
    int y = p->wanim_origin.y;
    printf("time_elapsed: %d, one_frame_time: %f\n", time_elapsed, one_frame_time);
    if(time_elapsed < one_frame_time){
        set_origin(p,fire_anim[0]);
        draw_weapon(mr,fire_anim[0],x,y);
    } else {
        int i = 1;
        while(time_elapsed > i*one_frame_time && i<aa[ANIMATION].animation_len){
            i++;
        }
        set_origin(p,fire_anim[i-1]);
        draw_weapon(mr,fire_anim[i-1],x,y);
    }
    
    if (w->id!=0){
        patch *fire_layer = aa[FIRE].animation_sprites;
    }
}

void update_weapons(map_renderer *mr){
    player *p = mr->engine->p;
    weapon *w = wa->weapons[p->active_weapon];
    player_keybind *kb = p->keybinds;
    bool fists = keys[get_key_from_action(kb, "FISTS")], pistol = keys[get_key_from_action(kb, "PISTOL")];
    //, shotgun = keys[get_key_from_action(kb, "SHOTGUN")], chaingun = keys[get_key_from_action(kb, "CHAINGUN")], rocket = keys[get_key_from_action(kb, "ROCKET")], plasma = keys[get_key_from_action(kb, "PLASMA")], bfg = keys[get_key_from_action(kb, "BFG")];
    if(fists){
        if(p->active_weapon != 0){
            switch_weapon(p,0);
        }
    }
    if(pistol){
        if(p->active_weapon != 1 && p->ammo[1] >= 0){
            switch_weapon(p,1);
        }
    }
    update_animation(mr);

}


void free_animations_array(weapon *w){
    animations_array *aa = w->sprites;
    for(int i = 0; i<3;i++){
        if (aa[i].animation_sprites != NULL){
            free(aa[i].animation_sprites);
        }
    }
    free(aa);
}

void free_weapons_array(weapons_array* wa){
    for(int i = 0; i<wa->weapons_number;i++){
        weapon *w = wa->weapons[i];
        if (w != NULL){
            free(w->weapon_name);
            free(w->abbreviation);
            free_animations_array(w);
            free(w);
        }
    }
    free(wa->weapons);
    free(wa);
}

void set_origin(player *p, patch sprite){
    p->wanim_origin.x = -sprite.header.x_offset * X_SCALE;
    p->wanim_origin.y = -sprite.header.y_offset * Y_SCALE;
}

void switch_weapon(player* p, int weapon_id){
    p->active_weapon = weapon_id;
    set_origin(p,wa->weapons[p->active_weapon]->sprites[0].animation_sprites[0]);
    p->wanim_pos.x = p->wanim_origin.x;
    p->wanim_pos.y = p->wanim_origin.y;
}

void add_weapon(player* p, int weapon_id,weapons_array* wa){
    /*If the player doesnt have the weapon and the weapon have a limited magsize*/
    if (p->ammo[weapon_id] < 0){
        printf("tout va  bien\n");
        assert(weapon_id < wa->weapons_number);
        assert(wa->weapons[weapon_id] != NULL);
        if (wa->weapons[weapon_id]->magsize > 0){
            printf("Prends ton chargeur gamin\n");
            p->ammo[weapon_id] = wa->weapons[weapon_id]->magsize;
        } else {
            printf("Munitions infinies G_G\n");
            p->ammo[weapon_id] = -2;
        }
        switch_weapon(p,weapon_id);
    }
}

void add_ammo(player* p, int weapon_id, int ammo){
    if (p->ammo[weapon_id] >= 0){
        p->ammo[weapon_id] += ammo;
    }
}
