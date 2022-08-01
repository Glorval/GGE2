#pragma once
#include "GGE2_1.h"
#include "GlorwynUtilities.h"


Object createStaticStandardObject(float* vertices, unsigned int* index, int vertSize, int indSize);
Object createStaticVectorObject(float* vertices, unsigned int* index, int vertSize, int indSize);
void drawStandardObject(Object* shape);
void drawVectorObject(Object* shape);

void drawWorldObject(Object* shape, World* world);

void naturallyCentreVertices(float* input, int lines, int linewidth);