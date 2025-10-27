#include "cprocessing.h"
#include "util.h"
#include "World.h"
#include <math.h>
#include "menu.h"
#include "audio.h"
#include "options.h"
#include "tilemap.h"

CP_Vector menu_background[64][64];

//declare 2 rectangle with the type Rectange
Button start_button = {0};
Button option_button = {0};
Button exit_button = {0};

//declare window size
CP_Vector window_size;
CP_Font font_menu;

CP_Vector mouse_position;


int alpha;

float exit_timer;
float menu_offset;


void background_menu_init() {
	CP_Random_Seed(1);
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			menu_background[i][j] = CP_Vector_Set(0, CP_Random_RangeInt(0, 3));
		}
	}
}

void background_render(float* offset) {
	
	for (int i = 0; i < 64; i++) {
		for (int j = 0; j < 64; j++) {
			int current_x = i * 64 * 2 + *offset;
			int current_y = j * 64 * 2 + *offset;
			if (current_x >= 0 && current_x - 128 <= CP_System_GetWindowWidth() && current_y >= 0 && current_y - 128 <= CP_System_GetWindowHeight()) {
				draw_tile(tilemap_map, current_x, current_y, menu_background[i][j].x, menu_background[i][j].y, 20, 27, 2, FALSE, 0);
			}
			if (i == 63 && j == 63 && (current_x <= CP_System_GetWindowWidth() || current_y <= CP_System_GetWindowHeight())) {
				menu_offset = 0;
			}
		}
	}
}


void Main_Menu_Exit(void)
{

}

void draw_text(float position_x, float position_y, float size, const char* text, float* alpha) {
	CP_Font_Set(font_menu);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(size);



	CP_Settings_Fill(CP_Color_Create(0, 0, 0, *alpha));
	CP_Font_DrawText(text, position_x + 5, position_y + 5);

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, *alpha));
	CP_Font_DrawText(text, position_x, position_y);
}



void exit_menu(void) {
	//CP_Font_Free(font);
	free_audio();
	CP_Font_Free(font_menu);
	CP_Engine_Terminate();
}

void nextState(void) {
	CP_Engine_SetNextGameState(world_init, world_update, world_exit);
}

void options_menu(void) {
	CP_Engine_SetNextGameState(options_init, options_update, options_exit);
}

//draw a rectangle and check if its being clicked
void rect_button(struct Button* rect, CP_Vector mouse_position, void (*func)(void), const char* text, float* alpha) {
	if (rect->exit_activation == TRUE) {
		exit_timer += CP_System_GetDt();
		*alpha -= (*alpha) * (1 - exp(-CP_System_GetDt() * 10));
		if (exit_timer >= 0.3) {
			func();
		}
	}

	if (IsAreaClicked(rect->position.x, rect->position.y, rect->scale.x, rect->scale.y, mouse_position.x, mouse_position.y)) {
		rect->lerp += (1 - rect->lerp) * (1 - exp(-CP_System_GetDt() * 10));
		if (rect->lerp >= 1) {
			rect->lerp = 1;
		}
	}
	else {
		rect->lerp -= (1 - rect->lerp) * (1 - exp(-CP_System_GetDt() * 7));
		if (rect->lerp < 0) {
			rect->lerp = 0;
		}
	}

	CP_Vector draw_scale = CP_Vector_Set((CP_Math_LerpFloat(rect->scale.x, rect->scale.x * 1.05, rect->lerp)),
		(CP_Math_LerpFloat(rect->scale.y, rect->scale.y * 1.05, rect->lerp)));

	CP_Settings_RectMode(CP_POSITION_CENTER);


	rect->color = CP_Color_Create(10, 50, 100, *alpha);
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 0));
	CP_Settings_NoStroke();


	CP_Settings_Fill(CP_Color_Create(0 ,0, 0, *alpha - 50));
	CP_Graphics_DrawRectAdvanced(rect->position.x + 5, rect->position.y + 10, draw_scale.x, draw_scale.y, 0, 25);

	CP_Settings_Fill(rect->color);
	CP_Graphics_DrawRectAdvanced(rect->position.x, rect->position.y, draw_scale.x, draw_scale.y, 0, 25);


	CP_Settings_Fill(CP_Color_Create(255, 255, 255, *alpha));
	CP_Font_Set(font_menu);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
	CP_Settings_TextSize(CP_Math_LerpInt(50, 60, rect->lerp));
	CP_Font_DrawText(text, rect->position.x, rect->position.y);


	if (CP_Input_MouseClicked()) {
		if (IsAreaClicked(rect->position.x, rect->position.y, rect->scale.x, rect->scale.y, mouse_position.x, mouse_position.y)) {
			rect->exit_activation = TRUE;
		}
	}

}


void Main_Menu_Init(void)
{
	//get window size
	menu_offset = 0;
	window_size = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());
	exit_timer = 0;

	alpha = 255;

	//set position, width and height for start button
	start_button.position = CP_Vector_Set(window_size.x / 6, window_size.y / 2);
	start_button.scale = CP_Vector_Set(window_size.x / 6, window_size.y / 8);
	start_button.lerp = 0;
	start_button.exit_activation = FALSE;

	//set position, width and height for option button
	option_button.position = CP_Vector_Set(window_size.x / 6, window_size.y / 2 + 150);
	option_button.scale = CP_Vector_Set(window_size.x / 6, window_size.y / 8);
	option_button.lerp = 0;
	option_button.exit_activation = FALSE;

	//set position, width and height for exit button
	exit_button.position = CP_Vector_Set(window_size.x / 6, window_size.y / 2 + 300);
	exit_button.scale = CP_Vector_Set(window_size.x / 6, window_size.y / 8);
	exit_button.lerp = 0;
	exit_button.exit_activation = FALSE;
	font_menu = CP_Font_Load("Assets/PixelAE_Regular.ttf");

	tile_init();
	background_menu_init();
}

void Main_Menu_Start() {
	Main_Menu_Init();
	init_audio();
	play_music();
}

void Main_Menu_Update(void)
{
	mouse_position = CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY());

	menu_offset -= CP_System_GetDt() * 20;

	CP_Graphics_ClearBackground(CP_Color_CreateHex(0x91ffff));

	background_render(&menu_offset);

	CP_Settings_RectMode(CP_POSITION_CENTER);

	draw_text(window_size.x / 4, window_size.y / 4, 200, "ZomBAT", &alpha);

	rect_button(&start_button, mouse_position, nextState, "START", &alpha);
	rect_button(&option_button, mouse_position, options_menu, "OPTION", &alpha);
	rect_button(&exit_button, mouse_position, exit_menu, "EXIT", &alpha);

	audio_engine();

}


