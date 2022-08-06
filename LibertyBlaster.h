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

#define BOOLETSPEED (float)4
#define BOOLETLIFE 150

#define ENEMY_HP_DEFAULT 2
#define ENEMY_DISTANCE 50//500
#define ENEMY_MAX_SPEED 1.0
#define ENEMY_START_SPEED 0.0//0.8
#define STARTING_SPAWN_SPEED 5.0
#define DEFAULT_ENEMY_MAX 5
#define ENEMY_POS_RANGE 200//2500

#define OUR_ACCELERATION 0.005 //0.0008 //How much holding a key adds to heading
#define OUR_MAX_SPEED (float)0.4 //max speed
#define FORWARD_BACK_MULT 2 //How much more powerful forward/back is in relation to the other controls
#define OUR_FIRE_RATE 2 //is twice as fast due to the alternating guns
#define OUR_BOOLET_OFFSET .10 //how far to each side boolet spawn

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
static int fireKey = 8;


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

    float forward[4];
};
typedef struct enShip EnShip;

struct ourShip {
    int numHitboxes;
    float** hitboxes;
    float heading[4]; //x,y,z, velocity vector of where we are going
    char keysHolding[9]; //'toggled' controls on which keys are being pressed. w,a,s,d,q,e,c,v, fire guns,
    short int hp;
    int timeSinceLastFire;
    int fireRate;
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
void voidShip(EnShip* enemyShip);
void setShip(EnShip* enemyShip);
void resetShipVariation(EnShip* enemyShip);
void ourShipHandler();
void ourShipMotionHandler();

void expandedMouseClick(GLFWwindow* window, int button, int action, int mods);
void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

void gameCursorMovement();



void facePoint(EnShip* us, float targetPos[3]);