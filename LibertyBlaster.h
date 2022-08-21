//#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#include "GGE2_1.h"
#include "GSound.h"

//Defines, enums/struct setup, master variables, function decs
#define UICount 7 //update whenever adding another base UI layer
#define MainMenuUI masterUIList[0]
#define SettingsUI masterUIList[1]
#define BaseGameUI masterUIList[2]
#define CrosshairUI masterUIList[3]
#define DynamicGameUI masterUIList[4]
#define EndscreenUI masterUIList[5]
#define PauseScreenUI masterUIList[6]
static int CrosshairWingsID = 0;
static int CrosshairBigDotID = 0;
static int CrosshairSmallDotID = 0;

#define DONT_STATE_CHANGE -1

//Game state flags
#define IN_MAIN_MENU 0
#define STARTING_GAME 1
#define IN_GAME 2
#define BETWEEN_WAVES 3
#define END_SCREEN 4
#define IN_SETTINGS 5
#define PAUSE_MENU 6

#define CLEAR_GAME 10


//Game defines
#define MAX_MOUSE_MOVEMENT (double)30
#define MOUSE_MOVEMENT_DAMPER (double)1500

#define BOOLETSPEED (float)3
#define BOOLETLIFE 125
#define COMMS_UPDATE_DELAY (long int)8
static float  BASE_ARMOUR_REPAIR = 400; //How much armour will be repaired if no enemies pass
static float BASE_HULL_REPAIR = 100; //how much hull will repair if no enemies pass
static float PASSED_ENEMY_WORTH_ARMOUR = 3;//how much each passing enemy effect hull repair
static float PASSED_ENEMY_WORTH_HULL = 2;//how much each passing enemy effects hull repair
static int passedEnemies = 0;
static unsigned int score = 0;
#define SCORE_FOR_KILL 10
#define SCORE_LOSS_FOR_PASS 2

//Audio Settings
#define FULL_RETRO 0
#define FULL_RETRO_COMPAT 1
#define MODERN 2
#define NO_AUDIO 3
#define DEFAULT_AUDIO_SETTING MODERN

//GENERAL ENEMY ATTRIBUTES
static int ENEMY_DISTANCE = 500;//500
static int SPAWN_RATE = 20;//how many frames between spawns
#define MAX_ENEMY_COUNT 80
static int ENEMY_POS_RANGE = 2500;//2500
static int ENEMIES_PER_WAVE = 5;
//#define SPAWNING_TOLERANCE 10//how many enemies 

//SPECIFIC ENEMY ATTRIBUTES
static float ENEMY_MAX_SPEED = 1.5; //Goes to this speed on strafing runs
static float ENEMY_START_SPEED = 0.9;//0.8
static int ENEMY_HP = 2;
static float ENEMY_AGILITY = (float)10.0; //base of 2 for perfect agility, don't go lower
static int ENEMY_TARGET_DIST_MIN = 1600; //(SQUARED!!!!) How close before an enemy is forced to pull out
static int ENEMY_TARGET_DIST = 100; //how close an enemy can be on the z axis before they won't begin targeting
static int ENEMY_TARGET_DIST_MAX = 300; //how away far an enemy can be on the z axis when they won't begin targeting
static float ENEMY_TARGET_CHANCE = (float)0.25;//percent chance that a non-targeting ship will target
static int ENEMY_FIRE_RATE = 8; //is fource as fast due to the alternating guns
static float ENEMY_BOOLET_OFFSET = .15; //how far to each side boolet spawn
static int ENEMY_BURST_LEN = 16; //how many frames to be firing//was 
static float ENEMY_FIRE_CHANCE = (float)3.0;//Percent chance to begin firing in a frame if not already
static float ENEMY_SHIP_DEVIATION = (float)0.0015; //modifier to effect the perfect aiming

