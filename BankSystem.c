#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


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

void addAccount(const char *);
void updateAccount(const char *);
void deleteAccount(const char *);
void deleteHolderAccounts(const char *);
void searchAccount(const char*);
int displayAllAccounts(const char *);
void addOperation(const char*);
int checkName(const char *);
int checkEmail(const char *);
int checkAmount(double);

int main(){

    int open = 1;
    const char *fileName = "accounts.bin";

    do{
        printf("******************************\n");
        printf("*   1. Add account           *\n");
        printf("*   2. Update account        *\n");
        printf("*   3. Delete account        *\n");
        printf("*   4. Delete Holder accounts    *\n");
        printf("*   5. Search account        *\n");
        printf("*   6. Display all account   *\n");
        printf("*   7. Add operation         *\n");
        printf("*   8. Exit                  *\n");
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
                deleteAccount(fileName);
                break;

            case 4:
                deleteHolderAccounts(fileName);
                break;

            case 5:
                searchAccount(fileName);
                break;

            case 6:
                displayAllAccounts(fileName);
                break;

            case 7:
                addOperation(fileName);
                break;

            case 8:
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
    struct account acc;
    long accNum;

    printf("Enter holder name: ");
    scanf("%s", acc.holderName);
        if(!checkName(acc.holderName)){
        printf("Error: Invalid name. Please enter alphabetical characters only.\n");
        return;
    }
    printf("Enter holder email: ");
    scanf("%s", acc.holderEmail);
        if(!checkEmail(acc.holderEmail)){
        printf("Error: Invalid Email. Please enter a correct Email format.\n");
        return;
    }
    printf("Enter account number: ");
    while (getchar() != '\n');
    scanf("%ld", &accNum);

    // Check for the existance of the number from the file itself, so that when you close and 
    // open the program it takes into consideration the old accounts that are in there
    struct account tempAcc;
    FILE *tempFile = fopen(fileName, "rb");
    if (tempFile != NULL) {
        while (fread(&tempAcc, sizeof(struct account), 1, tempFile) == 1) {
            if (tempAcc.accNum == accNum) {
                printf("Account number already exists. Please enter a different account number.\n");
                fclose(tempFile);
                return;
            }
        }
        fclose(tempFile);
    }
    acc.accNum = accNum;


    printf("Enter initial balance: ");
    scanf("%lf", &acc.balance);
        if(acc.balance < 0){
            printf("Error: Initial balance must be positive.\n");
            return;
    }

    acc.numOfOper = 0;

    FILE *file;
    file = fopen(fileName, "ab");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    fwrite(&acc, sizeof(struct account), 1, file);
    printf("The data has been saved successfully.\n");

    fclose(file);
}

void updateAccount(const char *fileName){
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    struct account accounts[100];
    int count = 0;

    while (fread(&accounts[count], sizeof(struct account), 1, file) == 1) {
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
                if(!checkName(accounts[accountIndex].holderName)){
                    printf("Error: Invalid name. Please enter alphabetical characters only.\n");
                    break;
                }
                break;

            case 2:
                printf("Enter new account number: ");
                long newAccNum;
                scanf("%ld", &newAccNum);
                int found = 0;
                if(accNumToUpdate == newAccNum){
                    printf("Account number is already %ld.\n", newAccNum);
                    break;
                }
                else{
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


    file = fopen(fileName, "wb");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    // Writing down the whole array at once for convenience
    fwrite(accounts, sizeof(struct account), count, file);

    fclose(file);
}

void deleteAccount(const char *fileName) {
    const char *tempFilename = "temp.bin";
    FILE *file = fopen(fileName, "rb");
    FILE *tempFile = fopen(tempFilename, "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening the file.\n");

        // closing the opened file to ensure there are no memory leaks
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    printf("Enter the account number to delete: ");
    long target;
    int accountFound = 0;

    scanf("%ld", &target);
    
    struct account acc;

    while (fread(&acc, sizeof(struct account), 1, file) == 1) {
        if (acc.accNum == target) {
            accountFound = 1;
            printf("Account with account number %ld has been deleted.\n", acc.accNum);
            continue;
        }
        fwrite(&acc, sizeof(struct account), 1, tempFile);
    }

    if (!accountFound) {
        printf("Account number %ld was not found\n", target);
    }

    fclose(file);
    fclose(tempFile);
    remove(fileName);
    rename(tempFilename, fileName);
}

void deleteHolderAccounts(const char *fileName){
    const char *tempFilename = "temp.bin";
    FILE *file = fopen(fileName, "rb");
    FILE *tempFile = fopen(tempFilename, "wb");

    if (file == NULL || tempFile == NULL) {
        printf("Error opening the file.\n");

        // also closing the opened file to ensure there are no memory leaks
        if (file) fclose(file);
        if (tempFile) fclose(tempFile);
        return;
    }

    printf("Enter the account holder name to delete all associated accounts: ");
    char targetName[50];
    scanf(" %[^\n]s", targetName);

    struct account acc;
    int accountsDeleted = 0;

    while (fread(&acc, sizeof(struct account), 1, file) == 1) {
        if (strcmp(acc.holderName, targetName) == 0) {
            accountsDeleted++;
            printf("Account with account number %ld (Holder: %s) has been deleted.\n", acc.accNum, acc.holderName);
            continue;
        }
        fwrite(&acc, sizeof(struct account), 1, tempFile);
    }

    if (accountsDeleted == 0) {
        printf("No accounts found for the holder '%s'.\n", targetName);
    } else {
        printf("All accounts for holder '%s' have been deleted.\n", targetName);
    }

    fclose(file);
    fclose(tempFile);
    remove(fileName);
    rename(tempFilename, fileName);
}
void searchAccount(const char *fileName) {
    FILE *file = fopen(fileName, "rb");
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter the account number to search: ");
    long targetAccNum;
    scanf("%ld", &targetAccNum);

    struct account acc;
    int accountFound = 0;

    while (fread(&acc, sizeof(struct account), 1, file) == 1) {
        if (acc.accNum == targetAccNum) {
            accountFound = 1;
            printf("Account found:\n");
            printf("-------------------------------------------------\n");
            printf("Name: %s\n", acc.holderName);
            printf("Email: %s\n", acc.holderEmail);
            printf("Account Number: %ld\n", acc.accNum);
            printf("Number of operations: %d\n", acc.numOfOper);
            printf("Balance: %.2lf\n", acc.balance);
            printf("-------------------------------------------------\n");
            break;
        }
    }

    if (!accountFound) {
        printf("Account with account number %ld not found.\n", targetAccNum);
    }

    fclose(file);
}

int displayAllAccounts(const char *fileName) {
    FILE *file;
    struct account acc;
    int accountFound = 0;

    file = fopen(fileName, "rb"); 
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }
// testing
    printf("Displaying all accounts:\n");
    printf("-------------------------------------------------\n");

    while (fread(&acc, sizeof(struct account), 1, file) == 1) {
        accountFound = 1;
        printf("Name: %s\n", acc.holderName);
        printf("Email: %s\n", acc.holderEmail);
        printf("Account Number: %ld\n", acc.accNum);
        printf("Number of operations: %d\n", acc.numOfOper);
        printf("Balance: %.2lf\n", acc.balance);
        printf("Operations:\n");
        for (int i = 0; i < acc.numOfOper; i++) {
            printf("  %d. %c %.2lf\n", i + 1, acc.oper[i].op, acc.oper[i].amount);
        }
        printf("-------------------------------------------------\n");
    }

    if (!accountFound) {
        printf("No accounts found.\n");
        fclose(file);
        return 0;
    }

    fclose(file);
    return 1;
}

void addOperation(const char *fileName) {
    FILE *file = fopen(fileName, "rb+"); // So that we can read and write from the file
    if (file == NULL) {
        printf("No accounts found.\n");
        return;
    }

    printf("Enter the account number to add operation: ");
    long targetAccNum;
    scanf("%ld", &targetAccNum);

    struct account acc;
    int accountFound = 0;
    long pos;

    while ((pos = ftell(file)) >= 0 && fread(&acc, sizeof(struct account), 1, file) == 1) { // ftell returns the current position in the file and if there is an error it will return -1L
        if (acc.accNum == targetAccNum) {
            accountFound = 1;

            
            char opType;
            printf("Enter operation type (D for deposit, W for withdraw): ");
            scanf(" %c", &opType);
            opType = toupper(opType);
            if (opType != 'D' && opType != 'W') {
                printf("Invalid operation type.\n");
                break;
            }

            double opAmount;
            printf("Enter amount: ");
            scanf("%lf", &opAmount);
            if (!checkAmount(opAmount)) {
                printf("Amount must be positive.\n");
                break;
            }

            struct operation newOper;
            newOper.op = opType;
            newOper.amount = opAmount;

            if (opType == 'D') {
                acc.balance += opAmount;
            } 
            else if (opType == 'W') {
                if (opAmount > acc.balance) {
                    printf("Insufficient balance for withdrawal.\n");
                    break;
                }
                acc.balance -= opAmount;
            }

            acc.oper[acc.numOfOper] = newOper;
            acc.numOfOper++;

            // Move file pointer back to overwrite the account, SEEK_SET means start at the start of the file, and then move "pos" times
            fseek(file, pos, SEEK_SET);
            fwrite(&acc, sizeof(struct account), 1, file);

            printf("Operation added successfully.\n");
            printf("New balance: %.2lf\n", acc.balance);

            break;

        }
    }

    if (!accountFound) {
    printf("Account with account number %ld not found.\n", targetAccNum);
    }

    fclose(file);
}

int checkName(const char *name){
    int i;
    for(i=0;name[i]!='\0';i++){
        if(!isalpha(name[i]) && name[i]!=' '){ //isalpha() built in function to check if its a letter or not
            return 0;
        }
    }
    return 1;
}

int checkEmail(const char *email){
    const char *atSign = strchr(email,'@');
    const char *dotSign = strchr(email,'.');
    return (atSign && dotSign  &&  atSign < dotSign);
}

int checkAmount(double amount) {
    return amount > 0;
}
