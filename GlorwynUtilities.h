#pragma once
#include <stdio.h>
#include <stdlib.h>

#define X 0
#define Y 1
#define Z 2
#define W 3


char* readFile(char* FileName);

void gError(char* errorMessage);

void normalizeQuat(float* quat);

float* quatMult(float* o, float* c);

float* quatConj(float q[4]);