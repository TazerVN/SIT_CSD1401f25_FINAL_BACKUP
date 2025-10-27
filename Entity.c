#include "Entity.h"
#include "cprocessing.h"
#include "camera.h"

void initializeStaticRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area) {
	targetEntity->isActive = 1;
	targetEntity->isStatic = 1;
	targetEntity->pos = pos;
	targetEntity->posOS = pos;
	targetEntity->area = area;
	targetEntity->shapeType = rectangle;
}

void initializeStaticCircleEntity(struct Entity* targetEntity, CP_Vector pos, float diameter) {
	targetEntity->isActive = 1;
	targetEntity->isStatic = 1;
	targetEntity->pos = pos;
	targetEntity->posOS = pos;
	targetEntity->diameter = diameter;
	targetEntity->shapeType = circle;
}

void initializeRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area, float MaxSpeed, float speedIncrease,float drag,float velocityClamp) {
	targetEntity->isActive = 1;
	targetEntity->isStatic = 0;
	targetEntity->launchedTime = 0.0f;
	targetEntity->pos = pos;
	targetEntity->posOS = pos;
	targetEntity->area = area;
	targetEntity->shapeType = rectangle;

	targetEntity->iniSpeed = MaxSpeed;
	targetEntity->curMaxSpeed = MaxSpeed;
	targetEntity->curSpeed = 0.0f;
	targetEntity->iniaccel = speedIncrease;
	targetEntity->accel = speedIncrease;
	targetEntity->CoDrag = drag;
	targetEntity->velocityClamp = velocityClamp;

	targetEntity->velocity = CP_Vector_Zero();
	targetEntity->acceleration = CP_Vector_Zero();
	targetEntity->applied_force = CP_Vector_Zero();

	targetEntity->state = IDLE;
}

void initializeCircle(struct Entity* targetEntity, CP_Vector pos, float diameter, float MaxSpeed, float speedIncrease,float drag, float velocityClamp) {
	targetEntity->isActive = 1;
	targetEntity->isStatic = 0;
	targetEntity->launchedTime = 0.0f;
	targetEntity->pos = pos;
	targetEntity->posOS = pos;
	targetEntity->diameter = diameter;
	targetEntity->shapeType = circle;

	targetEntity->iniSpeed = MaxSpeed;
	targetEntity->curMaxSpeed = MaxSpeed;
	targetEntity->curSpeed = 0.0f;
	targetEntity->iniaccel = speedIncrease;
	targetEntity->accel = speedIncrease;
	targetEntity->CoDrag = drag;
	targetEntity->velocityClamp = velocityClamp;

	targetEntity->velocity = CP_Vector_Zero();
	targetEntity->acceleration = CP_Vector_Zero();
	targetEntity->applied_force = CP_Vector_Zero();

	targetEntity->state = IDLE;
}

void destroyEntity(struct Entity* targetEntity) {
	//enum
	targetEntity->state = UNIN;
	targetEntity->shapeType = uninshape;
	//floats
	targetEntity->isActive = 0;
	targetEntity->isStatic = 0;
	targetEntity->launchedTime = 0.0f;
	targetEntity->diameter = 0.0f;
	targetEntity->iniSpeed = 0.0f;
	targetEntity->curMaxSpeed = 0.0f;
	targetEntity->curSpeed = 0.0f;
	targetEntity->iniaccel = 0.0f;
	targetEntity->accel = 0.0f;
	targetEntity->CoDrag = 0.0f;
	targetEntity->velocityClamp = 0.0f;
	//CP_Vector
	targetEntity->pos = CP_Vector_Zero();
	targetEntity->posOS = CP_Vector_Zero();
	targetEntity->velocity = CP_Vector_Zero();
	targetEntity->acceleration = CP_Vector_Zero();
	targetEntity->applied_force = CP_Vector_Zero();
}

void updateEntity(struct Entity* targetEntity) {

	if (targetEntity->state == LAUNCHED) {
		targetEntity->launchedTime += CP_System_GetDt();
		if (targetEntity->launchedTime >= 0.5f) {
			targetEntity->state = IDLE;
			targetEntity->launchedTime = 0.0f;
		}
	}

	//if the target is initilaized and is not static then apply physics
	if (targetEntity->isActive && !targetEntity->isStatic) {

		//friction shit
		/*if (targetEntity->state == IDLE && (targetEntity->applied_force.x != 0.0f || targetEntity->applied_force.y != 0.0f)) {
			if (targetEntity->applied_force.x < 0.0f) {
				targetEntity->applied_force.x += targetEntity->accel * CP_System_GetDt();
				targetEntity->applied_force.x = (targetEntity->applied_force.x > 0.0f) ? 0.0f : targetEntity->applied_force.x;
			}
			else if (targetEntity->applied_force.x > 0.0f) {
				targetEntity->applied_force.x -= targetEntity->accel * CP_System_GetDt();
				targetEntity->applied_force.x = (targetEntity->applied_force.x < 0.0f) ? 0.0f : targetEntity->applied_force.x;
			}

			if (targetEntity->applied_force.y < 0.0f) {
				targetEntity->applied_force.y += targetEntity->accel * CP_System_GetDt();
				targetEntity->applied_force.y = (targetEntity->applied_force.y > 0.0f) ? 0.0f : targetEntity->applied_force.y;
			}
			else if (targetEntity->applied_force.y > 0.0f) {
				targetEntity->applied_force.y -= targetEntity->accel * CP_System_GetDt();
				targetEntity->applied_force.y = (targetEntity->applied_force.y < 0.0f) ? 0.0f : targetEntity->applied_force.y;
			}
		}*/

		//if idle start decelerating
		if (targetEntity->state == IDLE || targetEntity->state == ATTACKING) {
			targetEntity->acceleration.x = -targetEntity->velocity.x * targetEntity->CoDrag;
			targetEntity->acceleration.y = -targetEntity->velocity.y * targetEntity->CoDrag;
		}

		//acceleration
		targetEntity->velocity.x += targetEntity->acceleration.x * CP_System_GetDt();
		targetEntity->velocity.y += targetEntity->acceleration.y * CP_System_GetDt();

		//only clamp velocity when moving
		if (targetEntity->state == MOVING) {
			targetEntity->velocity.x = CP_Math_ClampFloat(targetEntity->velocity.x, -targetEntity->curMaxSpeed, targetEntity->curMaxSpeed);
			targetEntity->velocity.y = CP_Math_ClampFloat(targetEntity->velocity.y, -targetEntity->curMaxSpeed, targetEntity->curMaxSpeed);
		}

		//reset the velocity to 0 when very low
		if (absolute(targetEntity->velocity.x) < targetEntity->velocityClamp) targetEntity->velocity.x = 0.0f;
		if (absolute(targetEntity->velocity.y) < targetEntity->velocityClamp) targetEntity->velocity.y = 0.0f;

		//update position based on velocity
		if (targetEntity->velocity.x != 0.0f || targetEntity->velocity.y != 0.0f) {
			targetEntity->pos.x += targetEntity->velocity.x * CP_System_GetDt();
			targetEntity->pos.y += targetEntity->velocity.y * CP_System_GetDt();
		}

		targetEntity->posOS.x = targetEntity->pos.x + offset.x;
		targetEntity->posOS.y = targetEntity->pos.y + offset.y;
		
	}
}

void move(struct Entity* target, CP_Vector direction) {
	//apply the acceleration in the direction;
	target->acceleration.x = target->accel * direction.x;
	target->acceleration.y = target->accel * direction.y;
}