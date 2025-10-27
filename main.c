#include "cprocessing.h"
#include "World.h"
#include "menu.h"
#include "audio.h"


int main(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_SetNextGameState(Main_Menu_Start, Main_Menu_Update, Main_Menu_Exit);
	CP_Engine_Run(0);
	return 0;
}
