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


World* ourWorld;
void moveShape(Object* ourObject);
void printThingy(float thing[3]) {
	printf("%f, %f, %f\n", thing[0], thing[1], thing[2]);
}


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
	

	Object ourObject = createObject(vertices, indices,6, 18);
	ourObject.position[Z] = -2;
	ourObject.position[X] = -0.5;
	
	World world = createWorld();
	insertObjectIntoWorld(&world, &ourObject, 1);

	Object secondObject = createObject(verticestwo, indices, 6, 18);
	secondObject.position[Z] = -2;
	secondObject.position[X] = 0.5;
	insertObjectIntoWorld(&world, &secondObject, 1);


	ourWorld = &world;
	_beginthread(moveShape, 0, world.objects[0]);

	float counter = 1;
	while (1) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		//float temper = sin(counter);
		//counter += 0.01;
		//ourWorld.camera[0] += temper/75;
		//printf("%f\n", temper);

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
			float rotateQuat[4] = { cosf(0.0872665 / 2), 0, 1 * sinf(0.1 / 2), 0 };
			normalizeQuat(rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'j') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 0, 1 * sinf(-0.1 / 2), 0 };
			normalizeQuat(rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}


		if (input == 'i') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), 1 * sinf(0.1 / 2), 0, 0 };
			normalizeQuat(rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}
		if (input == 'k') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 1 * sinf(-0.1 / 2), 0, 0 };
			normalizeQuat(rotateQuat);
			quatMult(rotateQuat, &ourObject->position[W]);
			printf("\n%f, %f, %f, %f\n", ourObject->position[W], ourObject->position[I], ourObject->position[J], ourObject->position[K]);
			//normalizeQuat(&ourObject->position[W]);
		}













		double moveScale = 0.05;
		if (input == 't') {
			ourWorld->camera[X] += moveScale * ourWorld->camUp[X];
			ourWorld->camera[Y] += moveScale * ourWorld->camUp[Y];
			ourWorld->camera[Z] += moveScale * ourWorld->camUp[Z];
			printf("\n%f\n", ourWorld->camera[Y]);
		}
		if (input == 'g') {
			ourWorld->camera[X] -= moveScale * ourWorld->camUp[X];
			ourWorld->camera[Y] -= moveScale * ourWorld->camUp[Y];
			ourWorld->camera[Z] -= moveScale * ourWorld->camUp[Z];
			printf("\n%f\n", ourWorld->camera[Y]);
		}
		if (input == 'f') {
			ourWorld->camera[X] += moveScale * ourWorld->camLe[X];
			ourWorld->camera[Y] += moveScale * ourWorld->camLe[Y];
			ourWorld->camera[Z] += moveScale * ourWorld->camLe[Z];
			printf("\n%f\n", ourWorld->camera[X]);
		}
		if (input == 'h') {
			ourWorld->camera[X] -= moveScale * ourWorld->camLe[X];
			ourWorld->camera[Y] -= moveScale * ourWorld->camLe[Y];
			ourWorld->camera[Z] -= moveScale * ourWorld->camLe[Z];
			printf("\n%f\n", ourWorld->camera[X]);
		}
		if (input == 'r') {
			ourWorld->camera[X] -= moveScale * ourWorld->camFo[X];
			ourWorld->camera[Y] -= moveScale * ourWorld->camFo[Y];
			ourWorld->camera[Z] -= moveScale * ourWorld->camFo[Z];
			printf("\n%f\n", ourWorld->camera[Z]);
		}
		if (input == 'y') {
			ourWorld->camera[X] += moveScale * ourWorld->camFo[X];
			ourWorld->camera[Y] += moveScale * ourWorld->camFo[Y];
			ourWorld->camera[Z] += moveScale * ourWorld->camFo[Z];
			printf("\n%f\n", ourWorld->camera[Z]);
		}

		if (input == '7') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), 0, 0, 1 * sinf(0.1 / 2) };
			normalizeQuat(&rotateQuat);
			rotateCamera(ourWorld, &rotateQuat);
			quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '9') {
			float rotateQuat[4] = { cosf(-0.1 / 2), 0, 0, 1 * sinf(-0.1 / 2) };
			normalizeQuat(rotateQuat);
			rotateCamera(ourWorld, &rotateQuat);
			quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}


		if (input == '6') {
			float rotateQuat[4] = { cosf(0.1 / 2), 0, 1 * sinf(0.1 / 2), 0 };
			printf("\n");
			normalizeQuat(rotateQuat);
			rotateCamera(ourWorld, rotateQuat);
			printThingy(ourWorld->camLe);
			printThingy(ourWorld->camFo);
			printThingy(ourWorld->camUp);
			normalizeQuat(&ourWorld->camera[W]);
			vec4 stepone = quatMultNS(rotateQuat, &ourWorld->camera[W]);
			//stepone = quatMultNS((float*)&stepone, quatConj(rotateQuat));
			normalizeQuat((float*)&stepone);
			ourWorld->camera[W] = stepone.a[X];
			ourWorld->camera[I] = stepone.a[Y];
			ourWorld->camera[J] = stepone.a[Z];
			ourWorld->camera[K] = stepone.a[W];
			//quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '4') {
			float rotateQuat[4] = { cosf(-0.1 / 2), 0, 1 * sinf(-0.1 / 2), 0 };
			normalizeQuat(rotateQuat);
			rotateCamera(ourWorld, rotateQuat);
			quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}


		if (input == '8') {
			float rotateQuat[4] = { cosf(0.1 / 2), 1 * sinf(0.1 / 2), 0, 0 };
			normalizeQuat(rotateQuat);
			rotateCamera(ourWorld, &rotateQuat);
			quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '5') {
			float rotateQuat[4] = { cosf(-0.1 / 2), 1 * sinf(-0.1 / 2), 0, 0 };
			normalizeQuat(rotateQuat);
			rotateCamera(ourWorld, &rotateQuat);
			quatMultFull(rotateQuat, &ourWorld->camera[W]);
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W], ourWorld->camera[I], ourWorld->camera[J], ourWorld->camera[K]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		
	}
	
}