#include "../include/collision_map.h"
#include <stdio.h>
#include <stdlib.h>
#define map_size  8000 //j'en sais rien frere ????
#define collision_map_precision 10 //j'en sais toujours rien 
#define number_of_blocs map_size/collision_map_precision
#define number_of_linedefs 100 //qu'est ce que j'en sais putain


void display_linedefs_coords(linedef* linedefs){
    int i=0;
    for(i=0;i<10;i++){
        printf("%i,%i,%i",i,linedefs[i].start_vertex->x,linedefs[i].end_vertex->x);
    } 
}

//void display_collision_map(int** collision_map){
//    int i;
//    int j;
//    for(i=0;i<number_of_blocs;i++){
//        for(j=0;j<number_of_blocs;j++){
//            printf("x=%i,y=%i,is_full=%i",i*collision_map_precision,j*collision_map_precision,collision_map[i][j]);
//        }
//    }
//}

int** create_collision_map(void){
    int i=0;
    int j=0;
    int** collision_map=malloc(sizeof(int)*number_of_blocs*number_of_blocs); //flemme d'import math
    for(i=0;i<number_of_blocs;i++){
        collision_map[i]=malloc(sizeof(int)*number_of_blocs);
        for(j=0;j<number_of_blocs;j++){
            collision_map[i][j]=0;
        }
    }
    return(collision_map);
}


void free_collision_map(int** collision_map) {
    int i=0;
    for(i=0;i<number_of_blocs;i++){
        free(collision_map[i]);
    }
  free(collision_map);
}


int from_coords_to_collision_map(int coord){
    return(coord/collision_map_precision)+number_of_blocs/2;
}


void construct_collision_map(int** collision_map,linedef* linedefs){
    int i=0;
    for(i=0;i<number_of_linedefs;i++){
        int x1=linedefs[i].start_vertex->x; //segfault si 0 remplacé par quoi que ca soit d'autre 
        int y1=linedefs[i].start_vertex->y;
        int x2=linedefs[i].end_vertex->x;
        int y2=linedefs[i].end_vertex->y;
        double dist =sqrt(pow((x1-x2),2)+pow((y1-y2),2));
        double number_of_points =dist/collision_map_precision;
        double stepx = (x2-x1)/number_of_points;
        double stepy = (y2-y1)/number_of_points;
        int j=0;
        for(j=0;j<number_of_points;j++){
            int x=from_coords_to_collision_map(x1);
            int y=from_coords_to_collision_map(y1);
            printf("%i,%i\n",x,y);
            collision_map[x][y]=1;
            x1=x1+stepx;
            y1=y1+stepy;
        }
    }
    printf("%i\n",collision_map[0][0]);
}


