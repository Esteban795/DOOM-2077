#include "../include/linedef.h"
#include <SDL2/SDL_log.h>
#include <limits.h>
#include <stdio.h>

// LINEDEF PART
vertex *get_vertex_from_linedef(i16 vertex_id, vertex *vertexes) {
  return &vertexes[vertex_id];
}

sidedef *get_sidedef_from_linedef(i16 sidedef_id, sidedef *sidedefs) {
  return &sidedefs[sidedef_id];
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
  line->back_sidedef = line->has_back_sidedef
                           ? get_sidedef_from_linedef(back_sidedef_id, sidedefs)
                           : NULL;
  line->is_colllidable = false;
  line->is_repeatable = false;
  line->is_shootable = false;
  line->has_doors = false;
  line->door = NULL;
  line->used = false;
  line->is_pushable = false;
  return line;
}

linedef **get_linedefs_from_lump(FILE *f, lump *directory, int lump_index,
                                 int num_bytes, int header_length,
                                 int len_linedefs, vertex *vertexes,
                                 sidedef *sidedefs) {
  lump lump_info = directory[lump_index];
  linedef **linedefs = malloc(sizeof(linedef *) * len_linedefs);
  for (int i = 0; i < len_linedefs; i++) {
    int offset = lump_info.lump_offset + i * num_bytes + header_length;
    linedefs[i] = read_linedef(f, offset, vertexes, sidedefs);
  }
  return linedefs;
}

void linedefs_free(linedef **linedefs, int len_linedefs) {
  for (int i = 0; i < len_linedefs; i++) {
    free(linedefs[i]);
  }
  free(linedefs);
}

// DOORS 

bool is_a_door(linedef *line) {
  i16 l = line->line_type;
  return l == CSC_SR_S || l == CSC_SR_F || l == CSC_S1_S || l == CSC_S1_F ||
         l == CSC_WR_S || l == CSC_WR_F || l == CSC_W1_S || l == CSC_W1_F ||
         l == CWO_SR_S || l == CWO_S1_S || l == CWO_WR_S || l == CWO_W1_S ||
         l == OSO_P1_S || l == OSO_P1_F || l == OSO_WR_S || l == OSO_WR_F ||
         l == OSO_W1_S || l == OSO_W1_F || l == OSO_SR_S || l == OSO_SR_F ||
         l == OSO_S1_S || l == OSO_S1_F || l == OSO_GR_S || l == OWC_PR_S ||
         l == OWC_PR_F || l == OWC_SR_F || l == OWC_S1_S || l == OWC_S1_F ||
         l == OWC_WR_S || l == OWC_WR_F || l == OWC_W1_S || l == OWC_W1_F ||
         l == DR_RED_KEY || l == DR_YELLOW_KEY || l == DR_BLUE_KEY;
}

bool is_a_direct_door(
    linedef *line) { // linedef where a door is RIGHT behind it
  i16 l = line->line_type;
  return !(line->sector_tag != 0 || l == CSC_SR_S || l == CSC_SR_F || l == CSC_S1_S || l == CSC_S1_F ||
           l == CWO_SR_S || l == CWO_S1_S || l == OSO_SR_S || l == OSO_SR_F ||
           l == OSO_S1_S || l == OSO_S1_F || l == OWC_SR_S || l == OWC_SR_F ||
           l == OWC_S1_S || l == OWC_S1_F);
}

