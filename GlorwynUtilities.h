#pragma once
#include <stdio.h>
#include <stdlib.h>

#define X_pos 0
#define Y_pos 1
#define Z_pos 2
#define W_pos 3


char* readFile(char* FileName);

void gError(char* errorMessage);

void normalizeQuat(float* quat);

float* quatMult(float* o, float* c);

float* quatMultRS(float* o, float* c);

float* quatMultNS(float* o, float* c);

float* quatConj(float q[4]);