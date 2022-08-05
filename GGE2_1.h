#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include "GlorwynMemory.h"
#include "GlorwynUtilities.h"

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <string.h>
#include <math.h>

/*
There are 3 types of objects:
Standard (3d solid stuff with colours on each vector)
Vector (3d vector things with no colours, rendered in line mode)
UI (Usually 2d stuff rendered in triangle mode like standard and with colours on each vector)
*/



#define countof(thing) sizeof(thing)/sizeof(thing[0])

#define windX 1200
#define windY 900

#define FOV (float)45


#define READY_FOR_ACTION 0 //Is ready to be activated
#define COMMITTING_ACTION -1 //Is in the process of being activated
#define AWAITING_ACTION 1 //Should be activated the next time we look at it

#define ACTION 0
#define CUSTOM_ACTION 1
#define NO_ACTION -1

#define APPLICATIONNAME "GGE2.1"//"GGE2"

#define ERROR -1
#define FAIL -1
#define SUCCESS 0

//#define X_pos 0
//#define Y_pos 1
//#define Z_pos 2
//#define W_pos 3
//#define I_pos 4
//#define J_pos 5
//#define K_pos 6

#define IND_SIZE sizeof(unsigned int)
#define VERTEX_LENGTH 6//The length of the vertices, 6 entries per,  x/y/z r/g/b
#define VERTEX_SIZE sizeof(float)//The size in bytes of a vertex, sizeof(float)

#define VECTOR_VERTEX_LENGTH 3//The length of the vertices, 3 entries per for just vector graphics

#define RENDER_MODE_NORMAL 0
#define RENDER_MODE_POS_ONLY 1
#define RENDER_MODE_VECTOR 2
#define RENDER_MODE_VECT_POS_ONLY 3

struct programData {
    int shaderID;
    int cordinatesLoc;
    int orientationLoc;
    int perspectiveLoc;
    int scaleLoc;
    int cameraPosLoc;
    int camAngleLoc;
    int flagsLoc;
    int colourLoc;
    int windowRatioLoc;
};
struct programData ProgramData;

GLFWwindow* startup();
int setupShaders();
void window_resize_handler(GLFWwindow* window, int width, int height);


struct object {
    unsigned int ID, VBO, EBO;
    int indexCount;

    float position[7]; //x,y,z, quaternion orientation
    float scale;
};
typedef struct object Object;

struct world {
    Object** objects;//List of all items, ptp because it allows easy deletion of items
    char worldType;//0 = full, 1 = vector
    int objectCount;
    int objectListSize;//size of the pointer
    int* renderObject;
    float camera[7];
    float up[4];
    float back[4];
    float left[4];
    float vecColour[4];
};
typedef struct world World;

struct datablock {
    char data[64];
};
typedef struct datablock Block;


struct uielement {
    unsigned int ID, VBO, EBO;
    int indexCount;
    float scale;
    short int elementActive;
    int actionNeeded;
    float clickArea[4];//left x, right x, top y, bottom y.
    float position[3];//xyz position on the screen
    char defaultAction;
    long long int data; //Can be used as a pointer, too
    short int clickData;
    long long int(*action)(void*, long long int, short int);//short int for the 'click data', like left click, right click, etc.
};
typedef struct uielement UIElement;

struct ui {
    UIElement** elements;//List of all items, ptp because it allows easier deletion of items
    int elementCount;
    int elementListSize;//size of the pointer
    int active;
    char renderMode;
    float vecColour[4];
};
typedef struct ui UI;

volatile UI** masterUIList;
volatile int masterUIListLength;


struct unfinishedObject {
    float* verts;
    unsigned int* indices;
    int vLineCount;
    int iCount;
    float scale;
};
typedef struct unfinishedObject UnfinObj;

void defaultMoustClick(GLFWwindow* window, int button, int action, int mods);
void defaultButtonPress(GLFWwindow* window, int key, int scancode, int action, int mods);

//simply fills the unfinobj with the pointers and stuff, no need to free this one after
UnfinObj fillUnfinObj(float* verts, unsigned int* inds, int vLen, int iCount);
//Used for easily creating objects, especially from hardcoded stuff that'd die when touched by realloc
UnfinObj createUnfinObjFromStatic(float* verts, unsigned int* inds, int vLen, int iCount);
UnfinObj freeUnfinObj(UnfinObj obj);

//Appends objTwo into objOne. Needs both to have their arrays dynamically allocated
UnfinObj appendUnfinisheds(UnfinObj* objOne, UnfinObj* objTwo);

UnfinObj appendVec(UnfinObj* objOne, UnfinObj* objTwo);


#include "ObjectManager.h"
#include "WorldManager.h"
#include "UIManager.h"