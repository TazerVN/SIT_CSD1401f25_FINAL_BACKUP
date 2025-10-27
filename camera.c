#include "cprocessing.h"
#include "World.h"
#include "math.h"
#include "timer.h"

CP_Vector offset;
CP_Vector offset_stored;


Timer shake_timer;

typedef struct Effect {
	CP_BOOL play;
	Timer duration;
	CP_BOOL looping;
};


void camera_init() {
	shake_timer = timer_init(0.2, 0.2);
	offset.x = 1;
	offset.y = 1;
}

void camera_shake(float magnitude) {
	if (timer_update(&shake_timer)) {
		CP_Vector random = CP_Vector_Set(CP_Random_Gaussian(), CP_Random_Gaussian());
		offset.x += random.x * magnitude ;
		offset.y += random.y * magnitude ;
	}
}

//change offset base on the player position to keep player center
void camera_update(float px, float py) {
	CP_Vector screen_center = CP_Vector_Set(CP_System_GetWindowWidth()/2, CP_System_GetWindowHeight()/2);
		offset.x = screen_center.x - px;
		offset.y = screen_center.y - py;
		camera_shake(5.0);
}




