#include "util.h"
#include "Entity.h"
#include "hb.h"
#include "Player.h"
#include "World.h"

struct Player pl1;
struct hurtbox hurtboxArr[5];

CP_Font font;
int isDebug = 0;

void render();
void checkHBCollision(struct hitbox* hib, struct hurtbox* hub);

void world_init() {
	font = CP_Font_Load("Assets/PixelAE_Regular.ttf");
	
	//when creating player look at this
	/*PlayerAttackHitbox = createHitbox();*/
	initializePlayer(&pl1, circle, CP_Vector_Set(100.0f, 100.0f),300.0f,200.0f);
	
	//test case for hurtboxes
	for (int i = 0; i < sizeof(hurtboxArr) / sizeof(hurtboxArr[0]); ++i) {
		float padding = 100.0f;
		struct hurtbox temp = createHurtbox();
		float x = CP_Random_RangeFloat(0.0f + padding,CP_System_GetWindowWidth() - padding);
		float y = CP_Random_RangeFloat(0.0f + padding, CP_System_GetWindowHeight() - padding);
		hurtboxArr[i] = temp;
		initializeCircleHurtbox(&hurtboxArr[i], CP_Vector_Set(x,y), 50.0f,1.0f);
	}
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
	
	//look here when u code player
	updatePlayer(&pl1);

	//check collision for hurtboxes
	for (int i = 0; i < sizeof(hurtboxArr) / sizeof(hurtboxArr[0]); ++i) {
		checkHBCollision(&pl1.hitbox, &hurtboxArr[i]);
		updateHurtbox(&hurtboxArr[i],hurtboxArr[i].body.pos);
	}
	//render
	render();
}
void world_exit() {
	CP_Font_Free(font);
}

//=====================================================

void render() {
	//render active hurtbox
	for (int i = 0; i < sizeof(hurtboxArr) / sizeof(hurtboxArr[0]); ++i) {
		if (hurtboxArr[i].isActive) {
			switch (hurtboxArr[i].body.shapeType)
			{
			case circle: {
				CP_Settings_Fill(CP_Color_CreateHex(hurtboxArr[i].color));
				CP_Graphics_DrawCircle(hurtboxArr[i].body.pos.x, hurtboxArr[i].body.pos.y, hurtboxArr[i].body.diameter);
			}
			case rectangle: {
				CP_Settings_Fill(CP_Color_CreateHex(hurtboxArr[i].color));
				CP_Graphics_DrawRect(hurtboxArr[i].body.pos.x, hurtboxArr[i].body.pos.y, hurtboxArr[i].body.area.x, hurtboxArr[i].body.area.y);
			}
			default:
				break;
			}
		}
	}

	//player always on top and rendering player
	if (pl1.isAlive) {
		renderPlayer(&pl1);
	}

	if (isDebug) {
		CP_Settings_Fill(CP_Color_CreateHex(pl1.hurtbox.color));
		CP_Graphics_DrawCircle(pl1.hurtbox.body.pos.x, pl1.hurtbox.body.pos.y, pl1.hurtbox.body.diameter);

		char buffer[50] = { 0 };
		sprintf_s(buffer, 50, "Current HitboxTime: %.2f", pl1.hitbox.currenttime);
		CP_Settings_Fill(CP_Color_CreateHex(black));
		CP_Settings_TextSize(50);
		CP_Font_Set(font);
		CP_Font_DrawTextBox(buffer, 30, 30, 300);
	}
}

void checkHBCollision(struct hitbox* hib, struct hurtbox* hub) {
	if (hib->isActive && hub->isActive) {
		int intersect = (interSectRectToCircle(hib->body.pos.x, hib->body.pos.y, hib->body.area.x, hib->body.area.y, hub->body.pos.x, hub->body.pos.y, hub->body.diameter));
		//if hitbox is in active frame and hurtbox can be hit
		if (intersect&&(hib->state == Active) && (hub->canBehit)) {
			isHitHUB(hub);
		}
	}
}