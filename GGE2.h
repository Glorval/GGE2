#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>
//#include "GGE2.c"

#define APPLICATIONNAME "GGE2"

#define ERROR -1
#define SUCCESS 0

#define X 0
#define Y 1
#define Z 2
#define W 3
#define I 4
#define J 5
#define K 6

struct programData {
    int shaderID;
    int cordinatesLoc;
    int orientationLoc;
    int perspectiveLoc;
    int scaleLoc;
    float temp[3];
};

struct programData ProgramData;

struct Object {
    unsigned int ID, EBO;
    int indexCount;

    float position[7]; //x,y,z, quaternion orientation
    float scale;
    int updateOpenGLData; //Whether or not we need to refresh this object to OpenGL (For multithreading mainly)
};

GLFWwindow* startup();
int setupShaders();

struct Object createObject(float* vertices, unsigned int* index, int vertSize, int indSize);

void drawShape(struct Object* shape);


