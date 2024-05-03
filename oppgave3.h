#ifndef __OPPGAVE2_H__
#define __OPPGAVE2_H__

#include <stdbool.h>

#define BUFFER_SIZE 1014
#define MAX_INT_LEN 10
#define MAX_STR_LEN 20

struct TABLE {
	int iTableId;
	char cCustomerName[MAX_STR_LEN];
	int iSeats;
	int iTime;
	struct ORDER *pOrder;
	struct TABLE *pNext;
	struct TABLE *pPrev;
};

struct BOOKING_LIST {
	struct TABLE *pHead;
	struct TABLE *pTail;
};

struct ORDER {
	char cCustomerName[MAX_STR_LEN];
	char cFoodName[MAX_STR_LEN];
	int iFoodPrice;
	struct ORDER *pNext;
};

struct TABLE *getTableInfoById(struct BOOKING_LIST *sBookingList, char *cInput);
struct TABLE *getTableInfoByName(struct BOOKING_LIST *sBookingList, char *cInput);

bool deleteByTableNumber(struct BOOKING_LIST *sBookingList, char *cInput);
bool addOrderToTable(struct TABLE *sTable, char *cCustomerName, char *cFoodName, int iFoodPrice);

void addTable(struct BOOKING_LIST *sBookingList, int iTableId, char *customerName, int iSeats, int iTime);
void printBooking(const struct TABLE *sTable);

#endif
