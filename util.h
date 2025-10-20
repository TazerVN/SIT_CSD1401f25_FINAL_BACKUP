#pragma once
#ifndef UTIL_HEADER
#define UTIL_HEADER

#include "cprocessing.h"

#define red	0xFF0000FF
#define yellow 0xFDFF00FF
#define green 0x00FF00FF
#define blue 0x0000FFFF
#define white 0xFFFFFFFF
#define black 0x000000FF

int intersectPointToCircle(float pointX, float pointY, float circleX, float circleY, float diameter);
int interSectRectToRect(float r1X, float r1Y, float r1W, float r1H, float r2X, float r2Y, float r2W, float r2H);
int interSectCircleToCircle(float c1X, float c1Y, float c1D, float c2X, float c2Y, float c2D);
int interSectRectToCircle(float rX, float rY, float rW, float rH, float cX, float cY, float cD);
#endif // !UTIL_HEADER
