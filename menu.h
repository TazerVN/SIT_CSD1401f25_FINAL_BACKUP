#pragma once
#ifndef MENU_HEADER
#define MENU_HEADER

#include "cprocessing.h"


//define a button object that can be clicked to actitvate
typedef struct Button {
	CP_Vector position;
	CP_Vector scale;
	CP_Color color;
	float lerp;
	CP_BOOL exit_activation;
}Button;

extern CP_Vector mouse_position;
extern float alpha;
extern float exit_timer;
extern CP_Font font_menu;


void rect_button(struct Button* rect, CP_Vector mouse_position, void (*func)(void), const char* text, float* alpha);
void Main_Menu_Init(void);
void Main_Menu_Update(void);
void Main_Menu_Start();
void Main_Menu_Exit(void);
void exit_menu(void);
void nextState(void);

#endif // MENU
