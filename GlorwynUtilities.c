#define _CRT_SECURE_NO_WARNINGS
#include "GlorwynUtilities.h"
#include <math.h>

#define LINEREADSIZE 100

char* readFile(char* FileName) {
	FILE* file = fopen(FileName, "r");
	if (file == NULL) {
		gError("Error: File failed to open.");
		printf("File: %s\n", FileName);
		return(NULL);
	}

	char* fileData = calloc(LINEREADSIZE + 1, sizeof(char));
	int position = 0;
	int prevpos = -1;
	int count = 0;
	//int space = LINEREADSIZE;
	while (1) {
		fgets(&fileData[position - count], LINEREADSIZE, file);
		count++;
		prevpos = position;
		position = ftell(file);
		fileData = realloc(fileData, position + LINEREADSIZE + 1);
		if (position == prevpos) {
			break;
		}
	}

	return(fileData);
}

void gError(char* errorMessage) {
	printf("\n\x1B[38;5;88m%s\x1B[0m\n", errorMessage);
}

void normalizeQuat(float* quat) {
	float temp = 0;
	temp = quat[0];
	temp = quat[1];
	temp = quat[2];
	temp = quat[3];
	float magnitude = sqrt((quat[0] * quat[0]) + (quat[1] * quat[1]) + (quat[2] * quat[2]) + (quat[3] * quat[3]));

	quat[0] /= magnitude;
	quat[1] /= magnitude;
	quat[2] /= magnitude;
	quat[3] /= magnitude;
}

float* quatMult(float* o, float* c) {
	float temp[4];
	temp[X_pos] = (o[X_pos] * c[X_pos]) - (o[Y_pos] * c[Y_pos]) - (o[Z_pos] * c[Z_pos]) - (o[W_pos] * c[W_pos]);
	temp[Y_pos] = (o[X_pos] * c[Y_pos]) + (o[Y_pos] * c[X_pos]) + (o[Z_pos] * c[W_pos]) - (o[W_pos] * c[Z_pos]);
	temp[Z_pos] = (o[X_pos] * c[Z_pos]) - (o[Y_pos] * c[W_pos]) + (o[Z_pos] * c[X_pos]) + (o[W_pos] * c[Y_pos]);
	temp[W_pos] = (o[X_pos] * c[W_pos]) + (o[Y_pos] * c[Z_pos]) - (o[Z_pos] * c[Y_pos]) + (o[W_pos] * c[X_pos]);

	c[X_pos] = temp[X_pos];
	c[Y_pos] = temp[Y_pos];
	c[Z_pos] = temp[Z_pos];
	c[W_pos] = temp[W_pos];

	return(temp);
}

float* quatMultRS(float* o, float* c) {
	float temp[4];
	temp[X_pos] = (o[X_pos] * c[X_pos]) - (o[Y_pos] * c[Y_pos]) - (o[Z_pos] * c[Z_pos]) - (o[W_pos] * c[W_pos]);
	temp[Y_pos] = (o[X_pos] * c[Y_pos]) + (o[Y_pos] * c[X_pos]) + (o[Z_pos] * c[W_pos]) - (o[W_pos] * c[Z_pos]);
	temp[Z_pos] = (o[X_pos] * c[Z_pos]) - (o[Y_pos] * c[W_pos]) + (o[Z_pos] * c[X_pos]) + (o[W_pos] * c[Y_pos]);
	temp[W_pos] = (o[X_pos] * c[W_pos]) + (o[Y_pos] * c[Z_pos]) - (o[Z_pos] * c[Y_pos]) + (o[W_pos] * c[X_pos]);

	o[X_pos] = temp[X_pos];
	o[Y_pos] = temp[Y_pos];
	o[Z_pos] = temp[Z_pos];
	o[W_pos] = temp[W_pos];

	return(temp);
}

double* quatMultD(double* o, double* c) {
	double temp[4];
	temp[X_pos] = (o[X_pos] * c[X_pos]) - (o[Y_pos] * c[Y_pos]) - (o[Z_pos] * c[Z_pos]) - (o[W_pos] * c[W_pos]);
	temp[Y_pos] = (o[X_pos] * c[Y_pos]) + (o[Y_pos] * c[X_pos]) + (o[Z_pos] * c[W_pos]) - (o[W_pos] * c[Z_pos]);
	temp[Z_pos] = (o[X_pos] * c[Z_pos]) - (o[Y_pos] * c[W_pos]) + (o[Z_pos] * c[X_pos]) + (o[W_pos] * c[Y_pos]);
	temp[W_pos] = (o[X_pos] * c[W_pos]) + (o[Y_pos] * c[Z_pos]) - (o[Z_pos] * c[Y_pos]) + (o[W_pos] * c[X_pos]);

	c[X_pos] = temp[X_pos];
	c[Y_pos] = temp[Y_pos];
	c[Z_pos] = temp[Z_pos];
	c[W_pos] = temp[W_pos];

	return(temp);
}

double* quatMultRSD(double* o, double* c) {
	double temp[4];
	temp[X_pos] = (o[X_pos] * c[X_pos]) - (o[Y_pos] * c[Y_pos]) - (o[Z_pos] * c[Z_pos]) - (o[W_pos] * c[W_pos]);
	temp[Y_pos] = (o[X_pos] * c[Y_pos]) + (o[Y_pos] * c[X_pos]) + (o[Z_pos] * c[W_pos]) - (o[W_pos] * c[Z_pos]);
	temp[Z_pos] = (o[X_pos] * c[Z_pos]) - (o[Y_pos] * c[W_pos]) + (o[Z_pos] * c[X_pos]) + (o[W_pos] * c[Y_pos]);
	temp[W_pos] = (o[X_pos] * c[W_pos]) + (o[Y_pos] * c[Z_pos]) - (o[Z_pos] * c[Y_pos]) + (o[W_pos] * c[X_pos]);

	o[X_pos] = temp[X_pos];
	o[Y_pos] = temp[Y_pos];
	o[Z_pos] = temp[Z_pos];
	o[W_pos] = temp[W_pos];

	return(temp);
}

float* quatMultNS(float* o, float* c) {
	float temp[4];
	temp[X_pos] = (o[X_pos] * c[X_pos]) - (o[Y_pos] * c[Y_pos]) - (o[Z_pos] * c[Z_pos]) - (o[W_pos] * c[W_pos]);
	temp[Y_pos] = (o[X_pos] * c[Y_pos]) + (o[Y_pos] * c[X_pos]) + (o[Z_pos] * c[W_pos]) - (o[W_pos] * c[Z_pos]);
	temp[Z_pos] = (o[X_pos] * c[Z_pos]) - (o[Y_pos] * c[W_pos]) + (o[Z_pos] * c[X_pos]) + (o[W_pos] * c[Y_pos]);
	temp[W_pos] = (o[X_pos] * c[W_pos]) + (o[Y_pos] * c[Z_pos]) - (o[Z_pos] * c[Y_pos]) + (o[W_pos] * c[X_pos]);

	return(temp);
}

float* quatConj(float q[4]) {
	q[Y_pos] = -q[Y_pos];
	q[Z_pos] = -q[Z_pos];
	q[W_pos] = -q[W_pos];
	return(q);
}