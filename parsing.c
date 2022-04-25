#include "parsing.h"
#include <string.h>
#include <stdio.h>
#include <mysql.h>


void parseAccessKeys(char* fileName) {
	MYSQL* conn;
	MYSQL_RES* res;
	MYSQL_ROW row;
	char server[] = "localhost";
	char user[] = "root";
	char password[] = "LizIronheart";
	char database[] = "main_project";

	conn = mysql_init(NULL);
	if (!mysql_real_connect(conn, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(conn));
		exit(1);
	}


	FILE* ourCSV;
	ourCSV = fopen(fileName, "r");
							

	//char insertStart[] = "INSERT INTO Access_Key VALUES(";
	int segment = 0;
	int result = 1;
	char temp[512];
	fgets(temp, 255, ourCSV);//cut off the top
	while (result != NULL) {				//0					1				2				3				4					5				6						7				8			9
		char curLine[512] = { 0 };//AccessKey, SurveyId, CompanyID, UserID, Permissions, BillingPlan, Company Name, Region, UserLevel, Name
		char accessKey[512] = { "INSERT INTO Access_Key (AccessKey, SurveyID, CompanyID, UserID, Permissions) VALUES(" };
		//char accessKeyFinisher[256] = { "ON DUPLICATE KEY UPDATE " };
		char company[512] = { "INSERT INTO Company (CompanyID, BillingPlan, CompanyName, Region) VALUES(" };
		char companyFinisher[256] = { "ON DUPLICATE KEY UPDATE " };
		char foreignUser[512] = { "INSERT INTO Foreign_User (CompanyID, UserID, UserLevel, Name) VALUES(" };
		char foreignUserFinisher[256] = { "ON DUPLICATE KEY UPDATE " };


		result = fgets(curLine, 510, ourCSV);
		if (result == NULL) {
			break;
		}

		char* newlineReplace = strchr(curLine, '\n');
		if (newlineReplace != NULL) {
			*newlineReplace = '\0';
		}

		char delim[] = ",";
		char* token = strtok(curLine, delim);
		int companyAllowed = 1;
		int userAllowed = 1;
		//AccessKey, non-nullable		
		strcat(accessKey, "\"");
		strcat(accessKey, token);
		strcat(accessKey, "\"");
		strcat(accessKey, ", ");
		token = strtok(NULL, delim);

		//SurveyID
		strcat(accessKey, token);
		strcat(accessKey, ", ");
		token = strtok(NULL, delim);

		//CompanyID
		if (strcmp(token, "NULL") == 0) {
			companyAllowed = 0;
		}
		strcat(accessKey, token);
		strcat(accessKey, ", ");

		strcat(company, token);
		strcat(company, ", ");

		strcat(companyFinisher, " CompanyID = ");
		strcat(companyFinisher, token);
		strcat(companyFinisher, ", ");

		strcat(foreignUser, token);
		strcat(foreignUser, ", ");

		strcat(foreignUserFinisher, " CompanyID = ");
		strcat(foreignUserFinisher, token);
		strcat(foreignUserFinisher, ",");
		token = strtok(NULL, delim);

		//UserID
		if (strcmp(token, "NULL") == 0) {
			userAllowed = 0;
		}
		strcat(accessKey, token);
		strcat(accessKey, ", ");

		strcat(foreignUser, token);
		strcat(foreignUser, ", ");

		strcat(foreignUserFinisher, " UserID = ");
		strcat(foreignUserFinisher, token);
		strcat(foreignUserFinisher, ",");

		token = strtok(NULL, delim);

		//Permissions, non-nullable
		strcat(accessKey, "\"");
		strcat(accessKey, token);
		strcat(accessKey, "\"");
		strcat(accessKey, ")");//final
		token = strtok(NULL, delim);

		//BillingPlan, non-nullable
		strcat(company, "\"");
		strcat(company, token);
		strcat(company, "\"");
		strcat(company, ", ");

		strcat(companyFinisher, " BillingPlan = \"");
		strcat(companyFinisher, token);
		strcat(companyFinisher, "\", ");

		token = strtok(NULL, delim);

		//company name, non-nullable
		strcat(company, "\"");
		strcat(company, token);
		strcat(company, "\"");
		strcat(company, ", ");

		strcat(companyFinisher, " CompanyName = \"");
		strcat(companyFinisher, token);
		strcat(companyFinisher, "\", ");
		token = strtok(NULL, delim);

		//Region, non-nullable
		strcat(company, "\"");
		strcat(company, token);
		strcat(company, "\")");//final

		strcat(companyFinisher, " Region = \"");
		strcat(companyFinisher, token);
		strcat(companyFinisher, "\"");
		token = strtok(NULL, delim);

		//Userlevel Non-nullable
		strcat(foreignUser, "\"");
		strcat(foreignUser, token);
		strcat(foreignUser, "\",");

		strcat(foreignUserFinisher, " UserLevel = \"");
		strcat(foreignUserFinisher, token);
		strcat(foreignUserFinisher, "\", ");
		token = strtok(NULL, delim);

		//Name, nullable
		if (strcmp(token, "NULL") != 0) {
			strcat(foreignUser, "\"");
			strcat(foreignUser, token);
			strcat(foreignUser, "\")");//final

			strcat(foreignUserFinisher, " Name = \"");
			strcat(foreignUserFinisher, token);
			strcat(foreignUserFinisher, "\"");
		} else {
			strcat(foreignUser, "NULL)");//final

			strcat(foreignUserFinisher, " Name = NULL");
		}
		
		strcat(foreignUser, foreignUserFinisher);
		strcat(company, companyFinisher);
		//TODO - Put the SQL call in here to update the database
		


		//DEBUG
		printf("\n\n%s\n", accessKey);
		if (companyAllowed) {
			if (mysql_query(conn, company)) {
				printf("%s\n", mysql_error(conn));
			}
			printf("%s\n", company);
		}
		if (userAllowed) {
			if (mysql_query(conn, foreignUser)) {
				printf("%s\n", mysql_error(conn));
			}
			printf("%s\n", foreignUser);
		}
		if (mysql_query(conn, accessKey)) {
			printf("%s\n", mysql_error(conn));
		}
	}
	

}


