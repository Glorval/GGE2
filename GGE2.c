#pragma once
#include "GGE2.h"
#include "ObjectManager.h"
#include "GlorwynUtilities.h"
#include <math.h>



GLFWwindow* startup() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
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

	float aspect = 4 / 3;
	float fov = 45;
	float far = 1000;
	float near = 0.1;

	float perspective[4][4];
	float f = 1000;
	float n = 0.1;

	float tanHalfFovy = tan(fov / 2);

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

	glfwSetMouseButtonCallback(window, defaultMoustClick);

	//int cameraDefault[4] = { 0, 0, 1, 0 };
	//glUniformMatrix4fv(ProgramData.cameraLoc, 1, 0, cameraDefault);

	setupUI(USE_BUILT_IN_FONT);
	return(window);
}

int setupShaders() {
	//Vertex shader
	char* fileData = NULL;

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fileData = readFile("VShader.glsl");
	if (fileData == NULL) {
		gError("Vertex Shader failed to read.");
		return;
	}
	glShaderSource(vertexShader, 1, &fileData, NULL);
	glCompileShader(vertexShader);


	//Fragment Shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	fileData = readFile("FShader.glsl");
	if (fileData == NULL) {
		gError("Vertex Shader failed to read.");
		return;
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

void appendUnfinisheds(UnfinObj* objOne, UnfinObj* objTwo) {
	int iCount = objOne->iCount;
	int vLength = objOne->vLineCount;
	objOne->iCount += objTwo->iCount;
	objOne->vLineCount += objTwo->vLineCount;

	objOne->verts = realloc(objOne->verts, objOne->vLineCount * VERTEX_LENGTH * VERTEX_SIZE);
	objOne->indices = realloc(objOne->indices, IND_SIZE * objOne->iCount);

	for (int c = iCount; c < objOne->iCount; c++) {
		printf("Cind: %d, %d   ", c, objTwo->indices[c - iCount] + iCount);
		objOne->indices[c] = objTwo->indices[c - iCount] + iCount;//C is the offset here in a sense, the indices from the 2nd entry will have been for themselves and need to be offset to point at the right vertiecs
	}
	printf("\n");

	for (int c = vLength; c < objOne->vLineCount; c++) {
		for (int cEntry = 0; cEntry < VERTEX_LENGTH; cEntry++) {
			printf("Cv: %d, %f   ", cEntry + (c*6), objTwo->verts[(c * VERTEX_LENGTH) - vLength + cEntry]);
			objOne->verts[(c * VERTEX_LENGTH)+cEntry] = objTwo->verts[(c * VERTEX_LENGTH) - vLength + cEntry];
		}
	}
	printf("\n\n");
}


//The default handler of mouse clicks
void defaultMoustClick(GLFWwindow* window, int button, int action, int mods) {
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);
	for (int masterPos = 0; masterPos < masterUIListLength; masterPos++) {
		for (int cItem = 0; cItem < masterUIList[masterPos]->elementCount; cItem++) {
			if (masterUIList[masterPos]->elements[cItem]->elementActive == 1 && masterUIList[masterPos]->elements[cItem]->actionNeeded == READY_FOR_ACTION) {
				UIElement* ref = masterUIList[masterPos]->elements[cItem];
				if (xpos > ref->clickArea[0] && xpos < ref->clickArea[1] && ypos < ref->clickArea[2] && ypos > ref->clickArea[3]) {
					masterUIList[masterPos]->elements[cItem]->actionNeeded = AWAITING_ACTION;
					int* modifier = &masterUIList[0]->elements[0]->data;
					modifier[0] = button;
					modifier[1] = action;
				}
				
			}
		}
	}
}