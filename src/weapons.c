#include "../include/game_states.h"

#include <jansson.h>
#include <assert.h>
#include <stdio.h>

#include "../include/weapons.h"
#include "../include/player.h"
#include "../include/component/position.h"

#define ANIMATION_WIDTH 100
#define ANIMATION_HEIGTH 50

void set_origin(animation_sprite *as){
    patch sprite = as->animation_sprite;
    as->wanim_origin.x = -sprite.header.x_offset * X_SCALE;
    as->wanim_origin.y = HEIGHT - sprite.header.height * Y_SCALE;
    as->wanim_pos.x = as->wanim_origin.x;
    as->wanim_pos.y = as->wanim_origin.y;
    as->wanim_speed.x = 0;
    as->wanim_speed.y = 0;
}

void init_animation_sprite(animation_sprite *as, patch sprite, bool (*linked_function)(engine *e)){
    as->animation_sprite = sprite;
    as->linked_function = linked_function;
    as->y_difference = HEIGHT - sprite.header.height * Y_SCALE + sprite.header.y_offset * Y_SCALE;
    set_origin(as);
    as->layer_index_len = 0;
    as -> layers_index = NULL;
    if (strcmp(sprite.patchname, "PUNGB0") == 0){
        as->duration = 50;
        return;
    }
    if(strcmp(sprite.patchname, "PUNGC0") == 0){
        as->duration = 25;
        return;
    }
    if(strcmp(sprite.patchname, "PUNGD0") == 0){
        as->duration = 200;
        return;
    }
    if(strcmp(sprite.patchname, "SHTGC0") == 0){
        as->duration = 300;
        return;
    }
    if(strcmp(sprite.patchname, "SHTGD0") == 0){
        as->duration = 300;
        return;
    }
    as->duration = 100;
    //printf("Default init of %s with duration %d\n",sprite.patchname,as->duration);
    return;
}

