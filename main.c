#define _CRT_SECURE_NO_WARNINGS
#include "GGE2_1.h"


#include <time.h>
#include <stdio.h>
#include <math.h>


World testworld;
World lineworld;
void moveCam(World* ourWorld);

//int WinMain(HINSTANCE hInstance,	HINSTANCE hPrevInstance,	LPSTR lpCmdLine,	int nShowCmd) {
int main(){
	printf("Hi\n");

	GLFWwindow* window = startup();

	//testworld = createWorld(STANDARD_WORLD);	

	lineworld = createWorld(VECTOR_WORLD);
	lineworld.vecColour[0] = 0;
	lineworld.vecColour[1] = 1;
	lineworld.vecColour[2] = 0;
	lineworld.vecColour[3] = 1;

	float vertices[] = {
		-0.25f, 0.25f, -0.25f, 0.6f, 0.0f, 0.0f,
		0.25f, 0.25f, -0.25f, 0.0f, 0.6f, 0.0f,
		-0.25f, 0.25f, 0.25f, 0.6f, 0.6f, 0.6f,
		0.25f, 0.25f, 0.25f, 0.0f, 0.0f, 0.6,
		0.0f, 0.5f, 0.0f, 0.1f, 0.1f, 0.1f
	};
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
		-0.4, -0.04, -0.50,				//11
		0.4, -0.04, -0.50,				//12

		//back frame
		-0.08, 0.0, -0.50,				//13
		0.08, 0.0, -0.50,				//14
		-0.06, 0.1, -0.30,				//15//back of the windshield
		0.06, 0.1, -0.30,				//16
		-0.08, 0.0, -0.38,				//17
		0.08, 0.0, -0.38,				//18
		-0.08, -0.08, -0.50,			//19//back middle
		0.08, -0.08, -0.50,			//20
	};

	unsigned int indstwo[] = { 
		0,1, 0,2, 2,3, 3,1, 0,4, 1,5, 4,2, 5,3,//windshield
		0,6, 6,1, 6,7, 6,8, 7,8, 0,7, 1,8, 7,4, 8,5,//front frame and attaching to windshield
		7,9, 9,4, 8,10, 10,5, 9,11, 10,12, 7,11, 8,12,//wings
		13,4, 14,5, 13,11, 14,12, 15,2, 16,3, 15,17, 16,18, 15,16, 13,14,//mix of wing, back cockpit, back frame
		7,19, 8,20, 19,20, 11,19, 12,20, //bottom frame and its connections
	};
	unsigned int indices[] = { 0,1,2, 1,2,3, 0,1,4, 1,3,4, 2,3,4, 0,2,4 };

	Object vectorobj = createStaticVectorObject(vertices_two, indstwo, (sizeof(vertices_two)/VECTOR_VERTEX_LENGTH)/sizeof(float), sizeof(indstwo)/sizeof(unsigned int));
	vectorobj.position[X_pos] = 0;
	vectorobj.position[Y_pos] = 0;
	vectorobj.position[Z_pos] = -0.6;
	vectorobj.position[W_pos] = 1;
	vectorobj.position[I_pos] = 0;
	vectorobj.position[J_pos] = 0;
	vectorobj.position[K_pos] = 0;
	normalizeQuat(&vectorobj.position[W_pos]);

	insertObjectIntoWorld(&lineworld, &vectorobj, 1);

	//_beginthread(moveCam, 0, &testworld);
	_beginthread(moveCam, 0, &lineworld);
	
	/*Object ourObject = createStaticStandardObject(vertices, indices, 6, 18);
	ourObject.position[X_pos] = 0;
	ourObject.position[Y_pos] = 0;
	ourObject.position[Z_pos] = -4;
	ourObject.position[W_pos] = 0;
	ourObject.position[I_pos] = 0;
	ourObject.position[J_pos] = 1;
	ourObject.position[K_pos] = 0;
	normalizeQuat(&ourObject.position[W_pos]);

	insertObjectIntoWorld(&testworld, &ourObject, 1);*/

	//glLineWidth(50);

	float counter = 1;
	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//glLineWidth(2);

		//drawWorld(&testworld);
		drawWorld(&lineworld);
		//runMasterUI();

		float rotateQuat[4] = { cosf(0.0018726646 / 2), 0 , 1 * sinf(0.0018726646 / 2), 0 };
		float rotQuatConj[4] = { rotateQuat[0], 0, -rotateQuat[2], 0 };
		normalizeQuat(&rotateQuat);
		normalizeQuat(&rotQuatConj);
		//quatMult(rotateQuat, &vectorobj.position[W_pos]);

		
			
			

		glfwSwapBuffers(window);
		glfwPollEvents();
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
			ourWorld->camera[X_pos] -= ourWorld->back[1] * (.02 + (posMag/10));
			ourWorld->camera[Y_pos] -= ourWorld->back[2] * (.02 + (posMag / 10));
			ourWorld->camera[Z_pos] -= ourWorld->back[3] * (.02 + (posMag / 10));
			printf("\n%f, %f, %f\n", ourWorld->camera[X_pos], ourWorld->camera[Y_pos], ourWorld->camera[Z_pos]);
		}
		if (input == 's') {
			ourWorld->camera[X_pos] += ourWorld->back[1] * (.02 + (posMag / 10));
			ourWorld->camera[Y_pos] += ourWorld->back[2] * (.02 + (posMag / 10));
			ourWorld->camera[Z_pos] += ourWorld->back[3] * (.02 + (posMag / 10));
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