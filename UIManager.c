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
			0, 15,			0, 0.8, 0.8, 0.8,//1
			-25, -40,		0, 0.8, 0.8, 0.8,//2
			-15, -40,		0, 0.8, 0.8, 0.8,//3
			25, -40,		0, 0.8, 0.8, 0.8,//4
			15, -40,			0, 0.8, 0.8, 0.8,//5
			10, 0, 			0, 0.8, 0.8, 0.8,//6
			10, -8, 			0, 0.8, 0.8, 0.8,//7
			-10, 0, 			0, 0.8, 0.8, 0.8,//8
			-10, -8,			0, 0.8, 0.8, 0.8//9
		};
		convertPixelSpaceToOpenGL(A, _countof(A)/VERTEX_LENGTH);

		unsigned int Ainds[] = {
			0, 1, 2,		2, 3, 1,//left bar
			0, 1, 4,		4, 5, 1,//right bar
			6, 7, 8,		8, 9, 7//middle bar
		};
		font['A'] = createUnfinObjFromStatic(A, Ainds, _countof(A)/VERTEX_LENGTH, _countof(Ainds));

		/*float a[] = {
			0, 20,			0, 0.8, 0.8, 0.8,//0  
			0, 8,				0, 0.8, 0.8, 0.8,//1
			-23, -40,		0, 0.8, 0.8, 0.8,//2
			-15, -40,		0, 0.8, 0.8, 0.8,//3
			23, -40,		0, 0.8, 0.8, 0.8,//4
			15, -40,			0, 0.8, 0.8, 0.8,//5
			10, -12, 		0, 0.8, 0.8, 0.8,//6
			10, -16, 		0, 0.8, 0.8, 0.8,//7
			-10, -12, 		0, 0.8, 0.8, 0.8,//8
			-10, -16,		0, 0.8, 0.8, 0.8//9
		};
		convertPixelSpaceToOpenGL(a, _countof(a) / VERTEX_LENGTH);

		unsigned int ainds[] = {
			0, 1, 2,		2, 3, 1,//left bar
			0, 1, 4,		4, 5, 1,//right bar
			6, 7, 8,		8, 9, 7//middle bar
		};
		font['a'] = createUnfinObjFromStatic(a, ainds, _countof(a) / VERTEX_LENGTH, _countof(ainds));*/

		float a[] = {
			//the o part of the a
			20, 5,			0, 0.8, 0.8, 0.8,//0 
			20, -3,			0, 0.8, 0.8, 0.8,//1
			-17, 5,			0, 0.8, 0.8, 0.8,//2
			-25, -3,			0, 0.8, 0.8, 0.8,//3
			-25, -32,		0, 0.8, 0.8, 0.8,//4
			-17, -40,		0, 0.8, 0.8, 0.8,//5
			20, -32,		0, 0.8, 0.8, 0.8,//6
			25, -40,		0, 0.8, 0.8, 0.8,//7

			//back stem
			20, 12,			0, 0.8, 0.8, 0.8,//8 
			10, 12,			0, 0.8, 0.8, 0.8,//9
			20, -40,			0, 0.8, 0.8, 0.8,//10
			10, -40,			0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(a, _countof(a) / VERTEX_LENGTH);

		unsigned int ainds[] = {
			0,1,2, 2,3,1, 2,4,5, 2,3,4, 6,7,5, 6,5,4,//the o part
			8,9,10, 9,10,11,//back stem
		};
		font['a'] = createUnfinObjFromStatic(a, ainds, _countof(a) / VERTEX_LENGTH, _countof(ainds));

		float B[] = {
			//back
			-25, 40,		0, 0.8, 0.8, 0.8,//0  
			-17, 40,			0, 0.8, 0.8, 0.8,//1  
			-25, -40,		0, 0.8, 0.8, 0.8,//2  
			-17, -40,		0, 0.8, 0.8, 0.8,//3 

			//bottom o part
			-20, 5,			0, 0.8, 0.8, 0.8,//4
			-20, -3,			0, 0.8, 0.8, 0.8,//5  
			17, 5,			0, 0.8, 0.8, 0.8,//6  
			25, -3,			0, 0.8, 0.8, 0.8,//7 
			25, -32,		0, 0.8, 0.8, 0.8,//8
			17, -40,			0, 0.8, 0.8, 0.8,//9
			-25, -32,		0, 0.8, 0.8, 0.8,//10
			-25, -40,		0, 0.8, 0.8, 0.8,//11

			//upper part that makes it a B not a b
			-25, 32,		0, 0.8, 0.8, 0.8,//12
			9, 40,			0, 0.8, 0.8, 0.8,//13
			17, 32,			0, 0.8, 0.8, 0.8,//14
			9, 5,				0, 0.8, 0.8, 0.8,//15		
		};
		convertPixelSpaceToOpenGL(B, _countof(B) / VERTEX_LENGTH);

		unsigned int Binds[] = {
			0,1,2, 2,1,3,//back
			4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8,//bottom o part
			0,12,13, 12,13,14, 13,14,15, 14,15,6,//top part
		};
		font['B'] = createUnfinObjFromStatic(B, Binds, _countof(B) / VERTEX_LENGTH, _countof(Binds));


		float b[] = {
			//back
			-25, 40,		0, 0.8, 0.8, 0.8,//0  
			-17, 40,			0, 0.8, 0.8, 0.8,//1  
			-25, -40,		0, 0.8, 0.8, 0.8,//2  
			-17, -40,		0, 0.8, 0.8, 0.8,//3 

			//bottom o part
			-20, 5,			0, 0.8, 0.8, 0.8,//4
			-20, -3,			0, 0.8, 0.8, 0.8,//5  
			17, 5,			0, 0.8, 0.8, 0.8,//6  
			25, -3,			0, 0.8, 0.8, 0.8,//7 
			25, -32,		0, 0.8, 0.8, 0.8,//8
			17, -40,			0, 0.8, 0.8, 0.8,//9
			-25, -32,		0, 0.8, 0.8, 0.8,//10
			-25, -40,		0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(b, _countof(b) / VERTEX_LENGTH);

		unsigned int binds[] = {
			0,1,2, 2,1,3,//back
			4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8//bottom o part
		};
		font['b'] = createUnfinObjFromStatic(b, binds, _countof(b) / VERTEX_LENGTH, _countof(binds));

		float C[] = {
			20, 40,			0, 0.8, 0.8, 0.8,//0  
			25, 32,			0, 0.8, 0.8, 0.8,//1
			-16, 40,			0, 0.8, 0.8, 0.8,//2
			-10, 32,			0, 0.8, 0.8, 0.8,//3  
			-25, 10,			0, 0.8, 0.8, 0.8,//4
			-17, 10,			0, 0.8, 0.8, 0.8,//5 
			-25, -10,		0, 0.8, 0.8, 0.8,//6
			-17, -10,		0, 0.8, 0.8, 0.8,//7 
			-16, -40,		0, 0.8, 0.8, 0.8,//8
			-10, -32,		0, 0.8, 0.8, 0.8,//9  
			20, -40,		0, 0.8, 0.8, 0.8,//10  
			25, -32,		0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(C, _countof(C) / VERTEX_LENGTH);

		unsigned int Cinds[] = {
			0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11,
		};
		font['C'] = createUnfinObjFromStatic(C, Cinds, _countof(C) / VERTEX_LENGTH, _countof(Cinds));



		float c[] = {
			20, 5,			0, 0.8, 0.8, 0.8,//0  
			25, -3,			0, 0.8, 0.8, 0.8,//1
			-16, 5,			0, 0.8, 0.8, 0.8,//2
			-12, -3,			0, 0.8, 0.8, 0.8,//3  
			-25, -10,		0, 0.8, 0.8, 0.8,//4
			-17, -10,		0, 0.8, 0.8, 0.8,//5 
			-25, -25,		0, 0.8, 0.8, 0.8,//6
			-17, -25,		0, 0.8, 0.8, 0.8,//7 
			-16, -40,		0, 0.8, 0.8, 0.8,//8
			-12, -32,		0, 0.8, 0.8, 0.8,//9  
			20, -40,		0, 0.8, 0.8, 0.8,//10  
			25, -32,		0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(c, _countof(c) / VERTEX_LENGTH);

		unsigned int cinds[] = {
			0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11,
		};
		font['c'] = createUnfinObjFromStatic(c, cinds, _countof(c) / VERTEX_LENGTH, _countof(cinds));


		float D[] = {
			-25, 40,		0, 0.8, 0.8, 0.8,//0  
			-18, 32,			0, 0.8, 0.8, 0.8,//1
			6, 40,			0, 0.8, 0.8, 0.8,//2
			2, 32,			0, 0.8, 0.8, 0.8,//3  
			25, 10,			0, 0.8, 0.8, 0.8,//4
			17, 8,			0, 0.8, 0.8, 0.8,//5 
			25, -10,			0, 0.8, 0.8, 0.8,//6
			17, -8,			0, 0.8, 0.8, 0.8,//7 
			6, -40,			0, 0.8, 0.8, 0.8,//8
			2, -32,			0, 0.8, 0.8, 0.8,//9  
			-25, -40,		0, 0.8, 0.8, 0.8,//10  
			-18, -32,		0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(D, _countof(D) / VERTEX_LENGTH);

		unsigned int Dinds[] = {
			0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11,//arcing part
			0,1,10, 1,10,11,//back
		};
		font['D'] = createUnfinObjFromStatic(D, Dinds, _countof(D) / VERTEX_LENGTH, _countof(Dinds));


		float d[] = {
			//back
			25, 40,			0, 0.8, 0.8, 0.8,//0  
			17, 40,			0, 0.8, 0.8, 0.8,//1  
			25, -40,		0, 0.8, 0.8, 0.8,//2  
			17, -40,			0, 0.8, 0.8, 0.8,//3 

			//bottom o part
			20, 5,			0, 0.8, 0.8, 0.8,//4
			20, -3,			0, 0.8, 0.8, 0.8,//5  
			-17, 5,			0, 0.8, 0.8, 0.8,//6  
			-25, -3,			0, 0.8, 0.8, 0.8,//7 
			-25, -32,		0, 0.8, 0.8, 0.8,//8
			-17, -40,		0, 0.8, 0.8, 0.8,//9
			25, -32,		0, 0.8, 0.8, 0.8,//10
			25, -40,		0, 0.8, 0.8, 0.8,//11
		};
		convertPixelSpaceToOpenGL(d, _countof(d) / VERTEX_LENGTH);

		unsigned int dinds[] = {
			0,1,2, 2,1,3,//back
			4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8//bottom o part
		};
		font['d'] = createUnfinObjFromStatic(d, dinds, _countof(d) / VERTEX_LENGTH, _countof(dinds));


		float E[] = {
			//back
			-25, 40,		0, 0.8, 0.8, 0.8,//0  
			-17, 40,			0, 0.8, 0.8, 0.8,//1  
			-25, -40,		0, 0.8, 0.8, 0.8,//2  
			-17, -40,		0, 0.8, 0.8, 0.8,//3 

			//spurs
			25, 40,			0, 0.8, 0.8, 0.8,//4
			20, 32,			0, 0.8, 0.8, 0.8,//5  
			-25, 32,		0, 0.8, 0.8, 0.8,//6  

			20, 4,			0, 0.8, 0.8, 0.8,//7
			-20, 4,			0, 0.8, 0.8, 0.8,//8  
			20, -4,			0, 0.8, 0.8, 0.8,//9  
			-20, -4,			0, 0.8, 0.8, 0.8,//10  

			20, -32,		0, 0.8, 0.8, 0.8,//11  
			25, -40,		0, 0.8, 0.8, 0.8,//12 
			-25, -32,		0, 0.8, 0.8, 0.8,//13  
		};
		convertPixelSpaceToOpenGL(E, _countof(E) / VERTEX_LENGTH);

		unsigned int Einds[] = {
			0,1,2, 2,1,3,//back
			1,4,5, 5,1,6,//spurs
			7,8,9, 8,9,10,
			11,12,13, 12,13,3
		};
		font['E'] = createUnfinObjFromStatic(E, Einds, _countof(E) / VERTEX_LENGTH, _countof(Einds));



		/*float a[] = {
			//stem
			11, 9,				0, 0.8, 0.8, 0.8,//0  
			15, 10,			0, 0.8, 0.8, 0.8,//1
			15, -31,			0, 0.8, 0.8, 0.8,//2
			19, -30,			0, 0.8, 0.8, 0.8,//3
			//stem base
			22, -33,		0, 0.8, 0.8, 0.8,//4
			19, -35,			0, 0.8, 0.8, 0.8,//5
			25, -34, 		0, 0.8, 0.8, 0.8,//6
			24, -36, 		0, 0.8, 0.8, 0.8,//7

			//The curved biot, aka pain part one, the realization hitting properly.
			14, 7, 			0, 0.8, 0.8, 0.8,//8 //these two are the 'top mount'
			14, 3,			0, 0.8, 0.8, 0.8,//9

			2, 6,				0, 0.8, 0.8, 0.8,//10  
			7, 2,				0, 0.8, 0.8, 0.8,//11
			-4, 1,				0, 0.8, 0.8, 0.8,//12
			-9, 2,			0, 0.8, 0.8, 0.8,//13
			-11, -4,			0, 0.8, 0.8, 0.8,//14
			-14, -3,			0, 0.8, 0.8, 0.8,//15
			-17, -12, 		0, 0.8, 0.8, 0.8,//16	//furthest left point, a point of mirroring
			-14, -12, 		0, 0.8, 0.8, 0.8,//17	//inner point for the furthest left
			-14, -21, 		0, 0.8, 0.8, 0.8,//18
			-11, -20, 		0, 0.8, 0.8, 0.8,//19
			-9, -26,			0, 0.8, 0.8, 0.8,//20
			-4, -25, 		0, 0.8, 0.8, 0.8,//21
			7, -26, 			0, 0.8, 0.8, 0.8,//22
			2, -30, 			0, 0.8, 0.8, 0.8,//23
			2, -27, 			0, 0.8, 0.8, 0.8,//24
			17, -29, 		0, 0.8, 0.8, 0.8,//25
			17, -25, 		0, 0.8, 0.8, 0.8,//26
		};
		convertPixelSpaceToOpenGL(a, _countof(a) / VERTEX_LENGTH);
		unsigned int ainds[] = {
			0,1,2, 2,3,1, //main stem block
			2,3,4, 4,5,2, 4,6,7, 7,4,5,//stem base
			8,9,10, 10,11,9, 10,11,12, 12,10,13, 13,15,12, 12,14,15, 15,14,16, 14,16,17,//top part of the loop
			16,17,19, 19,18,16, 19,18,20, 19,20,21, 20,21,23, 23,25,24, 23,24,21, 26,25,24
		};
		font['a'] = createUnfinObjFromStatic(a, ainds, _countof(a) / VERTEX_LENGTH, _countof(ainds));*/

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

	float fontHeight = (fontSize * ((float)FONT_SIZE_Y / ((float)windY / 2)))/ ((float)FONT_SIZE_Y* .95);//((float)FONT_SIZE_Y * fontSize) / ((float)windY);
	float fontWidth = (fontSize * ((float)FONT_SIZE_X / ((float)windX / 2)))/ ((float)FONT_SIZE_Y * .85);// ((float)FONT_SIZE_X * fontSize) / ((float)windX );

	UnfinObj returnData = { 0 };

	for (int cChar = 0; cChar < textLength; cChar++) {
		UnfinObj temp = { 0 };
		UnfinObj character = mergeUnfinisheds(temp, font[text[cChar]]);//so as to not mess up the font

		printf("Cur Index: %d,\t", cChar);
		printf("Cur char: %c\n", text[cChar]);

		if (text[cChar] != '\n') {
			for (int cV = 0; cV < character.vLineCount; cV++) {
				//printf("Cur V: %d.   ", cV);
			
				character.verts[cV * VERTEX_LENGTH] *= (fontSize / FONT_SIZE_Y);
				character.verts[(cV * VERTEX_LENGTH) + 1] *= (fontSize / FONT_SIZE_Y);

				character.verts[cV * VERTEX_LENGTH] += xpos + xOffset;
				character.verts[(cV * VERTEX_LENGTH) + 1] += ypos + yOffset;
				
				//printf("%f, %f\n", temp.verts[cV * VERTEX_LENGTH], temp.verts[(cV * VERTEX_LENGTH) + 1]);
			}
			xOffset += fontWidth;
			printf("\n\n");
			//appendUnfinisheds(&returnData, &temp);
			returnData = mergeUnfinisheds(returnData, character);
			free(character.verts);
			free(character.indices);
		} else {
			xOffset = 0;
			yOffset -= fontHeight; //minus becuase we need to go down not upsies
		}


	}

	return(returnData);
}