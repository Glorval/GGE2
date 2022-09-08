#include "WaveformHandler.h"

void GlorLoadSDL(){
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
}


void playWav(char* name) {
    Mix_Chunk* wave = NULL;
    wave = Mix_LoadWAV(name);
    if (wave == NULL) {
        SDL_Log("Couldn't load %s: %s\n", name, SDL_GetError());
    }

    /* Play and then exit */
    Mix_PlayChannel(0, wave, 0);

    while (Mix_Playing(0)) {

        SDL_Delay(1);

    }
}