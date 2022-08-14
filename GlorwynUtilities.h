#pragma once
#include <stdio.h>
#include <stdlib.h>


#define X_pos 0
#define Y_pos 1
#define Z_pos 2
#define W_pos 3
#define I_pos 4
#define J_pos 5
#define K_pos 6


char* readFile(char* FileName);

//prints, in red, the error surrounded by newlines
void gError(char* errorMessage);

//inclusive
int randNum(int lower, int upper);

void normalizeQuat(float* quat);

float* quatMult(float* o, float* c);

float* quatMultRS(float* o, float* c);

float* quatMultNS(float* o, float* c);

double* quatMultRSD(double* o, double* c);
double* quatMultD(double* o, double* c);

float* quatConj(float q[4]);

float vecLen3(const float vec[3]);
float dotP3(const float first[3], const float sec[3]);
float* crossP3(const float first[3], const float sec[3]);
void norm3(float* vec);