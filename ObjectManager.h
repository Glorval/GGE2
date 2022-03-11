#pragma once
#include "GGE2.h"
#include "GlorwynUtilities.h"


Object createObject(float* vertices, unsigned int* index, int vertSize, int indSize);
void drawObject(Object* shape);
void drawWorldObject(Object* shape, World* world);