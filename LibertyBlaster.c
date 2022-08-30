#include "LibertyBlaster.h"
#define _CRT_SECURE_NO_WARNINGS
#include "booletmanager.h"
#include "Entities.h"
// 
//#define centeredText(charcount) ((((charcount - 1) / 2) * -1.05)- 0.025)
#define centeredText(charcount) (float)((((double)charcount - 1.0) * -1.05)/ 2.0)

//#include "shipAIManager.h"

#define DFS 0.0255//idk, ease of use? Don't change, not meant 

#define END_OF_GOOD 5
#define END_OF_OKAY 9
#define END_OF_BAD 15
static const char commsLines[16][100] = {
	"Good job out there!",//0
	"It's clear for a bit, let's make sure you're running smooth.",//1
	"We'll give your ship a good examination.",//2
	"They'll be back soon, get ready.",//3
	"Keep up the good work, really helping thin them out.",//4
	"Don't think we'd hold back here without you.",//5
	"Bit rough here, but we'll fix you up.",//6
	"Our boys back here aren't doing too hot, but we're here.",//7
	"FDS BROADCAST: ALL FEDERAL DEFENCE FORCES FALL BACK",//8
	"FDS BROADCAST: OUTER PERIMETER COMPROMISED",//9
	"AUTOMATED BROADCAST: UNIT 7, RESPOND", //10
	"AUTOMATED BROADCAST: UNIT 9, RESPOND", //11
	"AUTOMATED BROADCAST: UNIT 2, RESPOND", //12
	"AUTOMATED BROADCAST: UNIT 11, RESPOND", //13
	"AUTOMATED BRO...",//14
	"Sorry, don't think we're getting through to you out there...",//15
};

struct ourShip OurShip = { 0 };
idSet enShipStuff = { 0 };

volatile static int ShouldBeFullscreen = 1;
volatile static int IsFullscreen = 1;
volatile static Object* DirectionalArrow = 0;

void disableAllUILayers() {
	for (int cLayer = 0; cLayer < UICount; cLayer++) {
		masterUIList[cLayer]->active = 0;
	}
}

void activateGameUI() {
	BaseGameUI->active = 1;
	DynamicGameUI->active = 1;
	CrosshairUI->active = 1;
}

void gotoEndscreen(unsigned int* curWave) {
	glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	getsetGamestate(END_SCREEN);
	setupEndscreen(curWave);
}

void setOurShip() {
	char* clearing = &OurShip;
	for (int cByte = 0; cByte < sizeof(struct ourShip); cByte++) {
		clearing[cByte] = 0;
	}

	
	gameworld.camera[X_pos] = 0;
	gameworld.camera[Y_pos] = 0;
	gameworld.camera[Z_pos] = 10000;
	gameworld.camera[W_pos] = 1;
	gameworld.camera[I_pos] = 0;
	gameworld.camera[J_pos] = 0;
	gameworld.camera[K_pos] = 0;
	gameworld.up[0] = 0;
	gameworld.up[1] = 0;
	gameworld.up[2] = -1;
	gameworld.up[3] = 0;

	gameworld.back[0] = 0;
	gameworld.back[1] = 0;
	gameworld.back[2] = 0;
	gameworld.back[3] = 1;

	gameworld.left[0] = 0;
	gameworld.left[1] = -1;
	gameworld.left[2] = 0;
	gameworld.left[3] = 0;

	OurShip.fireRate = OUR_FIRE_RATE;
	//The following hp's comments are 'should be's really, but can be change via defines
	OurShip.shields = MAX_SHIELDS; //Interget offset by 1, so 100% shields will be displayed for 1,000
	OurShip.armour = MAX_ARMOUR; //Interget offset by 1, so 100% shields will be displayed for 1,000
	OurShip.hp = MAX_HULL;
}

int getsetGamestate(int flag) {
	static int ourState = IN_MAIN_MENU;
	if (flag != DONT_STATE_CHANGE) {
		ourState = flag;
	}
	return ourState;
}

//returns which comms thing to select and display
int updatePlayerOnWave(unsigned int waveNum) {
	//int broadcastState = 0;
	int selectedBroadcast = 0;
	if (passedEnemies >= 150) {
		//broadcastState = 2;//bad
		selectedBroadcast = randNum(END_OF_OKAY + 1, END_OF_BAD);
	} else if (passedEnemies >= 75) {
		//broadcastState = 1;//meh
		selectedBroadcast = randNum(END_OF_GOOD + 1, END_OF_OKAY);
	} else {
		selectedBroadcast = randNum(0, END_OF_GOOD);
	}
	
	int hullRepairAvailable = (int)(BASE_HULL_REPAIR / ((float)passedEnemies * PASSED_ENEMY_WORTH_HULL));
	//no sense doing all the maf if we don't need repairs
	if (OurShip.armour < MAX_ARMOUR) {
		//find how much armour repair we can have at max
		int armourRepairAvailable = (int)(BASE_ARMOUR_REPAIR - ((float)passedEnemies * PASSED_ENEMY_WORTH_ARMOUR));
		//if we don't have enough, it's simple math of adding the max repair to our current armour and being short
		if (OurShip.armour + armourRepairAvailable < MAX_ARMOUR) {
			OurShip.armour += armourRepairAvailable;
		} else {
			//otherwise, if we have more than enough repair, we need not do any math! Just set armour to max
			OurShip.armour = MAX_ARMOUR;
		}
	}

	//Second verse, same as the first
	//no sense doing all the maf if we don't need repairs
	if (OurShip.hp < MAX_HULL) {
		//find how much armour repair we can have at max
		int hullRepairAvailable = (int)(BASE_HULL_REPAIR - ((float)passedEnemies * PASSED_ENEMY_WORTH_HULL));
		//if we don't have enough, it's simple math of adding the max repair to our current armour and being short
		if (OurShip.hp + hullRepairAvailable < MAX_ARMOUR) {
			OurShip.hp += hullRepairAvailable;
		} else {
			//otherwise, if we have more than enough repair, we need not do any math! Just set armour to max
			OurShip.hp = MAX_HULL;
		}
	}
	passedEnemies = 0;

	return(selectedBroadcast);
}

void updateEnemiesOnWave(unsigned int waveNum, int* maxOnScreenEnemies) {
	//ENEMY_DISTANCE = 500  -This one doesn't need to be changed terribly much
	//ENEMY_POS_RANGE = 2500
	//ENEMY_HP = 2
	//ENEMY_TARGET_CHANCE = 0.25
	//ENEMY_TARGET_DIST_MIN = 1600; //(SQUARED!!!!) How close before an enemy is forced to pull out
	//ENEMY_TARGET_DIST = 100; //how close an enemy can be on the z axis before they won't begin targeting
	//ENEMY_TARGET_DIST_MAX = 300; //how far an enemy can be on the z axis before they won't begin targeting
	//ENEMY_FIRE_CHANCE = 3.0
	//SPAWN_RATE = 20;//how many frames between spawns

	//'start' game, wave 0 - 1
	//'early-mid game' wave 2-4
	//'mid game' wave 5-8
	//'late game' wave 9-19
	//'death mode' wave 20+

	//'start' game, settings wave 0 - 1
	if (waveNum == 0) {
		ENEMY_DISTANCE = 400;
		ENEMY_HP = 1;//1
		ENEMY_POS_RANGE = 1800;
		ENEMY_TARGET_CHANCE = 0.1;//0.10
		ENEMY_TARGET_DIST_MIN = 1600;//1600
		ENEMY_TARGET_DIST = 150;
		ENEMY_TARGET_DIST_MAX = 300;//300
		ENEMY_FIRE_CHANCE = 1.5;//1.5
		SPAWN_RATE = 30;//30
		ENEMIES_PER_WAVE = 80;
		*maxOnScreenEnemies = 20;//20
	}

	//'early-mid game' settings wave 2-4 
	else if (waveNum >= 1 && waveNum <= 4) {
		ENEMY_DISTANCE = 500;
		ENEMY_HP = 2;
		ENEMY_POS_RANGE = 2200;
		ENEMY_TARGET_CHANCE = 0.2;
		ENEMY_TARGET_DIST_MIN = 1600;
		ENEMY_TARGET_DIST = 100;
		ENEMY_TARGET_DIST_MAX = 325;
		ENEMY_FIRE_CHANCE = 2.5;
		SPAWN_RATE = 20;
		ENEMIES_PER_WAVE = 70;
		*maxOnScreenEnemies = 30;
	}

	//'mid game' settings wave 5-8
	else if (waveNum >= 5 && waveNum <= 8) {
		ENEMY_DISTANCE = 500;
		ENEMY_HP = 2;
		ENEMY_POS_RANGE = 2500;
		ENEMY_TARGET_CHANCE = 0.25;
		ENEMY_TARGET_DIST_MIN = 1500;
		ENEMY_TARGET_DIST = 100;
		ENEMY_TARGET_DIST_MAX = 325;
		ENEMY_FIRE_CHANCE = 3.0;
		SPAWN_RATE = 15;
		ENEMIES_PER_WAVE = 120;
		*maxOnScreenEnemies = 40;
	}

	//'late game' settings wave 9-19
	else if (waveNum >= 9 && waveNum <= 19) {
		ENEMY_DISTANCE = 500;
		ENEMY_HP = 3;
		ENEMY_POS_RANGE = 2800;
		ENEMY_TARGET_CHANCE = 0.3;
		ENEMY_TARGET_DIST_MIN = 1400;
		ENEMY_TARGET_DIST = 90;
		ENEMY_TARGET_DIST_MAX = 350;
		ENEMY_FIRE_CHANCE = 3.0;
		SPAWN_RATE = 12;
		ENEMIES_PER_WAVE = 220;
		*maxOnScreenEnemies = 65;
	}

	//'death mode' settings wave 20+
	else if (waveNum >= 20) {
		ENEMY_DISTANCE = 550;
		ENEMY_HP = 3;
		ENEMY_POS_RANGE = 2800;
		ENEMY_TARGET_CHANCE = 0.35;
		ENEMY_TARGET_DIST_MIN = 1400;
		ENEMY_TARGET_DIST = 90;
		ENEMY_TARGET_DIST_MAX = 400;
		ENEMY_FIRE_CHANCE = 3.0;
		SPAWN_RATE = 8;
		ENEMIES_PER_WAVE = 300;
		*maxOnScreenEnemies = MAX_ENEMY_COUNT;
	}

}

