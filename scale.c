#include "cprocessing.h"
CP_Image logo;
void scale_init(void)
{
	logo = CP_Image_Load("Assets/DigiPen_BLACK.png"); //loads image from the $Dir/Assets/
	CP_Settings_ImageMode(CP_POSITION_CORNER); //set image postion to corner
}

void scale_update(void)
{
	float x = CP_Input_GetMouseX();
	float y = CP_Input_GetMouseY();
	CP_Graphics_ClearBackground(CP_Color_Create(128, 128, 128, 255)); //clear background with solid gray color
	CP_Image_Draw(logo, 0.f, 0.f, x, y, 255);
}

void scale_exit(void)
{
	CP_Image_Free(logo);
}