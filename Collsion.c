#include "Collision.h"

void checkEntityCollision(struct Entity* entity1,struct Entity* entity2) {
	int intersect = 0;
	intersect = interSectCircleToCircle(entity1->pos.x, entity1->pos.y, entity1->diameter, entity2->pos.x, entity2->pos.y, entity2->diameter);
	if (intersect) {
		CollisionResolveCTC(entity1,entity2);
	}
}

void checkHBCollision(struct hitbox* hib, struct Entity* victimBody, struct hurtbox* hub) {
	int intersect = 0;
	switch (hib->body.shapeType)
	{
	case rectangle: {
		if (hib->isActive && hub->isActive) {
			intersect = (interSectRectToCircle(hib->body.pos.x, hib->body.pos.y, hib->body.area.x, hib->body.area.y, hub->body.pos.x, hub->body.pos.y, hub->body.diameter));
			//if hitbox is in active frame and hurtbox can be hit
		}
		break;
	}
	case circle: {
		if (hib->isActive && hub->isActive) {
			intersect = interSectCircleToCircle(hib->body.pos.x, hib->body.pos.y, hib->body.diameter, hub->body.pos.x, hub->body.pos.y, hub->body.diameter);
			//if hitbox is in active frame and hurtbox can be hit
		}
		break;
	}
	default:
		break;
	}
	if ((hib->state == Active) && (hub->canBehit) && intersect) {
		isHitHUB(hub, victimBody, hib);
	}
}

int CollisionResolveCTC(struct Entity* entity1, struct Entity* entity2) {
	if (entity1->isActive && entity2->isActive) {
		CP_Vector currentAccel = entity2->acceleration;
		CP_Vector currentVelocity = entity2->velocity;

		entity2->acceleration = CP_Vector_Zero();
		entity2->velocity = CP_Vector_Zero();

		float r1 = entity1->diameter * 0.5f;
		float r2 = entity2->diameter * 0.5f;
		float radii = r1 + r2;

		float dx = entity2->pos.x - entity1->pos.x;
		float dy = entity2->pos.y - entity1->pos.y;

		CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(dx, dy));
		float distance = CP_Vector_Distance(entity1->pos,entity2->pos);

		float depth = radii - distance;
		float overlap = depth * 0.5 + 0.9f;

		entity1->pos.x -= overlap * direction.x;	//displace position opposite to the target object 
		entity1->pos.y -= overlap * direction.y;

		entity2->pos.x += overlap * direction.x;	//displace position opposite to the target object 
		entity2->pos.y += overlap * direction.y;

		entity2->acceleration.x = -currentAccel.x * 0.2f;
		entity2->acceleration.y = -currentAccel.y * 0.2f;
		entity2->velocity.x = -currentVelocity.x * 0.2f;
		entity2->velocity.y = -currentVelocity.y * 0.2f;

		entity1->acceleration.x = currentAccel.x * 0.8f;
		entity1->acceleration.y = currentAccel.y * 0.8f;
		entity1->velocity.x = currentVelocity.x * 0.8f;
		entity1->velocity.y = currentVelocity.y * 0.8f;
	}
}