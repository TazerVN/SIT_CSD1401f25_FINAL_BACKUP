#include "cprocessing.h"
#include <math.h>
#include "audio.h"
/*CP_SOUND_GROUP_1 - Music
* CP_SOUND_GROUP_2 - SFX
* CP_SOUND_GROUP_3 - Ambience

*/

CP_Sound SFX[10];
CP_Sound music;
float lerping;

AudioSetting au;


void load_music() {
	music = CP_Sound_Load("Assets/distill.wav");
}

void load_sfx() {
	SFX[0] = CP_Sound_Load("Assets/sfx/bat_swing.wav");
	SFX[1] = CP_Sound_Load("Assets/sfx/bat_hit.wav");
	SFX[2] = CP_Sound_Load("Assets/sfx/body_hit.wav");
}

void init_audio() {
	load_music();
	load_sfx();
	au.music.volume = 0.1f;
	au.music.pitch = 1.0f;
	au.music.looping = TRUE;
	au.music.group = CP_SOUND_GROUP_1;

	au.sfx.volume = 0.3f;
	au.sfx.pitch = 1.0f;
	au.sfx.looping = FALSE;
	au.sfx.group = CP_SOUND_GROUP_2;

	au.ambience.volume = 0.5f;
	au.ambience.pitch = 1.0f;
	au.ambience.looping = TRUE;
	au.ambience.group = CP_SOUND_GROUP_3;

	lerping = 1.0f;
}





void play_music() {
	CP_Sound_PlayAdvanced(music, au.music.volume, au.music.pitch, au.music.looping, au.music.group);
}

void play_sound(int index) {
	CP_Sound_PlayAdvanced(SFX[index], au.sfx.volume, au.sfx.pitch, au.sfx.looping, au.sfx.group);
}

void free_audio() {
	CP_Sound_Free(music);
}

void exit_music() {
	lerping -= (1 - lerping) * (1 - exp(-CP_System_GetDt() * 7));
	if (lerping < 0) {
		lerping = 0;
	}
	float current_volume = CP_Math_LerpFloat(0, au.music.volume, lerping);
	CP_Sound_SetGroupVolume(au.music.group, current_volume);
	
	if (lerping == 0) {
		free_audio();
	}
}

void audio_engine() {
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_1, au.music.volume);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_2, au.sfx.volume);
	CP_Sound_SetGroupVolume(CP_SOUND_GROUP_3, au.ambience.volume);
}
