#pragma once

#include "cprocessing.h"

typedef struct UIBar {
	CP_Vector position;
	CP_Vector scale;
	CP_Color color;
}UIBar;

typedef struct Slider {
	float max;
	float current;
	struct UIBar max_bar;
	struct UIBar current_bar;
}Slider;

void options_init();
void options_update();
void options_exit();