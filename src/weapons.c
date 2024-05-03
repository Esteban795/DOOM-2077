#include "../include/weapons.h"
#include <assert.h>

animations_array *init_animations_array(map_renderer *mr,char *abbreviation){
    animations_array *ar = malloc(2*sizeof(animations_array)); // A priori pas besoin de plus vu qu'on a que 2 animations 
                                                            //pour les armes : IDLE et Firing
    int m = strlen(abbreviation);
    int indexes[10]; // J'ai supposé qu'on aurait pas plus de 10 sprites pour l'animation
    for (int i = 0; i < 10; i++) {
        indexes[i] = -1;
    }
    int animation_len = 0;
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
        printf("Animations pour l'arme : %s \n", sprites[i].patchname);
        indexes[k] = i;
        k++;
        i++;
        strcpy(to_cmp,sprites[i].patchname);
        to_cmp[m]='\0';
        animation_len ++;
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
    ar[FIRE] = fire_anim;
    

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
    for(int i = 0; i<aa[1].animation_len; i++){
        printf("Animation %d de tir de %s : %s \n", i, w->weapon_name,aa[1].animation_sprites[i].patchname);
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

void free_animations_array(weapon *w){
    animations_array *aa = w->sprites;
    free(aa[0].animation_sprites);
    free(aa[1].animation_sprites);
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
    p->active_weapon = weapon_id;
}

void add_weapon(player* p, int weapon_id,weapons_array* wa){
    /*If the player doesnt have the weapon and the weapon have a limited magsize*/
    if (p->ammo[weapon_id] < 0){
        printf("tout va  bien\n");
        assert(weapon_id < wa->weapons_number);
        assert(wa->weapons[weapon_id] != NULL);
        if (wa->weapons[weapon_id]->magsize > 0){
            printf("Prends ton chargeur gamin");
            p->ammo[weapon_id] = wa->weapons[weapon_id]->magsize;
        } else {
            printf("Munitions infinies G_G");
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
