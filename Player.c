#include "Player.h"
#include "cprocessing.h"

void initializePlayer(struct Player* player, enum EntityShape shape, CP_Vector pos,float maxSpd, float speedincrease) {
	float diameter = 50.0f;
	float padding = 7.0f;	//hurtbox padding for actual body

	player->maxSpeed = maxSpd;
	player->speedincrease = speedincrease;
	player->invulTime = 2.0f;

	player->isAlive = 1;
	initializeCircle(&player->PlayerBod,pos,diameter,player->maxSpeed,player->speedincrease);

	player->hitbox = createHitbox();	//create hitbox not initialized yet

	player->hurtbox = createHurtbox();	//create hurtbox not initialized yet

	switch (shape)	//initialize hurtbox based on shape
	{
		case circle:
		{
			initializeCircleHurtbox(&player->hurtbox, player->PlayerBod.pos, player->PlayerBod.diameter - padding, player->invulTime);
			break;
		}
		case rectangle:
		{
			float x = player->PlayerBod.area.x - padding;
			float y = player->PlayerBod.area.y - padding;
			initializeRectangleHurtbox(&player->hurtbox, player->PlayerBod.pos, CP_Vector_Set(x, y), player->invulTime);
			break;
		}
		default:
			break;
	}
}

int updatePlayer(struct Player* player) {
	float dx = 0.0f;
	float dy = 0.0f;
	if (CP_Input_KeyDown(KEY_W)) { 
		dy--; }
	if (CP_Input_KeyDown(KEY_S)) { dy++; }
	if (CP_Input_KeyDown(KEY_A)) { dx--; }
	if (CP_Input_KeyDown(KEY_D)) { dx++; }

	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
		if (CP_Input_MouseDown(KEY_LEFT_SHIFT)) {
			heavy_attack(&player->hitbox, &player->PlayerBod);
		}
		else {
			light_attack(&player->hitbox, &player->PlayerBod);
		}
	}

	CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(dx, dy));
	//update body
	applyAcceleration(&player->PlayerBod, direction);
	updateEntity(&player->PlayerBod);
	//update hurtbox
	updateHurtbox(&player->hurtbox, player->PlayerBod.pos);
	updateHitbox(&player->hitbox,player->PlayerBod.pos);

	return player->isAlive;
}

void renderPlayer(struct Player* player){
	if (player->isAlive) {
		switch (player->PlayerBod.shapeType)
		{
		case circle:
		{
			float playerX = player->PlayerBod.pos.x;
			float playerY = player->PlayerBod.pos.y;

			CP_Settings_Fill(CP_Color_CreateHex(white));
			CP_Graphics_DrawCircle(playerX, playerY, player->PlayerBod.diameter);

			//draw a line the indicate the direction to mouse from player
			float x = CP_Input_GetMouseX() - player->PlayerBod.pos.x;
			float y = CP_Input_GetMouseY() - player->PlayerBod.pos.y;
			CP_Vector direction = CP_Vector_Normalize(CP_Vector_Set(x,y));
			float lineEndPointX = playerX + direction.x * player->PlayerBod.diameter * 0.5f;
			float lineEndPointY = playerY + direction.y * player->PlayerBod.diameter * 0.5f;

			CP_Settings_Fill(CP_Color_CreateHex(black));
			CP_Graphics_DrawLine(playerX, playerY,lineEndPointX,lineEndPointY);

			break;
		}
		case rectangle:
		{
			CP_Settings_Fill(CP_Color_CreateHex(white));
			CP_Graphics_DrawRect(player->PlayerBod.pos.x, player->PlayerBod.pos.y, player->PlayerBod.area.x, player->PlayerBod.area.y);
			break;
		}
		default:
			break;
		}

		if (player->hitbox.isActive) {
			CP_Settings_Fill(CP_Color_CreateHex(player->hitbox.color));
			CP_Graphics_DrawRect(player->hitbox.body.pos.x, player->hitbox.body.pos.y, player->hitbox.body.area.x, player->hitbox.body.area.y);
		}
	}
}

void heavy_attack(struct hitbox* AtkHitbox,struct Entity* attackerBod) {
	initializeHitbox(AtkHitbox, attackerBod, CP_Vector_Set(50, 50), 0.5f, 0.3f, 0.2f);
}
void light_attack(struct hitbox* AtkHitbox, struct Entity* attackerBod) {
	initializeHitbox(AtkHitbox, attackerBod, CP_Vector_Set(50, 50), 0.3, 0.3f, 0.1f);
}