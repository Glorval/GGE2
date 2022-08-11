#include "GSound.h"
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <process.h>

volatile AudioTrack** masterList = NULL;
volatile unsigned int* killSelf = NULL;
volatile unsigned int* trackCount = NULL;
volatile unsigned int* playfreq = NULL;
volatile unsigned int* defToneLen = NULL;
volatile unsigned int listCount = 0;
//volatile int IDToUpdate = -1;

void firstinit() {
	masterList = calloc(1, sizeof(AudioTrack*));
	masterList[0] = NULL;
	trackCount = calloc(1, sizeof(unsigned int));
	playfreq = calloc(1, sizeof(unsigned int));
	defToneLen = calloc(1, sizeof(unsigned int));
	killSelf = calloc(1, sizeof(uintptr_t));
}

int createTrackHandler() {
	printf("\n\nCreating handler:\nList count: %d\n", listCount + 1);
	if (masterList == NULL) {
		printf("Doing FirstInit");
		firstinit();
	} else {
		masterList = realloc(masterList, sizeof(AudioTrack*) * (listCount + 1));
		masterList[listCount] = NULL;
		trackCount = realloc(trackCount, sizeof(unsigned int) * listCount + 1);
		trackCount[listCount] = 0;
		playfreq = realloc(playfreq, sizeof(unsigned int) * listCount + 1);
		playfreq[listCount] = DEFAULT_PLAY_FREQ;
		defToneLen = realloc(defToneLen, sizeof(unsigned int) * listCount + 1);
		defToneLen[listCount] = DEFAULT_TONE_LENGTH;
		killSelf = realloc(killSelf, sizeof(uintptr_t) * listCount + 1);
		killSelf[listCount] = 0;
	}	
	printf("\n\n");
	 _beginthread(runAudioLoop, 0, listCount);
	listCount++;	
	return(listCount - 1);
}

void destroyTrackHandler(int audioID) {
	free(masterList[audioID]);
	trackCount[audioID] = 0;
	killSelf[audioID] = 1;
}

int addTrack(int audioID, AudioTrack track) {
	TrackList = realloc(TrackList, sizeof(AudioTrack) * (TrackCount+ 1));
	TrackList[TrackCount] = track;
	TrackCount++;
	return(TrackCount - 1);
}

void removeTrack(int audioID, int trackID) {
	free(masterList[audioID][trackID].tones);
	free(masterList[audioID][trackID].toneLengths);
	masterList[audioID][trackID].cPos = 0;
	masterList[audioID][trackID].loop = 0;
	masterList[audioID][trackID].tracklen = -1;
}

void priorityTrackRunner(volatile AudioTrack* prioTrack) {
	int* trackStatuses = calloc(1, sizeof(int));
	int totalStatuses = 0;
	int curStatus = 0;
	for (int cList = 0; cList < listCount; cList++) {
		totalStatuses += trackCount[cList];
		trackStatuses = realloc(trackStatuses, totalStatuses * sizeof(int));
		for (int cTrack = 0; cTrack < trackCount[cList]; cTrack++) {
			if (masterList[cList][cTrack].enabled != TRACK_DISABLED) {
				trackStatuses[curStatus] = masterList[cList][cTrack].enabled;
				curStatus++;
				masterList[cList][cTrack].enabled = TRACK_ADVANCE_SILENT;
			} else {
				trackStatuses[curStatus] = masterList[cList][cTrack].enabled;
				curStatus++;
			}
		}
	}

	while (prioTrack->cPos < prioTrack->tracklen) {
		if (prioTrack->toneLengths == NULL || prioTrack->toneLengths[prioTrack->cPos] == 0) {
			Beep(prioTrack->tones[prioTrack->cPos], DEFAULT_TONE_LENGTH);
		} else {
			Beep(prioTrack->tones[prioTrack->cPos], prioTrack->toneLengths[prioTrack->cPos]);
		}
		prioTrack->cPos++;
	}

	curStatus = 0;
	for (int cList = 0; cList < listCount; cList++) {
		for (int cTrack = 0; cTrack < trackCount[cList]; cTrack++) {
			masterList[cList][cTrack].enabled = trackStatuses[curStatus];
		}
	}
}


