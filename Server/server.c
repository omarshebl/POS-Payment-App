#include "server.h"
#include <stdio.h>

FILE *file;
struct ST_accountsDB_t accounts[MAX_ELEMENTS] = {0};
struct ST_transaction_t transactions[MAX_ELEMENTS] = {0};
uint32_t accountsNum;
uint32_t transNum;

EN_transState_t recieveTransactionData(ST_transaction_t *transData) {
	ST_accountsDB_t *accountReference;
	
	if(isValidAccount(&transData->cardHolderData, &accountReference) == ACCOUNT_NOT_FOUND) return FRAUD_CARD;
	if(isBlockedAccount(accountReference) == BLOCKED_ACCOUNT) return DECLINED_STOLEN_CARD;
	if(isAmountAvailable(&transData->terminalData, accountReference) == LOW_BALANCE) return DECLINED_INSUFFECIENT_FUND;

	accountReference->balance -= transData->terminalData.transAmount;
	
	return APPROVED;
}

EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t **accountReference) {
	for (int i = 0; i < MAX_ELEMENTS-1; i++) {		
		if (atoi((const char *) accounts[i].primaryAccountNumber) == atoi((const char *) cardData->primaryAccountNumber)) {
			*accountReference = &accounts[i];
			return SERVER_OK;
		}
	}
	return ACCOUNT_NOT_FOUND;
}


EN_serverError_t isAmountAvailable(ST_terminalData_t *termData, ST_accountsDB_t *accountReference) {
	if (termData->transAmount > accountReference->balance) return LOW_BALANCE;
	return SERVER_OK;
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountReference) {
	if (accountReference->state == BLOCKED) return BLOCKED_ACCOUNT;
	return SERVER_OK;
}

EN_serverError_t saveTransaction(ST_transaction_t *transData) {
	transData->transactionSequenceNumber = transNum;
	transactions[(int)transNum] = *transData;
	transNum++;
	
	listSavedTransactions();

	return SERVER_OK;
}

char* returnTransStateString(EN_transState_t state) {
	char *transStateString[] = {"APPROVED", "DECLINED_INSUFFECIENT_FUND", "DECLINED_STOLEN_CARD", "FRAUD_CARD", "INTERNAL_SERVER_ERROR"};
	return transStateString[state];
}

char* returnAccountStateString(EN_accountState_t state) {
	char *accountStateString[] = {"RUNNING", "BLOCKED"};
	return accountStateString[state];
}

void listSavedTransactions(void) {
	for (int i=0; i<transNum; i++) {
		printf("#########################\n");
		printf("Transaction Sequence Number: %d\n", transactions[i].transactionSequenceNumber);
		printf("Transaction Date: %s\n", transactions[i].terminalData.transactionDate);
		printf("Transaction Amount: %.2f\n", transactions[i].terminalData.transAmount);
		printf("Transaction State: %s\n", returnTransStateString(transactions[i].transState));
		printf("Transaction Max Amount: %.2f\n", transactions[i].terminalData.maxTransAmount);
		printf("Cardholder Name: %s\n", transactions[i].cardHolderData.cardHolderName);
		printf("PAN: %s\n", transactions[i].cardHolderData.primaryAccountNumber);
		printf("Card Expiration Date: %s\n", transactions[i].cardHolderData.cardExpirationDate);
		printf("#########################\n");
	}
}

void listAccounts(void) {
	for (int i=0; i<accountsNum; i++) {
		printf("#########################\n");
		printf("Account Balance: %.2f\n", accounts[i].balance);
		printf("Account State: %s\n", returnAccountStateString(accounts[i].state));
		printf("Primary Account Number: %s\n", accounts[i].primaryAccountNumber);
		printf("#########################\n");
	}
}

void readTrans(void) {
	struct ST_transaction_t input;
	
	file = fopen("Server/data_transactions.dat", "r");
	if (file == NULL) {
		fprintf(stderr, "\nCouldn't open file\n");
		exit (1);
	}
	
	while(fread(&input, sizeof(struct ST_transaction_t), 1, file)) {
		transactions[transNum++] = input;
	}
	
	fclose(file);
}

void writeTrans(void) {	
	file = fopen ("Server/data_transactions.dat", "w");
	if (file == NULL) {
		fprintf(stderr, "\nCouldn't open file\n");
		exit (1);
	}
	
	for(int i=0; i < transNum; i++) {
		fwrite (&transactions[i], sizeof(struct ST_transaction_t), 1, file);
	}
	
	if(fwrite != 0) printf("Transaction File Written Successfully!\n");
	else printf("File NOT Written!\n");

	fclose (file);
}

void readAccounts(void) {
	struct ST_accountsDB_t input;
	
	file = fopen("Server/data_accounts.dat", "r");
	if (file == NULL) {
		fprintf(stderr, "\nCouldn't open file\n");
		exit (1);
	}
	
	while(fread(&input, sizeof(struct ST_accountsDB_t), 1, file)) {
		accounts[accountsNum++] = input;
	}
	
	fclose(file);
}

void writeAccounts(void) {	
	file = fopen ("Server/data_accounts.dat", "w");
	if (file == NULL) {
		fprintf(stderr, "\nCouldn't open file\n");
		exit (1);
	}
	
	for(int i=0; i < accountsNum; i++) {
		fwrite (&accounts[i], sizeof(struct ST_accountsDB_t), 1, file);
	}
	
	if(fwrite != 0) printf("Accounts File Written Successfully!\n");
	else printf("File NOT Written!\n");
	
	fclose (file);
}
