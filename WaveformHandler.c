#define _CRT_SECURE_NO_WARNINGS
#include "WaveformHandler.h"
#include <string.h>
#include <stdio.h>





int getSetSongCount(int SC) {
    static int SongCount = 0;
    if (SC >= 0) {
        SongCount = SC;
    }
    return(SongCount);
}

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
    MusicFiles = NULL;
    char* test = "dir GameMusic\\*.wav /b >GameMusic/audioList.txt";
    system(test);//WINDOWS CALL
    FILE* audioList = fopen("GameMusic\\audioList.txt", "r");
    char line[100] = { 0 };
    int newlineStrip = 0;

    char base[] = "GameMusic/";

    char filePath[200] = { 0 };


ReadFile:;
    MusicFiles = realloc(MusicFiles, sizeof(struct musicFiles) * (getSetSongCount(-1) + 1));
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
    newlineStrip = 0;
NewlineRemoved:;
    filePath[0] = 0;
    strcat(filePath, base);
    strcat(filePath, line);

    //check to see if it's one of the special files, if not, increment song count
    if (strcmp(line, "Fire.wav") == 0) {
        KeySounds.Fire = KeySounds.SoundCount;
        KeySounds.sfxData = realloc(KeySounds.sfxData, (KeySounds.SoundCount + 1)* sizeof(Mix_Chunk*));
        KeySounds.sfxData[KeySounds.Fire] = Mix_LoadWAV(filePath);
        KeySounds.SoundCount++;
        goto ReadFile;//skip the song addition because this isnt a song
    }else if (strcmp(line, "ManyHits.wav") == 0) {
        KeySounds.ManyHits = KeySounds.SoundCount;
        KeySounds.sfxData = realloc(KeySounds.sfxData, (KeySounds.SoundCount + 1) * sizeof(Mix_Chunk*));
        KeySounds.sfxData[KeySounds.ManyHits] = Mix_LoadWAV(filePath);
        KeySounds.SoundCount++;
        goto ReadFile;//skip the song addition because this isnt a song
    } else if (strcmp(line, "EnemyDied.wav") == 0) {
        KeySounds.EnemyDied = KeySounds.SoundCount;
        KeySounds.sfxData = realloc(KeySounds.sfxData, (KeySounds.SoundCount + 1) * sizeof(Mix_Chunk*));
        KeySounds.sfxData[KeySounds.EnemyDied] = Mix_LoadWAV(filePath);
        KeySounds.SoundCount++;
        goto ReadFile;//skip the song addition because this isnt a song
    } else if (strcmp(line, "MainTheme.wav") == 0) {
        KeySounds.MainMenuTheme = getSetSongCount(-1);
        printf("main menu should be %d, is %d\n", getSetSongCount(-1), KeySounds.MainMenuTheme);
    } else if (strcmp(line, "EndTheme.wav") == 0) {
        KeySounds.EndTheme = getSetSongCount(-1);
                
    }

    //add the music file to the 'directory' of songs to load/play later
    MusicFiles[getSetSongCount(-1)].name = calloc(strlen(filePath), sizeof(char));
    strcpy(MusicFiles[getSetSongCount(-1)].name, filePath);
    getSetSongCount(getSetSongCount(-1) + 1);
    

    goto ReadFile;

}


void playWav(Mix_Chunk* waveData) {
    /* Play and then exit */
    if (waveData != NULL) {
        SDL_ClearError();
        int successValue = Mix_PlayChannel(-1, waveData, 0);        
        if (successValue == -1) {
            char* error = SDL_GetError();
            printf("SDL Error %s\n", error);
        }
    }    
}

Mix_Chunk* playWavName(char* fileName) {
    /* Play and then exit */
    Mix_Chunk* data = Mix_LoadWAV(fileName);
    Mix_PlayChannel(0, data, 0);
    return(data);
}

