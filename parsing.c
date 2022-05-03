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
	while (result != NULL) {				//0					1				2				3				4					5				6						7				8			9			10	
		char curLine[512] = { 0 };//AccessKey, SurveyId, CompanyID, UserID, Permissions, BillingPlan, Company Name, Region, UserLevel, FName		Linit
		char accessKey[512] = { "INSERT INTO Access_Key (AccessKey, SurveyID, CompanyID, UserID, Permissions) VALUES(" };
		//char accessKeyFinisher[256] = { "ON DUPLICATE KEY UPDATE " };
		char company[512] = { "INSERT INTO Company (CompanyID, BillingPlan, CompanyName, Region) VALUES(" };
		char companyFinisher[256] = { "ON DUPLICATE KEY UPDATE " };
		char foreignUser[512] = { "INSERT INTO Foreign_User (CompanyID, UserID, UserLevel, FName, LInit) VALUES(" };
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

		//Userlevel Non-nullable
		strcat(foreignUser, "\"");
		strcat(foreignUser, token);
		strcat(foreignUser, "\",");

		strcat(foreignUserFinisher, " UserLevel = \"");
		strcat(foreignUserFinisher, token);
		strcat(foreignUserFinisher, "\", ");
		token = strtok(NULL, delim);

		//Region, non-nullable
		strcat(company, "\"");
		strcat(company, token);
		strcat(company, "\")");//final

		strcat(companyFinisher, " Region = \"");
		strcat(companyFinisher, token);
		strcat(companyFinisher, "\"");
		token = strtok(NULL, delim);

		//FName, nullable
		if (strcmp(token, "NULL") != 0) {
			strcat(foreignUser, "\"");
			strcat(foreignUser, token);
			strcat(foreignUser, "\",");

			strcat(foreignUserFinisher, " FName = \"");
			strcat(foreignUserFinisher, token);
			strcat(foreignUserFinisher, "\",");
		} else {
			strcat(foreignUser, "NULL, ");

			strcat(foreignUserFinisher, " FName = NULL, ");
		}
		token = strtok(NULL, delim);

		//LInit, nullable, last name entry
		if (strcmp(token, "NULL") != 0) {
			strcat(foreignUser, "\"");
			strcat(foreignUser, token);
			strcat(foreignUser, "\")");//final

			strcat(foreignUserFinisher, " LInit = \"");
			strcat(foreignUserFinisher, token);
			strcat(foreignUserFinisher, "\"");
		} else {
			strcat(foreignUser, "NULL)");//final

			strcat(foreignUserFinisher, " LInit = NULL");
		}


		
		strcat(foreignUser, foreignUserFinisher);
		strcat(company, companyFinisher);

		//DEBUG PRINTFS
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

void parseSurveyData(char* fileName) {
	MYSQL_RES* res;
	MYSQL_ROW row;

	FILE* ourCSV;
	ourCSV = fopen(fileName, "r");


	//char insertStart[] = "INSERT INTO Access_Key VALUES(";
	int segment = 0;
	int result = 1;
	char temp[512];
	fgets(temp, 255, ourCSV);//cut off the top
	while (result != NULL) {		//1					2						3					4					5					6					7				8						9						10						11						12					13						14
		char curLine[512] = { 0 };//SurveyID	CompanyID	UserID	VisualAssetData	QuestionNo	Language	QuestionType	FormatData	PossibleAnswer	CharacterSet	FlagImage	Description	ExperimentalFlag		Active
		char survey[1024] = { "INSERT INTO survey (SurveyID, CompanyID, UserID, VisualAssetData, `Language`, Active) VALUES(" };
		char surveyFinisher[256] = { "ON DUPLICATE KEY UPDATE " };

		char language[1024] = { "INSERT INTO `language` (`Language`, Character_Set, Flag_Image, `Description`, Experimental) VALUES(" };
		char languageFinisher[256] = { "ON DUPLICATE KEY UPDATE " };

		char questionOptions[1024] = { "INSERT INTO question_options (SurveyID, QuestionNo, QuestionType, PossibleAnswer) VALUES(" };
		char questionOptionsFinisher[256] = { "ON DUPLICATE KEY UPDATE " };

		char questionTypes[1024] = { "INSERT INTO question_types (QuestionType, FormatData) VALUES(" };
		char questionTypesFinisher[256] = { "ON DUPLICATE KEY UPDATE " };


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

		//SurveyID, non nullable int on both
		strcat(survey, token);
		strcat(survey, ",");

		strcat(surveyFinisher, "SurveyID = ");
		strcat(surveyFinisher, token);
		strcat(surveyFinisher, ", ");

		strcat(questionOptions, token);
		strcat(questionOptions, ",");

		strcat(questionOptionsFinisher, "SurveyID = ");
		strcat(questionOptionsFinisher, token);
		strcat(questionOptionsFinisher, ", ");

		//CompanyID, nullable int
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(survey, token);
			strcat(survey, ", ");

			strcat(surveyFinisher, " CompanyID = ");
			strcat(surveyFinisher, token);
			strcat(surveyFinisher, ", ");
		} else {
			strcat(survey, "NULL, ");

			strcat(surveyFinisher, " CompanyID = NULL,");
		}

		//UserID, nullable int
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(survey, token);
			strcat(survey, ", ");

			strcat(surveyFinisher, " UserID = ");
			strcat(surveyFinisher, token);
			strcat(surveyFinisher, ", ");
		} else {
			strcat(survey, "NULL, ");

			strcat(surveyFinisher, " UserID = NULL, ");
		}

		//VisualAssetData, nullable
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(survey, "\"");
			strcat(survey, token);
			strcat(survey, "\",");

			strcat(surveyFinisher, " VisualAssetData = \"");
			strcat(surveyFinisher, token);
			strcat(surveyFinisher, "\",");
		} else {
			strcat(survey, "NULL, ");
			strcat(surveyFinisher, " VisualAssetData = NULL, ");
		}

		//QuestionNo, non nullable int
		token = strtok(NULL, delim);
		strcat(questionOptions, token);
		strcat(questionOptions, ",");

		strcat(questionOptionsFinisher, " QuestionNo = ");
		strcat(questionOptionsFinisher, token);
		strcat(questionOptionsFinisher, ",");

		//Language, nullable
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(survey, "\"");
			strcat(survey, token);
			strcat(survey, "\",");

			strcat(surveyFinisher, " `Language` = \"");
			strcat(surveyFinisher, token);
			strcat(surveyFinisher, "\", ");

			strcat(language, "\"");
			strcat(language, token);
			strcat(language, "\",");

			strcat(languageFinisher, " `Language` = \"");
			strcat(languageFinisher, token);
			strcat(languageFinisher, "\", ");
		} else {
			strcat(survey, "NULL, ");
			strcat(surveyFinisher, " `Language` = NULL, ");

			strcat(language, "NULL, ");
			strcat(languageFinisher, " `Language` = NULL, ");
		}

		//QuestionType, non-nullable
		token = strtok(NULL, delim);
		strcat(questionOptions, "\"");
		strcat(questionOptions, token);
		strcat(questionOptions, "\",");

		strcat(questionOptionsFinisher, " QuestionType = \"");
		strcat(questionOptionsFinisher, token);
		strcat(questionOptionsFinisher, "\",");

		strcat(questionTypes, "\"");
		strcat(questionTypes, token);
		strcat(questionTypes, "\",");

		strcat(questionTypesFinisher, " QuestionType = \"");
		strcat(questionTypesFinisher, token);
		strcat(questionTypesFinisher, "\",");

		//FormatData, non-nullable, final for question types
		token = strtok(NULL, delim);
		strcat(questionTypes, "\"");
		strcat(questionTypes, token);
		strcat(questionTypes, "\") ");

		strcat(questionTypesFinisher, " FormatData = \"");
		strcat(questionTypesFinisher, token);
		strcat(questionTypesFinisher, "\";");

		//PossibleAnswer, nullable, final for question options
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(questionOptions, "\"");
			strcat(questionOptions, token);
			strcat(questionOptions, "\")");//final

			strcat(questionOptionsFinisher, " PossibleAnswer = \"");
			strcat(questionOptionsFinisher, token);
			strcat(questionOptionsFinisher, "\";");
		} else {
			strcat(questionOptions, "NULL)");//final

			strcat(questionOptionsFinisher, " PossibleAnswer = NULL;");
		}

		//Characterset, non-nullable
		token = strtok(NULL, delim);
		strcat(language, "\"");
		strcat(language, token);
		strcat(language, "\",");

		strcat(languageFinisher, " Character_Set = \"");
		strcat(languageFinisher, token);
		strcat(languageFinisher, "\",");


		//Flag_Image, non-nullable
		token = strtok(NULL, delim);
		strcat(language, "\"");
		strcat(language, token);
		strcat(language, "\",");

		strcat(languageFinisher, " Flag_Image = \"");
		strcat(languageFinisher, token);
		strcat(languageFinisher, "\",");

		//Description, non-nullable
		token = strtok(NULL, delim);
		strcat(language, "\"");
		strcat(language, token);
		strcat(language, "\",");

		strcat(languageFinisher, " `Description` = \"");
		strcat(languageFinisher, token);
		strcat(languageFinisher, "\",");

		//Experimental Flag, non-nullable int last of Language
		token = strtok(NULL, delim);
		strcat(language, "");
		strcat(language, token);
		strcat(language, ") ");

		strcat(languageFinisher, " Experimental = ");
		strcat(languageFinisher, token);
		strcat(languageFinisher, ";");//final

		//Active, non-nullable final for survey
		token = strtok(NULL, delim);
		strcat(survey, "");
		strcat(survey, token);
		strcat(survey, ") ");//final

		strcat(surveyFinisher, " Active = ");
		strcat(surveyFinisher, token);
		strcat(surveyFinisher, ";");//final



		//adding the 'if we need to update' part to the mains
		strcat(language , languageFinisher);
		strcat(questionOptions, questionOptionsFinisher);
		strcat(survey, surveyFinisher);
		strcat(questionTypes, questionTypesFinisher);

		////PRINTFS ARE DEBUG
		printf("\n\n%s\n", language);
		if (mysql_query(mysqlConnection, language)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}

		printf("%s\n", survey);
		if (mysql_query(mysqlConnection, survey)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}	

		printf("%s\n", questionTypes);
		if (mysql_query(mysqlConnection, questionTypes)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}

		printf("%s\n", questionOptions);
		if (mysql_query(mysqlConnection, questionOptions)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}
		
	}

	fclose(ourCSV);
}

