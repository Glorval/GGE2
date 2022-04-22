#pragma once
#include "GGE2.h"

#define M_BUTTON 0
#define M_ACTION 1
#define OUR_DESIRED_PAGE 7

#define MAINPAGE 0
#define SPLASHSCREEN 1
#define UPONE 2

#define PAGECOUNT 3

int programState;
long long int pageSwitcher(long long int data);


UnfinObj createButton(float x, float y, float xScale, float yScale, float spacing, float* baseRGB, float* innerRGB);
long long int testFunction(long long int data);
void startupDataBase(GLFWwindow* window);

