#ifndef COLLISION_MAP_H
#define COLLISION_MAP_H
#include "linedef.h"
#include <stdio.h>
#include <math.h>

void display_linedefs_coords(linedef* linedefs);

//typedef struct _collision_map {
//  int x;
//  int y;
//  int is_full;
//}collision_map;



int** create_collision_map(void);

//void display_collision_map(int** collision_map);

void free_collision_map(int** collision_map);

void construct_collision_map(int** collision_map,linedef* linedefs);


typedef struct _linepoint{
    double x;
    double y;
}linepoint;

#endif



