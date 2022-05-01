#include "DataBase.h"
#include <string.h>
#include <process.h>
#include <windows.h>
#include "parsing.h"

void clearTempStuff() {
	for (int cElement = 0; cElement < temporaryStuff->elementCount; cElement++) {
		free(temporaryStuff->elements[cElement]);
		temporaryStuff->elements[cElement] = NULL;
	}
	temporaryStuff->elementCount = 0;
}

//to be ran in its own thread whilst waiting for the data to be set to clear it.
void clearTempAfterExecute(volatile long long int* data) {
	while (*data != 0) {
	}
	clearTempStuff();
	return;
}

long long int pageSwitcher(long long int data, short int clickData) {
	char* clickdat = &clickData;
	char* ourData = &data;
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		printf("We were clicked. Desired page: %d\n", ourData[OUR_DESIRED_PAGE]);
		printf("Data %lld\n", data);
		printf("Data chunks %d, %d, %d\n", clickdat[0], clickdat[1], ourData[7]);

		//disable all other pages except the base
		for (int cPage = 1; cPage < PAGECOUNT; cPage++) {
			if (masterUIList[cPage] != NULL) {//safety
				masterUIList[cPage]->active = 0;
			}
		}

		clearTempStuff();
		

		//enable our desired page
		switch (ourData[OUR_DESIRED_PAGE]) {
			case REPORTONE:
				//create the page upon the temporary page
				accessKeysReportCreation(masterUIList[TEMPSTUFF]);
				masterUIList[TEMPSTUFF]->active = 1;
				programState = ON_REPORT;
				break;
			case REPORTTWO:
				//create the page upon the temporary page
				backendReportCreation(masterUIList[TEMPSTUFF]);
				masterUIList[TEMPSTUFF]->active = 1;
				programState = ON_REPORT;
				break;
			case REPORTTHREE:
				//create the page upon the temporary page
				responsesReportCreation(masterUIList[TEMPSTUFF]);
				masterUIList[TEMPSTUFF]->active = 1;
				programState = ON_REPORT;
				break;
			default:
				//load the premade page and display the temp page in case there needs to be dynamically done stuff
				masterUIList[ourData[OUR_DESIRED_PAGE]]->active = 1;
				masterUIList[TEMPSTUFF]->active = 1;
				programState = 0;
				break;
		}	
		
		
	}


	return(data);
}

long long int accessKeyUploadSelect(long long int data) {
	char* filePath = fileSelector();
	clearTempStuff();
	float textColour[] = { 0,0,0 };
	float position[] = {
		-1 + convFromPixelX(100),
		 1 - convFromPixelY(520),
		 0.8
	};
	float buttonBaseColour[] = { 189.0 / 255, 115.0 / 255, 58.0 / 255, };
	float buttonInnerColour[] = { 211.0 / 255, 207.0 / 255, 193.0 / 255, };
	float clickarea[] = { 
		-(windX/2) + 100, //left
		-(windX / 2) + 320, //right
		-185, //bottom
		-126, //top
	};

	char displayedText[512] = "File:\n";
	strcat(displayedText, filePath);

	UnfinObj text = createUnFinText(displayedText, convFromPixelX(28), convFromPixelY(16), 32, textColour);
	UnfinObj button = createButton(0, -56, 220, 60, 5, buttonBaseColour, buttonInnerColour);
	UnfinObj buttonText = createUnFinText("Confirm?", convFromPixelX(28), convFromPixelY(-86), 32, textColour);
	text = mergeUnfinisheds(text, button);
	text = mergeUnfinisheds(text, buttonText);

	//THIS IS THE DIFFERENTIATING LINE, the button we create here has a function attached to it for the specific upload
	UIElement* displayedPath = createElement(text.verts, text.indices, text.vLineCount, text.iCount, position, accessKeyUpload, NULL, ACTION, 1, clickarea);
	displayedPath->data = (long long int)filePath;
	insertElementIntoUI(masterUIList[TEMPSTUFF], displayedPath);

	return(data);
}

