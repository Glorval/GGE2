#include "LibertyBlaster.h"
#include "Entities.c"
#include "booletmanager.c"


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
		glLineWidth(1);
		glDisable(GL_LINE_SMOOTH);
		enemyShipList = calloc(DEFAULT_ENEMY_MAX, sizeof(EnShip));
		enShipStuff = getRefID(ENSHIP);
		for (int cShip = 0; cShip < DEFAULT_ENEMY_MAX; cShip++) {
			//enemyShipList[cShip].ID = enShipStuff.ID;
			//enemyShipList[cShip].indexCount = enShipStuff.indC;
			setShip(&enemyShipList[cShip]);
			insertObjectIntoWorld(&gameworld, &enemyShipList[cShip], 1);
		}
		setupBoolet();
		OurShip.fireRate = OUR_FIRE_RATE;
		gameworld.camera[Z_pos] = 10000;
		MainMenuUI->active = 0;
		getsetGamestate(IN_GAME);
	}else if (flagSetting == IN_GAME) {
		for (int cShip = 0; cShip < DEFAULT_ENEMY_MAX; cShip++) {
			facePoint(&enemyShipList[cShip], OurShip.heading);
		}
		
		gameCursorMovement();
		ourShipHandler();
		updateBoolets(enemyShipList, DEFAULT_ENEMY_MAX);
		runMasterUI();
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
	return(&gameworld);
}

void setupMainMenu() {	
	//glLineWidth(1);
	//glEnable(GL_LINE_SMOOTH);
	masterUIList = calloc(1, sizeof(UI*));
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
	float buttontextpos[] = { 0,0,0 };
	
	UnfinObj buttontext = createVecText("START GAME", buttontextpos, 0.05);
	insertElementIntoUI(MainMenuUI, createVectorElement(gameStartButton, gameStartInds, (sizeof(gameStartButton) / sizeof(float)) / VECTOR_VERTEX_LENGTH, sizeof(gameStartInds) / sizeof(unsigned int), pos, startGameButton, 1, clickarea));
	passer = createVectorElement(buttontext.verts, buttontext.indices, buttontext.vLineCount, buttontext.iCount, pos, NULL, 1, NULL);
	passer->position[X_pos] = -0.18;
	passer->position[Y_pos] = -0.325;
	passer->scale = 0.051;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(buttontext);


	float titlePos[] = { -0.58,0.3,-0.1 };
	UnfinObj title = createVecText("BLAZING\nLIBERTY", titlePos, 0.3);
	passer = createVectorElement(title.verts, title.indices, title.vLineCount, title.iCount, titlePos, NULL, 1, NULL);
	passer->scale = 0.3;
	insertElementIntoUI(MainMenuUI, passer);
	freeUnfinObj(title);






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
			add_boolet(pos, velocity, BOOLETLIFE);
		}else if (OurShip.timeSinceLastFire == OUR_FIRE_RATE / 2) {
			pos[X_pos] += gameworld.left[Y_pos] * OUR_BOOLET_OFFSET;
			pos[Y_pos] += gameworld.left[Z_pos] * OUR_BOOLET_OFFSET;
			pos[Z_pos] += gameworld.left[W_pos] * OUR_BOOLET_OFFSET;
			add_boolet(pos, velocity, BOOLETLIFE);
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

	//update position
	for (int cShip = 0; cShip < maxShipCount; cShip++) {
		enemyShipList[cShip].position[X_pos] += enemyShipList[cShip].heading[X_pos];
		enemyShipList[cShip].position[Y_pos] += enemyShipList[cShip].heading[Y_pos];
		enemyShipList[cShip].position[Z_pos] += enemyShipList[cShip].heading[Z_pos];

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
	enemyShip->forward[0] = 0;
	enemyShip->forward[1] = 0;
	enemyShip->forward[2] = 0;
	enemyShip->forward[3] = 1;
}

void resetShipVariation(EnShip* enemyShip) {
	//printf("Resetting Ship\n");
	//setEnShipCollisionSet(enemyShip);
	enemyShip->ID = enShipStuff.ID;
	//enemyShip->indexCount = enShipStuff.indC;
	//enemyShip->scale = 2;
	enemyShip->hp = ENEMY_HP_DEFAULT;
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

	enemyShip->heading[X_pos] = 0;
	enemyShip->heading[Y_pos] = 0;
	enemyShip->heading[Z_pos] = ENEMY_START_SPEED;
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



void facePoint(EnShip* us, float targetPos[3]) {
	float forwardVector[3] = {
		(gameworld.camera[X_pos] + (OurShip.heading[X_pos] * 20))- us->position[X_pos],
		(gameworld.camera[Y_pos] + (OurShip.heading[Y_pos] * 20)) - us->position[Y_pos],
		(gameworld.camera[Z_pos] + (OurShip.heading[Z_pos] * 20)) - us->position[Z_pos],
	};

	norm3(forwardVector);

	float dot = dotP3(&us->forward[1], forwardVector);
	
	if (fabsf(dot + 1.0) < 0.00001 || fabsf(dot - 1.0) < 0.00001) {
	} else {
		if (dot >= 1 || dot <= -1) {
			//printf("saved");
			return;
		}
		float rotAngle = acosf(dot);
		float* rotAxis = crossP3(&us->forward[1], forwardVector);
		norm3(rotAxis);
		//printf("%f, %f, %f, %f\n", rotAngle, rotAxis[0], rotAxis[1], rotAxis[2]);
		float ha = rotAngle / (float)20.0;
		float sina = sinf(ha);
		float rotQuat[] = {
			cosf(ha),
			rotAxis[0] * sina,
			rotAxis[1] * sina,
			rotAxis[2] * sina,
		};

		normalizeQuat(rotQuat);

		float conj[] = {
			rotQuat[0],
			-rotQuat[1],
			-rotQuat[2],
			-rotQuat[3],
		};
		normalizeQuat(conj);
		quatMult(rotQuat, &us->position[W_pos]);

		quatMult(rotQuat, us->forward);
		quatMultRS(us->forward, conj);
	}
}