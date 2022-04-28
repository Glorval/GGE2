#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <mysql.h>
#include "GGE2.h"

volatile MYSQL* mysqlConnection;


void connectToDatabase();

void parseAccessKeys(char* fileName);
void accessKeysReportCreation(UI* PageToInsertTo);