animations_array *init_animations_array(engine* e,char *abbreviation, json_t *fire_frames, json_t *fire_layers){
    animations_array *ar = malloc(3*sizeof(animations_array)); // A priori pas besoin de plus vu qu'on a que 2 animations 
                                                            //pour les armes : IDLE et animation + la partie qui se rajoute sur l'animation
    // PREMIERE PARTIE : RECUPERE LES INDEX TOUS LES SPRITES RELATIF A L'ARME + LA LONGUEUR DE L'ANIMATION
    int m = strlen(abbreviation);
    int indexes[10]; // J'ai supposé qu'on aurait pas plus de 10 sprites pour l'animation
    for (int i = 0; i < 10; i++) {
        indexes[i] = -1;
    }
    int animation_len = 0;
    int all_sprites_len = 0;
    int i = 0;
    int special_index = 0; // Index spécial pour rajouter des frames dans l'animation
    int special_sprite_index = 0; // Index spécial pour rajouter des frames dans l'animation
    patch *sprites = e->wData->sprites;
    char* to_cmp = malloc(10*sizeof(char));
    strcpy(to_cmp,sprites[i].patchname);
    to_cmp[m]='\0';
    while(!(strcmp(abbreviation,to_cmp)==0) && i < e->wData->len_sprites){
        i++;
        strcpy(to_cmp,sprites[i].patchname);
        to_cmp[m]='\0';
    };
    
    int k = 0;
    while(strcmp(abbreviation,to_cmp)==0 && i< e->wData->len_sprites){
        indexes[k] = i;
        k++;
        if(k == special_index){
            indexes[k] = special_sprite_index;
            animation_len++;
            all_sprites_len++;
            k++;
        }
        i++;
        strcpy(to_cmp,sprites[i].patchname);
        if (!(to_cmp[m] == 'F')){
            animation_len++;
        }
        all_sprites_len++;
        to_cmp[m]='\0';
        
        //Gère les index spéciaux pour rajouter des frames dans l'animation
        if(strcmp("SHTGC0",sprites[i].patchname) == 0){
            special_index = k+2;
            special_sprite_index = i;
        }
    }
    if(i==e->wData->len_sprites){
        printf("Erreur lors du chargement des sprites \n");
        return NULL;
    }

    //DEUXIEME PARTIE : LES CLASSE & PLACE DANS L'ARRAY D'ANIMATIONS

    //Crée la partie IDLE
    animation_sprite *idle_animation_sprites_array = malloc(sizeof(animation_sprite));
    init_animation_sprite(idle_animation_sprites_array,sprites[indexes[0]],NULL);
    animations_array idle_anim = {.animation_sprites_array = idle_animation_sprites_array,.animation_len = 1};
    ar[IDLE] = idle_anim;

    //Crée la partie d'animation de l'animation array
    animation_sprite *fire_anim_sprites = malloc(sizeof(animation_sprite)*(animation_len-1));
    for(int j = 1; j<animation_len;j++){
        init_animation_sprite(&fire_anim_sprites[j-1],sprites[indexes[j]],NULL);
    }


    animations_array fire_anim = {.animation_sprites_array = fire_anim_sprites,.animation_len = animation_len-1};
    ar[ANIMATION] = fire_anim;
    
    //Crée la partie de la couche qui se rajoute sur l'animation
    if(strcmp(abbreviation,"PUN") == 0){
        animations_array fire_layer = {.animation_sprites_array = NULL,.animation_len = 0};
        ar[FIRE] = fire_layer;
    } else {
        //Crée un tableau d'animation_sprite de taille all_sprites_len - animation_len pour les layers
        animation_sprite *layer_sprites = malloc(sizeof(animation_sprite)*(all_sprites_len - animation_len));
        //Initialise les cases du tableau avec les sprites correspondants
        for(int j = animation_len; j<all_sprites_len;j++){
            init_animation_sprite(&layer_sprites[j-animation_len],sprites[indexes[j]],NULL);
        }
        //Ajoute le tableau dans l'animation array
        animations_array fire_layer = {.animation_sprites_array = layer_sprites,.animation_len = all_sprites_len - animation_len};
        ar[FIRE] = fire_layer;

        int n = json_array_size(fire_frames); // Récupère le nombre de frames de l'animation qui ont un layer du fichier json
        for(int i = 0; i<n;i++){
            //Pour chaque frame de l'animation
            
            
            //Tant que la frame d'animation suivante est la même que la frame actuelle dans l'array, c'est a dire que la frame à plusieurs layers
            //On avance en indice pour récupérer tous les layers de la frame
            int j = i;
            while(i+1<n && json_integer_value(json_array_get(fire_frames,i+1)) == json_integer_value(json_array_get(fire_frames,i))){
                i++;
            }
            //J = première occurence de la frame, i = dernière occurence de la frame


            //On récupère les index et la durée de chaque layers de l'animation
            //Alloue le tableau des index des layers de l'animation, avec pour chaque case l'indice du layer a utiliser dans le tableau ar[FIRE].animation_sprites_array
            int* layer_index = malloc((i-j+1)*sizeof(int));
            int layer_index_len = i-j+1;
            int duration = 0;
            for(int k = 0; k<i-j+1;k++){
                //Pour chaque layer de la frame, on récupère l'index du layer dans le tableau ar[FIRE].animation_sprites_array et l'ajoute au tableau layer_index
                layer_index[k] = json_integer_value(json_array_get(fire_layers,j+k));
                //printf("Layer index %d\n",layer_index[k]);
                duration += ar[FIRE].animation_sprites_array[layer_index[k]].duration;
            }

            //On récupère l'index de la frame d'animation en question depuis l'origine (j)
            int index = json_integer_value(json_array_get(fire_frames,j));
            if(index == 0){
                //Si l'indice vaut 0, le layer s'applique à l'idle
                ar[IDLE].animation_sprites_array[0].layers_index = layer_index;
                ar[IDLE].animation_sprites_array[0].layer_index_len = layer_index_len;
                ar[IDLE].animation_sprites_array[0].duration = duration;
            } else {
                //Sinon, il s'applique à la frame d'animation correspondante
                ar[ANIMATION].animation_sprites_array[index-1].layers_index = layer_index;
                ar[ANIMATION].animation_sprites_array[index-1].layer_index_len = layer_index_len;
                ar[ANIMATION].animation_sprites_array[index-1].duration = duration;
            }
        }
    }

    //Initialise les positions des layers de l'animation et les décale comme il faut
    animation_sprite as = ar[IDLE].animation_sprites_array[0];
    for(int k = 0; k<as.layer_index_len;k++){
            //printf("Original pos y %f of the layer %d\n",ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y,as.layers_index[k]);
            ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y = -ar[FIRE].animation_sprites_array[as.layers_index[k]].animation_sprite.header.y_offset * Y_SCALE + as.y_difference;
            //printf("Initialisation pos y %f of the layer %d\n",ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y,as.layers_index[k]);
            ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_pos.y = ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y;
    }
    for(int i = 0; i<ar[ANIMATION].animation_len;i++){
        as = ar[ANIMATION].animation_sprites_array[i];
        //printf("Animation %d : %s\n",i,as.animation_sprite.patchname);
        for(int k = 0; k<as.layer_index_len;k++){
            //printf("Original pos of the animation frame %f %f \n",as.wanim_origin.x,as.wanim_origin.y);
            //printf("Original pos y %d of the layer %d\n",ar[FIRE].animation_sprites_array[as.layers_index[k]].animation_sprite.header.y_offset * Y_SCALE,as.layers_index[k]);
            ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y = -ar[FIRE].animation_sprites_array[as.layers_index[k]].animation_sprite.header.y_offset * Y_SCALE + as.y_difference;
            //printf("Initialisation pos y %f of the layer %d\n",ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y,as.layers_index[k]);
            ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_pos.y = ar[FIRE].animation_sprites_array[as.layers_index[k]].wanim_origin.y;
        }
        ar->animation_duration += ar[ANIMATION].animation_sprites_array[i].duration;
    }
    free(to_cmp);
    return ar;
}

