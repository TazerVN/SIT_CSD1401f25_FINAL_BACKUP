#pragma once
#ifndef UTIL_HEADER
#define UTIL_HEADER

#include "Global.h"

float absolute(float num);
int intersectPointToCircle(float pointX, float pointY, float circleX, float circleY, float diameter);
int interSectRectToRect(float r1X, float r1Y, float r1W, float r1H, float r2X, float r2Y, float r2W, float r2H);
int interSectCircleToCircle(float c1X, float c1Y, float c1D, float c2X, float c2Y, float c2D);
int interSectRectToCircle(float rX, float rY, float rW, float rH, float cX, float cY, float cD);
int IsAreaClicked(float area_center_x, float area_center_y, float area_width, float area_height, float click_x, float click_y);
int isSliderClicked(float left_x, float left_y, float right_x, float right_y, float click_x, float click_y);
float SliderDragReturn(float left_x, float left_y, float right_x, float right_y, float click_x, float click_y);
#endif // !UTIL_HEADER
