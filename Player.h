#pragma once
#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "Entity.h"
#include "hb.h"
#include "cprocessing.h"

typedef struct Player {
	int isAlive;
	int invulTime;
	float maxSpeed, speedincrease;
	struct Entity PlayerBod;
	struct hitbox hitbox;
	struct hurtbox hurtbox;
};

void initializePlayer(struct Player* player, enum EntityShape shape, CP_Vector pos,float maxSpd,float speedincrease);
int updatePlayer(struct Player* player);
void renderPlayer(struct Player* player);

//player moves
void heavy_attack(struct hitbox* AtkHitbox,struct Entity* attackerBod);
void light_attack(struct hitbox* AtkHitbox, struct Entity* attackerBod);

#endif // !PLAYER_HEADER
