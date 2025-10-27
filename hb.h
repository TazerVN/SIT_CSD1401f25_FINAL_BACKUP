#pragma once
#ifndef HB_HEADER
#define HB_HEADER

#include "util.h"
#include "cprocessing.h"
#include "Entity.h"

//==========================================================Hitbox===============================================================================
enum hitboxState { Startup, Active, Recovery };

typedef struct hitbox {
	struct Entity body;
	int isActive, isStatic;
	float force;
	int color;
	enum hitboxState state;
	float totalTime, startuptime, activetime, recoverytime, currenttime;
	CP_Vector attackerBodyPreviousPos, previousPos;
	float attackRange;
};

struct hitbox createHitbox();
void initializeHitbox(struct hitbox* targetHiB, struct Entity* attackerBod, CP_Vector area, float startuptime, float activetime, float recoverytime,float force);
void initializeStaticHitbox(struct hitbox* targetHiB, struct Entity* attackerBod,float force);
void updateHitbox(struct hitbox* targetHiB, struct Entity* attackerBod);
void destroyHitbox(struct hitbox* targetHiB, struct Entity* attackerBod);

//===============================================================================================================================================

//==========================================================Hurtbox==============================================================================
enum hurtboxState { UNINITIALIZED, HUB_IDLE , INVULNERABLE };

typedef struct hurtbox {
	float inimaxHP, maxHP, curHP;
	struct Entity body;
	int isActive, canBehit;
	int color;
	enum hurtboxState state;
	float invul_time, timeSinceHit;
	enum hurtboxShape shape;
};

struct hurtbox createHurtbox();
void initializeCircleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, float diameter, float maxHP, float invulTime);
void initializeRectangleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, CP_Vector area, float maxHP, float invulTime);

void updateHurtbox(struct hurtbox* targetHuB,CP_Vector newPos);
void isHitHUB(struct hurtbox* targetHuB,struct Entity* victimBody, struct hitbox* attack);
void destroyHurtbox(struct hurtbox* targetHuB);
#endif // !HB_HEADER