door *create_door_from_linedef(linedef *line, sidedef *sd,
                               enum DoorTypes line_type) {
  door *d;
  enum DoorTransitionSpeed speed;
  bool repeatable;
  switch (line_type) {
  case CSC_SR_S:
  case CSC_SR_F:
    speed = line_type == CSC_SR_S ? SLOW : FAST;
    line->is_colllidable = false;
    line->is_shootable = false;
    line->is_repeatable = true;
    line->is_pushable = true;
    d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, sd->sector);
    break;
  case CSC_S1_S:
  case CSC_S1_F:
    line->is_colllidable = false;
    line->is_shootable = false;
    line->is_repeatable = false;
    line->is_pushable = true;
    speed = line_type == CSC_S1_S ? SLOW : FAST;
    d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, sd->sector);
    break;
  case CSC_WR_S:
  case CSC_WR_F:
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = true;
    speed = line_type == CSC_WR_S ? SLOW : FAST;
    d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, sd->sector);
    break;
  case CSC_W1_S:
  case CSC_W1_F:
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = false;
    speed = line_type == CSC_W1_S ? SLOW : FAST;
    d = door_create(NULL, speed, CLOSE_STAY_CLOSED, 0, sd->sector);
    break;
  case CWO_SR_S:
  case CWO_S1_S:
    repeatable = line_type == CWO_SR_S;
    line->is_colllidable = false;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    line->is_pushable = true;
    d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 30000, sd->sector);
    break;
  case CWO_WR_S:
  case CWO_W1_S:
    repeatable = line_type == CWO_WR_S;
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    d = door_create(NULL, SLOW, CLOSE_WAIT_OPEN, 30000, sd->sector);
    break;
  case OSO_P1_S:
  case OSO_P1_F:
  case OSO_WR_S:
  case OSO_WR_F:
  case OSO_W1_S:
  case OSO_W1_F:
    repeatable = line_type == OSO_WR_S || line_type == OSO_WR_S;
    speed = (line_type == OSO_P1_S || line_type == OSO_WR_S ||
             line_type == OSO_W1_S)
                ? SLOW
                : FAST;
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    d = door_create(NULL, speed, OPEN_STAY_OPEN, 0, sd->sector);
    break;
  case OSO_SR_S:
  case OSO_SR_F:
  case OSO_S1_S:
  case OSO_S1_F:
    speed = (line_type == OSO_SR_S || line_type == OSO_S1_S) ? SLOW : FAST;
    repeatable = line_type == OSO_SR_S || line_type == OSO_SR_S;
    line->is_colllidable = false;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    line->is_pushable = true;
    d = door_create(NULL, speed, OPEN_STAY_OPEN, 0, sd->sector);
    break;
  case OSO_GR_S:
    line->is_colllidable = false;
    line->is_shootable = true;
    line->is_repeatable = false;
    d = door_create(NULL, SLOW, OPEN_STAY_OPEN, 0, sd->sector);
    break;
  case OWC_PR_S:
  case OWC_PR_F:
  case OWC_WR_S:
  case OWC_WR_F:
  case OWC_W1_S:
  case OWC_W1_F:
    repeatable = !(line_type == OWC_W1_S || line_type == OWC_W1_S);
    speed = (line_type == OWC_PR_S || line_type == OWC_WR_S ||
             line_type == OWC_W1_S)
                ? SLOW
                : FAST;
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    d = door_create(NULL, speed, OPEN_WAIT_CLOSE, 4000, sd->sector);
    break;
  case OWC_SR_S:
  case OWC_SR_F:
  case OWC_S1_S:
  case OWC_S1_F:
    speed = (line_type == OWC_SR_S || line_type == OWC_S1_S) ? SLOW : FAST;
    repeatable = line_type == OWC_SR_S || line_type == OWC_SR_S;
    line->is_colllidable = true;
    line->is_shootable = false;
    line->is_repeatable = repeatable;
    line->is_pushable = true;
    d = door_create(NULL, speed, OPEN_WAIT_CLOSE, 4000, sd->sector);
    break;
  case DR_RED_KEY:
  case DR_YELLOW_KEY:
  case DR_BLUE_KEY:
    line->is_colllidable = true;
    line->is_shootable = true;
    line->is_repeatable = true;
    line->is_pushable = false;
    d = door_create(NULL, SLOW, OPEN_WAIT_CLOSE, 4000, sd->sector);
    break;
  default:
    printf("Linedef with unknown type:  %d\n", line_type);
    exit(1);
  }
  line->has_doors = true;
  return d;
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

linedef* get_linedef_by_sector_id(linedef **linedefs, int len_linedefs,
                                   i16 sector_id) {
  for (int i = 0; i < len_linedefs; i++) {
    if (linedefs[i]->has_back_sidedef && (linedefs[i]->front_sidedef->sector_id == sector_id || linedefs[i]->back_sidedef->sector_id == sector_id)) {
      return linedefs[i];
    }
  }
  return NULL;
}

