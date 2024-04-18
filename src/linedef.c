#include "../include/linedef.h"
#include <stdio.h>

tuple_index *linedefs_tuples = NULL;
door **door_sector = NULL;
int *height_neighboring_sectors = NULL;

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
          line_type == CWO_S || line_type == DRK || line_type == ACCEPTED);
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
    d = door_create(NULL, SLOW, OPEN_WAIT_CLOSE, 4, false, sd->sector);
    break;
  case OWC_F_4:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4, false, sd->sector);
    break;
  case OSO_S:
    d = door_create(NULL, SLOW, OPEN_STAY_OPEN, 0, false, sd->sector);
    break;
  case OSO_F:
    d = door_create(NULL, FAST, OPEN_STAY_OPEN, 0, false, sd->sector);
    break;
  case CSC_S:
    d = door_create(NULL, SLOW, CLOSE_STAY_CLOSED, 0, false, sd->sector);
    break;
  case CSC_F:
    d = door_create(NULL, FAST, CLOSE_STAY_CLOSED, 0, false, sd->sector);
    break;
  case CWO_S:
    d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 0, false, sd->sector);
    break;
  case ACCEPTED:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4, false, sd->sector);
    break;
  default:
    d = door_create(NULL, FAST, OPEN_WAIT_CLOSE, 4, false, sd->sector);
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
  door_sector = malloc(sizeof(door *) * len_sectors);
  height_neighboring_sectors = malloc(sizeof(int) * len_sectors);
  for (int i = 0; i < len_sectors; i++) {
    height_neighboring_sectors[i] = MINIMUM_FLOOR_HEIGHT;
  }
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset, vertexes, sidedefs);
  }
  free(door_sector);
  free(height_neighboring_sectors);
  return linedefs;
}

door **get_doors(linedef **linedefs, int len_linedefs, int *doors_count,
                 int len_sectors) {
  *doors_count = DOORS_COUNT;
  linedefs_tuples = malloc(sizeof(tuple_index) * len_sectors);
  for (int i = 0; i < len_sectors; i++) { // init
    linedefs_tuples[i].index1 = -1;
    linedefs_tuples[i].index2 = -1;
  }

  sidedef *sector_sidedef = NULL;
  sidedef *neighbor_sidedef = NULL;
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
  door **doors = malloc(sizeof(door *) * DOORS_COUNT);
  int door_index = 0;
  sidedef *sector_sidedef1 = NULL;
  sidedef *neighbor_sidedef1 = NULL;
  sidedef *sector_sidedef2 = NULL;
  sidedef *neighbor_sidedef2 = NULL;
  for (int i = 0; i < len_sectors; i++) {
    if (linedefs_tuples[i].index1 != -1 && linedefs_tuples[i].index2 != -1) {
      linedef *line1 = linedefs[linedefs_tuples[i].index1];
      linedef *line2 = linedefs[linedefs_tuples[i].index2];
      set_correct_sidedefs(line1, &sector_sidedef1, &neighbor_sidedef1);
      set_correct_sidedefs(line1, &sector_sidedef2, &neighbor_sidedef2);
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
  *doors_count = door_index;
  free(linedefs_tuples);
  return doors;
}

void linedefs_free(linedef **linedefs, int len_linedefs) {
  for (int i = 0; i < len_linedefs; i++) {
    free(linedefs[i]);
  }
  free(linedefs);
}