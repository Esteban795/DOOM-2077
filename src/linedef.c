#include "../include/linedef.h"
#include <stdio.h>

door **door_sector = NULL;
int *height_neighboring_sectors = NULL;

vertex *get_vertex_from_linedef(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

sidedef *get_sidedef_from_linedef(i16 sidedef_id, sidedef *sidedefs) {
  return &sidedefs[sidedef_id];
}

bool is_a_door(u16 line_type) {
  return line_type == OWC_S_4 || line_type == OWC_F_4 || line_type == OSO_S ||
         line_type == OSO_F || line_type == CSC_S || line_type == CSC_F ||
         line_type == CWO_S;
}

door *create_door_from_linedef(sidedef *sd, enum LinedefDoorTypes line_type) {
  door* d;
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
  default:
    d = NULL;
    break;
  }
  return d;
}

linedef read_linedef(FILE *f, int offset, vertex *vertexes, sidedef *sidedefs) {
  linedef line;
  line.start_vertex = get_vertex_from_linedef(read_i16(f, offset), vertexes);
  line.end_vertex = get_vertex_from_linedef(read_i16(f, offset + 2), vertexes);
  line.flag = read_i16(f, offset + 4);
  line.line_type = read_i16(f, offset + 6);
  line.sector_tag = read_i16(f, offset + 8);
  i16 front_sidedef_id = read_i16(f, offset + 10);
  i16 back_sidedef_id = read_i16(f, offset + 12);
  line.has_back_sidedef = back_sidedef_id != -1;
  line.front_sidedef = get_sidedef_from_linedef(front_sidedef_id, sidedefs);
  line.door = NULL;
  line.back_sidedef = line.has_back_sidedef ? get_sidedef_from_linedef(back_sidedef_id, sidedefs) : NULL;

  if (!line.has_back_sidedef || !is_a_door(line.line_type)) return line; // make sure it is both two sided and a door
  sidedef* sd = NULL;
  if (line.front_sidedef->sector->ceiling_height == line.front_sidedef->sector->floor_height) { // which sidedef is actually the door ? depends if linedef is correctly oriented
    sd = line.front_sidedef;
  } else if (line.back_sidedef->sector->ceiling_height == line.back_sidedef->sector->floor_height) {
    sd = line.back_sidedef;
  }
  if (height_neighboring_sectors[sd->sector_id] == MINIMUM_FLOOR_HEIGHT) { //first linedef of the two that are important for doors
    height_neighboring_sectors[sd->sector_id] = sd->sector->ceiling_height;
    door_sector[sd->sector_id] = create_door_from_linedef(sd, line.line_type);
  } else {
    line.door = door_sector[sd->sector_id];
    
    int delta_height = min(sd->sector->ceiling_height, height_neighboring_sectors[sd->sector_id]) - sd->sector->ceiling_height;
    door_update_height(line.door, delta_height);
    door_print(line.door);
  }
  return line;
}

linedef *get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                int num_bytes, int header_length,
                                int len_linedefs, vertex *vertexes,
                                sidedef *sidedefs, int len_sectors) {
  lump lump_info = directory[lump_index];
  linedef *linedefs = malloc(sizeof(linedef) * len_linedefs);
  door_sector = malloc(sizeof(door*) * len_sectors);
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