void parseResponseData(char* fileName) {
	MYSQL_RES* res;
	MYSQL_ROW row;

	FILE* ourCSV;
	ourCSV = fopen(fileName, "r");


	int segment = 0;
	int result = 1;
	char temp[512];
	fgets(temp, 255, ourCSV);//cut off the top	
	while (result != NULL) {		//1						2						3					4				5					6									7					8					9						10						11					12
		char curLine[512] = { 0 };//SurveyeeID	HowReached		SurveyID		Region		Account	AllowedToContact	DisplayName		Language		QuestionNo		Question		QuestionType		Answer
		char surveyee[1024] = { "INSERT INTO surveyee (SurveyeeID, HowReached, Region, Account, AllowedToContact, DisplayName) VALUES(" };
		char surveyeeF[256] = { "ON DUPLICATE KEY UPDATE " };

		char sQ[1024] = { "INSERT INTO survey_questions (SurveyID, `Language`, QuestionNo, Question, QuestionType) VALUES(" };
		char sQF[256] = { "ON DUPLICATE KEY UPDATE " };

		char sR[1024] = { "INSERT INTO survey_responses (SurveyeeID, SurveyID,QuestionNo, Answer) VALUES(" };
		char sRF[256] = { "ON DUPLICATE KEY UPDATE " };


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

		
		//surveyeeID, non nullable int
		strcat(surveyee, token);
		strcat(surveyee, ",");
		strcat(surveyeeF, "surveyeeID = ");
		strcat(surveyeeF, token);
		strcat(surveyeeF, ", ");

		strcat(sR, token);
		strcat(sR, ",");
		strcat(sRF, "surveyeeID = ");
		strcat(sRF, token);
		strcat(sRF, ", ");
		
		
		//HowReached, nullable
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(surveyee, "\"");
			strcat(surveyee, token);
			strcat(surveyee, "\",");
			strcat(surveyeeF, "HowReached = \"");
			strcat(surveyeeF, token);
			strcat(surveyeeF, "\", ");
		} else {
			strcat(surveyee, "NULL");
			strcat(surveyeeF, "HowReached = NULL,");
		}
		

		//SurveyID, non nullable int on all 3
		token = strtok(NULL, delim);

		strcat(sQ, token);
		strcat(sQ, ",");
		strcat(sQF, "SurveyID = ");
		strcat(sQF, token);
		strcat(sQF, ", ");

		strcat(sR, token);
		strcat(sR, ",");
		strcat(sRF, "SurveyID = ");
		strcat(sRF, token);
		strcat(sRF, ", ");

		//Region, non nullable
		token = strtok(NULL, delim);
		strcat(surveyee, "\"");
		strcat(surveyee, token);
		strcat(surveyee, "\",");
		strcat(surveyeeF, "Region = \"");
		strcat(surveyeeF, token);
		strcat(surveyeeF, "\", ");

		///Account, nullable
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(surveyee, "\"");
			strcat(surveyee, token);
			strcat(surveyee, "\",");
			strcat(surveyeeF, "Account = \"");
			strcat(surveyeeF, token);
			strcat(surveyeeF, "\", ");
		} else {
			strcat(surveyee, "NULL, ");
			strcat(surveyeeF, "Account = NULL,");
		}

		//Allowed To contact,  nullable int
		token = strtok(NULL, delim);
		strcat(surveyee, token);
		strcat(surveyee, ",");
		strcat(surveyeeF, "AllowedToContact = ");
		strcat(surveyeeF, token);
		strcat(surveyeeF, ", ");

		///Display name, nullable, final entry for surveyee
		token = strtok(NULL, delim);
		if (strcmp(token, "NULL") != 0) {
			strcat(surveyee, "\"");
			strcat(surveyee, token);
			strcat(surveyee, "\")");

			strcat(surveyeeF, "DisplayName = \"");
			strcat(surveyeeF, token);
			strcat(surveyeeF, "\";");
		} else {
			strcat(surveyee, "NULL)");
			strcat(surveyeeF, "DisplayName = NULL;");
		}


		//Language, non nullable
		token = strtok(NULL, delim);
		strcat(sQ, "\"");
		strcat(sQ, token);
		strcat(sQ, "\",");
		strcat(sQF, "`Language` = \"");
		strcat(sQF, token);
		strcat(sQF, "\", ");

		//Question Number, non nullable int
		token = strtok(NULL, delim);
		strcat(sQ, token);
		strcat(sQ, ",");
		strcat(sQF, "QuestionNo = ");
		strcat(sQF, token);
		strcat(sQF, ", ");

		strcat(sR, token);
		strcat(sR, ",");
		strcat(sRF, "QuestionNo = ");
		strcat(sRF, token);
		strcat(sRF, ", ");

		//Question, non nullable
		token = strtok(NULL, delim);
		strcat(sQ, "\"");
		strcat(sQ, token);
		strcat(sQ, "\",");
		strcat(sQF, "Question = \"");
		strcat(sQF, token);
		strcat(sQF, "\", ");

		//Question Type, non nullable, final for survey questions
		token = strtok(NULL, delim);
		strcat(sQ, "\"");
		strcat(sQ, token);
		strcat(sQ, "\") ");
		strcat(sQF, "QuestionType = \"");
		strcat(sQF, token);
		strcat(sQF, "\";");

		//Answer, non nullable, final for resposnes
		token = strtok(NULL, delim);
		strcat(sR, "\"");
		strcat(sR, token);
		strcat(sR, "\") ");
		strcat(sRF, "Answer = \"");
		strcat(sRF, token);
		strcat(sRF, "\";");


		strcat(sQ, sQF);
		strcat(sR, sRF);
		strcat(surveyee, surveyeeF);

		////PRINTFS ARE DEBUG
		printf("\n\n%s\n", sQ);
		if (mysql_query(mysqlConnection, sQ)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}

		printf("%s\n", surveyee);
		if (mysql_query(mysqlConnection, surveyee)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}

		printf("%s\n", sR);
		if (mysql_query(mysqlConnection, sR)) {
			printf("%s\n", mysql_error(mysqlConnection));
		}

	}

	fclose(ourCSV);
}

