#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <glad\glad.h>
#include <GLFW\glfw3.h>


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

#define IND_SIZE sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b
#define VERTEX_SIZE sizeof(float)//The size in bytes of a vertex, sizeof(float)

struct programData {
    int shaderID;
    int cordinatesLoc;
    int orientationLoc;
    int perspectiveLoc;
    int scaleLoc;
    int cameraPosLoc;
    int camAngleLoc;
};
struct programData ProgramData;

GLFWwindow* startup();
int setupShaders();


struct object {
    unsigned int ID, VBO, EBO;
    int indexCount;

    float position[7]; //x,y,z, quaternion orientation
    float scale;
    int updateOpenGLData; //Whether or not we need to refresh this object to OpenGL (For multithreading mainly)
};
typedef struct object Object;

struct world {
    Object** objects;//List of all items, ptp because it allows easy deletion of items
    int objectCount;
    int objectListSize;//size of the pointer
    int* objectRender;
    float camera[7];
    float up[4];
    float back[4];
    float left[4];
};
typedef struct world World;