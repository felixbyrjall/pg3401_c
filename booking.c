#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./include/oppgave3.h"

//Adds a table to the BOOKING_LIST
void addTable(struct BOOKING_LIST *sBookingList, int iTableId, char* cCustomerName, int iSeats, int iTime) {
	struct TABLE *sNewTable = malloc(sizeof(struct TABLE));
	if(sNewTable == NULL) {
		perror("Error: ");
		return;
	}
	
	//Create a TABLE with the provided data
	sNewTable->iTableId = iTableId;
	sNewTable->iSeats = iSeats;
	sNewTable->iTime = iTime;
	strncpy(sNewTable->cCustomerName, cCustomerName, sizeof(sNewTable->cCustomerName) - 1);
	sNewTable->cCustomerName[sizeof(sNewTable->cCustomerName) - 1] = '\0';
	sNewTable->pOrder = NULL;
	sNewTable->pNext = NULL;
	sNewTable->pPrev = NULL;
	
	struct TABLE *sCurrentTable = sBookingList->pHead;
	struct TABLE *sPreviousTable = NULL;
	
	//Finds the correct position to insert the new table for the table list to be sorted by name
	while(sCurrentTable != NULL && strcmp(sCurrentTable->cCustomerName, cCustomerName) < 0) {
		sPreviousTable = sCurrentTable;
		sCurrentTable = sCurrentTable->pNext;
	}
	
	if(sPreviousTable == NULL) {
		sNewTable->pNext = sBookingList->pHead;
		if(sBookingList->pHead != NULL) {
			sBookingList->pHead->pPrev = sNewTable;
		}
		sBookingList->pHead = sNewTable;
		if(sBookingList->pTail == NULL) {
			sBookingList->pTail = sNewTable;
		}
	} else {
		sNewTable->pNext = sCurrentTable;
		sNewTable->pPrev = sPreviousTable;
		sPreviousTable->pNext = sNewTable;
		if(sCurrentTable != NULL) {
			sCurrentTable->pPrev = sNewTable;
		} else {
			sBookingList->pTail = sNewTable;
		}
	}
}

//Returns a TABLE from table number search
struct TABLE *getTableInfoById(struct BOOKING_LIST *sBookingList, char *cInput) {
	struct TABLE *sCurrentTable = NULL;
	
	if(sBookingList->pHead == NULL) {
		return NULL;
	} else {
		sCurrentTable = sBookingList->pHead;
	}
	while(sCurrentTable != NULL) {
		char tableIdStr[MAX_INT_LEN];
			
		//Converts a int to string to make it comparable
		snprintf(tableIdStr, sizeof(tableIdStr), "%d", sCurrentTable->iTableId);
		if(strcmp(tableIdStr, cInput) == 0) {
			return sCurrentTable;
		}
	sCurrentTable = sCurrentTable->pNext;
	}
	return NULL;
}

//Deletes a TABLE and all attached ORDERS, and frees all allocated memory, returns true if successful
bool deleteByTableNumber(struct BOOKING_LIST *sBookingList, char *cInput) {
	struct TABLE *sCurrentTable = sBookingList->pHead;
	struct TABLE *sPreviousTable = NULL;
	
	while(sCurrentTable != NULL) {
		char tableIdStr[MAX_INT_LEN];
		
		//Converts a int to string
        snprintf(tableIdStr, sizeof(tableIdStr), "%d", sCurrentTable->iTableId);
        
		if(strcmp(tableIdStr, cInput) == 0) {
			//Free all orders
			struct ORDER *sCurrentOrder = sCurrentTable->pOrder;
			while(sCurrentOrder != NULL) {
				struct ORDER *sTemp = sCurrentOrder;
				sCurrentOrder = sCurrentOrder->pNext;
				free(sTemp);
			}
			
			//If the current table is the only one
			if(sCurrentTable == sBookingList->pTail && sCurrentTable == sBookingList->pHead) {
				sBookingList->pTail = NULL;
				sBookingList->pHead = NULL;
				free(sCurrentTable);
				return true;
			
			//If sCurrentTable is the head
			} else if(sCurrentTable == sBookingList->pHead) {
				sBookingList->pHead = sCurrentTable->pNext;
				if(sBookingList->pHead) {
					sBookingList->pHead->pPrev = NULL;
				}
				free(sCurrentTable);
				return true;
			
			//If sCurrentTable is the tail
			} else if(sCurrentTable == sBookingList->pTail) {
				sBookingList->pTail = sCurrentTable->pPrev;
				if(sBookingList->pTail) {
					sBookingList->pTail->pNext = NULL;
				}
				free(sCurrentTable);
				return true;
			} 
			
			if(sCurrentTable->pNext) {
				sCurrentTable->pNext->pPrev = sCurrentTable->pPrev;
			}
			
			if(sCurrentTable->pPrev) {
				sCurrentTable->pPrev->pNext = sCurrentTable->pNext;
			}
			free(sCurrentTable);
			return true;
		}
		sCurrentTable = sCurrentTable->pNext;
	}
	return false;
}

//Adds an ORDER to a TABLE and returns true if successful
bool addOrderToTable(struct TABLE *sCurrentTable, char *cCustomerName, char *cFoodName, int iFoodPrice) {
	struct ORDER *sNewOrder = malloc(sizeof(struct ORDER));
	if(sNewOrder == NULL) {
		perror("Error ");
		return false;
	}

	strncpy(sNewOrder->cCustomerName, cCustomerName, sizeof(sNewOrder->cCustomerName) - 1);
	sNewOrder->cCustomerName[sizeof(sNewOrder->cCustomerName) - 1] = '\0';
	strncpy(sNewOrder->cFoodName, cFoodName, sizeof(sNewOrder->cFoodName) - 1);
	sNewOrder->cFoodName[sizeof(sNewOrder->cFoodName) - 1] = '\0';
	
	sNewOrder->iFoodPrice = iFoodPrice;
	sNewOrder->pNext = sCurrentTable->pOrder;
	sCurrentTable->pOrder = sNewOrder;
	
	return true;
}

//Returns a TABLE by name search
struct TABLE *getTableInfoByName(struct BOOKING_LIST *sBookingList, char *cCustomerName) {
	struct TABLE *sCurrentTable = sBookingList->pHead;

	while(sCurrentTable != NULL) {
		if(strcmp(sCurrentTable->cCustomerName, cCustomerName) == 0) {
		return sCurrentTable;
		}
		sCurrentTable = sCurrentTable->pNext;
	}
	return NULL;
}
