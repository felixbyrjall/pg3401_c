#ifndef __OPPGAVE2_H__
#define __OPPGAVE2_H__

#include <stdbool.h>

//Definition of constants used throughout the program
#define MAX_WORD_LENGTH 20
#define BUFFER_SIZE 1014
#define MAX_WORDS 10

//Declaration of methods in sourcefiles
bool isLowerCase(const char* str);
bool isUpperCase(const char* str);
bool isPalindrome(const char* word);
bool isHeterogram(const char* word);
bool isAnagram(const char* word1, const char* word2);
bool areDisjoint(const char* word1, const char* word2);

//Declaration and definition
struct TASK2_WORD_METADATA {
	int iIndex; //The index of the word, first = 1
	bool bIsPalindrom;
	bool bIsHeterogram;
	bool bIsUppercase;
	bool bIsLowercase;
	bool bIsAnagram;
	bool bIsDisjoint;
	int iSize; //The length of the word
	char szWord[MAX_WORD_LENGTH]; //The word written "after" the struct
};
#endif
