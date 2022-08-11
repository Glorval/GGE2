#pragma once

#define ENSHIP "Enemy Ship"

struct idset {
	unsigned int ID, indC;
};
typedef struct idset idSet;

void addRefEntry(unsigned int ID, int indlen, char* name) {
	masterObjLenght++;
	masterObjList = realloc(masterObjList, sizeof(RefObj) * masterObjLenght);
	masterObjList[masterObjLenght - 1].ID = ID;
	masterObjList[masterObjLenght - 1].objName = name;
	masterObjList[masterObjLenght - 1].indlen = indlen;
}

idSet getRefID(char* refName) {
	idSet returns = { 0 };
	for (unsigned int cItem = 0; cItem < masterObjLenght; cItem++) {
		if (strcmp(refName, masterObjList[cItem].objName) == 0) {
			returns.ID = masterObjList[cItem].ID;
			returns.indC = masterObjList[cItem].indlen;
			return(returns);
		}
	}
}

unsigned int loadEnemyShip() {
	float shipVerts[] = {
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
	//naturallyCentreVertices(shipVerts, 21, 3);

	float shipVertsBig[] = {
		//windshield
		-0.10, 0.0, 0.0,					//0
		0.10, 0.0, 0.0,					//1
		-0.12, 0.2, -0.28, 				//2
		0.12, 0.2, -0.28,				//3
		-0.16, 0.0, -0.14,				//4
		0.16, 0.0, -0.14,					//5 

		//front frame	
		0, -0.10, 0.16,					//6 - front point
		-0.14, -0.16, 0.04,				//7 
		0.14, -0.16, 0.04,				//8 

		//wings
		-0.30, -0.08, -0.12,			//9
		0.30, -0.08, -0.12,				//10
		-0.8, -0.08, -1.10,				//11
		0.8, -0.08, -1.10,				//12

//back frame
-0.16, 0.0, -0.90,				//13
0.16, 0.0, -0.90,				//14
-0.12, 0.2, -0.60,				//15//back of the windshield
0.12, 0.2, -0.60,				//16
-0.16, 0.0, -0.76,				//17
0.16, 0.0, -0.76,				//18
-0.16, -0.16, -0.90,			//19//back middle
0.16, -0.16, -0.90,				//20
	};
	naturallyCentreVertices(shipVertsBig, 21, 3);

	unsigned int shipInds[] = {
		0,1, 0,2, 2,3, 3,1, 0,4, 1,5, 4,2, 5,3,//windshield
		0,6, 6,1, 6,7, 6,8, 7,8, 0,7, 1,8, 7,4, 8,5,//front frame and attaching to windshield
		7,9, 9,4, 8,10, 10,5, 9,11, 10,12, 7,11, 8,12,//wings
		13,4, 14,5, 13,11, 14,12, 15,2, 16,3, 15,17, 16,18, 15,16, 13,14,//mix of wing, back cockpit, back frame
		7,19, 8,20, 19,20, 11,19, 12,20, //bottom frame and its connections
	};

	Object vectorobj = createStaticVectorObject(shipVertsBig, shipInds, (sizeof(shipVerts) / VECTOR_VERTEX_LENGTH) / sizeof(float), sizeof(shipInds) / sizeof(unsigned int));
	//vectorobj.scale = 3;
	//char* enemyShip = calloc(sizeof("Enemy Ship"), 1);
	//memcpy(enemyShip, "Enemy Ship", sizeof("Enemy Ship") / sizeof(char));
	static char* enemyShip = ENSHIP;
	addRefEntry(vectorobj.ID, vectorobj.indexCount, enemyShip);

}

void faceShip(EnShip* us, struct ourShip OurShip) {
	float forwardVector[3] = {
		(gameworld.camera[X_pos] + (OurShip.heading[X_pos] * 40)) - us->position[X_pos],
		(gameworld.camera[Y_pos] + (OurShip.heading[Y_pos] * 40)) - us->position[Y_pos],
		(gameworld.camera[Z_pos] + (OurShip.heading[Z_pos] * 40)) - us->position[Z_pos],
	};

	forwardVector[0] *= 1 + (((float)(rand() % 20000) / 100) - 100) * ENEMY_SHIP_DEVIATION;
	forwardVector[1] *= 1 + (((float)(rand() % 20000) / 100) - 100) * ENEMY_SHIP_DEVIATION;
	forwardVector[2] *= 1 + (((float)(rand() % 20000) / 100) - 100) * ENEMY_SHIP_DEVIATION;

	norm3(forwardVector);

	float dot = dotP3(&us->forward[1], forwardVector);

	if (fabsf(dot + 1.0) < 0.00001 || fabsf(dot - 1.0) < 0.00001) {
	} else {
		if (dot >= 1 || dot <= -1) {
			//printf("saved");
			return;
		}
		float rotAngle = acosf(dot);
		float* rotAxis = crossP3(&us->forward[1], forwardVector);
		norm3(rotAxis);
		//printf("%f, %f, %f, %f\n", rotAngle, rotAxis[0], rotAxis[1], rotAxis[2]);
		float ha = rotAngle / ENEMY_AGILITY;
		float sina = sinf(ha);
		float rotQuat[] = {
			cosf(ha),
			rotAxis[0] * sina,
			rotAxis[1] * sina,
			rotAxis[2] * sina,
		};

		normalizeQuat(rotQuat);

		float conj[] = {
			rotQuat[0],
			-rotQuat[1],
			-rotQuat[2],
			-rotQuat[3],
		};
		normalizeQuat(conj);
		quatMult(rotQuat, &us->position[W_pos]);

		quatMult(rotQuat, us->forward);
		quatMultRS(us->forward, conj);

		quatMult(rotQuat, us->up);
		quatMultRS(us->up, conj);

		quatMult(rotQuat, us->right);
		quatMultRS(us->right, conj);
	}
}

void alignTo(EnShip* us, const float axis[3]) {
	float forwardVector[3] = {
		axis[X_pos],// - us->position[X_pos],
		axis[Y_pos],// - us->position[Y_pos],
		axis[Z_pos],// - us->position[Z_pos],
	};

	norm3(forwardVector);

	float dot = dotP3(&us->forward[1], forwardVector);

	if (fabsf(dot + 1.0) < 0.00001 || fabsf(dot - 1.0) < 0.00001) {
	} else {
		if (dot >= 1 || dot <= -1) {
			//printf("saved");
			return;
		}
		float rotAngle = acosf(dot);
		float* rotAxis = crossP3(&us->forward[1], forwardVector);
		norm3(rotAxis);
		//printf("%f, %f, %f, %f\n", rotAngle, rotAxis[0], rotAxis[1], rotAxis[2]);
		float ha = rotAngle / (ENEMY_AGILITY* 4);
		float sina = sinf(ha);
		float rotQuat[] = {
			cosf(ha),
			rotAxis[0] * sina,
			rotAxis[1] * sina,
			rotAxis[2] * sina,
		};

		normalizeQuat(rotQuat);

		float conj[] = {
			rotQuat[0],
			-rotQuat[1],
			-rotQuat[2],
			-rotQuat[3],
		};
		normalizeQuat(conj);
		quatMult(rotQuat, &us->position[W_pos]);

		quatMult(rotQuat, us->forward);
		quatMultRS(us->forward, conj);

		quatMult(rotQuat, us->up);
		quatMultRS(us->up, conj);

		quatMult(rotQuat, us->right);
		quatMultRS(us->right, conj);
	}
}



void updateOurAI(EnShip* us, struct ourShip PlayerShip, char targetAllowed) {
	//if we are allowed to become targeting
	const static float realignVector[3] = {
		0,0,1
	};
	if (targetAllowed == 1) {
		//and are not targetting
		if (us->targeting == 0) {
			//and we aren't too close/far to start
			if ((gameworld.camera[Z_pos] - us->position[Z_pos]) > ENEMY_TARGET_DIST &&
				((gameworld.camera[Z_pos] - us->position[Z_pos]) < ENEMY_TARGET_DIST_MAX)) {
				//roll a check to see if we should start
				if ((float)(rand() % 10000) / 100 <= ENEMY_TARGET_CHANCE) {
					us->targeting = 1;
				}
			}
		}
	}

	//if we're trying to kill the player
	if (us->targeting == 1) {
		us->speed = ENEMY_MAX_SPEED;
		//quickly make sure we shouldn't stop targetting
		if (((gameworld.camera[X_pos] - us->position[X_pos]) * (gameworld.camera[X_pos] - us->position[X_pos]) +
			(gameworld.camera[Y_pos] - us->position[Y_pos]) * (gameworld.camera[Y_pos] - us->position[Y_pos]) +
			(gameworld.camera[Z_pos] - us->position[Z_pos]) * (gameworld.camera[Z_pos] - us->position[Z_pos]))
			< ENEMY_TARGET_DIST_MIN
			){
			us->targeting = 2;//dip out
			goto targettingTwoCheck;
		}

		//face where they're going
		faceShip(us, PlayerShip);
		//try to start shooting if applicable
		if (us->fireFrame == 0) {
			//check against fire start chance
			if ((float)(rand() % 10000) / 100 < ENEMY_FIRE_CHANCE) {
				us->fireFrame = ENEMY_BURST_LEN + 1;//since zero is not doing it
			}
		}

		if (us->fireFrame > 0) {
			us->fireFrame--;
			const float velocity[3] = {
				us->forward[X_pos + 1] * BOOLETSPEED,
				us->forward[Y_pos + 1] * BOOLETSPEED,
				us->forward[Z_pos + 1] * BOOLETSPEED,
			};

			float booletPos[7] = {
				us->position[X_pos],
				us->position[Y_pos],
				us->position[Z_pos],
				us->position[W_pos],
				us->position[I_pos],
				us->position[J_pos],
				us->position[K_pos],
			};

			//from the enemy ship's perspective
			//up and to right
			if (us->fireFrame % ENEMY_FIRE_RATE == 0) {
				booletPos[X_pos] += ENEMY_BOOLET_OFFSET * (us->right[X_pos + 1] + (us->up[X_pos + 1] * 0.5));
				booletPos[Y_pos] += ENEMY_BOOLET_OFFSET * (us->right[Y_pos + 1] + (us->up[Y_pos + 1] * 0.5));
				booletPos[Z_pos] += ENEMY_BOOLET_OFFSET * (us->right[Z_pos + 1] + (us->up[Z_pos + 1] * 0.5));
				add_boolet(booletPos, velocity, BOOLETLIFE, ENEMY_BOOLET);
			}
			//up and to left
			else if (us->fireFrame % ENEMY_FIRE_RATE == 2) {
				booletPos[X_pos] += ENEMY_BOOLET_OFFSET * ((us->up[X_pos + 1]*0.5) - us->right[X_pos + 1]);
				booletPos[Y_pos] += ENEMY_BOOLET_OFFSET * ((us->up[Y_pos + 1] * 0.5) - us->right[Y_pos + 1]);
				booletPos[Z_pos] += ENEMY_BOOLET_OFFSET * ((us->up[Z_pos + 1] * 0.5) - us->right[Z_pos + 1]);
				add_boolet(booletPos, velocity, BOOLETLIFE, ENEMY_BOOLET);
			}
			//down and to right
			else if (us->fireFrame % ENEMY_FIRE_RATE == 4) {
				booletPos[X_pos] -= ENEMY_BOOLET_OFFSET * ((us->up[X_pos + 1] * 0.5) - us->right[X_pos + 1]);
				booletPos[Y_pos] -= ENEMY_BOOLET_OFFSET * ((us->up[Y_pos + 1] * 0.5) - us->right[Y_pos + 1]);
				booletPos[Z_pos] -= ENEMY_BOOLET_OFFSET * ((us->up[Z_pos + 1] * 0.5) - us->right[Z_pos + 1]);
				add_boolet(booletPos, velocity, BOOLETLIFE, ENEMY_BOOLET);
			}
			//down and to left
			else if (us->fireFrame % ENEMY_FIRE_RATE == 6) {
				booletPos[X_pos] -= ENEMY_BOOLET_OFFSET * (us->right[X_pos + 1] + (us->up[X_pos + 1] * 0.5));
				booletPos[Y_pos] -= ENEMY_BOOLET_OFFSET * (us->right[Y_pos + 1] + (us->up[Y_pos + 1] * 0.5));
				booletPos[Z_pos] -= ENEMY_BOOLET_OFFSET * (us->right[Z_pos + 1] + (us->up[Z_pos + 1] * 0.5));
				add_boolet(booletPos, velocity, BOOLETLIFE, ENEMY_BOOLET);
			}
			
		}
	}

	targettingTwoCheck:;
	if (us->targeting == 2) {
		
		alignTo(us, realignVector);
	}
}

