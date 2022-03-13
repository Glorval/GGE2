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


	GLFWwindow* window = glfwCreateWindow(800, 600, APPLICATIONNAME, NULL, NULL);
	if (window == NULL) {
		printf("Failed to create GLFW window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		printf("GLAD failed to load.");
	}

	glViewport(0, 0, 800, 600);

	ProgramData.shaderID = setupShaders();
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_DEPTH_CLAMP);

	ProgramData.cordinatesLoc = glGetUniformLocation(ProgramData.shaderID, "cordinates");
	ProgramData.orientationLoc = glGetUniformLocation(ProgramData.shaderID, "orientation");
	ProgramData.scaleLoc = glGetUniformLocation(ProgramData.shaderID, "scale");
	ProgramData.perspectiveLoc = glGetUniformLocation(ProgramData.shaderID, "perspective");
	ProgramData.cameraLoc = glGetUniformLocation(ProgramData.shaderID, "cameraPosition");
	ProgramData.camAngleLoc = glGetUniformLocation(ProgramData.shaderID, "cameraOrientation");

	float aspect = 800 / 600;
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


	//int cameraDefault[4] = { 0, 0, 1, 0 };
	//glUniformMatrix4fv(ProgramData.cameraLoc, 1, 0, cameraDefault);
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





