#ifndef MAIN_H
#define MAIN_H

enum SfxType {
	S_START,
	S_JUMP,
	S_MATCH,
	MAXSFX
};
extern enum GameMode { MODE_TITLE, MODE_MENU, MODE_GAME, MODE_WINNER } gameMode;

void playSound( SfxType id);

#endif
