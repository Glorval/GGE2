#include "DataBase.h"

Block testFunction(Block data) {
	int* useable = &data;
	//if (useable[0] == GLFW_MOUSE_BUTTON_1 && useable[1] == GLFW_PRESS) {
		printf("clicked\n");
	//}
	
	
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

	float uivertstwo[] = {
		-1.0f, 0.5f, 0.0f, 0.8f,0.5f,0.8f,
		0.75f, 0.5f, 0.0f, 0.8f, 0.5f, 0.8f,
		0.75f, 0.75f, 0.0f, 0.8f, 0.5f, 0.8f,
		-1.0f, 0.75f, 0.0f, 0.8f, 0.5f, 0.8f,
	};
	float elementTwoClickArea[4] = { -1.0f, 0.75f, 0.5f, 0.75f };


	UI* mainPage = createUI();
	UIElement* first = createElement(uiverts, uiinds, 4, 6, testFunction, 1, elementOneClickArea);
	insertElementIntoUI(mainPage, first);
	UIElement* second = createElement(uivertstwo, uiinds, 4, 6, testFunction, 1, elementTwoClickArea);
	insertElementIntoUI(mainPage, second);

	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = mainPage;
	masterUIListLength = 1;
}