void runGame(GLFWwindow* window, int flagSetting) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volatile static EnShip* enemyShipList = NULL;
	static unsigned int waveNum = 0;
	static int maxOnScreenEnemies = MAX_ENEMY_COUNT;
	static long int enteredBetweenWave = 0;
	static int currentCommsBroadcast = -1;

	//used for just going back to the main menu from wherever
	if (flagSetting == CLEAR_GAME) {
		waveNum = 0;
		maxOnScreenEnemies = MAX_ENEMY_COUNT;
		enteredBetweenWave = 0;
		currentCommsBroadcast = -1;
		attemptToResetGameVariables();
		updateBroadcast(-1, 0);
		//our ship gets set up upon game start anyhow
		getsetGamestate(IN_MAIN_MENU);				
	}


	if (flagSetting == IN_MAIN_MENU) { //MAIN MENU LOOP
		MainMenuUI->active = 1;//ez way to make sure the right layer is up
		SettingsUI->active = 0;
		runMasterUI();
	}
	else if (flagSetting == IN_KEYSCREEN) { //MAIN MENU LOOP
		runMasterUI();
	}
	else if (flagSetting == IN_SETTINGS) { //SETTINGS LOOP
		if (ShouldBeFullscreen == 1 && IsFullscreen != 1) {
			GLFWmonitor* monitor;
			monitor = glfwGetPrimaryMonitor();
			GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(gamewindow, monitor, 0, 0, mode->width, mode->height, 60);
			IsFullscreen = 1;
		} else if (ShouldBeFullscreen == 0 && IsFullscreen == 1) {			
			GLFWmonitor* monitor;
			monitor = glfwGetPrimaryMonitor();
			GLFWvidmode* mode = glfwGetVideoMode(monitor);
			glfwSetWindowMonitor(gamewindow, NULL, 5, 20, (int)((float)mode->width * 0.9), (int)((float)mode->height * 0.9), 60);
			IsFullscreen = 0;
		}
		SettingsUI->active = 1;
		CrosshairUI->active = 1;
		MainMenuUI->active = 0;		
		runMasterUI();
	}
	else if (flagSetting == STARTING_GAME) { //GAME LOAD
		glLineWidth(1);//just in case, make sure we're in the right mode
		glDisable(GL_LINE_SMOOTH);

		//set up enemy ship list, if it hasn't been set up at all yet we need to do that.
		if (enemyShipList == NULL) {
			enemyShipList = calloc(MAX_ENEMY_COUNT, sizeof(EnShip));
			enShipStuff = getRefID(ENSHIP);
			for (int cShip = 0; cShip < MAX_ENEMY_COUNT; cShip++) {
				setShip(&enemyShipList[cShip]);
				enemyShipList[cShip].worldID = insertObjectIntoWorld(&gameworld, (Object*)&enemyShipList[cShip], 1);
				voidShip(&enemyShipList[cShip]);
			}
		} 
		//otherwise, just set and void ships
		else {
			for (int cShip = 0; cShip < MAX_ENEMY_COUNT; cShip++) {
				setShip(&enemyShipList[cShip]);
				voidShip(&enemyShipList[cShip]);
			}
		}

		//make sure all the big variables are set to what they need to be.
		waveNum = 0;
		maxOnScreenEnemies = MAX_ENEMY_COUNT;
		enteredBetweenWave = 0;
		currentCommsBroadcast = -1;
		attemptToResetGameVariables();
		updateBroadcast(-1, 0);
		

		//initialize boolet system
		setupBoolet();

		//set up a couple of our own ship's things
		setOurShip();

		//update this in case it needs to update off base numbers and such
		updateBroadcast(-1, 0);
		updateDynamicUI(-1);

		//disable all the layers,
		disableAllUILayers();

		//enable the ones we need
		activateGameUI();
		

		//select the next game state
		updateEnemiesOnWave(waveNum, &maxOnScreenEnemies);//in case it differs from base defines
		getsetGamestate(IN_GAME);
	}
	else if (flagSetting == IN_GAME) { //GAME LOOP
		gameCursorMovement();
		ourShipHandler();
		dirArrowHandler();
		if (updateBoolets(enemyShipList, maxOnScreenEnemies, &OurShip) == 1 || OurShip.hp <= 0) {
			gotoEndscreen(&waveNum);
			goto ENDSCREEN;
		}
		updateDynamicUI(-1);
		runMasterUI();
		enemyShipList = enemyShipHandler(enemyShipList, maxOnScreenEnemies);
		if (ENEMIES_PER_WAVE == -1) {//if wave is out of enemies
			waveNum++;
			//printf("Switching to between waves, Wave %d", waveNum);
			enteredBetweenWave = 0;
			currentCommsBroadcast = -1;
			getsetGamestate(BETWEEN_WAVES);
		}
		drawWorld(&gameworld);
	} 
	//Render it all, update our ship constantly, on first run select a comms broadcast and re-set our variables for how many enemies and such
	else if (flagSetting == BETWEEN_WAVES) {
		gameCursorMovement();
		ourShipHandler();
		dirArrowHandler();
		if (updateBoolets(enemyShipList, maxOnScreenEnemies, &OurShip) == 1) {
			gotoEndscreen(waveNum);
			goto ENDSCREEN;
		}
		updateDynamicUI(waveNum);
		runMasterUI();		
		drawWorld(&gameworld);
		//if we JUST entered between the waves, do updates to all the variables and select our current comms broadcast
		if (enteredBetweenWave == 0) {
			updateEnemiesOnWave(waveNum, &maxOnScreenEnemies);
			currentCommsBroadcast = updatePlayerOnWave(waveNum);
		}
		long int broadcastFlag = updateBroadcast(currentCommsBroadcast, enteredBetweenWave/COMMS_UPDATE_DELAY);
		enteredBetweenWave++;
		

		if (broadcastFlag < 0) {
			//printf("Wave starting");
			getsetGamestate(IN_GAME);
		}
		
	} 
	else if (flagSetting == PAUSE_MENU) { //GAME LOOP
		runMasterUI();
		drawWorld(&gameworld);
	} 
	else if (flagSetting == END_SCREEN) {
ENDSCREEN:;
		runMasterUI();


	} 


EndOfFunction:;
}


World* loadGame() {
	gamewindow = startup(expandedMouseClick, keypressHandler);

	FILE* debugfile = fopen("debugfile.txt", "a");
	fputs("Past startup call completely\n\n", debugfile);
	fclose(debugfile);
	AudioSetting = DEFAULT_AUDIO_SETTING;

	masterObjLenght = 0;
	masterObjList = NULL;
	gameworld = createWorld(VECTOR_WORLD);
	gameworld.vecColour[0] = 0;
	gameworld.vecColour[1] = 1;
	gameworld.vecColour[2] = 0;
	gameworld.vecColour[3] = 1;
	
	setAudioFunctions(DEFAULT_AUDIO_SETTING);

	loadEnemyShip();
	DirectionalArrow = loadDirectionArrow();

	setupMasterUIList();
	setupMainMenu();
	setupPauseMenu();
	setupKeysUI();
	setupSettingsUI();
	setupGameUI();
	setupCrosshairUI();
	setupDynamicUI();
	return(&gameworld);
}

void setupMasterUIList() {
	masterUIList = calloc(UICount, sizeof(UI*));
	masterUIListLength = UICount;
	for (int cLayer = 0; cLayer < UICount; cLayer++) {
		masterUIList[cLayer] = calloc(1, sizeof(UI));
	}
}

