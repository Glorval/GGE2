#pragma once
#include "GGE2_1.h"
#include "ObjectManager.h"
#include "GlorwynUtilities.h"
#include <math.h>



GLFWwindow* startup(void* clickfunc, void* keypressfunc) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


	GLFWwindow* window = glfwCreateWindow(windX, windY, APPLICATIONNAME, NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD failed to load.");
	}

	glViewport(0, 0, windX, windY);

	ProgramData.shaderID = setupShaders();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_CLAMP);

	ProgramData.cordinatesLoc = glGetUniformLocation(ProgramData.shaderID, "cordinates");
	ProgramData.orientationLoc = glGetUniformLocation(ProgramData.shaderID, "orientation");
	ProgramData.scaleLoc = glGetUniformLocation(ProgramData.shaderID, "scale");
	ProgramData.perspectiveLoc = glGetUniformLocation(ProgramData.shaderID, "perspective");
	ProgramData.cameraPosLoc = glGetUniformLocation(ProgramData.shaderID, "cameraPosition");
	ProgramData.camAngleLoc = glGetUniformLocation(ProgramData.shaderID, "cameraOrientation");
	ProgramData.flagsLoc = glGetUniformLocation(ProgramData.shaderID, "flags");
	ProgramData.colourLoc = glGetUniformLocation(ProgramData.shaderID, "vectorColours");
	ProgramData.windowRatioLoc = glGetUniformLocation(ProgramData.shaderID, "windowRatio");

	float aspect = windX / windY;
	//float fov = 45;
	float perspective[4][4];
	float f = 1000;
	float n = 0.1;

	float tanHalfFovy = tan(FOV / 2);

	for (int c = 0; c < 4; c++) {
		for (int a = 0; a < 4; a++) {
			perspective[c][a] = 0;
		}
	}

	perspective[0][0] = 1 / (aspect * tanHalfFovy);
	perspective[1][1] = 1 / (tanHalfFovy);
	perspective[2][2] = f / (n - f);
	perspective[2][3] = -1.0;
	perspective[3][2] = -((f * n) / (f - n));
	glUniformMatrix4fv(ProgramData.perspectiveLoc, 1, 0, perspective);
	glUniform1f(ProgramData.windowRatioLoc, ((float)windY / (float)windX));

	glfwSetWindowSizeCallback(window, window_resize_handler);

	if (clickfunc != NULL) {
		glfwSetMouseButtonCallback(window, clickfunc);//defaultMoustClick);
	} else {
		glfwSetMouseButtonCallback(window, defaultMoustClick);
	}
	
	if (keypressfunc != NULL) {
		glfwSetKeyCallback(window, keypressfunc);//defaultMoustClick);
	} else {
		glfwSetKeyCallback(window, defaultButtonPress);
	}


	//int cameraDefault[4] = { 0, 0, 1, 0 };
	//glUniformMatrix4fv(ProgramData.cameraLoc, 1, 0, cameraDefault);

	setupUI(USE_BUILT_IN_VECTOR_FONT);
	return(window);
}

int setupShaders() {
	//Vertex shader
	char* fileData = NULL;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fileData = readFile("VShader.glsl");
	if (fileData == NULL) {
		gError("Vertex Shader failed to read.");
		return(-1);
	}
	glShaderSource(vertexShader, 1, &fileData, NULL);
	glCompileShader(vertexShader);


	//Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fileData = readFile("FShader.glsl");
	if (fileData == NULL) {
		gError("Vertex Shader failed to read.");
		return(-1);
	}
	glShaderSource(fragmentShader, 1, &fileData, NULL);
	glCompileShader(fragmentShader);

	//Link the two shaders
	//unsigned int shaderProgram;
	int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	//clear the unhooked shaders to free memory
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//setup openGLs ability to read inputs

	glUseProgram(shaderProgram);
	return(shaderProgram);
}

