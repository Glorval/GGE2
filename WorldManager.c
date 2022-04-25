#pragma once
#include <malloc.h>
#include "WorldManager.h"

World createWorld() {
	World newWorld;
	newWorld.objectCount = 0;
	newWorld.objectListSize = 0;
	newWorld.objectRender = NULL;
	newWorld.objects = NULL;
	newWorld.camera[X_pos] = 0;
	newWorld.camera[Y_pos] = 0;
	newWorld.camera[Z_pos] = 0;
	newWorld.camera[W_pos] = 1;
	newWorld.camera[I_pos] = 0;
	newWorld.camera[J_pos] = 0;
	newWorld.camera[K_pos] = 0;

	newWorld.up[0] = 0;
	newWorld.up[1] = 0;
	newWorld.up[2] = -1;
	newWorld.up[3] = 0;

	newWorld.back[0] = 0;
	newWorld.back[1] = 0;
	newWorld.back[2] = 0;
	newWorld.back[3] = 1;

	newWorld.left [0] = 0;
	newWorld.left[1] = -1;
	newWorld.left[2] = 0;
	newWorld.left[3] = 0;

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

//might not work rn
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

	glUniform1i(ProgramData.flagsLoc, 0);
	glUniform3f(ProgramData.cameraPosLoc, world->camera[X_pos], world->camera[Y_pos], world->camera[Z_pos]);
	glUniform4f(ProgramData.camAngleLoc, world->camera[W_pos], world->camera[I_pos], world->camera[J_pos], world->camera[K_pos]);
	while (current < world->objectCount) {
		if (world->objectRender[current] == 1) {
			drawObject(world->objects[current]);
		}
		current++;
	}
}