void setupMainMenu() {	
	//glLineWidth(1);
	//glEnable(GL_LINE_SMOOTH);
	MainMenuUI->active = 1;
	MainMenuUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	MainMenuUI->vecColour[0] = 0;
	MainMenuUI->vecColour[1] = 1;
	MainMenuUI->vecColour[2] = 0;
	MainMenuUI->vecColour[3] = 1;

	UIElement* passer = NULL;


	float gameStartButton[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	unsigned int buttonInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	float pos[] = { 0,-0.15,0};
	float clickarea[] = { 0.3,0.3,0.06,0.06 };
	float buttontextpos[] = { -4.725,0,0 };
	
	UnfinObj buttontext = createVecText("START GAME", buttontextpos, 0.05);
	insertElementIntoUI(MainMenuUI, createVectorElement(gameStartButton, buttonInds, (sizeof(gameStartButton) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, startGameButton, 1, clickarea));
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.175;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);

	float settingsButtonverts[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	buttontextpos[0] = -3.675;
	pos[1] = -0.35;
	insertElementIntoUI(MainMenuUI, createVectorElement(settingsButtonverts, buttonInds, (sizeof(settingsButtonverts) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, settingsButton, 1, clickarea));
	buttontext = createVecText("SETTINGS", buttontextpos, 0.05);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.375;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);

	buttontextpos[0] = centeredText((countof("SHOW KEYS") - 1));
	pos[1] = -0.55;
	insertElementIntoUI(MainMenuUI, createVectorElement(settingsButtonverts, buttonInds, (sizeof(settingsButtonverts) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, gotoKeysPageButton, 1, clickarea));
	buttontext = createVecText("SHOW KEYS", buttontextpos, 0.05);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.575;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);


	//Exit gaem button
	buttontextpos[0] = -4.275;
	pos[1] = -0.75;
	insertElementIntoUI(MainMenuUI, createVectorElement(settingsButtonverts, buttonInds, (sizeof(settingsButtonverts) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, exitGameButton, 1, clickarea));
	buttontext = createVecText("EXIT GAME", buttontextpos, 0.05);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.775;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);


	float titlePos[] = { centeredText(7), 0.0,0 };
	float titleScreenpos[] = { 0, 0.45, 0 };
	UnfinObj title = createVecText("BLAZING\nLIBERTY", titlePos, 0.3);//BLAZING\nLIBERTY
	//UnfinObj title = createVecText("ABCDEFG\n01234\n56789", titlePos, 0.15);
	passer = createVectorElement(title.verts, title.indices, title.vLineCount, title.iCount, titleScreenpos, NULL, 1, NULL);
	passer->scale = 0.3;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(title);
}

void setupPauseMenu() {
	PauseScreenUI->active = 0;
	PauseScreenUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	PauseScreenUI->vecColour[0] = 0;
	PauseScreenUI->vecColour[1] = 1;
	PauseScreenUI->vecColour[2] = 0;
	PauseScreenUI->vecColour[3] = 1;

	UIElement* passer = NULL;


	float buttonBase[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	unsigned int buttonInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	float pos[] = { 0,0.3,0 };
	float clickarea[] = { 0.2,0.2,0.06,0.06 };
	float buttontextpos[] = { -4.225,0,0 };

	UnfinObj buttontext = createVecText("MAIN MENU", buttontextpos, 0.05);
	insertElementIntoUI(PauseScreenUI, createVectorElement(buttonBase, buttonInds, (sizeof(buttonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, returnToMenuButton, 1, clickarea));
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = 0.275;
	passer->scale = 0.051;
	insertElementIntoUI(PauseScreenUI, passer);
	freeUnfinObj(buttontext);

	pos[1] = -0.3;
	insertElementIntoUI(PauseScreenUI, createVectorElement(buttonBase, buttonInds, (sizeof(buttonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, exitGameButton, 1, clickarea));
	buttontext = createVecText("EXIT GAME", buttontextpos, 0.05);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.325;
	passer->scale = 0.051;
	insertElementIntoUI(PauseScreenUI, passer);
	freeUnfinObj(buttontext);
}

void setupSettingsUI() {
	SettingsUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	SettingsUI->vecColour[0] = 0;
	SettingsUI->vecColour[1] = 1;
	SettingsUI->vecColour[2] = 0;
	SettingsUI->vecColour[3] = 1;
	UIElement* passer = NULL;

	float buttonBase[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	unsigned int buttonInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	float pos[] = { 0,0.7,0 };
	float clickarea[] = { 0.3,0.3,0.06,0.06 };
	float buttontextpos[] = { -4.725,0,0 };
	
	//0
	insertElementIntoUI(SettingsUI, createVectorElement(buttonBase, buttonInds, (sizeof(buttonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, fullscreenButton, 1, clickarea));

	UnfinObj buttontext = createVecText("FULLSCREEN", buttontextpos, 0.05);	
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = 0.675;
	passer->scale = 0.051;
	insertElementIntoUI(SettingsUI, passer);//1
	freeUnfinObj(buttontext);


	pos[1] = -0.5;
	buttontextpos[0] = -1.725;
	insertElementIntoUI(SettingsUI, createVectorElement(buttonBase, buttonInds, (sizeof(buttonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, returnToMenuButton, 1, clickarea));
	buttontext = createVecText("BACK", buttontextpos, 0.05);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.525;
	passer->scale = 0.051;
	insertElementIntoUI(SettingsUI, passer);//1
	freeUnfinObj(buttontext);

	pos[1] = 0;
	buttontextpos[0] = -8.95;
	buttontext = createVecText("CROSSHAIR SETTINGS", buttontextpos, 0.0751);
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = 0.40;
	passer->scale = 0.0751;
	insertElementIntoUI(SettingsUI, passer);//1
	freeUnfinObj(buttontext);


	const float dotClickArea[] = { 0.08, 0.08, 0.06, 0.001 };
	buttontextpos[0] = -1.05;
	buttontextpos[1] = 0;
	buttontext = createVecText("DOT", buttontextpos, 0.052);
	pos[1] = 0.3;
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, dotSettingsButton, 1, dotClickArea);
	passer->scale = 0.052;
	insertElementIntoUI(SettingsUI, passer);//1
	freeUnfinObj(buttontext);


	const float wingsClickArea[] = { 0.14, 0.14, 0.06, 0 };
	buttontextpos[0] = -2.1;
	buttontextpos[1] = 0;
	buttontext = createVecText("WINGS", buttontextpos, 0.051);
	pos[1] = 0.20;
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, wingsSettingButton, 1, wingsClickArea);
	passer->scale = 0.051;
	insertElementIntoUI(SettingsUI, passer);//1
	freeUnfinObj(buttontext);
}

void setupKeysUI() {
	KeypageUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	KeypageUI->vecColour[0] = 0;
	KeypageUI->vecColour[1] = 1;
	KeypageUI->vecColour[2] = 0;
	KeypageUI->vecColour[3] = 1;

	float pos[3] = { 0,0,0 };	
	float textpos[] = { 0,0,0 };
	float* textOffset = &textpos[0];

	*textOffset = centeredText((countof("UP/DOWN:  W/S") - 1));
	UnfinObj textobj = createVecText("UP/DOWN:  W/S", textpos, 0.05);
	UIElement* passer = createVectorElement(textobj.verts, textobj.indices, textobj.vLineCount, textobj.iCount, pos, NULL, 1, NULL);
	passer->position[Y_pos] = 0.5;
	passer->scale = 0.051;
	insertElementIntoUI(KeypageUI, passer);//1
	freeUnfinObj(textobj);

	*textOffset = centeredText((countof("LEFT/RIGHT:  A/D") - 1));
	textobj = createVecText("LEFT/RIGHT:  A/D", textpos, 0.05);
	passer = createVectorElement(textobj.verts, textobj.indices, textobj.vLineCount, textobj.iCount, pos, NULL, 1, NULL);
	passer->position[Y_pos] = 0.3;
	passer->scale = 0.051;
	insertElementIntoUI(KeypageUI, passer);//1
	freeUnfinObj(textobj);

	*textOffset = centeredText((countof("FORWARD/BACK:  V/C") - 1));
	textobj = createVecText("FORWARD/BACK:  V/C", textpos, 0.05);
	passer = createVectorElement(textobj.verts, textobj.indices, textobj.vLineCount, textobj.iCount, pos, NULL, 1, NULL);
	passer->position[Y_pos] = 0.1;
	passer->scale = 0.051;
	insertElementIntoUI(KeypageUI, passer);//1
	freeUnfinObj(textobj);

	float buttonBase[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	unsigned int buttonInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	pos[1] = -0.1;
	float clickarea[] = { 0.3,0.3,0.06,0.06 };
	insertElementIntoUI(KeypageUI, createVectorElement(buttonBase, buttonInds, (sizeof(buttonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, returnToMenuButton, 1, clickarea));
	*textOffset = centeredText((countof("BACK") - 1));
	textobj = createVecText("BACK", textpos, 0.05);
	passer = createVectorElement(textobj.verts, textobj.indices, textobj.vLineCount, textobj.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.125;
	passer->scale = 0.051;
	insertElementIntoUI(KeypageUI, passer);//1
	freeUnfinObj(textobj);
}

//End screen is a one off creation because I didn't want to make dynamic ui stuff for it
void setupEndscreen(unsigned int* endingWave) {
	
	static int alreadySetUpBase = 0;

	static int ourScoreID = 0;
	static int ourWaveID = 0;

	disableAllUILayers();
	EndscreenUI->active = 1;
	//load up the regular background bits into memory only once
	if (alreadySetUpBase == 0) {		
		alreadySetUpBase = 1;
		EndscreenUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
		EndscreenUI->vecColour[0] = 0;
		EndscreenUI->vecColour[1] = 1;
		EndscreenUI->vecColour[2] = 0;
		EndscreenUI->vecColour[3] = 1;

		UIElement* passer = NULL;


		float buttonBase[] = {
			-0.3, 0.06, -0.5,
			0.3, 0.06, -0.5,
			0.3, -0.06, -0.5,
			-0.3, -0.06, -0.5,
		};
		float wideButtonBase[] = {
			-0.4, 0.06, -0.5,
			0.4, 0.06, -0.5,
			0.4, -0.06, -0.5,
			-0.4, -0.06, -0.5,
		};
		unsigned int buttonInds[] = {
			0,1,2,1, 2,3, 3,0
		};
		float pos[] = { 0,0,0 };
		float clickarea[] = { 0.3,0.3,0.06,0.06 };
		float wideClickArea[] = { 0.4,0.4,0.06,0.06 };
		float buttontextpos[] = { -6.725,-0.5,0 };

		//main menu button
		pos[1] = -0.4;
		UnfinObj buttontext = createVecText("RETURN TO MENU", buttontextpos, 0.05);
		insertElementIntoUI(EndscreenUI, createVectorElement(wideButtonBase, buttonInds, (sizeof(wideButtonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, returnToMenuButton, 1, wideClickArea));
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.051;
		insertElementIntoUI(EndscreenUI, passer);
		freeUnfinObj(buttontext);

		//start again button
		pos[1] = -0.6;
		buttontextpos[0] = -5.225;
		buttontext = createVecText("START AGAIN", buttontextpos, 0.05);
		insertElementIntoUI(EndscreenUI, createVectorElement(wideButtonBase, buttonInds, (sizeof(wideButtonBase) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(buttonInds) / sizeof(unsigned int), pos, startGameButton, 1, wideClickArea));
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.051;
		insertElementIntoUI(EndscreenUI, passer);
		freeUnfinObj(buttontext);

		//the static text that remains unchanged game to game, aka, not the score itself or wave number
		pos[1] = 0.6;
		buttontextpos[0] = -4.2;
		buttontext = createVecText("GAME OVER", buttontextpos, 0.25);
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.25;
		insertElementIntoUI(EndscreenUI, passer);
		freeUnfinObj(buttontext);
		
		pos[1] = 0.3;
		buttontextpos[0] = -5.25;
		buttontext = createVecText("FINAL SCORE", buttontextpos, 0.065);
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.065;
		insertElementIntoUI(EndscreenUI, passer);

		pos[1] = 0.0;
		buttontextpos[0] = -5.775;
		buttontext = createVecText("WAVE REACHED", buttontextpos, 0.065);
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.065;
		insertElementIntoUI(EndscreenUI, passer);

		//score's object
		buttontextpos[0] = 0;
		buttontext = createVecText("F", buttontextpos, 0.065);
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.065;
		passer->position[1] = 0.18;
		ourScoreID = insertElementIntoUI(EndscreenUI, passer);

		//wave's object
		buttontextpos[0] = 0;
		buttontext = createVecText("D", buttontextpos, 0.065);
		passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
		passer->scale = 0.065;
		passer->position[1] = -0.12;
		ourWaveID = insertElementIntoUI(EndscreenUI, passer);
	}
	
	char mainString[25] = { 0 };
	//Wave number display
	float pos[] = {
		0,0,0,
	};	
	int temp = (int)*endingWave;
	_itoa(temp, mainString, 10);
	pos[0] -= ((strlen(mainString) - 1) * 0.525);
	UnfinObj newData = createVecText(mainString, pos, 0.06);	

	glBindVertexArray(EndscreenUI->elements[ourWaveID]->ID);
	glBindBuffer(GL_ARRAY_BUFFER, EndscreenUI->elements[ourWaveID]->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EndscreenUI->elements[ourWaveID]->EBO);
	glBufferData(GL_ARRAY_BUFFER, newData.vLineCount * VERTEX_SIZE * VECTOR_VERTEX_LENGTH, newData.verts, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newData.iCount * IND_SIZE, newData.indices, GL_STREAM_DRAW);
	EndscreenUI->elements[ourWaveID]->indexCount = newData.iCount;
	glBindVertexArray(0);
	freeUnfinObj(newData);

	//Score display
	_ultoa_s((unsigned long)score, mainString, 23, 10);
	pos[0] = 0;
	pos[0] -= ((strlen(mainString) - 1) * 0.525);
	newData = createVecText(mainString, pos, 0.06);

	glBindVertexArray(EndscreenUI->elements[ourScoreID]->ID);
	glBindBuffer(GL_ARRAY_BUFFER, EndscreenUI->elements[ourScoreID]->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EndscreenUI->elements[ourScoreID]->EBO);
	glBufferData(GL_ARRAY_BUFFER, newData.vLineCount* VERTEX_SIZE* VECTOR_VERTEX_LENGTH, newData.verts, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newData.iCount* IND_SIZE, newData.indices, GL_STREAM_DRAW);
	EndscreenUI->elements[ourScoreID]->indexCount = newData.iCount;
	glBindVertexArray(0);
	freeUnfinObj(newData);

}


void setupGameUI() {
	BaseGameUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	BaseGameUI->vecColour[0] = 0;
	BaseGameUI->vecColour[1] = 1;
	BaseGameUI->vecColour[2] = 0;
	BaseGameUI->vecColour[3] = 1;


	const float bottomAnchor[] = {
		0, -1, 0
	};
	
	UIElement* passer = NULL;
	const float upleftBox[] = {
		-0.01,-0.20,0,			 //0
		0.34,-0.15,0,            //1
		0.38,0.01,0,            //2
	};
	const float uprightBox[] = {
		0.01,-0.20,0,				//0
		-0.34,-0.15,0,		  //1
		-0.38,0.01,0,        //2
	};
	const unsigned int cornBoxInd[] = {
		0,1,1,2,
	};
	const float ulbPos[] = {
		-1, 1, 0
	};
	const float urbPos[] = {
		1, 1, 0
	};
	const float shieldTextPos[] = {
		1.25, -1.5, 0
	};
	const float shieldPos[] = {
		-1, 1, 0
	};
	const float armourTextPos[] = {
		-7.75, -1.5, 0
	};
	const float armourPos[] = {
		1, 1, 0
	};
	const float zeroPos[] = {
		0,0,0
	};

	UnfinObj shields = createVecText("SHIELDS:", shieldTextPos, 0.04);
	passer = createVectorElement(shields.verts, shields.indices, shields.vLineCount, shields.iCount, shieldPos, NULL, 1, NULL);
	passer->scale = 0.04;
	insertElementIntoUI(BaseGameUI, passer);
	passer = createVectorElement(upleftBox, cornBoxInd, countof(upleftBox) / VECTOR_VERTEX_LENGTH, countof(cornBoxInd), ulbPos, NULL, 1, NULL);
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(shields);

	UnfinObj armour = createVecText("ARMOUR:", armourTextPos, 0.04);
	passer = createVectorElement(armour.verts, armour.indices, armour.vLineCount, armour.iCount, armourPos, NULL, 1, NULL);
	passer->scale = 0.04;
	insertElementIntoUI(BaseGameUI, passer);
	passer = createVectorElement(uprightBox, cornBoxInd, countof(uprightBox) / VECTOR_VERTEX_LENGTH, countof(cornBoxInd), urbPos, NULL, 1, NULL);
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(armour);
	//free(passer);

	float dash[] = {
		-0.90,-0.05,0,			//0
		-0.40,0.20,0,			//1
		0.40,0.20,0,				//2
		0.90,-0.05,0,			//3
	};
	unsigned int dashind[] = {
		0,1, 1,2, 2,3,
	};

	passer = createVectorElement(dash, dashind, countof(dash) / VECTOR_VERTEX_LENGTH, countof(dashind), bottomAnchor, NULL, 1, NULL);
	insertElementIntoUI(BaseGameUI, passer);

	//const float ammoTextPos[] = {
	//	-16, 6, 0
	//};
	
	//UnfinObj ammo = createVecText("AMMUNITION:", ammoTextPos, 0.0255);
	//passer = createVectorElement(ammo.verts, ammo.indices, ammo.vLineCount, ammo.iCount, bottomAnchor, NULL, 1, NULL);
	//passer->scale = 0.0255;
	//insertElementIntoUI(BaseGameUI, passer);
	//freeUnfinObj(ammo);

	const float speedTextPos[] = {
		4.5, 5.5, 0
	};
	UnfinObj speed = createVecText("SPEED:", speedTextPos, 0.0255);
	passer = createVectorElement(speed.verts, speed.indices, speed.vLineCount, speed.iCount, bottomAnchor, NULL, 1, NULL);
	passer->scale = 0.0255;
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(speed);

	const float shipIntTextPos[] = {
		4.5, 2, 0
	};
	UnfinObj shipInt = createVecText("HULL STATUS:", shipIntTextPos, 0.0255);//Note, there's going to be a lil light to the right of this that makes the unevenness okay
	passer = createVectorElement(shipInt.verts, shipInt.indices, shipInt.vLineCount, shipInt.iCount, bottomAnchor, NULL, 1, NULL);
	passer->scale = 0.0255;
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(shipInt);

	const float commsTextPos[] = {
		-23, 2, 0
	};
	UnfinObj comms = createVecText("COMMS:", commsTextPos, 0.0255);
	passer = createVectorElement(comms.verts, comms.indices, comms.vLineCount, comms.iCount, bottomAnchor, NULL, 1, NULL);
	passer->scale = 0.0255;
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(comms);

	const float commsBox[] = {
		-0.4, 0.03, 0,			//0
		-0.4, 0.10, 0,				//1
		0.05, 0.10, 0,			//2
		0.05, 0.03, 0,			//3
	};
	const unsigned int commsBoxInd[] = {
		0,1, 1,2, 2,3, 3,0,
	};

	passer = createVectorElement(commsBox, commsBoxInd, countof(commsBox)/VECTOR_VERTEX_LENGTH, countof(commsBoxInd), bottomAnchor, NULL, 1, NULL);
	insertElementIntoUI(BaseGameUI, passer);

	const float arrowTextPos[] = {
		-15, 5.5, 0
	};
	UnfinObj arrowText = createVecText("TARGET LOCK", arrowTextPos, 0.0255);
	passer = createVectorElement(arrowText.verts, arrowText.indices, arrowText.vLineCount, arrowText.iCount, bottomAnchor, NULL, 1, NULL);
	passer->scale = 0.0255;
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(arrowText);

	const float arrowBox[] = {
		-0.06, 0.178, 0,			//0
		-0.06, 0.128, 0,				//1
		-0.01, 0.128, 0,			//2
		-0.01, 0.178, 0,			//3
	};
	const unsigned int arrowBoxInd[] = {
		0,1, 1,2, 2,3, 3,0,
	};

	passer = createVectorElement(arrowBox, arrowBoxInd, countof(arrowBox) / VECTOR_VERTEX_LENGTH, countof(arrowBoxInd), bottomAnchor, NULL, 1, NULL);
	insertElementIntoUI(BaseGameUI, passer);
}

void setupCrosshairUI() {
	CrosshairUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	CrosshairUI->vecColour[0] = 0;
	CrosshairUI->vecColour[1] = 1;
	CrosshairUI->vecColour[2] = 0;
	CrosshairUI->vecColour[3] = 1;

	UIElement* passer = NULL;
	const float zeroPos[] = { 0,0,0 };

	const float bigCrosshair[] = {
		   -0.25, 0.00, 0,			//0
		   -0.2, 0.00, 0,			//1
		   -0.1, 0.05, 0,				//2
		   -0.1, -0.05, 0,			//3

		   0.25, 0.00, 0,			//4
		   0.2, 0.00, 0,				//5
		   0.1, 0.05, 0,				//6
		   0.1, -0.05, 0,				//7
	};
	const unsigned int bigCrosshairInds[] = {
		0,1, 1,2, 1,3,
		4,5, 5,6, 5,7,
	};

	passer = createVectorElement(bigCrosshair, bigCrosshairInds, countof(bigCrosshair) / VECTOR_VERTEX_LENGTH, countof(bigCrosshairInds), zeroPos, NULL, 1, NULL);
	CrosshairWingsID = insertElementIntoUI(CrosshairUI, passer);

	const float smolDot[] = {
		0.0, 0.001, 0,			//0
		0.0, -0.001, 0,			//1
	};
	const unsigned int smolDotInds[] = {
		0,1,
	};

	passer = createVectorElement(smolDot, smolDotInds, countof(smolDot) / VECTOR_VERTEX_LENGTH, countof(smolDotInds), zeroPos, NULL, 1, NULL);
	CrosshairSmallDotID = insertElementIntoUI(CrosshairUI, passer);

	const float bigDot[] = {
		0.0, 0.005, 0,			//0
		0.0, -0.005, 0,			//1
	};
	const unsigned int bigDotInds[] = {
		0,1,
	};

	passer = createVectorElement(bigDot, bigDotInds, countof(bigDot) / VECTOR_VERTEX_LENGTH, countof(bigDotInds), zeroPos, NULL, 1, NULL);
	CrosshairBigDotID = insertElementIntoUI(CrosshairUI, passer);
}

void setupDynamicUI() {
	UIElement* passer = NULL;
	DynamicGameUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	DynamicGameUI->vecColour[0] = 0;
	DynamicGameUI->vecColour[1] = 1;
	DynamicGameUI->vecColour[2] = 0;
	DynamicGameUI->vecColour[3] = 1;

	char placeholder[] = "0";
	float textpos[] = {
		0,0,0,
	};

	//shield, 0
	float shieldPos[] = {
		-1,1,0,
	};
	UnfinObj string = createVecText(placeholder, textpos, 0.3);	
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, shieldPos, NULL, 1, NULL);
	passer->scale = 0.06;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//armour, 1
	float armourPos[] = {
		1,1,0,
	};
	string = createVecText(placeholder, textpos, 0.3);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, armourPos, NULL, 1, NULL);
	passer->scale = 0.06;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//speed, 2
	float speedPos[] = {
		0,-1,0,
	};
	string = createVecText(placeholder, textpos, DFS);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, speedPos, NULL, 1, NULL);
	passer->scale = DFS;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//hull, 3
	string = createVecText(placeholder, textpos, DFS);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, speedPos, NULL, 1, NULL);
	passer->scale = DFS;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//score, 4
	float scorePos[] = {
		0,1,0,
	};
	string = createVecText(placeholder, textpos, 0.06);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, scorePos, NULL, 1, NULL);
	passer->scale = 0.06;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//broadcast, 5, don't include in 'update dynamic ui' because it is handled in 'update broadcast'
	float broadcastPos[] = {
		0,-1,0,
	};
	string = createVecText(" ", textpos, DFS);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, broadcastPos, NULL, 1, NULL);
	passer->scale = DFS;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);

	//wave display, 6
	float wavePos[] = {
		0,0.6,0,
	};
	string = createVecText(" ", textpos, 0.05);
	passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, wavePos, NULL, 1, NULL);
	passer->scale = 0.05;
	insertElementIntoUI(DynamicGameUI, passer);
	freeUnfinObj(string);
}

void updateDynamicUISegment(UnfinObj newData, unsigned int currentEntry) {
	glBindVertexArray(DynamicGameUI->elements[currentEntry]->ID);
	glBindBuffer(GL_ARRAY_BUFFER, DynamicGameUI->elements[currentEntry]->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, DynamicGameUI->elements[currentEntry]->EBO);
	glBufferData(GL_ARRAY_BUFFER, newData.vLineCount * VERTEX_SIZE * VECTOR_VERTEX_LENGTH, newData.verts, GL_STREAM_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, newData.iCount * IND_SIZE, newData.indices, GL_STREAM_DRAW);
	DynamicGameUI->elements[currentEntry]->indexCount = newData.iCount;
	glBindVertexArray(0);
}

//wave display is for displaying 'Coming Wave: %d', if -1, no display
void updateDynamicUI(unsigned int waveDisplay) {
	//set up our vars
	char* mainString = calloc(20, sizeof(char));



	//Shield update
	float shieldTextPos[] = {
		1.20,-2.35,0,
	};
	_itoa(OurShip.shields/ SHIELD_DIVIDER, mainString, 10);
	strcat(mainString, "%");
	UnfinObj string = createVecText(mainString, shieldTextPos, 0.06);
	updateDynamicUISegment(string, 0);
	freeUnfinObj(string);

	//Armour update
	float armourTextPos[] = {
		-0.15,-2.35,0,
	};
	_itoa(OurShip.armour / ARMOUR_DIVIDER, mainString, 10);
	strcat(mainString, "%");
	armourTextPos[0] -= (strlen(mainString) * 1.05);
	string = createVecText(mainString, armourTextPos, 0.06);
	updateDynamicUISegment(string, 1);
	freeUnfinObj(string);

	//Speed update
	float speedTextPos[] = {
		11, 5.5, 0
	};
	int speed =
		(int)((sqrtf((OurShip.heading[X_pos] * OurShip.heading[X_pos])+
		(OurShip.heading[Y_pos] * OurShip.heading[Y_pos])+
		(OurShip.heading[Z_pos] * OurShip.heading[Z_pos])))*MS_DISP_MULT * 10);
	_itoa(speed, mainString, 10);
	//add the period to it
	if (speed > 999) {
		mainString[4] = mainString[3];
		mainString[3] = '.';
		mainString[5] = '\0';
	} else if (speed > 99) {
		mainString[3] = mainString[2];
		mainString[2] = '.';
		mainString[4] = '\0';
	} else {
		mainString[2] = mainString[1];
		mainString[1] = '.';
		mainString[3] = '\0';
	}
	//armourTextPos[0] -= (strlen(mainString) * 1.05);
	string = createVecText(mainString, speedTextPos, DFS);
	updateDynamicUISegment(string, 2);
	freeUnfinObj(string);

	//Hull update
	float hullTextPos[] = {
		17, 2, 0
	};
	_itoa(OurShip.hp / HULL_DIVIDER, mainString, 10);
	//armourTextPos[0] -= (strlen(mainString) * 1.05);
	string = createVecText(mainString, hullTextPos, DFS);
	updateDynamicUISegment(string, 3);
	freeUnfinObj(string);


	//Score update
	float scoreTextPos[] = {
		0, -1.25, 0
	};
	_itoa(score, mainString, 10);
	size_t scoreLen = strlen(mainString);
	for (size_t c = 1; c < scoreLen; c++) {
		scoreTextPos[0] -= 0.525;
	}
	//armourTextPos[0] -= (strlen(mainString) * 1.05);
	string = createVecText(mainString, scoreTextPos, 0.06);
	updateDynamicUISegment(string, 4);
	freeUnfinObj(string);

	static int currentlyDisplayingWave = 0;
	if (waveDisplay != -1 && currentlyDisplayingWave == 0) {
		//Wave update
		char waveString[35] = { "Wave " };
		float waveTextPos[] = {
			0, 0, 0
		};
		_ultoa_s((unsigned long)waveDisplay, mainString, 19, 10);
		strncat_s(waveString, 34, mainString, 8);
		strncat_s(waveString, 34, " Starting", 11);
		size_t waveStringLen = strlen(waveString);
		for (size_t c = 1; c < waveStringLen; c++) {
			waveTextPos[0] -= 0.525;
		}
		//armourTextPos[0] -= (strlen(mainString) * 1.05);
		string = createVecText(waveString, waveTextPos, 0.06);
		updateDynamicUISegment(string, 6);
		freeUnfinObj(string);

		currentlyDisplayingWave = 1;
	} else if(waveDisplay == -1 && currentlyDisplayingWave == 1){
		char waveString[2] = { " " };
		float waveTextPos[] = {
			0, 0, 0
		};
		string = createVecText(waveString, waveTextPos, 1);
		updateDynamicUISegment(string, 6);
		freeUnfinObj(string);
		currentlyDisplayingWave = 0;
	}
	

dynamicUpdateClosure:;
	free(mainString);
}

//returns -1 on end of broadcast, >0 otherwise
//Passing in -1 on broadcastSelected clears. (Technically, setting a really high progress could do that, but that's 'undefined')
long int updateBroadcast(int broadcastSelected, long int progressOnBroadcast) {
	//printf("Updating Broadcast at progress: %d\n", progressOnBroadcast);
	float broadcastTextPos[] = {
		-15,2,0,
	};
	//set all to a blank space, except the last entry
	char displayString[17] = { ' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ',' ', ' ',' ', ' ', 0, };

	//check if we need to just clear the broadcast
	if (broadcastSelected == -1) {
		UnfinObj string = createVecText(displayString, broadcastTextPos, 0.06);
		updateDynamicUISegment(string, 5);
		freeUnfinObj(string);
		return(-1);
	}

	//go through and fetch the correct character for the 16 character comms display
	for (int cChar = 0; cChar < 16; cChar++) {
		if (progressOnBroadcast + cChar - 16 < 0) {
			continue;
		}
		displayString[cChar] = commsLines[broadcastSelected][progressOnBroadcast + cChar - 16];
		//if we're into the dead space of the comm line, either break or realize we've completed the line
		if (displayString[cChar] == 0) {
			if (cChar == 0) {
				displayString[0] = 0;
				UnfinObj string = createVecText(displayString, broadcastTextPos, 0.06);
				updateDynamicUISegment(string, 5);
				freeUnfinObj(string);
				return(-1);
			}
			break;
		}
	}
	UnfinObj string = createVecText(displayString, broadcastTextPos, 0.06);
	updateDynamicUISegment(string, 5);
	freeUnfinObj(string);
	//printf("Updating Broadcast with string: %s\n", displayString);

	return(2);
}

long long int startGameButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		getsetGamestate(STARTING_GAME);
		glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
}

long long int settingsButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		getsetGamestate(IN_SETTINGS);
	}
}

long long int fullscreenButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		//printf("setting fullscreen");
		if (ShouldBeFullscreen == 1) {
			ShouldBeFullscreen = 0;
		} else {
			ShouldBeFullscreen = 1;
		}
	}
}

long long int returnToMenuButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		disableAllUILayers();
		MainMenuUI->active = 1;
		//updateBroadcast(-1, 0);
		getsetGamestate(IN_MAIN_MENU);
	}
}

long long int exitGameButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		exit(69);//ez
	}
}

