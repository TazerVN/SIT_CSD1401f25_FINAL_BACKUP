#include "hb.h"
#include "cprocessing.h"
#include "camera.h"
#include "audio.h"

struct hitbox createHitbox() {
	struct hitbox newHitbox;
	newHitbox.isActive = 0;
	newHitbox.isStatic = 0;
	newHitbox.attackRange = 20.0f;
	newHitbox.body.pos = CP_Vector_Zero();
	newHitbox.body.area = CP_Vector_Zero();
	newHitbox.startuptime = 0.0f;
	newHitbox.activetime = 0.0f;
	newHitbox.recoverytime = 0.0f;
	newHitbox.totalTime = 0.0f;
	newHitbox.currenttime = 0.0f;
	newHitbox.attackerBodyPreviousPos = CP_Vector_Zero();
	newHitbox.color = white;

	return newHitbox;
}
void initializeHitbox(struct hitbox* targetHiB, struct Entity* attackerBod, CP_Vector area, float startuptime, float activetime, float recoverytime,float force) {
	
	targetHiB->isStatic = 0;
	targetHiB->force = force;
	targetHiB->startuptime = startuptime;
	targetHiB->activetime = activetime;
	targetHiB->recoverytime = recoverytime;
	targetHiB->totalTime = startuptime + activetime + recoverytime;

	//attach the hitbox to attackerbody //determine new position
	float x = CP_Input_GetMouseX() - attackerBod->pos.x;
	float y = CP_Input_GetMouseY() - attackerBod->pos.y;
	CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(x, y));

	float X = attackerBod->pos.x + direction.x * (attackerBod->diameter * 0.5f + targetHiB->attackRange);
	float Y = attackerBod->pos.y + direction.y * (attackerBod->diameter * 0.5f + targetHiB->attackRange);

	targetHiB->body.pos = CP_Vector_Set(X,Y);
	targetHiB->body.area = area;
	targetHiB->body.shapeType = rectangle;
	targetHiB->isActive = 1;
	targetHiB->state = Startup;

	targetHiB->previousPos = CP_Vector_Set(X, Y);
	targetHiB->attackerBodyPreviousPos = attackerBod->pos;
}

void initializeStaticHitbox(struct hitbox* targetHiB, struct Entity* attackerBod,float force) {
	targetHiB->isStatic = 1;
	targetHiB->state = Active;
	targetHiB->force = force;
	targetHiB->body.pos = attackerBod->pos;
	targetHiB->body.diameter = attackerBod->diameter - 10.0f;
	targetHiB->body.shapeType = circle;
	targetHiB->isActive = 1;
}

