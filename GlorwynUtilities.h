#pragma once
#include <stdio.h>
#include <stdlib.h>

#define X 0
#define Y 1
#define Z 2
#define W 3

struct Vec4 {
	float a[4];
};
typedef struct Vec4 vec4;

char* readFile(char* FileName);

void gError(char* errorMessage);

void normalizeQuat(float* quat);

float* quatMult(float* o, float* c);

float* quatMultFull(float* o, float* c);

vec4 quatMultNS(float* o, float* c);

float* quatMultShortRightNS(float* o, float* c);

float* quatConj(float q[4]);

vec4 quatConjNS(float q[4]);