long long int gotoKeysPageButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		getsetGamestate(IN_KEYSCREEN);
		disableAllUILayers();
		KeypageUI->active = 1;
		
	}
}

long long int dotSettingsButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		if (CrosshairUI->elements[CrosshairBigDotID]->elementActive == 1) {
			CrosshairUI->elements[CrosshairBigDotID]->elementActive = 0;
			CrosshairUI->elements[CrosshairSmallDotID]->elementActive = 1;
		} else if (CrosshairUI->elements[CrosshairSmallDotID]->elementActive == 1) {
			CrosshairUI->elements[CrosshairBigDotID]->elementActive = 0;
			CrosshairUI->elements[CrosshairSmallDotID]->elementActive = 0;
		} else {
			CrosshairUI->elements[CrosshairBigDotID]->elementActive = 1;
		}
	}
}

long long int wingsSettingButton(void* ourself, long long int data, short int clickData) {
	//UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	printf("In wings function\n");
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		if (CrosshairUI->elements[CrosshairWingsID]->elementActive == 1) {
			CrosshairUI->elements[CrosshairWingsID]->elementActive = 0;
		} else {
			CrosshairUI->elements[CrosshairWingsID]->elementActive = 1;
		}
	}
}


void expandedMouseClick(GLFWwindow* window, int button, int action, int mods) {
	defaultMoustClick(window, button, action, mods);

	if (button == GLFW_MOUSE_BUTTON_2) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[fireKey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[fireKey] = 0;
		}
	}
	else if (button == GLFW_MOUSE_BUTTON_1) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[fireKey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[fireKey] = 0;
		}
	}
}

