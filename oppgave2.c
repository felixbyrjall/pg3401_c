#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "./include/oppgave2.h"

#define BUFFER_SIZE 1014

int main(void) {
	FILE *fpText = NULL;
	FILE *fpBinary = NULL;
	char buffer[BUFFER_SIZE];
	struct TASK2_WORD_METADATA *meta = NULL;
	
	fpText = fopen("eksamen_v24_oppgave2.txt", "r");
	if(fpText != NULL) {
		while (fgets(buffer, BUFFER_SIZE, fpText) != NULL) {
			
			int id = 1;
			id++;
			char delim[] = "\r\n";
			char *token = strtok(buffer, delim);
			
			meta = (struct TASK2_WORD_METADATA *)malloc(sizeof(struct TASK2_WORD_METADATA));
			if(meta == NULL) return 1;
			
			printf("%d", id);
			
			while (token != NULL) {
				printf("\n%s is a", token);
				if(isUpperCase(token)) {
					printf(" lower");
				}
				if(isLowerCase(token)) {
					printf(" upper");
				}
				if(isPalindrome(token)) {
					printf(" palindrome");
				}
				if(isHeterogram(token)) {
					printf(" heterogram");
				}
				token = strtok(NULL, delim);
			}
			
			free(meta);
		}
		fclose(fpText);
	} else {
		perror("Error ");
		return -1;
	}
}

void changeMetadata(struct TASK2_WORD_METADATA *meta, int iIndex) {
	int i;
}
