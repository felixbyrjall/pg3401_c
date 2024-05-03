#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "./include/oppgave3.h"

void printMenu();
void addReservationMenu(struct BOOKING_LIST *sBookingList);
void getReservationInfoMenu(struct BOOKING_LIST *sBookingList);
void deleteReservationMenu(struct BOOKING_LIST *sBookingList);
void getReservationByNameMenu(struct BOOKING_LIST *sBookingList);
void addOrderToTableMenu(struct BOOKING_LIST *sBookingList);
void getOrdersForTable(struct BOOKING_LIST *sBookingList);
void getOrderForCustomerAtTable(struct BOOKING_LIST *sBookingList);

void freeAllResources(struct BOOKING_LIST *sBookingList);
void printTableInfo(struct BOOKING_LIST *sBookingList);

void clear();
int readInt(char *cInput, int iSize);
int readString(char *cInput, int iSize);
int readString(char *cInput, int iSize);
bool isNum(char *cInput);
bool checkForValidInput(char *input);

int main(void) {
	int iInput = 0;
	char cInput[BUFFER_SIZE];
	memset(cInput, 0, sizeof(cInput));
	
	struct BOOKING_LIST *sBookingList = malloc(sizeof(struct BOOKING_LIST));
	sBookingList->pHead = NULL;
	sBookingList->pTail = NULL;
	
	if(sBookingList == NULL) {
		perror("Error: ");
		return 1;
	}
	
	do {
		printMenu();
		printf("Please enter an option: ");
		iInput = readInt(cInput, sizeof(cInput));
		
		switch(iInput) {
			case 1:
				printTableInfo(sBookingList);
				addReservationMenu(sBookingList);
				break;
			case 2:
				getReservationInfoMenu(sBookingList);
				break;
			case 3:
				getReservationByNameMenu(sBookingList);
				break;
			case 4:
				deleteReservationMenu(sBookingList);
				break;
			case 5:
				addOrderToTableMenu(sBookingList);
				break;
			case 6:
				getOrdersForTable(sBookingList);
				break;
			case 7:
				getOrderForCustomerAtTable(sBookingList);
				break;
			case -1:
				printf("EXITING PROGRAM...\n");
				break;
			default:
				printf("Please enter a valid NUMBER\n");
				break;
		}
	} while(iInput != -1);
	
	//Dynamically frees memory allocated to the linked list
	freeAllResources(sBookingList);
}

//Prints out the main menu
void printMenu() {
	printf("\n");
	printf("----------MENU----------\n");
	printf(" 1. Add a reservation    \n");
	printf(" 2. Get table info  \n");
	printf(" 3. Search for reservation\n");
	printf(" 4. Delete a resevation  \n");
	printf(" 5. Add order to a table \n");
	printf(" 6. Get receipt for table\n");
	printf(" 7. Get separate bill\n");
	printf("-1. Exit\n");
	printf("------------------------\n");
	
	/* IMPROVED MENU, ADD AT THE END?
	printf("/-----------MENU-----------\\\n");
	printf("| 1. Add a reservation     |\n");
	printf("| 2. Get reservation info  |\n");
	printf("| 3. Search for resevation |\n");
	printf("| 4. Delete a resevation   |\n");
	printf("| 5. Add order to a table  |\n");
	printf("| 6. Get receipt for table |\n");
	printf("| 7. Get separate bill     |\n");
	printf("| -1. Exit                  |\n");
	printf("\\--------------------------/\n");
	*/
}

