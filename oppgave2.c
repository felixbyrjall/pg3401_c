#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "./include/oppgave2.h"

void changeMetadata(struct TASK2_WORD_METADATA *meta, char* token, int iIndex);

int main(void) {
	//Declaring and defining variables
	int iIndex = 0;
	char buffer[BUFFER_SIZE];
	char aszWords[MAX_WORDS][MAX_WORD_LENGTH];
	struct TASK2_WORD_METADATA *meta = NULL;
	struct TASK2_WORD_METADATA aStructs[MAX_WORDS];
	
	//Clears the contents of the initialized array aszWords[]
	memset(aszWords, 0, sizeof(aszWords));
	memset(aStructs, 0, sizeof(aStructs));
	
	//Error handling if file reading goes wrong
	FILE *fpText = fopen("eksamen_v24_oppgave2.txt", "r");
	if(fpText == NULL) {
		perror("Error: ");
		return 1;
	}
	
	//Error handling if opening output file goes wrong
	FILE *fpBinary = fopen("output.bin", "wb");
	if(fpBinary != NULL) {

		//Reads the strings from the file
		//Removes the \n that gets read, and tokenizes each string
		//Each token gets put into the array aszWords
		while (fgets(buffer, BUFFER_SIZE, fpText) != NULL && iIndex < MAX_WORDS) {
			char strDelim[] = "\r\n";
			char *strToken = strtok(buffer, strDelim);
	
			if(strToken != NULL) {
				strncpy(aszWords[iIndex], strToken, sizeof(aszWords[0] - 1));
				aszWords[iIndex][MAX_WORD_LENGTH - 1] = '\0';
				strToken = strtok(NULL, strDelim);
				iIndex++;
			}
		}
		fclose(fpText);
		
		//Allocates memory to the struct
		meta = (struct TASK2_WORD_METADATA *)malloc(sizeof(struct TASK2_WORD_METADATA));
		if(meta == NULL) {
			return 1;
		}
		
		int length = sizeof(aszWords)/sizeof(aszWords[0]);
		
		for(int i = 0; i < length; i++) {
			
			//Changes the some of the words metadata
			changeMetadata(meta, aszWords[i], i + 1);
			
			//Sets the default values
			meta->bIsAnagram = false;
			meta->bIsDisjoint = true;
			
			//Goes through each word, comparing it to all the other words
			for(int j = 0; j < length; j++) {
				if(i == j)continue;
				//If one word is an anagram of another, it sets metadata to true
				if(isAnagram(aszWords[i], aszWords[j])) {
					meta->bIsAnagram = true;
				}
				//Disjoint is set to true by default, and set to false if two words aren't disjoint
				//The disjoint function is case insensitive, which means JUMP isn't disjoint, otherwise JUMP would be disjoint
				if(!areDisjoint(aszWords[i], aszWords[j])) {
					meta->bIsDisjoint = false;
				}
			}
			//Sets [i] element in the array to the current metadata and word
			aStructs[i] = *meta;
		}
		//Writes to the file from the array of structs and frees the memory allocated to the struct
		fwrite(&aStructs, sizeof(aStructs), 1, fpBinary);
		free(meta);
		
	} else {
		perror("Error ");
		return 1;
	}
	
	//Closes the output file
	fclose(fpBinary);
}

//Function for changing some of the metadata for cleaner code
void changeMetadata(struct TASK2_WORD_METADATA *meta, char* word, int i) {
	memset(meta, 0, sizeof(struct TASK2_WORD_METADATA));
	
	meta->iIndex = i;
	
	meta->bIsPalindrom = isPalindrome(word);
	meta->bIsHeterogram = isHeterogram(word);
	meta->bIsUppercase = isUpperCase(word);
	meta->bIsLowercase = isLowerCase(word);
	
	//Sets iSize to the length of the word passed in as a parameter, and copies the word into szWord[]
	meta->iSize = strlen(word);
	strncpy(meta->szWord, word, MAX_WORD_LENGTH - 1);
	meta->szWord[MAX_WORD_LENGTH - 1] = '\0';
}
