#include "cprocessing.h"
#include "tilemap.h"
#include "math.h"

CP_Image tilemap_map;
CP_Image tilemap_player;


const float square(float x) {
	return x * x;
}

float anglecal(float origin_x, float origin_y, float end1_x , float end1_y, float end2_x, float end2_y) {
	const double PI = 3.1415926;
	CP_Vector v1 = CP_Vector_Set(end1_x - origin_x, (end1_y - origin_y));
	CP_Vector v2 = CP_Vector_Set(end2_x - origin_x, (end2_y - origin_y));

	float x = (v1.x * v2.x + v1.y * v2.y) / (sqrt(square(v1.x) + square(-v1.y)) * (sqrt(square(v2.x) + square(v2.y))));

	if (v2.y > 0) {
		return acos(x) * 180 / PI;
	}
	else {
		return 360 - (acos(x) * 180 / PI);
	}
	

}



void tile_init() {
	tilemap_map = CP_Image_Load("Assets/tilesheet_complete_2X.png");
	tilemap_player = CP_Image_Load("Assets/spritesheet_characters.png");
}

void sprite_init(struct Sprite* p,const char *filepath, int index_x, int index_y, int width, int height) {
	p->spritesheet = CP_Image_Load(filepath);
	p->index.x = index_x;
	p->index.y = index_y;
	p->size.x = width;
	p->size.y = height;
}



void draw_tile(CP_Image tilemap, float position_x, float position_y, int index_row, int index_collumn, int row, int collumn, int scale, CP_BOOL flip, float rotation) {
	int tm_width = CP_Image_GetWidth(tilemap);
	int tm_height = CP_Image_GetHeight(tilemap);

	float tw = (scale * tm_width / collumn);
	float th = (scale * tm_height / row);

	float s0 = index_collumn * tm_width / collumn;
	float t0 = index_row * tm_height / row;

	float s1 = (index_collumn + 1) * tm_width / collumn;
	float t1 = (index_row + 1) * tm_height / row;

	CP_Settings_ImageFilterMode(CP_IMAGE_FILTER_NEAREST);
	if (flip == FALSE) {
		CP_Image_DrawSubImageAdvanced(tilemap, position_x, position_y, tw, th, s0, t0, s1, t1, 255, rotation);
	}
	else {
		CP_Image_DrawSubImageAdvanced(tilemap, position_x, position_y, -tw, th, s0, t0, s1, t1, 255, rotation);
	}
}

void generate_tile(CP_Image tilemap, int row, int collumn, int scale) {
	int tm_width = CP_Image_GetWidth(tilemap);
	int tm_height = CP_Image_GetHeight(tilemap);

	int window_width = CP_System_GetWindowWidth();
	int window_height = CP_System_GetWindowHeight();

	int tw = tm_width / collumn;
	int th = tm_height / row;


	CP_Random_Seed(1);
	for (int i = 0; i < window_width / (tw * scale) * 2; i++) {
		for (int j = 0; j < window_height / (th * scale) * 2; j++) {
			int random_index_row = CP_Random_RangeInt(2, 5);
			int random_index_collumn = CP_Random_RangeInt(0, 2);
			draw_tile(tilemap, i * tw * scale, j * th * scale, random_index_collumn, random_index_row, row, collumn, scale, FALSE, 0);
		}
	}
}