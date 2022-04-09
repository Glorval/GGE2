#pragma once
#include "GGE2.h"

UI* createUI();
void drawUI(UI* ui);
void runUI(UI* ui);
void insertElementIntoUI(UI* ui, UIElement* element);
void drawElement(UIElement* uiItem);
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, void* action, int active, float clickArea[4]);