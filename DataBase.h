#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "GGE2.h"



//the space allocation of the data of a button
#define M_BUTTON 0
#define M_ACTION 1
#define OUR_DESIRED_PAGE 7

#define MAINPAGE 0
#define SPLASHSCREEN 1
#define UPONE 2
#define UPTWO 3
#define UPTHREE 4
#define TEMPSTUFF 5

#define PAGECOUNT 6

int programState;
UI* temporaryStuff;

void clearTempStuff();
char* fileSelector();
long long int pageSwitcher(long long int data, short int clickData);

long long int accessKeyUpload(long long data, short int clickData);
long long int backendUpload(long long data, short int clickData);
long long int surveyUpload(long long data, short int clickData);

UnfinObj createButton(float x, float y, float xScale, float yScale, float spacing, float* baseRGB, float* innerRGB);
void startupDataBase(GLFWwindow* window);

