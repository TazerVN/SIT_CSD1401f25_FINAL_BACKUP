#pragma once
#ifndef COLLISION_HEADER
#define COLLISION_HEADER

#include "util.h"
#include "Entity.h"
#include "hb.h"
#include <math.h>

void checkEntityCollision(struct Entity* entity1, struct Entity* entity2);
void checkHBCollision(struct hitbox* hib, struct Entity* victimBody, struct hurtbox* hub);
int CollisionResolveCTC(struct Entity* entity1,struct Entity* entity2);

#endif // !COLLISION_HEADER
