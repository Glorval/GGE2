#pragma once
#include "WorldManager.h"

World createWorld(int worldType) {
	World newWorld;
	newWorld.objectCount = 0;
	newWorld.objectListSize = 0;
	newWorld.renderObject = NULL;
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

	newWorld.worldType = worldType;

	return(newWorld);
}

//returns the ID of the object so it can be accessed later
int insertObjectIntoWorld(World* world, Object* object, int renderIt) {
	if (world->objectCount == world->objectListSize) {
		//POTENTIAL PERFORMANCE TODO- Could check the actual size of the list versus what we needand spare some realloc calls that might be redundant
		world->objectCount++;
		world->objectListSize = world->objectCount;
		world->objects = realloc(world->objects, world->objectCount * sizeof(Object*));
		world->objects[world->objectCount - 1] = object;

		world->renderObject = realloc(world->renderObject, world->objectCount * sizeof(int*));
		world->renderObject[world->objectCount - 1] = renderIt;
		return(world->objectCount - 1);
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

	
	glUniform3f(ProgramData.cameraPosLoc, world->camera[X_pos], world->camera[Y_pos], world->camera[Z_pos]);
	glUniform4f(ProgramData.camAngleLoc, world->camera[W_pos], world->camera[I_pos], world->camera[J_pos], world->camera[K_pos]);

	if (world->worldType == STANDARD_WORLD) {
		glUniform1i(ProgramData.flagsLoc, RENDER_MODE_NORMAL);
		while (current < world->objectCount) {
			if (world->renderObject[current] == 1) {
				drawStandardObject(world->objects[current]);
			}
			current++;
		}
	} else if (world->worldType == VECTOR_WORLD) {
		glUniform1i(ProgramData.flagsLoc, RENDER_MODE_VECTOR);
		glUniform4f(ProgramData.colourLoc, world->vecColour[0], world->vecColour[1], world->vecColour[2], world->vecColour[3]);// 4, world->vecColour);
		while (current < world->objectCount) {
			if (world->renderObject[current] == 1) {
				drawVectorObjectSET(world->objects[current]);
			}
			current++;
		}
	}
	
}