#pragma once

#define EXTRA_BOOLET_TOLERANCE 1.5 //How much extra distance + the speed should be added to the hit sphere
#define BOOLET_LENIANCY 3 //inverse, higher numbers = lower leniancy. Shouldn't really be one, since then it'd just be checking the frame before & after against collision

static int BooletID = 0;
static float BooletVerts[] = {
	0, 0.05, 0.3,
	0, -0.05, -0.3,
};

struct boolet {
	float pos[7];
	float velocity[3];
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

int checkBooletAgainstAllShips(Boolet* boolet, EnShip* enemyShipList, int shipCount) {
	static const float bodyHitbox[] = {//each hitbox goes x left, x right, y bottom, y top, z back, z front
		-0.8, 0.8,
		-0.1, 0,
		-1.00, 0.08,
	};

	//x,y,z axis
	//axis weight
	static const float off[] = {
		1,
		0.25,
		1,
	};


	static int temp = 0;
	temp++;

	for (int c = 0; c < shipCount; c++) {
		//float dist = sqrtf(
		//	((fabsf(enemyShipList[c].position[X_pos]) - fabsf(boolet->pos[X_pos])) * (fabsf(enemyShipList[c].position[X_pos]) - fabsf(boolet->pos[X_pos]))) +
		//	((fabsf(enemyShipList[c].position[Y_pos]) - fabsf(boolet->pos[Y_pos])) * (fabsf(enemyShipList[c].position[Y_pos]) - fabsf(boolet->pos[Y_pos]))) +
		//	((fabsf(enemyShipList[c].position[Z_pos]) - fabsf(boolet->pos[Z_pos])) * (fabsf(enemyShipList[c].position[Z_pos]) - fabsf(boolet->pos[Z_pos])))
		//);
		//printf("%f, %f\n", 
		//	((enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) * (enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) * off[0]),
		//	((enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) * (enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) * off[1])
		//);
		float dist = sqrtf(
			((enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) * (enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) / off[0]) +
			((enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) * (enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) / off[1]) +
			((enemyShipList[c].position[Z_pos] - boolet->pos[Z_pos]) * (enemyShipList[c].position[Z_pos] - boolet->pos[Z_pos]) / off[2])
		);
		
		float tolerance = ((fabsf(boolet->velocity[0]) + fabsf(boolet->velocity[1]) + fabsf(boolet->velocity[2]))/ BOOLET_LENIANCY) + EXTRA_BOOLET_TOLERANCE;
		//printf("%f,\t%f\n", dist, tolerance);
		if (dist < tolerance) {
			return(c);
		}
	}
		//potential method: Orientate the 'hitbox' according to the ship, then test the distance between
		//the boolet and the ship using the scaled hitbox (akin to the heading stuff) as a tolerance
		/*float back[] = { 0,0,0,-1 };
		float left[] = { 0,-1,0,0 };
		float up[] = { 0,0,1,0 };

		quatMult(&enemyShipList[c].position[W_pos], &up);
		quatMultRS(&up, &enemyShipList[c].position[W_pos]);

		quatMult(&enemyShipList[c].position[W_pos], &left);
		quatMultRS(&left, &enemyShipList[c].position[W_pos]);

		quatMult(&enemyShipList[c].position[W_pos], &back);
		quatMultRS(&back, &enemyShipList[c].position[W_pos]);


		float xpos = (boolet->pos[X_pos] * left[1]) + (boolet->pos[X_pos] * left[2]) + (boolet->pos[X_pos] * left[3]);
		float ypos = (boolet->pos[Y_pos] * up[1]) + (boolet->pos[Y_pos] * up[2]) + (boolet->pos[Y_pos] * up[3]);
		float zpos = (boolet->pos[Z_pos] * back[1]) + (boolet->pos[Z_pos] * back[2]) + (boolet->pos[Z_pos] * back[3]);
		
		float xdist = fabsf(xpos) - fabsf(enemyShipList->position[X_pos]);*/
		/*if (
			(boolet->pos[X_pos] + (boolet->velocity[X_pos] / BOOLET_LENIANCY) > enemyShipList->position[X_pos] + bodyHitbox[0] ||
			boolet->pos[X_pos] > enemyShipList->position[X_pos] + bodyHitbox[0] ||
			boolet->pos[X_pos] - (boolet->velocity[X_pos] / BOOLET_LENIANCY) > enemyShipList->position[X_pos] + bodyHitbox[0]
			)&&
			(boolet->pos[X_pos] + (boolet->velocity[X_pos] / BOOLET_LENIANCY) < enemyShipList->position[X_pos] + bodyHitbox[1] ||
			boolet->pos[X_pos] < enemyShipList->position[X_pos] + bodyHitbox[1] ||
			boolet->pos[X_pos] - (boolet->velocity[X_pos] / BOOLET_LENIANCY) < enemyShipList->position[X_pos] + bodyHitbox[1]
			)&&

			(boolet->pos[Y_pos] + (boolet->velocity[Y_pos] / BOOLET_LENIANCY) > enemyShipList->position[Y_pos] + bodyHitbox[2] ||
			boolet->pos[Y_pos] > enemyShipList->position[Y_pos] + bodyHitbox[2] ||
			boolet->pos[Y_pos] - (boolet->velocity[Y_pos] / BOOLET_LENIANCY) > enemyShipList->position[Y_pos] + bodyHitbox[2]
			) &&
			(boolet->pos[Y_pos] + (boolet->velocity[Y_pos] / BOOLET_LENIANCY) < enemyShipList->position[Y_pos] + bodyHitbox[3] ||
			boolet->pos[Y_pos] < enemyShipList->position[Y_pos] + bodyHitbox[3] ||
			boolet->pos[Y_pos] - (boolet->velocity[Y_pos] / BOOLET_LENIANCY) < enemyShipList->position[Y_pos] + bodyHitbox[3]
			)&&

			(boolet->pos[Z_pos] + (boolet->velocity[Z_pos] / BOOLET_LENIANCY) > enemyShipList->position[Z_pos] + bodyHitbox[4] ||
			boolet->pos[Z_pos] > enemyShipList->position[Z_pos] + bodyHitbox[4] ||
			boolet->pos[Z_pos] - (boolet->velocity[Z_pos] / BOOLET_LENIANCY) > enemyShipList->position[Z_pos] + bodyHitbox[4]
			) &&
			(boolet->pos[Z_pos] + (boolet->velocity[Z_pos] / BOOLET_LENIANCY) < enemyShipList->position[Z_pos] + bodyHitbox[5] ||
			boolet->pos[Z_pos]< enemyShipList->position[Z_pos] + bodyHitbox[5]||
			boolet->pos[Z_pos] - (boolet->velocity[Z_pos] / BOOLET_LENIANCY) < enemyShipList->position[Z_pos] + bodyHitbox[5]
			)
			) {
			printf("Ship Hit here\n");
			return(c);
		}
	}*/
	return(FAIL);
}

void updateBoolets(EnShip* enemyShipList, int shipCount) {
	if (booletlist.nextBoolet != NULL) {
		glLineWidth(2);
		glBindVertexArray(BooletID);
		glUniform1f(ProgramData.flagsLoc, RENDER_MODE_VECTOR);
		glUniform1f(ProgramData.scaleLoc, 0);
		glUniform3f(ProgramData.cameraPosLoc, gameworld.camera[X_pos], gameworld.camera[Y_pos], gameworld.camera[Z_pos]);
		glUniform4f(ProgramData.camAngleLoc, gameworld.camera[W_pos], gameworld.camera[I_pos], gameworld.camera[J_pos], gameworld.camera[K_pos]);
		Boolet* curBoolet = booletlist.nextBoolet;
		Boolet* prev = &booletlist;

		while (curBoolet != NULL) {
			int shipHit = checkBooletAgainstAllShips(curBoolet, enemyShipList, shipCount);
			if (shipHit != FAIL) {
				enemyShipList[shipHit].hp--;
				printf("Ship Hit, HP: %d\n", enemyShipList[shipHit].hp);
				//clear this boolet, it has done its job
				prev->nextBoolet = curBoolet->nextBoolet;
				free(curBoolet);
				curBoolet = prev->nextBoolet;
			}
			else if (curBoolet->framesToLive < 1) {
				prev->nextBoolet = curBoolet->nextBoolet;
				free(curBoolet);
				curBoolet = prev->nextBoolet;
			} else {				
				glUniform3f(ProgramData.cordinatesLoc, curBoolet->pos[X_pos], curBoolet->pos[Y_pos], curBoolet->pos[Z_pos]);
				glUniform4f(ProgramData.orientationLoc, -curBoolet->pos[W_pos], curBoolet->pos[I_pos], curBoolet->pos[J_pos], curBoolet->pos[K_pos]);
				
				curBoolet->pos[X_pos] += curBoolet->velocity[X_pos];
				curBoolet->pos[Y_pos] += curBoolet->velocity[Y_pos];
				curBoolet->pos[Z_pos] += curBoolet->velocity[Z_pos];
				glDrawArrays(GL_LINES, 0, 2);
				curBoolet->framesToLive = curBoolet->framesToLive - 1;
				prev = curBoolet;
				curBoolet = curBoolet->nextBoolet;
				
			}
		}
	}
	glLineWidth(1);
}

void add_boolet(float* pos/*7 longxyzwijk*/, float* velocity/*3 long, xyz*/, int framesToLive) {
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
	curBoolet->velocity[0] = velocity[0];
	curBoolet->velocity[1] = velocity[1];
	curBoolet->velocity[2] = velocity[2];
	curBoolet->framesToLive = framesToLive;
}