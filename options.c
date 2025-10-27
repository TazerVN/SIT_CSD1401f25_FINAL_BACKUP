#include "cprocessing.h"
#include "menu.h"
#include "World.h"
#include "options.h"
#include "audio.h"
#include "util.h"

Button returning;

Slider music_slider;
Slider sfx_slider;
Slider ambience_slider;

Slider slider_init(float current, float max, float position_x, float position_y, float width, float height) {
	Slider slider;
	slider.current = current;
	slider.max = max;

	slider.current_bar.position.x = position_x;
	slider.current_bar.position.y = position_y;
	slider.current_bar.scale.x = width * (current/max);
	slider.current_bar.scale.y = height;
	slider.current_bar.color = CP_Color_Create(255,255,255,255);

	slider.max_bar.position.x = position_x;
	slider.max_bar.position.y = position_y;
	slider.max_bar.scale.x = width;
	slider.max_bar.scale.y = height;
	slider.max_bar.color = CP_Color_Create(0,0,0,255);
	return slider;
}

void slider_update(Slider* slider, SoundSetting* setting) {

	float right_x = slider->max_bar.position.x + 300 + slider->max_bar.scale.x;
	float right_y = slider->max_bar.position.y + slider->max_bar.scale.y;
	if (CP_Input_MouseDown(MOUSE_BUTTON_1)) {
		if (isSliderClicked(slider->max_bar.position.x + 300, slider->max_bar.position.y, right_x, right_y, CP_Input_GetMouseX(), CP_Input_GetMouseY())) {
			slider->current = SliderDragReturn(slider->max_bar.position.x + 300, slider->max_bar.position.y, right_x, right_y, CP_Input_GetMouseX(), CP_Input_GetMouseY());
			slider->current_bar.scale.x = slider->max_bar.scale.x * (slider->current/slider->max);
			setting->volume = slider->current;
		}
	}
}


void slider_render(Slider* slider, SoundSetting* setting, const char* text, int alpha) {

	slider_update(slider, setting);

	slider->current_bar.color = CP_Color_Create(255, 255, 255, alpha);
	slider->max_bar.color = CP_Color_Create(0, 0, 0, alpha);

	
	CP_Font_Set(font_menu);
	CP_Settings_TextSize(75);
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_BOTTOM);
	//shadow drop
	CP_Settings_Fill(CP_Color_Create(0, 0, 25, alpha));
	CP_Font_DrawText(text, slider->current_bar.position.x + 5, slider->current_bar.position.y + 70);

	CP_Settings_Fill(CP_Color_Create(255, 255, 255, alpha));
	CP_Font_DrawText(text, slider->current_bar.position.x, slider->current_bar.position.y + 65);


	//full bar render
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(CP_Color_Create(0, 0, 0, alpha));
	CP_Graphics_DrawRectAdvanced(slider->max_bar.position.x + 300, slider->max_bar.position.y + 10, slider->max_bar.scale.x, slider->max_bar.scale.y, 0, 5);
	CP_Settings_Fill(slider->max_bar.color);
	CP_Graphics_DrawRectAdvanced(slider->max_bar.position.x + 300, slider->max_bar.position.y, slider->max_bar.scale.x, slider->max_bar.scale.y, 0, 5);



	//current bar
	CP_Settings_RectMode(CP_POSITION_CORNER);
	CP_Settings_Fill(slider->current_bar.color);
	CP_Graphics_DrawRectAdvanced(slider->current_bar.position.x + 300, slider->current_bar.position.y, slider->current_bar.scale.x, slider->current_bar.scale.y, 0, 5);

}
void options_return(void) {
	CP_Engine_SetNextGameState(Main_Menu_Init, Main_Menu_Update, Main_Menu_Exit);
}

void options_init(void) {
	CP_Vector window_size = CP_Vector_Set(CP_System_GetWindowWidth(), CP_System_GetWindowHeight());

	float slider_width = 5 * window_size.x / 8;
	float slider_height = window_size.y / 20;
	float slider_origin_x = window_size.x / 6 - window_size.x / 12;
	float slider_origin_y = window_size.y / 6;
	
	music_slider = slider_init(au.music.volume, 1, slider_origin_x, slider_origin_y, slider_width, slider_height);
	sfx_slider = slider_init(au.sfx.volume, 1, slider_origin_x, slider_origin_y + 150, slider_width, slider_height);
	ambience_slider = slider_init(au.ambience.volume, 1, slider_origin_x, slider_origin_y + 300, slider_width, slider_height);

	//set position, width and height for exit button
	returning.position = CP_Vector_Set(window_size.x / 6, window_size.y / 2 + 300);
	returning.scale = CP_Vector_Set(window_size.x / 6, window_size.y / 8);
	returning.lerp = 0;
	returning.exit_activation = FALSE;
	alpha = 255;
	exit_timer = 0;

}

void options_update(void) {
	CP_Graphics_ClearBackground(CP_Color_CreateHex(0x91ffff));
	mouse_position = CP_Vector_Set(CP_Input_GetMouseWorldX(), CP_Input_GetMouseWorldY());
	rect_button(&returning, mouse_position, options_return, "RETURN", &alpha);
	slider_render(&music_slider, &au.music , "Music", alpha);
	slider_render(&sfx_slider, &au.sfx , "SFX", alpha);
	slider_render(&ambience_slider, &au.ambience  ,"Ambience", alpha);

	audio_engine();
}

void options_exit(void) {
}