weapon* init_one_weapon(engine* e,int id, char* weapon_name, char* abbreviation, json_t *fire_frames, json_t *fire_layers,
                            int magsize, int max_damage, int min_damage, double fire_rate, double spray, int ammo_bounce, int ammo_id, int type){
    weapon* w = malloc(sizeof(weapon));
    w->id = id;
    w->weapon_name = strdup(weapon_name);
    w->abbreviation = strdup(abbreviation);
    w->sprites = init_animations_array(e,abbreviation,fire_frames,fire_layers);
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
    printf("Animation d'idle de base :%s \n",aa[0].animation_sprites_array->animation_sprite.patchname);
    for(int j = 1; j<3;j++){
        printf("Animation %d \n",j);
        for(int i = 0; i<aa[j].animation_len; i++){
            printf("Case %d \n", i);
            printf("Animation/Layer %d de %s : %s \n", i, w->weapon_name,aa[j].animation_sprites_array[i].animation_sprite.patchname);
            printf("Durée de l'animation : %d\n",aa[j].animation_sprites_array[i].duration);
            printf("Nombre de layers : %d\n",aa[j].animation_sprites_array[i].layer_index_len);
            printf("Layers : ");
            for(int k = 0; k<aa[j].animation_sprites_array[i].layer_index_len;k++){
                printf("%d ",aa[j].animation_sprites_array[i].layers_index[k]);
            }
            printf("\n");
        }
    }
}

