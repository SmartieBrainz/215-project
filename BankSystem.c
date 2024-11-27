#include <stdio.h>
#include <stdlib.h>


struct operation
{
    char op; // deposit 'd', withdraw 'w'
    double amount;
};

struct account
{
    long accNum;
    double balance;
    char holderName[50];
    char holderEmail[100];
    int numOfOper;
    struct operation oper[100];
};

long accNums[100];
int countOfacc = 0;

void addAccount(const char *);
void updateAccount(const char *);
void deleteHolderAccounts(const char *);
int displayAllAccounts(const char *);

int main(){
    FILE *file;

    int open = 1;
    const char *fileName = "accounts.bin";
    file = fopen(fileName, "wb");

    do{
        printf("******************************\n");
        printf("*   1. Add account           *\n");
        printf("*   2. Update account        *\n");
        printf("*   3. Delete all account    *\n");
        printf("*   4. Display all account   *\n");
        printf("*   5. Exit                  *\n");
        printf("******************************\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch(choice){

            case 1:
                addAccount(fileName);   
                break;

            case 2:
                updateAccount(fileName);
                break;

            case 3:
                deleteHolderAccounts(fileName);
                break;

            case 4:
                displayAllAccounts(fileName);
                break;

            case 5:
                printf("Thanks, and goodbye.");
                open = 0;
                break;

            default:
                printf("Invalid choice\n");
                break;
        }
    }
    while(open);
    return 0;
}


void addAccount(const char *fileName){
    FILE *file;
    file = fopen(fileName, "ab");
    struct account acc;
    long accNum;

    printf("Enter holder name: ");
    scanf("%s", acc.holderName);
    printf("Enter holder email: ");
    scanf("%s", acc.holderEmail);
    printf("Enter account number: ");
    while (getchar() != '\n');
    scanf("%ld", &accNum);
    if(countOfacc == 0){
        accNums[countOfacc] = accNum;
        countOfacc++;
        acc.accNum = accNum;
    }else{
        int i;
        for(i = 0; i < countOfacc; i++){
            if(accNums[i] == accNum){
                printf("Account number already exists. Please enter a different account number.\n");
                return;
            }
        }
        accNums[countOfacc] = accNum;
        countOfacc++;
        acc.accNum = accNum;
    }
    printf("Enter initial balance: ");
    scanf("%lf", &acc.balance);

    acc.numOfOper = 0;

    fprintf(file, "Name: %s\n", acc.holderName);
    fprintf(file, "Email: %s\n", acc.holderEmail);
    fprintf(file, "Account number: %ld\n", acc.accNum);
    fprintf(file, "Number of operations: %d\n", acc.numOfOper);
    fprintf(file, "Balance: %lf\n", acc.balance);

    printf("The data has been saved successfully.\n");

    fclose(file);
}

void updateAccount(const char *fileName){
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    struct account accounts[100];
    int count = 0;

    while (fscanf(file, "Name: %s\nEmail: %s\nAccount number: %ld\nNumber of operations: %d\nBalance: %lf\n-----\n",
                  accounts[count].holderName, accounts[count].holderEmail, &accounts[count].accNum,
                  &accounts[count].numOfOper, &accounts[count].balance) == 5) {
        count++;
    }

    fclose(file); 

    if (!displayAllAccounts(fileName))
        return;

    int open = 1;
    printf("Enter the account number you want to update: ");
    long accNumToUpdate;
    scanf("%ld", &accNumToUpdate);

    int accountIndex = -1;
    for (int i = 0; i < count; i++) {
        if (accounts[i].accNum == accNumToUpdate) {
            accountIndex = i;
            break;
        }
    }

    if (accountIndex == -1) {
        printf("Account not found.\n");
        return;
    }

    do {
        printf("******************************\n");
        printf("*   1. Update holder name    *\n");
        printf("*   2. Update account number *\n");
        printf("*   3. Exit                  *\n");
        printf("******************************\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        switch(choice) {
            case 1:
                printf("Enter new holder name: ");
                scanf("%s", accounts[accountIndex].holderName);
                break;

            case 2:
                printf("Enter new account number: ");
                long newAccNum;
                scanf("%ld", &newAccNum);
                int found = 0;
                if(accNumToUpdate == newAccNum){
                    printf("Account number is already %ld.\n", newAccNum);
                    break;
                }else{
                for (int i = 0; i < count; i++) {
                    if (accounts[i].accNum == newAccNum) {
                        printf("Account number already exists. Please enter a different account number.\n");
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    accounts[accountIndex].accNum = newAccNum;
                }
                }
                break;

            case 3:
                open = 0;
                break;

            default:
                printf("Invalid choice.\n");           
                break;
        }
    } while (open);


    file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Name: %s\n", accounts[i].holderName);
        fprintf(file, "Email: %s\n", accounts[i].holderEmail);
        fprintf(file, "Account number: %ld\n", accounts[i].accNum);
        fprintf(file, "Number of operations: %d\n", accounts[i].numOfOper);
        fprintf(file, "Balance: %lf\n", accounts[i].balance);
        fprintf(file, "-----\n");
    }

    fclose(file);
}

void deleteHolderAccounts(const char *fileName){
    FILE *file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return;
    }

    fclose(file);
    printf("All accounts are deleted.\n");
}

int displayAllAccounts(const char *fileName) {
    FILE *file;
    struct account acc;
    int accountFound = 0;

    file = fopen(fileName, "r"); 
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }
// testing
    printf("Displaying all accounts:\n");
    printf("-------------------------------------------------\n");

    while (fscanf(file, "Name: %s\nEmail: %s\nAccount number: %ld\nNumber of operations: %d\nBalance: %lf\n-----\n",
                  acc.holderName, acc.holderEmail, &acc.accNum, &acc.numOfOper, &acc.balance) == 5) {
        accountFound = 1;
        printf("Name: %s\n", acc.holderName);
        printf("Email: %s\n", acc.holderEmail);
        printf("Account Number: %ld\n", acc.accNum);
        printf("Number of operations: %d\n", acc.numOfOper);
        printf("Balance: %.2lf\n", acc.balance);
        printf("-------------------------------------------------\n");
    }

    if (!accountFound) {
        printf("No accounts found.\n");
        return 0;
    }

    fclose(file);
    return 1;
}
// testing