//Takes input from the user to add a new TABLE to the BOOKING_LIST
void addReservationMenu(struct BOOKING_LIST *sBookingList) {
	int iTableId = 0;
	int iSeats = 0;
	int iTime = 0;
	char cInput[MAX_INT_LEN];
	char cCustomerName[MAX_STR_LEN];
	bool bTableExists = false;
	bool bCustomerExists = false;
	memset(cInput, 0, sizeof(cInput));

	while(true) {
		printf("Enter table number (1-50): ");
		int iInput = readInt(cInput, sizeof(cInput));
		
		if(iInput < 1 || iInput > 50) {
			printf("Table numbers range from 1 - 50\n");
			continue;
		}
		
		struct TABLE *sCurrentTable = sBookingList->pHead;
		while(sCurrentTable != NULL) {
			if(iInput == sCurrentTable->iTableId) {
				printf("Table %d for %s is already booked.\n", iInput, sCurrentTable->cCustomerName);
				bTableExists = true;
				break;
			}
			sCurrentTable = sCurrentTable->pNext;
		}
		
		if(!bTableExists) {
			//Get input for customer name
			printf("Enter customer name: ");
			readString(cCustomerName, sizeof(cCustomerName));
			
			//Check if the customer already has a reservation
			sCurrentTable = sBookingList->pHead;
			while(sCurrentTable != NULL) {
				if(strncmp(sCurrentTable->cCustomerName, cCustomerName, sizeof(sCurrentTable) - 1) == 0) {
					printf("A reservation already exists for %s\n", cCustomerName);
					bCustomerExists = true;
					break;
				}
				sCurrentTable = sCurrentTable->pNext;
			}
			
			//breaks out of the outer loop if the customer exists
			if(bCustomerExists) {
				break;
			}
			
			iTableId = iInput;
			
			//Get input for amount of seats
			printf("Enter amount of seats: ");
			iInput = readInt(cInput, sizeof(cInput));
			if(!isNum(cInput)) {
				printf("Not a number\n");
				break;
			} else if(iInput < 1) {
				printf("Must be at least 1 person");
				break;
			}
			iSeats = iInput;
			
			//Get input for the hour
			printf("Enter the booking hour (16-24): ");
			int iHour = readInt(cInput, sizeof(cInput));
			if (!isNum(cInput) || iHour < 16 || iHour > 24) {
				printf("Hour must be between 16 and 24.\n");
				break;
			}
			
			//Get input for the minutes
			printf("Enter the booking minutes (0-59): ");
			int iMinutes = readInt(cInput, sizeof(cInput));
			if (!isNum(cInput) || iMinutes < 0 || iMinutes > 59) {
				printf("Minutes must be between 0 and 59.\n");
				break;
    		}
    		
    		//Combine the hours and minutes
    		iTime = iHour * 100 + iMinutes;

			printf("Table %d is now booked for %d people, at %d, for %s.\n", iTableId, iSeats, iTime, cCustomerName);
			addTable(sBookingList, iTableId, cCustomerName, iSeats, iTime);
			break;
		}
	}
}

//Gets the info from 
void getReservationInfoMenu(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_STR_LEN];
	memset(cInput, 0, sizeof(cInput));
	struct TABLE *sCurrentTable = NULL;
	
	printf("Enter table number: ");
	readString(cInput, sizeof(cInput));

	if(!isNum(cInput)) {
		printf("Not a number\n");
	}
	
	sCurrentTable = getTableInfoById(sBookingList, cInput);
	if(sCurrentTable == NULL) {
		printf("Table %s is not booked", cInput);
		return;
	}
	
	printf("-------------------\n");
	printf("Table number: %d\n", sCurrentTable->iTableId);
	printf("Customer: %s\n", sCurrentTable->cCustomerName);
	printf("Amount of seats: %d\n", sCurrentTable->iSeats);
	printf("Time: %d\n", sCurrentTable->iTime);
	
	if(sCurrentTable->pOrder != NULL) {
		struct ORDER *sCurrentOrder = sCurrentTable->pOrder;
		int i = 0;
		printf("Orders: \n");
		
		while(sCurrentOrder != NULL) {
			i++;
			printf("<><><><><><><><><>\n");
			printf("Order nr %d\n", i);
			printf("Customer: %s\n", sCurrentOrder->cCustomerName);
			printf("Food item: %s\n", sCurrentOrder->cFoodName);
			printf("Price: %5d NOK\n", sCurrentOrder->iFoodPrice);
			
			sCurrentOrder = sCurrentOrder->pNext;
		}
		printf("<><><><><><><><><>\n");
	} else{
		printf("No orders for this table\n");
	}
	printf("-------------------\n");

}

