#pragma once
#define _CRT_SECURE_NO_WARNINGS

char* fileSelector();//this is because windows.h is being an utter pain


#include <windows.h>// <- fuc u
//#include <mysql.h>
#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

#include "GGE2.h"
#include "GlorwynUtilities.h"
#include "DataBase.h"


World* ourWorld;
World* ourUI;
void testmysql();

int main() {
	printf("Hi\n");
	//	testmysql();
	

	GLFWwindow* window = startup();


	World uis = createWorld();
	startupDataBase(window);

	
	float counter = 1;
	while (1) {
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		//drawWorld(&world);//We dont need the world, just the UI right now
		runMasterUI();
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	char t = 'c';
	scanf("%c", &t);

	glfwTerminate();
	return 0;
}


char* fileSelector() {
	int ret = 1;
	wchar_t path[256] = { 0 };
	OPENFILENAMEW ofn = {
		.lStructSize = sizeof(ofn),
		.lpstrFile = path,
		.nMaxFile = sizeof(path),
	};

	if (GetOpenFileNameW(&ofn)) {
		DWORD n = lstrlenW(path);
		//HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
		//ret = !WriteConsoleW(h, path, n, &n, 0);
	}

	char* charPath = calloc(256, sizeof(char));
	for (int c = 0; c < 256; c++) {
		charPath[c] = path[c];
	}
	return(charPath);
}








/*
void testmysql() {
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;

	char server[] = "localhost";
	char user[] = "root";
	char password[] = "LizIronheart";
	char database[] = "Testing";

	conn = mysql_init(NULL);

	
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	if (mysql_query(conn, "show tables")) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}

	res = mysql_use_result(conn);




	printf("MySQL Tables in mysql database:\n");

	while ((row = mysql_fetch_row(res)) != NULL)
		printf("%s \n", row[0]);

	mysql_free_result(res);

	if (mysql_query(conn, "insert into main_table values (5)")) {
		printf("%s\n", mysql_error(conn));
		exit(1);
	}

	mysql_close(conn);

}*/