void window_resize_handler(GLFWwindow* window, int width, int height) {
	printf("%d, %d\n", width, height);
	if (width == 0 || height == 0) {//safety
		return;
	}
	float aspect = (float)((double)width / (double)height);
	//float fov = 45;
	float far = 1000;
	float near = 0.1;

	float perspective[4][4];
	float f = 1000;
	float n = 0.1;

	float tanHalfFovy = tan(FOV / 2);

	for (int c = 0; c < 4; c++) {
		for (int a = 0; a < 4; a++) {
			perspective[c][a] = 0;
		}
	}

	perspective[0][0] = 1 / (aspect * tanHalfFovy);
	perspective[1][1] = 1 / (tanHalfFovy);
	perspective[2][2] = f / (n - f);
	perspective[2][3] = -1.0;
	perspective[3][2] = -((f * n) / (f - n));
	glUniformMatrix4fv(ProgramData.perspectiveLoc, 1, 0, perspective);
	glUniform1f(ProgramData.windowRatioLoc, ((float)height / (float)width));
	glViewport(0, 0, width, height);
}

UnfinObj fillUnfinObj(float* verts, unsigned int* inds, int vLen, int iCount) {
	UnfinObj returnObj;
	returnObj.iCount = iCount;
	returnObj.vLineCount = vLen;
	returnObj.verts = verts;
	returnObj.indices = inds;

	return(returnObj);
}

UnfinObj createUnfinObjFromStatic(float* verts, unsigned int* inds, int vLen, int iCount) {
	UnfinObj returnObj;
	returnObj.iCount = iCount;
	returnObj.vLineCount = vLen;

	returnObj.verts = calloc(vLen * VERTEX_LENGTH, VERTEX_SIZE);
	returnObj.indices = calloc(iCount, IND_SIZE);

	for (int c = 0; c < vLen * VERTEX_LENGTH; c++) {
		returnObj.verts[c] = verts[c];
	}

	for (int c = 0; c < iCount; c++) {
		returnObj.indices[c] = inds[c];
	}

	return(returnObj);
}

UnfinObj freeUnfinObj(UnfinObj obj) {
	free(obj.verts);
	free(obj.indices);
	return(obj);
}

//puts object two behind obj one
UnfinObj appendVec(UnfinObj* objOne, UnfinObj* objTwo) {
	UnfinObj returnObj = { 0 };
	returnObj.verts = calloc(objOne->vLineCount + objTwo->vLineCount, VECTOR_VERTEX_LENGTH * VERTEX_SIZE);
	returnObj.indices = calloc(objOne->iCount + objTwo->iCount, IND_SIZE);
	returnObj.iCount = objOne->iCount + objTwo->iCount;
	returnObj.vLineCount = objOne->vLineCount + objTwo->vLineCount;

	for (int cInd = 0; cInd < objOne->iCount; cInd++) {//Copy the first one over
		returnObj.indices[cInd] = objOne->indices[cInd];
	}
	for (int cInd = objOne->iCount; cInd < objOne->iCount + objTwo->iCount; cInd++) {//Copy the second over with the offset to point at the correct vertex
		returnObj.indices[cInd] = objTwo->indices[cInd - objOne->iCount] + objOne->vLineCount;
	}

	for (int cVert = 0; cVert < objOne->vLineCount * VECTOR_VERTEX_LENGTH; cVert++) {//Copy first over
		returnObj.verts[cVert] = objOne->verts[cVert];
	}
	for (int cVert = objOne->vLineCount * VECTOR_VERTEX_LENGTH; cVert < (objOne->vLineCount + objTwo->vLineCount) * VECTOR_VERTEX_LENGTH; cVert++) {//Copy second over
		returnObj.verts[cVert] = objTwo->verts[cVert - (objOne->vLineCount * VECTOR_VERTEX_LENGTH)];
	}

	return(returnObj);
}

