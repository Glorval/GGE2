#pragma once

#include "SDL.h"
#include "SDL_mixer.h"

struct keySounds {
	int Fire, ManyHits, EnemyDied, ButtonPress, MainMenuTheme, EndTheme;
	int SoundCount;
	Mix_Chunk** sfxData;
};


struct keySounds KeySounds;

struct musicFiles {
	char* name;
};
struct musicFiles* MusicFiles;
//static int SongCount = 0;

int getSetSongCount(int SC);

void glorLoadSDL();

void gatherWaveforms();

//stays in function until waveform is done playing
void playWav(Mix_Chunk* waveData);

//returns the mix chunk for you to free
Mix_Chunk* playWavName(char* fileName);
