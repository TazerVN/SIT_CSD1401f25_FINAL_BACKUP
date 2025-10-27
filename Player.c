#include "Player.h"
#include "cprocessing.h"
#include "camera.h"
#include "audio.h"

void initializePlayer(struct Player* player, enum EntityShape shape, CP_Vector pos,float maxSpd, float speedincrease,float drag,float maxHP) {
	float diameter = 50.0f;
	float padding = 0.0f;	//hurtbox padding for actual body
	float playerVelocityClamp = 3.0f;

	player->attackCD = 1.0f;
	player->curattackCD = 0.0f;
	player->invulTime = 2.0f;
	player->canAttack = 1;

	player->isAlive = 1;
	initializeCircle(&player->PlayerBod,pos,diameter,maxSpd,speedincrease,drag,playerVelocityClamp);

	player->hitbox = createHitbox();	//create hitbox not initialized yet

	player->hurtbox = createHurtbox();	//create hurtbox not initialized yet

	switch (shape)	//initialize hurtbox based on shape
	{
		case circle:
		{
			initializeCircleHurtbox(&player->hurtbox, player->PlayerBod.pos, player->PlayerBod.diameter - padding ,maxHP,player->invulTime);
			break;
		}
		case rectangle:
		{
			float x = player->PlayerBod.area.x - padding;
			float y = player->PlayerBod.area.y - padding;
			initializeRectangleHurtbox(&player->hurtbox, player->PlayerBod.pos, CP_Vector_Set(x, y),maxHP, player->invulTime);
			break;
		}
		default:
			break;
	}
}

void destroyPlayer(struct Player* player) {
	player->invulTime = 0.0f;
	player->attackCD = 0.0f;
	player->curattackCD = 0.0f;
	player->canAttack = 0;
	destroyHurtbox(&player->hurtbox);
	destroyEntity(&player->PlayerBod);
}

int updatePlayer(struct Player* player) {

	if (player->isAlive) {
		if (!player->hurtbox.isActive) {
			player->isAlive = 0;
			destroyPlayer(player);
		}

		if (!player->canAttack) {
			if (player->curattackCD >= player->attackCD) {
				player->canAttack = 1;
				player->curattackCD = 0.0f;
			}
			else {
				player->curattackCD += CP_System_GetDt();
			}
		}

		float dx = 0.0f;
		float dy = 0.0f;
		if ((CP_Input_KeyDown(KEY_W))) { dy--; }
		if ((CP_Input_KeyDown(KEY_S))) { dy++; }
		if ((CP_Input_KeyDown(KEY_A))) { dx--; }
		if ((CP_Input_KeyDown(KEY_D))) { dx++; }

		CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(dx, dy));
		if (dx != 0 || dy != 0) {
			if(player->PlayerBod.state != ATTACKING) {
			player->PlayerBod.state = MOVING;
			}
			move(&player->PlayerBod, direction);
		}
		if (player->PlayerBod.state != ATTACKING && (dx == 0 && dy == 0)) player->PlayerBod.state = IDLE;

		//update body
		updateEntity(&player->PlayerBod);
		//update hurtbox
		updateHurtbox(&player->hurtbox, player->PlayerBod.pos);

		//check attack
		if (player->canAttack) {
			attack(player);
		}
		//after attack finishes
		updateHitbox(&player->hitbox, &player->PlayerBod);

		return player->isAlive;
	}
}

void renderPlayer(struct Player* player){
	if (player->isAlive) {
		float playerX = player->PlayerBod.posOS.x;
		float playerY = player->PlayerBod.posOS.y;
		switch (player->PlayerBod.shapeType)
		{
		case circle:
		{
			CP_Settings_Fill(CP_Color_CreateHex(white));
			CP_Graphics_DrawCircle(playerX, playerY, player->PlayerBod.diameter);

			//draw a line the indicate the direction to mouse from player
			if (player->PlayerBod.state != ATTACKING) {
				float x = CP_Input_GetMouseX() - playerX;
				float y = CP_Input_GetMouseY() - playerY;
				CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(x, y));
				float lineEndPointX = playerX + direction.x * player->PlayerBod.diameter * 0.5f;
				float lineEndPointY = playerY + direction.y * player->PlayerBod.diameter * 0.5f;
				CP_Settings_Fill(CP_Color_CreateHex(black));
				CP_Graphics_DrawLine(playerX, playerY, lineEndPointX, lineEndPointY);
			}

			break;
		}
		case rectangle:
		{
			CP_Settings_Fill(CP_Color_CreateHex(white));
			CP_Graphics_DrawRect(playerX, playerY, player->PlayerBod.area.x, player->PlayerBod.area.y);
			break;
		}
		default:
			break;
		}

		if (player->hitbox.isActive) {
			CP_Settings_Fill(CP_Color_CreateHex(player->hitbox.color));
			CP_Graphics_DrawRect(player->hitbox.body.posOS.x, player->hitbox.body.posOS.y, player->hitbox.body.area.x, player->hitbox.body.area.y);
		}
	}
}

void attack(struct Player* player) {
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
		player->canAttack = 0;
		light_attack(&player->hitbox, &player->PlayerBod);
	}
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_2)) {
		player->canAttack = 0;
		heavy_attack(&player->hitbox, &player->PlayerBod);
	}
}
void heavy_attack(struct hitbox* AtkHitbox,struct Entity* attackerBod) {
	attackerBod->state = ATTACKING;
	initializeHitbox(AtkHitbox, attackerBod, CP_Vector_Set(20, 20), 0.5f, 0.3f, 0.2f,1000.0f);
}
void light_attack(struct hitbox* AtkHitbox, struct Entity* attackerBod) {
	attackerBod->state = ATTACKING;
	initializeHitbox(AtkHitbox, attackerBod, CP_Vector_Set(20, 20), 0.3, 0.3f, 0.1f,500.0f);
}