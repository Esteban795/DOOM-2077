#include "../include/linedef.h"
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>

vertex* get_vertex_from_linedef(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

sidedef* get_sidedef_from_linedef(i16 sidedef_id, sidedef *sidedefs) {
  return &sidedefs[sidedef_id];
}

linedef* read_linedef(FILE *f, int offset, vertex *vertexes,sidedef *sidedefs){
  linedef* line = malloc(sizeof(linedef));
  line->start_vertex = get_vertex_from_linedef(read_i16(f, offset), vertexes);
  line->end_vertex = get_vertex_from_linedef(read_i16(f, offset + 2), vertexes);
  line->flag = read_i16(f, offset + 4);
  line->line_type = read_i16(f, offset + 6);
  line->sector_tag = read_i16(f, offset + 8);
  i16 front_sidedef_id = read_i16(f, offset + 10);
  i16 back_sidedef_id = read_i16(f, offset + 12);
  line->has_back_sidedef = back_sidedef_id != -1;
  line->front_sidedef = get_sidedef_from_linedef(front_sidedef_id, sidedefs);
  if (line->has_back_sidedef) line->back_sidedef = get_sidedef_from_linedef(back_sidedef_id, sidedefs);
  line->is_collidable = line->is_shootable = line->is_pushable = line->is_repeatable = line->has_lifts = false;
  line->lifts = NULL;
  return line;
}

linedef **get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes,sidedef *sidedefs) {
  lump lump_info = directory[lump_index];
  linedef **linedefs = malloc(sizeof(linedef*) * len_linedefs);
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset, vertexes,sidedefs);
  }
  return linedefs;
}


// LIFTS

// Gets the lowest height of adjacent sectors, sector_id included
int get_lnf(linedef** lines,int len_linedefs,i16 sector_id) {
  int min_height = INT_MAX;
  for (int i = 0; i < len_linedefs;i++) {
    linedef* line = lines[i];
    if (line->front_sidedef->sector_id == sector_id) min_height = min(min_height,line->back_sidedef->sector->floor_height);
    if (line->back_sidedef->sector_id == sector_id) min_height = min(min_height,line->front_sidedef->sector->floor_height);
  }
  return min_height;
}

// Gets the lowest height, that is > sector_height, of adjacent sectors
int get_rnf(linedef** lines,int len_linedefs,i16 sector_id,int sector_height) {
  int height = INT_MAX;
  for (int i = 0; i < len_linedefs;i++) {
    linedef* line = lines[i];
    sector* front_sector = line->front_sidedef->sector;
    sector* back_sector = line->back_sidedef->sector;
    if (line->front_sidedef->sector_id == sector_id && front_sector->floor_height > sector_height) height = min(height,back_sector->floor_height);
    if (line->back_sidedef->sector_id == sector_id && back_sector->floor_height > sector_height) height = min(height,front_sector->floor_height);
  }
  return height;
}

// gets the highest and lowest height of adjacent sectors, sector_id included
void get_lfhp(linedef** lines,int len_linedefs,i16 sector_id,int* low_height,int* high_height) {
  *low_height = INT_MAX;
  *high_height = INT_MIN;
  for (int i = 0; i < len_linedefs;i++) {
    linedef* line = lines[i];
    sector* front_sector = line->front_sidedef->sector;
    sector* back_sector = line->back_sidedef->sector;
    if (line->front_sidedef->sector_id == sector_id) {
      *low_height = min(*low_height,back_sector->floor_height);
      *high_height = max(*high_height,back_sector->floor_height);
    }
    if (line->back_sidedef->sector_id == sector_id) {
      *low_height = min(*low_height,front_sector->floor_height);
      *high_height = max(*high_height,front_sector->floor_height);
    }
  }
}

