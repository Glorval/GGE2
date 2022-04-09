#pragma once
#include "GGE2.h"
#define USE_BUILT_IN_FONT 1
#define LOAD_FONT 0

UIElement* font;

void setupUI(int flag);

UI* createUI();
void drawUI(UI* ui);
void runUI(UI* ui);
void insertElementIntoUI(UI* ui, UIElement* element);
void drawElement(UIElement* uiItem);
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, void* action, void* customAction, char defaultAction, int active, float clickArea[4]);