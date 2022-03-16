#pragma once
#include <malloc.h>
#include "WorldManager.h"
#include "GlorwynUtilities.h"

void rotateCamera(World* world, float rotQuat[4]) {
	float* temp = calloc(4, sizeof(float));
	vec4 conj = quatConjNS(rotQuat);
	float Conj[4];
	Conj[0] = conj.a[0];
	Conj[1] = conj.a[1];
	Conj[2] = conj.a[2];
	Conj[3] = conj.a[3];
	
	temp[0] = 0;
	temp[1] = world->camFo[X];
	temp[2] = world->camFo[Y];
	temp[3] = world->camFo[Z];
	
	vec4 stepone = quatMultNS(rotQuat, temp);
	stepone = quatMultNS((float*)&stepone, Conj);
	normalizeQuat((float*)&stepone);
	world->camFo[X] = stepone.a[Y];
	world->camFo[Y] = stepone.a[Z];
	world->camFo[Z] = stepone.a[W];

	temp[0] = 0;
	temp[1] = world->camLe[X];
	temp[2] = world->camLe[Y];
	temp[3] = world->camLe[Z];
	vec4 steptwo = quatMultNS(rotQuat, temp);
	steptwo = quatMultNS((float*)&steptwo, Conj);
	normalizeQuat((float*)&steptwo);
	world->camLe[X] = steptwo.a[Y];
	world->camLe[Y] = steptwo.a[Z];
	world->camLe[Z] = steptwo.a[W];

	temp[0] = 0;
	temp[1] = world->camUp[X];
	temp[2] = world->camUp[Y];
	temp[3] = world->camUp[Z];
	vec4 stepthree = quatMultNS(rotQuat, temp);
	stepthree = quatMultNS((float*)&stepthree, Conj);
	normalizeQuat((float*)&stepthree);
	world->camUp[X] = stepthree.a[Y];
	world->camUp[Y] = stepthree.a[Z];
	world->camUp[Z] = stepthree.a[W];
}


World createWorld() {
	World newWorld;
	newWorld.objectCount = 0;
	newWorld.objectListSize = 0;
	newWorld.objectRender = NULL;
	newWorld.objects = NULL;
	newWorld.camera[X] = 0;
	newWorld.camera[Y] = 0;
	newWorld.camera[Z] = 0;
	newWorld.camera[W] = 1;
	newWorld.camera[I] = 0;
	newWorld.camera[J] = 0;
	newWorld.camera[K] = 0;
	newWorld.camUp[0] = 0;
	newWorld.camUp[1] = 1;
	newWorld.camUp[2] = 0;
	newWorld.camFo[0] = 0;
	newWorld.camFo[1] = 0;
	newWorld.camFo[2] = -1;
	newWorld.camLe[0] = -1;
	newWorld.camLe[1] = 0;
	newWorld.camLe[2] = 0;
	return(newWorld);
}

void insertObjectIntoWorld(World* world, Object* object, int renderIt) {
	if (world->objectCount == world->objectListSize) {
		//POTENTIAL PERFORMANCE TODO- Could check the actual size of the list versus what we needand spare some realloc calls that might be redundant
		world->objectCount++;
		world->objectListSize = world->objectCount;
		world->objects = realloc(world->objects, world->objectCount * sizeof(Object*));
		world->objects[world->objectCount - 1] = object;

		world->objectRender = realloc(world->objectRender, world->objectCount * sizeof(int*));
		world->objectRender[world->objectCount - 1] = renderIt;
	}
}

void removeObjectFromWorld(World* world, int ID, int FreeIt) {
	int current = 0;
	while (current < world->objectCount) {
		if (world->objects[current]->ID == ID) {
			if (FreeIt) {
				free(world->objects[current]);
			}
			world->objects[current] = world->objects[world->objectCount - 1];
			world->objects[world->objectCount - 1] = NULL;
			world->objectCount--;
			return;
		}
	}
}


void drawWorld(World* world) {
	int current = 0;

	glUniform4f(ProgramData.cameraLoc, world->camera[W], world->camera[I], world->camera[J], world->camera[K]);
	while (current < world->objectCount) {
		if (world->objectRender[current] == 1) {
			Object* temp = world->objects[current];
			drawWorldObject(world->objects[current], world);
		}
		current++;
	}
}