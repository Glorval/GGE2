#pragma once
#include "GGE2_1.h"

#define MainMenuUI masterUIList[0]
#define DONT_STATE_CHANGE -1

#define IN_MAIN_MENU 0
#define IN_GAME 1

struct referObj {
	int ID;
	int indlen;
	char* objName;	
};
typedef struct referObj RefObj;

volatile static RefObj* masterObjList;
volatile static unsigned int masterObjLenght;
volatile World gameworld;

World* loadGame();
void setupMainMenu();

int getsetGamestate(int flag);
void runGame(GLFWwindow* window, int flagSetting);

long long int startGameButton(void* ourself, long long int data, short int clickData);


