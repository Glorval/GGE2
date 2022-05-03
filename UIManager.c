#include "UIManager.h"
#include <string.h>

void convertPixelSpaceToOpenGL(float* input, int lines) {
	for (int cLine = 0; cLine < lines; cLine++) {
		input[cLine * 6] = input[cLine * 6] /(windX / 2);
		input[cLine * 6  + 1] = input[cLine * 6 + 1] / (windY / 2);
	}
}



//All fonts go +/- 40 on the Y axis, +/- 25 on the X axis to make a 50:80 x/y ratio

void setupSectionA() {
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
	convertPixelSpaceToOpenGL(A, _countof(A) / VERTEX_LENGTH);

	unsigned int Ainds[] = {
		0, 1, 2,		2, 3, 1,//left bar
		0, 1, 4,		4, 5, 1,//right bar
		6, 7, 8,		8, 9, 7//middle bar
	};
	font['A'] = createUnfinObjFromStatic(A, Ainds, _countof(A) / VERTEX_LENGTH, _countof(Ainds));

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
		12, 5,			0, 0.8, 0.8, 0.8,//9
		20, -40,		0, 0.8, 0.8, 0.8,//10
		12, -40,			0, 0.8, 0.8, 0.8,//11
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
		-17, 32,			0, 0.8, 0.8, 0.8,//1
		6, 40,			0, 0.8, 0.8, 0.8,//2
		2, 32,			0, 0.8, 0.8, 0.8,//3  
		25, 10,			0, 0.8, 0.8, 0.8,//4
		17, 8,			0, 0.8, 0.8, 0.8,//5 
		25, -10,			0, 0.8, 0.8, 0.8,//6
		17, -8,			0, 0.8, 0.8, 0.8,//7 
		6, -40,			0, 0.8, 0.8, 0.8,//8
		2, -32,			0, 0.8, 0.8, 0.8,//9  
		-25, -40,		0, 0.8, 0.8, 0.8,//10  
		-17, -32,		0, 0.8, 0.8, 0.8,//11
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

		20, 5,			0, 0.8, 0.8, 0.8,//7
		-20, 5,			0, 0.8, 0.8, 0.8,//8  
		20, -3,			0, 0.8, 0.8, 0.8,//9  
		-20, -3,			0, 0.8, 0.8, 0.8,//10  

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

	float e[] = {
		//main back curve making a c
		17, 5,			0, 0.8, 0.8, 0.8,//0
		25, -3,			0, 0.8, 0.8, 0.8,//1  
		-17, 5,			0, 0.8, 0.8, 0.8,//2 
		-25, -3,			0, 0.8, 0.8, 0.8,//3  
		21, -32,			0, 0.8, 0.8, 0.8,//4
		21, -40,			0, 0.8, 0.8, 0.8,//5 
		-25, -32,		0, 0.8, 0.8, 0.8,//6 
		-17, -40,		0, 0.8, 0.8, 0.8,//7  
		//the bar for the middle
		25, -11,			0, 0.8, 0.8, 0.8,//8 
		17, -19,			0, 0.8, 0.8, 0.8,//9  
		-22, -19,		0, 0.8, 0.8, 0.8,//10  
		-22, -11,		0, 0.8, 0.8, 0.8,//11  
	};
	convertPixelSpaceToOpenGL(e, _countof(e) / VERTEX_LENGTH);

	unsigned int einds[] = {
		0,1,2, 1,2,3,//top
		4,5,6, 5,6,7,//bottom
		2,3,6, 2,6,7,//back
		8,9,10, 10,11,8, 8,9,1, 1,0,9,//middle
	};
	font['e'] = createUnfinObjFromStatic(e, einds, _countof(e) / VERTEX_LENGTH, _countof(einds));


	float F[] = {
		//back
		-25, 40,		0, 0.8, 0.8, 0.8,//0  
		-17, 40,			0, 0.8, 0.8, 0.8,//1  
		-25, -40,		0, 0.8, 0.8, 0.8,//2  
		-17, -40,		0, 0.8, 0.8, 0.8,//3 

		//spurs
		25, 40,			0, 0.8, 0.8, 0.8,//4
		20, 32,			0, 0.8, 0.8, 0.8,//5  
		-25, 32,		0, 0.8, 0.8, 0.8,//6  

		20, 5,			0, 0.8, 0.8, 0.8,//7
		-20, 5,			0, 0.8, 0.8, 0.8,//8  
		20, -3,			0, 0.8, 0.8, 0.8,//9  
		-20, -3,			0, 0.8, 0.8, 0.8,//10  
	};
	convertPixelSpaceToOpenGL(F, _countof(F) / VERTEX_LENGTH);

	unsigned int Finds[] = {
		0,1,2, 2,1,3,//back
		1,4,5, 5,1,6,//spurs
		7,8,9, 8,9,10,
	};
	font['F'] = createUnfinObjFromStatic(F, Finds, _countof(F) / VERTEX_LENGTH, _countof(Finds));

	float f[] = {
		//back
		-20, 32,		0, 0.8, 0.8, 0.8,//0  
		-12, 40,			0, 0.8, 0.8, 0.8,//1  
		-20, -40,		0, 0.8, 0.8, 0.8,//2  
		-12, -40,		0, 0.8, 0.8, 0.8,//3 

		//spurs
		20, 40,			0, 0.8, 0.8, 0.8,//4
		15, 32,			0, 0.8, 0.8, 0.8,//5  

		15, 5,			0, 0.8, 0.8, 0.8,//6
		-25, 5,			0, 0.8, 0.8, 0.8,//7
		15, -3,			0, 0.8, 0.8, 0.8,//8
		-25, -3,			0, 0.8, 0.8, 0.8,//9
	};
	convertPixelSpaceToOpenGL(f, _countof(f) / VERTEX_LENGTH);

	unsigned int finds[] = {
		0,1,2, 2,1,3,//back
		1,4,5, 5,1,0,//spurs
		6,7,8, 7,8,9,
	};
	font['f'] = createUnfinObjFromStatic(f, finds, _countof(f) / VERTEX_LENGTH, _countof(finds));

	float G[] = {
		//thing is a big loop okay
		18, 40,			0, 0.8, 0.8, 0.8,//0  
		25, 32,			0, 0.8, 0.8, 0.8,//1   
		-18, 40,			0, 0.8, 0.8, 0.8,//2  
		-25, 32,		0, 0.8, 0.8, 0.8,//3  
		-18, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		18, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,		0, 0.8, 0.8, 0.8,//7  
		25, -3,			0, 0.8, 0.8, 0.8,//8  
		18, 5,			0, 0.8, 0.8, 0.8,//9
		-5, 5,			0, 0.8, 0.8, 0.8,//10  
		-5, -3,			0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(G, _countof(G) / VERTEX_LENGTH);

	unsigned int Ginds[] = {
		0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 6,7,8, 6,8,9, 8,9,10, 10,11,8,
	};
	font['G'] = createUnfinObjFromStatic(G, Ginds, _countof(G) / VERTEX_LENGTH, _countof(Ginds));

	float g[] = {
		//main back curve making a c
		-18, 5,			0, 0.8, 0.8, 0.8,//0
		-25, -3,			0, 0.8, 0.8, 0.8,//1  
		18, 5,			0, 0.8, 0.8, 0.8,//2 
		25, -3,			0, 0.8, 0.8, 0.8,//3  
		-21, -40,		0, 0.8, 0.8, 0.8,//4
		-21, -48,		0, 0.8, 0.8, 0.8,//5 
		25, -40,		0, 0.8, 0.8, 0.8,//6 
		18, -48,			0, 0.8, 0.8, 0.8,//7  
		//the bar for the middle
		-25, -19,		0, 0.8, 0.8, 0.8,//8 
		-18, -27,		0, 0.8, 0.8, 0.8,//9  
		22, -27,		0, 0.8, 0.8, 0.8,//10  
		22, -19,			0, 0.8, 0.8, 0.8,//11  
	};
	convertPixelSpaceToOpenGL(g, _countof(g) / VERTEX_LENGTH);

	unsigned int ginds[] = {
		0,1,2, 1,2,3,//top
		4,5,6, 5,6,7,//bottom
		2,3,6, 2,6,7,//back
		8,9,10, 10,11,8, 8,9,1, 1,0,9,//middle
	};
	font['g'] = createUnfinObjFromStatic(g, ginds, _countof(g) / VERTEX_LENGTH, _countof(ginds));
}

void setupSectionB() {
	float H[] = {
		//three bars, not complex
		-25, 40,		0, 0.8, 0.8, 0.8,//0
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3

		25, 40,			0, 0.8, 0.8, 0.8,//4
		17, 40,			0, 0.8, 0.8, 0.8,//5
		17, -40,			0, 0.8, 0.8, 0.8,//6
		25, -40,		0, 0.8, 0.8, 0.8,//7

		20, 5,			0, 0.8, 0.8, 0.8,//8
		20, -3,			0, 0.8, 0.8, 0.8,//9
		-20, 5,			0, 0.8, 0.8, 0.8,//10
		-20, -3,			0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(H, _countof(H) / VERTEX_LENGTH);

	unsigned int Hinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
		8,9,10, 9,10,11
	};
	font['H'] = createUnfinObjFromStatic(H, Hinds, _countof(H) / VERTEX_LENGTH, _countof(Hinds));


	float h[] = {
		//three bars, not complex
		-25, 40,		0, 0.8, 0.8, 0.8,//0
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3

		25, -3,			0, 0.8, 0.8, 0.8,//4
		17, 5,			0, 0.8, 0.8, 0.8,//5
		17, -40,			0, 0.8, 0.8, 0.8,//6
		25, -40,		0, 0.8, 0.8, 0.8,//7

		-20, 5,			0, 0.8, 0.8, 0.8,//8
		-20, -3,			0, 0.8, 0.8, 0.8,//9
	};
	convertPixelSpaceToOpenGL(h, _countof(h) / VERTEX_LENGTH);

	unsigned int hinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
		4,5,8, 8,4,9,
	};
	font['h'] = createUnfinObjFromStatic(h, hinds, _countof(h) / VERTEX_LENGTH, _countof(hinds));



	float _I[] = {
		//three bars, not complex
		-4, 40,			0, 0.8, 0.8, 0.8,//0
		4, 40,			0, 0.8, 0.8, 0.8,//1
		-4, -40,			0, 0.8, 0.8, 0.8,//2
		4, -40,			0, 0.8, 0.8, 0.8,//3

		25, 40,			0, 0.8, 0.8, 0.8,//4
		25, 32,			0, 0.8, 0.8, 0.8,//5
		-25, 32,		0, 0.8, 0.8, 0.8,//6
		-25, 40,		0, 0.8, 0.8, 0.8,//7

		25, -40,		0, 0.8, 0.8, 0.8,//8
		25, -32,		0, 0.8, 0.8, 0.8,//9
		-25, -32,		0, 0.8, 0.8, 0.8,//10
		-25, -40,		0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(_I, _countof(_I) / VERTEX_LENGTH);

	unsigned int Iinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
		8,9,10, 8,10,11,
	};
	font['I'] = createUnfinObjFromStatic(_I, Iinds, _countof(_I) / VERTEX_LENGTH, _countof(Iinds));

	float i[] = {
		//a shaft and ball xdddd
		-4, 5,			0, 0.8, 0.8, 0.8,//0
		4, 5,				0, 0.8, 0.8, 0.8,//1
		-4, -40,			0, 0.8, 0.8, 0.8,//2
		4, -40,			0, 0.8, 0.8, 0.8,//3

		4, 12,			0, 0.8, 0.8, 0.8,//4
		-4, 12,			0, 0.8, 0.8, 0.8,//5
		4, 20,			0, 0.8, 0.8, 0.8,//6
		-4, 20,			0, 0.8, 0.8, 0.8,//7
	};
	convertPixelSpaceToOpenGL(i, _countof(i) / VERTEX_LENGTH);

	unsigned int iinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 5,6,7,
	};
	font['i'] = createUnfinObjFromStatic(i, iinds, _countof(i) / VERTEX_LENGTH, _countof(iinds));

	float _J[] = {
		//middle bar
		18, 40,			0, 0.8, 0.8, 0.8,//0
		10, 40,			0, 0.8, 0.8, 0.8,//1
		18, -32,			0, 0.8, 0.8, 0.8,//2
		10, -40,			0, 0.8, 0.8, 0.8,//3

		//top bar
		25, 40,			0, 0.8, 0.8, 0.8,//4
		25, 32,			0, 0.8, 0.8, 0.8,//5
		-25, 32,		0, 0.8, 0.8, 0.8,//6
		-25, 40,		0, 0.8, 0.8, 0.8,//7

		//base
		-25, -32,		0, 0.8, 0.8, 0.8,//8
		-18, -40,		0, 0.8, 0.8, 0.8,//9

		//lil up stem on the left of the J
		-25, -11,		0, 0.8, 0.8, 0.8,//10
		-18, -11,		0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(_J, _countof(_J) / VERTEX_LENGTH);

	unsigned int Jinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
		2,3,8, 3,8,9,
		8,9,10, 9,10,11,
	};
	font['J'] = createUnfinObjFromStatic(_J, Jinds, _countof(_J) / VERTEX_LENGTH, _countof(Jinds));

	float j[] = {
		10, 5,			0, 0.8, 0.8, 0.8,//0
		18, 5,			0, 0.8, 0.8, 0.8,//1
		10, -48,			0, 0.8, 0.8, 0.8,//2
		18, -40,			0, 0.8, 0.8, 0.8,//3  

		18, 12,			0, 0.8, 0.8, 0.8,//4
		10, 12,			0, 0.8, 0.8, 0.8,//5
		18, 20,			0, 0.8, 0.8, 0.8,//6
		10, 20,			0, 0.8, 0.8, 0.8,//7

		-10, -48,		0, 0.8, 0.8, 0.8,//8  
		-18, -40,		0, 0.8, 0.8, 0.8,//9  
		-10, -24,		0, 0.8, 0.8, 0.8,//10  
		-18, -32,		0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(j, _countof(j) / VERTEX_LENGTH);

	unsigned int jinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 5,6,7,
		2,3,8, 3,8,9, 8,9,10, 9,10,11,
	};
	font['j'] = createUnfinObjFromStatic(j, jinds, _countof(j) / VERTEX_LENGTH, _countof(jinds));

	float _K[] = {
		//back
		-25, 40,		0, 0.8, 0.8, 0.8,//0   
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3  

		//outer
		15, 40,			0, 0.8, 0.8, 0.8,//4   
		25, 40,			0, 0.8, 0.8, 0.8,//5   
		-25, 0,			0, 0.8, 0.8, 0.8,//6   
		-15, 0,			0, 0.8, 0.8, 0.8,//7   
		15, -40,			0, 0.8, 0.8, 0.8,//8   
		25, -40,		0, 0.8, 0.8, 0.8,//9   
	};
	convertPixelSpaceToOpenGL(_K, _countof(_K) / VERTEX_LENGTH);

	unsigned int Kinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 5,6,7,
		6,7,8, 7,8,9,
	};
	font['K'] = createUnfinObjFromStatic(_K, Kinds, _countof(_K) / VERTEX_LENGTH, _countof(Kinds));

	float k[] = {
		//back
		-25, 32,		0, 0.8, 0.8, 0.8,//0   
		-17, 32,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3  

		//outer
		10, 19,			0, 0.8, 0.8, 0.8,//4   
		20, 19,			0, 0.8, 0.8, 0.8,//5   
		-25, -16,		0, 0.8, 0.8, 0.8,//6   
		-20, -21,		0, 0.8, 0.8, 0.8,//7   

		-20, -10,		0, 0.8, 0.8, 0.8,//8 
		-10, -10,		0, 0.8, 0.8, 0.8,//9
		10, -40,			0, 0.8, 0.8, 0.8,//10   
		20, -40,		0, 0.8, 0.8, 0.8,//11   
	};
	convertPixelSpaceToOpenGL(k, _countof(k) / VERTEX_LENGTH);

	unsigned int kinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 5,6,7,
		8,9,10, 9,10,11,
	};
	font['k'] = createUnfinObjFromStatic(k, kinds, _countof(k) / VERTEX_LENGTH, _countof(kinds));

	float L[] = {
		//back
		-25, 40,		0, 0.8, 0.8, 0.8,//0   
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2  
		-17, -40,		0, 0.8, 0.8, 0.8,//3  
		//base
		-25, -32,		0, 0.8, 0.8, 0.8,//4  
		25, -32,		0, 0.8, 0.8, 0.8,//5  
		25, -40,		0, 0.8, 0.8, 0.8,//6  
	};
	convertPixelSpaceToOpenGL(L, _countof(L) / VERTEX_LENGTH);

	unsigned int Linds[] = {
		0,1,2, 1,2,3,//back
		2,4,6, 4,5,6,//base
	};
	font['L'] = createUnfinObjFromStatic(L, Linds, _countof(L) / VERTEX_LENGTH, _countof(Linds));

	float l[] = {
		//back
		-9, 32,			0, 0.8, 0.8, 0.8,//0   
		-1, 40,			0, 0.8, 0.8, 0.8,//1
		-9, -40,		0, 0.8, 0.8, 0.8,//2
		-1, -40,			0, 0.8, 0.8, 0.8,//3  
		//lil protrusion to help with spacing
		10, -40,			0, 0.8, 0.8, 0.8,//4  
		-1, -32,			0, 0.8, 0.8, 0.8,//5  
		-1, -32,			0, 0.8, 0.8, 0.8,//6  
	};
	convertPixelSpaceToOpenGL(l, _countof(l) / VERTEX_LENGTH);

	unsigned int linds[] = {
		0,1,2, 1,2,3,
		3,4,6,
	};
	font['l'] = createUnfinObjFromStatic(l, linds, _countof(l) / VERTEX_LENGTH, _countof(linds));

	float M[] = {
		//back
		-25, 40,		0, 0.8, 0.8, 0.8,//0   
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3  

		//front
		25, 40,			0, 0.8, 0.8, 0.8,//4  
		17, 40,			0, 0.8, 0.8, 0.8,//5
		25, -40,		0, 0.8, 0.8, 0.8,//6
		17, -40,			0, 0.8, 0.8, 0.8,//7

		//middle
		0, -40,			0, 0.8, 0.8, 0.8,//8   //-10
		0, -20,				0, 0.8, 0.8, 0.8,//9   //-6
	};
	convertPixelSpaceToOpenGL(M, _countof(M) / VERTEX_LENGTH);

	unsigned int Minds[] = {
		0,1,2, 1,2,3,//back
		4,5,6, 5,6,7,//front
		4,5,9, 8,9,4, 0,1,9, 8,9,0,
	};
	font['M'] = createUnfinObjFromStatic(M, Minds, _countof(M) / VERTEX_LENGTH, _countof(Minds));

	/*float m[] = {//old m that looks like a mini M
		//back
		-25, 5,		0, 0.8, 0.8, 0.8,//0
		-17, 5,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3

		//front
		25, 5,			0, 0.8, 0.8, 0.8,//4
		17, 5,			0, 0.8, 0.8, 0.8,//5
		25, -40,		0, 0.8, 0.8, 0.8,//6
		17, -40,			0, 0.8, 0.8, 0.8,//7

		//middle
		0, -40,			0, 0.8, 0.8, 0.8,//8
		0, -24,				0, 0.8, 0.8, 0.8,//9
	};
	convertPixelSpaceToOpenGL(m, _countof(m) / VERTEX_LENGTH);

	unsigned int minds[] = {
		0,1,2, 1,2,3,//back
		4,5,6, 5,6,7,//front
		4,5,9, 8,9,4, 0,1,9, 8,9,0,
	};
	font['m'] = createUnfinObjFromStatic(m, minds, _countof(m) / VERTEX_LENGTH, _countof(minds));*/

	float m[] = {
		//back stem
		-25, 12,			0, 0.8, 0.8, 0.8,//0   
		-17, 5,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3
		//arc across
		-25, 5,			0, 0.8, 0.8, 0.8,//4   
		-25, -3,			0, 0.8, 0.8, 0.8,//5   
		17, 5,			0, 0.8, 0.8, 0.8,//6   
		25, -3,			0, 0.8, 0.8, 0.8,//7   
		17, -40,			0, 0.8, 0.8, 0.8,//8   
		25, -40,		0, 0.8, 0.8, 0.8,//9   
		//middle stem
		-4, 5,			0, 0.8, 0.8, 0.8,//10   
		-4, -40,			0, 0.8, 0.8, 0.8,//11   
		4, 5,				0, 0.8, 0.8, 0.8,//12
		4, -40,			0, 0.8, 0.8, 0.8,//13   
	};
	convertPixelSpaceToOpenGL(m, _countof(m) / VERTEX_LENGTH);

	unsigned int minds[] = {
		0,1,2, 2,3,1,//back stem
		4,5,6, 5,6,7, 7,8,9, 6,7,8,//arc across
		10,11,12, 11,12,13,//middle stem
	};
	font['m'] = createUnfinObjFromStatic(m, minds, _countof(m) / VERTEX_LENGTH, _countof(minds));

	float N[] = {
		//back
		-25, 40,		0, 0.8, 0.8, 0.8,//0   
		-17, 40,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3  

		//front
		25, 40,			0, 0.8, 0.8, 0.8,//4  
		17, 40,			0, 0.8, 0.8, 0.8,//5
		25, -40,		0, 0.8, 0.8, 0.8,//6
		17, -40,			0, 0.8, 0.8, 0.8,//7

	};
	convertPixelSpaceToOpenGL(N, _countof(N) / VERTEX_LENGTH);

	unsigned int Ninds[] = {
		0,1,2, 1,2,3,//back
		4,5,6, 5,6,7,//front
		0,1,6, 0,6,7,//cross section
	};
	font['N'] = createUnfinObjFromStatic(N, Ninds, _countof(N) / VERTEX_LENGTH, _countof(Ninds));

	float n[] = {
		//back stem
		-25, 12,			0, 0.8, 0.8, 0.8,//0   
		-17, 5,			0, 0.8, 0.8, 0.8,//1
		-25, -40,		0, 0.8, 0.8, 0.8,//2
		-17, -40,		0, 0.8, 0.8, 0.8,//3
		//arc across
		-25, 5,			0, 0.8, 0.8, 0.8,//4   
		-25, -3,			0, 0.8, 0.8, 0.8,//5   
		17, 5,			0, 0.8, 0.8, 0.8,//6   
		25, -3,			0, 0.8, 0.8, 0.8,//7   
		17, -40,			0, 0.8, 0.8, 0.8,//8   
		25, -40,		0, 0.8, 0.8, 0.8,//9   
	};
	convertPixelSpaceToOpenGL(n, _countof(n) / VERTEX_LENGTH);

	unsigned int ninds[] = {
		0,1,2, 2,3,1,//back stem
		4,5,6, 5,6,7, 7,8,9, 6,7,8//arc across
	};
	font['n'] = createUnfinObjFromStatic(n, ninds, _countof(n) / VERTEX_LENGTH, _countof(ninds));


	float O[] = {
		//thing is a big loop okay
		17, 40,			0, 0.8, 0.8, 0.8,//0  
		25, 32,			0, 0.8, 0.8, 0.8,//1   
		-17, 40,			0, 0.8, 0.8, 0.8,//2  
		-25, 32,		0, 0.8, 0.8, 0.8,//3  
		-17, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		17, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,		0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(O, _countof(O) / VERTEX_LENGTH);

	unsigned int Oinds[] = {
		0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,
	};
	font['O'] = createUnfinObjFromStatic(O, Oinds, _countof(O) / VERTEX_LENGTH, _countof(Oinds));


	float o[] = {
		//thing is a big loop okay
		17, 5,			0, 0.8, 0.8, 0.8,//0  
		25, -3,			0, 0.8, 0.8, 0.8,//1   
		-17, 5,			0, 0.8, 0.8, 0.8,//2  
		-25, -3,			0, 0.8, 0.8, 0.8,//3  
		-17, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		17, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,		0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(o, _countof(o) / VERTEX_LENGTH);

	unsigned int oinds[] = {
		0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,
	};
	font['o'] = createUnfinObjFromStatic(o, oinds, _countof(o) / VERTEX_LENGTH, _countof(oinds));

	float P[] = {
		//back
		-25, -40,		0, 0.8, 0.8, 0.8,//0  
		-17, -40,		0, 0.8, 0.8, 0.8,//1  
		-25, 40,		0, 0.8, 0.8, 0.8,//2  
		-17, 40,			0, 0.8, 0.8, 0.8,//3 

		//o part
		-20, -5,			0, 0.8, 0.8, 0.8,//4
		-20, 3,			0, 0.8, 0.8, 0.8,//5  
		17, -5,			0, 0.8, 0.8, 0.8,//6  
		25, 3,			0, 0.8, 0.8, 0.8,//7 
		25, 32,			0, 0.8, 0.8, 0.8,//8
		17, 40,			0, 0.8, 0.8, 0.8,//9
		-25, 32,		0, 0.8, 0.8, 0.8,//10
		-25, 40,		0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(P, _countof(P) / VERTEX_LENGTH);

	unsigned int Pinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8//bottom o part
	};
	font['P'] = createUnfinObjFromStatic(P, Pinds, _countof(P) / VERTEX_LENGTH, _countof(Pinds));

	float p[] = {
		//back
		-25, -48,		0, 0.8, 0.8, 0.8,//0  
		-17, -48,		0, 0.8, 0.8, 0.8,//1  
		-25, -3,			0, 0.8, 0.8, 0.8,//2  
		-17, 5,			0, 0.8, 0.8, 0.8,//3 

		//bottom o part
		-20, -27,		0, 0.8, 0.8, 0.8,//4
		-20, -19,		0, 0.8, 0.8, 0.8,//5  
		17, -27,			0, 0.8, 0.8, 0.8,//6  
		25, -19,			0, 0.8, 0.8, 0.8,//7 
		25, -3,			0, 0.8, 0.8, 0.8,//8
		17, 5,			0, 0.8, 0.8, 0.8,//9
		-25, -3,			0, 0.8, 0.8, 0.8,//10
	};
	convertPixelSpaceToOpenGL(p, _countof(p) / VERTEX_LENGTH);

	unsigned int pinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,3,9, 10,9,8//bottom o part
	};
	font['p'] = createUnfinObjFromStatic(p, pinds, _countof(p) / VERTEX_LENGTH, _countof(pinds));

	float Q[] = {
		//thing is a big loop okay
		10, 40,			0, 0.8, 0.8, 0.8,//0  
		18, 32,			0, 0.8, 0.8, 0.8,//1   
		-18, 40,			0, 0.8, 0.8, 0.8,//2  
		-25, 32,		0, 0.8, 0.8, 0.8,//3  
		-18, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		10, -40,			0, 0.8, 0.8, 0.8,//6  
		18, -32,			0, 0.8, 0.8, 0.8,//7 

		//lil stem
		25, -48,		0, 0.8, 0.8, 0.8,//8  
		17, -48,			0, 0.8, 0.8, 0.8,//9  
	};
	convertPixelSpaceToOpenGL(Q, _countof(Q) / VERTEX_LENGTH);

	unsigned int Qinds[] = {
		0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,//big loopy bit
		6,7,9, 7,8,9,
	};
	font['Q'] = createUnfinObjFromStatic(Q, Qinds, _countof(Q) / VERTEX_LENGTH, _countof(Qinds));

	float q[] = {
		//back
		25, -48,		0, 0.8, 0.8, 0.8,//0  
		17, -48,			0, 0.8, 0.8, 0.8,//1  
		25, -3,			0, 0.8, 0.8, 0.8,//2  
		17, 5,			0, 0.8, 0.8, 0.8,//3 

		//bottom o part
		20, -27,		0, 0.8, 0.8, 0.8,//4
		20, -19,			0, 0.8, 0.8, 0.8,//5  
		-17, -27,		0, 0.8, 0.8, 0.8,//6  
		-25, -19,		0, 0.8, 0.8, 0.8,//7 
		-25, -3,			0, 0.8, 0.8, 0.8,//8
		-17, 5,			0, 0.8, 0.8, 0.8,//9
		25, -3,			0, 0.8, 0.8, 0.8,//10
	};
	convertPixelSpaceToOpenGL(q, _countof(q) / VERTEX_LENGTH);

	unsigned int qinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,3,9, 10,9,8//bottom o part
	};
	font['q'] = createUnfinObjFromStatic(q, qinds, _countof(q) / VERTEX_LENGTH, _countof(qinds));

	float R[] = {
		//back
		-25, -40,		0, 0.8, 0.8, 0.8,//0  
		-17, -40,		0, 0.8, 0.8, 0.8,//1  
		-25, 40,		0, 0.8, 0.8, 0.8,//2  
		-17, 40,			0, 0.8, 0.8, 0.8,//3 

		//o part
		-20, -5,			0, 0.8, 0.8, 0.8,//4
		-18, 3,			0, 0.8, 0.8, 0.8,//5  
		17, -5,			0, 0.8, 0.8, 0.8,//6  
		25, 3,			0, 0.8, 0.8, 0.8,//7 
		25, 32,			0, 0.8, 0.8, 0.8,//8
		17, 40,			0, 0.8, 0.8, 0.8,//9
		-25, 32,		0, 0.8, 0.8, 0.8,//10
		-25, 40,		0, 0.8, 0.8, 0.8,//11

		//angled stem that differentiates us from a P
		-10, -5,			0, 0.8, 0.8, 0.8,//12
		0, -5,			0, 0.8, 0.8, 0.8,//13  
		15, -40,			0, 0.8, 0.8, 0.8,//14
		25, -40,		0, 0.8, 0.8, 0.8,//15 
	};
	convertPixelSpaceToOpenGL(R, _countof(R) / VERTEX_LENGTH);

	unsigned int Rinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8,//o part
		12,13,14, 13,14,15,//bottom stem
	};
	font['R'] = createUnfinObjFromStatic(R, Rinds, _countof(R) / VERTEX_LENGTH, _countof(Rinds));

	float r[] = {
		//back stem
		-21, 12,			0, 0.8, 0.8, 0.8,//0   
		-13, 5,			0, 0.8, 0.8, 0.8,//1
		-21, -40,		0, 0.8, 0.8, 0.8,//2
		-13, -40,		0, 0.8, 0.8, 0.8,//3
		//arc across
		-17, 5,			0, 0.8, 0.8, 0.8,//4   
		-17, -3,			0, 0.8, 0.8, 0.8,//5   
		13, 5,			0, 0.8, 0.8, 0.8,//6   
		21, -3,			0, 0.8, 0.8, 0.8,//7   
		13, -10,			0, 0.8, 0.8, 0.8,//8   
		21, -10,			0, 0.8, 0.8, 0.8,//9   
	};
	convertPixelSpaceToOpenGL(r, _countof(r) / VERTEX_LENGTH);

	unsigned int rinds[] = {
		0,1,2, 2,3,1,//back stem
		4,5,6, 5,6,7, 7,8,9, 6,7,8//arc across
	};
	font['r'] = createUnfinObjFromStatic(r, rinds, _countof(r) / VERTEX_LENGTH, _countof(rinds));
}

