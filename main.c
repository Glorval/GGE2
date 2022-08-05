#define _CRT_SECURE_NO_WARNINGS
//#include "GGE2_1.h"

#include "LibertyBlaster.h" //Includes GGE2.1

#include <time.h>
#include <stdio.h>
#include <math.h>
#include <process.h>


void moveCam(World* ourWorld);

//int WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,	LPSTR lpCmdLine,	int nShowCmd) {
int main(){
	printf("Hi\n");

	//testworld = createWorld(STANDARD_WORLD);	

	volatile World* lineworld = loadGame();

	

	_beginthread(moveCam, 0, lineworld);
	
	setupMainMenu();

	float counter = 1;
	int gameFlag = IN_MAIN_MENU;
	clock_t start, end;
	srand(time(NULL));
	while (!glfwWindowShouldClose(gamewindow)) {
		//drawWorld(lineworld);
		//runMasterUI();

		start = clock();
		runGame(gamewindow, gameFlag);
		end = clock();

		//printf("Time for this frame: %d\n", end - start);
		gameFlag = getsetGamestate(DONT_STATE_CHANGE);
		counter += 1;
	}



	glfwTerminate();
	
	return 0;
}





void moveCam(World* ourWorld) {
	while (1) {
		char input = getche();
		/*
		if (input == 'w') {
			ourWorld->camera[Y_pos] += 0.05;
			printf("\n%f\n", ourWorld->camera[Y_pos]);
		}
		if (input == 's') {
			ourWorld->camera[Y_pos] -= 0.05;
			printf("\n%f\n", ourWorld->camera[Y_pos]);
		}
		if (input == 'a') {
			ourWorld->camera[X_pos] -= 0.05;
			printf("\n%f\n", ourWorld->camera[X_pos]);
		}
		if (input == 'd') {
			ourWorld->camera[X_pos] += 0.05;
			printf("\n%f\n", ourWorld->camera[X_pos]);
		}
		if (input == 'q') {
			float temp = ourWorld->camera[Z_pos];
			if (temp < 0) {
				temp *= -1;
			}
			temp = temp / 10;
			ourWorld->camera[Z_pos] -= (0.15 + temp);
			printf("\n%f\n", ourWorld->camera[Z_pos]);
		}
		if (input == 'e') {
			float temp = ourWorld->camera[Z_pos];
			if (temp < 0) {
				temp *= -1;
			}
			temp = temp / 10;
			ourWorld->camera[Z_pos] += (0.15 + temp);
			printf("\n%f\n", ourWorld->camera[Z_pos]);
		}*/
		float posMag = (ourWorld->camera[X_pos] + ourWorld->camera[Y_pos] + ourWorld->camera[Z_pos]) / 3;
		if (posMag < 0) {
			posMag *= -1;
		}
		if (input == 'w') {
			ourWorld->camera[X_pos] -= ourWorld->back[1] * (.05 + (posMag/10));
			ourWorld->camera[Y_pos] -= ourWorld->back[2] * (.05 + (posMag / 10));
			ourWorld->camera[Z_pos] -= ourWorld->back[3] * (.05 + (posMag / 10));
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 's') {
			ourWorld->camera[X_pos] += ourWorld->back[1] * (.05 + (posMag / 10));
			ourWorld->camera[Y_pos] += ourWorld->back[2] * (.05 + (posMag / 10));
			ourWorld->camera[Z_pos] += ourWorld->back[3] * (.05 + (posMag / 10));
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 'a') {
			ourWorld->camera[X_pos] += ourWorld->left[1] * .01;
			ourWorld->camera[Y_pos] += ourWorld->left[2] * .01;
			ourWorld->camera[Z_pos] += ourWorld->left[3] * .01;
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 'd') {
			ourWorld->camera[X_pos] -= ourWorld->left[1] * .01;
			ourWorld->camera[Y_pos] -= ourWorld->left[2] * .01;
			ourWorld->camera[Z_pos] -= ourWorld->left[3] * .01;
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 'q') {
			ourWorld->camera[X_pos] += ourWorld->up[1] * .01;
			ourWorld->camera[Y_pos] += ourWorld->up[2] * .01;
			ourWorld->camera[Z_pos] += ourWorld->up[3] * .01;
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 'e') {
			ourWorld->camera[X_pos] -= ourWorld->up[1] * .01;
			ourWorld->camera[Y_pos] -= ourWorld->up[2] * .01;
			ourWorld->camera[Z_pos] -= ourWorld->up[3] * .01;
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}

		if (input == '6') {

			//float rotateQuat[4] = { cosf(0.0872665 / 2), 0 , 1 * sinf(0.0872665 / 2), 0 };
			float rotateQuat[4] = { cosf(0.0872665 / 2), 0 , 1 * sinf(0.0872665 / 2), 0 };
			float rotQuatConj[4] = { rotateQuat[0], 0, -rotateQuat[2], 0 };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			//normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;
			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '4') {
			//float rotateQuat[4] = { cosf(-0.0872665 / 2), ourWorld->up[1] * sinf(-0.0872665 / 2), ourWorld->up[2] * sinf(-0.0872665 / 2), ourWorld->up[3] * sinf(-0.0872665 / 2) };
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 0, 1 * sinf(-0.0872665 / 2), 0 };
			float rotQuatConj[4] = { rotateQuat[0], 0, -rotateQuat[2], 0 };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;

			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}


		if (input == '8') {
			//float rotateQuat[4] = { cosf(0.0872665 / 2), ourWorld->left[1] * sinf(0.0872665 / 2), ourWorld->left[2] * sinf(0.0872665 / 2), ourWorld->left[3] * sinf(0.0872665 / 2) };
			float rotateQuat[4] = { cosf(-0.0872665 / 2), 1 * sinf(-0.0872665 / 2), 0, 0 };
			float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], 0, 0 };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;

			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}
		if (input == '5') {
			//float rotateQuat[4] = { cosf(-0.0872665 / 2), ourWorld->left[1] * sinf(-0.0872665 / 2), ourWorld->left[2] * sinf(-0.0872665 / 2), ourWorld->left[3] * sinf(-0.0872665 / 2) };
			float rotateQuat[4] = { cosf(0.0872665 / 2), 1 * sinf(0.0872665 / 2), 0, 0 };
			float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], 0, 0 };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;

			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}

		if (input == '9') {
			float rotateQuat[4] = { cosf(-0.0872665 / 2), ourWorld->back[1] * sinf(-0.0872665 / 2), ourWorld->back[2] * sinf(-0.0872665 / 2), ourWorld->back[3] * sinf(-0.0872665 / 2) };
			float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], -rotateQuat[2], -rotateQuat[3] };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;

			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}

		if (input == '7') {
			float rotateQuat[4] = { cosf(0.0872665 / 2), ourWorld->back[1] * sinf(0.0872665 / 2), ourWorld->back[2] * sinf(0.0872665 / 2), ourWorld->back[3] * sinf(0.0872665 / 2) };
			float rotQuatConj[4] = { rotateQuat[0], -rotateQuat[1], -rotateQuat[2], -rotateQuat[3] };
			normalizeQuat(&rotateQuat);
			normalizeQuat(&rotQuatConj);
			quatMult(rotateQuat, &ourWorld->camera[W_pos]);
			normalizeQuat(&ourWorld->camera[W_pos]);

			quatMult(rotateQuat, ourWorld->up);
			quatMultRS(ourWorld->up, rotQuatConj);
			ourWorld->up[0] = 0;

			quatMult(rotQuatConj, ourWorld->back);
			quatMultRS(ourWorld->back, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->back[0], ourWorld->back[1], ourWorld->back[2], ourWorld->back[3]);
			ourWorld->back[0] = 0;

			quatMult(rotQuatConj, ourWorld->left);
			quatMultRS(ourWorld->left, rotateQuat);
			printf("\n%f, %f, %f, %f\n", ourWorld->left[0], ourWorld->left[1], ourWorld->left[2], ourWorld->left[3]);
			ourWorld->left[0] = 0;

			printf("\n%f, %f, %f, %f\n", ourWorld->camera[W_pos], ourWorld->camera[I_pos], ourWorld->camera[J_pos], ourWorld->camera[K_pos]);
			//normalizeQuat(&ourWorld->camera[W]);
		}

	}

}













