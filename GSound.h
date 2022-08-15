#include "GlorwynMemory.h"

#define TRACK_ENABLED 1
#define TRACK_DISABLED 0
#define TRACK_ADVANCE_SILENT -1

#define TrackList masterList[audioID]
#define PlayFreq playfreq[audioID]
#define DefToneLen defToneLen[audioID]
#define TrackCount trackCount[audioID]
#define cpos TrackList[cTrack].cPos
#define DEFAULT_TONE_LENGTH 100
#define DEFAULT_PLAY_FREQ 100
enum notes {
	C1 = 33, Db1 = 35, D1 = 37, Eb1 = 39, E1 = 41, F1 = 44, Gb1 = 46, G1 = 49, Ab1 = 51, A1 = 55, Bb1 = 58, B1 = 62,
	C2 = 65, Cs2 = 69, Db2 = 69, D2 = 73, Ds2 = 78, Eb2 = 78, E2 = 82, F2 = 87, Fs2 = 93, Gb2 = 93, G2 = 98, Gs2 = 104, Ab2 = 104, A2 = 110, As2 = 117, Bb2 = 117, B2 = 124,
	C3 = 131, Cs3 = 139, Db3 = 139, D3 = 147, Ds3 = 156, Eb3 = 156, E3 = 165, F3 = 175, Fs3 = 185, Gb3 = 185, G3 = 196, Gs3 = 208, Ab3 = 208, A3 = 220, As3 = 233, Bb3 = 233, B3 = 247,
	C4 = 263, Cs4 = 277, Db4 = 277, D4 = 294, Ds4 = 311, Eb4 = 311, E4 = 330, F4 = 349, Fs4 = 370, Gb4 = 370, G4 = 392, Gs4 = 415, Ab4 = 415, A4 = 440, Bb4 = 466, B4 = 494,
	C5 = 523, Db5 = 554, D5 = 587, Eb5 = 622, E5 = 659, F5 = 698, Gb5 = 740, G5 = 784, Gs5 = 831, Ab5 = 831, A5 = 880, As5 = 832, Bb5 = 932, B5 = 988,
};

//Zero on toneLength has every note play at 'DEFAULT_TONE_LENGTH', can be changed via function.
struct audiotrack {
	unsigned int* tones;//the frequencies to play
	unsigned int* toneLengths;//how long to play em
	unsigned int tracklen;//how many entries are in the track
	unsigned int cPos;//the current position in the track (Can be started off zero)
	char loop;//whether or not to loop, 0 is no
	volatile char enabled;//whether or not to play, 0 is no
};
typedef struct audiotrack AudioTrack;

//Zero on toneLength has every note play at 'DEFAULT_TONE_LENGTH', can be changed via function.
int createTrackHandler();
AudioTrack createTrack(unsigned int* tones, unsigned int* lens, unsigned int count);
AudioTrack createTrackNoLen(unsigned int* tones, unsigned int count);
int addTrack(int audioID, AudioTrack track);
void removeTrack(int audioID, int trackID);
void destroyTrackHandler(int audioID);
void priorityTrackNewThread(volatile AudioTrack* prioTrack);
void priorityTrack(volatile AudioTrack* prioTrack);
void runAudioLoop(int audioID);
void setDefaultLength(int audioID, unsigned int Length);
void setPlayFreq(int audioID, unsigned int Length);