//PLAYER SHIP ATTRIBUTES
#define OUR_ACCELERATION 0.005 //0.0008 //How much holding a key adds to heading
#define OUR_MAX_SPEED (float)0.4 //max speed
#define MS_DISP_MULT 500 //Max speed * this = what gets displayed as the highest speed attainable.
#define FORWARD_BACK_MULT 2 //How much more powerful forward/back is in relation to the other controls
#define OUR_FIRE_RATE 8 //is twice as fast due to the alternating guns
#define OUR_BOOLET_OFFSET .10 //how far to each side boolet spawn
#define MAX_SHIELDS 100
#define SHIELD_RECHARGE_START 120//frames between being hit and shields being able to be recharged
#define SHIELD_RECHARGE_RATE (float)0.25 //shield hp per frame to heal
//#define SHIELD_RECHARGE_RATE_ABOVE_ONE //flag for another place
#define MAX_ARMOUR 400
#define MAX_HULL 200
#define SHIELD_DIVIDER 1//for display
#define ARMOUR_DIVIDER 4//for display
#define HULL_DIVIDER 2//for display
#define FRAMES_TO_COUNT_HITS_FOR_AUDIO 6//How many frames to track for seeing if we need to play an audio queue for being blasted a bunch
#define FTHS FRAMES_TO_COUNT_HITS_FOR_AUDIO
#define HITS_TO_PLAY_HITS 5

//Bullet specific stuff
#define EXTRA_BOOLET_TOLERANCE 1.5 //How much extra distance + the speed should be added to the hit sphere
#define BOOLET_LENIANCY 3 //inverse, higher numbers = lower leniancy. Shouldn't really be one, since then it'd just be checking the frame before & after against collision
#define PLAYER_BOOLET_TOLERANCE 3
#define PLAYER_BOOLET 0
#define ENEMY_BOOLET -1


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

    float speed;
    float forward[4];
    float up[4];
    float right[4];

    short int hp;
    short int currentlyDed;
    short int fireFrame;
    char targeting;    

    int worldID;
};
typedef struct enShip EnShip;

struct ourShip {
    float heading[4]; //x,y,z, velocity vector of where we are going
    char keysHolding[9]; //'toggled' controls on which keys are being pressed. w,a,s,d,q,e,c,v, fire guns,
    short int shields;
    short int armour;
    short int hp;
    int timeSinceLastFire;
    int fireRate;
    long long int shieldChargeTimeTracker;
    float subShieldHP;//for use if the shield recharge is below one
    short int lastHits[FTHS];
};
//typedef struct ourShip OurShip;


volatile static RefObj* masterObjList;
volatile static unsigned int masterObjLenght;
volatile static World gameworld;
volatile GLFWwindow* gamewindow;

static int AudioSetting;


World* loadGame();
void setupSettingsUI();
void setupMasterUIList();
void setupMainMenu();
void setupPauseMenu();
void setupGameUI();
void setupCrosshairUI();

int getsetGamestate(int flag);
void runGame(GLFWwindow* window, int flagSetting);
void setupDynamicUI();
void updateDynamicUI();
void setupEndscreen();
void removeEndscreen();

//button actions
long long int startGameButton(void* ourself, long long int data, short int clickData);
long long int settingsButton(void* ourself, long long int data, short int clickData);
long long int fullscreenButton(void* ourself, long long int data, short int clickData);
long long int returnToMenuButton(void* ourself, long long int data, short int clickData);
long long int exitGameButton(void* ourself, long long int data, short int clickData);
long long int dotSettingsButton(void* ourself, long long int data, short int clickData);
long long int wingsSettingButton(void* ourself, long long int data, short int clickData);

EnShip* enemyShipHandler(EnShip* enemyShipList, int upEnemyShips);
void updateShipLifeStatus(EnShip* enemyShip);
void killShip(EnShip* enemyShip);
void liveShip(EnShip* enemyShip);
void voidShip(EnShip* enemyShip);
void passedShip(EnShip* enemyShip);
void setShip(EnShip* enemyShip);
void respawnShip(EnShip* enemyShip);
void ourShipHandler();
void ourShipMotionHandler();

void expandedMouseClick(GLFWwindow* window, int button, int action, int mods);
void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods);

void gameCursorMovement();


void attemptToResetGameVariables();


void debugCommands();



//Audio handler dec's
void setAudioFunctions(int audioSetting);

static unsigned long long playingBulletSound = 0;
void (*realBulletAudioHandler)(int);
void silentBulletHandler(int newFrameUpdate);
void bulletAudioHandler(int newFrameUpdate);
void retroBulletAudioHandler(int newFrameUpdate);