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

	returnObject.position[X_pos] = 0;
	returnObject.position[Y_pos] = 0;
	returnObject.position[Z_pos] = 0;
	returnObject.position[W_pos] = 1;
	returnObject.position[I_pos] = 0;
	returnObject.position[J_pos] = 1;//1, idk why
	returnObject.position[K_pos] = 0;
	normalizeQuat(&returnObject.position[W_pos]);

	returnObject.updateOpenGLData = 0;

	return(returnObject);
}

//draws object as is
void drawObject(Object* shape) {
	glBindVertexArray(shape->ID);
	glUniform3f(ProgramData.cordinatesLoc, shape->position[X_pos], shape->position[Y_pos], shape->position[Z_pos]);
	glUniform4f(ProgramData.orientationLoc, shape->position[W_pos], shape->position[I_pos], shape->position[J_pos], shape->position[K_pos]);
	glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
}


//Draws an object from a world setting all the uniforms and such
void drawWorldObject(Object* shape, World* world) {
	glBindVertexArray(shape->ID);
	glUniform1i(ProgramData.flagsLoc, 0);
	glUniform3f(ProgramData.cameraPosLoc, world->camera[X_pos], world->camera[Y_pos], world->camera[Z_pos]);
	glUniform4f(ProgramData.camAngleLoc, world->camera[W_pos], world->camera[I_pos], world->camera[J_pos], world->camera[K_pos]);
	glUniform3f(ProgramData.cordinatesLoc, shape->position[X_pos], shape->position[Y_pos], shape->position[Z_pos]);
	glUniform4f(ProgramData.orientationLoc, shape->position[W_pos], shape->position[I_pos], shape->position[J_pos], shape->position[K_pos]);
	glDrawElements(GL_TRIANGLES, shape->indexCount, GL_UNSIGNED_INT, 0);
}
