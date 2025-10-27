#pragma once
#include "cprocessing.h"
#include "timer.h"


extern CP_Vector offset;
extern Timer shake_timer;


void camera_init();
void camera_update(float px, float py);
void camera_shake(float magnitude);