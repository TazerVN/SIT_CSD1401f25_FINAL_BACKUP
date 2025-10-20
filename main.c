#include "cprocessing.h"
#include "World.h"


int main(void)
{
	CP_System_SetWindowSize(1600, 900);
	CP_Engine_SetNextGameState(world_init, world_update, world_exit);
	CP_Engine_Run(0);
	return 0;
}
