#define _CRT_SECURE_NO_WARNINGS
#define _WIN32_WINNT 0x0400

//#include "GGE2_1.h"
#include <Windows.h>


#include "LibertyBlaster.h" //Includes GGE2.1

#include <time.h>
//#include <stdio.h>
#include <math.h>
#include <process.h>


//void moveCam(World* ourWorld);

int WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,	LPSTR lpCmdLine,	int nShowCmd) {
//int main(){
	
	



	FILE* debugfile = fopen("debugfile.txt", "w");
	fputs("The program at least begins to execute.\n\n", debugfile);
	fclose(debugfile);
	debugfile = fopen("debugfile.txt", "a");
	fputs("Test second line\n\n", debugfile);
	fclose(debugfile);
	//testworld = createWorld(STANDARD_WORLD);	

	volatile World* lineworld = loadGame();


	/*int easySwap = glfwExtensionSupported("GLX_EXT_swap_control_tear");
	if (easySwap == GLFW_FALSE) {
		easySwap = glfwExtensionSupported("WGL_EXT_swap_control_tear");
	}

	if (easySwap == 1) {
		glfwSwapInterval(-100);
	} else {
		glfwSwapInterval(0);
	}*/

	
	
	_beginthread(debugCommands, 0, NULL);

	/*int audioID = createTrackHandler();
	AudioTrack testTrack = { 0 };
	int tracklen = 18;
	testTrack.tones = calloc(tracklen, sizeof(UINT));
	testTrack.tones[0] = Ab3;
	testTrack.tones[1] = Ab3;
	testTrack.tones[2] = Ab4;
	testTrack.tones[3] = Ab3;
	testTrack.tones[4] = Gb4;
	testTrack.tones[5] = Ab3;
	testTrack.tones[6] = E4;
	testTrack.tones[7] = Eb4;
	testTrack.tones[8] = E4;
	testTrack.tones[9] = F4;
	testTrack.tones[10] = Gb4;
	testTrack.tones[11] = Ab4;	
	testTrack.tones[12] = G4;
	testTrack.tones[13] = E4;
	testTrack.tones[14] = D4;
	testTrack.tones[15] = Ab4;
	testTrack.tones[16] = Ab4;
	testTrack.tones[17] = Ab4;
	testTrack.toneLengths = calloc(tracklen, sizeof(UINT));
	testTrack.toneLengths[0] = 200;
	testTrack.toneLengths[1] = 200;
	testTrack.toneLengths[2] = 200;
	testTrack.toneLengths[3] = 400;
	testTrack.toneLengths[4] = 200;
	testTrack.toneLengths[5] = 400;
	testTrack.toneLengths[6] = 400;
	testTrack.toneLengths[7] = 400;
	testTrack.toneLengths[8] = 200;
	testTrack.toneLengths[9] = 200;
	testTrack.toneLengths[10] = 200;
	testTrack.toneLengths[11] = 400;
	testTrack.toneLengths[12] = 400;
	testTrack.toneLengths[13] = 400;
	testTrack.toneLengths[14] = 400;
	testTrack.toneLengths[15] = 200;
	testTrack.toneLengths[16] = 400;
	testTrack.toneLengths[17] = 600;
	testTrack.cPos = 0;
	testTrack.enabled = 1;
	testTrack.loop = 1;
	testTrack.tracklen = tracklen;
	setPlayFreq(audioID, 0);
	addTrack(audioID, testTrack);*/

	/*int audioID = createTrackHandler();
	AudioTrack testTrack = { 0 };
	static unsigned int tones[] = { Ab3, Ab3, Ab4, Ab3, Gb4, Ab3, Eb4, D4, C4, Bb3, Bb3, Ab3, Ab3, Ab3, Bb3, Ab3, Gb3, Ab3, C4, Ab3, Ab3, Ab3, Fs3, Ab3, Ab3, Ab3, F3, Ab3,};
	int tracklen = countof(tones);
	testTrack.tones = tones;
	testTrack.toneLengths = calloc(tracklen, sizeof(UINT));
	testTrack.toneLengths[2] = 400;
	testTrack.toneLengths[3] = 400;
	testTrack.toneLengths[4] = 400;
	testTrack.toneLengths[5] = 400;

	testTrack.cPos = 0;
	testTrack.enabled = 1;
	testTrack.loop = 1;
	testTrack.tracklen = tracklen;
	setDefaultLength(audioID, 200);
	addTrack(audioID, testTrack);*/

	float counter = 1;
	int gameFlag = IN_MAIN_MENU;
	srand(time(NULL));
	int framesTillCheck = 60;
	ULONGLONG startOf, endOf;
	startOf = GetTickCount64();




	//pain, did fixed physics
	//GLFWmonitor* primary = glfwGetPrimaryMonitor();
	//GLFWvidmode* mode = glfwGetVideoMode(primary);
	HANDLE hTimer = NULL;
	LARGE_INTEGER liDueTime;
	liDueTime.QuadPart = -160000;//-166666 hundred ns, 16.666ms, 1/60 of 1 second. BUT, rounded down for inaccuracy of system timer
	hTimer = CreateWaitableTimer(NULL, FALSE, NULL);	
	if (hTimer == NULL) {
		goto end;
	}
	glfwSwapInterval(0);
	SetWaitableTimer(hTimer, &liDueTime, 16, NULL, NULL, 0);
start:;
	

	runGame(gamewindow, gameFlag);
	glFlush();
	WaitForSingleObject(hTimer, INFINITE);
	glfwSwapBuffers(gamewindow);
	glfwPollEvents();
	gameFlag = getsetGamestate(DONT_STATE_CHANGE);

	framesTillCheck--;
	if (framesTillCheck == 0) {
		endOf = GetTickCount64();
		float fps = 60000.0 / (float)(endOf - startOf);
		startOf = endOf;

		printf("FPS: %f\n", fps);
		framesTillCheck = 60;
	}

	if (glfwWindowShouldClose(gamewindow)) {
		goto end;
	}
	goto start;
	









	/*
	if (mode->refreshRate != 60 && easySwap == 1) {
		
		SetWaitableTimer(hTimer, &liDueTime, 1, NULL, NULL, 0);
		glfwSwapInterval(0);
		
		while (!glfwWindowShouldClose(gamewindow)) {
			
			runGame(gamewindow, gameFlag);

			glFlush();
			WaitForSingleObject(hTimer, 12);

			framesTillCheck--;
			if (framesTillCheck == 0) {
				endOf = GetTickCount64();
				float fps = 60000.0 / (float)(endOf - startOf);
				startOf = endOf;

				printf("FPS: %f\n", fps);
				framesTillCheck = 60;
			}

			glfwSwapBuffers(gamewindow);
			glfwPollEvents();
			gameFlag = getsetGamestate(DONT_STATE_CHANGE);
		}
	} else if (mode->refreshRate != 60 && easySwap == 0) {
		HANDLE hTimer = NULL;
		LARGE_INTEGER liDueTime;
		liDueTime.QuadPart = -16666666;
		hTimer = CreateWaitableTimer(NULL, TRUE, NULL);
		glfwSwapInterval(0);

		while (!glfwWindowShouldClose(gamewindow)) {
			SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0);
			runGame(gamewindow, gameFlag);

			glFlush();
			WaitForSingleObject(hTimer, 10);

			glfwSwapBuffers(gamewindow);
			glfwPollEvents();
			gameFlag = getsetGamestate(DONT_STATE_CHANGE);
		}
	} else {
		while (!glfwWindowShouldClose(gamewindow)) {

			runGame(gamewindow, gameFlag);
			glfwSwapBuffers(gamewindow);
			glfwPollEvents();

			framesTillCheck--;
			if (framesTillCheck == 0) {
				endOf = GetTickCount64();
				float fps = 60000.0 / (float)(endOf - startOf);
				startOf = endOf;

				//printf("FPS: %f\n", fps);
				framesTillCheck = 60;
			}

			gameFlag = getsetGamestate(DONT_STATE_CHANGE);
			//counter += 1;
		}
	}*/

	

end:;
	debugfile = fopen("debugfile.txt", "w");
	fputs("\n\nPast the main game loop, closing momentarily\n\n", debugfile);
	fclose(debugfile);
	glfwTerminate();
	
	return 0;
}




/*
void moveCam(World* ourWorld) {
	while (1) {
		char input = getche();
		
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
		}
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






left front circle
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