#ifndef APP_H
#define APP_H

#include "dbg.h"
#include "../Card/card.h"
#include "../Server/server.h"
#include "../Terminal/terminal.h"


void getCard(ST_cardData_t *cardData);
void printCard(ST_cardData_t *cardData);
void deleteCard(ST_cardData_t *cardData);
void Card(ST_cardData_t *cardData);
void getTerminal(ST_terminalData_t *termData);
void printTerminal(ST_terminalData_t *termData);
void deleteTerminal(ST_terminalData_t *termData);
void Terminal(ST_terminalData_t *termData);
void Process(ST_terminalData_t *termData, ST_cardData_t *cardData, ST_transaction_t *transactionData);
void appStart(void);

#endif