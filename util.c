#include "util.h"
#include "cprocessing.h"

int intersectPointToCircle(float pointX, float pointY, float circleX, float circleY, float diameter) {
	float radius = diameter * 0.5f;
	float dx = pointX - circleX;
	float dy = pointY - circleY;
	float distanceSquared = (dx*dx+dy*dy);

	if (distanceSquared <= radius * radius)
	{
		return 1;
		int i = 0;
	}
	else {
		return 0; 
		int i = 0;
	}
}

int interSectRectToRect(float r1X, float r1Y, float r1W, float r1H, float r2X, float r2Y, float r2W, float r2H) {
	float r1_minX = r1X - r1W * 0.5f;
	float r1_maxX = r1X + r1W * 0.5f;
	float r1_minY = r1Y - r1H * 0.5f;
	float r1_maxY = r1Y + r1H * 0.5f;

	float r2_minX = r2X - r2W * 0.5f;
	float r2_maxX = r2X + r2W * 0.5f;
	float r2_minY = r2Y - r2H * 0.5f;
	float r2_maxY = r2Y + r2H * 0.5f;

	if (r1_minX <= r2_maxX && r1_maxX >= r2_minX &&
		r1_minY <= r2_maxY && r1_maxY >= r2_minY)
	{
		return 1;
	}
	else return 0;
}

int interSectCircleToCircle(float c1X, float c1Y, float c1D, float c2X, float c2Y, float c2D) {
	float r1 = c1D * 0.5f;
	float r2 = c2D * 0.5f;
	float radii = r1 + r2;

	float dx = c1X - c2X;
	float dy = c1Y - c2Y;
	float distanceSquared = dx * dx + dy * dy;

	if (distanceSquared <= radii * radii) return 1;
	else return 0;
}

int interSectRectToCircle(float rX, float rY, float rW, float rH, float cX, float cY, float cD) {
	float minX = rX - rW * 0.5f;
	float maxX = rX + rW * 0.5f;

	float minY = rY - rH * 0.5f;
	float maxY = rY + rH * 0.5f;

	float nearestX = CP_Math_ClampFloat(cX, minX, maxX);
	float nearestY = CP_Math_ClampFloat(cY, minY, maxY);

	//if nearest point intersect with circle then it is intersecting.
	if (intersectPointToCircle(nearestX, nearestY, cX, cY, cD)) return 1;
	else return 0;
		

}