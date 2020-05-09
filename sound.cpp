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
	"data/none.wav",
    "data/quest.wav",
	"data/welcome.wav",

	"data/ugh1.wav",
	"data/ugh2.wav",
	"data/ugh3.wav",

	"data/hah.wav",

	"data/firebreath.wav",
	"data/zap.wav",
	"data/missed.wav",

	"data/flames.wav",

	"data/die.wav",

	"data/win.wav",
	"data/lose.wav",
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
	for(int i=0;i<SFX_MAX;i++) {
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

