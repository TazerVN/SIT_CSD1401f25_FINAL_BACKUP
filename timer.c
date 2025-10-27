#include "cprocessing.h"
#include "timer.h"



Timer timer_init(float current, float max_timer) {
	Timer t;
	t.current_time = current;
	t.max_time = max_timer;
	t.start = TRUE;
	return t;
}

int timer_update(Timer* timer) {
	if (timer->start == TRUE && timer->current_time < timer->max_time) {
		timer->current_time += CP_System_GetDt();
		return 1;
	}
	else {
		timer->start = FALSE;
		return 0;
	}
}

void timer_reset(Timer* timer) {
	timer->current_time = 0.0;
	timer->start = TRUE;
}
