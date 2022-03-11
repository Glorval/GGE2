#pragma once
#include "GGE2.h"

World createWorld();
void insertObjectIntoWorld(World* world, Object* object, int renderIt);//Render it is the default for whether or not it shall be rendered
void removeObjectFromWorld(World* world, int ID, int FreeIt);//Removes it from existence. Freeit specifies if it needs to be free'd or not
void drawWorld(World* world);//updateObjects checks each object for the 'update' flag, if true it will update OpenGL's knowledge of the object and then set the flag to zero