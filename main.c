#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>


#include <process.h>

#include "GGE2.h"
#include "GlorwynUtilities.h"


void moveShape(struct Object ourObject);

int main() {
	printf("Hi\n");


	GLFWwindow* window = startup();
	


	float vertices[] = {
		-0.25f, 0.25f, -0.25f, 0.6f, 0.0f, 0.0f,
		0.25f, 0.25f, -0.25f, 0.0f, 0.6f, 0.0f,
		-0.25f, 0.25f, 0.25f, 0.6f, 0.6f, 0.6f,
		0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 0.6,
		0.0f, 0.5f, 0.0f, 0.1f, 0.1f, 0.1f
	};
	unsigned int indices[] = { 0,1,2, 1,2,3, 0,1,4, 1,3,4, 2,3,4, 0,2,4};
	

	Object ourObject = createObject(vertices, indices,6, 18);
	ourObject.position[X] = 0;
	ourObject.position[Y] = 0;
	ourObject.position[Z] = 0;
	ourObject.position[W] = 0;
	ourObject.position[I] = 0;
	ourObject.position[J] = 1;
	ourObject.position[K] = 0;
	normalizeQuat(&ourObject.position[W]);

	_beginthread(moveShape, 0, &ourObject);

	//float temp[4] = { cosf(0.05 / 2), 1 * sinf(0.05 / 2), 0, 0 };
	
	float tempTwo[4] = { cosf(0.01 / 2), 0 , 0, 1 * sinf(0.01 / 2) };
	normalizeQuat(tempTwo);

	while (1) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);


		drawShape(&ourObject);
		//quatMult(tempTwo, temp);
		//normalizeQuat(temp);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	char t = 'c';
	scanf("%c", &t);

	glfwTerminate();
	return 0;
}





void moveShape(struct Object* ourObject) {
	while (1) {
		char input = getche();
		if (input == 'w') {
			ourObject->position[Y] += 0.05;
			printf("\n%f\n", ourObject->position[Y]);
		}
		if (input == 's') {
			ourObject->position[Y] -= 0.05;
			printf("\n%f\n", ourObject->position[Y]);
		}
		if (input == 'a') {
			ourObject->position[X] -= 0.05;
			printf("\n%f\n", ourObject->position[X]);
		}
		if (input == 'd') {
			ourObject->position[X] += 0.05;
			printf("\n%f\n", ourObject->position[X]);
		}
		if (input == 'q') {
			ourObject->position[Z] -= 0.05;
			printf("\n%f\n", ourObject->position[Z]);
		}
		if (input == 'e') {
			ourObject->position[Z] += 0.05;
			printf("\n%f\n", ourObject->position[Z]);
		}

		if (input == 'l') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), 0, 1 * sinf(0.1 / 2), 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'j') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 0, 1 * sinf(-0.1 / 2), 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			//normalizeQuat(&ourObject->position[W]);
		}


		if (input == 'i') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), 1 * sinf(0.1 / 2), 0, 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'k') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 1 * sinf(-0.1 / 2), 0, 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			//normalizeQuat(&ourObject->position[W]);
		}
		
	}
	
}