//Delete a reservation by table number
void deleteReservationMenu(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_STR_LEN];
	memset(cInput, 0, sizeof(cInput));
	printf("To remove a booking,\n");
	printf("Enter table number: ");
	
	readString(cInput, sizeof(cInput));
	if(deleteByTableNumber(sBookingList, cInput)) {
		printf("Table %s is no longer reserved", cInput);
	} else{
		printf("Invalid table number or table not found");
	}
}

//Gets a reservation by name search
void getReservationByNameMenu(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_STR_LEN];
	memset(cInput, 0, sizeof(cInput));
	struct TABLE *sCurrentTable = NULL;
	
	printf("Enter customer name: ");
	readString(cInput, sizeof(cInput));
	sCurrentTable = getTableInfoByName(sBookingList, cInput);
	
	if(sCurrentTable == NULL) {
		printf("No reservation found for %s\n", cInput);
	} else{
		printf("-------------------\n");
		printf("Table number: %d\n", sCurrentTable->iTableId);
		printf("Customer: %s\n", sCurrentTable->cCustomerName);
		printf("Amount of seats: %d\n", sCurrentTable->iSeats);
		printf("Time: %d\n", sCurrentTable->iTime);
		printf("-------------------\n");
	}
}

//Adds an order to an existing table
void addOrderToTableMenu(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_INT_LEN];
	char cCustomerName[MAX_STR_LEN];
	char cFoodName[MAX_STR_LEN];
	int iFoodPrice;
	int iTableId;
	
	printf("Enter table number for the order: ");
	iTableId = readInt(cInput, sizeof(cInput));
	
	struct TABLE *sCurrentTable = sBookingList->pHead;
	while(sCurrentTable != NULL) {
		if(iTableId == sCurrentTable->iTableId) {
			printf("Adding an order to table %d\n", iTableId);
			
			printf("Enter customer name for this order: ");
			readString(cCustomerName, sizeof(cCustomerName));
			
			printf("Enter food name: ");
			readString(cFoodName, sizeof(cFoodName));
			
			printf("Enter food price: ");
			iFoodPrice = readInt(cInput, sizeof(cInput));
			
			if(addOrderToTable(sCurrentTable, cCustomerName, cFoodName, iFoodPrice)) {
				printf("Order successfully added to table %d:\n", iTableId);
				printf("Customer: %s\n", cCustomerName);
				printf("Food: %s\n", cFoodName);
				printf("Price: %d\n", iFoodPrice);
			} else{
				printf("Failed to add order\n");
			}
			return;
		}
		sCurrentTable = sCurrentTable->pNext;
	}
	printf("Table %d not found\n", iTableId);
}

void getOrdersForTable(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_STR_LEN];
	memset(cInput, 0, sizeof(cInput));
	struct TABLE *sCurrentTable = NULL;
	int iSum = 0;
	
	printf("Enter table number for receipt: ");
	readString(cInput, sizeof(cInput));
	
	if(!isNum(cInput)) {
		printf("Not a number\n");
		return;
	}
	
	sCurrentTable = getTableInfoById(sBookingList, cInput);
	if(sCurrentTable == NULL) {
		printf("Table %s is not booked\n", cInput);
		return;
	}
	printf("RECEIPT FOR TABLE %d\n", sCurrentTable->iTableId);
	if(sCurrentTable->pOrder != NULL) {
		struct ORDER *sCurrentOrder = sCurrentTable->pOrder;
		int i = 0;
		while(sCurrentOrder != NULL) {
			i++;
			printf("%-10s - %5d NOK\n", sCurrentOrder->cFoodName, sCurrentOrder->iFoodPrice);
			iSum += sCurrentOrder->iFoodPrice;
			sCurrentOrder = sCurrentOrder->pNext;
		}
		printf("Total cost: %6d NOK\n", iSum);
	} else {
		printf("No orders for this table");
	}
}

