#pragma once
#include "GGE2_1.h"
#define USE_BUILT_IN_FONT 1
#define FONT_NAME NULL

#define FONT_SIZE_Y 80
#define FONT_SIZE_X 50
#define FONT_RATIO (FONT_SIZE_X/FONT_SIZE_Y)
#define convFromPixelX(pixels) pixels/((float)windX / (float)2)
#define convFromPixelY(pixels) pixels/((float)windY / (float)2)

//UI* font;
UnfinObj* font;

//Used when making fonts such that the font can be given in 'pixels' and thusly converted into OpenGL coords no matter what the set up window size is.
//Assumes it's in the format used, however, that is x/y/z/r/g/b on each line and length is how many lines
void convertPixelSpaceToOpenGL(float* input, int lines);

void setupUI(int flag);


UI* createUI();
void drawUI(UI* ui);
void runMasterUI();
void runUI(UI* ui);
void insertElementIntoUI(UI* ui, UIElement* element);
void removeElementFromUI(UI* ui, UIElement* element);
void drawElement(UIElement* uiItem);

//'click area' is leftmost, rightmost, bottommost, topmost
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, float* pos, void* action, void* customAction, char defaultAction, int active, float clickArea[4]);

UnfinObj createUnFinText(char* text, float xpos, float ypos, float fontSize, float* rgb);
UnfinObj createUnFinTextWithZ(char* text, float xpos, float ypos, float zpos, float fontSize, float* rgb);