weapons_array* init_weapons_array(engine* en){
    weapons_array* wa = malloc(sizeof(weapons_array));

    json_error_t error;
    json_t *root = json_load_file("weapons.json", 0, &error);

    if(!root) {
        printf("Error: on line %d: %s\n", error.line, error.text);
        free(wa);
        return NULL;
    }

    json_t *weapons = json_object_get(root, "weapons");
    if(!json_is_array(weapons)) {
        printf("Error: weapons is not an array\n");
        json_decref(root);
        free(wa);
        return NULL;
    }

    wa->weapons_number = json_array_size(weapons);
    WeaponInventory winv = malloc(wa->weapons_number * sizeof(weapon*));

    size_t i;
    json_t *weapon_data;
    json_array_foreach(weapons, i, weapon_data) {
        int id = json_integer_value(json_object_get(weapon_data, "id"));
        const char *weapon_name = json_string_value(json_object_get(weapon_data, "weapon_name"));
        const char *abbreviation = json_string_value(json_object_get(weapon_data, "abbreviation"));
        int magsize = json_integer_value(json_object_get(weapon_data, "magsize"));
        int max_damage = json_integer_value(json_object_get(weapon_data, "max_damage"));
        int min_damage = json_integer_value(json_object_get(weapon_data, "min_damage"));
        double fire_rate = json_real_value(json_object_get(weapon_data, "fire_rate"));
        double spray = json_real_value(json_object_get(weapon_data, "spray"));
        int ammo_bounce = json_integer_value(json_object_get(weapon_data, "ammo_bounce"));
        int ammo_id = json_integer_value(json_object_get(weapon_data, "ammo_id"));
        int type = json_integer_value(json_object_get(weapon_data, "type"));
        json_t *fire_frames = json_object_get(weapon_data, "fire_frames");
        json_t *fire_layers = json_object_get(weapon_data, "fire_layers");

        //printf("id: %d, weapon_name: %s, abbreviation: %s, magsize: %d, max_damage: %d, min_damage: %d, fire_rate: %lf, spray: %lf, ammo_bounce: %d, ammo_id: %d, type: %d\n",id,weapon_name,abbreviation,magsize,max_damage,min_damage,fire_rate,spray,ammo_bounce,ammo_id,type);
        weapon *new_weapon = init_one_weapon(en, id, (char *)weapon_name, (char *) abbreviation, fire_frames, fire_layers, 
                    magsize, max_damage, min_damage, fire_rate, spray, ammo_bounce, ammo_id, type);
        winv[i] = new_weapon;
    }

    wa->weapons = winv;
    json_decref(root);

    return wa;
}

//Affiche le sprite a la bonne taille a l'endroit donné
void draw_weapon(engine* e, patch sprite, int x, int y) {
    SDL_Rect dest_rect;
    dest_rect.x = x;
    dest_rect.y = y;
    //printf("x: %d, y: %d\n", dest_rect.x, dest_rect.y);
    dest_rect.w = sprite.header.width * X_SCALE;
    dest_rect.h = sprite.header.height * Y_SCALE;

    SDL_RenderCopy(e->renderer, sprite.tex, NULL, &dest_rect);
}

void idle_weapon_animation(engine *e,weapon *w, bool is_moving){
    animation_sprite *idle_anim_sprite = &w->sprites->animation_sprites_array[0];
    
    if(is_moving){
        if(idle_anim_sprite->wanim_speed.x == 0){
            idle_anim_sprite->wanim_speed.x = -2;
        }
        if(idle_anim_sprite->wanim_speed.y == 0){
            idle_anim_sprite->wanim_speed.y = -2;
        }
    } else {
        idle_anim_sprite->wanim_speed.x = 0;
        idle_anim_sprite->wanim_speed.y = 0;
        idle_anim_sprite->wanim_pos.x = idle_anim_sprite->wanim_origin.x;
        idle_anim_sprite->wanim_pos.y = idle_anim_sprite->wanim_origin.y;
    }

    int origin_x = idle_anim_sprite->wanim_origin.x;
    int origin_y = idle_anim_sprite->wanim_origin.y;
    int init_x = idle_anim_sprite->wanim_pos.x;
    int init_y = idle_anim_sprite->wanim_pos.y;
    int dx = idle_anim_sprite->wanim_speed.x;
    int dy = idle_anim_sprite->wanim_speed.y;

    if(init_x + dx > origin_x + ANIMATION_WIDTH/2 || init_x + dx < origin_x - ANIMATION_WIDTH/2){
        dx = -dx;
        idle_anim_sprite->wanim_speed.x = dx;
    }

    if(init_y + dy > origin_y + ANIMATION_HEIGTH/2 || init_y + dy < origin_y - ANIMATION_HEIGTH/2){
        dy = -dy;
        idle_anim_sprite->wanim_speed.y = dy;
    }
    idle_anim_sprite->wanim_pos.x += dx;
    idle_anim_sprite->wanim_pos.y += dy;

    draw_weapon(e,idle_anim_sprite->animation_sprite,idle_anim_sprite->wanim_pos.x,idle_anim_sprite->wanim_pos.y);

}