void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	static int PauseMenuStatus = -1;
	if (key == GLFW_KEY_W) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[wkey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[wkey] = 0;
		}
	}

	if (key == GLFW_KEY_S) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[skey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[skey] = 0;
		}
	}

	if (key == GLFW_KEY_A) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[akey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[akey] = 0;
		}
	}

	if (key == GLFW_KEY_D) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[dkey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[dkey] = 0;
		}
	}

	if (key == GLFW_KEY_V) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[vkey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[vkey] = 0;
		}
	}

	if (key == GLFW_KEY_C) {
		if (action == GLFW_PRESS) {
			OurShip.keysHolding[ckey] = 1;
		} else if (action == GLFW_RELEASE) {
			OurShip.keysHolding[ckey] = 0;
		}
	}

	if (key == GLFW_KEY_Z) {
		OurShip.heading[X_pos] = 0;
		OurShip.heading[Y_pos]  = 0;
		OurShip.heading[Z_pos]  = 0;
	}

	if (key == GLFW_KEY_ESCAPE) {
		if (action == GLFW_PRESS) {
			int curGameState = getsetGamestate(DONT_STATE_CHANGE);
			if (curGameState == IN_GAME || curGameState == BETWEEN_WAVES) {
				PauseMenuStatus = curGameState; //save this for later
				getsetGamestate(PAUSE_MENU);
				glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
				PauseScreenUI->active = 1;
			}
			if (curGameState == PAUSE_MENU) {
				getsetGamestate(PauseMenuStatus);
				glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
				PauseScreenUI->active = 0;
			}
		}		
	}
}

