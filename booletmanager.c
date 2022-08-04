#pragma once

static int BooletID = 0;
static float BooletVerts[] = {
	0, 0, 0,
	0, 0, 0.4
};

struct boolet {
	float pos[7];
	float speed;
	int framesToLive;
	void* nextBoolet;
};
typedef struct boolet Boolet;
Boolet booletlist;

void setupBoolet() {
	booletlist.nextBoolet = NULL;
	unsigned int VBO = 0;
	unsigned int EBO = 0;
	glGenVertexArrays(1, &BooletID);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(BooletID);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), BooletVerts, GL_STATIC_DRAW);

	// position attribute, first three
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void renderBoolet() {
	if (booletlist.nextBoolet != NULL) {
		glBindVertexArray(BooletID);
		glUniform1f(ProgramData.flagsLoc, RENDER_MODE_VECTOR);
		glUniform1f(ProgramData.scaleLoc, 0);
		Boolet* curBoolet = booletlist.nextBoolet;
		Boolet* prev = &booletlist;

		while (curBoolet != NULL) {
			if (curBoolet->framesToLive < 1) {
				prev->nextBoolet = curBoolet->nextBoolet;
				free(curBoolet);
				curBoolet = prev->nextBoolet;
			} else {
				curBoolet->pos[X_pos] += curBoolet->pos[X_pos] * curBoolet->speed;
				curBoolet->pos[Y_pos] += curBoolet->pos[Y_pos] * curBoolet->speed;
				curBoolet->pos[Z_pos] += curBoolet->pos[Z_pos] * curBoolet->speed;
				glUniform3f(ProgramData.cordinatesLoc, curBoolet->pos[X_pos], curBoolet->pos[Y_pos], curBoolet->pos[Z_pos]);
				glUniform4f(ProgramData.orientationLoc, curBoolet->pos[W_pos], curBoolet->pos[I_pos], curBoolet->pos[J_pos], curBoolet->pos[K_pos]);
				
				glDrawArrays(GL_LINES, 0, 2);
				curBoolet = curBoolet->nextBoolet;
			}
		}
	}
	
}

void add_boolet(float* pos/*7 long*/, float speed /*speed is based off pos since it goes straight*/, int framesToLive) {
	Boolet* nextBoolet = booletlist.nextBoolet;

	Boolet* curBoolet = NULL;
	curBoolet = realloc(curBoolet, sizeof(Boolet));
	curBoolet->nextBoolet = nextBoolet;
	booletlist.nextBoolet = curBoolet;
	curBoolet->pos[0] = pos[0];
	curBoolet->pos[1] = pos[1];
	curBoolet->pos[2] = pos[2];
	curBoolet->pos[3] = pos[3];
	curBoolet->pos[4] = pos[4];
	curBoolet->pos[5] = pos[5];
	curBoolet->pos[6] = pos[6];
	curBoolet->speed = speed;
	curBoolet->framesToLive = framesToLive;
}