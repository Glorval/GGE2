#pragma once
#include "GGE2_1.h"

#define MainMenuUI masterUIList[0]
#define DONT_STATE_CHANGE -1

#define IN_MAIN_MENU 0
#define IN_GAME 1

#define MAX_MOUSE_MOVEMENT (double)20
#define MOUSE_MOVEMENT_DAMPER (double)1000

enum heading{Xhead, YHead, ZHead, Velocity};
enum holdingKeys{wkey, akey, skey,dkey,qkey,ekey,vkey,ckey};

struct referObj {
	int ID;
	int indlen;
	char* objName;	
};
typedef struct referObj RefObj;

struct enShip {
    unsigned int ID, VBO, EBO;
    int indexCount;

    float position[7]; //x,y,z, quaternion orientation
    float scale;

    float heading[4]; //x,y,z, velocity vector of where we are going
    short int hp;
    char targeting;
    int numHitboxes;
    float** hitboxes;
};
typedef struct enShip EnShip;

struct ourShip {
    int numHitboxes;
    float** hitboxes;
    float heading[4]; //x,y,z, velocity vector of where we are going
    char keysHolding[8]; //'toggled' controls on which keys are being pressed. w,a,s,d,q,e,c,v
    short int hp;
};
//typedef struct ourShip OurShip;




volatile static RefObj* masterObjList;
volatile static unsigned int masterObjLenght;
volatile World gameworld;
volatile GLFWwindow* gamewindow;







World* loadGame();
void setupMainMenu();

int getsetGamestate(int flag);
void runGame(GLFWwindow* window, int flagSetting);

long long int startGameButton(void* ourself, long long int data, short int clickData);



void ourShipMotionHandler();

void expandedMouseClick(GLFWwindow* window, int button, int action, int mods);
void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

void gameCursorMovement();