#pragma once
#ifndef ENTITY_HEADER
#define ENTITY_HEADER

#include "util.h"
#include "cprocessing.h"

enum EntityShape { uninshape,circle,rectangle};
enum EntityState { UNIN, IDLE, MOVING, ATTACKING, LAUNCHED ,DEAD };

typedef struct Entity {
	int isActive, isStatic;
	CP_Vector pos;
	CP_Vector posOS;
	CP_Vector area;	//if rectangle used
	float diameter; //if circle used
	float accel, iniaccel;
	float curMaxSpeed, curSpeed, iniSpeed;
	float CoDrag, velocityClamp;
	float launchedTime;
	enum EntityShape shapeType;
	enum EntityState state;
	CP_Vector velocity; 
	CP_Vector acceleration;
	CP_Vector applied_force;
};

void initializeStaticRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area);
void initializeStaticCircleEntity(struct Entity* targetEntity, CP_Vector pos, float diameter);
void initializeRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area, float MaxSpeed, float speedIncrease,float drag, float velocityClamp);
void initializeCircle(struct Entity* targetEntity, CP_Vector pos, float diameter, float MaxSpeed, float speedIncrease,float drag, float velocityClamp);
void destroyEntity(struct Entity* targetEntity);

void updateEntity(struct Entity* targetEntity);
void move(struct Entity* target, CP_Vector direction);
#endif // !ENTITY_HEADER
