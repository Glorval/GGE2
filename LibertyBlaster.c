#include "LibertyBlaster.h"
#include "Entities.c"

struct ourShip OurShip = { 0 };

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
		enemyShipList = calloc(DEFAULT_ENEMY_MAX, sizeof(EnShip));
		for (int cShip = 0; cShip < DEFAULT_ENEMY_MAX; cShip++) {
			resetShipVariation(&enemyShipList[cShip]);
			insertObjectIntoWorld(&gameworld, &enemyShipList[cShip], 1);
		}
		getsetGamestate(IN_GAME);
	}else if (flagSetting == IN_GAME) {
		drawWorld(&gameworld);
		
		runMasterUI();

		gameCursorMovement();		
		ourShipMotionHandler();
		enemyShipList = enemyShipHandler(enemyShipList, 0);
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
	return(&gameworld);
}

void setupMainMenu() {	
	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = calloc(1, sizeof(UI));
	MainMenuUI->active = 1;
	MainMenuUI->renderMode = RENDER_MODE_VECT_POS_ONLY;
	MainMenuUI->vecColour[0] = 0;
	MainMenuUI->vecColour[1] = 1;
	MainMenuUI->vecColour[2] = 0;
	MainMenuUI->vecColour[3] = 1;
	float gameStartButton[] = {
		-0.3, 0.06, -0.5,
		0.3, 0.06, -0.5,
		0.3, -0.06, -0.5,
		-0.3, -0.06, -0.5,
	};
	unsigned int gameStartInds[] = {
		0,1,2,1, 2,3, 3,0
	};
	float pos[] = { 0,0.3,0};
	float clickarea[] = { 0.2,0.2,0.06,0.06 };
	insertElementIntoUI(MainMenuUI, createVectorElement(gameStartButton, gameStartInds, (sizeof(gameStartButton) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(gameStartInds) / sizeof(unsigned int), pos, startGameButton, 1, clickarea));
	masterUIListLength++;
}

long long int startGameButton(void* ourself, long long int data, short int clickData) {
	UIElement* us = ourself;
	us->elementActive = 0;
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
		OurShip.heading[X_pos] -= gameworld.up[1] * .0001;
		OurShip.heading[Y_pos] -= gameworld.up[2] * .0001;
		OurShip.heading[Z_pos] -= gameworld.up[3] * .0001;
	}
	if (OurShip.keysHolding[skey] == 1) {
		OurShip.heading[X_pos] += gameworld.up[1] * .0001;
		OurShip.heading[Y_pos] += gameworld.up[2] * .0001;
		OurShip.heading[Z_pos] += gameworld.up[3] * .0001;
	}

	//left/right
	if (OurShip.keysHolding[akey] == 1) {
		OurShip.heading[X_pos] += gameworld.left[1] * .0001;
		OurShip.heading[Y_pos] += gameworld.left[2] * .0001;
		OurShip.heading[Z_pos] += gameworld.left[3] * .0001;
	}
	if (OurShip.keysHolding[dkey] == 1) {
		OurShip.heading[X_pos] -= gameworld.left[1] * .0001;
		OurShip.heading[Y_pos] -= gameworld.left[2] * .0001;
		OurShip.heading[Z_pos] -= gameworld.left[3] * .0001;
	}

	//forward/back
	if (OurShip.keysHolding[vkey] == 1) {
		OurShip.heading[X_pos] -= gameworld.back[1] * .0003;
		OurShip.heading[Y_pos] -= gameworld.back[2] * .0003;
		OurShip.heading[Z_pos] -= gameworld.back[3] * .0003;
	}
	if (OurShip.keysHolding[ckey] == 1) {
		OurShip.heading[X_pos] += gameworld.back[1] * .0003;
		OurShip.heading[Y_pos] += gameworld.back[2] * .0003;
		OurShip.heading[Z_pos] += gameworld.back[3] * .0003;
	}
	//printf("%f,%f,%f\n", OurShip.heading[X_pos], OurShip.heading[Y_pos], OurShip.heading[Z_pos]);
	//add a max heading


	//OurShip.heading[Velocity] = ()
	gameworld.camera[X_pos] += OurShip.heading[X_pos];
	gameworld.camera[Y_pos] += OurShip.heading[Y_pos];
	gameworld.camera[Z_pos] += OurShip.heading[Z_pos];
}

