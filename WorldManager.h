#pragma once
#include "GGE2_1.h"

#define STANDARD_WORLD 0
#define VECTOR_WORLD 1

World createWorld(int worldType);
void rotateCamera(World* world, float rotQuat[4]);
void insertObjectIntoWorld(World* world, Object* object, int renderIt);//Render it is the default for whether or not it shall be rendered
void removeObjectFromWorld(World* world, int ID, int FreeIt);//Removes it from existence. Freeit specifies if it needs to be free'd or not
void drawWorld(World* world);//updateObjects checks each object for the 'update' flag, if true it will update OpenGL's knowledge of the object and then set the flag to zero