/*//left front circle
		0.0 - 0.1083,			-0.0482 - 0.0318,	 0.06,		//9
		0.0283 - 0.1083,	-0.039 - 0.0318, 0.06,			//10
		0.0459 - 0.1083,	-0.0149 - 0.0318, 0.06,			//11
		0.0459 - 0.1083,	0.0149 - 0.0318, 0.06,			//12
		0.0283 - 0.1083,	0.039 - 0.0318, 0.06,			//13
		0.0 - 0.1083,			0.0482 - 0.0318, 0.06,			//14
		-0.0283 - 0.1083,	0.039 - 0.0318, 0.06,			//15
		-0.0459 - 0.1083,	0.0149 - 0.0318, 0.06,			//16
		-0.0459 - 0.1083,	-0.0149 - 0.0318, 0.06,			//17
		-0.0283 - 0.1083,	-0.039 - 0.0318, 0.06,			//18

		//right front circle
		0.0 + 0.1083,			-0.0482 - 0.0318,	 0.06,		//19
		0.0283 + 0.1083,	-0.039 - 0.0318, 0.06,			//20
		0.0459 + 0.1083,	-0.0149 - 0.0318, 0.06,			//21
		0.0459 + 0.1083,	0.0149 - 0.0318, 0.06,			//22
		0.0283 + 0.1083,	0.039 - 0.0318, 0.06,			//23
		0.0 + 0.1083,			0.0482 - 0.0318, 0.06,			//24
		-0.0283 + 0.1083,	0.039 - 0.0318, 0.06,			//25
		-0.0459 + 0.1083,	0.0149 - 0.0318, 0.06,			//26
		-0.0459 + 0.1083,	-0.0149 - 0.0318, 0.06,			//27
		-0.0283 + 0.1083,	-0.039 - 0.0318, 0.06,			//28


		//left rear circle
		0.0 - 0.1083,			-0.0482 - 0.0318,	 -0.50,		//29
		0.0283 - 0.1083,	-0.039 - 0.0318, -0.50,			//30
		0.0459 - 0.1083,	-0.0149 - 0.0318, -0.50,		//31
		0.0459 - 0.1083,	0.0149 - 0.0318, -0.50,			//32
		0.0283 - 0.1083,	0.039 - 0.0318, -0.50,			//33
		0.0 - 0.1083,			0.0482 - 0.0318, -0.50,		//34
		-0.0283 - 0.1083,	0.039 - 0.0318, -0.50,			//35
		-0.0459 - 0.1083,	0.0149 - 0.0318, -0.50,			//36
		-0.0459 - 0.1083,	-0.0149 - 0.0318, -0.50,		//37
		-0.0283 - 0.1083,	-0.039 - 0.0318, -0.50,			//38

		//right rear circle
		0.0 + 0.1083,			-0.0482 - 0.0318,	 -0.50,		//39
		0.0283 + 0.1083,	-0.039 - 0.0318, -0.50,			//40
		0.0459 + 0.1083,	-0.0149 - 0.0318, -0.50,		//41
		0.0459 + 0.1083,	0.0149 - 0.0318, -0.50,			//42
		0.0283 + 0.1083,	0.039 - 0.0318, -0.50,			//43
		0.0 + 0.1083,			0.0482 - 0.0318, -0.50,		//44
		-0.0283 + 0.1083,	0.039 - 0.0318, -0.50,			//45
		-0.0459 + 0.1083,	0.0149 - 0.0318, -0.50,			//46
		-0.0459 + 0.1083,	-0.0149 - 0.0318, -0.50,		//47
		-0.0283 + 0.1083,	-0.039 - 0.0318, -0.50,			//48
		
		9,10, 10,11, 11,12, 12,13, 13,14, 14,15, 15,16, 16,17, 17,18, 18,9, // left front circle
		19,20, 20,21, 21,22, 22,23, 23,24, 24,25, 25,26, 26,27, 27,28, 28,19, // right front circle
		29,30, 30,31, 31,32, 32,33, 33,34, 34,35, 35,36, 36,37, 37,38, 38,29, // left rear circle
		39,40, 40,41, 41,42, 42,43, 43,44, 44,45, 45,46, 46,47, 47,48, 48,39, // right rear circle
		
		
		
		
		
		*/