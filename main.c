#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>

#include <process.h>

#include "GGE2.h"
#include "ObjectManager.h"
#include "WorldManager.h"

#include "GlorwynUtilities.h"

#include <time.h>

float up[] = { 0, 0, -1, 0, 0 };
float back[] = { 0, 0, 0 ,1 };
float right[] = { 0, 1, 0 ,0 };


World* ourWorld;
void moveShape(Object* ourObject);

int main() {
	printf("Hi\n");


	GLFWwindow* window = startup();

	float vertices[] = {
		-0.25f, -0.25f, -0.25f, 0.6f, 0.0f, 0.0f,
		0.25f, -0.25f, -0.25f, 0.0f, 0.6f, 0.0f,
		-0.25f, -0.25f, 0.25f, 0.6f, 0.6f, 0.6f,
		0.25f, -0.25f, 0.25f, 0.0f, 0.0f, 0.6,
		0.0f, 0.25f, 0.0f, 0.1f, 0.1f, 0.1f
	};
	float verticestwo[] = {
		-0.50f, -0.1f, -0.25f, 0.6f, 0.0f, 0.0f,
		0.50f, -0.1f, -0.25f, 0.0f, 0.6f, 0.0f,
		-0.50f, -0.1f, 0.25f, 0.6f, 0.6f, 0.6f,
		0.50f, -0.1f, 0.25f, 0.0f, 0.0f, 0.6,
		0.0f, 0.1f, 0.0f, 0.1f, 0.1f, 0.1f
	};
	unsigned int indices[] = { 0,1,2, 1,2,3, 0,1,4, 1,3,4, 2,3,4, 0,2,4};
	

	Object ourObject = createObject(vertices, indices,6, 18, 0);
	ourObject.position[Z] = -2;
	ourObject.position[X] = -0.5;
	
	World world = createWorld();
	insertObjectIntoWorld(&world, &ourObject, 1);

	Object secondObject = createObject(verticestwo, indices, 6, 18, 1);
	secondObject.position[Z] = -2;
	secondObject.position[X] = 0.5;
	insertObjectIntoWorld(&world, &secondObject, 1);


	ourWorld = &world;
	_beginthread(moveShape, 0, world.objects[0]);

	float counter = 1;
	while (1) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		drawWorld(&world);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	char t = 'c';
	scanf("%c", &t);

	glfwTerminate();
	return 0;
}



