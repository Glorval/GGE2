#pragma once
#include <malloc.h>
#include "WorldManager.h"

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