long long int backendUploadSelect(long long int data, short int clickData) {
	char* filePath = fileSelector();
	clearTempStuff();
	float textColour[] = { 0,0,0 };
	float position[] = {
		-1 + convFromPixelX(100),
		 1 - convFromPixelY(520),
		 0.8
	};
	float buttonBaseColour[] = { 189.0 / 255, 115.0 / 255, 58.0 / 255, };
	float buttonInnerColour[] = { 211.0 / 255, 207.0 / 255, 193.0 / 255, };
	float clickarea[] = {
		-(windX / 2) + 100, //left
		-(windX / 2) + 320, //right
		-185, //bottom
		-126, //top
	};

	char displayedText[512] = "File:\n";
	strcat(displayedText, filePath);

	UnfinObj text = createUnFinText(displayedText, convFromPixelX(28), convFromPixelY(16), 32, textColour);
	UnfinObj button = createButton(0, -56, 220, 60, 5, buttonBaseColour, buttonInnerColour);
	UnfinObj buttonText = createUnFinText("Confirm?", convFromPixelX(28), convFromPixelY(-86), 32, textColour);
	text = mergeUnfinisheds(text, button);
	text = mergeUnfinisheds(text, buttonText);

	//THIS IS THE DIFFERENTIATING LINE, the button we create here has a function attached to it for the specific upload
	UIElement* displayedPath = createElement(text.verts, text.indices, text.vLineCount, text.iCount, position, backendUpload, NULL, ACTION, 1, clickarea);
	displayedPath->data = (long long int)filePath;
	insertElementIntoUI(masterUIList[TEMPSTUFF], displayedPath);

	return(data);
}

long long int  surveyUploadSelect(long long int data, short int clickData) {
	char* filePath = fileSelector();
	clearTempStuff();
	float textColour[] = { 0,0,0 };
	float position[] = {
		-1 + convFromPixelX(100),
		 1 - convFromPixelY(520),
		 0.8
	};
	float buttonBaseColour[] = { 189.0 / 255, 115.0 / 255, 58.0 / 255, };
	float buttonInnerColour[] = { 211.0 / 255, 207.0 / 255, 193.0 / 255, };
	float clickarea[] = {
		-(windX / 2) + 100, //left
		-(windX / 2) + 320, //right
		-185, //bottom
		-126, //top
	};

	char displayedText[512] = "File:\n";
	strcat(displayedText, filePath);

	UnfinObj text = createUnFinText(displayedText, convFromPixelX(28), convFromPixelY(16), 32, textColour);
	UnfinObj button = createButton(0, -56, 220, 60, 5, buttonBaseColour, buttonInnerColour);
	UnfinObj buttonText = createUnFinText("Confirm?", convFromPixelX(28), convFromPixelY(-86), 32, textColour);
	text = mergeUnfinisheds(text, button);
	text = mergeUnfinisheds(text, buttonText);

	//THIS IS THE DIFFERENTIATING LINE, the button we create here has a function attached to it for the specific upload
	UIElement* displayedPath = createElement(text.verts, text.indices, text.vLineCount, text.iCount, position, surveyUpload, NULL, ACTION, 1, clickarea);
	displayedPath->data = (long long int)filePath;
	insertElementIntoUI(masterUIList[TEMPSTUFF], displayedPath);

	return(data);
}

long long int accessKeyUpload(long long int data, short int clickData) {
	//data parsing and upload can go here
	char* clickdat = &clickData;
	char* ourData = &data;
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		char* string = data;

		parseAccessKeys(string);

		_beginthread(clearTempAfterExecute, 0, &data);
		free(string);
		data = 0;
	}
	return(0);
}

long long int backendUpload(long long int data, short int clickData) {
	//data parsing and upload can go here
	char* clickdat = &clickData;
	char* ourData = &data;
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		char* string = data;

		parseSurveyData(string);

		//the cursed line
		_beginthread(clearTempAfterExecute, 0, &data);
		free(string);
		data = 0;
	}
	return(0);
}

long long int surveyUpload(long long int data, short int clickData) {
	//data parsing and upload can go here
	char* clickdat = &clickData;
	char* ourData = &data;
	if (clickdat[0] == GLFW_MOUSE_BUTTON_1 && clickdat[1] == GLFW_PRESS) {
		char* string = data;

		parseResponseData(string);

		//the cursed line
		_beginthread(clearTempAfterExecute, 0, &data);
		free(string);
		data = 0;
	}
	return(0);
}

