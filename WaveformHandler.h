#pragma once

#include "SDL.h"
#include "SDL_mixer.h"


void GlorLoadSDL();

//stays in function until waveform is done playing
void playWav(char* name);