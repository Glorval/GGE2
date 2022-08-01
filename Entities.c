#pragma once
//#include "Entities.h"


void addRefEntry(unsigned int ID, int indlen, char* name) {
	masterObjLenght++;
	masterObjList = realloc(masterObjList, sizeof(RefObj) * masterObjLenght);
	masterObjList[masterObjLenght - 1].ID = ID;
	masterObjList[masterObjLenght - 1].objName = name;
	masterObjList[masterObjLenght - 1].indlen = indlen;
}

unsigned int loadEnemyShip() {
	float vertices_two[] = {
		//windshield
		-0.05, 0.0, 0.0,					//0
		0.05, 0.0, 0.0,					//1
		-0.06, 0.1, -0.14, 				//2
		0.06, 0.1, -0.14,					//3
		-0.08, 0.0, -0.07,				//4
		0.08, 0.0, -0.07,				//5 

		//front frame	
		0, -0.05, 0.08,					//6 - front point
		-0.07, -0.08, 0.02,			//7 
		0.07, -0.08, 0.02,				//8 

		//wings
		-0.15, -0.04, -0.06,			//9
		0.15, -0.04, -0.06,				//10
		-0.4, -0.04, -0.55,				//11
		0.4, -0.04, -0.55,				//12

		//back frame
		-0.08, 0.0, -0.45,				//13
		0.08, 0.0, -0.45,				//14
		-0.06, 0.1, -0.30,				//15//back of the windshield
		0.06, 0.1, -0.30,				//16
		-0.08, 0.0, -0.38,				//17
		0.08, 0.0, -0.38,				//18
		-0.08, -0.08, -0.45,			//19//back middle
		0.08, -0.08, -0.45,			//20
	};
	naturallyCentreVertices(vertices_two, 21, 3);

	unsigned int indstwo[] = {
		0,1, 0,2, 2,3, 3,1, 0,4, 1,5, 4,2, 5,3,//windshield
		0,6, 6,1, 6,7, 6,8, 7,8, 0,7, 1,8, 7,4, 8,5,//front frame and attaching to windshield
		7,9, 9,4, 8,10, 10,5, 9,11, 10,12, 7,11, 8,12,//wings
		13,4, 14,5, 13,11, 14,12, 15,2, 16,3, 15,17, 16,18, 15,16, 13,14,//mix of wing, back cockpit, back frame
		7,19, 8,20, 19,20, 11,19, 12,20, //bottom frame and its connections
	};

	Object vectorobj = createStaticVectorObject(vertices_two, indstwo, (sizeof(vertices_two) / VECTOR_VERTEX_LENGTH) / sizeof(float), sizeof(indstwo) / sizeof(unsigned int));

	//char* enemyShip = calloc(sizeof("Enemy Ship"), 1);
	//memcpy(enemyShip, "Enemy Ship", sizeof("Enemy Ship") / sizeof(char));
	static char* enemyShip = "Enemy Ship";
	addRefEntry(vectorobj.ID, vectorobj.indexCount, enemyShip);

}