void setupSectionC() {
	/*verboten S
		float S[] = {
			//back
			-5, 35,			0, 0.8, 0.8, 0.8,//0
			5, 40,			0, 0.8, 0.8, 0.8,//1
			-10, -10,			0, 0.8, 0.8, 0.8,//2
			0, -5,			0, 0.8, 0.8, 0.8,//3
			//front
			-5, -40,			0, 0.8, 0.8, 0.8,//4
			5, -35,			0, 0.8, 0.8, 0.8,//5
			0, 5,			0, 0.8, 0.8, 0.8,//6
			10, 10,			0, 0.8, 0.8, 0.8,//7
		};
		convertPixelSpaceToOpenGL(S, _countof(S) / VERTEX_LENGTH);

		unsigned int Sinds[] = {
			0,1,2, 2,3,1,//left
			4,5,6, 5,6,7,
		};
		font['S'] = createUnfinObjFromStatic(S, Sinds, _countof(S) / VERTEX_LENGTH, _countof(Sinds));*/

	float S[] = {
		//double arcing loop
		25, 32,			0, 0.8, 0.8, 0.8,//0   
		17, 40,			0, 0.8, 0.8, 0.8,//1   
		-17, 40,			0, 0.8, 0.8, 0.8,//2   
		-25, 32,		0, 0.8, 0.8, 0.8,//3   

		-25, 5,			0, 0.8, 0.8, 0.8,//4   
		-17, -3,			0, 0.8, 0.8, 0.8,//5   
		25, -3,			0, 0.8, 0.8, 0.8,//6   
		17, 5,			0, 0.8, 0.8, 0.8,//7   

		25, -32,		0, 0.8, 0.8, 0.8,//8   
		17, -40,			0, 0.8, 0.8, 0.8,//9   

		-25, -32,		0, 0.8, 0.8, 0.8,//10   
		-17, -40,		0, 0.8, 0.8, 0.8,//11   
	};
	convertPixelSpaceToOpenGL(S, _countof(S) / VERTEX_LENGTH);

	unsigned int Sinds[] = {
		0,1,2, 0,2,3, 2,3,4, 2,4,5, 4,5,6, 4,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11
	};
	font['S'] = createUnfinObjFromStatic(S, Sinds, _countof(S) / VERTEX_LENGTH, _countof(Sinds));

	float s[] = {
		//double arcing loop
		25, -3,			0, 0.8, 0.8, 0.8,//0   
		17, 5,			0, 0.8, 0.8, 0.8,//1   
		-17, 5,			0, 0.8, 0.8, 0.8,//2   
		-25, -3,		0, 0.8, 0.8, 0.8,//3   

		-25, -11,			0, 0.8, 0.8, 0.8,//4   
		-17, -19,			0, 0.8, 0.8, 0.8,//5   
		25, -19,			0, 0.8, 0.8, 0.8,//6   
		17, -11,			0, 0.8, 0.8, 0.8,//7   

		25, -32,		0, 0.8, 0.8, 0.8,//8   
		17, -40,			0, 0.8, 0.8, 0.8,//9   

		-25, -32,		0, 0.8, 0.8, 0.8,//10   
		-17, -40,		0, 0.8, 0.8, 0.8,//11   
	};
	convertPixelSpaceToOpenGL(s, _countof(s) / VERTEX_LENGTH);

	unsigned int sinds[] = {
		0,1,2, 0,2,3, 2,3,4, 2,4,5, 4,5,6, 4,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11

	};
	font['s'] = createUnfinObjFromStatic(s, sinds, _countof(s) / VERTEX_LENGTH, _countof(sinds));

	float T[] = {
		//three bars, not complex
		-4, 40,			0, 0.8, 0.8, 0.8,//0
		4, 40,			0, 0.8, 0.8, 0.8,//1
		-4, -40,			0, 0.8, 0.8, 0.8,//2
		4, -40,			0, 0.8, 0.8, 0.8,//3

		25, 40,			0, 0.8, 0.8, 0.8,//4
		25, 32,			0, 0.8, 0.8, 0.8,//5
		-25, 32,		0, 0.8, 0.8, 0.8,//6
		-25, 40,		0, 0.8, 0.8, 0.8,//7
	};
	convertPixelSpaceToOpenGL(T, _countof(T) / VERTEX_LENGTH);

	unsigned int Tinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
	};
	font['T'] = createUnfinObjFromStatic(T, Tinds, _countof(T) / VERTEX_LENGTH, _countof(Tinds));

	float t[] = {
		//three bars, not complex
		-4, 40,			0, 0.8, 0.8, 0.8,//0
		4, 40,			0, 0.8, 0.8, 0.8,//1
		-4, -40,			0, 0.8, 0.8, 0.8,//2
		4, -40,			0, 0.8, 0.8, 0.8,//3

		25, 5,			0, 0.8, 0.8, 0.8,//4
		25, 13,			0, 0.8, 0.8, 0.8,//5
		-25, 13,			0, 0.8, 0.8, 0.8,//6
		-25, 5,			0, 0.8, 0.8, 0.8,//7
	};
	convertPixelSpaceToOpenGL(t, _countof(t) / VERTEX_LENGTH);

	unsigned int tinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
	};
	font['t'] = createUnfinObjFromStatic(t, tinds, _countof(t) / VERTEX_LENGTH, _countof(tinds));

	float U[] = {
		//thing is a big loop okay
		17, 40,			0, 0.8, 0.8, 0.8,//0  
		25, 40,			0, 0.8, 0.8, 0.8,//1   
		-17, 40,			0, 0.8, 0.8, 0.8,//2  
		-25, 40,		0, 0.8, 0.8, 0.8,//3  
		-17, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		17, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,			0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(U, _countof(U) / VERTEX_LENGTH);

	unsigned int Uinds[] = {
		2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,//big loopy bit
	};
	font['U'] = createUnfinObjFromStatic(U, Uinds, _countof(U) / VERTEX_LENGTH, _countof(Uinds));

	float u[] = {
		//thing is a big loop okay
		17, 5,			0, 0.8, 0.8, 0.8,//0  
		25, 5,			0, 0.8, 0.8, 0.8,//1   
		-17, 5,			0, 0.8, 0.8, 0.8,//2  
		-25, 5,			0, 0.8, 0.8, 0.8,//3  
		-17, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		17, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,		0, 0.8, 0.8, 0.8,//7 

		//lil stem
		/*25, -44,		0, 0.8, 0.8, 0.8,//8
		17, -44,			0, 0.8, 0.8, 0.8,//9  */
	};
	convertPixelSpaceToOpenGL(u, _countof(u) / VERTEX_LENGTH);

	unsigned int uinds[] = {
		2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,//big loopy bit
		/*6,7,9, 7,8,9,*/
	};
	font['u'] = createUnfinObjFromStatic(u, uinds, _countof(u) / VERTEX_LENGTH, _countof(uinds));

	float V[] = {
		0, -40,			0, 0.8, 0.8, 0.8,//0  
		0, -15,			0, 0.8, 0.8, 0.8,//1
		-25, 40,		0, 0.8, 0.8, 0.8,//2
		-15, 40,			0, 0.8, 0.8, 0.8,//3
		25, 40,			0, 0.8, 0.8, 0.8,//4
		15, 40,			0, 0.8, 0.8, 0.8,//5
	};
	convertPixelSpaceToOpenGL(V, _countof(V) / VERTEX_LENGTH);

	unsigned int Vinds[] = {
		0, 1, 2,		2, 3, 1,//left bar
		0, 1, 4,		4, 5, 1,//right bar
	};
	font['V'] = createUnfinObjFromStatic(V, Vinds, _countof(V) / VERTEX_LENGTH, _countof(Vinds));

	float v[] = {
		0, -40,			0, 0.8, 0.8, 0.8,//0  
		0, -25,			0, 0.8, 0.8, 0.8,//1
		-25, 5,			0, 0.8, 0.8, 0.8,//2
		-15, 5,			0, 0.8, 0.8, 0.8,//3
		25, 5,			0, 0.8, 0.8, 0.8,//4
		15, 5,			0, 0.8, 0.8, 0.8,//5
	};
	convertPixelSpaceToOpenGL(v, _countof(v) / VERTEX_LENGTH);

	unsigned int vinds[] = {
		0, 1, 2,		2, 3, 1,//left bar
		0, 1, 4,		4, 5, 1,//right bar
	};
	font['v'] = createUnfinObjFromStatic(v, vinds, _countof(v) / VERTEX_LENGTH, _countof(vinds));

	float _W[] = {
		//back
		-25, -40,		0, 0.8, 0.8, 0.8,//0   
		-17, -40,			0, 0.8, 0.8, 0.8,//1
		-25, 40,		0, 0.8, 0.8, 0.8,//2
		-17, 40,		0, 0.8, 0.8, 0.8,//3  

		//front
		25, -40,			0, 0.8, 0.8, 0.8,//4  
		17, -40,			0, 0.8, 0.8, 0.8,//5
		25, 40,		0, 0.8, 0.8, 0.8,//6
		17, 40,			0, 0.8, 0.8, 0.8,//7

		//middle
		0, 5,			0, 0.8, 0.8, 0.8,//8   //-10
		0, -8,				0, 0.8, 0.8, 0.8,//9   //-6
	};
	convertPixelSpaceToOpenGL(_W, _countof(_W) / VERTEX_LENGTH);

	unsigned int Winds[] = {
		0,1,2, 1,2,3,//back
		4,5,6, 5,6,7,//front
		4,5,9, 8,9,4, 0,1,9, 8,9,0,
	};
	font['W'] = createUnfinObjFromStatic(_W, Winds, _countof(_W) / VERTEX_LENGTH, _countof(Winds));

	float w[] = {
		//back
		-25, -40,		0, 0.8, 0.8, 0.8,//0   
		-17, -40,		0, 0.8, 0.8, 0.8,//1
		-25, 5,			0, 0.8, 0.8, 0.8,//2
		-17, 5,			0, 0.8, 0.8, 0.8,//3  

		//front
		25, -40,		0, 0.8, 0.8, 0.8,//4  
		17, -40,			0, 0.8, 0.8, 0.8,//5
		25, 5,			0, 0.8, 0.8, 0.8,//6
		17, 5,			0, 0.8, 0.8, 0.8,//7

		//middle
		0, 5,				0, 0.8, 0.8, 0.8,//8 
		0, -8,			0, 0.8, 0.8, 0.8,//9
	};
	convertPixelSpaceToOpenGL(w, _countof(w) / VERTEX_LENGTH);

	unsigned int winds[] = {
		0,1,2, 1,2,3,//back
		4,5,6, 5,6,7,//front
		4,5,9, 8,9,4, 0,1,9, 8,9,0,
	};
	font['w'] = createUnfinObjFromStatic(w, winds, _countof(w) / VERTEX_LENGTH, _countof(winds));


	float _X[] = {
		//two crossing bars
		25, 40,				0, 0.8, 0.8, 0.8,//0 
		15, 40,				0, 0.8, 0.8, 0.8,//1 
		-25, -40,			0, 0.8, 0.8, 0.8,//2 
		-15, -40,			0, 0.8, 0.8, 0.8,//3 

		-25, 40,			0, 0.8, 0.8, 0.8,//4 
		-15, 40,				0, 0.8, 0.8, 0.8,//5 
		25, -40,			0, 0.8, 0.8, 0.8,//6 
		15, -40,				0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(_X, _countof(_X) / VERTEX_LENGTH);

	unsigned int Xinds[] = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,
	};
	font['X'] = createUnfinObjFromStatic(_X, Xinds, _countof(_X) / VERTEX_LENGTH, _countof(Xinds));

	float x[] = {
		//two crossing bars
		25, 5,				0, 0.8, 0.8, 0.8,//0 
		15, 5,				0, 0.8, 0.8, 0.8,//1 
		-25, -40,			0, 0.8, 0.8, 0.8,//2 
		-15, -40,			0, 0.8, 0.8, 0.8,//3 

		-25, 5,				0, 0.8, 0.8, 0.8,//4 
		-15, 5,				0, 0.8, 0.8, 0.8,//5 
		25, -40,			0, 0.8, 0.8, 0.8,//6 
		15, -40,				0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(x, _countof(x) / VERTEX_LENGTH);

	unsigned int xinds[] = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,
	};
	font['x'] = createUnfinObjFromStatic(x, xinds, _countof(x) / VERTEX_LENGTH, _countof(xinds));

	float _Y[] = {
		//top bars
		25, 40,				0, 0.8, 0.8, 0.8,//0 
		17, 40,				0, 0.8, 0.8, 0.8,//1 
		-25, 40,			0, 0.8, 0.8, 0.8,//2 
		-17, 40,				0, 0.8, 0.8, 0.8,//3 
		//bottom middle bar, top bars connect to it
		4, 5,					0, 0.8, 0.8, 0.8,//4 
		-4, 5,				0, 0.8, 0.8, 0.8,//5 
		4, -40,				0, 0.8, 0.8, 0.8,//6 
		-4, -40,				0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(_Y, _countof(_Y) / VERTEX_LENGTH);

	unsigned int Yinds[] = {
		0,1,4, 1,4,5, 2,3,4, 2,4,5,//top piece
		4,5,6, 5,6,7,//bottom bar
	};
	font['Y'] = createUnfinObjFromStatic(_Y, Yinds, _countof(_Y) / VERTEX_LENGTH, _countof(Yinds));


	float y[] = {
		//two touching bars
		25, 5,				0, 0.8, 0.8, 0.8,//0 
		15, 5,				0, 0.8, 0.8, 0.8,//1 
		-29, -48,			0, 0.8, 0.8, 0.8,//2 
		-19, -48,			0, 0.8, 0.8, 0.8,//3 

		-25, 5,				0, 0.8, 0.8, 0.8,//4 
		-15, 5,				0, 0.8, 0.8, 0.8,//5 
		4, -19,				0, 0.8, 0.8, 0.8,//6 
		-5, -20,				0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(y, _countof(y) / VERTEX_LENGTH);

	unsigned int yinds[] = {
		0,1,2, 0,2,3,
		4,5,6, 4,6,7,
	};
	font['y'] = createUnfinObjFromStatic(y, yinds, _countof(y) / VERTEX_LENGTH, _countof(yinds));

	float _Z[] = {
		//top, bottom, then connect the two
		25, 32,				0, 0.8, 0.8, 0.8,//0 
		25, 40,				0, 0.8, 0.8, 0.8,//1 
		-25, 40,			0, 0.8, 0.8, 0.8,//2 
		-25, 32, 			0, 0.8, 0.8, 0.8,//3 

		25, -32,			0, 0.8, 0.8, 0.8,//4 
		25, -40,			0, 0.8, 0.8, 0.8,//5 
		-25, -40,			0, 0.8, 0.8, 0.8,//6 
		-25, -32, 			0, 0.8, 0.8, 0.8,//7  

		17, 32,				0, 0.8, 0.8, 0.8,//8 
		-17, -32,			0, 0.8, 0.8, 0.8,//9 
	};
	convertPixelSpaceToOpenGL(_Z, _countof(_Z) / VERTEX_LENGTH);

	unsigned int Zinds[] = {
		0,1,2, 0,2,3, 4,5,6, 4,6,7,//top and bottom
		0,8,7, 7,0,9
	};
	font['Z'] = createUnfinObjFromStatic(_Z, Zinds, _countof(_Z) / VERTEX_LENGTH, _countof(Zinds));

	float z[] = {
		//top, bottom, then connect the two
		25, -3,				0, 0.8, 0.8, 0.8,//0 
		25, 5,				0, 0.8, 0.8, 0.8,//1 
		-25, 5,				0, 0.8, 0.8, 0.8,//2 
		-25, -3, 			0, 0.8, 0.8, 0.8,//3 

		25, -32,			0, 0.8, 0.8, 0.8,//4 
		25, -40,			0, 0.8, 0.8, 0.8,//5 
		-25, -40,			0, 0.8, 0.8, 0.8,//6 
		-25, -32, 			0, 0.8, 0.8, 0.8,//7  

		14, -3,				0, 0.8, 0.8, 0.8,//8 
		-14, -32,			0, 0.8, 0.8, 0.8,//9 
	};
	convertPixelSpaceToOpenGL(z, _countof(z) / VERTEX_LENGTH);

	unsigned int zinds[] = {
		0,1,2, 0,2,3, 4,5,6, 4,6,7,//top and bottom
		0,8,7, 7,0,9
	};
	font['z'] = createUnfinObjFromStatic(z, zinds, _countof(z) / VERTEX_LENGTH, _countof(zinds));
}

void setupNumbers() {
	float One[] = {
		//Base1
		-20, -32,			0, 0.8, 0.8, 0.8,//0      
		-20, -40,			0, 0.8, 0.8, 0.8,//1      
		20, -32,			0, 0.8, 0.8, 0.8,//2      
		20, -40,			0, 0.8, 0.8, 0.8,//3     
		
		//stem
		4, 40,				0, 0.8, 0.8, 0.8,//4    
		4, -40,				0, 0.8, 0.8, 0.8,//5     
		-4, 40,				0, 0.8, 0.8, 0.8,//6     
		-4, -40,				0, 0.8, 0.8, 0.8,//7  
		
		//sticky outy bit to the left
		-20, 24,			0, 0.8, 0.8, 0.8,//8      
		-12, 24,				0, 0.8, 0.8, 0.8,//9      
	};
	convertPixelSpaceToOpenGL(One, _countof(One) / VERTEX_LENGTH);

	unsigned int Oneinds[] = {
		0,1,2, 1,2,3,//base
		4,5,6, 5,6,7,//stem
		8,9,4, 8,4,6//sticky outy bit
	};
	font['1'] = createUnfinObjFromStatic(One, Oneinds, _countof(One) / VERTEX_LENGTH, _countof(Oneinds));

	float Two[] = {
		//double arcing loop
		-25, 32,			0, 0.8, 0.8, 0.8,//0   
		-25, 40,			0, 0.8, 0.8, 0.8,//1   
		17, 40,				0, 0.8, 0.8, 0.8,//2   
		25, 32,				0, 0.8, 0.8, 0.8,//3   

		25, 5,				0, 0.8, 0.8, 0.8,//4   
		17, -3,				0, 0.8, 0.8, 0.8,//5   
		-25, -3,				0, 0.8, 0.8, 0.8,//6   
		-17, 5,				0, 0.8, 0.8, 0.8,//7   

		-17, -32,			0, 0.8, 0.8, 0.8,//8   
		-25, -40,			0, 0.8, 0.8, 0.8,//9   

		25, -32,			0, 0.8, 0.8, 0.8,//10   
		25, -40,			0, 0.8, 0.8, 0.8,//11   
	};
	convertPixelSpaceToOpenGL(Two, _countof(Two) / VERTEX_LENGTH);

	unsigned int Twoinds[] = {
		0,1,2, 0,2,3, 2,3,4, 2,4,5, 4,5,6, 4,6,7, 6,7,8, 6,8,9, 8,9,10, 9,10,11
	};
	font['2'] = createUnfinObjFromStatic(Two, Twoinds, _countof(Two) / VERTEX_LENGTH, _countof(Twoinds));

	float Three[] = {
		//back
		17, 40,				0, 0.8, 0.8, 0.8,//0  
		25, 32,				0, 0.8, 0.8, 0.8,//1  
		17, -40,				0, 0.8, 0.8, 0.8,//2  
		25, -32,			0, 0.8, 0.8, 0.8,//3 

		//spurs
		-25, 40,			0, 0.8, 0.8, 0.8,//4
		-20, 32,			0, 0.8, 0.8, 0.8,//5  
		25, 32,				0, 0.8, 0.8, 0.8,//6  

		-20, 5,				0, 0.8, 0.8, 0.8,//7
		20, 5,				0, 0.8, 0.8, 0.8,//8  
		-20, -3,				0, 0.8, 0.8, 0.8,//9  
		20, -3,				0, 0.8, 0.8, 0.8,//10  

		-20, -32,			0, 0.8, 0.8, 0.8,//11  
		-25, -40,			0, 0.8, 0.8, 0.8,//12 
	};
	convertPixelSpaceToOpenGL(Three, _countof(Three) / VERTEX_LENGTH);

	unsigned int Threeinds[] = {
		0,1,2, 2,1,3,//back
		1,4,5, 4,6,0,//spurs
		7,8,9, 8,9,10,
		11,12,2, 11,2,3
	};
	font['3'] = createUnfinObjFromStatic(Three, Threeinds, _countof(Three) / VERTEX_LENGTH, _countof(Threeinds));

	float Four[] = {
		//three bars, not complex
		25, -40,		0, 0.8, 0.8, 0.8,//0
		17, -40,			0, 0.8, 0.8, 0.8,//1
		25, 40,			0, 0.8, 0.8, 0.8,//2
		17, 40,			0, 0.8, 0.8, 0.8,//3

		-25, 3,			0, 0.8, 0.8, 0.8,//4
		-17, -5,			0, 0.8, 0.8, 0.8,//5
		-17, 40,			0, 0.8, 0.8, 0.8,//6
		-25, 40,		0, 0.8, 0.8, 0.8,//7

		25, -5,			0, 0.8, 0.8, 0.8,//8
		25, 3,			0, 0.8, 0.8, 0.8,//9
	};
	convertPixelSpaceToOpenGL(Four, _countof(Four) / VERTEX_LENGTH);

	unsigned int Fourinds[] = {
		0,1,2, 1,2,3,
		4,5,6, 4,6,7,
		4,5,8, 8,4,9,
	};
	font['4'] = createUnfinObjFromStatic(Four, Fourinds, _countof(Four) / VERTEX_LENGTH, _countof(Fourinds));

	float Five[] = {
		//double arcing loop
		25, 32,			0, 0.8, 0.8, 0.8,//0   
		25, 40,			0, 0.8, 0.8, 0.8,//1   
		-25, 40,		0, 0.8, 0.8, 0.8,//2   
		-17, 32,			0, 0.8, 0.8, 0.8,//3   

		-17, 5,			0, 0.8, 0.8, 0.8,//4   
		-25, -3,			0, 0.8, 0.8, 0.8,//5   
		25, -3,			0, 0.8, 0.8, 0.8,//6   
		17, 5,			0, 0.8, 0.8, 0.8,//7   

		25, -32,		0, 0.8, 0.8, 0.8,//8   
		17, -40,			0, 0.8, 0.8, 0.8,//9   

		-25, -32,		0, 0.8, 0.8, 0.8,//10   
		-25, -40,		0, 0.8, 0.8, 0.8,//11   
	};
	convertPixelSpaceToOpenGL(Five, _countof(Five) / VERTEX_LENGTH);

	unsigned int Fiveinds[] = {
		0,1,2, 0,2,3, 2,3,4, 2,4,5, 4,5,6, 4,6,7, 6,7,8, 7,8,9, 8,9,10, 9,10,11
	};
	font['5'] = createUnfinObjFromStatic(Five, Fiveinds, _countof(Five) / VERTEX_LENGTH, _countof(Fiveinds));


	float Six[] = {
		//back
		-25, 32,		0, 0.8, 0.8, 0.8,//0  
		-17, 40,			0, 0.8, 0.8, 0.8,//1  
		-25, -32,		0, 0.8, 0.8, 0.8,//2  
		-17, -40,		0, 0.8, 0.8, 0.8,//3 

		//bottom o part
		-20, 5,			0, 0.8, 0.8, 0.8,//4
		-20, -3,			0, 0.8, 0.8, 0.8,//5  
		17, 5,			0, 0.8, 0.8, 0.8,//6  
		25, -3,			0, 0.8, 0.8, 0.8,//7 
		25, -32,		0, 0.8, 0.8, 0.8,//8
		17, -40,			0, 0.8, 0.8, 0.8,//9
		-25, -32,		0, 0.8, 0.8, 0.8,//10
		-17, -40,		0, 0.8, 0.8, 0.8,//11

		//top
		20, 40,			0, 0.8, 0.8, 0.8,//12  
		25, 32,			0, 0.8, 0.8, 0.8,//13  
	};
	convertPixelSpaceToOpenGL(Six, _countof(Six) / VERTEX_LENGTH);

	unsigned int Sixinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8,//bottom o part
		0,1,12, 0,12,13,
	};
	font['6'] = createUnfinObjFromStatic(Six, Sixinds, _countof(Six) / VERTEX_LENGTH, _countof(Sixinds));


	float Seven[] = {
		//angled section
		17, 40,			0, 0.8, 0.8, 0.8,//0  
		25, 40,			0, 0.8, 0.8, 0.8,//1  
		-23, -40,		0, 0.8, 0.8, 0.8,//2  
		-15, -40,		0, 0.8, 0.8, 0.8,//3   

		//top
		-25, 40,		0, 0.8, 0.8, 0.8,//4   
		-25, 32,		0, 0.8, 0.8, 0.8,//5   
		20, 32,			0, 0.8, 0.8, 0.8,//6   
	};
	convertPixelSpaceToOpenGL(Seven, _countof(Seven) / VERTEX_LENGTH);

	unsigned int Seveninds[] = {
		0,1,2, 1,2,3,//angled section
		1,4,5, 1,5,6,//top
	};
	font['7'] = createUnfinObjFromStatic(Seven, Seveninds, _countof(Seven) / VERTEX_LENGTH, _countof(Seveninds));


	//pain
	float Eight[] = {
		//top
		17, 40,			0, 0.8, 0.8, 0.8,//0 
		25, 32,			0, 0.8, 0.8, 0.8,//1 
		-17, 40,			0, 0.8, 0.8, 0.8,//2 
		-25,  32,		0, 0.8, 0.8, 0.8,//3 

		//bottom
		17, -40,			0, 0.8, 0.8, 0.8,//4 
		25, -32,		0, 0.8, 0.8, 0.8,//5 
		-17, -40,		0, 0.8, 0.8, 0.8,//6 
		-25,  -32,		0, 0.8, 0.8, 0.8,//7 

		//confusing middle bit, is triangular on either end
		-17, 1,			0, 0.8, 0.8, 0.8,//8 
		-25, 5,			0, 0.8, 0.8, 0.8,//9 
		-25, -3,			0, 0.8, 0.8, 0.8,//10 

		17, 1,				0, 0.8, 0.8, 0.8,//11 
		25, 5,			0, 0.8, 0.8, 0.8,//12 
		25, -3,			0, 0.8, 0.8, 0.8,//13 
	};
	convertPixelSpaceToOpenGL(Eight, _countof(Eight) / VERTEX_LENGTH);

	unsigned int Eightinds[] = {
		0,1,2, 1,2,3,//top bar
		4,5,6, 5,6,7,//bottom bar
		9,12,8, 8,12,11, 10,13,11, 10,11,8,//middle bar
		2,3,9, 2,9,8, 0,1,11, 1,12,11,//top connectors to middle
		4,5,13, 4,13,11, 6,7,10, 10,8,6,//bottom connectors to middle
	};
	font['8'] = createUnfinObjFromStatic(Eight, Eightinds, _countof(Eight) / VERTEX_LENGTH, _countof(Eightinds));


	float Nine[] = {
		//back
		25, -40,		0, 0.8, 0.8, 0.8,//0  
		17, -40,			0, 0.8, 0.8, 0.8,//1  
		25, 32,			0, 0.8, 0.8, 0.8,//2  
		17, 40,			0, 0.8, 0.8, 0.8,//3 

		//o part
		20, -5,			0, 0.8, 0.8, 0.8,//4
		20, 3,			0, 0.8, 0.8, 0.8,//5  
		-17, -5,			0, 0.8, 0.8, 0.8,//6  
		-25, 3,			0, 0.8, 0.8, 0.8,//7 
		-25, 32,		0, 0.8, 0.8, 0.8,//8
		-17, 40,			0, 0.8, 0.8, 0.8,//9
		25, 32,			0, 0.8, 0.8, 0.8,//10
		17, 40,			0, 0.8, 0.8, 0.8,//11
	};
	convertPixelSpaceToOpenGL(Nine, _countof(Nine) / VERTEX_LENGTH);

	unsigned int Nineinds[] = {
		0,1,2, 2,1,3,//back
		4,5,6, 6,7,5, 6,8,9, 6,7,8, 10,11,9, 10,9,8//bottom o part
	};
	font['9'] = createUnfinObjFromStatic(Nine, Nineinds, _countof(Nine) / VERTEX_LENGTH, _countof(Nineinds));

	//todo, make cross not as wide
	float Zero[] = {
		//thing is a big loop okay
		17, 40,			0, 0.8, 0.8, 0.8,//0  
		25, 32,			0, 0.8, 0.8, 0.8,//1   
		-17, 40,			0, 0.8, 0.8, 0.8,//2  
		-25, 32,		0, 0.8, 0.8, 0.8,//3  
		-17, -40,		0, 0.8, 0.8, 0.8,//4  
		-25, -32,		0, 0.8, 0.8, 0.8,//5   
		17, -40,			0, 0.8, 0.8, 0.8,//6  
		25, -32,		0, 0.8, 0.8, 0.8,//7 
	};
	convertPixelSpaceToOpenGL(Zero, _countof(Zero) / VERTEX_LENGTH);

	unsigned int Zeroinds[] = {
		0,1,2, 1,2,3, 2,3,4, 3,4,5, 4,5,6, 5,6,7, 0,1,6, 1,6,7,//loop
		0,1,4, 0,4,5,//cross
	};
	font['0'] = createUnfinObjFromStatic(Zero, Zeroinds, _countof(Zero) / VERTEX_LENGTH, _countof(Zeroinds));
}

void setupSymbols() {
	float colon[] = {
		//top square
		-6, 10,			0, 0.8, 0.8, 0.8,//0      
		-6, 22,			0, 0.8, 0.8, 0.8,//1      
		6, 10,			0, 0.8, 0.8, 0.8,//2      
		6, 22,			0, 0.8, 0.8, 0.8,//3     

		//bottom square
		-6, -20,			0, 0.8, 0.8, 0.8,//4      
		-6, -8,			0, 0.8, 0.8, 0.8,//5      
		6, -20,			0, 0.8, 0.8, 0.8,//6      
		6, -8,			0, 0.8, 0.8, 0.8,//7     
	};
	convertPixelSpaceToOpenGL(colon, _countof(colon) / VERTEX_LENGTH);

	unsigned int coloninds[] = {
		0,1,2, 1,2,3,//top
		4,5,6, 5,6,7//bottom
	};
	font[':'] = createUnfinObjFromStatic(colon, coloninds, _countof(colon) / VERTEX_LENGTH, _countof(coloninds));

	float backslash[] = {
		//issa line
		-24, 40,			0, 0.8, 0.8, 0.8,//0      
		-16, 40,				0, 0.8, 0.8, 0.8,//1      
		24, -40,			0, 0.8, 0.8, 0.8,//2      
		16, -40,				0, 0.8, 0.8, 0.8,//3     
	};
	convertPixelSpaceToOpenGL(backslash, _countof(backslash) / VERTEX_LENGTH);

	unsigned int backslashinds[] = {
		0,1,2, 0,2,3,
	};
	font['\\'] = createUnfinObjFromStatic(backslash, backslashinds, _countof(backslash) / VERTEX_LENGTH, _countof(backslashinds));

	float forslash[] = {
		//issa line
		-24, 40,			0, 0.8, 0.8, 0.8,//0      
		-16, 40,				0, 0.8, 0.8, 0.8,//1      
		24, -40,			0, 0.8, 0.8, 0.8,//2      
		16, -40,				0, 0.8, 0.8, 0.8,//3     
	};
	convertPixelSpaceToOpenGL(forslash, _countof(forslash) / VERTEX_LENGTH);

	unsigned int forslashinds[] = {
		0,1,2, 0,2,3,
	};
	font['/'] = createUnfinObjFromStatic(forslash, forslashinds, _countof(forslash) / VERTEX_LENGTH, _countof(forslashinds));

	float dash[] = {
		//issa line
		-25, 5,				0, 0.8, 0.8, 0.8,//0      
		-25, -3,				0, 0.8, 0.8, 0.8,//1      
		25, -3,				0, 0.8, 0.8, 0.8,//2      
		25, 5,				0, 0.8, 0.8, 0.8,//3     
	};
	convertPixelSpaceToOpenGL(dash, _countof(dash) / VERTEX_LENGTH);

	unsigned dashinds[] = {
		0,1,2, 0,2,3,
	};
	font['-'] = createUnfinObjFromStatic(dash, dashinds, _countof(dash) / VERTEX_LENGTH, _countof(dashinds));

	float question[] = {
		//?is a squiggly line
		-17, 40,			0, 0.8, 0.8, 0.8,//0      
		-25, 32,			0, 0.8, 0.8, 0.8,//1      
		17, 40,				0, 0.8, 0.8, 0.8,//2      
		25,32,				0, 0.8, 0.8, 0.8,//3     

		25, 5,				0, 0.8, 0.8, 0.8,//4     
		17,-3,				0, 0.8, 0.8, 0.8,//5     
		4, 5,					0, 0.8, 0.8, 0.8,//6     
		-4,-3,				0, 0.8, 0.8, 0.8,//7     

		-4,-24,				0, 0.8, 0.8, 0.8,//8     
		4,-24,				0, 0.8, 0.8, 0.8,//9     

		//bottom point
		4,-40,				0, 0.8, 0.8, 0.8,//10     
		-4,-40,				0, 0.8, 0.8, 0.8,//11     
		4,-32,				0, 0.8, 0.8, 0.8,//12     
		-4,-32,				0, 0.8, 0.8, 0.8,//13     
	};
	convertPixelSpaceToOpenGL(question, _countof(question) / VERTEX_LENGTH);

	unsigned questioninds[] = {
		0,1,2, 1,2,3, 2,3,4, 2,4,5, 4,5,6, 5,6,7, 6,7,8, 6,8,9,//turn-ey line
		10,11,12, 11,12,13,
	};
	font['?'] = createUnfinObjFromStatic(question, questioninds, _countof(question) / VERTEX_LENGTH, _countof(questioninds));

	float period[] = {
		//it's a dot...
		-4, -40,			0, 0.8, 0.8, 0.8,//0      
		-4, -32,			0, 0.8, 0.8, 0.8,//1      
		4, -40,				0, 0.8, 0.8, 0.8,//2      
		4,-32,				0, 0.8, 0.8, 0.8,//3     
	};
	convertPixelSpaceToOpenGL(period, _countof(period) / VERTEX_LENGTH);

	unsigned periodinds[] = {
		0,1,2, 1,2,3,
	};
	font['.'] = createUnfinObjFromStatic(period, periodinds, _countof(period) / VERTEX_LENGTH, _countof(periodinds));
}


void setupUI(int flag) {
	font = calloc(128, sizeof(UnfinObj));//MEMORYTODO - can reduce consumption by several kb if done differently
	if (flag) {//Using the default 'built in' characterset
		
		//split up so as to not break stack (?) size
		setupSectionA();
		setupSectionB();
		setupSectionC();
		setupNumbers();
		setupSymbols();

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

void runMasterUI() {
	for (int c = 0; c < masterUIListLength; c++) {
		if (masterUIList[c] != NULL) {//safety
			if (masterUIList[c]->active == 1) {
				runUI(masterUIList[c]);
			}
		}
		
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
					ui->elements[current]->data = ui->elements[current]->action(ui->elements[current]->data, ui->elements[current]->clickData);
					break;
				case CUSTOM_ACTION:
					*ui->elements[current]->blockData = ui->elements[current]->customAction(ui->elements[current]->blockData, ui->elements[current]->clickData);
					break;
			}
			ui->elements[current]->actionNeeded = READY_FOR_ACTION;
		}

		current++;
	}
}

void insertElementIntoUI(UI* ui, UIElement* element){
	if (ui->elementCount > ui->elementListSize) {//This is because the list might be bigger than the current count from deleting elements 

		//POTENTIAL PERFORMANCE TODO- Could check the actual size of the list versus what we needand spare some realloc calls that might be redundant
		ui->elementCount++;
		ui->elementListSize = ui->elementCount;
		ui->elements[ui->elementCount - 1] = element;
	} else {
		ui->elementCount++;
		ui->elementListSize = ui->elementCount;
		ui->elements = realloc(ui->elements, ui->elementCount * sizeof(UIElement*));
		ui->elements[ui->elementCount - 1] = element;
	}
}

void removeElementFromUI(UI* ui, UIElement* element) {
	for (int cEl = 0; cEl < ui->elementCount; cEl++) {
		if (ui->elements[cEl]->ID == element->ID) {
			//if it's the last, we just null it
			if (cEl == ui->elementCount) {
				ui->elements[cEl] = NULL;
			} else {//otherwise, replace it with the current final element
				ui->elements[cEl] = ui->elements[ui->elementCount - 1];
			}
			ui->elementCount--;
		}
	}
}


void drawElement(UIElement* uiItem) {
	glBindVertexArray(uiItem->ID);
	glUniform3f(ProgramData.cordinatesLoc, uiItem->position[X_pos], uiItem->position[Y_pos], uiItem->position[Z_pos]);
	glDrawElements(GL_TRIANGLES, uiItem->indexCount, GL_UNSIGNED_INT, 0);
}

//'click area' is leftmost, rightmost, bottommost, topmost
UIElement* createElement(float* vertices, unsigned int* index, int vertSize, int indSize, float* pos, void* action, void* customAction, char defaultAction, int active, float clickArea[4]){
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
	returnElement->position[X_pos] = pos[X_pos];
	returnElement->position[Y_pos] = pos[Y_pos];
	returnElement->position[Z_pos] = pos[Z_pos];

	if (clickArea != NULL) {
		returnElement->clickArea[0] = clickArea[0];
		returnElement->clickArea[1] = clickArea[1];
		returnElement->clickArea[2] = clickArea[2];
		returnElement->clickArea[3] = clickArea[3];
		/*
		returnElement->clickArea[0] = (clickArea[0] + 1) * windX / 2;
		returnElement->clickArea[1] = (clickArea[1] + 1) * windX / 2;
		returnElement->clickArea[2] = ((clickArea[2] * -1) + 1) * windY / 2;
		returnElement->clickArea[3] = ((clickArea[3] * -1) + 1) * windY / 2;*/
	}	
	//printf("%f, %f, %f, %f\n", returnElement->clickArea[0], returnElement->clickArea[1], returnElement->clickArea[2], returnElement->clickArea[3]);


	return(returnElement);
}


//Font size is in pixels and for the height. X/Y pos are in opengl coords
UnfinObj createUnFinText(char* text, float xpos, float ypos, float fontSize, float* rgb) {
	int textLength = strlen(text);
	float xOffset = 0;
	float yOffset = 0;

	float fontHeight = ((16*(fontSize/(float)100)) + fontSize)/ ((float)windY / 2);
	float fontWidth = ((8 * (fontSize / (float)100)) + (fontSize* 0.625)) / ((float)windX / 2);
	//float fontHeight = (fontSize * ((float)FONT_SIZE_Y / ((float)windY / 2)))/ ((float)FONT_SIZE_Y* .95);//((float)FONT_SIZE_Y * fontSize) / ((float)windY);
	//float fontWidth = (fontSize * ((float)FONT_SIZE_X / ((float)windX / 2)))/ ((float)FONT_SIZE_Y * .85);// ((float)FONT_SIZE_X * fontSize) / ((float)windX );

	UnfinObj returnData = { 0 };

	for (int cChar = 0; cChar < textLength; cChar++) {
		UnfinObj temp = { 0 };
		UnfinObj character = mergeUnfinisheds(temp, font[text[cChar]]);//so as to not mess up the font

		//printf("Cur Index: %d,\t", cChar);
		//printf("Cur char: %c\n", text[cChar]);

		if (text[cChar] != '\n') {
			for (int cV = 0; cV < character.vLineCount; cV++) {
				//printf("Cur V: %d.   ", cV);
			
				character.verts[cV * VERTEX_LENGTH] *= (fontSize / FONT_SIZE_Y);
				character.verts[(cV * VERTEX_LENGTH) + 1] *= (fontSize / FONT_SIZE_Y);

				character.verts[cV * VERTEX_LENGTH] += xpos + xOffset;
				character.verts[(cV * VERTEX_LENGTH) + 1] += ypos + yOffset;
				
				character.verts[(cV * VERTEX_LENGTH) + 3] = rgb[0];
				character.verts[(cV * VERTEX_LENGTH) + 4] = rgb[1];
				character.verts[(cV * VERTEX_LENGTH) + 5] = rgb[2];

				//printf("%f, %f\n", temp.verts[cV * VERTEX_LENGTH], temp.verts[(cV * VERTEX_LENGTH) + 1]);
			}
			xOffset += fontWidth;
			//printf("\n\n");
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




//Font size is in pixels and for the height. X/Y pos are in opengl coords
UnfinObj createUnFinTextWithZ(char* text, float xpos, float ypos, float zpos, float fontSize, float* rgb) {
	int textLength = strlen(text);
	float xOffset = 0;
	float yOffset = 0;

	float fontHeight = ((16 * (fontSize / (float)100)) + fontSize) / ((float)windY / 2);
	float fontWidth = ((8 * (fontSize / (float)100)) + (fontSize * 0.625)) / ((float)windX / 2);
	//float fontHeight = (fontSize * ((float)FONT_SIZE_Y / ((float)windY / 2)))/ ((float)FONT_SIZE_Y* .95);//((float)FONT_SIZE_Y * fontSize) / ((float)windY);
	//float fontWidth = (fontSize * ((float)FONT_SIZE_X / ((float)windX / 2)))/ ((float)FONT_SIZE_Y * .85);// ((float)FONT_SIZE_X * fontSize) / ((float)windX );

	UnfinObj returnData = { 0 };

	for (int cChar = 0; cChar < textLength; cChar++) {
		UnfinObj temp = { 0 };
		UnfinObj character = mergeUnfinisheds(temp, font[text[cChar]]);//so as to not mess up the font

		//printf("Cur Index: %d,\t", cChar);
		//printf("Cur char: %c\n", text[cChar]);

		if (text[cChar] != '\n') {
			for (int cV = 0; cV < character.vLineCount; cV++) {
				//printf("Cur V: %d.   ", cV);

				character.verts[cV * VERTEX_LENGTH] *= (fontSize / FONT_SIZE_Y);
				character.verts[(cV * VERTEX_LENGTH) + 1] *= (fontSize / FONT_SIZE_Y);
				character.verts[(cV * VERTEX_LENGTH) + 2] = zpos;

				character.verts[cV * VERTEX_LENGTH] += xpos + xOffset;
				character.verts[(cV * VERTEX_LENGTH) + 1] += ypos + yOffset;

				character.verts[(cV * VERTEX_LENGTH) + 3] = rgb[0];
				character.verts[(cV * VERTEX_LENGTH) + 4] = rgb[1];
				character.verts[(cV * VERTEX_LENGTH) + 5] = rgb[2];

				//printf("%f, %f\n", temp.verts[cV * VERTEX_LENGTH], temp.verts[(cV * VERTEX_LENGTH) + 1]);
			}
			xOffset += fontWidth;
			//printf("\n\n");
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