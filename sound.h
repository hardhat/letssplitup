#ifndef SOUND_H
#define SOUND_H

enum SoundEffects {
	SFX_NONE,

	SFX_OPEN_WOOD_DOOR,
	SFX_FAST_OPEN_WOOD_DOOR,
	SFX_CLOSE_WOOD_DOOR,

	SFX_OPEN_SLIDING_DOOR,
	SFX_CLOSE_SLIDING_DOOR,

	SFX_CREEPY_VOICE_HINT,

	SFX_SWITCH,
	SFX_DOWN_PRESSURE_PLATE,
	SFX_UP_PRESSURE_PLATE,

	SFX_WALL_FALL,
	SFX_TUNNEL_EXPLODE,

	SFX_QUIET_WARNING,
	SFX_LOW_ERROR,
	SFX_HIGH_ERROR,

	SFX_GAME_OVER,

	SFX_SYSTEM_ERROR,

	SFX_GAME_WIN,
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