EnShip* enemyShipHandler(EnShip* enemyShipList, int upEnemyShips) {
	static int maxShipCount = DEFAULT_ENEMY_MAX;
	static int framesSinceLastSpawn = 0;
	static int shipSpawnSpeed = STARTING_SPAWN_SPEED;
	
	if (enemyShipList == NULL) {
		enemyShipList = calloc(maxShipCount, sizeof(EnShip));
	}
	if (upEnemyShips != 0) {
		maxShipCount = upEnemyShips;
		enemyShipList = realloc(enemyShipList, maxShipCount * sizeof(EnShip));
	}

	for (int cShip = 0; cShip < maxShipCount; cShip++) {
		enemyShipList[cShip].position[X_pos] += enemyShipList[cShip].heading[X_pos];
		enemyShipList[cShip].position[Y_pos] += enemyShipList[cShip].heading[Y_pos];
		enemyShipList[cShip].position[Z_pos] += enemyShipList[cShip].heading[Z_pos];
	}

	framesSinceLastSpawn++;
	if (framesSinceLastSpawn >= shipSpawnSpeed) {
		for (int cShip = 0; cShip < maxShipCount; cShip++) {		
			//It's greater than because people come out of the void of the negative Z's
			if (enemyShipList[cShip].position[Z_pos] > gameworld.camera[Z_pos] + ENEMY_DISTANCE) {
				framesSinceLastSpawn = 0;
				resetShip(&enemyShipList[cShip]);
				return(enemyShipList);
			}
			if (enemyShipList[cShip].hp == 0) {
				framesSinceLastSpawn = 0;
				resetShip(&enemyShipList[cShip]);
				return(enemyShipList);
			}
		}
	}
	return(enemyShipList);
}

void resetShip(EnShip* enemyShip) {
	printf("Resetting Ship\n");
	static char weHaveFound = 0;
	static idSet enShipStuff;
	if (weHaveFound != 0) {
		enemyShip->ID = enShipStuff.ID;
		enemyShip->indexCount = enShipStuff.indC;
		enemyShip->hp = ENEMY_HP_DEFAULT;
		enemyShip->position[X_pos] = 0;
		enemyShip->position[Y_pos] = 0;
		enemyShip->position[Z_pos] = -ENEMY_DISTANCE;
		enemyShip->position[W_pos] = 1;
		enemyShip->position[I_pos] = 0;
		enemyShip->position[J_pos] = 0;
		enemyShip->position[K_pos] = 0;

		enemyShip->heading[X_pos] = 0;
		enemyShip->heading[Y_pos] = 0;
		enemyShip->heading[Z_pos] = ENEMY_START_SPEED;
	} else {
		enShipStuff = getRefID(ENSHIP);
		weHaveFound++;
		enemyShip->ID = enShipStuff.ID;
		enemyShip->indexCount = enShipStuff.indC;
		enemyShip->hp = ENEMY_HP_DEFAULT;
		enemyShip->position[X_pos] = 0;
		enemyShip->position[Y_pos] = 0;
		enemyShip->position[Z_pos] = -ENEMY_DISTANCE;
		enemyShip->position[W_pos] = 1;
		enemyShip->position[I_pos] = 0;
		enemyShip->position[J_pos] = 0;
		enemyShip->position[K_pos] = 0;

		enemyShip->heading[X_pos] = 0;
		enemyShip->heading[Y_pos] = 0;
		enemyShip->heading[Z_pos] = ENEMY_START_SPEED;
	}
}

void resetShipVariation(EnShip* enemyShip) {
	printf("Resetting Ship\n");
	static char weHaveFound = 0;
	static idSet enShipStuff;
	if (weHaveFound != 0) {
		enemyShip->ID = enShipStuff.ID;
		enemyShip->indexCount = enShipStuff.indC;
		enemyShip->hp = ENEMY_HP_DEFAULT;
		enemyShip->position[X_pos] = ((float)(rand() % ENEMY_POS_RANGE)- (ENEMY_POS_RANGE / 2))/ 50.0;
		enemyShip->position[Y_pos] = ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 50.0;
		enemyShip->position[Z_pos] = -ENEMY_DISTANCE + ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 10.0;
		enemyShip->position[W_pos] = 1;
		enemyShip->position[I_pos] = 0;
		enemyShip->position[J_pos] = 0;
		enemyShip->position[K_pos] = 0;

		enemyShip->heading[X_pos] = 0;
		enemyShip->heading[Y_pos] = 0;
		enemyShip->heading[Z_pos] = ENEMY_START_SPEED;
	} else {
		enShipStuff = getRefID(ENSHIP);
		weHaveFound++;
		enemyShip->ID = enShipStuff.ID;
		enemyShip->indexCount = enShipStuff.indC;
		enemyShip->hp = ENEMY_HP_DEFAULT;
		enemyShip->position[X_pos] = ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 50.0;
		enemyShip->position[Y_pos] = ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 50.0;
		enemyShip->position[Z_pos] = -ENEMY_DISTANCE + ((float)(rand() % ENEMY_POS_RANGE) - (ENEMY_POS_RANGE / 2)) / 5.0;
		enemyShip->position[W_pos] = 1;
		enemyShip->position[I_pos] = 0;
		enemyShip->position[J_pos] = 0;
		enemyShip->position[K_pos] = 0;

		enemyShip->heading[X_pos] = 0;
		enemyShip->heading[Y_pos] = 0;
		enemyShip->heading[Z_pos] = ENEMY_START_SPEED;
	}
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