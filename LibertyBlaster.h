#pragma once
#include "GGE2_1.h"


#define MainMenuUI masterUIList[0]
#define DONT_STATE_CHANGE -1

#define IN_MAIN_MENU 0
#define STARTING_GAME 1
#define IN_GAME 2
#define IN_SETTINGS 3

#define MAX_MOUSE_MOVEMENT (double)30
#define MOUSE_MOVEMENT_DAMPER (double)1500

#define ENEMY_HP_DEFAULT 5
#define ENEMY_DISTANCE 200//350
#define ENEMY_MAX_SPEED 1.0
#define ENEMY_START_SPEED 0.8//0.6
#define STARTING_SPAWN_SPEED 10.0
#define DEFAULT_ENEMY_MAX 50
#define ENEMY_POS_RANGE 2000//2500

#define OUR_ACCELERATION 0.0008 //How much holding a key adds to heading
#define OUR_MAX_SPEED 0.8 //max speed
#define FORWARD_BACK_MULT 2 //How much more powerful forward/back is in relation to the other controls

enum heading{Xhead, YHead, ZHead, Velocity};
//enum holdingKeys{wkey, akey, skey,dkey,qkey,ekey,vkey,ckey};
static int wkey = 0;
static int akey = 1;
static int skey = 2;
static int dkey = 3;
static int qkey = 4;
static int ekey = 5;
static int vkey = 6;
static int ckey = 7;


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


EnShip* enemyShipHandler(EnShip* enemyShipList, int upEnemyShips);
void resetShip(EnShip* enemyShip);
void resetShipVariation(EnShip* enemyShip);
void ourShipMotionHandler();

void expandedMouseClick(GLFWwindow* window, int button, int action, int mods);
void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

void gameCursorMovement();