UnfinObj appendUnfinisheds(UnfinObj* objOne, UnfinObj* objTwo) {
	UnfinObj returnObj = { 0 };

	returnObj.verts = calloc(objOne->vLineCount + objTwo->vLineCount, VERTEX_LENGTH * VERTEX_SIZE);
	returnObj.indices = calloc(objOne->iCount + objTwo->iCount, IND_SIZE);
	returnObj.iCount = objOne->iCount + objTwo->iCount;
	returnObj.vLineCount = objOne->vLineCount + objTwo->vLineCount;

	for (int cInd = 0; cInd < objOne->iCount; cInd++) {//Copy the first one over
		returnObj.indices[cInd] = objOne->indices[cInd];
	}
	for (int cInd = objOne->iCount; cInd < objOne->iCount + objTwo->iCount; cInd++) {//Copy the second over with the offset to point at the correct vertex
		returnObj.indices[cInd] = objTwo->indices[cInd - objOne->iCount] + objOne->vLineCount;
	}

	for (int cVert = 0; cVert < objOne->vLineCount * VERTEX_LENGTH; cVert++) {//Copy first over
		returnObj.verts[cVert] = objOne->verts[cVert];
	}
	for (int cVert = objOne->vLineCount * VERTEX_LENGTH; cVert < (objOne->vLineCount + objTwo->vLineCount) * VERTEX_LENGTH; cVert++) {//Copy second over
		returnObj.verts[cVert] = objTwo->verts[cVert - (objOne->vLineCount * VERTEX_LENGTH)];
	}

	return(returnObj);
}


//The default handler of mouse clicks
void defaultMoustClick(GLFWwindow* window, int button, int action, int mods) {
	
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	int width;
	int height;
	glfwGetWindowSize(window, &width, &height);
	float wiRat = ((float)width / (float)height);

	xpos = xpos * wiRat;
	xpos = (xpos * 2)/ width;
	xpos -= wiRat;
	
	ypos *= -1;
	ypos = (ypos * 2)/ height;
	ypos += 1;
	//printf("%f, %f\n", xpos, ypos);
	for (int masterPos = 0; masterPos < masterUIListLength; masterPos++) {//go through all UI layers
		if (masterUIList[masterPos] != NULL && masterUIList[masterPos]->active == 1) {//safety & active UI layer checker
			for (int cItem = 0; cItem < masterUIList[masterPos]->elementCount; cItem++) {//Go through all UI elements in the layer
				UIElement* cItm = masterUIList[masterPos]->elements[cItem];//make the following lines less massive
				//check to see if this button is both active and has an action set, no action set = we cant do anything anyhow
				if (cItm->elementActive == 1 && cItm->action != NULL) {
					//make sure we're not on a pending action, would be painful to miss stuff or muddy up our memory
					if (cItm->actionNeeded == READY_FOR_ACTION) {						
						/*float temp = cItm->clickArea[0] + (cItm->position[X_pos] * wiRat);
						temp = (cItm->clickArea[1] + (cItm->position[X_pos] * wiRat));
						temp = (cItm->clickArea[2] + cItm->position[Y_pos]);
						temp = (-cItm->clickArea[3] + cItm->position[Y_pos]);*/
						//check if click is within click area
						if (xpos > (-cItm->clickArea[0] + (cItm->position[X_pos]*wiRat)) && 
							xpos < (cItm->clickArea[1] + (cItm->position[X_pos] * wiRat)) && 
							ypos < (cItm->clickArea[2] + cItm->position[Y_pos]) && //Y pos is not changed via window shape, all of it is done on X
							ypos > (-cItm->clickArea[3] + cItm->position[Y_pos])
							) {
							masterUIList[masterPos]->elements[cItem]->actionNeeded = AWAITING_ACTION;
							char* modifier = &masterUIList[masterPos]->elements[cItem]->clickData;
							modifier[0] = button;
							modifier[1] = action;
						}
					}
				}
				
			}
		}

	}
}

void defaultButtonPress(GLFWwindow* window, int key, int scancode, int action, int mods) {

}