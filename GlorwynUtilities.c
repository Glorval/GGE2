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
	temp[X] = (o[X] * c[X]) - (o[Y] * c[Y]) - (o[Z] * c[Z]) - (o[W] * c[W]);
	temp[Y] = (o[X] * c[Y]) + (o[Y] * c[X]) + (o[Z] * c[W]) - (o[W] * c[Z]);
	temp[Z] = (o[X] * c[Z]) - (o[Y] * c[W]) + (o[Z] * c[X]) + (o[W] * c[Y]);
	temp[W] = (o[X] * c[W]) + (o[Y] * c[Z]) - (o[Z] * c[Y]) + (o[W] * c[X]);

	c[X] = temp[X];
	c[Y] = temp[Y];
	c[Z] = temp[Z];
	c[W] = temp[W];

	return(temp);
}

float* quatMultNS(float* o, float* c) {
	float temp[4];
	temp[X] = (o[X] * c[X]) - (o[Y] * c[Y]) - (o[Z] * c[Z]) - (o[W] * c[W]);
	temp[Y] = (o[X] * c[Y]) + (o[Y] * c[X]) + (o[Z] * c[W]) - (o[W] * c[Z]);
	temp[Z] = (o[X] * c[Z]) - (o[Y] * c[W]) + (o[Z] * c[X]) + (o[W] * c[Y]);
	temp[W] = (o[X] * c[W]) + (o[Y] * c[Z]) - (o[Z] * c[Y]) + (o[W] * c[X]);

	return(temp);
}

float* quatConj(float q[4]) {
	q[Y] = -q[Y];
	q[Z] = -q[Z];
	q[W] = -q[W];
	return(q);
}