int min_neighboring_heights(linedef **lines, int len_linedefs, i16 sector_id) {
  int mini = INT_MAX;
  sidedef *door_sidedef = NULL;
  sidedef *neighbor_sidedef = NULL;
  for (int i = 0; i < len_linedefs; i++) {
    linedef *line = lines[i];
    if (!line->has_back_sidedef)
      continue; // no back sidedef, so it is uncrossable and height doesn't
                // matter
    set_correct_sidedefs(line, &door_sidedef, &neighbor_sidedef);
    if (door_sidedef == NULL || neighbor_sidedef == NULL)
      continue; // line isn't part of a door sector
    if (door_sidedef->sector_id == sector_id) {
      mini = min(mini, neighbor_sidedef->sector->ceiling_height);
    }
  }
  return mini;
}

door **get_doors(linedef **linedefs, int len_linedefs, int *doors_count,
                 sector *sectors, int len_sectors) {
  door **sectors_doors =
      malloc(sizeof(door *) *
             len_sectors); // since a door can be targeted by several linedefs
                           // with differents actions, we need to log each door
  for (int i = 0; i < len_sectors; i++) {
    sectors_doors[i] = NULL;
  }
  door **doors = malloc(sizeof(door *) * DOORS_COUNT); // actual door* array
  int door_index = 0;

  *doors_count = DOORS_COUNT;

  sidedef *door_sidedef = NULL;
  sidedef *neighbor_sidedef = NULL;
  for (int i = 0; i < len_linedefs; i++) {
    linedef *line = linedefs[i];
    if (!is_a_door(line))
      continue;                   // not a door
    if (is_a_direct_door(line)) { // door is RIGHT behind this linedef
      set_correct_sidedefs(line, &door_sidedef,
                           &neighbor_sidedef); // set up the correct sidedefs to make sure we're setting the right sector for the door
      i16 door_sector_id = door_sidedef->sector_id;
      door *d = create_door_from_linedef(line, door_sidedef, line->line_type);

      if (sectors_doors[door_sector_id] == NULL) { // no door registered yet
        line->door = line->has_doors ? add_door(line->door, d) : d; // in case linedefs can activate multiple doors
        sectors_doors[door_sector_id] = d;
        doors[door_index] = d;
        door_index++;
      } else {
        line->door = add_door(line->door, sectors_doors[door_sector_id]); // the door already exists, just link the linedef to it
        free(d);
        continue;
      }

      int minimal_neighbor_height = min_neighboring_heights(
          linedefs, len_linedefs,
          door_sector_id); // find the minimal heights of surrounding sectors
      int delta_height =
          minimal_neighbor_height -
          door_sidedef->sector->ceiling_height; // calculates height difference
      door_update_height(line->door, delta_height);
    } else { // can be either a button to activate or just walking through a
             // specific linedef
      u16 sector_tag = line->sector_tag;
      for (int sector_id = 0; sector_id < len_sectors;sector_id++) {
        if (sectors[sector_id].tag_number != sector_tag) continue; // not the right sector
        linedef* sector_linedef = get_linedef_by_sector_id(linedefs, len_linedefs, sector_id); //  get a linedef that is part of the door sector
        set_correct_sidedefs(sector_linedef, &door_sidedef, &neighbor_sidedef);
        door* d = create_door_from_linedef(line, door_sidedef,
                                           line->line_type);
        if (sectors_doors[sector_id] == NULL) {
          line->door = line->has_doors ? add_door(line->door, d) : d;
          sectors_doors[sector_id] = d;
          doors[door_index] = d;
          door_index++;
        } else {
          line->door = add_door(line->door, sectors_doors[sector_id]);
          free(d);
          continue;
        }
        int minimal_neighbor_height =
            min_neighboring_heights(linedefs, len_linedefs, sector_id);
        int delta_height = minimal_neighbor_height -
                           door_sidedef->sector->ceiling_height;
        fflush(stdout);
        door_update_height(d, delta_height);
      }
    }
  }
  *doors_count = door_index;
  return doors;
}