void add_fire_layer(animations_array *fire_layer,animation_sprite as,int time_elapsed,engine* e){
    if (fire_layer == NULL){
        return;
    }
    //printf("J'allume le feu\n");
    int *layers_index = as.layers_index;
    int layer_index_len = as.layer_index_len;
    int layer_accumulated_duration = 0;
    animation_sprite *fire_layer_anim_sprite; 
    int x;
    int y;
    for (int j = 0; j < layer_index_len; j++) {
        int layer_duration = fire_layer->animation_sprites_array[layers_index[j]].duration;
        if (time_elapsed > layer_accumulated_duration && time_elapsed <= layer_accumulated_duration + layer_duration) {
            fire_layer_anim_sprite = &fire_layer->animation_sprites_array[layers_index[j]];
            x = fire_layer_anim_sprite->wanim_origin.x;
            y = fire_layer_anim_sprite->wanim_origin.y;
            draw_weapon(e, fire_layer->animation_sprites_array[layers_index[j]].animation_sprite, x, y);
        }
        layer_accumulated_duration += layer_duration;
    }
}

void fire_weapon_animation(engine* e,weapon *w){
    player *p = e->p;
    animations_array *aa = w->sprites;
    animation_sprite *fire_anim = aa[ANIMATION].animation_sprites_array; //Tableau de toutes les animations de l'arme
    animation_sprite *idle_anim = aa[IDLE].animation_sprites_array; // Idle animation
    animations_array *fire_layer = NULL; //Layer a ajouter
    if (w->id!=0){ // Si c'est pas les poings
        fire_layer = &aa[FIRE]; // On récupère la couche qui se rajoute sur l'animation
    }
    int x; //Position x du sprite
    int y; //Position y du sprite

    int animation_len = aa[ANIMATION].animation_len; //Nombre d'animations de l'arme
    int time_elapsed = time_elapsed_in_game - p->t_last_shot; //Temps écoulé depuis le début de l'animation
    int total_duration = 0; //Durée totale de l'animation
    //Calcul de la durée totale de l'animation
    total_duration = aa->animation_duration + idle_anim[0].duration;

    //printf("time_elapsed: %d\n", time_elapsed);
    //printf("total_duration: %d\n", total_duration);
    
    if(time_elapsed < idle_anim[0].duration){
        x = idle_anim[0].wanim_origin.x;
        y = idle_anim[0].wanim_origin.y;
        draw_weapon(e,idle_anim[0].animation_sprite,x,y);
        add_fire_layer(fire_layer,idle_anim[0],time_elapsed,e);
    } else {
        int fire_time_elapsed = time_elapsed - idle_anim[0].duration; // Subtract the duration of the idle animation
        if(fire_time_elapsed < fire_anim[0].duration){
            x = fire_anim[0].wanim_origin.x;
            y = fire_anim[0].wanim_origin.y;
            draw_weapon(e,fire_anim[0].animation_sprite,x,y);
            add_fire_layer(fire_layer,fire_anim[0],fire_time_elapsed,e);
            fire_bullet(e->players, NUM_PLAYERS, p, wa);
        } else {
            int i = 1;
            int accumulated_duration = fire_anim[0].duration;
            while (fire_time_elapsed > accumulated_duration && i < animation_len && fire_time_elapsed <= total_duration) {
                accumulated_duration += fire_anim[i].duration;
                i++;
            }
            //printf("Sprite affiché : %s\n",fire_anim[i-1].animation_sprite.patchname);
            x = fire_anim[i - 1].wanim_origin.x;
            y = fire_anim[i - 1].wanim_origin.y;
            draw_weapon(e,fire_anim[i-1].animation_sprite,x,y);
            add_fire_layer(fire_layer,fire_anim[i-1],fire_time_elapsed,e);
            fire_bullet(e->players, NUM_PLAYERS, p, wa);
        }
    }
}

