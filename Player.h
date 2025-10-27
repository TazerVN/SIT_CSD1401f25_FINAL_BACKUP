#pragma once
#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#include "Entity.h"
#include "hb.h"
#include "cprocessing.h"

typedef struct Player {
	int isAlive;
	float invulTime;
	int canAttack;
	float attackCD, curattackCD;
	struct Entity PlayerBod;
	struct hitbox hitbox;
	struct hurtbox hurtbox;

};

void initializePlayer(struct Player* player, enum EntityShape shape, CP_Vector pos,float maxSpd,float speedincrease,float drag,float maxHP);
int updatePlayer(struct Player* player);
void renderPlayer(struct Player* player);
void destroyPlayer(struct Player* player);
void attack(struct Player* player);

//player moves
void heavy_attack(struct hitbox* AtkHitbox,struct Entity* attackerBod);
void light_attack(struct hitbox* AtkHitbox, struct Entity* attackerBod);

#endif // !PLAYER_HEADER
