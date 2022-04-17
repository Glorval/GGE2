#include "DataBase.h"

long long int testFunction(long long int data) {
	int* useable = &data;
	if (useable[0] == GLFW_MOUSE_BUTTON_1 && useable[1] == GLFW_PRESS) {
		printf("clicked\n");
	}
}

void startupDataBase(GLFWwindow* window) {
	UI* mainPage = createUI();
	
	float colours[] = { 1,1,1 };
	float coloursTwo[] = { 0.5,0.5,.5 };
	float coloursThree[] = { 0.1, 0.0, 0.6 };
	float pos[] = { 0,0,0 };
	UnfinObj testText = createUnFinText("ABCDEFGHI\nabcdefghi\nJKLMNOPQR\njklmnopqr\nSTUVWXYZ\nstuvwxyz", -0.95, .8, 30, colours);
	UIElement* ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, pos, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);
	
	/*testText = createUnFinText("ABCDEFGHI\nabcdefghi\nJKLMNOPQR\njklmnopqr\nSTUVWXYZ\nstuvwxyz", -0.6, 0.8, 80,colours);
	ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, pos, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);

	testText = createUnFinText("So this is how the\nfont looks thus far", -0.9, -.5, 80, colours);
	ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, pos, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);*/
	
	testText = createUnFinText("Imma button", 0.04, -0.1, 27, coloursThree);

	UnfinObj buttonthingy = createButton(0, 0, 0.4, 0.2, 0.01, colours, coloursTwo);
	buttonthingy = mergeUnfinisheds(buttonthingy, testText);
	UIElement* buttonthingyCompleter = createElement(buttonthingy.verts, buttonthingy.indices, buttonthingy.vLineCount, buttonthingy.iCount, pos, NULL, NULL, 0, 1, NULL);
	
	insertElementIntoUI(mainPage, buttonthingyCompleter);

	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = mainPage;
	masterUIListLength = 1;
}

UnfinObj createButton(float x, float y, float xScale, float yScale, float spacing, float* baseRGB, float* innerRGB) {
	float button[] = {
		x, y,										0.02, baseRGB[0], baseRGB[1], baseRGB[2],//0 
		x + xScale, y,						0.02, baseRGB[0], baseRGB[1], baseRGB[2],//1 
		x, y - yScale,						0.02, baseRGB[0], baseRGB[1], baseRGB[2],//2 
		x + xScale, y - yScale,			0.02, baseRGB[0], baseRGB[1], baseRGB[2],//3 

		x + spacing, y - spacing,								0.01, innerRGB[0], innerRGB[1], innerRGB[2],//4 
		x - spacing + xScale, y - spacing,					0.01, innerRGB[0], innerRGB[1], innerRGB[2],//5 
		x + spacing, y - yScale + spacing,					0.01, innerRGB[0], innerRGB[1], innerRGB[2],//6 
		x - spacing + xScale, y - yScale + spacing,		0.01, innerRGB[0], innerRGB[1], innerRGB[2],//7
	};
	unsigned int inds[] = {
		0,1,2, 1,2,3,
		4,5,6, 5,6,7,
	};
	UnfinObj returnObj = { 0 };
	returnObj.iCount = 12;
	returnObj.vLineCount = 8;
	returnObj.verts = calloc(8 * VERTEX_LENGTH, VERTEX_SIZE);
	returnObj.indices = calloc(12, IND_SIZE);
	for (int cv = 0; cv < 8 * VERTEX_LENGTH; cv++) {
		returnObj.verts[cv] = button[cv];
	}
	for (int ci = 0; ci < 12; ci++) {
		returnObj.indices[ci] = inds[ci];
	}
	return(returnObj);
}