#include "terminal.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

EN_terminalError_t getTransactionDate(ST_terminalData_t *termData) {
	uint8_t *transactionDate = (uint8_t *) malloc(20);
	if (transactionDate == NULL) return WRONG_DATE;
	
	printf("\nPlease enter transaction date (DD/MM/YYYY)");
	printf("\n>> ");
	
	scanf(" %[^\n]s", transactionDate);
	
	int dateL = strlen((const char *) transactionDate);
	if (dateL != 10) return WRONG_DATE;
	
	for (int i=0; i<10; i++) {
		char check = *(transactionDate+i);
		
		if(i != 2 && i != 5) {
			if (isdigit(check) == 0) return WRONG_DATE;
		} else {
			if (check != '/') return WRONG_DATE;
		}
	}
	
	int tD, tM;
	
	tD = convertDateToInt(transactionDate, 0, 1);
	tM = convertDateToInt(transactionDate, 3, 4);

	if (tD < 1 || tD > 31 || tM < 1 || tM > 12) return WRONG_DATE;
	
	for (int i=0; i<=10; i++) {
		termData->transactionDate[i] = *(transactionDate+i);
	}
	
	free(transactionDate);
	
	return TERMINAL_OK;
}

EN_terminalError_t isCardExpired(ST_cardData_t *cardData, ST_terminalData_t *termData) {
	
	int cM, cY;
	int tM, tY;
	
	cM = convertDateToInt(cardData->cardExpirationDate, 0, 1);
	cY = convertDateToInt(cardData->cardExpirationDate, 3, 4) + 2000;
	
	tM = convertDateToInt(termData->transactionDate, 3, 4);
	tY = convertDateToInt(termData->transactionDate, 6, 9);
	
	if (cY < tY) return EXPIRED_CARD;
	else if (cY == tY) if (cM < tM) return EXPIRED_CARD;
	
	
	return TERMINAL_OK;
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t *termData) {
	float transAmount = 0;
	
	printf("\nPlease enter transaction amount (>0)");
	printf("\n>> ");
	
	scanf(" %f", &transAmount);
	
	if (transAmount <= 0) return INVALID_AMOUNT;
	
	termData->transAmount = transAmount;
	
	return TERMINAL_OK;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t *termData) {
	if (termData->transAmount > termData->maxTransAmount) return EXCEED_MAX_AMOUNT;
		
	return TERMINAL_OK;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t *termData, float maxAmount) {
//	float maxTransAmount = 0;
//	
//	printf("\nPlease set max transaction amount (>0)");
//	printf("\n>> ");
//	
//	scanf(" %f", &maxTransAmount);
	
	if (maxAmount <= 0) return INVALID_MAX_AMOUNT;
	
	termData->maxTransAmount = maxAmount;
	
	return TERMINAL_OK;
}

EN_terminalError_t isValidCardPAN(ST_cardData_t *cardData){	
	int PANlen = strlen((const char*) cardData->primaryAccountNumber);
	int sum = 0, isSecond = 0, x = 0;
	
	for (int i = PANlen-1; i >= 0; i--) {
		x = ((int)cardData->primaryAccountNumber[i]-48);
		if(isSecond == 1) x *= 2;
		isSecond = (isSecond==0)?1:0;
		sum += (x/10);
		sum += (x%10);
	}
	
	if (sum%10 == 0) return TERMINAL_OK;
	
	return INVALID_CARD;
} // Bonus

