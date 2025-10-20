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
	int isActive;
	int color;
	enum hitboxState state;
	float totalTime, startuptime, activetime, recoverytime, currenttime;
};

struct hitbox createHitbox();
void initializeHitbox(struct hitbox* targetHiB, struct Entity* attackerBod, CP_Vector area, float startuptime, float activetime, float recoverytime);
void updateHitbox(struct hitbox* targetHiB, CP_Vector newPos);
void destroyHitbox(struct hitbox* targetHiB);

//===============================================================================================================================================

//==========================================================Hurtbox==============================================================================
enum hurtboxState { UNINITIALIZED, HUB_IDLE , INVULNERABLE };

typedef struct hurtbox {
	struct Entity body;
	int isActive, canBehit;
	int color;
	enum hurtboxState state;
	float invul_time, timeSinceHit;
	enum hurtboxShape shape;
};

struct hurtbox createHurtbox();
void initializeCircleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, float diameter, float invulTime);
void initializeRectangleHurtbox(struct hurtbox* targetHuB, CP_Vector pos, CP_Vector area, float invulTime);

void updateHurtbox(struct hurtbox* targetHuB,CP_Vector newPos);
void isHitHUB(struct hurtbox* targetHuB);
void destroyHurtbox(struct hurtbox* targetHuB);
#endif // !HB_HEADER
