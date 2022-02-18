#pragma once
#include "GGE2.h"
#include "GlorwynUtilities.h"
#include <math.h>


#define IND_SIZE sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b
#define VERTEX_SIZE sizeof(float)//The size in bytes of a vertex, sizeof(float)


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

struct Object createObject(float* vertices, unsigned int* index, int vertSize, int indSize) {
	unsigned int VBO;
	struct Object returnObject;
	returnObject.indexCount = indSize;
	glGenVertexArrays(1, &returnObject.ID);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &returnObject.EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(returnObject.ID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * VERTEX_SIZE * VERTEX_LENGTH, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnObject.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * IND_SIZE, index, GL_DYNAMIC_DRAW);

	// position attribute, first three
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute, second three (Obviously offset by three)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	return(returnObject);
}

void drawShape(struct Object* shape) {
	glBindVertexArray(shape->ID);


	//normalizeQuat(temp);
	float temp = shape->position[X];
	temp = shape->position[Y];
	temp = shape->position[Z];
	//glUniform1f(ProgramData.scaleLoc, shape->scale);
	glUniform3f(ProgramData.cordinatesLoc, shape->position[X], shape->position[Y], shape->position[Z]);
	glUniform4f(ProgramData.orientationLoc, shape->position[W], shape->position[I], shape->position[J], shape->position[K]);


	glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
}