void getOrderForCustomerAtTable(struct BOOKING_LIST *sBookingList) {
	char cInput[MAX_STR_LEN];
	char cCustomerName[MAX_STR_LEN];
	printf("Enter table number for specific customer's orders: ");
	readString(cInput, sizeof(cInput));
	
	if(!isNum(cInput)) {
		printf("Not a number\n");
		return;
	}
	
	struct TABLE *sCurrentTable = getTableInfoById(sBookingList, cInput);
	if(sCurrentTable == NULL) {
		printf("Table %s is not booked\n", cInput);
		return;
	}
	
	printf("Enter customer name for orders: ");
	readString(cCustomerName, sizeof(cCustomerName));
	
	printf("Orders for %s at table %d\n", cCustomerName, sCurrentTable->iTableId);
	if(sCurrentTable->pOrder != NULL) {
		struct ORDER *sCurrentOrder = sCurrentTable->pOrder;
		int i = 0;
		int iSum = 0;
		bool bFoundOrder = false;
		while(sCurrentOrder != NULL) {
			if(strncmp(sCurrentOrder->cCustomerName, cCustomerName, sizeof(sCurrentOrder) - 1) == 0) {
				i++;
				printf("%d. %-10s - %5d NOK\n", i, sCurrentOrder->cFoodName, sCurrentOrder->iFoodPrice);
				iSum += sCurrentOrder->iFoodPrice;
				bFoundOrder = true;
			}
			sCurrentOrder = sCurrentOrder->pNext;
		}
		if(!bFoundOrder) {
			printf("No orders for %s on this table\n", cCustomerName);
		} else{
			printf("Sum: %d NOK\n", iSum);
		}
	} else{
		printf("No orders for this table\n");
	}
	printf("-------------------\n");
}

//Prints all the tables that are booked
void printTableInfo(struct BOOKING_LIST *sBookingList) {
	struct TABLE *sCurrentTable = sBookingList->pHead;
	
	if(sBookingList->pHead == NULL) {
		printf("\n");
		printf("There are no reservations\n");
		printf("\n");
	} else {
		printf("\n ------BOOKED------\n");
		
		while(sCurrentTable != NULL) {
			printf("/Table number: %-6d \\\n", sCurrentTable->iTableId);
			printf("|Customer: %-10s |\n", sCurrentTable->cCustomerName);
			printf("\\Amount of seats: %-3d /\n", sCurrentTable->iSeats);
			sCurrentTable = sCurrentTable->pNext;
		}
		printf(" ---------------------\n");
	}
}

//Frees all resources allocated throughout the running program
void freeAllResources(struct BOOKING_LIST *sBookingList) {
	struct TABLE *sCurrentTable = sBookingList->pHead;
	struct TABLE *sNextTable;
	
	while(sCurrentTable != NULL) {
		
		struct ORDER *sCurrentOrder = sCurrentTable->pOrder;
		struct ORDER *sNextOrder;
		
		while(sCurrentOrder != NULL) {
			sNextOrder = sCurrentOrder->pNext;
			free(sCurrentOrder);
			sCurrentOrder = sNextOrder;
		}
		sNextTable = sCurrentTable->pNext;
		free(sCurrentTable);
		sCurrentTable = sNextTable;
	}
	free(sBookingList);
}

//Reads a string and converts it to an int
int readInt(char *cInput, int iSize) {
	memset(cInput, 0, iSize);
	
	int iInput = 0;
	int i = 0;
	while((iInput = getchar()) != '\n' && i < iSize - 1) {
		cInput[i++] = (char)iInput;
	}
	
	return (int)strtol(cInput, NULL, 10);
}

//Reads string
int readString(char *cInput, int iSize) {
	memset(cInput, 0, iSize);
	
	int iInput = 0;
	int i = 0;
	while((iInput = getchar()) != '\n' && i < iSize - 1) {
		cInput[i++] = (char)iInput;
	}
	cInput[i] = '\0';
}

//Checks if a string contains int
bool isNum(char *cInput) {
	for(int i = 0; cInput[i] != '\0'; i++) {
		if (cInput[i] < '0' || cInput[i] > '9') {
		return false;
		}
	}
	return true;
}

//Clears the screen
void clear() {
	system("clear");
}
