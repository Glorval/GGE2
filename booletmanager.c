#pragma once

static int BooletID = 0;
static float BooletVerts[] = {
	0, 0.05, 0.3,
	0, -0.05, -0.3,
};

struct boolet {
	float pos[7];
	float velocity[3];
	int framesToLive;
	int booletType;
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
		float dist = //sqrtf(
			((enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) * (enemyShipList[c].position[X_pos] - boolet->pos[X_pos]) / off[0]) +
			((enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) * (enemyShipList[c].position[Y_pos] - boolet->pos[Y_pos]) / off[1]) +
			((enemyShipList[c].position[Z_pos] - boolet->pos[Z_pos]) * (enemyShipList[c].position[Z_pos] - boolet->pos[Z_pos]) / off[2]);
		//);

		/*unsigned int i = *(unsigned int*)&dist;
		i += 127 << 23;
		i >>= 1;
		dist = *(float*)&i;*/

		float tolerance = ((fabsf(boolet->velocity[0]) + fabsf(boolet->velocity[1]) + fabsf(boolet->velocity[2]))/ BOOLET_LENIANCY) + EXTRA_BOOLET_TOLERANCE;
		//printf("%f,\t%f\n", dist, tolerance);
		if (dist < tolerance * tolerance) {
			return(c);
		}
	}
		//potential method: Orientate the 'hitbox' according to the ship, then test the distance between
		//the boolet and the ship using the scaled hitbox (akin to the heading stuff) as a tolerance
		
	return(FAIL);
}

//returns 0 normally, 1 on error, -1 on player dying
int updateBoolets(EnShip* enemyShipList, int shipCount, struct ourShip* playerShip) {
	int returnFlag = 0;
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
			//PLAYER BULLET MANAGEMENT- TARGETTING ENEMIES
			if (curBoolet->booletType == PLAYER_BOOLET) {
				int shipHit = checkBooletAgainstAllShips(curBoolet, enemyShipList, shipCount);
				if (shipHit != FAIL) {
					enemyShipList[shipHit].hp--;
					//printf("Ship Hit, HP: %d\n", enemyShipList[shipHit].hp);
					//clear this boolet, it has done its job
					prev->nextBoolet = curBoolet->nextBoolet;
					free(curBoolet);
					curBoolet = prev->nextBoolet;
				} else if (curBoolet->framesToLive < 1) {
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
			//AI BULLET HANDLING, AGAINST US
			else {
				//checking against the ship's position
				float dist = //sqrtf(
					((gameworld.camera[X_pos] - curBoolet->pos[X_pos]) * (gameworld.camera[X_pos] - curBoolet->pos[X_pos])) +
					((gameworld.camera[Y_pos] - curBoolet->pos[Y_pos]) * (gameworld.camera[Y_pos] - curBoolet->pos[Y_pos])) +
					((gameworld.camera[Z_pos] - curBoolet->pos[Z_pos]) * (gameworld.camera[Z_pos] - curBoolet->pos[Z_pos]));
				//);

				/*unsigned int i = *(unsigned int*)&dist;
				i += 127 << 23;
				i >>= 1;
				dist = *(float*)&i;*/

				//printf("%f,\t%f\n", dist, tolerance);
				if (dist < PLAYER_BOOLET_TOLERANCE) {
					//printf("We're hit!\n");
					//priorityTrack(&blastLine);
					bulletAudioHandler(0);
					if (playerShip->shields > 0) {
						playerShip->shields--;
					}
					else if (playerShip->shields == 0) {
						if (playerShip->armour > 0) {
							playerShip->armour--;
						} else {
							playerShip->hp--;
							if (playerShip->hp <= 0) {
								returnFlag = -1;
							}
						}
					}

					if (playerShip->shields < 0 || playerShip->armour < 0) {
						gError("Player shields/armour violation");
						printf("Values: %d, %d, %d\n", playerShip->shields, playerShip->armour, playerShip->hp);
						returnFlag = 1; 
						goto EndOfBooletUpdate;
					}

					prev->nextBoolet = curBoolet->nextBoolet;
					free(curBoolet);
					if (prev->nextBoolet == NULL) {
						goto EndOfBooletUpdate;
					} else {
						curBoolet = prev->nextBoolet;
					}					
				}

				if (curBoolet->framesToLive < 1) {
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
	}

EndOfBooletUpdate:;
	glLineWidth(1);
	bulletAudioHandler(1);
	return(returnFlag);
}

void add_boolet(float* pos/*7 longxyzwijk*/, float* velocity/*3 long, xyz*/, int framesToLive, int playerOrEnemy) {
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
	curBoolet->booletType = playerOrEnemy;
	curBoolet->framesToLive = framesToLive;
}