#include "ObjectManager.h"


Object createObject(float* vertices, unsigned int* index, int vertSize, int indSize) {
	//unsigned int VBO;
	Object returnObject;
	//returnObject.ID = ID;

	returnObject.indexCount = indSize;
	glGenVertexArrays(1, &returnObject.ID);
	glGenBuffers(1, &returnObject.VBO);
	glGenBuffers(1, &returnObject.EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(returnObject.ID);

	glBindBuffer(GL_ARRAY_BUFFER, returnObject.VBO);
	glBufferData(GL_ARRAY_BUFFER, vertSize * VERTEX_SIZE * VERTEX_LENGTH, vertices, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, returnObject.EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indSize * IND_SIZE, index, GL_DYNAMIC_DRAW);

	// position attribute, first three
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute, second three (Obviously offset by three)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	returnObject.position[X] = 0;
	returnObject.position[Y] = 0;
	returnObject.position[Z] = 0;
	returnObject.position[W] = 0;
	returnObject.position[I] = 0;
	returnObject.position[J] = 1;//1, idk why
	returnObject.position[K] = 0;

	returnObject.updateOpenGLData = 1;

	return(returnObject);
}

void drawObject(Object* shape) {
	glBindVertexArray(shape->ID);

	//normalizeQuat(temp);
	//glUniform1f(ProgramData.scaleLoc, shape->scale);
	glUniform3f(ProgramData.cordinatesLoc, shape->position[X], shape->position[Y], shape->position[Z]);
	glUniform4f(ProgramData.orientationLoc, shape->position[W], shape->position[I], shape->position[J], shape->position[K]);
	glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
}



float modifiedPos[3] = { 0 };
void drawWorldObject(Object* shape, World* world) {
	glBindVertexArray(shape->ID);

	//quatMultNS(shape->position[W], world->camera[X])


	
	modifiedPos[X] = shape->position[X] + world->camera[X];
	modifiedPos[Y] = shape->position[Y] + world->camera[Y];
	modifiedPos[Z] = shape->position[Z] + world->camera[Z];
	/*
	float rotated[4] = {0};
	rotated[0] = world->camera[W];
	rotated[1] = world->camera[I];
	rotated[2] = world->camera[J];
	rotated[3] = world->camera[K];
	quatMult(&shape->position[W], rotated);
	normalizeQuat(rotated);*/


	glUniform4f(ProgramData.camAngleLoc, world->camera[W], world->camera[I], world->camera[J], world->camera[K]);
	//glUniform3f(ProgramData.cordinatesLoc, modifiedPos[X], modifiedPos[Y], modifiedPos[Z]);
	glUniform3f(ProgramData.cordinatesLoc, shape->position[X], shape->position[Y], shape->position[Z]);
	glUniform4f(ProgramData.orientationLoc, shape->position[W], shape->position[I], shape->position[J], shape->position[K]);
	glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
}
