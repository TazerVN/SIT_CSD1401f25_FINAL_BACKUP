#pragma once
#ifndef ENEMY_HEADER
#define ENEMY_HEADER

#include "Entity.h"
#include "hb.h"
#include "cprocessing.h"

typedef struct Enemy {
	int isAlive;
	int invulTime;
	struct Entity Bod;
	struct hitbox hitbox;
	struct hurtbox hurtbox;
};

void initializeEnemy(struct Enemy* enemy, enum EntityShape shape, CP_Vector pos, float maxSpd, float speedincrease,float drag,float maxHP);
int updateEnemy(struct Enemy* enemy);
void renderEnemy(struct Enemy* enemy);
void destroyEnemy(struct Enemy* enemy);

#endif // !ENEMY_HEADER
