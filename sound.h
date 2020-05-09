#ifndef SOUND_H
#define SOUND_H

enum SoundEffects {
	SFX_NONE,
	SFX_QUEST,
	SFX_WELCOME,

	SFX_UGH1,
	SFX_UGH2,
	SFX_UGH3,

	SFX_HAH,

	SFX_FIREBREATH,
	SFX_ZAP,
	SFX_MISSED,

	SFX_FLAMES,

	SFX_DIE,	// specifically the hero.

	SFX_WIN,
	SFX_LOSE,

	SFX_MAX
};

class Sound {
public:
	Sound();
	~Sound();
	void loadAll();
	int playOnce(SoundEffects sfx);
};

extern Sound sound;

#endif
