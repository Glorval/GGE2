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

float* quatMultFull(float* o, float* c) {
	float temp[4];
	float conj[4] = { o[0], -o[1], -o[2], -o[3] };
	//quatConj(conj);
	temp[X] = (o[X] * c[X]) - (o[Y] * c[Y]) - (o[Z] * c[Z]) - (o[W] * c[W]);
	temp[Y] = (o[X] * c[Y]) + (o[Y] * c[X]) + (o[Z] * c[W]) - (o[W] * c[Z]);
	temp[Z] = (o[X] * c[Z]) - (o[Y] * c[W]) + (o[Z] * c[X]) + (o[W] * c[Y]);
	temp[W] = (o[X] * c[W]) + (o[Y] * c[Z]) - (o[Z] * c[Y]) + (o[W] * c[X]);

	/*float result[4];
	result[X] = (temp[X] * conj[X]) - (temp[Y] * conj[Y]) - (temp[Z] * conj[Z]) - (temp[W] * conj[W]);
	result[Y] = (temp[X] * conj[Y]) + (temp[Y] * conj[X]) + (temp[Z] * conj[W]) - (temp[W] * conj[Z]);
	result[Z] = (temp[X] * conj[Z]) - (temp[Y] * conj[W]) + (temp[Z] * conj[X]) + (temp[W] * conj[Y]);
	result[W] = (temp[X] * conj[W]) + (temp[Y] * conj[Z]) - (temp[Z] * conj[Y]) + (temp[W] * conj[X]);
	*/
	c[X] = temp[X];
	c[Y] = temp[Y];
	c[Z] = temp[Z];
	c[W] = temp[W];

	return(temp);
}

vec4 quatMultNS(float* o, float* c) {
	float temp[4];
	temp[X] = (o[X] * c[X]) - (o[Y] * c[Y]) - (o[Z] * c[Z]) - (o[W] * c[W]);
	temp[Y] = (o[X] * c[Y]) + (o[Y] * c[X]) + (o[Z] * c[W]) - (o[W] * c[Z]);
	temp[Z] = (o[X] * c[Z]) - (o[Y] * c[W]) + (o[Z] * c[X]) + (o[W] * c[Y]);
	temp[W] = (o[X] * c[W]) + (o[Y] * c[Z]) - (o[Z] * c[Y]) + (o[W] * c[X]);

	return(*(vec4*)&temp);
}

/*
float* quatMultNS(float* o, float* c) {
	float temp[4];
	temp[X] = (o[X] * c[X]) - (o[Y] * c[Y]) - (o[Z] * c[Z]) - (o[W] * c[W]);
	temp[Y] = (o[X] * c[Y]) + (o[Y] * c[X]) + (o[Z] * c[W]) - (o[W] * c[Z]);
	temp[Z] = (o[X] * c[Z]) - (o[Y] * c[W]) + (o[Z] * c[X]) + (o[W] * c[Y]);
	temp[W] = (o[X] * c[W]) + (o[Y] * c[Z]) - (o[Z] * c[Y]) + (o[W] * c[X]);

	return(temp);
}*/

float* quatMultShortRightNS(float* o, float* c) {
	float debuga[4];
	debuga[0] = o[0];
	debuga[1] = o[1];
	debuga[2] = o[2];
	debuga[3] = o[3];

	float debugb[3];
	debugb[0] = c[0];
	debugb[1] = c[1];
	debugb[2] = c[2];
	float temp[4];
	temp[X] = (o[Y] * c[X]) - (o[Z] * c[Y]) - (o[W] * c[Z]);
	temp[Y] = (o[X] * c[X]) + (o[Z] * c[Z]) - (o[W] * c[Y]);
	temp[Z] = (o[X] * c[Y]) - (o[Y] * c[Z]) + (o[W] * c[X]);
	temp[W] = (o[X] * c[Z]) + (o[Y] * c[Y]) - (o[Z] * c[X]);

	return(temp);
}

float* quatConj(float q[4]) {
	q[Y] = -q[Y];
	q[Z] = -q[Z];
	q[W] = -q[W];
	return(q);
}

vec4 quatConjNS(float q[4]) {
	float temp[4];
	temp[0] = q[0];
	temp[1] = -q[1];
	temp[2] = -q[2];
	temp[3] = -q[3];
	return(*(vec4*)&temp);
}