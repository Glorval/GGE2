#include "DataBase.h"

long long int testFunction(long long int data) {
	char* useable = &data;

	if (useable[0] == GLFW_MOUSE_BUTTON_1 && useable[1] == GLFW_PRESS) {
		printf("clicked\n");
	}
}

void startupDataBase(GLFWwindow* window) {
	programState = 0;
	UI* mainPage = createUI();
	
	float colours[] = { 1,1,1 };
	float coloursTwo[] = { 0.5,0.5,.5 };
	float coloursThree[] = { 0.1, 0.0, 0.6 };
	float pos[] = { 0,0,0 };
	float clickArea[] = { -130, -130 + 400, -70 - 60, -70};
	UnfinObj testText = createUnFinText("ABCDEFGHI\nabcdefghi\nJKLMNOPQR\njklmnopqr\nSTUVWXYZ\nstuvwxyz", -0.95, .8, 30, colours);
	UIElement* ourText = createElement(testText.verts, testText.indices, testText.vLineCount, testText.iCount, pos, NULL, NULL, 0, 1, NULL);
	insertElementIntoUI(mainPage, ourText);

	
	testText = createUnFinText("Imma button", convFromPixelX(-100), convFromPixelY(-100), 40, coloursThree);

	UnfinObj buttonthingy = createButton(-130, -70, 400, 60, 5, colours, coloursTwo);
	buttonthingy = mergeUnfinisheds(buttonthingy, testText);
	UIElement* buttonthingyCompleter = createElement(buttonthingy.verts, buttonthingy.indices, buttonthingy.vLineCount, buttonthingy.iCount, pos, testFunction, NULL, ACTION, 1, clickArea);
	
	insertElementIntoUI(mainPage, buttonthingyCompleter);

	masterUIList = calloc(1, sizeof(UI*));
	masterUIList[0] = mainPage;
	masterUIListLength = 1;
}

UnfinObj createButton(float xIn, float yIn, float xScaleIn, float yScaleIn, float spacingIn, float* baseRGB, float* innerRGB) {
	float x = convFromPixelX(xIn);
	float y = convFromPixelY(yIn);
	float xScale = convFromPixelX(xScaleIn);
	float yScale = convFromPixelY(yScaleIn);
	float spacingX = convFromPixelX(spacingIn);
	float spacingY = convFromPixelY(spacingIn);

	float button[] = {
		x, y,										0.02, baseRGB[0], baseRGB[1], baseRGB[2],//0 
		x + xScale, y,						0.02, baseRGB[0], baseRGB[1], baseRGB[2],//1 
		x, y - yScale,						0.02, baseRGB[0], baseRGB[1], baseRGB[2],//2 
		x + xScale, y - yScale,			0.02, baseRGB[0], baseRGB[1], baseRGB[2],//3 

		x + spacingX, y - spacingY,								0.01, innerRGB[0], innerRGB[1], innerRGB[2],//4 
		x - spacingX + xScale, y - spacingY,					0.01, innerRGB[0], innerRGB[1], innerRGB[2],//5 
		x + spacingX, y - yScale + spacingY,					0.01, innerRGB[0], innerRGB[1], innerRGB[2],//6 
		x - spacingX + xScale, y - yScale + spacingY,		0.01, innerRGB[0], innerRGB[1], innerRGB[2],//7
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