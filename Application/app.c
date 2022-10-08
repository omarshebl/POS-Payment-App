#include "app.h"
#include <stdio.h>
#include <stdlib.h>

void getCard(ST_cardData_t *cardData) {
	EN_cardError_t cardError = WRONG_NAME;
	while(cardError == WRONG_NAME) cardError = getCardHolderName(cardData);
	
	cardError = WRONG_EXP_DATE;
	while(cardError == WRONG_EXP_DATE) cardError = getCardExpiryDate(cardData);
	
	cardError = WRONG_PAN;
	while(cardError == WRONG_PAN) cardError = getCardPAN(cardData);
	
	printf("\n Card Added Successfully!");
}

void printCard(ST_cardData_t *cardData) {
	printf("#########################\n");
	printf("Card Holder Name: %s\n", cardData->cardHolderName);
	printf("PAN: %s\n", cardData->primaryAccountNumber);
	printf("Card Expiration Date: %s\n", cardData->cardExpirationDate);
	printf("#########################\n");
}

void deleteCard(ST_cardData_t *cardData) {
	for(int i=0; i<strlen((const char *)cardData->primaryAccountNumber); i++) {
		cardData->primaryAccountNumber[i] = '\0';
	}
	
	for(int i=0; i<strlen((const char *)cardData->cardHolderName); i++) {
		cardData->cardHolderName[i] = '\0';
	}
	
	for(int i=0; i<strlen((const char *)cardData->cardExpirationDate); i++) {
		cardData->cardHolderName[i] = '\0';
	}
	
	printf("\n Card Deleted Successfully!");
}

void Card(ST_cardData_t *cardData) {
	int choice = 0;	
	while(1) {
		printf("\n\n=========== Card Menu ===========\n");
		printf("1 - Add Card.\n");
		printf("2 - Display Card.\n");
		printf("3 - Delete Saved Card.\n");
		printf("4 - Back\n");
		while(choice < 1 || choice > 4) {
			printf("\n>>> ");
			scanf(" %d", &choice);
		}
		if (choice == 4) break;
		switch (choice) {
			case 1:
				if (cardData->primaryAccountNumber[0] == '\0') {getCard(cardData);}
				else printf("\nCard already added, please delete it!\n");
				break;
			case 2:
				if (cardData->primaryAccountNumber[0] != '\0') {printCard(cardData);}
				else printf("\nCard not found, please add it!\n");
				break;
			case 3:
				if (cardData->primaryAccountNumber[0] != '\0') {deleteCard(cardData);}
				else printf("\nCard not found, please add it!\n");
				break;
		}
		choice = 0;
	}
}

void getTerminal(ST_terminalData_t *termData) {
	EN_terminalError_t termError = WRONG_DATE;
	while(termError == WRONG_DATE) termError = getTransactionDate(termData);
	
	termError = INVALID_AMOUNT;
	while(termError == INVALID_AMOUNT) termError = getTransactionAmount(termData);
	
	printf("\n Terminal Data Added Successfully!");
}

void printTerminal(ST_terminalData_t *termData) {
	printf("#########################\n");
	printf("Transaction Amount: %.2f\n", termData->transAmount);
	printf("Max Trans Amount: %.2f\n", termData->maxTransAmount);
	printf("Transaction Date: %s\n", termData->transactionDate);
	printf("#########################\n");
}

void deleteTerminal(ST_terminalData_t *termData) {
	termData->transAmount = 0;
	
	for(int i=0; i<strlen((const char *)termData->transactionDate); i++) {
		termData->transactionDate[i] = '\0';
	}
	
	printf("\n Terminal Data Deleted Successfully!");
}

void Terminal(ST_terminalData_t *termData) {
	int choice = 0;	
	while(1) {
		printf("\n\n=========== Terminal Menu ===========\n");
		printf("1 - Add Terminal Data.\n");
		printf("2 - Display Terminal Data.\n");
		printf("3 - Delete Terminal Data.\n");
		printf("4 - Back\n");
		while(choice < 1 || choice > 4) {
			printf("\n>>> ");
			scanf(" %d", &choice);
		}
		if (choice == 4) break;
		switch (choice) {
			case 1:
				if (termData->transAmount <= 0 ) {getTerminal(termData);}
				else printf("\nTerminal data already added, please delete it!\n");
				break;
			case 2:
				if (termData->transAmount > 0 ) {printTerminal(termData);}
				else printf("\nTerminal data not found, please add it!\n");
				break;
			case 3:
				if (termData->transAmount > 0 ) {deleteTerminal(termData);}
				else printf("\nTerminal data not found, please add it!\n");
				break;
		}
		choice = 0;
	}
}

void Process(ST_terminalData_t *termData, ST_cardData_t *cardData, ST_transaction_t *transactionData) {
	printf("Processing Data....\n");
	printCard(cardData);
	printTerminal(termData);
	
	
	if (isCardExpired(cardData, termData) == EXPIRED_CARD) {
		printf("Card is Expired, aborting...\n");
		return;
	}
	
	if (isValidCardPAN(cardData) == INVALID_CARD) {
		printf("Card is invalid, aborting...\n");
		return;
	}
	
	if (isBelowMaxAmount(termData) == EXCEED_MAX_AMOUNT) {
		printf("Amount Exceeding Limit, aborting...\n");
		return;
	}
	
	transactionData->cardHolderData = *cardData;
	transactionData->terminalData = *termData;
	transactionData->transState = recieveTransactionData(transactionData);
	if (saveTransaction(transactionData) != SERVER_OK) printf("Transaction not saved!\n");
}

void appStart(void) {
	
	ST_cardData_t cardData = {.primaryAccountNumber[0] = '\0'};
	ST_terminalData_t termData = {.transAmount = 0};
	ST_transaction_t transaction;
	
	setMaxAmount(&termData, 10000.00);
	
	readAccounts();
	readTrans();
	
	int choice = 0;
	while(1) {
		printf("\n\n=========== Main Menu ===========\n");
		printf("1 - Card.\n");
		printf("2 - Terminal.\n");
		printf("3 - Process Transaction.\n");
		printf("4 - Display Accounts.\n");
		printf("5 - Display Saved Transactions.\n");
		printf("6 - Quit.\n");
		
		while(choice < 1 || choice > 6) {
			printf("\n>>> ");
			scanf(" %d", &choice);
		}
		
		if(choice == 6) {
			writeAccounts();
			writeTrans();
			break;
		}
		
		switch (choice) {
			case 1:
				//printf("\e[1;1H\e[2J");
				Card(&cardData);
				break;
			case 2:
				//printf("\e[1;1H\e[2J");
				Terminal(&termData);
				break;
			case 3:
				//printf("\e[1;1H\e[2J");
				if (cardData.primaryAccountNumber[0] != '\0') {
					if (termData.transAmount > 0) Process(&termData, &cardData, &transaction);
					else printf("Cann process transaction without terminal data!\n");
				} else printf("Cannot process transaction without card!\n");
				break;
			case 4:
				//printf("\e[1;1H\e[2J");
				listAccounts();
				break;
			case 5:
				//printf("\e[1;1H\e[2J");
				listSavedTransactions();
				break;
		}
		choice = 0;
	}
	
}
