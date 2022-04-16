#include "DataBase.h"

long long int testFunction(long long int data) {
	int* useable = &data;
	if (useable[0] == GLFW_MOUSE_BUTTON_1 && useable[1] == GLFW_PRESS) {
		printf("clicked\n");
	}
}

void startupDataBase(GLFWwindow* window) {
	UI* mainPage = createUI();
	

	UnfinObj testText = createUnFinText("ABCDEFGHI\nabcdefghi\nJKLMNOPQR\njklmnopqr\nSTUV", -0.95, .8, 30);
	UIElement* ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);

	testText = createUnFinText("ABCDEFGHI\nabcdefghi\nJKLMNOPQR\njklmnopqr\nSTUV", -0.55, 0.8, 200);
	ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);


	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = mainPage;
	masterUIListLength = 1;
}