void moveShape(Object* ourObject) {
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
			float rotateQuat[4] = { cosf(0.0872665 / 2), up[1] * sinf(0.1 / 2), up[2] * sinf(0.1 / 2), up[3] * sinf(0.1 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			normalizeQuat(&ourObject->position[W]);

			quatMult(rotateQuat, up);
			quatMultRS(up, quatConj(rotateQuat));
			up[0] = 0;
			quatMult(rotateQuat, back);
			quatMultRS(back, quatConj(rotateQuat));
			back[0] = 0;
			quatMult(rotateQuat, right);
			quatMultRS(right, quatConj(rotateQuat));
			right[0] = 0;


			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'j') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 0, 1 * sinf(0.0872665 / 2), 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}


		if (input == 'i') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), right[1] * sinf(0.0872665 / 2), right[2] * sinf(0.0872665 / 2), right[3] * sinf(0.0872665 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);

			quatMult(rotateQuat, up);
			quatMultRS(up, quatConj(rotateQuat));
			up[0] = 0;
			quatMult(rotateQuat, back);
			quatMultRS(back, quatConj(rotateQuat));
			back[0] = 0;
			quatMult(rotateQuat, right);
			quatMultRS(right, quatConj(rotateQuat));
			right[0] = 0;

			printf("\ttemp: %f, %f, %f", up[1], up[2], up[3]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'k') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 1 * sinf(-0.1 / 2), 0, 0 };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}














		if (input == 't') {
			ourWorld->camera[X] += ourWorld->back[1] * .01;
			ourWorld->camera[Y] += ourWorld->back[2] * .01;
			ourWorld->camera[Z] += ourWorld->back[3] * .01;
			printf("\n%f\n", ourWorld->camera[Y]);
		}
		if (input == 'g') {
			ourWorld->camera[X] -= ourWorld->back[1] * .01;
			ourWorld->camera[Y] -= ourWorld->back[2] * .01;
			ourWorld->camera[Z] -= ourWorld->back[3] * .01;
			printf("\n%f\n", ourWorld->camera[Y]);
		}
		if (input == 'f') {
			ourWorld->camera[X] += ourWorld->left[1] * .01;
			ourWorld->camera[Y] += ourWorld->left[2] * .01;
			ourWorld->camera[Z] += ourWorld->left[3] * .01;
			printf("\n%f\n", ourWorld->camera[X]);
		}
		if (input == 'h') {
			ourWorld->camera[X] -= ourWorld->left[1] * .01;
			ourWorld->camera[Y] -= ourWorld->left[2] * .01;
			ourWorld->camera[Z] -= ourWorld->left[3] * .01;
			printf("\n%f\n", ourWorld->camera[X]);
		}
		if (input == 'r') {
			ourWorld->camera[X] += ourWorld->up[1] * .01;
			ourWorld->camera[Y] += ourWorld->up[2] * .01;
			ourWorld->camera[Z] += ourWorld->up[3] * .01;
			printf("\n%f\n", ourWorld->camera[Z]);
		}
		if (input == 'y') {
			ourWorld->camera[X] -= ourWorld->up[1] * .01;
			ourWorld->camera[Y] -= ourWorld->up[2] * .01;
			ourWorld->camera[Z] -= ourWorld->up[3] * .01;
			printf("\n%f\n", ourWorld->camera[Z]);
		}

		if (input == '6') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), ourWorld->up[1] * sinf(0.0872665 / 2), ourWorld->up[2] * sinf(0.0872665 / 2), ourWorld->up[3] * sinf(0.0872665 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourWorld->camera[W]);
			normalizeQuat(&ourWorld->camera[W]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, quatConj(rotateQuat));
			ourWorld->up[0] = 0;
			quatMult(rotateQuat, ourWorld->back);
			quatMultRS(ourWorld->back, quatConj(rotateQuat));
			ourWorld->back[0] = 0;
			quatMult(rotateQuat, ourWorld->left);
			quatMultRS(ourWorld->left, quatConj(rotateQuat));
			ourWorld->left[0] = 0;
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '4') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), ourWorld->up[1] * sinf(-0.0872665 / 2), ourWorld->up[2] * sinf(-0.0872665 / 2), ourWorld->up[3] * sinf(-0.0872665 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourWorld->camera[W]);
			normalizeQuat(&ourWorld->camera[W]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, quatConj(rotateQuat));
			ourWorld->up[0] = 0;
			quatMult(rotateQuat, ourWorld->back);
			quatMultRS(ourWorld->back, quatConj(rotateQuat));
			ourWorld->back[0] = 0;
			quatMult(rotateQuat, ourWorld->left);
			quatMultRS(ourWorld->left, quatConj(rotateQuat));
			ourWorld->left[0] = 0;
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}


		if (input == '8') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), ourWorld->left[1] * sinf(0.0872665 / 2), ourWorld->left[2] * sinf(0.0872665 / 2), ourWorld->left[3] * sinf(0.0872665 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourWorld->camera[W]);
			normalizeQuat(&ourWorld->camera[W]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, quatConj(rotateQuat));
			ourWorld->up[0] = 0;
			quatMult(rotateQuat, ourWorld->back);
			quatMultRS(ourWorld->back, quatConj(rotateQuat));
			ourWorld->back[0] = 0;
			quatMult(rotateQuat, ourWorld->left);
			quatMultRS(ourWorld->left, quatConj(rotateQuat));
			ourWorld->left[0] = 0;
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '5') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), ourWorld->left[1] * sinf(-0.0872665 / 2), ourWorld->left[2] * sinf(-0.0872665 / 2), ourWorld->left[3] * sinf(-0.0872665 / 2) };
			normalizeQuat(&rotateQuat);
			quatMult(rotateQuat, &ourWorld->camera[W]);
			normalizeQuat(&ourWorld->camera[W]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, quatConj(rotateQuat));
			ourWorld->up[0] = 0;
			quatMult(rotateQuat, ourWorld->back);
			quatMultRS(ourWorld->back, quatConj(rotateQuat));
			ourWorld->back[0] = 0;
			quatMult(rotateQuat, ourWorld->left);
			quatMultRS(ourWorld->left, quatConj(rotateQuat));
			ourWorld->left[0] = 0;
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		
	}
	
}