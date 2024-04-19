#include "../include/linedef.h"
#include <stdbool.h>
#include <stdio.h>

tuple_index *linedefs_tuples = NULL;

vertex *get_vertex_from_linedef(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

sidedef *get_sidedef_from_linedef(i16 sidedef_id, sidedef *sidedefs) {
  return &sidedefs[sidedef_id];
}

bool is_a_door(linedef *line) {
  i16 line_type = line->line_type;
  return line->has_back_sidedef &&
         (line_type == OWC_S_4 || line_type == OWC_F_4 || line_type == OSO_S ||
          line_type == OSO_F || line_type == CSC_S || line_type == CSC_F ||
          line_type == CWO_S || line_type == DRK || line_type == REMOTE_OPEN_STAY_OPEN || line_type == ACCEPTED);
}

void set_correct_sidedefs(linedef *line, sidedef **sector_sidedef,
                          sidedef **neighbor_sidedef) {
  if (line->front_sidedef->sector->ceiling_height ==
      line->front_sidedef->sector
          ->floor_height) { // which sidedef is actually the door ? depends if
                            // linedef is correctly oriented
    *sector_sidedef = line->front_sidedef;
    *neighbor_sidedef = line->back_sidedef;
  } else if (line->back_sidedef->sector->ceiling_height ==
             line->back_sidedef->sector->floor_height) {
    *sector_sidedef = line->back_sidedef;
    *neighbor_sidedef = line->front_sidedef;
  }
}

door *create_door_from_linedef(sidedef *sd, enum LinedefDoorTypes line_type) {
  door *d;
  switch (line_type) {
  case OWC_S_4:
    d = door_create(NULL, SLOW, OPEN_WAIT_CLOSE, 4000, true,true, sd->sector);
    break;
  case OWC_F_4:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4000, true,true, sd->sector);
    break;
  case OSO_S:
    d = door_create(NULL, SLOW, OPEN_STAY_OPEN, 0, true,true, sd->sector);
    break;
  case OSO_F:
    d = door_create(NULL, FAST, OPEN_STAY_OPEN, 0, true,true, sd->sector);
    break;
  case CSC_S:
    d = door_create(NULL, SLOW, CLOSE_STAY_CLOSED, 0, true,true, sd->sector);
    break;
  case CSC_F:
    d = door_create(NULL, FAST, CLOSE_STAY_CLOSED, 0, true,true, sd->sector);
    break;
  case CWO_S:
    d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 0, true,true, sd->sector);
    break;
  case ACCEPTED:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4000, true,true, sd->sector);
    break;
  case REMOTE_OPEN_STAY_OPEN:
    d = door_create(NULL, FAST, OPEN_STAY_OPEN, 0, false,false, sd->sector);
    break;
  case DRK:
    d = door_create(NULL, FAST, OPEN_STAY_OPEN, 0, true,true, sd->sector);
    break;
  default:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4000, true,true, sd->sector);
    break;
  }
  return d;
}

linedef *read_linedef(FILE *f, int offset, vertex *vertexes,
                      sidedef *sidedefs) {
  linedef *line = malloc(sizeof(linedef));
  line->start_vertex = get_vertex_from_linedef(read_i16(f, offset), vertexes);
  line->end_vertex = get_vertex_from_linedef(read_i16(f, offset + 2), vertexes);
  line->flag = read_i16(f, offset + 4);
  line->line_type = read_i16(f, offset + 6);
  line->sector_tag = read_i16(f, offset + 8);
  i16 front_sidedef_id = read_i16(f, offset + 10);
  i16 back_sidedef_id = read_i16(f, offset + 12);
  line->has_back_sidedef = back_sidedef_id != -1;
  line->front_sidedef = get_sidedef_from_linedef(front_sidedef_id, sidedefs);
  line->door = NULL;
  line->back_sidedef = line->has_back_sidedef
                           ? get_sidedef_from_linedef(back_sidedef_id, sidedefs)
                           : NULL;
  line->door = NULL;
  return line;
}

linedef **get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                 int num_bytes, int header_length,
                                 int len_linedefs, vertex *vertexes,
                                 sidedef *sidedefs, int len_sectors) {
  lump lump_info = directory[lump_index];
  linedef **linedefs = malloc(sizeof(linedef *) * len_linedefs);
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset, vertexes, sidedefs);
  }
  return linedefs;
}

linedef* get_linedef_by_sector_tag(linedef **linedefs, int len_linedefs, u16 sector_tag) {
  for (int i = 0; i < len_linedefs; i++) {
    if (linedefs[i]->sector_tag == sector_tag) {
      return linedefs[i];
    }
  }
  return NULL;
}

