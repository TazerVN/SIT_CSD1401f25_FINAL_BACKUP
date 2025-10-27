#include "util.h"
#include "Collision.h"
#include "Entity.h"
#include "hb.h"
#include "Player.h"
#include "Enemy.h"
#include "World.h"
#include "camera.h"

struct Player pl1;
struct Enemy enemyArr[15];
int enArrsize = sizeof(enemyArr) / sizeof(enemyArr[0]);

CP_Font font;
int isDebug = 0;

float p_maxSpeed = 200.0f, p_speedIncrease = 200.0f, p_drag = 3.0f, p_HP = 3.0f;
float e_maxSpeed = 200.0f, e_speedIncrease = 200.0f, e_drag = 0.8f, e_HP = 2.0f;

void render();
void respawn();
//void spawnEnemy(struct Enemy* enemyarray, int arraySize);
void spawnEnemy();

void world_init() {
	font = CP_Font_Load("Assets/PixelAE_Regular.ttf");
	
	CP_Settings_Stroke(CP_Color_Create(0, 0, 0, 255));
	camera_init();
	//when creating player look at this
	initializePlayer(&pl1, circle, CP_Vector_Set(100.0f, 100.0f),p_maxSpeed,p_speedIncrease,p_drag,p_HP);
	
	//initialize Enemy
	for (int i = 0; i < enArrsize; ++i) {
		float padding = 100.0f;
		float x = CP_Random_RangeFloat(0.0f + padding, CP_System_GetWindowWidth() - padding);
		float y = CP_Random_RangeFloat(0.0f + padding, CP_System_GetWindowHeight() - padding);
		initializeEnemy(&enemyArr[i],circle,CP_Vector_Set(x,y),e_maxSpeed,e_speedIncrease,e_drag,e_HP);
	}
	//initializeEnemy(&enemy, circle, CP_Vector_Set(800, 450), 200.0f, 200.0f, 2.0f);
}
void world_update() {
	CP_Graphics_ClearBackground(CP_Color_CreateHex(white));
	//user inputs
	if (CP_Input_KeyTriggered(KEY_F1)) {
		isDebug = !isDebug;
	}
	if (CP_Input_KeyDown(KEY_Q)) {
		CP_Engine_Terminate();
	}
	if (CP_Input_KeyDown(KEY_EQUAL)) {
		respawn();
	}
	if (CP_Input_KeyDown(KEY_BACKSLASH)) {
		//spawnEnemy(&enemyArr,enArrsize);
		spawnEnemy();
	}
	if (CP_Input_KeyDown(KEY_M)) {
		timer_reset(&shake_timer);
	}
	//look here when u code player
	updatePlayer(&pl1);

	//check collision for hitboxes and hurtboxes
	for (int i = 0; i < enArrsize; ++i) {
		checkHBCollision(&pl1.hitbox,&enemyArr[i].Bod, &enemyArr[i].hurtbox);	//check if player hits enemies
		checkHBCollision(&enemyArr[i].hitbox, &pl1.PlayerBod , &pl1.hurtbox);
		for (int j = 0; j < enArrsize; ++j) {
			if (enemyArr[i].isAlive && enemyArr[j].isAlive && &enemyArr[i] != &enemyArr[j]) {
				checkEntityCollision(&enemyArr[i].Bod, &enemyArr[j].Bod);
			}
		}
		checkEntityCollision(&enemyArr[i].hitbox ,&pl1.PlayerBod);
		updateEnemy(&enemyArr[i]);
	}

	//checkHBCollision(&pl1.hitbox, &enemy.Bod, &enemy.hurtbox);	//check if player hits enemies
	//checkHBCollision(&enemy.hitbox, &pl1.PlayerBod , &pl1.hurtbox);
	//updateEnemy(&enemy);

	//render
	render();
}
void world_exit() {
	CP_Font_Free(font);
}

//=====================================================

