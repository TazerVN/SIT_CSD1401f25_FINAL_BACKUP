#include "Entity.h"
#include "cprocessing.h"

void initializeRectangleEntity(struct Entity* targetEntity, CP_Vector pos, CP_Vector area, float MaxSpeed, float speedIncrease) {
	targetEntity->isActive = 1;
	targetEntity->pos = pos;
	targetEntity->area = area;
	targetEntity->shapeType = rectangle;

	targetEntity->iniSpeed = MaxSpeed;
	targetEntity->curMaxSpeed = MaxSpeed;
	targetEntity->curSpeed = 0.0f;
	targetEntity->accel = speedIncrease;

	targetEntity->direction = CP_Vector_Zero();
	targetEntity->velocity = CP_Vector_Zero();
	targetEntity->acceleration = CP_Vector_Zero();
}

void initializeCircle(struct Entity* targetEntity, CP_Vector pos, float diameter, float MaxSpeed, float speedIncrease) {
	int isActive = 1; targetEntity->isActive = 1;
	targetEntity->pos = pos;
	targetEntity->diameter = diameter;
	targetEntity->shapeType = circle;

	targetEntity->iniSpeed = MaxSpeed;
	targetEntity->curMaxSpeed = MaxSpeed;
	targetEntity->curSpeed = 0.0f;
	targetEntity->accel = speedIncrease;

	targetEntity->direction = CP_Vector_Zero();
	targetEntity->velocity = CP_Vector_Zero();
	targetEntity->acceleration = CP_Vector_Zero();
}

void updateEntity(struct Entity* targetEntity) {
	if (targetEntity->isActive) {
		if (targetEntity->acceleration.x != 0 || targetEntity->acceleration.y != 0) {
			targetEntity->velocity.x += targetEntity->acceleration.x * CP_System_GetDt();
			targetEntity->velocity.y += targetEntity->acceleration.y * CP_System_GetDt();

			targetEntity->velocity.x = CP_Math_ClampFloat(targetEntity->velocity.x, -targetEntity->curMaxSpeed, targetEntity->curMaxSpeed);
			targetEntity->velocity.y = CP_Math_ClampFloat(targetEntity->velocity.y, -targetEntity->curMaxSpeed, targetEntity->curMaxSpeed);
		}

		if (targetEntity->acceleration.x == 0 && targetEntity->acceleration.y == 0) {
			if (targetEntity->velocity.x > 0) {
				targetEntity->velocity.x -= targetEntity->accel * CP_System_GetDt();
				if (targetEntity->velocity.x < 0) targetEntity->velocity.x = 0.0f;
			}
			else if (targetEntity->velocity.x < 0) {
				targetEntity->velocity.x += targetEntity->accel * CP_System_GetDt();
				if (targetEntity->velocity.x > 0) targetEntity->velocity.x = 0.0f;
			}

			if (targetEntity->velocity.y > 0) {
				targetEntity->velocity.y -= targetEntity->accel * CP_System_GetDt();
				if (targetEntity->velocity.y < 0) targetEntity->velocity.y = 0.0f;
			}
			else if (targetEntity->velocity.y < 0) {
				targetEntity->velocity.y += targetEntity->accel * CP_System_GetDt();
				if (targetEntity->velocity.y > 0) targetEntity->velocity.y = 0.0f;
			}
		}

		if (targetEntity->velocity.x != 0.0f || targetEntity->velocity.y != 0.0f) {
			targetEntity->pos.x += targetEntity->velocity.x * CP_System_GetDt();
			targetEntity->pos.y += targetEntity->velocity.y * CP_System_GetDt();
		}
	}
}

void applyAcceleration(struct Entity* target, CP_Vector direction) {
	target->acceleration.x = target->accel * direction.x;
	target->acceleration.y = target->accel * direction.y;
}

void setAcceleration(struct Entity* target, CP_Vector newAccel) {
	target->acceleration = newAccel;
}