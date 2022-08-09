#include "LibertyBlaster.h"
#include "booletmanager.c"
#include "Entities.c"
//#include "shipAIManager.h"


struct ourShip OurShip = { 0 };
idSet enShipStuff = { 0 };

int getsetGamestate(int flag) {
	static int ourState = IN_MAIN_MENU;
	if (flag != DONT_STATE_CHANGE) {
		ourState = flag;
	}
	return ourState;
}

void runGame(GLFWwindow* window, int flagSetting) {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	volatile static EnShip* enemyShipList = NULL;


	if (flagSetting == IN_MAIN_MENU) {
		runMasterUI();
	}
	else if (flagSetting == STARTING_GAME) {
		glLineWidth(1);//just in case, make sure we're in the right mode
		glDisable(GL_LINE_SMOOTH);

		//set up enemy ship list
		enemyShipList = calloc(DEFAULT_ENEMY_MAX, sizeof(EnShip));
		enShipStuff = getRefID(ENSHIP);
		for (int cShip = 0; cShip < DEFAULT_ENEMY_MAX; cShip++) {
			//enemyShipList[cShip].ID = enShipStuff.ID;
			//enemyShipList[cShip].indexCount = enShipStuff.indC;
			setShip(&enemyShipList[cShip]);
			insertObjectIntoWorld(&gameworld, &enemyShipList[cShip], 1);
		}

		//initialize boolet system
		setupBoolet();

		//set up a couple of our own ship's things
		OurShip.fireRate = OUR_FIRE_RATE;
		gameworld.camera[Z_pos] = 10000;

		//disable the main menu UI
		MainMenuUI->active = 0;

		//enable the in game main UI
		BaseGameUI->active = 1;

		//select the next game state
		getsetGamestate(IN_GAME);
	}else if (flagSetting == IN_GAME) {
		
		gameCursorMovement();
		ourShipHandler();
		updateBoolets(enemyShipList, DEFAULT_ENEMY_MAX);
		runMasterUI();
		updateDynamicUI();
		drawWorld(&gameworld);		
		enemyShipList = enemyShipHandler(enemyShipList, 0);
	} else if (flagSetting == IN_SETTINGS) {

	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}


World* loadGame() {
	gamewindow = startup(expandedMouseClick, keypressHandler);

	masterObjLenght = 0;
	masterObjList = NULL;
	gameworld = createWorld(VECTOR_WORLD);
	gameworld.vecColour[0] = 0;
	gameworld.vecColour[1] = 1;
	gameworld.vecColour[2] = 0;
	gameworld.vecColour[3] = 1;
	
	loadEnemyShip();
	setupMasterUIList();
	setupMainMenu();
	setupGameUI();
	return(&gameworld);
}

void setupMasterUIList() {
	masterUIList = calloc(UICount, sizeof(UI*));
}

void setupMainMenu() {	
	//glLineWidth(1);
	//glEnable(GL_LINE_SMOOTH);
	
	masterUIList[0] = calloc(1, sizeof(UI));
	masterUIListLength++;
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
	unsigned int gameStartInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	float pos[] = { 0,-0.3,0};
	float clickarea[] = { 0.2,0.2,0.06,0.06 };
	float buttontextpos[] = { -4.725,0,0 };
	
	UnfinObj buttontext = createVecText("START GAME", buttontextpos, 0.05);
	insertElementIntoUI(MainMenuUI, createVectorElement(gameStartButton, gameStartInds, (sizeof(gameStartButton) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(gameStartInds) / sizeof(unsigned int), pos, startGameButton, 1, clickarea));
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	//passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.325;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);


	float titlePos[] = { -3.15, 0.0,0 };
	float titleScreenpos[] = { 0, 0.3, 0 };
	UnfinObj title = createVecText("BLAZING\nLIBERTY", titlePos, 0.3);
	//UnfinObj title = createVecText("ABCDEFG\n01234\n56789", titlePos, 0.15);
	passer = createVectorElement(title.verts, title.indices, title.vLineCount, title.iCount, titleScreenpos, NULL, 1, NULL);
	passer->scale = 0.15;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(title);
}

void setupGameUI() {
	BaseGameUI = calloc(1, sizeof(UI));
	masterUIListLength++;
	BaseGameUI->active = 0;
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

	const float ammoTextPos[] = {
		-16, 6, 0
	};
	
	UnfinObj ammo = createVecText("AMMUNITION:", ammoTextPos, 0.0255);
	passer = createVectorElement(ammo.verts, ammo.indices, ammo.vLineCount, ammo.iCount, bottomAnchor, NULL, 1, NULL);
	passer->scale = 0.0255;
	insertElementIntoUI(BaseGameUI, passer);
	freeUnfinObj(ammo);

	const float speedTextPos[] = {
		4.5, 6, 0
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
	UnfinObj comms = createVecText("COMMS:", commsTextPos, 0.0255);//Note, there's going to be a lil light to the right of this that makes the unevenness okay
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
}

void setupDynamicUI() {
	char* ourString = calloc(20, sizeof(char));
	float shieldTextPos[] = {
		0,0,0,
	};
	float shieldPos[] = {
		0,0,0,
	};

	UnfinObj string = createVecText(ourString, shieldTextPos, 0.3);
	/*passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, shieldPos, NULL, 1, NULL);
	passer->scale = 0.3;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(string);*/

	free(ourString);
}

void updateDynamicUI() {
	char* ourString = calloc(20, sizeof(char));



	_itoa(OurShip.shields, ourString, 10);

	UIElement* passer = NULL;

	float shieldTextPos[] = {
		0,0,0,
	};
	float shieldPos[] = {
		0,0,0,
	};

	UnfinObj string = createVecText(ourString, shieldTextPos, 0.3);
	/*passer = createVectorElement(string.verts, string.indices, string.vLineCount, string.iCount, shieldPos, NULL, 1, NULL);
	passer->scale = 0.3;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(string);*/


dynamicUpdateClosure:;
	free(ourString);
}







long long int startGameButton(void* ourself, long long int data, short int clickData) {
	UIElement* us = ourself;
	//us->elementActive = 0;
	char* clickdat = &clickData;
	char* ourData = &data;
	//standard left click
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		getsetGamestate(STARTING_GAME);
		glfwSetInputMode(gamewindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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
}


void keypressHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
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
	float tempos[] = {
		0,0,0,1,0,0,0
	};
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
		float* temp = NULL;
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
		
	}
}

EnShip* enemyShipHandler(EnShip* enemyShipList, int upEnemyShips) {
	static int maxShipCount = DEFAULT_ENEMY_MAX;
	static int framesSinceLastSpawn = 0;
	static int shipSpawnSpeed = STARTING_SPAWN_SPEED;
	
	/*if (enemyShipList == NULL) {
		enemyShipList = calloc(maxShipCount, sizeof(EnShip));
	}*/
	if (upEnemyShips != 0) {
		maxShipCount = upEnemyShips;
		enemyShipList = realloc(enemyShipList, maxShipCount * sizeof(EnShip));
	}

	//update AI and position
	for (int cShip = 0; cShip < maxShipCount; cShip++) {
		updateOurAI(&enemyShipList[cShip], OurShip, 1);
		enemyShipList[cShip].position[X_pos] += enemyShipList[cShip].forward[X_pos + 1] * enemyShipList[cShip].speed;
		enemyShipList[cShip].position[Y_pos] += enemyShipList[cShip].forward[Y_pos + 1] * enemyShipList[cShip].speed;
		enemyShipList[cShip].position[Z_pos] += enemyShipList[cShip].forward[Z_pos + 1] * enemyShipList[cShip].speed;

		if (enemyShipList[cShip].hp == 0) {
			voidShip(&enemyShipList[cShip]);
		}
	}

	//respawn checker, end of update function
	framesSinceLastSpawn++;
	if (framesSinceLastSpawn >= shipSpawnSpeed) {
		for (int cShip = 0; cShip < maxShipCount; cShip++) {		
			//It's greater than because people come out of the void of the negative Z's
			if (enemyShipList[cShip].position[Z_pos] > gameworld.camera[Z_pos] + ENEMY_DISTANCE) {
				framesSinceLastSpawn = 0;
				resetShipVariation(&enemyShipList[cShip]);
				return(enemyShipList);
			}
			if (enemyShipList[cShip].hp < 1) {
				framesSinceLastSpawn = 0;
				resetShipVariation(&enemyShipList[cShip]);
				return(enemyShipList);
			}
		}
	}
	return(enemyShipList);
}


void voidShip(EnShip* enemyShip) {
	//printf("Voiding Ship\n");
	enemyShip->position[X_pos] = -100000;
	enemyShip->position[Y_pos] = -100000;
	enemyShip->position[Z_pos] = 100000;
}

void setShip(EnShip* enemyShip) {
	//printf("Setting Ship\n");
	enShipStuff = getRefID(ENSHIP);
	enemyShip->ID = enShipStuff.ID;
	//enemyShip->scale = 2;
	enemyShip->indexCount = enShipStuff.indC;
	enemyShip->hp = 0;
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

void resetShipVariation(EnShip* enemyShip) {
	//printf("Resetting Ship\n");
	//setEnShipCollisionSet(enemyShip);
	enemyShip->ID = enShipStuff.ID;
	//enemyShip->indexCount = enShipStuff.indC;
	//enemyShip->scale = 2;
	enemyShip->hp = ENEMY_HP_DEFAULT;
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

	float secondCamPos[4] = {
		gameworld.camera[W_pos],gameworld.camera[I_pos],gameworld.camera[J_pos],gameworld.camera[K_pos],
	};
	
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



