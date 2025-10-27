#include "cprocessing.h"

typedef struct SoundSetting {
	float volume;
	float pitch;
	CP_BOOL looping;
	CP_SOUND_GROUP group;
} SoundSetting;

typedef struct AudioSetting {
	struct SoundSetting sfx;
	struct SoundSetting ambience;
	struct SoundSetting music;
}AudioSetting;

extern AudioSetting au;

void play_music();
void play_sound(int index);
void init_audio();
void exit_music();
void free_audio();
void audio_engine();