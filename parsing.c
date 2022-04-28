#include "parsing.h"
#include <string.h>
#include <stdio.h>



void connectToDatabase() {
	char server[] = "localhost";
	char user[] = "root";
	char password[] = "LizIronheart";
	char database[] = "main_project";

	mysqlConnection = mysql_init(NULL);
	if (!mysql_real_connect(mysqlConnection, server, user, password, database, 0, NULL, 0)) {
		fprintf(stderr, "%s\n", mysql_error(mysqlConnection));
		exit(1);
	}
}

void parseAccessKeys(char* fileName) {
	MYSQL_RES* res;
	MYSQL_ROW row;

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
			if (mysql_query(mysqlConnection, company)) {
				printf("%s\n", mysql_error(mysqlConnection));
			}
			printf("%s\n", company);
		}
		if (userAllowed) {
			if (mysql_query(mysqlConnection, foreignUser)) {
				printf("%s\n", mysql_error(mysqlConnection));
			}
			printf("%s\n", foreignUser);
		}
		if (mysql_query(mysqlConnection, accessKey)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}
	}
	
	fclose(ourCSV);
}




void accessKeysReportCreation(UI* PageToInsertTo) {
	
	mysql_query(mysqlConnection, "SELECT company.CompanyName, company.CompanyID, company.CompanyNo, company.Region, company.BillingPlan, access_key.AccessKey, access_key.SurveyID, access_key.Permissions, foreign_user.userID, foreign_user.Name, foreign_user.UserLevel FROM access_key LEFT JOIN company ON access_key.CompanyID = company.CompanyID LEFT JOIN foreign_user ON access_key.UserID = foreign_user.UserID ORDER BY CompanyID DESC;");
	MYSQL_RES* datablock = mysql_store_result(mysqlConnection);
	MYSQL_ROW row;

	float rgb[] = { 0,0,0 };
	float zeropos[] = { 0,0,0 };
	//these will be manipulated around to create multiple rectangles as we go
	float vertsDark[] = {
		-1 + convFromPixelX(50), convFromPixelY(120), 0.79,			189.0 / 255, 115.0 / 255, 58.0 / 255,
		1 - convFromPixelX(100), convFromPixelY(120), 0.79,			189.0 / 255, 115.0 / 255, 58.0 / 255,
		-1 + convFromPixelX(50), convFromPixelY(0), 0.79,			189.0 / 255, 115.0 / 255, 58.0 / 255,
		1 - convFromPixelX(100), convFromPixelY(0), 0.79,			189.0 / 255, 115.0 / 255, 58.0 / 255,
	};
	float vertsLight[] = {
		-1 + convFromPixelX(100), 0, 0.79,					211.0 / 255, 207.0 / 255, 193.0 / 255,
		convFromPixelX(((windX/2) - 200)), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
		-1 + convFromPixelX(100), 0, 0.79,					211.0 / 255, 207.0 / 255, 193.0 / 255,
		convFromPixelX(((windX / 2) - 200)), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
	};
	unsigned int inds[] = {
		0,1,2, 1,2,3
	};


	UnfinObj displayObject = createUnFinTextWithZ("Companies", -1 + convFromPixelX(80), convFromPixelY(70), 0.7, 64, rgb);

	
	float curOffset = 0;
	float width = convFromPixelX((windX - 100));
	float xPosLeftmost = -1 + convFromPixelX(80);
	float xPosRightoffset = -1 + convFromPixelX(120);
	float zpos = 0.6;
	int titlefontsize = 32;
	int datafontsize = 24;
	float titleOffset = -convFromPixelY(40);
	float dataOffset = -convFromPixelY(32);

	char* curID = calloc(10, sizeof(char));
	
	if (curID == NULL) {
		exit(1);
	}
	curID[0] = '-';
	curID[1] = '1';
	//companies loop
	int firstTime = 0;
	while (firstTime < 6) {
		row = mysql_fetch_row(datablock);
		if (row == NULL) {
			break;
		}

		//MAIN ENTRY
		if (strcmp(curID, row[1]) != 0 && strcmp("NULL", row[1]) != 0) {
			strcpy(curID, row[1]);//save our current id

			if (firstTime != 0) {
				vertsLight[2 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
				vertsLight[3 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
				curOffset += (dataOffset);
			}
			UnfinObj curThing = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//set up our title bar
			curThing = createUnFinTextWithZ("Name          CID  CNO  Region    Billing", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += titleOffset;	

			
			//now print out the company's specific information
			curThing = createUnFinTextWithZ(row[0], xPosLeftmost, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//CID
			curThing = createUnFinTextWithZ(row[1], xPosLeftmost + (11 * convFromPixelX(titlefontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//CNO
			curThing = createUnFinTextWithZ(row[2], xPosLeftmost + (14 * convFromPixelX(titlefontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//Region
			curThing = createUnFinTextWithZ(row[3], xPosLeftmost + (18 * convFromPixelX(titlefontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//Billing
			curThing = createUnFinTextWithZ(row[4], xPosLeftmost + (24 * convFromPixelX(titlefontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += titleOffset;


			vertsLight[1] = curOffset + convFromPixelY((titlefontsize/2));
			vertsLight[1 + VERTEX_LENGTH] = curOffset + convFromPixelY((titlefontsize / 2));
			curThing = createUnFinTextWithZ("Key   SurveyID   Perms   UID   Name     User Level", xPosRightoffset, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += titleOffset;


			
		}

		//SUB ENTRY
		//if we're 'within our group', as it were, print out all the other information for this line
		if (strcmp(curID, row[1]) == 0) {
			//key
			UnfinObj curThing = createUnFinTextWithZ(row[5], xPosRightoffset, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//survey id
			if (row[6] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (7 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[6], xPosRightoffset + (7 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//perms
			curThing = createUnFinTextWithZ(row[7], xPosRightoffset + (12 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//user id
			if (row[8] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (18 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[8], xPosRightoffset + (18 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//username
			if (row[9] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (22 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[9], xPosRightoffset + (22 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//user level
			if (row[10] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (30 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[10], xPosRightoffset + (30 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}
		}

		curOffset += dataOffset;	
		firstTime++;
	}

	//last 'highlight' rectangle
	vertsLight[2 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
	vertsLight[3 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
	UnfinObj finishingHighlight = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);


	vertsDark[2 * VERTEX_LENGTH + 1] = curOffset;
	vertsDark[3 * VERTEX_LENGTH + 1] = curOffset;
	finishingHighlight = createUnfinObjFromStatic(vertsDark, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);

	
	mysql_free_result(datablock);
	UIElement* finalElement = createElement(displayObject.verts, displayObject.indices, displayObject.vLineCount, displayObject.iCount, zeropos, NULL, NULL, NO_ACTION, 1, NULL);
	insertElementIntoUI(PageToInsertTo, finalElement);
}


