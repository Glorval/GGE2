#pragma once
#include "GGE2.h"


Object createObject(float* vertices, unsigned int* index, int vertSize, int indSize);
void drawObject(Object* shape);