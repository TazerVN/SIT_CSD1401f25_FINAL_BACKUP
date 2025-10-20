#include "hb.h"
#include "cprocessing.h"

struct hitbox createHitbox() {
	struct hitbox newHitbox;
	newHitbox.isActive = 0;
	newHitbox.body.pos = CP_Vector_Zero();
	newHitbox.body.area = CP_Vector_Zero();
	newHitbox.startuptime = 0.0f;
	newHitbox.activetime = 0.0f;
	newHitbox.recoverytime = 0.0f;
	newHitbox.totalTime = 0.0f;
	newHitbox.currenttime = 0.0f;
	newHitbox.color = white;

	return newHitbox;
}
void initializeHitbox(struct hitbox* targetHiB, struct Entity* attackerBod, CP_Vector area, float startuptime, float activetime, float recoverytime) {
	float attackRange = 20.f;

	targetHiB->startuptime = startuptime;
	targetHiB->activetime = activetime;
	targetHiB->recoverytime = recoverytime;
	targetHiB->totalTime = startuptime + activetime + recoverytime;

	//attach the hitbox to attackerbody //determine new position
	float x = CP_Input_GetMouseX() - attackerBod->pos.x;
	float y = CP_Input_GetMouseY() - attackerBod->pos.y;
	CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(x, y));
	float X = attackerBod->pos.x + direction.x * (attackerBod->diameter * 0.5f + attackRange);
	float Y = attackerBod->pos.y + direction.y * (attackerBod->diameter * 0.5f + attackRange);

	targetHiB->body.pos = CP_Vector_Set(X,Y);
	targetHiB->body.area = area;
	targetHiB->body.shapeType = rectangle;
	targetHiB->isActive = 1;
	targetHiB->state = Startup;

}

void updateHitbox(struct hitbox* targetHiB, CP_Vector newPos) {
	if (targetHiB->isActive) {
		if (targetHiB->state == Startup && targetHiB->currenttime > targetHiB->startuptime) {
			targetHiB->state = Active;
			
		}
		else if (targetHiB->state == Active && targetHiB->currenttime > targetHiB->startuptime + targetHiB->activetime) {
			targetHiB->state = Recovery;
		}
		else;

		targetHiB->currenttime += CP_System_GetDt();
		if (targetHiB->currenttime >= targetHiB->totalTime) { destroyHitbox(targetHiB); }
	}
	if (targetHiB->state == Startup) {
		targetHiB->color = yellow;
	}
	if (targetHiB->state == Active) {
		targetHiB->color = red;
	}
	if (targetHiB->state == Recovery) {
		targetHiB->color = blue;
	}
}

void destroyHitbox(struct hitbox* targetHiB) {
	targetHiB->isActive = 0;
	targetHiB->body.pos = CP_Vector_Zero();
	targetHiB->body.area = CP_Vector_Zero();
	targetHiB->startuptime = 0.0f;
	targetHiB->activetime = 0.0f;
	targetHiB->recoverytime = 0.0f;
	targetHiB->totalTime = 0.0f;
	targetHiB->currenttime = 0.0f;
	targetHiB->color = white;
}

//=====================================================

struct hurtbox createHurtbox() {
	struct hurtbox newHurtbox;
	newHurtbox.isActive = 0;
	newHurtbox.canBehit = 0;
	newHurtbox.body.pos = CP_Vector_Zero();
	newHurtbox.body.area = CP_Vector_Zero();
	newHurtbox.body.diameter = 0.0f;
	newHurtbox.state = UNINITIALIZED;
	newHurtbox.invul_time = 0.0f;
	newHurtbox.timeSinceHit = 0.0f;
	newHurtbox.color = white;

	return newHurtbox;
}
void initializeCircleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, float diameter,float invulTime) {
	targetHuB->isActive = 1;
	targetHuB->body.pos = pos;
	targetHuB->body.diameter = diameter;
	targetHuB->body.shapeType = circle;
	targetHuB->canBehit = 1;
	targetHuB->state = HUB_IDLE;
	targetHuB->invul_time = invulTime;
	targetHuB->timeSinceHit = 0.0f;
}
void initializeRectangleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, CP_Vector area, float invulTime) {
	targetHuB->isActive = 1;
	targetHuB->body.pos = pos;
	targetHuB->body.area = area;
	targetHuB->body.shapeType = rectangle;
	targetHuB->canBehit = 1;
	targetHuB->state = HUB_IDLE;
	targetHuB->invul_time = invulTime;
	targetHuB->timeSinceHit = 0.0f;
}
void updateHurtbox(struct hurtbox* targetHuB, CP_Vector newPos) {
	targetHuB->body.pos = newPos;
	destroyHurtbox(targetHuB);
	if (targetHuB->isActive && targetHuB->state == INVULNERABLE) {
		targetHuB->timeSinceHit += CP_System_GetDt();
		if (targetHuB->timeSinceHit >= targetHuB->invul_time) {
			targetHuB->canBehit = 1;
			targetHuB->timeSinceHit = 0.0f;
			targetHuB->state = HUB_IDLE;
		}
	}

	if (targetHuB->state == HUB_IDLE) {
		targetHuB->color = yellow;
	}
	if (targetHuB->state == INVULNERABLE) {
		targetHuB->color = green;
	}
}
void isHitHUB(struct hurtbox* targetHuB) {
	if (targetHuB->isActive) {
		if (targetHuB->state == HUB_IDLE) {
			targetHuB->state = INVULNERABLE;
			targetHuB->canBehit = 0;
			targetHuB->timeSinceHit = 0.0f;
		}
	}
}
void destroyHurtbox(struct hurtbox* targetHuB) {
	targetHuB->isActive = 0;
	targetHuB->body.pos = CP_Vector_Zero();
	targetHuB->body.area = CP_Vector_Zero();
	targetHuB->body.diameter = 0.0f;
	targetHuB->state = UNINITIALIZED;
	targetHuB->invul_time = 0.0f;
	targetHuB->canBehit = 0;
	targetHuB->timeSinceHit = 0.0f;
	targetHuB->color = white;
}