void priorityTrackNewThread(volatile AudioTrack* prioTrack) {
	_beginthread(priorityTrackRunner, 0, prioTrack);
	return;
}

void priorityTrack(volatile AudioTrack* prioTrack) {
	priorityTrackRunner(prioTrack);
}


void runAudioLoop(int audioID) {
	unsigned int toneLen = 0;
	printf("Beginning audio loop at ID %d\n", audioID);
start:;
	if (killSelf[audioID] == 1) {
		printf("Killing self");
		return;
	}

	printf("\nWe startin, ID %d. TrackCount of %d\n", audioID, TrackCount);
	for (int cTrack = 0; cTrack < TrackCount; cTrack++) {
		printf("%d   ", trackCount[audioID]);
		if(TrackList[cTrack].enabled == TRACK_ENABLED) {
			printf("IN enabled\n");
			//check the current position in track against our length,
			//if we're too far we gotta handle it
			if (cpos >= TrackList[cTrack].tracklen) {
				if (TrackList[cTrack].loop != 0) {//loop enabled
					TrackList[cTrack].cPos = 0;//set pos to zero, we loopin
					goto playingSound;//since we still need to play, go to the play segment
				}
				TrackList[cTrack].enabled = 0;
				continue;//to the next track
			}
	
		playingSound:;
			//if there is no tone length specified, play default tone length. 
			if (TrackList[cTrack].toneLengths == NULL || TrackList[cTrack].toneLengths[cpos] == 0) {
				printf("Trying to beep for %d at %dhz.\n", DefToneLen, TrackList[cTrack].tones[cpos]);
				Beep(TrackList[cTrack].toneLengths == NULL || TrackList[cTrack].tones[cpos], DefToneLen);
				fflush(stdout);
				fflush(stdin);
				fflush(stderr);
			} else {//Otherwise, play on our tone len
				printf("Trying to beep for %d at %dhz.\n", TrackList[cTrack].toneLengths[cpos], TrackList[cTrack].tones[cpos]);
				Beep(TrackList[cTrack].tones[cpos], TrackList[cTrack].toneLengths[cpos]);
				fflush(stdout);//hopes and prayers is what this is here for making it play better
				fflush(stdin);
				fflush(stderr);
			}
			TrackList[cTrack].cPos++;
		} else if (TrackList[cTrack].enabled == TRACK_ADVANCE_SILENT) {
			//figuring out whether or not 
			if (TrackList[cTrack].toneLengths == NULL || TrackList[cTrack].toneLengths[cpos] == 0) {
				printf("Playing silently for %d at %dhz.\n", DefToneLen, TrackList[cTrack].tones[cpos]);
				Sleep(DefToneLen);
			} else {
				printf("Playing silently for %d at %dhz.\n", TrackList[cTrack].toneLengths[cpos], TrackList[cTrack].tones[cpos]);
				Sleep(TrackList[cTrack].toneLengths[cpos]);
			}
			TrackList[cTrack].cPos++;
			if (cpos >= TrackList[cTrack].tracklen) {
				if (TrackList[cTrack].loop != 0) {//loop enabled
					TrackList[cTrack].cPos = 0;//set pos to zero, we loopin
				}
				TrackList[cTrack].enabled = 0;
				continue;//to the next track
			}
		} else {
			printf("Track disabled");
		}


	}//end of looping through tracks

	if (PlayFreq != 0) {
		printf("sleeping\n");
		Sleep(PlayFreq);
	}	
	goto start;
}

void setDefaultLength(int audioID, unsigned int Length){
	DefToneLen = Length;
}

void setPlayFreq(int audioID, unsigned int Length) {
	PlayFreq = Length;
}