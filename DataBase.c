#include "DataBase.h"

long long int testFunction(long long int data) {
	int* useable = &data;
	if (useable[0] == GLFW_MOUSE_BUTTON_1 && useable[1] == GLFW_PRESS) {
		printf("clicked\n");
	}
}

void startupDataBase(GLFWwindow* window) {
	

	float uiverts[] = {
		-0.5f, -0.5f, 0.0f, 0.5f,0.5f,0.5f,
		0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
		0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
		-0.5f, 0.5f, 0.0f, 0.5f, 0.5f, 0.5f,
	};
	unsigned int uiinds[] = { 0, 1, 2, 0, 2, 3 };
	float elementOneClickArea[4] = {	-0.5f, 0.5f, -0.5f, 0.5f	};

	float A[] = {
			0, 1, 0, 0, 0, 0,//0  
			0, 0.9, 0, 0, 0 ,0,//1
			-0.8, -1, 0, 0, 0, 0,//2
			-0.6, -1,0, 0, 0, 0,//3
			0.8, -1, 0, 0, 0, 0,//4
			0.6, -1, 0, 0, 0, 0,//5
			0.32, 0.05, 0, 0, 0, 0,//6
			0.35, -0.05, 0, 0, 0, 0,//7
			-0.32, 0.05, 0, 0, 0, 0,//8
			-0.35, -0.05, 0, 0, 0, 0//9
	};
	float Ainds[] = {
		0, 2, 3,	0, 1, 3,//left bar
		0, 4, 5,	0, 1, 5,//right bar
		6, 7, 8,	6, 7, 9//middle bar
	};


	UI* mainPage = createUI();
	UIElement* first = createElement(uiverts, uiinds, 4, 6, testFunction, NULL, ACTION, 1, elementOneClickArea);
	//insertElementIntoUI(mainPage, first);
	//UIElement* second = createElement(A, Ainds, 10, 18, NULL, NULL, NO_ACTION, 1, NULL);
	//insertElementIntoUI(mainPage, second);

	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = mainPage;
	masterUIListLength = 0;
}