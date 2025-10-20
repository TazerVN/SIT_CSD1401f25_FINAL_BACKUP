#pragma once
#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include "util.h"
#include "cprocessing.h"

enum EntityShape {circle,rectangle};

typedef struct Entity {
	int isActive;
	CP_Vector pos;
	CP_Vector posOS;
	CP_Vector area;	//if rectangle used
	float diameter; //if circle used
	float accel, curMaxSpeed, curSpeed, iniSpeed;
	enum EntityShape shapeType;
	CP_Vector direction;
	CP_Vector velocity; 
	CP_Vector acceleration; 
};

void initializeRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area, float MaxSpeed, float speedIncrease);
void initializeCircle(struct Entity* targetEntity, CP_Vector pos, float diameter, float MaxSpeed, float speedIncrease);
void updateEntity(struct Entity* targetEntity);
void applyAcceleration(struct Entity* target, CP_Vector direction);
void setAcceleration(struct Entity* target, CP_Vector newAccel);
#endif // !ENTITY_HEADER