void ourShipMotionHandler() {
	//up/down
	if (OurShip.keysHolding[wkey] == 1) {
		OurShip.heading[X_pos] -= gameworld.up[1] * OUR_ACCELERATION;
		OurShip.heading[Y_pos] -= gameworld.up[2] * OUR_ACCELERATION;
		OurShip.heading[Z_pos] -= gameworld.up[3] * OUR_ACCELERATION;
	}
	if (OurShip.keysHolding[skey] == 1) {
		OurShip.heading[X_pos] += gameworld.up[1] * OUR_ACCELERATION;
		OurShip.heading[Y_pos] += gameworld.up[2] * OUR_ACCELERATION;
		OurShip.heading[Z_pos] += gameworld.up[3] * OUR_ACCELERATION;
	}

	//left/right
	if (OurShip.keysHolding[akey] == 1) {
		OurShip.heading[X_pos] += gameworld.left[1] * OUR_ACCELERATION;
		OurShip.heading[Y_pos] += gameworld.left[2] * OUR_ACCELERATION;
		OurShip.heading[Z_pos] += gameworld.left[3] * OUR_ACCELERATION;
	}
	if (OurShip.keysHolding[dkey] == 1) {
		OurShip.heading[X_pos] -= gameworld.left[1] * OUR_ACCELERATION;
		OurShip.heading[Y_pos] -= gameworld.left[2] * OUR_ACCELERATION;
		OurShip.heading[Z_pos] -= gameworld.left[3] * OUR_ACCELERATION;
	}

	//forward/back
	if (OurShip.keysHolding[vkey] == 1) {
		OurShip.heading[X_pos] -= gameworld.back[1] * OUR_ACCELERATION * FORWARD_BACK_MULT;
		OurShip.heading[Y_pos] -= gameworld.back[2] * OUR_ACCELERATION * FORWARD_BACK_MULT;
		OurShip.heading[Z_pos] -= gameworld.back[3] * OUR_ACCELERATION * FORWARD_BACK_MULT;
	}
	if (OurShip.keysHolding[ckey] == 1) {
		OurShip.heading[X_pos] += gameworld.back[1] * OUR_ACCELERATION * FORWARD_BACK_MULT;
		OurShip.heading[Y_pos] += gameworld.back[2] * OUR_ACCELERATION * FORWARD_BACK_MULT;
		OurShip.heading[Z_pos] += gameworld.back[3] * OUR_ACCELERATION * FORWARD_BACK_MULT;
	}
	//printf("%f,%f,%f\n", OurShip.heading[X_pos], OurShip.heading[Y_pos], OurShip.heading[Z_pos]);
	
	gameworld.camera[X_pos] += OurShip.heading[X_pos];
	gameworld.camera[Y_pos] += OurShip.heading[Y_pos];
	gameworld.camera[Z_pos] += OurShip.heading[Z_pos];
	const float velocity = sqrtf((OurShip.heading[X_pos] * OurShip.heading[X_pos]) + (OurShip.heading[Y_pos] * OurShip.heading[Y_pos]) + (OurShip.heading[Z_pos] * OurShip.heading[Z_pos]));
	if (velocity > OUR_MAX_SPEED) {
		const float multiplier = (float)1 - (velocity - OUR_MAX_SPEED);
		OurShip.heading[X_pos] *= multiplier;
		OurShip.heading[Y_pos] *= multiplier;
		OurShip.heading[Z_pos] *= multiplier;
	}
}

