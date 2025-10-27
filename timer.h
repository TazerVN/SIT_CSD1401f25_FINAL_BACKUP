#pragma once
#include "cprocessing.h"


typedef struct Timer {
	float current_time;
	float max_time;
	CP_BOOL start;
}Timer;


Timer timer_init(float current,float max_timer);

int timer_update(Timer* timer);

void timer_reset(Timer* timer);
