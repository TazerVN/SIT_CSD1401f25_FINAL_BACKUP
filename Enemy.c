#include "Enemy.h"
#include "cprocessing.h"
#include "camera.h"

void initializeEnemy(struct Enemy* enemy, enum EntityShape shape, CP_Vector pos, float maxSpd, float speedincrease,float drag,float maxHP) {

	float diameter = 50.0f;
	float padding = 0.0f;	//hurtbox padding for actual body
	float enemyVelocityClamp = 3.0f;

	enemy->invulTime = 1.0f;

	enemy->isAlive = 1;
	initializeCircle(&enemy->Bod, pos,diameter, maxSpd, speedincrease,drag,enemyVelocityClamp);

	enemy->hitbox = createHitbox();	//create hitbox not initialized yet

	initializeStaticHitbox(&enemy->hitbox,&enemy->Bod,10.0f);

	enemy->hurtbox = createHurtbox();	//create hurtbox not initialized yet

	switch (shape)	//initialize hurtbox based on shape
	{
	case circle:
	{
		initializeCircleHurtbox(&enemy->hurtbox, enemy->Bod.pos, enemy->Bod.diameter - padding,maxHP, enemy->invulTime);
		break;
	}
	case rectangle:
	{
		float x = enemy->Bod.area.x - padding;
		float y = enemy->Bod.area.y - padding;
		initializeRectangleHurtbox(&enemy->hurtbox, enemy->Bod.pos, CP_Vector_Set(x, y),maxHP, enemy->invulTime);
		break;
	}
	default:
		break;
	}
}
int updateEnemy(struct Enemy* enemy){

	if (!enemy->hurtbox.isActive) { 
		enemy->isAlive = 0; 
		destroyEnemy(enemy);
	}

	updateEntity(&enemy->Bod);
	//update hurtbox
	updateHurtbox(&enemy->hurtbox, enemy->Bod.pos);
	updateHitbox(&enemy->hitbox, &enemy->Bod);

	return enemy->isAlive;
}
void renderEnemy(struct Enemy* enemy) {
	if (enemy->isAlive) {
		float enemyX = enemy->Bod.posOS.x;
		float enemyY = enemy->Bod.posOS.y;
		switch (enemy->Bod.shapeType)
		{
		case circle:
		{
			CP_Settings_Fill(CP_Color_CreateHex(blue));
			CP_Graphics_DrawCircle(enemyX, enemyY, enemy->Bod.diameter);
			break;
		}
		case rectangle:
		{
			CP_Settings_Fill(CP_Color_CreateHex(blue));
			CP_Graphics_DrawRect(enemyX, enemyY, enemy->Bod.area.x, enemy->Bod.area.y);
			break;
		}
		default:
			break;
		}
	}
}

void destroyEnemy(struct Enemy* enemy) {
	destroyHitbox(&enemy->hitbox,&enemy->Bod);
	destroyHurtbox(&enemy->hurtbox);
	destroyEntity(&enemy->Bod);
}