void ourShipHandler() {
	ourShipMotionHandler();

	if (OurShip.keysHolding[fireKey] == 1) {
		float velocity[3] = { 
			(-gameworld.back[Y_pos] * BOOLETSPEED) + OurShip.heading[X_pos],
			(-gameworld.back[Z_pos] * BOOLETSPEED) + OurShip.heading[Y_pos],
			(-gameworld.back[W_pos] * BOOLETSPEED) + OurShip.heading[Z_pos],
		};
		float pos[7] = { 
			gameworld.camera[X_pos],
			gameworld.camera[Y_pos],
			gameworld.camera[Z_pos],
			gameworld.camera[W_pos],
			gameworld.camera[I_pos], 
			gameworld.camera[J_pos], 
			gameworld.camera[K_pos] };
		//no reorientating here because it's relative to the camera
		//quatMult(&gameworld.camera[W_pos], velocity);
		//temp = quatConj(&gameworld.camera[W_pos]);
		if (OurShip.timeSinceLastFire == 0) {
			pos[X_pos] -= gameworld.left[Y_pos] * OUR_BOOLET_OFFSET;
			pos[Y_pos] -= gameworld.left[Z_pos] * OUR_BOOLET_OFFSET;
			pos[Z_pos] -= gameworld.left[W_pos] * OUR_BOOLET_OFFSET;
			OurShip.timeSinceLastFire++;
			add_boolet(pos, velocity, BOOLETLIFE, PLAYER_BOOLET);
		}else if (OurShip.timeSinceLastFire == OUR_FIRE_RATE / 2) {
			pos[X_pos] += gameworld.left[Y_pos] * OUR_BOOLET_OFFSET;
			pos[Y_pos] += gameworld.left[Z_pos] * OUR_BOOLET_OFFSET;
			pos[Z_pos] += gameworld.left[W_pos] * OUR_BOOLET_OFFSET;
			add_boolet(pos, velocity, BOOLETLIFE, PLAYER_BOOLET);
			OurShip.timeSinceLastFire++;
		} else if (OurShip.timeSinceLastFire >= OUR_FIRE_RATE) {
			OurShip.timeSinceLastFire = 0;
		} else {
			OurShip.timeSinceLastFire++;
		}
		
	}//END OF FIRING

	//SHIELD MANAGEMENT
	if (OurShip.shields < MAX_SHIELDS) {
		if (OurShip.lastHits[FTHS - 2] != 0) {
			OurShip.shieldChargeTimeTracker = 0;
		}
		else if (OurShip.shieldChargeTimeTracker > SHIELD_RECHARGE_START) {
#ifdef SHIELD_RECHARGE_RATE_ABOVE_ONE
			OurShip.shields += SHIELD_RECHARGE_RATE;
#else
			OurShip.subShieldHP += SHIELD_RECHARGE_RATE;
			if (OurShip.subShieldHP > 1) {
				OurShip.shields++;
				OurShip.subShieldHP -= 1;
			}
#endif
		}
	}
	OurShip.shieldChargeTimeTracker++;
}

//call after updating our ship pls
void dirArrowHandler() {
	//gameworld.back is offset by 1 because it's a quaternion, remember
	DirectionalArrow->position[0] = gameworld.camera[0] - gameworld.back[1] + (gameworld.up[1] * 0.4715) + (gameworld.left[1] * 0.02);
	DirectionalArrow->position[1] = gameworld.camera[1] - gameworld.back[2] + (gameworld.up[2] * 0.4715) + (gameworld.left[2] * 0.02);
	DirectionalArrow->position[2] = gameworld.camera[2] - gameworld.back[3] + (gameworld.up[3] * 0.4715) + (gameworld.left[3] * 0.02);
}

EnShip* enemyShipHandler(volatile EnShip* enemyShipList, int upEnemyShips) {
	static int maxShipCount = MAX_ENEMY_COUNT;
	static int framesSinceLastSpawn = 0;

	int shipsToGoThrough = maxShipCount;
	if (upEnemyShips < maxShipCount && upEnemyShips > 0) {
		shipsToGoThrough = upEnemyShips;
	}

	//update AI, position, and ded status
	int allGone = 1; //A flag that gets set to zero if there are still ships left. If all ships ARE indeed gone, set the enemies left to -1 to indicate we are all done
	//printf("Ships to go through: %d\n", shipsToGoThrough);
	for (int cShip = 0; cShip < shipsToGoThrough; cShip++) {
		updateShipLifeStatus(&enemyShipList[cShip]);
		if (enemyShipList[cShip].currentlyDed == 0) {
			updateOurAI(&enemyShipList[cShip], OurShip, 1);
			enemyShipList[cShip].position[X_pos] += enemyShipList[cShip].forward[X_pos + 1] * enemyShipList[cShip].speed;
			enemyShipList[cShip].position[Y_pos] += enemyShipList[cShip].forward[Y_pos + 1] * enemyShipList[cShip].speed;
			enemyShipList[cShip].position[Z_pos] += enemyShipList[cShip].forward[Z_pos + 1] * enemyShipList[cShip].speed;
			allGone = 0;//we still have ships flyin about
		}
		if (enemyShipList[cShip].currentlyDed == 1 && framesSinceLastSpawn >= SPAWN_RATE && ENEMIES_PER_WAVE > 0) {
			framesSinceLastSpawn = 0;
			respawnShip(&enemyShipList[cShip]);
		}
		//respawnShipChecker(&enemyShipList[cShip]));
	}

	if (allGone == 1 && ENEMIES_PER_WAVE == 0) {
		ENEMIES_PER_WAVE = -1;
	}

	//respawn checker, end of update function
	framesSinceLastSpawn++;
	/*if (framesSinceLastSpawn >= SPAWN_RATE && ENEMIES_PER_WAVE > 0) {
		for (int cShip = 0; cShip < maxShipCount; cShip++) {		
			//It's greater than because people come out of the void of the negative Z's
			if (enemyShipList[cShip].position[Z_pos] > gameworld.camera[Z_pos] + (ENEMY_DISTANCE/2)) {
				
				passedEnemies++;
				score -= SCORE_LOSS_FOR_PASS;
				framesSinceLastSpawn = 0;
				ENEMIES_PER_WAVE--;//reduce since we're respawning a ship due to it going beyond the allotted region
				if (ENEMIES_PER_WAVE >= 0) {
					respawnShip(&enemyShipList[cShip]);
				} else {
					voidShip(&enemyShipList[cShip]);
				}			
				return(enemyShipList);
			}
			if (enemyShipList[cShip].hp < 1) {
				framesSinceLastSpawn = 0;			
				ENEMIES_PER_WAVE--;//reduce since we were ded
				if (ENEMIES_PER_WAVE >= 0) {
					respawnShip(&enemyShipList[cShip]);
				} else {
					voidShip(&enemyShipList[cShip]);
				}
				return(enemyShipList);
			}
		}
	} else if (ENEMIES_PER_WAVE == 0){
		int allGone = 1;
		for (int cShip = 0; cShip < maxShipCount; cShip++) {
			if (enemyShipList[cShip].position[Z_pos] > gameworld.camera[Z_pos] + (ENEMY_DISTANCE / 4)) {
				passedEnemies++;
				score -= SCORE_LOSS_FOR_PASS;
				killShip(&enemyShipList[cShip]);
			} else if (enemyShipList[cShip].hp < 1) {
				killShip(&enemyShipList[cShip]);
			} else {
				allGone = 0;
			}
		}
		if (allGone == 1) {
			ENEMIES_PER_WAVE = -1;
		}
	}*/
	return(enemyShipList);
}

//Checks if the ship needs to die, passed, etc.
void updateShipLifeStatus(EnShip* enemyShip) {
	//checking if the big ded
	if (enemyShip->hp <= 0 && enemyShip->currentlyDed != 1) {
		killShip(enemyShip);
		return;
	}

	//If we are alive, we need to check if we have passed the player by a lot so as to despawn and count that as a pass
	if (enemyShip->currentlyDed == 0) {
		//If there are still enemies to be spawned, despawning can wait a bit more
		if (ENEMIES_PER_WAVE > 0 && enemyShip->position[Z_pos] > gameworld.camera[Z_pos] + (ENEMY_DISTANCE / 2)) {
			passedShip(enemyShip);
		} else if (enemyShip->position[Z_pos] > gameworld.camera[Z_pos] + (ENEMY_DISTANCE / 4)) {
			passedShip(enemyShip);
		}
	}
	
}

//Counts as a kill and disables the ship
void killShip(EnShip* enemyShip) {
	score += SCORE_FOR_KILL;
	gameworld.renderObject[enemyShip->worldID] = 0;
	enemyShip->currentlyDed = 1;
	enemyShip->hp = 0;
}

//Counts as a spawn and enables the ship
void liveShip(EnShip* enemyShip) {
	ENEMIES_PER_WAVE--;
	gameworld.renderObject[enemyShip->worldID] = 1;
	enemyShip->currentlyDed = 0;
}

//disables the ship but does not count as a kill
void voidShip(EnShip* enemyShip) {
	gameworld.renderObject[enemyShip->worldID] = 0;
	enemyShip->currentlyDed = 1;
	enemyShip->hp = 0;
}

//disables the ship and counts as a passed ship
void passedShip(EnShip* enemyShip) {
	score -= SCORE_LOSS_FOR_PASS;
	gameworld.renderObject[enemyShip->worldID] = 0;
	enemyShip->currentlyDed = 1;
	enemyShip->hp = 0;
	passedEnemies++;
}

void setShip(EnShip* enemyShip) {
	//printf("Setting Ship\n");
	enShipStuff = getRefID(ENSHIP);
	enemyShip->ID = enShipStuff.ID;
	enemyShip->indexCount = enShipStuff.indC;
	enemyShip->scale = 0;
	enemyShip->hp = 0;
	enemyShip->currentlyDed = 1;
	enemyShip->targeting = 0;
	enemyShip->fireFrame = 0;
	enemyShip->speed = ENEMY_START_SPEED;
	enemyShip->forward[0] = 0;
	enemyShip->forward[1] = 0;
	enemyShip->forward[2] = 0;
	enemyShip->forward[3] = 1;	

	enemyShip->up[0] = 0;
	enemyShip->up[1] = 0;
	enemyShip->up[2] = 1;
	enemyShip->up[3] = 0;

	enemyShip->right[0] = 0;
	enemyShip->right[1] = -1;
	enemyShip->right[2] = 0;
	enemyShip->right[3] = 0;
}