void render() {

	camera_update(pl1.PlayerBod.pos.x, pl1.PlayerBod.pos.y);
	//render alive enemies
	for (int i = 0; i < enArrsize; ++i) {
		if (enemyArr[i].isAlive) {
			renderEnemy(&enemyArr[i]);
		}
		if (isDebug) {
			CP_Settings_Fill(CP_Color_CreateHex(enemyArr[i].hurtbox.color));
			CP_Graphics_DrawCircle(enemyArr[i].hurtbox.body.posOS.x, enemyArr[i].hurtbox.body.posOS.y, enemyArr[i].hurtbox.body.diameter);

			CP_Settings_Fill(CP_Color_CreateHex(enemyArr[i].hitbox.color));
			CP_Graphics_DrawCircle(enemyArr[i].hitbox.body.posOS.x, enemyArr[i].hitbox.body.posOS.y, enemyArr[i].hitbox.body.diameter);
		}
	}

	/*if (enemy.isAlive) {
		renderEnemy(&enemy);
	}
	if (isDebug) {
		CP_Settings_Fill(CP_Color_CreateHex(enemy.hurtbox.color));
		CP_Graphics_DrawCircle(enemy.hurtbox.body.pos.x, enemy.hurtbox.body.pos.y, enemy.hurtbox.body.diameter);

		CP_Settings_Fill(CP_Color_CreateHex(enemy.hitbox.color));
		CP_Graphics_DrawCircle(enemy.hitbox.body.pos.x, enemy.hitbox.body.pos.y, enemy.hitbox.body.diameter);
	}*/

	//player always on top and rendering player
	if (pl1.isAlive) {
		renderPlayer(&pl1);
	}

	if (isDebug) {
		CP_Settings_Fill(CP_Color_CreateHex(pl1.hurtbox.color));
		CP_Graphics_DrawCircle(pl1.hurtbox.body.posOS.x, pl1.hurtbox.body.posOS.y, pl1.hurtbox.body.diameter);

		char buffer[50] = { 0 };
		sprintf_s(buffer, 50, "Current HitboxTime: %.2f", pl1.hitbox.currenttime);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(buffer, 30, 30, 300);

		char dt[50] = { 0 };
		sprintf_s(dt, 50, "Current DT: %.4f", CP_System_GetDt());
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(dt, 30, 50, 300);

		char playerState[50] = { 0 };
		sprintf_s(playerState, 50, "Player State: %d", pl1.PlayerBod.state);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playerState, 30, 70, 300);

		char playerHP[50] = { 0 };
		sprintf_s(playerHP, 50, "Player HP: %d", (int)pl1.hurtbox.curHP);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playerHP, 30, 90, 300);

		char playerforce[50] = { 0 };
		sprintf_s(playerforce, 50, "Player Force: %.1f,%.1f", pl1.PlayerBod.applied_force.x, pl1.PlayerBod.applied_force.y);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playerforce, 30, 110, 300);

		char playerAcceleration[50] = { 0 };
		sprintf_s(playerAcceleration, 50, "Player Acceleration: %.1f,%.1f", pl1.PlayerBod.acceleration.x, pl1.PlayerBod.acceleration.y);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playerAcceleration, 30, 130, 300);

		char playervelo[50] = { 0 };
		sprintf_s(playervelo, 50, "Player Velocity: %.1f,%.1f", pl1.PlayerBod.velocity.x, pl1.PlayerBod.velocity.y);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playervelo, 30, 150, 300);

		char playerCanAttack[50] = { 0 };
		sprintf_s(playerCanAttack, 50, "Player CanAttack: %.d, Player AttackCD: %.2f", pl1.canAttack,pl1.curattackCD);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(20);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(playerCanAttack, 30, 170, 300);

		for (int i = 0; i < enArrsize; ++i) {
			char enemyStats[150] = { 0 };

			sprintf_s(enemyStats, 150, "Enemy %d: Velocity %.1f,%.1f, Acceleration %.1f,%.1f IA: %d HP: %d Enemy Body Active : %d", i ,enemyArr[i].Bod.velocity.x, enemyArr[i].Bod.velocity.y,
				enemyArr[i].Bod.acceleration.x, enemyArr[i].Bod.acceleration.y,enemyArr[i].isAlive,(int)enemyArr[i].hurtbox.curHP,enemyArr[i].Bod.isActive);
			CP_Settings_Fill(CP_Color_CreateHex(black));
			CP_Settings_TextSize(20);
			CP_Font_Set(font);
			CP_Font_DrawTextBox(enemyStats, 1000, (i * 20)+100 , 600);
		}
	}
}

void respawn() {
	if (!pl1.isAlive) initializePlayer(&pl1, circle, CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()), p_maxSpeed, p_speedIncrease, p_drag, p_HP);
}

void spawnEnemy() {
	for (int i = 0; i < enArrsize; ++i) {

		float padding = 100.0f;
		float x = CP_Random_RangeFloat(0.0f + padding, CP_System_GetWindowWidth() - padding);
		float y = CP_Random_RangeFloat(0.0f + padding, CP_System_GetWindowHeight() - padding);

		if (!enemyArr[i].isAlive) {
			//initializeEnemy(&enemyArr[i], circle, CP_Vector_Set(CP_Input_GetMouseX(), CP_Input_GetMouseY()), 200.0f, 200.0f, 2.0f);
			initializeEnemy(&enemyArr[i], circle, CP_Vector_Set(x, y), e_maxSpeed, e_speedIncrease, e_drag, e_HP);
			break;
		}
	}
}

void update_position() {

}