#include "card.h"

int convertDateToInt(uint8_t str[], int s, int e) {
    char *p;
    p = malloc(e-s+2);
    
    if (p == NULL) {
        printf("Unable to allocate memory.\n");
        exit(1);
    }
    
    for (int i = 0; i < (e-s+1); i++) {
        *(p+i) = str[s+i];
    }
    
    *(p+(e-s+1)) = '\0';
    
    int x = atoi(p);
    free(p);
    
    return x;
}

EN_cardError_t getCardHolderName(ST_cardData_t *cardData){
    
    uint8_t *cardHolderName = (uint8_t *) malloc(100);
    if (cardHolderName == NULL) return WRONG_NAME;

    printf("\nPlease enter card holder name (MIN:20, MAX:24)");
    printf("\n>> ");

    scanf(" %[^\n]s", cardHolderName);

    int nameL = strlen((const char *) cardHolderName);
    if (nameL < 20 || nameL > 24) return WRONG_NAME;
    
    for (int i=0; i<=25; i++) {
        cardData->cardHolderName[i] = *(cardHolderName+i);
    }
    
    free(cardHolderName);

    return CARD_OK;
}

void getCardHolderNameTest(void) {
    printf("Tester Name: Omar Shebl\n");
    printf("getCardHolderName\n");
    

}

EN_cardError_t getCardExpiryDate(ST_cardData_t *cardData){
    
    uint8_t *cardExpiryDate= (uint8_t *) malloc(10);
    if (cardExpiryDate == NULL) return WRONG_EXP_DATE;
    
    printf("\nPlease enter card expiry date (MM/YY)");
    printf("\n>> ");
    
    scanf(" %[^\n]s", cardExpiryDate);
    
    int cardEDL = strlen((const char *) cardExpiryDate);
    if (cardEDL != 5) return WRONG_EXP_DATE;
        
    for (int i=0; i<5; i++) {
        char check = *(cardExpiryDate+i);
        
        if(i != 2) {
            if (isdigit(check) == 0) return WRONG_EXP_DATE;
        } else {
            if (check != '/') return WRONG_EXP_DATE;
        }
    }
    
    int cM;
    cM = convertDateToInt(cardExpiryDate, 0, 1);
    if (cM < 1 || cM > 12) return WRONG_EXP_DATE;

    for (int i=0; i<=5; i++) {
        cardData->cardExpirationDate[i] = *(cardExpiryDate+i);
    }
    
    free(cardExpiryDate);
    
    return CARD_OK;
}

EN_cardError_t getCardPAN(ST_cardData_t *cardData){
    
    uint8_t *cardPAN= (uint8_t *) malloc(50);
    if (cardPAN == NULL) return WRONG_PAN;
    
    printf("\nPlease enter card PAN (MIN:16, MAX:19)");
    printf("\n>> ");
    
    scanf(" %[^\n]s", cardPAN);
    
    int cardPANL = strlen((const char *) cardPAN);
    if (cardPANL < 16 || cardPANL > 19) return WRONG_PAN;
    
    for (int i=0; i<=19; i++) {
        cardData->primaryAccountNumber[i] = *(cardPAN+i);
    }
    
    free(cardPAN);
    
    return CARD_OK;
}

    