door **get_doors(linedef **linedefs, int len_linedefs, int *doors_count,
                 sector* sectors,int len_sectors) {
  *doors_count = DOORS_COUNT;
  linedefs_tuples = malloc(sizeof(tuple_index) * len_sectors); // for each sector, have an entry (index1, index2) that are the index of collidable linedefs to open a door
  for (int i = 0; i < len_sectors; i++) { // init
    linedefs_tuples[i].index1 = -1;
    linedefs_tuples[i].index2 = -1;
  }

  sidedef *sector_sidedef = NULL;
  sidedef *neighbor_sidedef = NULL;
  door **doors = malloc(sizeof(door *) * DOORS_COUNT);
  int door_index = 0;
  for (int i = 0; i < len_linedefs; i++) {
    if (is_a_door(linedefs[i])) {
      set_correct_sidedefs(linedefs[i], &sector_sidedef, &neighbor_sidedef);
      if (linedefs_tuples[sector_sidedef->sector_id].index1 == -1) {
        linedefs_tuples[sector_sidedef->sector_id].index1 = i;
      } else {
        linedefs_tuples[sector_sidedef->sector_id].index2 = i;
      }
    }
  }

  sidedef *sector_sidedef1 = NULL;
  sidedef *neighbor_sidedef1 = NULL;
  sidedef *sector_sidedef2 = NULL;
  sidedef *neighbor_sidedef2 = NULL;
  linedef* line1 = NULL;
  linedef* line2 = NULL;
  for (int i = 0; i < len_sectors; i++) {
    line1 = NULL;
    line2 = NULL;
    if (sectors[i].tag_number != 0 && sectors[i].ceiling_height == sectors[i].floor_height) { //sector, which is a door,is subject to an action from another linedef, lets find which one
      printf("sector %d is a door\n", i);
      linedef* trigger_linedef = get_linedef_by_sector_tag(linedefs, len_linedefs, sectors[i].tag_number);
      // we then need to find the properties of the door, which is hidden in one of the linedefs that are part of the sector...
      for (int j = 0; j < len_linedefs;j++){
        if (!linedefs[j]->has_back_sidedef) continue; // does not have a backsidedef, useless
        if (linedefs[j]->front_sidedef->sector_id == i || linedefs[j]->back_sidedef->sector_id == i) {
          if (line1 == NULL) {
            line1 = linedefs[j];
          } else {
            line2 = linedefs[j];
            break;
          }
        }
      }
      printf("Line 1 is null ? %d\n", line1 == NULL);
      printf("Line 2 is null ? %d\n", line2 == NULL);
      // we found the two linedefs that are each side of the door, we can now create the door
      set_correct_sidedefs(line1, &sector_sidedef1, &neighbor_sidedef1);
      set_correct_sidedefs(line2, &sector_sidedef2, &neighbor_sidedef2);
      door *door = create_door_from_linedef(sector_sidedef1, trigger_linedef->line_type);
      trigger_linedef->door = door; // this is linedef that will be able to trigger the door
      int delta_height = min(neighbor_sidedef1->sector->ceiling_height,
                             neighbor_sidedef2->sector->ceiling_height) -
                         sector_sidedef1->sector->ceiling_height;
      door_update_height(door, delta_height);
      doors[door_index] = door;
      door_index++;
    } else if (linedefs_tuples[i].index1 != -1 && linedefs_tuples[i].index2 != -1) {
      linedef *line1 = linedefs[linedefs_tuples[i].index1];
      linedef *line2 = linedefs[linedefs_tuples[i].index2];
      set_correct_sidedefs(line1, &sector_sidedef1, &neighbor_sidedef1);
      set_correct_sidedefs(line2, &sector_sidedef2, &neighbor_sidedef2);
      door *door = create_door_from_linedef(sector_sidedef1, line1->line_type);
      line1->door = door;
      line2->door = door;
      int delta_height = min(neighbor_sidedef1->sector->ceiling_height,
                             neighbor_sidedef2->sector->ceiling_height) -
                         sector_sidedef1->sector->ceiling_height;
      door_update_height(door, delta_height);
      doors[door_index] = door;
      door_index++;
    } else if (linedefs_tuples[i].index1 != -1) {
      linedef *line = linedefs[linedefs_tuples[i].index1];

      set_correct_sidedefs(line, &sector_sidedef1, &neighbor_sidedef1);
      door *door = create_door_from_linedef(sector_sidedef1, line->line_type);
      line->door = door;
      doors[door_index] = door;
      door_index++;
      door_update_height(door, neighbor_sidedef1->sector->ceiling_height -
                                   sector_sidedef1->sector->ceiling_height);
    }
  }
  free(linedefs_tuples);
  return doors;
}

void linedefs_free(linedef **linedefs, int len_linedefs) {
  for (int i = 0; i < len_linedefs; i++) {
    free(linedefs[i]);
  }
  free(linedefs);
}