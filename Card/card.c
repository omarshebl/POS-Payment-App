#include "card.h"
#include <stdlib.h>
#include <ctype.h>

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
    
    uint8_t *cardHolderName = (uint8_t *) malloc(100);
    if (cardHolderName == NULL) return WRONG_NAME;

    printf("\nPlease enter card holder name (MIN:20, MAX:24)");
    printf("\n>> ");

    scanf("%[^\n]s", cardHolderName);

    int nameL = strlen((const char *) cardHolderName);
    if (nameL < 20 || nameL > 24) return WRONG_NAME;

    cardHolderName = (uint8_t *) realloc(cardHolderName, 25);
    if (cardHolderName == NULL) return WRONG_NAME;
    
    for (int i=0; i<=25; i++) {
        cardData->cardHolderName[i] = *(cardHolderName+i);
    }
    
    free(cardHolderName);

    return OK;
}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
    
    uint8_t *cardExpiryDate= (uint8_t *) malloc(10);
    if (cardExpiryDate == NULL) return WRONG_EXP_DATE;
    
    printf("\nPlease enter card expiry date (MM/YY)");
    printf("\n>> ");
    
    scanf("%[^\n]s", cardExpiryDate);
    
    int cardEDL = strlen((const char *) cardExpiryDate);
    if (cardEDL != 5) return WRONG_EXP_DATE;
    
    cardExpiryDate = (uint8_t *) realloc(cardExpiryDate, 6);
    if (cardExpiryDate == NULL) return WRONG_EXP_DATE;
    
    if (*cardExpiryDate != '0' && *cardExpiryDate != '1') return WRONG_EXP_DATE;
    
    for (int i=1; i<5; i++) {
        char check = *(cardExpiryDate+i);
        
        if(i != 2) {
            if (isdigit(check) == 0) return WRONG_EXP_DATE;
        } else {
            if (check != '/') return WRONG_EXP_DATE;
        }
    }
    
    for (int i=0; i<=5; i++) {
        cardData->cardExpirationDate[i] = *(cardExpiryDate+i);
    }
    
    free(cardExpiryDate);
    
    return OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){
    
    uint8_t *cardPAN= (uint8_t *) malloc(50);
    if (cardPAN == NULL) return WRONG_PAN;
    
    printf("\nPlease enter card PAN (MIN:16, MAX:19)");
    printf("\n>> ");
    
    scanf("%[^\n]s", cardPAN);
    
    int cardPANL = strlen((const char *) cardPAN);
    if (cardPANL < 16 || cardPANL > 19) return WRONG_PAN;
    
    cardPAN = (uint8_t *) realloc(cardPAN, 20);
    if (cardPAN == NULL) return WRONG_PAN;
    
    for (int i=0; i<=19; i++) {
        cardData->cardExpirationDate[i] = *(cardPAN+i);
    }
    
    free(cardPAN);
    
    return OK;
}

    