lift* create_lift_from_linedef(linedef** linedefs,int len_linedefs,linedef* line, sector* sector, i16 sector_id,enum LiftType type) {
  lift* l;
  enum LiftTransitionSpeed speed;
  int low_height;
  int high_height;
  switch (type) {
    case WR_3_S_LNF:
    case W1_3_S_LNF:
    case WR_3_F_LNF:
    case W1_3_F_LNF:
      line->is_repeatable =  type == WR_3_S_LNF;
      line->is_collidable = true;
      line->is_pushable = line->is_shootable = false;
      low_height = get_lnf(linedefs,len_linedefs,sector_id);
      speed = type == WR_3_S_LNF || type == W1_3_S_LNF ? L_SLOW : L_FAST;
      l = lift_create(NULL, sector, speed, low_height, sector->floor_height, 3, sector->floor_height);
      break;
    case SR_0_S_24U:
    case S1_0_S_24U:
      line->is_pushable = true;
      line->is_collidable = line->is_shootable = false;
      line->is_repeatable =  type == SR_0_S_24U;
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, sector->floor_height + 24, 0, sector->floor_height);
      break;
    case WR_0_S_24U:
    case W1_0_S_24U:
      line->is_collidable = true;
      line->is_shootable = line->is_pushable = false;
      line->is_repeatable = type == WR_0_S_24U;
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, sector->floor_height + 24, 0, sector->floor_height);
      break;
    case SR_0_S_32U:
    case S1_0_S_32U:
      line->is_pushable = true;
      line->is_collidable = line->is_shootable = false;
      line->is_repeatable =  type == SR_0_S_24U;
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, sector->floor_height + 32, 0, sector->floor_height);
      break;
    case WR_0_S_32U:
    case W1_0_S_32U:
      line->is_collidable = true;
      line->is_shootable = line->is_pushable = false;
      line->is_repeatable = type == WR_0_S_24U;
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, sector->floor_height + 32, 0, sector->floor_height);
      break;
    case SR_0_S_RNF:
    case S1_0_S_RNF:
      line->is_pushable = true;
      line->is_collidable = line->is_shootable = false;
      line->is_repeatable =  type == SR_0_S_RNF;
      high_height = get_rnf(linedefs,len_linedefs,sector_id,sector->floor_height);
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, high_height, 0, sector->floor_height);
      break;
    case WR_0_S_RNF:
    case W1_0_S_RNF:
      line->is_collidable = true;
      line->is_shootable = line->is_pushable = false;
      line->is_repeatable = type == WR_0_S_RNF;
      high_height = get_rnf(linedefs,len_linedefs,sector_id,sector->floor_height);
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, high_height, 0, sector->floor_height);
      break;
    case G1_0_S_RNF:
      line->is_shootable = true;
      line->is_collidable = line->is_pushable = false;
      line->is_repeatable = false;
      high_height = get_rnf(linedefs,len_linedefs,sector_id,sector->floor_height);
      l = lift_create(NULL, sector, L_SLOW, sector->floor_height, high_height, 0, sector->floor_height);
      break;
    case SR_3_S_LFHP:
    case S1_3_S_LFHP:
      line->is_pushable = true;
      line->is_collidable = line->is_shootable = false;
      line->is_repeatable = false; // runs indefinitely anyway..
      get_lfhp(linedefs,len_linedefs,sector_id,&low_height,&high_height);
      l = lift_create(NULL, sector, L_SLOW, low_height, high_height, 3, sector->floor_height);
      break;
    case WR_3_S_LFHP:
    case W1_3_S_LFHP:
      line->is_collidable = true;
      line->is_shootable = line->is_pushable = false;
      line->is_repeatable = false; // runs indefinitely anyway..
      get_lfhp(linedefs,len_linedefs,sector_id,&low_height,&high_height);
      l = lift_create(NULL, sector, L_SLOW, low_height, high_height, 3, sector->floor_height);
      break;
    case SR_3_S_LNF:
    case S1_3_S_LNF:
    case SR_3_F_LNF:
    case S1_3_F_LNF:
      line->is_collidable = line->is_shootable = false;
      line->is_pushable = true;
      line->is_repeatable = (type == SR_3_S_LNF || type == SR_3_F_LNF);
      speed = type == SR_3_S_LNF || type == S1_3_S_LNF ? L_SLOW : L_FAST;
      low_height = get_lnf(linedefs,len_linedefs,sector_id);
      l = lift_create(NULL, sector, speed, low_height, sector->floor_height, 3, sector->floor_height);
      break;
    default :
      printf("Unknown lift type\n");
      exit(1);
  }
  line->has_lifts = true;
  return l;
}

int get_lifts_count(linedef** linedefs,int len_linedefs) {
  int count = 0;
  for (int i = 0; i < len_linedefs;i++) {
    if (is_a_lift(linedefs[i]->line_type)) count++;
  }
  return count;
}

lift** get_lifts(linedef** linedefs,int len_linedefs, int* lifts_count,sector* sectors, int len_sectors){
  *lifts_count = get_lifts_count(linedefs,len_linedefs);
  lift** lifts_sectors = malloc(sizeof(lift*) * len_sectors);
  for (int i = 0; i < len_sectors;i++) {
    lifts_sectors[i] = NULL;
  }

  lift** lifts = malloc(sizeof(lift*) * *lifts_count);
  int lift_index = 0;

  for (int i = 0; i < len_linedefs; i++) {
    linedef* line = linedefs[i];
    if (is_a_lift(line->line_type)) {
      u16 sector_tag = line->sector_tag;
      for (int sector_id = 0; sector_id < len_sectors; sector_id++){
        if (sectors[sector_id].tag_number != sector_tag) continue;
        lift* l = create_lift_from_linedef(linedefs,len_linedefs,line,&sectors[sector_id],sector_id,line->line_type);
        if (lifts_sectors[sector_id] == NULL) {
          line->lifts = line->has_lifts ? lift_add(line->lifts,l) : l;
          lifts_sectors[sector_id] = l;
          lifts[lift_index] = l;
          lift_index++;
        } else {
          line->lifts = lift_add(line->lifts, lifts_sectors[sector_id]);
          free(l);
          continue;
        }
      }
    }
  }
  free(lifts_sectors);
  return lifts;
}