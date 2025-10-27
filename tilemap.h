#pragma once

#ifndef TILE_HEADER
#define TILE_HEADER

#include "cprocessing.h"

typedef struct Sprite {
	CP_Image spritesheet;
	CP_Vector size;
	CP_Vector index;
	CP_BOOL flip;

};

extern CP_Image tilemap_map;
extern CP_Image tilemap_player;

void tile_init();

float anglecal(float origin_x, float origin_y, float end1_x, float end1_y, float end2_x, float end2_y);

void sprite_init(struct Sprite* p, const char* filepath, int index_x, int index_y, int width, int height);

void draw_tile(CP_Image tilemap, float position_x, float position_y, int index_row, int index_collumn, int row, int collumn, int scale, CP_BOOL flip, float rotation);

void generate_tile(CP_Image tilemap, int row, int collumn, int scale);

#endif // !TILE_HEADER