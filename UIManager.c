#include "UIManager.h"
#include <string.h>

void convertPixelSpaceToOpenGL(float* input, int lines) {
	for (int cLine = 0; cLine < lines; cLine++) {
		input[cLine * 6] = input[cLine * 6] /(windX / 2);
		input[cLine * 6  + 1] = input[cLine * 6 + 1] / (windY / 2);
	}
}

//All fonts go +/- 40 on the Y axis, +/- 25 on the X axis to make a 50:80 x/y ratio
void setupUI(int flag) {
	font = calloc(128, sizeof(UnfinObj));//MEMORYTODO - can reduce consumption by several kb if done differently
	if (flag) {//Using the default 'built in' characterset
		float A[] = {
			0, 40,			0, 0.8, 0.8, 0.8,//0  
			0, 20,			0, 0.8, 0.8, 0.8,//1
			-25, -40,		0, 0.8, 0.8, 0.8,//2
			-15, -40,		0, 0.8, 0.8, 0.8,//3
			25, -40,		0, 0.8, 0.8, 0.8,//4
			15, -40,			0, 0.8, 0.8, 0.8,//5
			10, 0, 			0, 0.8, 0.8, 0.8,//6
			10, -8, 			0, 0.8, 0.8, 0.8,//7
			-10, 0, 			0, 0.8, 0.8, 0.8,//8
			-10, -8,			0, 0.8, 0.8, 0.8//9
		};
		convertPixelSpaceToOpenGL(A, 10);

		unsigned int Ainds[] = {
			0, 1, 2,		2, 3, 1,//left bar
			0, 1, 4,		4, 5, 1,//right bar
			6, 7, 8,		8, 9, 7//middle bar
		};
		font['A'] = createUnfinObjFromStatic(A, Ainds, (sizeof(A) / sizeof(float))/VERTEX_LENGTH, sizeof(Ainds) / sizeof(unsigned int));

		float a[] = {
			//stem
			19, 10,			0, 0.8, 0.8, 0.8,//0  
			11, 9,				0, 0.8, 0.8, 0.8,//1
			22, -30,		0, 0.8, 0.8, 0.8,//2
			14, -35,			0, 0.8, 0.8, 0.8,//3
			//base of stem
			25, -35,		0, 0.8, 0.8, 0.8,//4
			25, -40,		0, 0.8, 0.8, 0.8,//5
			//ring
			-25, -15,		0, 0.8, 0.8, 0.8,//6
			-15, -15,		0, 0.8, 0.8, 0.8//7
		};
		convertPixelSpaceToOpenGL(a, _countof(a) / VERTEX_LENGTH);
		unsigned int ainds[] = {
			0, 1, 2,		2, 3, 1,//main stem
			2, 4, 5,		5, 3, 2,//base stem connection
			0, 6, 1,		1, 6, 7,		3, 6, 7,		7, 2, 3// ring
		};
		//createUnfinObjFromStatic(aaaa, ainds, _countof(aaaa) / VERTEX_LENGTH, _countof(ainds));
		font['a'] = createUnfinObjFromStatic(a, ainds, _countof(a) / VERTEX_LENGTH, _countof(ainds));

	}
}

UI* createUI() {
	UI* newUI = calloc(1, sizeof(UI));
	newUI->elementCount = 0;
	newUI->elementListSize = 0;
	newUI->elements = NULL;

	return(newUI);
}


void drawUI(UI* ui) {
	int current = 0;
	glUniform1i(ProgramData.flagsLoc, 1);
	while (current < ui->elementCount) {
		if (ui->elements[current]->elementActive == 1) {
			drawElement(ui->elements[current]);
		}
		current++;
	}
}

void runUI(UI* ui) {
	int current = 0;
	glUniform1i(ProgramData.flagsLoc, 1);
	while (current < ui->elementCount) {
		if (ui->elements[current]->elementActive == 1) {
			Object* temp = ui->elements[current];
			drawElement(ui->elements[current]);
		}

		//UI actions
		if (ui->elements[current]->actionNeeded == 1 && ui->elements[current]->defaultAction != NO_ACTION) {
			ui->elements[current]->actionNeeded = COMMITTING_ACTION;
			switch (ui->elements[current]->defaultAction) {
				case ACTION:
					ui->elements[current]->data = ui->elements[current]->action(ui->elements[current]->data);
					break;
				case CUSTOM_ACTION:
					*ui->elements[current]->blockData = ui->elements[current]->customAction(*ui->elements[current]->blockData);
					break;
			}
			ui->elements[current]->actionNeeded = READY_FOR_ACTION;
		}

		current++;
	}
}

