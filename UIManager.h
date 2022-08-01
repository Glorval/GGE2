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

//'click area' is how far to the left, how far to the right, how far up, how far down, from 'position'.
//IE, if position is -1, 0, 0 and click area is '0, 0.2, 0.1, 0.1', the click area will go to the right 0.2, up and down 0.1 from the position.
//This is because the UI needs to stay at the given proportions, but the window may rescale. Doing things this way allows for 'side anchoring', set the position
//to be a side of the screen and have everything go away from the side and it'll always stay on the side and in proportion.
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, float* pos, void* action, int active, float clickArea[4]);
UIElement* createVectorElement(float* vertices, unsigned int* index, int vertSize, int indSize, float* pos, void* action, int active, float clickArea[4]);

UnfinObj createUnFinText(char* text, float xpos, float ypos, float fontSize, float* rgb);
UnfinObj createUnFinTextWithZ(char* text, float xpos, float ypos, float zpos, float fontSize, float* rgb);