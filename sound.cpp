#include <stdio.h>
#include <map>
#include <string>

#include <SDL.h>
#include <SDL_mixer.h>

#include "sound.h"

typedef std::map<SoundEffects,Mix_Chunk *> SoundMap;
Mix_Music *song;

Sound sound;

SoundMap soundMap;

const char *sfxPath[]={
	"data/Sounds/none.wav",

    "data/Sounds/door-open.wav",
    "data/Sounds/door-opening-a.wav",
    "data/Sounds/door-closing-a.wav",

    "data/Sounds/sliding-door-open.wav",
    "data/Sounds/sliding-door-close.wav",

    "data/Sounds/voice-open-the-door.wav",

    "data/Sounds/switch-click.wav",
    "data/Sounds/up-pressure-plate.wav",
    "data/Sounds/down-pressure-plate.wav",

    "data/Sounds/wall-explode.wav",
    "data/Sounds/big-explosion.wav",

    "data/Sounds/quiet-fail.wav",
    "data/Sounds/bing-low.wav",
    "data/Sounds/big-fail.wav",

    "data/Sounds/sad-trombone.wav",


   "data/Sounds/error-sound.wav",

   "data/Sounds/win-sound.wav",

};

Sound::Sound()
{

}

Sound::~Sound()
{
}

void Sound::loadAll()
{
	song=Mix_LoadMUS("data/cave themeb4.ogg");
	if(song) {
		printf("Starting background music\n");
		Mix_PlayMusic(song,-1);
	}
	// need to load the sound effects here.
	for(int i=0;i<SFX_HIGH_ERROR;i++) {
		FILE *file=fopen(sfxPath[i],"r");
		if(file) {
			fclose(file);
			Mix_Chunk *chunk=Mix_LoadWAV(sfxPath[i]);
			if(chunk) {
				soundMap[(SoundEffects)i]=chunk;
			} else {
				printf("File '%s' invalid\n",sfxPath[i]);
			}
		} else {
			printf("File '%s' not found.\n",sfxPath[i]);
		}
	}

}

int Sound::playOnce(SoundEffects sfx)
{
	Mix_Chunk *chunk=NULL;

	if(soundMap.find(sfx)!=soundMap.end()) {
		chunk=soundMap[sfx];
	}
	if(chunk==NULL) return -1;
	Mix_PlayChannel(-1,chunk,0);
	return 0;
}