void insertElementIntoUI(UI* ui, UIElement* element){
	if (ui->elementCount == ui->elementListSize) {//This is because the list might be bigger than the current count from deleting elements 

		//POTENTIAL PERFORMANCE TODO- Could check the actual size of the list versus what we needand spare some realloc calls that might be redundant
		ui->elementCount++;
		ui->elementListSize = ui->elementCount;
		ui->elements = realloc(ui->elements, ui->elementCount * sizeof(UIElement*));
		ui->elements[ui->elementCount - 1] = element;
	}
}



void drawElement(UIElement* uiItem) {
	glBindVertexArray(uiItem->ID);
	glDrawElements(GL_TRIANGLES, uiItem->indexCount, GL_UNSIGNED_INT, 0);
}

//'click area' is leftmost, rightmost, bottommost, topmost
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, void* action, void* customAction, char defaultAction, int active, float clickArea[4]){
	//unsigned int VBO;
	UIElement* returnElement = calloc(1, sizeof(UIElement));
	//returnElement.ID = ID;

	returnElement->indexCount = indSize;
	glGenVertexArrays(1, &returnElement->ID);
	glGenBuffers(1, &returnElement->VBO);
	glGenBuffers(1, &returnElement->EBO);
	// bind the Vertex Array Element first, then bind and set vertex buffer(s), and then configure vertex attributes(s)->
	glBindVertexArray(returnElement->ID);

	glBindBuffer(GL_ARRAY_BUFFER, returnElement->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * VERTEX_SIZE * VERTEX_LENGTH, vertices, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnElement->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * IND_SIZE, index, GL_DYNAMIC_DRAW);

	// position attribute, first three
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute, second three (Obviously offset by three)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	returnElement->action = action;
	returnElement->customAction = customAction;
	returnElement->defaultAction = defaultAction;
	returnElement->elementActive = active;

	if (clickArea != NULL) {
		returnElement->clickArea[0] = (clickArea[0] + 1) * windX / 2;
		returnElement->clickArea[1] = (clickArea[1] + 1) * windX / 2;
		returnElement->clickArea[2] = ((clickArea[2] * -1) + 1) * windY / 2;
		returnElement->clickArea[3] = ((clickArea[3] * -1) + 1) * windY / 2;
	}	
	//printf("%f, %f, %f, %f\n", returnElement->clickArea[0], returnElement->clickArea[1], returnElement->clickArea[2], returnElement->clickArea[3]);


	return(returnElement);
}


//Font size is in pixels and for the height. X/Y pos are in opengl coords
UnfinObj createUnFinText(char* text, float xpos, float ypos, float fontSize) {
	int textLength = strlen(text);
	float xOffset = 0;
	float yOffset = 0;

	float fontHeight = (float)FONT_SIZE_Y / (float)windY;
	float fontWidth = (float)FONT_SIZE_X / (float)windX;

	UnfinObj returnData = { 0 };

	for (int cChar = 0; cChar < textLength; cChar++) {
		UnfinObj temp = { 0 };
		appendUnfinisheds(&temp, &font[text[cChar]]);//so as to not mess up the font

		printf("Cur Index: %d,\t", cChar);
		printf("Cur char: %c\n", text[cChar]);

		if (text[cChar] != '\0') {
			for (int cV = 0; cV < temp.vLineCount; cV++) {
				printf("Cur V: %d.   ", cV);
			
				temp.verts[cV * VERTEX_LENGTH] *= (fontSize / FONT_SIZE_Y);
				temp.verts[(cV * VERTEX_LENGTH) + 1] *= (fontSize / FONT_SIZE_Y);

				temp.verts[cV * VERTEX_LENGTH] += xpos + xOffset;
				temp.verts[(cV * VERTEX_LENGTH) + 1] += ypos + yOffset;
				
				printf("%f, %f\n", temp.verts[cV * VERTEX_LENGTH], temp.verts[(cV * VERTEX_LENGTH) + 1]);
			}
			xOffset += fontWidth;
			printf("\n\n");
			appendUnfinisheds(&returnData, &temp);
			free(temp.verts);
			free(temp.indices);
		} else {
			xOffset = 0;
			yOffset -= fontHeight; //minus becuase we need to go down not upsies
		}


	}

	return(returnData);
}