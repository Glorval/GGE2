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

#define REPORTONE 6
#define REPORTTWO 7
#define REPORTTHREE 8

#define PAGECOUNT 6

#define ON_REPORT 1

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
void ourButtonPresses(GLFWwindow* window, int key, int scancode, int action, int mods);
void scrolling(GLFWwindow* window, double xoffset, double yoffset);
void drop_callback(GLFWwindow* window, int count, const char** paths);
void checkDragDrop();