void respawnShip(EnShip* enemyShip) {
	//printf("Resetting Ship\n");
	//setEnShipCollisionSet(enemyShip);
	liveShip(enemyShip);
	enemyShip->ID = enShipStuff.ID;
	//enemyShip->indexCount = enShipStuff.indC;
	//enemyShip->scale = 2;
	enemyShip->hp = ENEMY_HP;
	enemyShip->currentlyDed = 0;
	enemyShip->targeting = 0;
	enemyShip->fireFrame = 0;
	enemyShip->position[X_pos] = ((float)(rand() % ENEMY_POS_RANGE)- (ENEMY_POS_RANGE / 2))/ 50.0 + gameworld.camera[X_pos];
	enemyShip->position[Y_pos] = ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 50.0 + gameworld.camera[Y_pos];
	enemyShip->position[Z_pos] = -ENEMY_DISTANCE + ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 10.0 + gameworld.camera[Z_pos];
	enemyShip->position[W_pos] = 1;
	enemyShip->position[I_pos] = 0;
	enemyShip->position[J_pos] = 0;
	enemyShip->position[K_pos] = 0;

	enemyShip->forward[0] = 0;
	enemyShip->forward[1] = 0;
	enemyShip->forward[2] = 0;
	enemyShip->forward[3] = 1;

	enemyShip->up[0] = 0;
	enemyShip->up[1] = 0;
	enemyShip->up[2] = 1;
	enemyShip->up[3] = 0;

	enemyShip->right[0] = 0;
	enemyShip->right[1] = -1;
	enemyShip->right[2] = 0;
	enemyShip->right[3] = 0;

	enemyShip->speed = ENEMY_START_SPEED;

}

void gameCursorMovement() {
	double xpos, ypos;
	glfwGetCursorPos(gamewindow, &xpos, &ypos);
	if (xpos > MAX_MOUSE_MOVEMENT) {
		xpos = MAX_MOUSE_MOVEMENT;
	} else if (xpos < -MAX_MOUSE_MOVEMENT) {
		xpos = -MAX_MOUSE_MOVEMENT;
	}
	if (ypos > MAX_MOUSE_MOVEMENT) {
		ypos = MAX_MOUSE_MOVEMENT;
	} else if (ypos < -MAX_MOUSE_MOVEMENT) {
		ypos = -MAX_MOUSE_MOVEMENT;
	}

	//float orgCamPos[4] = {
	//	gameworld.camera[W_pos],gameworld.camera[I_pos],gameworld.camera[J_pos],gameworld.camera[K_pos],
	//};

	float rotateQuat[4] = { cos(xpos / MOUSE_MOVEMENT_DAMPER), 0 , 1 * sin(xpos / MOUSE_MOVEMENT_DAMPER), 0 };
	float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], -rotateQuat[2], -rotateQuat[3] };
	//normalizeQuat(&rotateQuat);
	//normalizeQuat(&rotQuatConj);
	quatMultRS(&gameworld.camera[W_pos],rotateQuat);
	//normalizeQuat(&ourWorld->camera[W_pos]);

	quatMultRS(&gameworld.up, rotateQuat);
	quatMult(rotQuatConj, &gameworld.up);
	//quatMult(rotateQuat, &gameworld.up);
	//quatMultRS(&gameworld.up, rotQuatConj);
	gameworld.up[0] = 0;

	quatMult(rotQuatConj, &gameworld.back);
	quatMultRS(&gameworld.back, rotateQuat);
	//printf("\n%f, %f, %f, %f\n", gameworld.back[0], gameworld.back[1], gameworld.back[2], gameworld.back[3]);
	gameworld.back[0] = 0;

	quatMult(rotQuatConj, gameworld.left);
	quatMultRS(gameworld.left, rotateQuat);	
	gameworld.left[0] = 0;
	//printf("\n%f, %f, %f, %f\n", gameworld.left[0], gameworld.left[1], gameworld.left[2], gameworld.left[3]);
	//printf("\n%f, %f, %f, %f\n", gameworld.camera[W_pos], gameworld.camera[I_pos], gameworld.camera[J_pos], gameworld.camera[K_pos]);

	
	float rotateQuatTwo[4] = { cos(ypos / MOUSE_MOVEMENT_DAMPER), 
		gameworld.left[1] * sin(ypos / MOUSE_MOVEMENT_DAMPER), 
		0* sin(ypos / MOUSE_MOVEMENT_DAMPER), 
		gameworld.left[3] * sin(ypos / MOUSE_MOVEMENT_DAMPER) };
	float rotQuatConjTwo[4] = { rotateQuatTwo[0], rotateQuatTwo[1] * ((double)-1), rotateQuatTwo[2] * ((double)-1), rotateQuatTwo[3] * ((double)-1) };
	//normalizeQuat(&rotateQuatTwo);
	//normalizeQuat(&rotQuatConjTwo);
	//quatMult(rotateQuatTwo, &gameworld.camera[W_pos]);
	quatMultRS(&gameworld.camera[W_pos], rotQuatConjTwo);
	//normalizeQuat(&ourWorld->camera[W_pos]);

	quatMult(rotateQuatTwo, &gameworld.up);
	quatMultRS(&gameworld.up, rotQuatConjTwo);
	gameworld.up[0] = 0;

	quatMult(rotateQuatTwo, &gameworld.back);
	quatMultRS(&gameworld.back, rotQuatConjTwo);
	//printf("\n%f, %f, %f, %f\n", gameworld.back[0], gameworld.back[1], gameworld.back[2], gameworld.back[3]);
	gameworld.back[0] = 0;

	quatMult(rotateQuatTwo, gameworld.left);
	quatMultRS(gameworld.left, rotQuatConjTwo);
	//printf("\n%f, %f, %f, %f\n", gameworld.left[0], gameworld.left[1], gameworld.left[2], gameworld.left[3]);
	gameworld.left[0] = 0;
	//printf("%f, %f, %f, %f\n\n", gameworld.camera[W_pos], gameworld.camera[I_pos], gameworld.camera[J_pos], gameworld.camera[K_pos]);
	
	//gameworld.camera[J_pos] = secondCamPos[2];
	//printf("%f, %f\n", xpos, ypos);
	glfwSetCursorPos(gamewindow, 0.0, 0.0);
}


void attemptToResetGameVariables() {
	passedEnemies = 0;
	score = 0;
}



//AUDIO HANDLING

//void (*realBulletAudioHandler)(int);

void setAudioFunctions(int audioSetting) {
	if (AudioSetting == NO_AUDIO) {
		realBulletAudioHandler = silentBulletHandler;//retroBulletAudioHandler;//noBulletAudioHandler;
	}

	else if (AudioSetting == DEFAULT_AUDIO_SETTING) {
		realBulletAudioHandler = silentBulletHandler;//retroBulletAudioHandler;//noBulletAudioHandler;
	}
}

void bulletAudioHandler(int newFrameUpdate) {
	realBulletAudioHandler(newFrameUpdate);
}
void silentBulletHandler(int newFrameUpdate) {
	if (newFrameUpdate == 0) {
		OurShip.lastHits[FTHS - 1]++;
		int totalHits = 0;
		for (int cHits = 0; cHits < FTHS; cHits++) {
			totalHits += OurShip.lastHits[cHits];
		}
	} else {
		for (int cHits = 0; cHits < FTHS - 1; cHits++) {
			OurShip.lastHits[cHits] = OurShip.lastHits[cHits + 1];
		}
		OurShip.lastHits[FTHS - 1] = 0;
	}
}

void retroBulletAudioHandler(int newFrameUpdate) {
	if (newFrameUpdate == 0) {
		OurShip.lastHits[FTHS - 1]++;
		int totalHits = 0;
		for (int cHits = 0; cHits < FTHS; cHits++) {
			totalHits += OurShip.lastHits[cHits];
		}
		if (totalHits < HITS_TO_PLAY_HITS) {
			return;
		}
		//if it has been long enough between sound queues
		if (GetTickCount64() > playingBulletSound + (unsigned long long)2100) {
			static unsigned long long timeSinceHitTracking = 0;
			volatile static AudioTrack blastLine = { 0 };
			volatile static unsigned int tones[7] = { 0 };
			volatile static unsigned int toneLengths[7] = { 0 };
			blastLine.tones = &tones;
			blastLine.tones[0] = Ab3;
			blastLine.tones[1] = Ab2;
			blastLine.tones[2] = Ab3;
			blastLine.tones[3] = Ab2;
			blastLine.tones[4] = Ab2;
			blastLine.tones[5] = Ab2;
			blastLine.tones[6] = 80;
			blastLine.toneLengths = &toneLengths;
			blastLine.toneLengths[0] = 80;
			blastLine.toneLengths[1] = 80;
			blastLine.toneLengths[2] = 80;
			blastLine.toneLengths[3] = 150;
			blastLine.toneLengths[4] = 100;
			blastLine.toneLengths[5] = 100;
			blastLine.toneLengths[6] = 100;
			blastLine.cPos = 0;
			blastLine.tracklen = 5;
			playingBulletSound = GetTickCount64();
			priorityTrackNewThread(&blastLine);
		}
	} else {
		for (int cHits = 0; cHits < FTHS - 1; cHits++) {
			OurShip.lastHits[cHits] = OurShip.lastHits[cHits + 1];
		}
		OurShip.lastHits[FTHS - 1] = 0;
	}
}





void debugCommands() {
	char input = 0;
	begin:;
	input = getche();
	if (input == 'a') {
		OurShip.armour -= 10;
	}
	else if (input == 'w') {
		ENEMIES_PER_WAVE = 0;
		gError("Clearing out spawn queue");
	}
	else if (input == 'k') {
		OurShip.hp = 0;
		gError("Attempting to naturally kill the player");
	}
	else if (input == 'K') {		
		glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		getsetGamestate(END_SCREEN);
		int temp = 0;
		setupEndscreen(&temp);
		OurShip.hp = 0;
		gError("Attempting to force kill the player");
	}
	else {
		gError("Invalid command");
	}
	goto begin;
}