void update_animation(engine* e){
    player *p = e->p;
    weapon_ct* weapon_ecs = player_get_weapon(p);
    weapon *w = wa->weapons[weapon_ecs->active_weapon];
    player_keybind *kb = p->keybinds;
    bool forward = keys[get_key_from_action(kb, "MOVE_FORWARD")];
    bool left = keys[get_key_from_action(kb, "MOVE_LEFT")];
    bool backward = keys[get_key_from_action(kb, "MOVE_BACKWARD")];
    bool right_d = keys[get_key_from_action(kb, "MOVE_RIGHT")];
    bool attack = keys[get_key_from_action(kb, "ATTACK")];

    bool move = forward || left || backward || right_d;

    if(attack){
        if(p->has_attacked == false){
            p->t_last_shot = time_elapsed_in_game;
            //printf("\n");
            //printf("Reset t_last_shot\n");
            //printf("\n");
        }
        p->has_attacked = true;
    }
    uint64_t casted_cooldown = (uint64_t) w->sprites->animation_duration + w->sprites[IDLE].animation_sprites_array[0].duration;
    
    if(p->has_attacked){
        if(time_elapsed_in_game - p->t_last_shot <= casted_cooldown){
            fire_weapon_animation(e,w);
        }
        if(time_elapsed_in_game - p->t_last_shot > casted_cooldown){
            p->has_attacked = false;
        }
    } else {
        idle_weapon_animation(e,w,move);
    }  
}

void update_weapons(engine* e){
    player *p = e->p;
    //weapon *w = wa->weapons[weapon->active_weapon];
    weapon_ct* weapon = player_get_weapon(p);
    player_keybind *kb = p->keybinds;
    bool fists = keys[get_key_from_action(kb, "FISTS")], pistol = keys[get_key_from_action(kb, "PISTOL")], chaingun = keys[get_key_from_action(kb, "CHAINGUN")], shotgun = keys[get_key_from_action(kb, "SHOTGUN")];
    //, rocket = keys[get_key_from_action(kb, "ROCKET")], plasma = keys[get_key_from_action(kb, "PLASMA")], bfg = keys[get_key_from_action(kb, "BFG")];
        if (!p->has_attacked){
            if(fists){
                if(weapon->active_weapon != 0){
                    switch_weapon(p,0);
                }
            }
            if(pistol){
                if(weapon->active_weapon != 1 && weapon->ammunitions[1] >= 0){
                    switch_weapon(p,1);
                }
            }
            if(chaingun){
                if(weapon->active_weapon != 2 &&weapon->ammunitions[2] >= 0){
                    switch_weapon(p,2);
                }
            }
            if(shotgun){
                if(weapon->active_weapon != 3 && weapon->ammunitions[3] >= 0){
                    switch_weapon(p,3);
                }
            }
        }
    update_animation(e);

}


void free_animations_array(weapon *w){
    animations_array *aa = w->sprites;
    for(int i = 0; i<3;i++){
        if (aa[i].animation_sprites_array != NULL){
            for(int j = 0; j<aa[i].animation_len;j++){
            if(aa[i].animation_sprites_array[j].layers_index != NULL){
                free(aa[i].animation_sprites_array[j].layers_index);
            }
            }
            free(aa[i].animation_sprites_array);
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



void switch_weapon(player* p, int weapon_id){
    weapon_ct* weapon = player_get_weapon(p);
    weapon->active_weapon = weapon_id;
}

void add_weapon(player* p, int weapon_id,weapons_array* wa){
    /*If the player doesnt have the weapon and the weapon have a limited magsize*/
    int* ammo = player_get_weapon(p)->ammunitions;
    //printf("tout va  bien\n");
    assert(weapon_id < wa->weapons_number);
    assert(wa->weapons[weapon_id] != NULL);
    if (wa->weapons[weapon_id]->magsize > 0){
        //printf("Prends ton chargeur gamin\n");
        ammo[weapon_id] = wa->weapons[weapon_id]->magsize;
    } else {
        //printf("Munitions infinies G_G\n");
        ammo[weapon_id] = -2;
    }
    switch_weapon(p,weapon_id);
}

void add_ammo(player* p, int weapon_id, int ammo){
    int* ammos = player_get_weapon(p)->ammunitions;
    if (ammos[weapon_id] >= 0){
        ammos[weapon_id] += ammo;
    }
}
