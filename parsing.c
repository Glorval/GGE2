#include "parsing.h"


void parseAccessKeys(char* fileName) {
	FILE* ourCSV;
	ourCSV = fopen(fileName, "r");
							

	//char insertStart[] = "INSERT INTO Access_Key VALUES(";
	int segment = 0;
	int result = 1;
	char temp[512];
	fgets(temp, 255, ourCSV);//cut off the top
	while (result != NULL) {				//0					1				2				3				4					5				6						7				8			9
		char curLine[512] = { 0 };//AccessKey, SurveyId, CompanyID, UserID, Permissions, BillingPlan, Company Name, Region, UserLevel, Name
		char accessKey[256] = { "INSERT INTO Access_Key (AccessKey, SurveyID, CompanyID, UserID, Permissions) VALUES(" };
		char company[256] = { "INSERT INTO Company (CompanyID, Billing_Plan, Company_Name, Region) VALUES(" };
		char foreignUser[256] = { "INSERT INTO Foreign_User (CompanyID, UserID, UserLevel, Name) VALUES(" };

		result = fgets(curLine, 255, ourCSV);
		if (result == NULL) {
			break;
		}

		char* newlineReplace = strchr(curLine, '\n');
		if (newlineReplace != NULL) {
			*newlineReplace = '\0';
		}
		
		char delim[] = ",";
		char* token = strtok(curLine, delim);

		//AccessKey
		strcat(accessKey, token);
		strcat(accessKey, ", ");
		token = strtok(NULL, delim);

		//SurveyID
		strcat(accessKey, token);
		strcat(accessKey, ", ");
		token = strtok(NULL, delim);

		//CompanyID
		strcat(accessKey, token);
		strcat(accessKey, ", ");

		strcat(company, token);
		strcat(company, ", ");

		strcat(foreignUser, token);
		strcat(foreignUser, ", ");
		token = strtok(NULL, delim);

		//UserID
		strcat(accessKey, token);
		strcat(accessKey, ", ");

		strcat(foreignUser, token);
		strcat(foreignUser, ", ");
		token = strtok(NULL, delim);

		//Permissions
		strcat(accessKey, token);
		strcat(accessKey, ")");//final
		token = strtok(NULL, delim);

		//Billing Plan
		strcat(company, token);
		strcat(company, ", ");
		token = strtok(NULL, delim);

		//company name
		strcat(company, token);
		strcat(company, ", ");
		token = strtok(NULL, delim);

		//Region
		strcat(company, token);
		strcat(company, ")");//final
		token = strtok(NULL, delim);

		//Userlevel
		strcat(foreignUser, token);
		strcat(foreignUser, ", ");
		token = strtok(NULL, delim);

		//Name
		strcat(foreignUser, token);
		strcat(foreignUser, ")");//final


		//TODO - Put the SQL call in here to update the database

		//DEBUG
		printf("\n\n%s\n%s\n%s", company, foreignUser, accessKey);
	}
	
	


}