void updateHitbox(struct hitbox* targetHiB, struct Entity* attackerBod) {
	//if not static do this
	if (targetHiB->isActive && !targetHiB->isStatic) {

		/*float newPosX = attackerBod->pos.x - targetHiB->attackerBodyPreviousPos.x;
		float newPosY = attackerBod->pos.y - targetHiB->attackerBodyPreviousPos.y;

		targetHiB->body.pos = CP_Vector_Set(targetHiB->previousPos.x + newPosX, targetHiB->previousPos.y + newPosY);

		targetHiB->previousPos = targetHiB->body.pos;
		targetHiB->attackerBodyPreviousPos = attackerBod->pos;*/

		//attach the hitbox to attackerbody //determine new position
		float x = CP_Input_GetMouseX() - attackerBod->posOS.x;
		float y = CP_Input_GetMouseY() - attackerBod->posOS.y;
		CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(x, y));
		float X = attackerBod->pos.x + direction.x * (attackerBod->diameter * 0.5f + targetHiB->attackRange);
		float Y = attackerBod->pos.y + direction.y * (attackerBod->diameter * 0.5f + targetHiB->attackRange);
		targetHiB->body.pos = CP_Vector_Set(X, Y);
		targetHiB->body.posOS = CP_Vector_Set(X + offset.x, Y + offset.y);

		if (targetHiB->state == Startup && targetHiB->currenttime == 0) {
			play_sound(0);
		}

		else if (targetHiB->state == Startup && targetHiB->currenttime > targetHiB->startuptime) {
			targetHiB->state = Active;
			play_sound(1);
			timer_reset(&shake_timer);
		}
		else if (targetHiB->state == Active && targetHiB->currenttime > targetHiB->startuptime + targetHiB->activetime) {
			targetHiB->state = Recovery;
		}
		else;

		targetHiB->currenttime += CP_System_GetDt();
		if (targetHiB->currenttime >= targetHiB->totalTime) { destroyHitbox(targetHiB,attackerBod); }
	}
	//If static do this
	else if (targetHiB->isActive && targetHiB->isStatic) {
		targetHiB->body.pos = attackerBod->pos;
	}

	// change color based on state
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
void destroyHitbox(struct hitbox* targetHiB,struct Entity* attackBod) {
	targetHiB->force = 0.0f;
	targetHiB->isActive = 0;
	targetHiB->isStatic = 0;
	targetHiB->body.pos = CP_Vector_Zero();
	targetHiB->body.area = CP_Vector_Zero(); 
	targetHiB->previousPos = CP_Vector_Zero();
	targetHiB->attackerBodyPreviousPos = CP_Vector_Zero();
	targetHiB->body.diameter = 0.0f;
	targetHiB->startuptime = 0.0f;
	targetHiB->activetime = 0.0f;
	targetHiB->recoverytime = 0.0f;
	targetHiB->totalTime = 0.0f;
	targetHiB->currenttime = 0.0f;
	targetHiB->color = white;
	attackBod->state = IDLE;
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
void initializeCircleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, float diameter,float maxHP,float invulTime) {
	targetHuB->isActive = 1;

	targetHuB->inimaxHP = maxHP;
	targetHuB->maxHP = maxHP;
	targetHuB->curHP = maxHP;

	targetHuB->body.pos = pos;
	targetHuB->body.diameter = diameter;
	targetHuB->body.shapeType = circle;
	targetHuB->canBehit = 1;
	targetHuB->state = HUB_IDLE;
	targetHuB->invul_time = invulTime;
	targetHuB->timeSinceHit = 0.0f;
}
void initializeRectangleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, CP_Vector area, float maxHP, float invulTime) {
	targetHuB->isActive = 1;

	targetHuB->inimaxHP = maxHP;
	targetHuB->maxHP = maxHP;
	targetHuB->curHP = maxHP;

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
	targetHuB->body.posOS.x = newPos.x + offset.x;
	targetHuB->body.posOS.y = newPos.y + offset.y;
	

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
void isHitHUB(struct hurtbox* targetHuB,struct Entity* victimBody,struct hitbox* attack) {
	if (targetHuB->isActive) {
		if (targetHuB->state == HUB_IDLE) {
			targetHuB->state = INVULNERABLE;
			targetHuB->canBehit = 0;
			targetHuB->timeSinceHit = 0.0f;
			--targetHuB->curHP;

			float forceX = (victimBody->pos.x - attack->body.pos.x);
			float forceY = (victimBody->pos.y - attack->body.pos.y);
			CP_Vector forceDirection = CP_Vector_Normalize(CP_Vector_Set(forceX, forceY));
			CP_Vector acceleratedForce = CP_Vector_Set(forceDirection.x * attack->force,forceDirection.y * attack->force);

			//apply force and add launched time;
			if (!victimBody->isStatic) {
				victimBody->acceleration = acceleratedForce;
				victimBody->state = LAUNCHED;
				victimBody->launchedTime = 0.001f;
			}
			
		}
		if (targetHuB->curHP <= 0) destroyHurtbox(targetHuB);
	}
}
void destroyHurtbox(struct hurtbox* targetHuB) {

	targetHuB->inimaxHP = 0.0f;
	targetHuB->maxHP = 0.0f;
	targetHuB->curHP = 0.0f;

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