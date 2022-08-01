#include "LibertyBlaster.h"
#include "Entities.c"

#define IN_MAIN_MENU 0
#define IN_GAME 1

void runGame(GLFWwindow* window, int flagSetting) {
	if (flagSetting == IN_MAIN_MENU) {
		runMasterUI();
		
	}
	else if (flagSetting == IN_GAME) {
		drawWorld(&gameworld);
		gameworld.objects[0]->position[Z_pos] += 0.2;
		runMasterUI();
	}
	glfwSwapBuffers(window);
	glfwPollEvents();
}


World* loadGame() {
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
		printf("here");
		Object* testship = calloc(1, sizeof(Object));
		testship->ID = masterObjList[0].ID;
		testship->indexCount = masterObjList[0].indlen;
		testship->position[W_pos] = 1;
		testship->position[Z_pos] = -10;
		insertObjectIntoWorld(&gameworld, testship, 1);
	}
}