void accessKeysReportCreation(UI* PageToInsertTo) {
	
	mysql_query(mysqlConnection, "SELECT company.CompanyName, company.CompanyID, company.CompanyNo, company.Region, company.BillingPlan, access_key.AccessKey, access_key.SurveyID, access_key.Permissions, foreign_user.userID, foreign_user.FName, foreign_user.LInit, foreign_user.UserLevel FROM access_key LEFT JOIN company ON access_key.CompanyID = company.CompanyID LEFT JOIN foreign_user ON access_key.UserID = foreign_user.UserID ORDER BY CompanyID DESC, foreign_user.FName;");
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
	while (1) {
		row = mysql_fetch_row(datablock);
		if (row == NULL || row[0] == NULL) {//If we have no entries or if there are no more *company* entries
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
			curThing = createUnFinTextWithZ("Name           CID  CNO  Region   Billing", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
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
			curThing = createUnFinTextWithZ("Key   SurveyID Perms  UID  FName  LInit User Level", xPosRightoffset, curOffset, zpos, datafontsize, rgb);
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
			curThing = createUnFinTextWithZ(row[7], xPosRightoffset + (11 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//user id
			if (row[8] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (15 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[8], xPosRightoffset + (16 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//Fname
			if (row[9] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (19 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[9], xPosRightoffset + (19 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//Linit
			if (row[10] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (25 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[10], xPosRightoffset + (25 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//user level
			if (row[11] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (30 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[11], xPosRightoffset + (30 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
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


	//ROGUE USER FORMATTING
	firstTime = 0;//reset these
	curID[0] = '-';
	curID[1] = '1';

	//add in the big text
	curOffset += titleOffset;
	UnfinObj usertitle = createUnFinTextWithZ("Rogue Users", -1 + convFromPixelX(80), curOffset, 0.7, 64, rgb);
	displayObject = mergeUnfinishedsFreeing(displayObject, usertitle);
	curOffset += dataOffset + dataOffset;

	if (row != NULL) {//this is a check to see if we ONLY had companies, no rogue users.
		while (1) {//Loop through data
			if (firstTime != 0) {
				row = mysql_fetch_row(datablock);
			}

			if (row == NULL) {//If we have no entries left to read, break
				break;
			}


			//MAIN ENTRY
			if (strcmp(curID, row[8]) != 0 && strcmp("NULL", row[8]) != 0) {
				strcpy(curID, row[8]);//save our current id

				if (firstTime != 0) {
					vertsLight[2 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
					vertsLight[3 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
					curOffset += (dataOffset);
				}

				UnfinObj curThing = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

				//set up our title bar
				curThing = createUnFinTextWithZ("First Name  LInit  UserID", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				curOffset += titleOffset;

				//Displaying user specific data

				//Fname
				curThing = createUnFinTextWithZ(row[9], xPosLeftmost, curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

				//Linit
				if (row[10] == NULL) {
					curThing = createUnFinTextWithZ("NULL", xPosRightoffset + (10 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				} else {
					curThing = createUnFinTextWithZ(row[10], xPosRightoffset + (10 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				}

				//userID
				curThing = createUnFinTextWithZ(row[8], xPosLeftmost + (19 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				curOffset += titleOffset;


				vertsLight[1] = curOffset + convFromPixelY((titlefontsize / 2));
				vertsLight[1 + VERTEX_LENGTH] = curOffset + convFromPixelY((titlefontsize / 2));
				curThing = createUnFinTextWithZ("Key   SurveyID   Permissions", xPosRightoffset, curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				curOffset += titleOffset;
			}


			//key
			UnfinObj curThing = createUnFinTextWithZ(row[5], xPosRightoffset, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			//survey id
			if (row[6] == NULL) {
				curThing = createUnFinTextWithZ("NULL", xPosRightoffset, curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			} else {
				curThing = createUnFinTextWithZ(row[6], xPosRightoffset + (7 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			}

			//Permissions
			curThing = createUnFinTextWithZ(row[7], xPosRightoffset + (12 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			curOffset += dataOffset;


			firstTime++;
		}

	}
	//last 'highlight' rectangle
	vertsLight[2 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
	vertsLight[3 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((titlefontsize / 2));
	finishingHighlight = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);





	//complete the big dark rectangle
	vertsDark[2 * VERTEX_LENGTH + 1] = curOffset;
	vertsDark[3 * VERTEX_LENGTH + 1] = curOffset;
	finishingHighlight = createUnfinObjFromStatic(vertsDark, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);

	mysql_free_result(datablock);
	UIElement* finalElement = createElement(displayObject.verts, displayObject.indices, displayObject.vLineCount, displayObject.iCount, zeropos, NULL, NULL, NO_ACTION, 1, NULL);
	insertElementIntoUI(PageToInsertTo, finalElement);
	freeUnfinObj(displayObject);
}


void backendReportCreation(UI* PageToInsertTo) {
	mysql_query(mysqlConnection, "select sv.SurveyID, sv.CompanyID, sv.UserID, sv.`Language`, qo.QuestionNo, qo.QuestionType, qo.PossibleAnswer, sv.VisualAssetData, qt.FormatData, ln.`Description`, ln.Experimental, ln.Character_Set, ln.Flag_Image from question_options as qo 	LEFT JOIN Question_Types as qt on qo.QuestionType = qt.QuestionType	LEFT JOIN survey as sv on qo.SurveyID = sv.SurveyID LEFT JOIN `language` as ln on sv.`Language` = ln.`Language` ORDER BY surveyID, QuestionNo;;");
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
		convFromPixelX(((windX / 2) - 200)), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
		-1 + convFromPixelX(100), 0, 0.79,					211.0 / 255, 207.0 / 255, 193.0 / 255,
		convFromPixelX(((windX / 2) - 200)), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
	};
	unsigned int inds[] = {
		0,1,2, 1,2,3
	};


	UnfinObj displayObject = createUnFinTextWithZ("Survey Backend Data", -1 + convFromPixelX(80), convFromPixelY(70), 0.7, 64, rgb);


	float curOffset = 0;
	float width = convFromPixelX((windX - 100));
	float xPosLeftmost = -1 + convFromPixelX(80);
	float xPosMiddle = -1 + convFromPixelX(120);
	float xPosRightmost = -1 + convFromPixelX(160);
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
	
	//How it works:
	//effectively loop through each unique survey ID, for each one do a sublooping for each possible answer and display it, then close off with printing the rest of the data

	row = mysql_fetch_row(datablock);
	while (1) {
		char ts[256] = "";
		if (row == NULL) {//If we have no entries, end it
			break;
		}

		strcpy(curID, row[0]);//save our current id

		//set up our title bar, 'Survey: #\n ~title bar stuffs~'
		UnfinObj curThing = createUnFinTextWithZ(strcat(strcat(ts,"Survey: "), row[0]), xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += titleOffset;

		curThing = createUnFinTextWithZ("CompID  UserID   Language", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += titleOffset;

		//CompanyID, nullable
		if (row[1] != NULL) {
			curThing = createUnFinTextWithZ(row[1], xPosLeftmost + (2 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("NULL", xPosLeftmost + (2 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}		

		//UserID, nullable
		if (row[2] != NULL) {
			curThing = createUnFinTextWithZ(row[2], xPosLeftmost + (9 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("NULL", xPosLeftmost + (9 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}		

		//Language, formally nullable but shouldnt have to...
		if (row[3] != NULL) {
			curThing = createUnFinTextWithZ(row[3], xPosLeftmost + (18 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("NULL", xPosLeftmost + (18 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}
		
		curOffset += dataOffset;


		//save all the 'finisher information' we need to
		char visda[256] = { 0 };
		strcpy(visda, row[7]);
		char formda[256] = { 0 };
		strcpy(formda, row[8]);
		char desc[256] = { 0 };
		strcpy(desc, row[9]);
		char experimental[3] = { 0 };
		experimental[0] = row[10][0];
		char charset[256] = { 0 };
		strcpy(charset, row[11]);
		char flagimg[256] = { 0 };
		strcpy(flagimg, row[12]);

		//set the lighter rectangle top
		vertsLight[0 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));
		vertsLight[1 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));

		//Loop through all questions within this survey
		while (1) {
			//display our q number and type, then set up the 'title' of our next loop
			curOffset += dataOffset/2;//give ourselves a lil headroom
			ts[0] = '\0';
			curThing = createUnFinTextWithZ(strcat(strcat(ts, "Question Number: "), row[4]), xPosMiddle, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

			ts[0] = '\0';
			curThing = createUnFinTextWithZ(strcat(strcat(ts, "Question Type: "), row[5]), xPosMiddle + (18 * convFromPixelX(datafontsize)), curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += titleOffset;

			curThing = createUnFinTextWithZ("Possible Answers", xPosRightmost, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += dataOffset;

			////Now loop through all the individual possible answers for this question.
			//Save our current question so we know when to break out of this loop.
			char curQuestion[20] = { 0 };
			strcpy(curQuestion, row[4]);
			while (1) {
				if (row[6] != NULL) {
					curThing = createUnFinTextWithZ(row[6], xPosRightmost, curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				} else {
					curThing = createUnFinTextWithZ("T-F/Free Answer/Other", xPosRightmost, curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				}
				curOffset += dataOffset;

				//fetch the next entry
				row = mysql_fetch_row(datablock);

				//if we're out of rows or onto the next question/survey break out of the loop.
				if (row == NULL || strcmp(curQuestion, row[4]) != 0 || strcmp(curID, row[0]) != 0) {
					break;
				}
			}//End of possible answers loop

			//check to see if we're onto another survey now, if so we need to wrap up this one's presentation and that requires breaking this loop,
			//as we are currently looping through all the questions in the now-previous survey.
			if (row == NULL || strcmp(curID, row[0]) != 0) {
				//set the lighter rectangle bottom
				vertsLight[2 * VERTEX_LENGTH + 1] = curOffset;
				vertsLight[3 * VERTEX_LENGTH + 1] = curOffset;
				UnfinObj highlight = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
				displayObject = mergeUnfinishedsFreeing(displayObject, highlight);

				break;
			}
		}//End of questions loop
		
		//here it is safe to assume 'row' is onto another survey and just need to do cleanup on the previous using the stuff we saved.
		curOffset += dataOffset / 2;//give ourselves a lil headroom
		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts, "Language Desc: "), desc), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts, "Is Language Experimental: "), experimental), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts, "Lang Charset: "), charset), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts, "Lang Flag: "), flagimg), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts,"Visual Assets: "), visda), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		ts[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(ts, "Format Data: "), formda), xPosLeftmost, curOffset, zpos, datafontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += dataOffset;

		curOffset += dataOffset;//give ourselves more headroom later lol
		if (row == NULL) {//out of entries
			break;
		}
	}//end of survey loop


	//complete the big dark rectangle
	vertsDark[2 * VERTEX_LENGTH + 1] = curOffset;
	vertsDark[3 * VERTEX_LENGTH + 1] = curOffset;
	UnfinObj finishingHighlight = createUnfinObjFromStatic(vertsDark, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);
	mysql_free_result(datablock);
	UIElement* finalElement = createElement(displayObject.verts, displayObject.indices, displayObject.vLineCount, displayObject.iCount, zeropos, NULL, NULL, NO_ACTION, 1, NULL);
	insertElementIntoUI(PageToInsertTo, finalElement);
	freeUnfinObj(displayObject);
}

void responsesReportCreation(UI* PageToInsertTo) {
	mysql_query(mysqlConnection, "select s.SurveyeeId, s.DisplayName, s.Account, s.Region, s.AllowedToContact, s.HowReached, sr.SurveyID, sq.`Language`, sq.QuestionNo, sq.QuestionType, sq.Question, sr.Answer from surveyee as s JOIN survey_responses as sr ON s.SurveyeeID = sr.SurveyeeID JOIN survey_questions as sq ON sr.SurveyID = sq.SurveyID AND sr.QuestionNo = sq.QuestionNo ORDER BY s.SurveyeeID, sr.SurveyID, sr.QuestionNo;");
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
		1 - convFromPixelX(150), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
		-1 + convFromPixelX(100), 0, 0.79,					211.0 / 255, 207.0 / 255, 193.0 / 255,
		1 - convFromPixelX(150), 0, 0.79,			211.0 / 255, 207.0 / 255, 193.0 / 255,
	};
	unsigned int inds[] = {
		0,1,2, 1,2,3
	};


	UnfinObj displayObject = createUnFinTextWithZ("Surveyees", -1 + convFromPixelX(80), convFromPixelY(70), 0.7, 64, rgb);


	float curOffset = 0;
	float width = convFromPixelX((windX - 100));
	float xPosLeftmost = -1 + convFromPixelX(80);
	float xPosMiddle = -1 + convFromPixelX(120);
	float xPosRightmost = -1 + convFromPixelX(160);
	float zpos = 0.6;
	int titlefontsize = 32;
	int datafontsize = 24;
	float titleOffset = -convFromPixelY(40);
	float dataOffset = -convFromPixelY(32);

	char* surveeNo = calloc(10, sizeof(char));

	if (surveeNo == NULL) {
		exit(1);
	}
	surveeNo[0] = '-';
	surveeNo[1] = '1';

	//How it works:
	//Outer loop for all unique surveyees,
	//middle loop for all their surveys they have answered, 
	//Inner loop for every question within said survey,
	//deep inner loop for getting all the answers to the question, fuck you multiselect

	row = mysql_fetch_row(datablock);
	char tempstr[256] = { 0 };
	while (1) {//loop through surveyees
		if (row == NULL) {//If we have no entries, end it
			break;
		}

		strcpy(surveeNo, row[0]);

		//Print out our current surveyeee's information
		tempstr[0] = '\0';
		UnfinObj curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Surveyee No "), row[0]), xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);

		tempstr[0] = '\0';
		if (row[1] != NULL) {
			curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Username: "), row[1]), xPosLeftmost + (15 * convFromPixelX(titlefontsize)), curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("Username: No Name", xPosLeftmost + (15 * convFromPixelX(titlefontsize)), curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}
		
		curOffset += titleOffset;

		tempstr[0] = '\0';
		if (row[2] != NULL) {
			curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Account: "), row[2]), xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("Account: No Account", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}

		tempstr[0] = '\0';
		if (row[3] != NULL) {
			curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Region: "), row[3]), xPosLeftmost + (24 * convFromPixelX(titlefontsize)), curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("Region: NULL", xPosLeftmost + (24 * convFromPixelX(titlefontsize)), curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}
		
		curOffset += titleOffset;

		tempstr[0] = '\0';
		if (row[4] != NULL) {
			curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Allowed To Contact: "), row[4]), xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		} else {
			curThing = createUnFinTextWithZ("Allowed To Contact: NA", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		}
		curOffset += titleOffset;

		tempstr[0] = '\0';
		curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "How Reached: "), row[5]), xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += titleOffset;

		curThing = createUnFinTextWithZ("Responses", xPosLeftmost, curOffset, zpos, titlefontsize, rgb);
		displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
		curOffset += titleOffset;

		//set the lighter rectangle top
		vertsLight[0 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));
		vertsLight[1 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));
		//then give a lil headroom
		curOffset += dataOffset/2;

		while (1) {//start looping through all surveys
			tempstr[0] = '\0';
			curThing = createUnFinTextWithZ(strcat(strcat(strcat(strcat(tempstr, "SurveyID: "), row[6]), "    Lan: "), row[7]), xPosMiddle, curOffset, zpos, datafontsize, rgb);
			displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
			curOffset += titleOffset;

			char survNo[20] = "";
			strcpy(survNo, row[6]);//save our surveyid

			
			while (1) {//loop through all questions of this survey
				tempstr[0] = '\0';
				curThing = createUnFinTextWithZ(strcat(strcat(strcat(strcat(tempstr, "Qnum: "), row[8]), "    QType: "), row[9]), xPosRightmost, curOffset, zpos, datafontsize, rgb);
				displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
				curOffset += dataOffset;


				tempstr[0] = '\0';
				int stringlength = strlen(row[10]);
				if (stringlength > 50) {
					strncat(strcat(tempstr, "Q: "), row[10], 50);
					strcat(tempstr, "\n");
					strcat(tempstr, &row[10][50]);
					curThing = createUnFinTextWithZ(tempstr, xPosRightmost, curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
					curOffset += 2 * dataOffset;
				} else {
					curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "Q: "), row[10]), xPosRightmost, curOffset, zpos, datafontsize, rgb);
					displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
					curOffset += dataOffset;
				}
				

				char qNo[20] = "";
				strcpy(qNo, row[8]);//Save our current question number
				while (1) {//Loop through all answers to the given question
					tempstr[0] = '\0';
					stringlength = strlen(row[11]);
					if (stringlength > 50) {
						strncat(strcat(tempstr, "A: "), row[11], 50);
						strcat(tempstr, "\n");
						strcat(tempstr, &row[11][50]);
						curThing = createUnFinTextWithZ(tempstr, xPosRightmost, curOffset, zpos, datafontsize, rgb);
						displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
						curOffset += 2 * dataOffset;
					} else {
						curThing = createUnFinTextWithZ(strcat(strcat(tempstr, "A: "), row[11]), xPosRightmost, curOffset, zpos, datafontsize, rgb);
						displayObject = mergeUnfinishedsFreeing(displayObject, curThing);
						curOffset += dataOffset;
					}
					
					//otherwise, get the next row for us to print here
					row = mysql_fetch_row(datablock);
					//If we have moved on to a new question, survey, or surveyee we are done here looping through the answers. Also will end on the final row
					if (row == NULL || strcmp(qNo, row[8]) != 0 || strcmp(survNo, row[6]) != 0 || strcmp(surveeNo, row[0]) != 0) {
						break;
					}
				}

				//If we have moved on to a new survey or surveyee we are done here in looping through the questions.
				if (row == NULL || strcmp(survNo, row[6]) != 0 || strcmp(surveeNo, row[0]) != 0) {
					break;
				}
			}//End of looping through all questions

			//set the lighter rectangle bottom
			vertsLight[2 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));
			vertsLight[3 * VERTEX_LENGTH + 1] = curOffset + convFromPixelY((datafontsize / 2));
			UnfinObj highlight = createUnfinObjFromStatic(vertsLight, inds, 4, 6);
			displayObject = mergeUnfinishedsFreeing(displayObject, highlight);

			//give a lil headroom before the next entry
			curOffset += dataOffset/2;

			//If we have moved on to another user, then we are done looping through this one's survey data
			if (row == NULL || strcmp(surveeNo, row[0]) != 0) {
				break;
			}
		}//End of surveys taken by this surveyee

	}//End of surveyees



	//complete the big dark rectangle
	vertsDark[2 * VERTEX_LENGTH + 1] = curOffset;
	vertsDark[3 * VERTEX_LENGTH + 1] = curOffset;
	UnfinObj finishingHighlight = createUnfinObjFromStatic(vertsDark, inds, 4, 6);
	displayObject = mergeUnfinishedsFreeing(displayObject, finishingHighlight);
	mysql_free_result(datablock);
	UIElement* finalElement = createElement(displayObject.verts, displayObject.indices, displayObject.vLineCount, displayObject.iCount, zeropos, NULL, NULL, NO_ACTION, 1, NULL);
	insertElementIntoUI(PageToInsertTo, finalElement);
	freeUnfinObj(displayObject);
}