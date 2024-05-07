#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 30
#define MAX_LINE_LEN 128
#define MAX_LINES 100

void spacesToTabs(char *cpLine);

int main(int iArgC, char *cArgV[]) {
	FILE *fpFileInput = NULL;
	FILE *fpFileOutput = NULL;
	char *pInputFilename = cArgV[1];
	char cOutputFilename[MAX_STR_LEN + 12]; //_beautified and null terminator
	char cLine[MAX_LINE_LEN];
	char cModifiedLine[MAX_LINE_LEN];
	int iInputLen = strlen(cArgV[1]);
	int iLineCount = 0;
	
	if(iArgC != 2) {
		printf("./oppgave6 <filename.c>");
		return 1;
	}
	
	//Checks if the input filename is longer than 30 characters long, including the extention
	if(iInputLen > MAX_STR_LEN) {
		printf("The filename of the input file is too long. Max length is 30\n");
		return 1;
	}
	
	//Checks if the input file ends with a .c extention
	if(!(iInputLen > 2 && pInputFilename[iInputLen - 2] == '.' && pInputFilename[iInputLen - 1] == 'c')) {
		printf("Input file needs to end with .c\n");
		return 1;
	}
	
	//
	snprintf(cOutputFilename, sizeof(cOutputFilename), "%.*s_beautified.c", iInputLen - 2, pInputFilename);
	
	fpFileInput = fopen(pInputFilename, "r");
	if(fpFileInput == NULL) {
		perror("Error");
		return 1;
	}
	
	char **cppLines = malloc(MAX_LINES * sizeof(char *));
	if(cppLines == NULL) {
		perror("Error");
		fclose(fpFileInput);
		return 1;
	}
	
	while(fgets(cLine, sizeof(cLine), fpFileInput) && iLineCount < MAX_LINES) {
		cppLines[iLineCount] = strdup(cLine);
		if(cppLines[iLineCount] == NULL) {
			perror("Error");
			for(int i = 0; i < iLineCount; i++) {
				free(cppLines[i]);
			}
			free(cppLines);
			fclose(fpFileInput);
			return 1;
		}
		//changeForToWhile()
		//forceHungarianNotation()
		spacesToTabs(cppLines[iLineCount]);
		iLineCount++;
	}
	fclose(fpFileInput);
	
	fpFileOutput = fopen(cOutputFilename, "w");
	if(fpFileOutput == NULL) {
		perror("Error");
		for(int i = 0; i < iLineCount; i++) {
			free(cppLines[i]);
		}
		free(cppLines);
		return 1;
	}
	
	for(int i = 0; i < iLineCount; i++) {
		fprintf(fpFileOutput, "%s", cppLines[i]);
		free(cppLines[i]);
	}
	
	free(cppLines);
	fclose(fpFileOutput);
	
	printf("File beautified in %s\n", cOutputFilename);
	return 0;
}

void spacesToTabs(char *cpLine) {
	int iSpaceCounter = 0;
	char *cLineReader = cpLine;
	char *cLineWriter = cpLine;
	char cTab = 9; //ASCII code 0x09 for tab
	
	for(char *cLineReader = cpLine; *cLineReader != '\0'; cLineReader++) {
		if(*cLineReader == ' ') {
			iSpaceCounter ++;
			if(iSpaceCounter == 3) {
				*cLineWriter = cTab;
				cLineWriter ++;
				iSpaceCounter = 0;
			}
		} else{
			while(iSpaceCounter > 0) {
				*cLineWriter = ' ';
				cLineWriter ++;
				iSpaceCounter --;
			}
			*cLineWriter = *cLineReader;
			cLineWriter ++;
		}
	}
	
	//If there aren't enough spaces to get converted into a tab (1 or 2), it writes a space and decrements the space counter
	while(iSpaceCounter > 0) {
		*cLineWriter = ' ';
		cLineWriter ++;
		iSpaceCounter --;
	}
	*cLineWriter = '\0';
}
