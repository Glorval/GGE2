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
	if (flagSetting == IN_MAIN_MENU) {
		runMasterUI();
	}
	else if (flagSetting == IN_GAME) {
		drawWorld(&gameworld);
		//gameworld.objects[0]->position[Z_pos] += 0.2;
		runMasterUI();

		gameCursorMovement();		
		ourShipMotionHandler();
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
		Object* testship = calloc(1, sizeof(Object));
		testship->ID = masterObjList[0].ID;
		testship->indexCount = masterObjList[0].indlen;
		testship->position[W_pos] = 1;
		testship->position[Z_pos] = -1;
		testship->position[Y_pos] = 0.5;
		insertObjectIntoWorld(&gameworld, testship, 1);

		
		Object*  ship2 = calloc(1, sizeof(Object));
		ship2->ID = masterObjList[0].ID;
		ship2->indexCount = masterObjList[0].indlen;
		ship2->position[W_pos] = 1;
		ship2->position[Z_pos] = 2;
		ship2->position[Y_pos] = 0;
		insertObjectIntoWorld(&gameworld, ship2, 1);

		Object* ship3 = calloc(1, sizeof(Object));
		ship3->ID = masterObjList[0].ID;
		ship3->indexCount = masterObjList[0].indlen;
		ship3->position[W_pos] = 1;
		ship3->position[Z_pos] = 0;
		ship3->position[Y_pos] = 0;
		ship3->position[X_pos] = 2;
		insertObjectIntoWorld(&gameworld, ship3, 1);
		getsetGamestate(IN_GAME);
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

	float orgCamPos[4] = {
		gameworld.camera[W_pos],gameworld.camera[I_pos],gameworld.camera[J_pos],gameworld.camera[K_pos],
	};

	float rotateQuat[4] = { cos(xpos / MOUSE_MOVEMENT_DAMPER), 0 , 1 * sin(xpos / MOUSE_MOVEMENT_DAMPER), 0 };
	float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], -rotateQuat[2], -rotateQuat[3] };
	//normalizeQuat(&rotateQuat);
	//normalizeQuat(&rotQuatConj);
	quatMultRS(&gameworld.camera[W_pos],rotateQuat);
	//normalizeQuat(&ourWorld->camera[W_pos]);

	quatMult(rotateQuat, &gameworld.up);
	quatMultRS(&gameworld.up, rotQuatConj);
	gameworld.up[0] = 0;

	quatMult(rotQuatConj, &gameworld.back);
	quatMultRS(&gameworld.back, rotateQuat);
	//printf("\n%f, %f, %f, %f\n", gameworld.back[0], gameworld.back[1], gameworld.back[2], gameworld.back[3]);
	gameworld.back[0] = 0;

	quatMult(rotQuatConj, gameworld.left);
	quatMultRS(gameworld.left, rotateQuat);	
	gameworld.left[0] = 0;
	//printf("\n%f, %f, %f, %f\n", gameworld.left[0], gameworld.left[1], gameworld.left[2], gameworld.left[3]);
	printf("\n%f, %f, %f, %f\n", gameworld.camera[W_pos], gameworld.camera[I_pos], gameworld.camera[J_pos], gameworld.camera[K_pos]);

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

	//quatMult(rotateQuatTwo, gameworld.left);
	//quatMultRS(gameworld.left, rotQuatConjTwo);
	//printf("\n%f, %f, %f, %f\n", gameworld.left[0], gameworld.left[1], gameworld.left[2], gameworld.left[3]);
	//gameworld.left[0] = 0;
	//printf("%f, %f, %f, %f\n\n", gameworld.camera[W_pos], gameworld.camera[I_pos], gameworld.camera[J_pos], gameworld.camera[K_pos]);
	
	gameworld.camera[J_pos] = secondCamPos[2];
	//printf("%f, %f\n", xpos, ypos);
	glfwSetCursorPos(gamewindow, 0.0, 0.0);
}