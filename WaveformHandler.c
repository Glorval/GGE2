#define _CRT_SECURE_NO_WARNINGS
#include "WaveformHandler.h"
#include <string.h>
#include <stdio.h>

static volatile int FireSound = -1;
static volatile int ManyHitsSound = -1;
static volatile int EnemyDiedSound = -1;
static volatile int MainMenuTheme = -1;
static volatile int EndTheme = -1;

struct audioFiles {
    char* name;
    Mix_Chunk* waveData;
};

struct audioFiles* AudioFiles;

void glorLoadSDL(){
    int audio_rate;
    Uint16 audio_format;
    int audio_channels;
    int loops = 0;
    int i;
    int reverse_stereo = 0;
    int reverse_sample = 0;

    audio_rate = MIX_DEFAULT_FREQUENCY;//MIX_DEFAULT_FREQUENCY
    audio_format = 0;
    audio_channels = 1;


    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        return(255);
    }

    Mix_OpenAudio(audio_rate, audio_format, audio_channels, 4096);
    Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
    SDL_Log("Opened audio at %d Hz %d bit%s %s", audio_rate,
        (audio_format & 0xFF),
        (SDL_AUDIO_ISFLOAT(audio_format) ? " (float)" : ""),
        (audio_channels > 2) ? "surround" :
        (audio_channels > 1) ? "stereo" : "mono");

    gatherWaveforms();
}

void gatherWaveforms() {
    AudioFiles = NULL;
    system("dir *.wav /b >audioList.txt");//WINDOWS CALL
    FILE* audioList = fopen("audioList.txt", "r");
    char line[100] = { 0 };
    int current = 0;
    int newlineStrip = 0;

ReadFile:;
    AudioFiles = realloc(AudioFiles, sizeof(struct audioFiles) * (current + 1));
    if (fgets(line, 99, audioList) == NULL) {
        return;
    }
    
    //strip the newline off the end
    while (line[newlineStrip] != '\n') {
        if (line[newlineStrip] == 0) {
            goto NewlineRemoved;
        }
        newlineStrip++;
    }
    line[newlineStrip] = 0;
NewlineRemoved:;

    //check to see if it's one of the special files
    if (strcmp(line, "Fire.wav") == 0) {
        FireSound = current;
    }else if (strcmp(line, "ManyHits.wav") == 0) {
        ManyHitsSound = current;
    } else if (strcmp(line, "EnemyDied.wav") == 0) {
        EnemyDiedSound = current;
    } else if (strcmp(line, "MainMenu.wav") == 0) {
        MainMenuTheme = current;
    } else if (strcmp(line, "Endtheme.wav") == 0) {
        EndTheme = current;
    }

    AudioFiles[current].waveData = Mix_LoadWAV(line);
    //AudioFiles[current].name = calloc;
    AudioFiles[current].name = 0;

    current++;
    goto ReadFile;

}


void playWav(int ID) {
    /* Play and then exit */
    Mix_PlayChannel(-1, AudioFiles[ID].waveData, 0);

    while (Mix_Playing(0)) {
        SDL_Delay(1);
    }
}


void fireAndForgetPlayWav(int ID) {

}