void startupDataBase(GLFWwindow* window) {
	connectToDatabase();

	glfwSetKeyCallback(window, ourButtonPresses);

	programState = 0;
	masterUIList = calloc(10, sizeof(UI*));
	masterUIListLength = PAGECOUNT;

	temporaryStuff = createUI();
	temporaryStuff->active = 1;
	masterUIList[TEMPSTUFF] = temporaryStuff;

	//Main page creation, will have all the page switching buttons, background, etc.
	UI* mainPage = createUI();
	mainPage->active = 1;
	float curPos[] = { 0,0,0 };
	float curClick[] = { 0, 0, 0, 0 };
	float zeroPos[] = { 0,0,0 };
	float textColour[] = { 0, 0, 0.2 };
	float buttonBaseColour[] = {189.0/255, 115.0 / 255, 58.0 / 255, };
	float buttonInnerColour[] = { 211.0 / 255, 207.0 / 255, 193.0 / 255, };//{ 157.0 / 255, 199.0 / 255, 225.0 / 255, };
	float emblem[] = {
		-1, 1,																					0.1, 0.83, 0.81, 0.75,//0 
		-1 + convFromPixelX(150), 1,												0.1, 0.83, 0.81, 0.75,//1 
		-1, 1 - convFromPixelY(150),												0.1, 0.83, 0.81, 0.75,//2 
		-1 + convFromPixelX(150), 1 - convFromPixelY(150),			0.1, 0.83, 0.81, 0.75,//3 

		-1 + convFromPixelX(75), 1 - convFromPixelY(140),			0.0, 50.0/255.0, 175.0/255, 1,//4 
		-1 + convFromPixelX(25), 1 - convFromPixelY(110),			0.0, 50.0 / 255, 175.0 / 255, 1,//5
		-1 + convFromPixelX(125), 1 - convFromPixelY(110),			0, 50.0 / 255.0, 175.0 / 255, 1,//6  
		-1 + convFromPixelX(25), 1 - convFromPixelY(25),			0, 50.0 / 255.0, 175.0 / 255, 1,//7  
		-1 + convFromPixelX(125), 1 - convFromPixelY(25),			0, 50.0 / 255.0, 175.0 / 255, 1,//8  

		-1 + convFromPixelX(100), 1 - convFromPixelY(15),			0, 50.0 / 255.0, 175.0 / 255, 1,//9  
		-1 + convFromPixelX(50), 1 - convFromPixelY(15),			0, 50.0 / 255.0, 175.0 / 255, 1,//10  
	};
	unsigned int emblemInds[] = {
		0,1,2, 1,2,3,//background
		4,5,6, 5,6,7, 6,7,8, 7,8,9, 7,9,10,//shield base
	};
	float rgbEmblemText[] = {0.7, 0.1,0.1};
	UnfinObj emblemBase = createUnfinObjFromStatic(emblem, emblemInds, 11, sizeof(emblemInds) / sizeof(unsigned int));
	emblemBase = mergeUnfinishedsFreeing(emblemBase, createUnFinTextWithZ("S", -1 + convFromPixelX(64), 1 - convFromPixelY(60), -0.01, 64, rgbEmblemText));
	emblemBase = mergeUnfinishedsFreeing(emblemBase, createUnFinTextWithZ("V", -1 + convFromPixelX(80), 1 - convFromPixelY(90), -0.01, 64, rgbEmblemText));
	insertElementIntoUI(mainPage, createElement(emblemBase.verts, emblemBase.indices, emblemBase.vLineCount, emblemBase.iCount, zeroPos, NULL, NULL, NULL, 1, NULL));

	float background[] = {
		-1, 1, .9, 73.0/255.0, 102.0/255.0, 156.0 / 255.0,
		1, 1, .9, 73.0 / 255.0, 102.0 / 255.0, 156.0 / 255.0,
		-1, -1, .9, 73.0 / 255.0, 102.0 / 255.0, 156.0 / 255.0,
		1, -1, .9, 73.0 / 255.0, 102.0 / 255.0, 156.0 / 255.0,
		// 212.0 / 255.0, 163.0 / 255.0, 1,
	};
	unsigned int backgroundInds[] = {
		0,1,2, 1,2,3,
	};
	insertElementIntoUI(mainPage, createElement(background, backgroundInds, 4, 6, zeroPos, NULL, NULL, NULL, 1, NULL));

	
															
	UnfinObj text = createUnFinText("Update Access", convFromPixelX(32), convFromPixelY(-30), 32, textColour);
	UnfinObj buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(150);
	curPos[Y_pos] = 1;
	curPos[Z_pos] = .1;
	curClick[0] = 150 - (windX/2);
	curClick[1] = 500 - (windX / 2);
	curClick[2] = (windY / 2) - 60;
	curClick[3] = (windY / 2);
	UIElement* curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	char* curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = UPONE;
	insertElementIntoUI(mainPage, curElement);
	
	text = createUnFinText("Update Backend", convFromPixelX(28), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(500);
	curPos[Y_pos] = 1;
	curPos[Z_pos] = .1;
	curClick[0] = 500 - (windX / 2);
	curClick[1] = 850 - (windX / 2);
	curClick[2] = (windY / 2) - 60;
	curClick[3] = (windY / 2);
	curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = UPTWO;
	insertElementIntoUI(mainPage, curElement);

	text = createUnFinText("Update Answers", convFromPixelX(28), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(850);
	curPos[Y_pos] = 1;
	curPos[Z_pos] = .1;
	curClick[0] = 850 - (windX / 2);
	curClick[1] = (windX / 2);
	curClick[2] = (windY / 2) - 60;
	curClick[3] = (windY / 2);
	curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = UPTHREE;
	insertElementIntoUI(mainPage, curElement);


	text = createUnFinText("View Accesses", convFromPixelX(32), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(150);
	curPos[Y_pos] = 1 - convFromPixelY(60);
	curPos[Z_pos] = .1;
	curClick[0] = -(windX/2) + 150;
	curClick[1] = -(windX / 2) + 500;
	curClick[2] = (windY / 2) - 120;
	curClick[3] = (windY / 2) - 60;
	curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = REPORTONE;
	insertElementIntoUI(mainPage, curElement);

	text = createUnFinText("View Backend", convFromPixelX(48), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(500);
	curPos[Y_pos] = 1 - convFromPixelY(60);
	curPos[Z_pos] = .1;
	curClick[0] = -(windX / 2) + 500;
	curClick[1] = -(windX / 2) + 850;
	curClick[2] = (windY / 2) - 120;
	curClick[3] = (windY / 2) - 60;
	curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = REPORTTWO;
	insertElementIntoUI(mainPage, curElement);

	text = createUnFinText("View Answers", convFromPixelX(48), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinisheds(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(850);
	curPos[Y_pos] = 1 - convFromPixelY(60);
	curPos[Z_pos] = .1;
	curClick[0] = -(windX / 2) + 850;
	curClick[1] = (windX / 2);
	curClick[2] = (windY / 2) - 120;
	curClick[3] = (windY / 2) - 60;
	curElement = createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, pageSwitcher, NULL, ACTION, 1, curClick);
	curData = &curElement->data;
	curData[OUR_DESIRED_PAGE] = REPORTTHREE;
	insertElementIntoUI(mainPage, curElement);
		
	masterUIList[MAINPAGE] = mainPage;
	
	//End of Main Screen

	//Splash screen
	UI* splashScreen = createUI();
	splashScreen->active = 1;

	text = createUnFinText("   Sir Vey\nAdministrative\n  Interface", convFromPixelX(20), convFromPixelX(-40), 80, zeroPos);
	curPos[X_pos] = -0.6;
	curPos[Y_pos] = 0.2;
	curPos[Z_pos] = 0.1;
	insertElementIntoUI(splashScreen, createElement(text.verts, text.indices, text.vLineCount, text.iCount, curPos, NULL, NULL, NO_ACTION, 1, NULL));

	masterUIList[SPLASHSCREEN] = splashScreen;


	//Upload One
	UI* upone = createUI();
	upone->active = 0;

	text = createUnFinText("Select File", convFromPixelX(48), convFromPixelY(-30), 32, textColour);
	buttonBase = createButton(0, 0, 350, 60, 5, buttonBaseColour, buttonInnerColour);
	buttonBase = mergeUnfinishedsFreeing(buttonBase, text);
	curPos[X_pos] = -1 + convFromPixelX(100);
	curPos[Y_pos] = 1 - convFromPixelY(400);
	curPos[Z_pos] = .1;
	curClick[0] = 100 - (windX / 2);
	curClick[1] = 450 - (windX / 2);
	curClick[2] = (windY / 2) - 460;
	curClick[3] = (windY / 2) - 400;
	insertElementIntoUI(upone, createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, accessKeyUploadSelect, NULL, ACTION, 1, curClick));

	//Upload Two, copies a lot of stuff from upload one
	UI* uptwo = createUI();
	uptwo->active = 0;
	insertElementIntoUI(uptwo, createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, backendUploadSelect, NULL, ACTION, 1, curClick));

	//Upload Three, copies a lot of stuff from upload one
	UI* upthree = createUI();
	upthree->active = 0;
	insertElementIntoUI(upthree, createElement(buttonBase.verts, buttonBase.indices, buttonBase.vLineCount, buttonBase.iCount, curPos, surveyUploadSelect, NULL, ACTION, 1, curClick));

	masterUIList[UPONE] = upone;
	masterUIList[UPTWO] = uptwo;
	masterUIList[UPTHREE] = upthree;
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





char* fileSelector() {
	int ret = 1;
	wchar_t path[256] = { 0 };
	OPENFILENAMEW ofn = {
		.lStructSize = sizeof(ofn),
		.lpstrFile = path,
		.nMaxFile = sizeof(path),
	};

	if (GetOpenFileNameW(&ofn)) {
		DWORD n = lstrlenW(path);
		//HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		//ret = !WriteConsoleW(h, path, n, &n, 0);
	}

	char* charPath = calloc(256, sizeof(char));
	for (int c = 0; c < 256; c++) {
		charPath[c] = path[c];
	}
	return(charPath);
}


void ourButtonPresses(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (key == GLFW_KEY_UP && (action == GLFW_PRESS || action == GLFW_REPEAT) && programState == ON_REPORT) {
		masterUIList[TEMPSTUFF]->elements[0]->position[1] -= 0.035;
	}
	else if (key == GLFW_KEY_DOWN && (action == GLFW_PRESS || action == GLFW_REPEAT) && programState == ON_REPORT) {
		masterUIList[TEMPSTUFF]->elements[0]->position[1] += 0.035;
	}
}
